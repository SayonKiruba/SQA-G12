"""
file_manager.py - reads and writes the Back End's text files.

GAMES PART ONLY for now: the Available Games File and the Games Collection File.
Teammates add readUsers/writeUsers and the transaction-reading methods to this
same class (they can reuse the two private helpers at the bottom).

Problems are reported by raising OSError/ValueError; the caller (BackEnd)
decides how to log them with ErrorLogger.
"""

from decimal import Decimal

from models import Game, GameOwnership
from record_format import (split_from_right, END_SENTINEL, USERNAME_WIDTH,
                           GAME_NAME_WIDTH, PRICE_WIDTH)


class FileManager:
    """Reads the old master files into objects and writes the new master files."""

    # ---------- games: reading ----------

    def readGames(self, path):
        """Read the old Available Games File into a list of Game (END record excluded)."""
        games = []
        for line in self._readRecordLines(path):
            name, seller, price = split_from_right(line, 2)
            games.append(Game(name, seller, Decimal(price)))
        return games

    def readCollections(self, path):
        """Read the old Games Collection File into a list of GameOwnership (END excluded)."""
        collections = []
        for line in self._readRecordLines(path):
            gameName, owner = split_from_right(line, 1)
            collections.append(GameOwnership(gameName, owner))
        return collections

    # ---------- games: writing ----------

    def writeGames(self, path, games):
        """Write the New Available Games File: 49-character records, then END."""
        self._writeRecordLines(path, [
            f"{game.name:<{GAME_NAME_WIDTH}} {game.seller:<{USERNAME_WIDTH}} "
            f"{game.price:0{PRICE_WIDTH}.2f}" for game in games])

    def writeCollections(self, path, collections):
        """Write the New Games Collection File: 42-character records, then END."""
        self._writeRecordLines(path, [
            f"{ownership.game_name:<{GAME_NAME_WIDTH}} {ownership.owner:<{USERNAME_WIDTH}}"
            for ownership in collections])

    # ---------- shared helpers (teammates' file methods can reuse these) ----------

    def _readRecordLines(self, path):
        """Return the non-empty lines of a file up to (not including) the END record."""
        lines = []
        with open(path, encoding="utf-8") as inputFile:
            for line in inputFile:
                line = line.rstrip("\r\n")
                if line.strip() == END_SENTINEL:
                    break
                if line.strip():
                    lines.append(line)
        return lines

    def _writeRecordLines(self, path, lines):
        """Write the lines followed by a bare END record (what the Front End expects)."""
        with open(path, "w", encoding="utf-8") as outputFile:
            for line in lines:
                outputFile.write(line + "\n")
            outputFile.write(END_SENTINEL + "\n")