# Cryptographic Failure Notes

<!--
## To Do

Check out this site to learn more about ECB Penguin attacks[^ecbpenguingithub].

I want to develop an example that is similar where AES-ECB is shown both statically (A poor implementation and dynamically), and can be decrypted through repeated byte strings.

This maps to CVE-2022-28382, which can be referenced within the report[^CVE202228382].

-->

## CVE-2022-28382: Verbatim Drives Using AES-256-ECB Enabling Image Reconstruction

CVSS 3.1 Score: 7.5 (CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:H/I:N/A:N) [^NIST2026AESCB]

Linked CWEs: [^NIST2026AESCB][^Dreeg2022AESECB]

- CWE-327: Use of a Broken or Risky Cryptographic Algorithm
- CWE-1240: Use of a Cryptographic Primitive with a Risky Implementation

### Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

This laboratory example is designed to emulate a weakness found within Verbatim Keypad Secure drives discovered in 2022. The INIC-3637EN chip firmware within the drives was coded to use AES-256-ECB for encryption. When encrypting with AES, a message or file is required to be broken down into 16 byte blocks, with cipher modes such as ECB chaining blocks together to produce a complete encrypted file.

ECB encrypts each block independently

<!--
When encrypting using AES, the message is required to be broken into blocks of 16 bytes. [When there is insufficient data to completely fill a block, such as during the final round, padding is added to meet this requirement.] To chain these individual blocks together, cipher modes such as electronic codebook ECB are employed. ECB functions by encrypting each block independently, which provides insufficient diffusion to properly obfuscate larger datasets and images. Using ECB in these instances results in a large number of repeatable
-->

### Demonstration of Exploitability

The initial laboratory example encrypts any file with AES-256-ECB using a random key, stores the `.enc` and `.key` files in `ecb_output/`, and decrypts them again.

To run the example, the following command was run in terminal:

```bash
python3 Assignment\ Notes\ and\ Code/3\ Cryptographic\ Failure/controller_ecb.py encrypt Assignment\ Notes\ and\ Code/3\ Cryptographic\ Failure/test.bmp
```

![Encryption Command](encryption_command.png)

This produced `test.bmp.enc` by encrypting the image `test.bmp`, as well as the secret key file `test.bmp.key`.

![Image prior to encryption](test.bmp)
*Figure: Sample Image Prior to Encryption.*

### Identifying the Vulnerability Using Techniques and Tools

When the encryption program is run, the raw `.enc` file can be viewed as raw data. Whilst the meaning of the bytes is obfuscated, large patterns of common bytes can be viewed, particularly within line 5 of the file.

![Repeated Bytes in File](repeated_bytes_in_file.png)
*Figure: Repeated bytes visible within Line 5 of the encrypted image.*

The `ecb_view.py` script demonstrates how an attacker can maniupulate this weakness by prepending a plaintext header with a guessed image width, enabling the disclosure of information through the reconstruction of the image.

*Listing: Function to Append Header*

```python
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
```

![Command to Reveal](encryption_attack_command.png)
*Figure: Attack being successfully executed.*

*Listing: Command Usage to Execute Attack*

```bash
python3 ecb_view.py test.bmp.enc
```

![ECB Image Reconstructed](../../ecb_output/ecb_exposed.bmp)
*Figure: Image Reconstructed from Attacking Script*

### How to Mitigate the Vulnerability

### Retesting For Resilience

## References

[^NIST2026AESCB]: <https://nvd.nist.gov/vuln/detail/cve-2022-28382>
[^Dreeg2022AESECB]: <https://packetstorm.news/files/id/167491>
