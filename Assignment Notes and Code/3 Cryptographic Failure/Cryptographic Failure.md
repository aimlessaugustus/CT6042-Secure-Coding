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

ECB encrypts each block independently without

<!--
When encrypting using AES, the message is required to be broken into blocks of 16 bytes. [When there is insufficient data to completely fill a block, such as during the final round, padding is added to meet this requirement.] To chain these individual blocks together, cipher modes such as electronic codebook ECB are employed. ECB functions by encrypting each block independently, which provides insufficient diffusion to properly obfuscate larger datasets and images. Using ECB in these instances results in a large number of repeatable
-->

### Demonstration of Exploitability

The initial laboratory example encrypts any file with AES-256-ECB using a random key, stores the `.enc` and `.key` files in `ecb_output/`, and decrypts them again.

To run the example, the following command was run in terminal:

```bash
python3 Assignment\ Notes\ and\ Code/3\ Cryptographic\ Failure/CVE-2022-28382.py encrypt Assignment\ Notes\ and\ Code/3\ Cryptographic\ Failure/test.bmp
```

### Identifying the Vulnerability Using Techniques and Tools

### How to Mitigate the Vulnerability

### Retesting For Resilience

## References

[^NIST2026AESCB]: <https://nvd.nist.gov/vuln/detail/cve-2022-28382>
[^Dreeg2022AESECB]: <https://packetstorm.news/files/id/167491>
