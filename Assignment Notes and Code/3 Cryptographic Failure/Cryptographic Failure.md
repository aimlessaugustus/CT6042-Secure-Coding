# Cryptographic Failure Notes

## To Do

Check out this site to learn more about ECB Penguin attacks[^ecbpenguingithub].

I want to develop an example that is similar where AES-ECB is shown both statically (A poor implementation and dynamically), and can be decrypted through repeated byte strings.

This maps to CVE-2022-28382, which can be referenced within the report[^CVE202228382].

[^ecbpenguingithub]: <https://github.com/robertdavidgraham/ecb-penguin>

[^CVE202228382]: <https://nvd.nist.gov/vuln/detail/cve-2022-28382>
