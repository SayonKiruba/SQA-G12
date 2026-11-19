"""
game_store.py - keeps the Available Games and Games Collection data and updates it.

Responsibility of this class: the two game-related master files and the changes
transactions make to them. It does NOT touch user accounts or credit balances,
and it reports problems by returning False / raising errors instead of logging,
so whoever calls it decides how to log them.
"""

from models import Game, GameOwnership
from record_format import (split_from_right, END_SENTINEL, USERNAME_WIDTH,
                           GAME_NAME_WIDTH, PRICE_WIDTH)
from decimal import Decimal


class GameStore:
    """Holds the games for sale and the game collections, and reads/writes their files."""

    def __init__(self):
        """Start with no games for sale and no owned games."""
        self.available_games = []   # list of Game
        self.collection = []        # list of GameOwnership

    # ---------- loading and saving ----------

    def load_available_games(self, path):
        """Read the old Available Games File. Raises OSError/ValueError on a bad file."""
        for line in self._record_lines(path):
            name, seller, price = split_from_right(line, 2)
            self.available_games.append(Game(name, seller, Decimal(price)))

    def load_collection(self, path):
        """Read the old Games Collection File. Raises OSError/ValueError on a bad file."""
        for line in self._record_lines(path):
            game_name, owner = split_from_right(line, 1)
            self.collection.append(GameOwnership(game_name, owner))

    def save_available_games(self, path):
        """Write the New Available Games File (49-character lines, then END)."""
        self._write_lines(path, [
            f"{g.name:<{GAME_NAME_WIDTH}} {g.seller:<{USERNAME_WIDTH}} "
            f"{g.price:0{PRICE_WIDTH}.2f}" for g in self.available_games])

    def save_collection(self, path):
        """Write the New Games Collection File (42-character lines, then END)."""
        self._write_lines(path, [
            f"{o.game_name:<{GAME_NAME_WIDTH}} {o.owner:<{USERNAME_WIDTH}}"
            for o in self.collection])

    # ---------- queries ----------

    def find_game(self, game_name):
        """Return the Game for sale with this name, or None."""
        for game in self.available_games:
            if game.name == game_name:
                return game
        return None

    def owns_game(self, username, game_name):
        """Return True if this user already owns this game."""
        return any(o.owner == username and o.game_name == game_name
                   for o in self.collection)

    # ---------- updates ----------

    def add_game_for_sale(self, game_name, seller, price):
        """List a game for sale. Returns False (no change) if that name is already for sale."""
        if self.find_game(game_name):
            return False
        self.available_games.append(Game(game_name, seller, price))
        return True

    def sell_to_buyer(self, game_name, buyer):
        """
        Complete a purchase: take the game off sale and add it to the buyer's collection.
        Returns False (no change) if the game is not for sale or the buyer already owns it.
        """
        listing = self.find_game(game_name)
        if listing is None or self.owns_game(buyer, game_name):
            return False
        self.available_games.remove(listing)
        self.collection.append(GameOwnership(game_name, buyer))
        return True

    def remove_user_games(self, username):
        """Cancel a deleted user's games for sale and drop the games they owned."""
        self.available_games = [g for g in self.available_games if g.seller != username]
        self.collection = [o for o in self.collection if o.owner != username]

    # ---------- helpers ----------

    def _record_lines(self, path):
        """Return the non-empty lines of a master file up to (not including) END."""
        lines = []
        with open(path, encoding="utf-8") as master_file:
            for line in master_file:
                line = line.rstrip("\r\n")
                if line.strip() == END_SENTINEL:
                    break
                if line.strip():
                    lines.append(line)
        return lines

    def _write_lines(self, path, lines):
        """Write the lines followed by a bare END record (what the Front End expects)."""
        with open(path, "w", encoding="utf-8") as master_file:
            for line in lines:
                master_file.write(line + "\n")
            master_file.write(END_SENTINEL + "\n")
