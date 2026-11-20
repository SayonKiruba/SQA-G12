"""
main.py - Back End entry point (Phase 4 rapid prototype).

Intention:
    Run once at the end of a day. Reads the old master files plus the daily
    transaction file produced by the Front End, applies the transactions, and
    writes the new master files for the next day.

Input files (old master files and the daily transaction file):
    current-users.txt     - Current User Accounts File
    available-games.txt   - Available Games File
    game-collection.txt   - Games Collection File
    daily-transactions.txt - Daily Transaction File (one record per line)

Output files (new master files, same layout as the input files):
    new-current-users.txt, new-available-games.txt, new-game-collection.txt

How to run (from the Phase04 folder):
    python main.py [users_in games_in collection_in transactions \
                    users_out games_out collection_out]
    With no arguments the default file names above are used.

Prototype status: create, delete and add-credit are applied; sell, buy and
refund are recognised but not applied yet (planned for Phase 5).
"""

import sys
from decimal import Decimal

from file_manager import FileManager
from record_format import split_from_right
from transaction_backend import TransactionStore
from transaction_manager import TransactionManager
from users_backend import UserStore

# Transaction codes used at the start of every transaction record.
CODE_END_OF_SESSION = "00"
CODE_CREATE = "01"
CODE_DELETE = "02"
CODE_SELL = "03"
CODE_BUY = "04"
CODE_REFUND = "05"
CODE_ADD_CREDIT = "06"

DEFAULT_FILES = ["current-users.txt", "available-games.txt", "game-collection.txt",
                 "daily-transactions.txt", "new-current-users.txt",
                 "new-available-games.txt", "new-game-collection.txt"]


class BackEnd:
    """Coordinates the three stores: loads files, applies transactions, saves files."""

    def __init__(self):
        """Create the user store, file manager, transaction manager and transaction store."""
        self.user_store = UserStore()
        self.file_manager = FileManager()
        self.transaction_manager = TransactionManager()
        self.transaction_store = TransactionStore()

    def load_all(self, users_path, games_path, collection_path, transactions_path):
        """Read the three old master files and the daily transaction file."""
        self.user_store.load_users(users_path)
        self.transaction_manager.loadData(
            self.user_store.users,
            self.file_manager.readGames(games_path),
            self.file_manager.readCollections(collection_path))
        self.transaction_store.load_transactions(transactions_path)

    def apply_all_transactions(self):
        """Apply every loaded transaction record, in order, printing any problem found."""
        for record in self.transaction_store.transactions:
            try:
                self.apply_transaction(record)
            except ValueError as error:
                print(f"Skipped bad record '{record}': {error}")

    def apply_transaction(self, record):
        """Apply one record: the first two characters are the transaction code."""
        code, details = record[:2], record[3:]
        if code == CODE_END_OF_SESSION:
            return  # marks a session boundary; nothing to change
        if code == CODE_CREATE:
            self._apply_create(details)
        elif code == CODE_DELETE:
            self._apply_delete(details)
        elif code == CODE_ADD_CREDIT:
            self._apply_add_credit(details)
        elif code in (CODE_SELL, CODE_BUY, CODE_REFUND):
            print(f"Not implemented yet (Phase 5): transaction code {code}")
        else:
            raise ValueError(f"unknown transaction code '{code}'")

    def save_all(self, users_path, games_path, collection_path):
        """Write the three new master files."""
        self.user_store.save_users(users_path)
        self.file_manager.writeGames(games_path, self.transaction_manager.games)
        self.file_manager.writeCollections(collection_path, self.transaction_manager.collections)

    # ---------- one method per implemented transaction ----------

    def _apply_create(self, details):
        """Create: details are 'username type credit'."""
        username, user_type, credit = split_from_right(details, 2)
        if not self.user_store.add_user(username, user_type, Decimal(credit)):
            print(f"Create failed: '{username}' already exists")

    def _apply_delete(self, details):
        """Delete: details are 'username type credit'; also removes their games."""
        username, _user_type, _credit = split_from_right(details, 2)
        if self.user_store.remove_user(username):
            self.transaction_manager.removeUserGames(username)
        else:
            print(f"Delete failed: '{username}' does not exist")

    def _apply_add_credit(self, details):
        """Add credit: details are 'username type amount'."""
        username, _user_type, amount = split_from_right(details, 2)
        if not self.user_store.adjust_credit(username, Decimal(amount)):
            print(f"Add credit failed for '{username}'")


def main(arguments):
    """Run the Back End using file names from the command line (or the defaults)."""
    files = arguments if len(arguments) == len(DEFAULT_FILES) else DEFAULT_FILES
    users_in, games_in, collection_in, transactions_in, \
        users_out, games_out, collection_out = files

    back_end = BackEnd()
    try:
        back_end.load_all(users_in, games_in, collection_in, transactions_in)
    except (OSError, ValueError) as error:
        print(f"Fatal error reading input files: {error}")
        return 1
    back_end.apply_all_transactions()
    back_end.save_all(users_out, games_out, collection_out)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))

"""To run 
python main.py ../Phase03/Frontend/data/current-users.txt ../Phase03/Frontend/data/available-games.txt ../Phase03/Frontend/data/game-collection.txt daily-transactions.txt new-users.txt new-games.txt new-collection.txt"""