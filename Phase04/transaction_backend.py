"""
transaction_backend.py - buffers and writes legacy daily transaction records.

Records stay in their existing line format. This class reports file errors by
raising them instead of logging, so whoever calls it decides how to log them.
"""

from record_format import END_SENTINEL


class TransactionStore:
    """Buffers transaction records and reads/writes the daily transaction file."""

    def __init__(self):
        """Start with no pending transaction records."""
        self.transactions = []  # list of legacy transaction record lines

    # ---------- loading and writing ----------

    def load_transactions(self, path):
        """Read transaction records up to (not including) END."""
        with open(path, encoding="utf-8") as transaction_file:
            for line in transaction_file:
                line = line.rstrip("\r\n")
                if line.strip() == END_SENTINEL:
                    break
                if line.strip():
                    self.transactions.append(line)

    def write_daily_transactions(self, path):
        """Append pending records to the daily transaction file in legacy format."""
        with open(path, "a", encoding="utf-8") as transaction_file:
            for record in self.transactions:
                transaction_file.write(record + "\n")

    # ---------- updates ----------

    def record_transaction(self, record):
        """Queue one legacy transaction record line."""
        if not record.strip() or "\n" in record or "\r" in record:
            raise ValueError("transaction record must be a non-empty single line")
        if record.strip() == END_SENTINEL:
            raise ValueError("END is reserved as the transaction-file sentinel")
        self.transactions.append(record)

    def clear_transactions(self):
        """Clear pending records after they have been written."""
        self.transactions.clear()