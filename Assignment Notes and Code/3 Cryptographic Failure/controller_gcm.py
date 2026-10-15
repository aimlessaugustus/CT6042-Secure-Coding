"""
CVE-2022-28382 mitigation (alternative 2): AES-256-GCM

This program:
    - takes any file (image, document, binary) as input
    - encrypts it with AES-256-GCM using a randomly generated key and nonce
    - stores the ciphertext (nonce prepended, tag appended) and key locally on disk

How GCM addresses CWE-327: a counter, seeded from a random 12-byte nonce,
is encrypted to produce a unique keystream for every block. Identical
plaintext blocks therefore produce different ciphertext blocks, and
encrypting the same file twice gives completely different output.

How GCM improves on CBC: it also computes a 16-byte authentication tag over
the ciphertext. Any modification to the stored data causes decryption to
fail, so tampering is detected (CWE-353). No padding is used, which removes
the padding oracle risk.

Remaining requirement: a nonce must never be reused with the same key.

Usage:
    python controller_gcm.py encrypt <input_file> [--out DIR]
    python controller_gcm.py decrypt <file.enc> --key <file.key> [--out DIR]

Requires: pip install cryptography
"""
import argparse
import os
import sys
from pathlib import Path

from cryptography.exceptions import InvalidTag
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

KEY_LEN = 32        # 32 bytes = 256-bit key
NONCE_LEN = 12      # 12 bytes = 96-bit nonce (recommended for GCM)
TAG_LEN = 16        # 16 bytes = 128-bit authentication tag


def encrypt(data: bytes, key: bytes) -> bytes:
    """
    Encrypts data with AES-256-GCM, using a random nonce.

    Args:
        data (bytes): The data to encrypt.
        key (bytes): The encryption key.

    Returns:
        bytes: The nonce, followed by the encrypted data and authentication tag.
    """
    nonce = os.urandom(NONCE_LEN)   # new random nonce for every encryption
    encryptor = Cipher(algorithms.AES(key), modes.GCM(nonce)  # <-- authenticated mode
                       ).encryptor()
    ciphertext = encryptor.update(data) + encryptor.finalize()
    return nonce + ciphertext + encryptor.tag


def decrypt(data: bytes, key: bytes) -> bytes:
    """
    Decrypts data with AES-256-GCM, verifying the authentication tag.

    Args:
        data (bytes): The nonce, followed by the encrypted data and authentication tag.
        key (bytes): The decryption key.

    Returns:
        bytes: The decrypted data.

    Raises:
        InvalidTag: If the data has been modified or the key is wrong.
    """
    nonce, ciphertext, tag = data[:NONCE_LEN], data[NONCE_LEN:-
                                                    TAG_LEN], data[-TAG_LEN:]
    decryptor = Cipher(algorithms.AES(key), modes.GCM(nonce, tag)).decryptor()
    return decryptor.update(ciphertext) + decryptor.finalize()


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

    print(f"[+] Encrypted {src} with AES-256-GCM")
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
        description="AES-256-GCM file encryption (ECB alternative).")
    sub = parser.add_subparsers(dest="cmd", required=True)

    enc = sub.add_parser("encrypt", help="encrypt a file")
    enc.add_argument("input")
    enc.add_argument("--out", default="gcm_output")
    enc.set_defaults(func=cmd_encrypt)

    dec = sub.add_parser("decrypt", help="decrypt a .enc file")
    dec.add_argument("input")
    dec.add_argument("--key", required=True)
    dec.add_argument("--out", default="gcm_output")
    dec.set_defaults(func=cmd_decrypt)

    args = parser.parse_args()
    try:
        args.func(args)
    except FileNotFoundError as exc:
        sys.exit(f"[!] File not found: {exc.filename}")
    except InvalidTag:
        sys.exit(
            "[!] Authentication failed: the file has been modified or the key is wrong.")


if __name__ == "__main__":
    main()
