"""
CVE-2022-28382: AES-256-ECB in Verbatim drives

Controlled laboratory example modelled on CVE-2022-28382, where the
INIC-3637EN USB-to-SATA bridge firmware in certain Verbatim drives
encrypts stored data with AES-256 in ECB mode.

This program does the same thing in software:
    - takes any file (image, document, binary) as input
    - encrypts it with AES-256-ECB using a randomly generated key
    - stores the ciphertext and key locally on disk

Weakness under study: CWE-327 (Use of a Broken or Risky Cryptographic
Algorithm). ECB encrypts each 16-byte block independently with no IV or
chaining, so identical plaintext blocks always give identical ciphertext
blocks.

Usage:
    python controller_ecb.py encrypt <input_file> [--out DIR]
    python controller_ecb.py decrypt <file.enc> --key <file.key> [--out DIR]

Requires: pip install cryptography
"""
import argparse
import os
import sys
from pathlib import Path

from cryptography.hazmat.primitives import padding
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

BLOCK_BITS = 128    # AES block size
KEY_LEN = 32        # 32 bytes = 256-bit key


def encrypt(data: bytes, key: bytes) -> bytes:
    """
    Encrypts data with AES-256-ECB, using PKCS7 padding.

    Args:
        data (bytes): The data to encrypt.
        key (bytes): The encryption key.

    Returns:
        bytes: The encrypted data.
    """
    padder = padding.PKCS7(BLOCK_BITS).padder()
    padded = padder.update(data) + padder.finalize()
    encryptor = Cipher(algorithms.AES(key), modes.ECB()  # <-- vulnerable mode
                       ).encryptor()
    return encryptor.update(padded) + encryptor.finalize()


def decrypt(data: bytes, key: bytes) -> bytes:
    """
    Decrypts data with AES-256-ECB, using PKCS7 unpadding.

    Args:
        data (bytes): The data to decrypt.
        key (bytes): The decryption key.

    Returns:
        bytes: The decrypted data.
    """

    decryptor = Cipher(algorithms.AES(key), modes.ECB()).decryptor()
    padded = decryptor.update(data) + decryptor.finalize()
    unpadder = padding.PKCS7(BLOCK_BITS).unpadder()
    return unpadder.update(padded) + unpadder.finalize()


def cmd_encrypt(args) -> None:
    """
    Encrypts a file and saves the ciphertext and key to the output directory.

    Args:
        args (argparse.Namespace): Parsed arguments containing the input file and output directory.
    """
    src = Path(args.input)
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)

    key = os.urandom(KEY_LEN)
    ciphertext = encrypt(src.read_bytes(), key)

    enc_path = out / f"{src.name}.enc"
    key_path = out / f"{src.name}.key"
    enc_path.write_bytes(ciphertext)
    # stored alongside ciphertext (lab simplification)
    key_path.write_bytes(key)

    print(f"[+] Encrypted {src} with AES-256-ECB")
    print(f"    ciphertext -> {enc_path}")
    print(f"    key        -> {key_path}")


def cmd_decrypt(args) -> None:
    """
    Decrypts a .enc file using its key file and saves the result to the output directory.

    Args:
        args (argparse.Namespace): Parsed arguments containing the input file, key file and output directory.
    """

    src = Path(args.input)
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)

    key = Path(args.key).read_bytes()
    plaintext = decrypt(src.read_bytes(), key)

    name = src.name.removesuffix(".enc")
    dst = out / f"decrypted_{name}"
    dst.write_bytes(plaintext)
    print(f"[+] Decrypted {src} -> {dst}")


def main() -> None:
    parser = argparse.ArgumentParser(
        description="AES-256-ECB file encryption (lab baseline).")
    sub = parser.add_subparsers(dest="cmd", required=True)

    enc = sub.add_parser("encrypt", help="encrypt a file")
    enc.add_argument("input")
    enc.add_argument("--out", default="ecb_output")
    enc.set_defaults(func=cmd_encrypt)

    dec = sub.add_parser("decrypt", help="decrypt a .enc file")
    dec.add_argument("input")
    dec.add_argument("--key", required=True)
    dec.add_argument("--out", default="ecb_output")
    dec.set_defaults(func=cmd_decrypt)

    args = parser.parse_args()
    try:
        args.func(args)
    except FileNotFoundError as exc:
        sys.exit(f"[!] File not found: {exc.filename}")


if __name__ == "__main__":
    main()
