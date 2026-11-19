"""
models.py - plain data holders for the things the Back End keeps track of.
"""

from dataclasses import dataclass
from decimal import Decimal


@dataclass
class User:
    """One account from the Current User Accounts File."""
    username: str      # at most 15 characters
    user_type: str     # AA=admin, FS=full-standard, BS=buy-standard, SS=sell-standard
    credit: Decimal    # available credit in dollars (never negative)


@dataclass
class Game:
    """One game listed in the Available Games File (a game for sale)."""
    name: str          # at most 25 characters, unique among games for sale
    seller: str        # username of the seller
    price: Decimal     # price in dollars, at most 999.99


@dataclass
class GameOwnership:
    """One line of the Games Collection File: this user owns this game."""
    game_name: str
    owner: str         # username of the owner
