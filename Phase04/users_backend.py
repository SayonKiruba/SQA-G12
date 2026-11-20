"""
users_backend.py - keeps the Current User Accounts data and updates it.

Responsibility of this class: the user account master file and account changes
made by transactions. It reports problems by returning False / raising errors
instead of logging, so whoever calls it decides how to log them.
"""

from decimal import Decimal

from models import User
from record_format import (CREDIT_WIDTH, END_SENTINEL, USERNAME_WIDTH,
                           USER_TYPE_WIDTH, split_from_right)


class UserStore:
    """Holds user accounts and reads/writes their master file."""

    MAX_CREDIT = Decimal("999999.99")

    def __init__(self):
        """Start with no user accounts."""
        self.users = []  # list of User

    # ---------- loading and saving ----------

    def load_users(self, path):
        """Read the Current User Accounts File. Raises OSError/ValueError on a bad file."""
        for line in self._record_lines(path):
            username, user_type, credit = split_from_right(line, 2)
            self.users.append(User(username, user_type, Decimal(credit)))

    def save_users(self, path):
        """Write the Current User Accounts File, followed by END."""
        self._write_lines(path, [
            f"{user.username:<{USERNAME_WIDTH}} {user.user_type:<{USER_TYPE_WIDTH}} "
            f"{user.credit:0{CREDIT_WIDTH}.2f}" for user in self.users])

    # ---------- queries ----------

    def find_user(self, username):
        """Return the account with this username, or None."""
        for user in self.users:
            if user.username == username:
                return user
        return None

    def search_users(self, search_term):
        """Return accounts whose usernames contain the term, ignoring case."""
        normalized_term = search_term.lower()
        return [user for user in self.users
                if normalized_term in user.username.lower()]

    # ---------- updates ----------

    def add_user(self, username, user_type, credit=Decimal("0.00")):
        """Add an account. Returns False (no change) if that username already exists."""
        if self.find_user(username):
            return False
        self.users.append(User(username, user_type, credit))
        return True

    def remove_user(self, username):
        """Remove an account. Returns False if that username does not exist."""
        user = self.find_user(username)
        if user is None:
            return False
        self.users.remove(user)
        return True

    def adjust_credit(self, username, amount):
        """Adjust an account's credit within 0.00..999999.99; return False if invalid."""
        user = self.find_user(username)
        if user is None:
            return False
        new_credit = user.credit + amount
        if new_credit < 0 or new_credit > self.MAX_CREDIT:
            return False
        user.credit = new_credit
        return True

    # ---------- helpers ----------

    def _record_lines(self, path):
        """Return non-empty master-file lines up to (not including) END."""
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
        """Write the records followed by a bare END record."""
        with open(path, "w", encoding="utf-8") as master_file:
            for line in lines:
                master_file.write(line + "\n")
            master_file.write(END_SENTINEL + "\n")