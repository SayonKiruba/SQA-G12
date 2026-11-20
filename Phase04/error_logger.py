"""
error_logger.py - prints Back End errors to the terminal in one common format.

Every error line starts with "ERROR:". Transaction errors (a transaction broke a
rule) let processing continue; fatal errors (a file problem) stop the program.
"""

import sys


class ErrorLogger:
    """Prints formatted transaction errors and fatal errors to the terminal."""

    @staticmethod
    def logTransactionError(errorType, description, transaction):
        """Print an ERROR line with the error type, description and offending transaction."""
        print(f"ERROR: {errorType}: {description}: {transaction}")

    @staticmethod
    def logFatalError(errorType, description, filename):
        """Print an ERROR line with the error type, description and file, then stop the program."""
        print(f"ERROR: {errorType}: {description}: {filename}")
        sys.exit(1)