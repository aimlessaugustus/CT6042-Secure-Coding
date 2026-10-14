# Cryptographic Failure Notes

<!--
## To Do

Check out this site to learn more about ECB Penguin attacks[^ecbpenguingithub].

I want to develop an example that is similar where AES-ECB is shown both statically (A poor implementation and dynamically), and can be decrypted through repeated byte strings.

This maps to CVE-2022-28382, which can be referenced within the report[^CVE202228382].

-->

## CVE-2022-28382: Verbatim Drives Using AES-256-ECB Enabling Image Reconstruction

CVSS 3.1 Score: 7.5 (CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:U/C:H/I:N/A:N) [^NIST2026AESCB]

Linked CWEs: [^NIST2026AESCB][]

- CWE-327: Use of a Broken or Risky Cryptographic Algorithm
- CWE-1240: Use of a Cryptographic Primitive with a Risky Implementation

### Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

This laboratory example is designed to emulate a weakness found within Verbatim Keypad Secure drives discovered in 2022. The INIC-3637EN chip firmware within the drives was coded to use AES-256-ECB for encryption.

<!--
When encrypting using AES, the message is required to be broken into blocks of 16 bytes. [When there is insufficient data to completely fill a block, such as during the final round, padding is added to meet this requirement.] To chain these individual blocks together, cipher modes such as electronic codebook ECB are employed. ECB functions by encrypting each block independently, which provides insufficient diffusion to properly obfuscate larger datasets and images. Using ECB in these instances results in a large number of repeatable
-->

### Demonstration of Exploitability

To convert .bmp images using ECB, the following can produce

```bash
head -c 54 test.bmp > test.header
tail -c +54 test.bmp > test.body
openssl enc -aes-128-ecb -nosalt -pass pass:"superpassword" -in test.body -out test.body.ecb
cat test.header test.body.ecb > test.ecb.bmp
```

```bash
head -c 54 test.bmp > test.header
tail -c +54 test.bmp > test.body
openssl enc -aes-128-cbc -nosalt -pass pass:"superpassword" -in test.body -out test.body.cbc
cat test.header test.body.cbc > test.cbc.bmp
```

### Identifying the Vulnerability Using Techniques and Tools

### How to Mitigate the Vulnerability

### Retesting For Resilience

## References

[^NIST2026AESCB]: <https://nvd.nist.gov/vuln/detail/cve-2022-28382>
