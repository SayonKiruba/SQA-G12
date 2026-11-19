"""
record_format.py - shared knowledge about how records look in the text files.

Every file the Back End reads or writes is made of lines of text fields
separated by single spaces. The only layout rule the Back End relies on is
"the LAST few fields never contain spaces, but the FIRST field (a game name
or username) might". Splitting from the right therefore works even when the
Front End pads a game name to a different width than the spec shows.

If the Front End is ever replaced, this is the first file to revisit.
"""

# Column widths used when WRITING the master files (taken from the spec templates).
USERNAME_WIDTH = 15
USER_TYPE_WIDTH = 2
CREDIT_WIDTH = 9
GAME_NAME_WIDTH = 26      # template in the spec has 26 'I' characters (25 usable)
PRICE_WIDTH = 6

# Last record of every master file.
END_SENTINEL = "END"


def split_from_right(text, trailing_field_count):
    """
    Split `text` into one leading field plus `trailing_field_count` single-word
    fields taken from the right-hand end, and return them all as a list.
    The leading field is stripped of padding spaces.
    Raises ValueError if the text has too few fields (a malformed record).
    """
    parts = text.rsplit(None, trailing_field_count)
    if len(parts) != trailing_field_count + 1:
        raise ValueError(f"expected {trailing_field_count + 1} fields in '{text}'")
    parts[0] = parts[0].strip()
    return parts
