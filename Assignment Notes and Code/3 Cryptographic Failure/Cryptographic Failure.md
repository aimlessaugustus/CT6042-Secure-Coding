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

ECB encrypts each block independently, which allows for faster encryption than other codebooks, but provides insufficent diffusion for structured files such as images that have large numbers of repeating bytes [^Riatma2025AESModes]. This enables the disclosure of information by a threat actor through the appending of individual blocks together with an arbitary image header.

<!--
When encrypting using AES, the message is required to be broken into blocks of 16 bytes. [When there is insufficient data to completely fill a block, such as during the final round, padding is added to meet this requirement.] To chain these individual blocks together, cipher modes such as electronic codebook ECB are employed. ECB functions by encrypting each block independently, which provides insufficient diffusion to properly obfuscate larger datasets and images. Using ECB in these instances results in a large number of repeatable
-->

### Demonstration of Exploitability

The initial laboratory example encrypts any file with AES-256-ECB using a random key, stores the `.enc` and `.key` files in `ecb_output/`, and decrypts them again. This is handled by the python Cryptography library, and with the program flow being shown below.

To run the example, the following command was run in terminal:

*Listing: Bash Command to Encrypt an Image*

```bash
python3 controller_ecb.py encrypt test.bmp
```

![Encryption Command](encryption_command.png)

This produced `test.bmp.enc` by encrypting the image `test.bmp`, as well as the secret key file `test.bmp.key`.

![Image prior to encryption](test.bmp)
*Figure: Sample Image Prior to Encryption.*

### Identifying the Vulnerability Using Techniques and Tools

 Whilst reputable tools such as Padbuster can be used to demonstrate and verify padding attacks against AES codebooks, no such tooling exists for image reconstruction. This means that vulnerability identification must be done using a combination of a custom python script for image reconstruction and manual file inspection to demonstrate and test the vulnerability for DAST, aligning to existing research methods [^Boussif2025AES][^Wattar2020AESImages]. Whilst this allows for the verfication of results with experimental data, it is also limited by a reliance on the accuracy of the python script.

When the encryption program is run, the raw `.enc` file can be viewed as raw data. Whilst the meaning of the bytes is obfuscated, large patterns of common bytes can be viewed, particularly within line 5 of the file.

![Repeated Bytes in File](repeated_bytes_in_file.png)
*Figure: Repeated bytes visible within Line 5 of the encrypted image.*

The `ecb_view.py` script demonstrates how an attacker can manipulate this weakness by prepending a plaintext header with a guessed image width, enabling the disclosure of information through the reconstruction of the image. The outcome can be seen in the figure below, confirming the presence of the ECB vulnerability within the code.

*Listing: Function to Append Header in ecb_view.py*

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
                1) & ~3       # rows padded to 4 bytes
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

Mitigation of the vulnerability would occur during the Requirements and Analysis phase of the SSDLC, when developers define appropriate encryption methods for their application use case. Developing firmware for constrained devices like the INIC-3637EN poses an additional challenge of balancing a cryptographically secure cipher mode with limited available processing power, which had to be considered when proposing mitigations. The first mitigation is to use AES-256-CBC, which uses an Initialisation Vector to chain blocks and eliminate the repeated byte patterns at the cost of processing power [^Munir2024ImageEncryption]. However, both ECB and CBC are vulnerable to padding attacks, leaving residual risk[^Boussif2025AES]. The second mitigation option is to use AES-256-GCM, which has the same benefits of CBC with added resistance to padding oracle attacks. Whilst more processing power is consumed than the other two methods, AES-256-GCM has still be identified as suitable for constrained device development [^Riatma2025AESModes]. Both provide higher diffusion than EBC.

### Retesting For Resilience

## References

[^NIST2026AESCB]: <https://nvd.nist.gov/vuln/detail/cve-2022-28382>
[^Dreeg2022AESECB]: <https://packetstorm.news/files/id/167491>
[^Riatma2025AESModes]: <https://jurnal.polinema.ac.id/index.php/jartel/article/view/7323>
[^Boussif2025AES]: <https://ieeexplore.ieee.org/document/9826324>
[^Wattar2020AESImages]: <https://turcomat.org/index.php/turkbilmat/article/view/14117>
[^Munir2024ImageEncryption]: <https://www.mdpi.com/2624-800X/4/1/7>
