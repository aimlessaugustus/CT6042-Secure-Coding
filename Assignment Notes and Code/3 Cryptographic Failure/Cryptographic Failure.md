# Cryptographic Failure Notes

## CVE-2022-28382: Verbatim Drives Using AES-256-ECB Enabling Image Reconstruction

CVSS 3.1 Score: 7.5 (CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:H/I:N/A:N) [^NIST2026AESCB]

Linked CWEs: [^NIST2026AESCB][^Dreeg2022AESECB]

- CWE-327: Use of a Broken or Risky Cryptographic Algorithm
- CWE-1240: Use of a Cryptographic Primitive with a Risky Implementation

OWASP

### Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

This laboratory example is designed to emulate a weakness found within Verbatim Keypad Secure drives discovered in 2022. The INIC-3637EN chip firmware within the drives was coded to use AES-256-ECB for encryption. When encrypting with AES, a message or file is required to be broken down into 16 byte blocks, with cipher modes such as ECB defining how blocks relate.

ECB encrypts each block independently, which allows for faster encryption than other modes, but results in deterministic block encryption, which is detrimental to structured files such as images that have large numbers of repeating bytes [^Dworkin2001NISTECBCBC][^Riatma2025AESModes]. This enables the disclosure of information by a threat actor through the prepending of individual blocks together with an arbitary image header.

### Demonstration of Exploitability

The initial laboratory example encrypts any file with AES-256-ECB using a random key, stores the `.enc` and `.key` files in `ecb_output/`, and decrypts them again. This is handled by the python Cryptography library, and with the program flow being shown below.

*Listing: ECB mode being used within the Python encryption file.*

```python
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
```

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

 This means that vulnerability identification must be done using a combination of a custom python script for image reconstruction and manual file inspection to demonstrate and test the vulnerability during black-box ciphertext analysis, aligning to existing research methods [^Boussif2025AES][^Wattar2020AESImages]. Whilst this allows for the verfication of results with experimental data, it is also limited by a reliance on the accuracy of the python script.

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

Mitigation of the vulnerability would occur during the Requirements and Analysis phase of the SSDLC, when developers define appropriate encryption methods for their application use case. Developing firmware for constrained devices like the INIC-3637EN poses an additional challenge of balancing a cryptographically secure cipher mode with limited available processing power, which had to be considered when proposing mitigations. The first mitigation is to use AES-256-CBC, which uses an Initialisation Vector to chain blocks and eliminate the repeated byte patterns at the cost of processing power [^Dworkin2001NISTECBCBC][^Munir2024ImageEncryption]. Whilst ECB and CBC are vulnerable to padding attacks, this particular risk is reduced, as drives have no oracle to report padding[^Boussif2025AES]. The second mitigation option is to use AES-256-GCM, which has the same benefits of CBC with added resistance to padding oracle attacks and authentication methods to identify tampering [^Dworkin2007NISTGCM]. Whilst more processing power is consumed than the other two methods, AES-256-GCM has still be identified as suitable for constrained device development [^Riatma2025AESModes]. Both address the lack of diffusion within EBC.

*Listing: Updated Code with CBC Encryption for Chaining*

```python
def encrypt(data: bytes, key: bytes) -> bytes:
    """
    Encrypts data with AES-256-CBC, using PKCS7 padding and a random IV.

    Args:
        data (bytes): The data to encrypt.
        key (bytes): The encryption key.

    Returns:
        bytes: The IV followed by the encrypted data.
    """
    iv = os.urandom(IV_LEN)     # new random IV for every encryption
    padder = padding.PKCS7(BLOCK_BITS).padder()
    padded = padder.update(data) + padder.finalize()
    encryptor = Cipher(algorithms.AES(key), modes.CBC(iv)  # <-- chained mode
                       ).encryptor()
    return iv + encryptor.update(padded) + encryptor.finalize()
```

*Listing: Amended Code Using AES-256-GCM for Encryption.*

```python
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
```

### Retesting For Resilience

The code was then retested using both mitigation methods, with both outputs being unidentifiable by the image reconstruction attack. Static analysis of the files also confirmed a lack of repeated bytes. One major limitation of both the testing and retesting methods is the requirement for human inspection, limiting automation without LLM usage. However, this is rooted in the nature of the vulnerability, which requires a threat actor to inspect an image for meaningful information.

![CBCAttemptedReconstruction](../../cbc_output/cbc_reconstruction.bmp)
*Figure: Failed Image Reconstruction Attempt When Using CBC.*

![GCMAttemptedReconstruction](../../gcm_output/gcm_reconstruction.bmp)
*Figure: Failed Image Reconstruction Attempt When Using GCM.*

## References

[^NIST2026AESCB]: <https://nvd.nist.gov/vuln/detail/cve-2022-28382>
[^Dreeg2022AESECB]: <https://packetstorm.news/files/id/167491>
[^Riatma2025AESModes]: <https://jurnal.polinema.ac.id/index.php/jartel/article/view/7323>
[^Boussif2025AES]: <https://ieeexplore.ieee.org/document/9826324>
[^Wattar2020AESImages]: <https://turcomat.org/index.php/turkbilmat/article/view/14117>
[^Munir2024ImageEncryption]: <https://www.mdpi.com/2624-800X/4/1/7>
[^Dworkin2001NISTECBCBC]: <https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38a.pdf>
[^Dworkin2007NISTGCM]: <https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38d.pdf>
