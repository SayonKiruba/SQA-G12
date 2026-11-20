"""
transaction_manager.py - applies transactions to the data held in memory.

GAMES PART ONLY for now: loading the game data, finding games, ownership checks,
selling and buying. Teammates add the user methods (createUser, deleteUser,
refund, addCredit, findUser, processTransaction ...) to this same class.

A "transaction" passed to sellGame/buyGame is any object with the attributes
game_name, seller, buyer and price (the parsed transaction record).
Rule violations are printed through ErrorLogger and the method returns False.
"""

from error_logger import ErrorLogger
from models import GameOwnership, Game


class TransactionManager:
    """Holds users, game listings and ownership records and applies transactions to them."""

    def __init__(self):
        """Start with no users, no games for sale and no owned games."""
        self.users = []          # list of User (managed by the user methods)
        self.games = []          # list of Game for sale
        self.collections = []    # list of GameOwnership

    def loadData(self, users, games, collections):
        """Store the loaded accounts, game listings and ownership records for processing."""
        self.users = users
        self.games = games
        self.collections = collections

    # ---------- queries ----------

    def findGame(self, gameName, seller):
        """Return the listing with this game name and seller, or None."""
        for game in self.games:
            if game.name == gameName and game.seller == seller:
                return game
        return None

    def ownsGame(self, username, gameName):
        """Return True if this user already owns this game."""
        return any(ownership.owner == username and ownership.game_name == gameName
                   for ownership in self.collections)

    # ---------- transactions ----------

    def sellGame(self, transaction):
        """List a game for sale; returns False (and logs) if this seller already lists it."""
        # TODO(Phase 5): check the seller's account type allows selling (User.hasPermission).
        if self.findGame(transaction.game_name, transaction.seller):
            ErrorLogger.logTransactionError(
                "Sell", "seller already has this game for sale", transaction)
            return False
        self.games.append(Game(transaction.game_name, transaction.seller, transaction.price))
        return True

    def buyGame(self, transaction):
        """
        Move a game from the for-sale list into the buyer's collection.
        Returns False (and logs) if the listing does not exist or the buyer owns the game.
        """
        # TODO(Phase 5): move the price from buyer to seller with User.adjustCredit,
        # and check account types. Not done here because those are the user methods.
        listing = self.findGame(transaction.game_name, transaction.seller)
        if listing is None:
            ErrorLogger.logTransactionError("Buy", "game is not for sale", transaction)
            return False
        if self.ownsGame(transaction.buyer, transaction.game_name):
            ErrorLogger.logTransactionError("Buy", "buyer already owns this game", transaction)
            return False
        self.games.remove(listing)
        self.collections.append(GameOwnership(transaction.game_name, transaction.buyer))
        return True

    def removeUserGames(self, username):
        """Cancel a deleted user's games for sale and drop the games they owned (deleteUser calls this)."""
        self.games = [game for game in self.games if game.seller != username]
        self.collections = [o for o in self.collections if o.owner != username]