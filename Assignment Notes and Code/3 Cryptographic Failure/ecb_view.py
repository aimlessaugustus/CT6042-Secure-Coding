"""
ecb_view.py - Exploit stage: view AES-256-ECB ciphertext as a BMP image

Demonstrates CWE-327 without the key. The attacker only has the .enc file.
The encrypted BMP header is unreadable, so a fresh plaintext header is built
and placed in front of the ciphertext. Any image viewer can then open it,
and because ECB encrypts identical blocks identically, the image's shapes
remain visible.

The attacker needs to guess only the image width. The height is worked out
from the ciphertext length.

Usage:
    python ecb_view.py <file.enc> --width 1024 [--out ecb_exposed.bmp]

Uses the standard library only.
"""
import argparse
import struct
import sys
from pathlib import Path

# 14-byte file header + 40-byte info header (24-bit BMP)
HEADER_SIZE = 54
BYTES_PER_PIXEL = 3     # 24-bit colour


def build_bmp_header(width: int, height: int) -> bytes:
    """
    Builds a standard 54-byte header for an uncompressed 24-bit BMP.

    Args:
        width (int): The image width in pixels.
        height (int): The image height in pixels.

    Returns:
        bytes: The BMP file header and info header.
    """
    row_size = (width * BYTES_PER_PIXEL +
                3) & ~3       # rows padded to 4 bytes
    pixel_bytes = row_size * height
    file_header = struct.pack(
        "<2sIHHI", b"BM", HEADER_SIZE + pixel_bytes, 0, 0, HEADER_SIZE)
    info_header = struct.pack("<IiiHHIIiiII", 40, width, height, 1, 24, 0,
                              pixel_bytes, 2835, 2835, 0, 0)
    return file_header + info_header


def expose(ciphertext: bytes, width: int) -> bytes:
    """
    Replaces the encrypted header with a plaintext one so the data can be viewed.

    Args:
        ciphertext (bytes): The AES-256-ECB encrypted BMP file.
        width (int): The guessed image width in pixels.

    Returns:
        bytes: A viewable BMP built from the ciphertext.
    """
    row_size = (width * BYTES_PER_PIXEL + 3) & ~3
    height = (len(ciphertext) - HEADER_SIZE) // row_size
    if height <= 0:
        raise ValueError("Width too large for this ciphertext.")
    pixels = ciphertext[HEADER_SIZE:HEADER_SIZE + row_size * height]
    return build_bmp_header(width, height) + pixels


def main() -> None:
    parser = argparse.ArgumentParser(
        description="View ECB ciphertext as a BMP (no key needed).")
    parser.add_argument("input", help="the .enc file")
    parser.add_argument("--width", type=int, required=True,
                        help="guessed image width in pixels")
    parser.add_argument("--out", default="ecb_exposed.bmp")
    args = parser.parse_args()

    try:
        ciphertext = Path(args.input).read_bytes()
        Path(args.out).write_bytes(expose(ciphertext, args.width))
    except (FileNotFoundError, ValueError) as exc:
        sys.exit(f"[!] {exc}")

    print(f"[+] Wrote {args.out} from {args.input} (no key used)")


if __name__ == "__main__":
    main()
