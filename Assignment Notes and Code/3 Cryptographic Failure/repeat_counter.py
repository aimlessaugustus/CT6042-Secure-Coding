"""
Count repeated 16-byte blocks in a file.

This program:
    - takes a user-specified file as input
    - reads the file as complete 16-byte binary blocks
    - counts repeated block occurrences after their first appearance

Incomplete trailing data that is shorter than 16 bytes is ignored.

Usage:
    python repeat_counter.py <input_file>
"""

import argparse
from collections import Counter
from pathlib import Path


BLOCK_SIZE = 16


def count_repeated_blocks(file_path: Path) -> int:
    """
    Count repeated 16-byte blocks in a file.

    Args:
        file_path (Path): The file to inspect.

    Returns:
        int: The number of repeated block occurrences.
    """
    with file_path.open("rb") as file:
        blocks = iter(lambda: file.read(BLOCK_SIZE), b"")
        complete_blocks = (
            block for block in blocks if len(block) == BLOCK_SIZE)
        block_counts = Counter(complete_blocks)

    return sum(count - 1 for count in block_counts.values() if count > 1)


def main() -> None:
    """
    Parse the input file path and print its repeated block count.
    """
    parser = argparse.ArgumentParser(
        description="Count repeated 16-byte blocks in a file."
    )
    parser.add_argument("file", type=Path, help="path to the file to inspect")
    args = parser.parse_args()

    try:
        repeated_blocks = count_repeated_blocks(args.file)
    except OSError as error:
        parser.error(str(error))

    print(
        f"The number of repeated 16-byte blocks in {args.file} is: {repeated_blocks}")


if __name__ == "__main__":
    main()
