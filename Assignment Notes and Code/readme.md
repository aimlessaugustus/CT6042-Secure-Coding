# CT6042 Secure Coding — Software Vulnerability Portfolio

This repository holds the code, laboratory reproductions and supplementary testing notes for the CT6042 Secure Coding portfolio. The accompanying report analyses, exploits, detects, mitigates and re-verifies five vulnerability classes, and the material here is what each case study was built and tested against.

## Scope

Every example is a controlled laboratory reproduction rather than a live system. Each lab isolates a single vulnerability, so weaknesses outside that focus, such as demo credentials and minimal fixtures, are deliberately left in place and are out of scope. The labs that execute attacker-controlled code, namely the Java deserialisation and Apache Solr cases, were run in isolated virtual machines. The remaining three were developed locally.

## Case Studies

Each case follows the same structure, moving from the vulnerable implementation through exploitation, detection, mitigation and a final retest.

- **Broken access control (IDOR)**: an Insecure Direct Object Reference in a Spring Boot invoice API, where authorisation is enforced at the endpoint but not at the object level.
- **Java deserialisation**: a CVE-2020-9484-style gadget chain reaching arbitrary code execution through an unfiltered `ObjectInputStream`.
- **Cryptographic failure**: AES-256-ECB image reconstruction, recovering structure from ciphertext while the key stays secret.
- **Memory safety**: a two-pass heap overflow modelled on CVE-2026-42945, arising from a stale escape flag shared across sizing and copy passes.
- **Supply chain**: Log4Shell (CVE-2021-44228) inherited through the `log4j-core` dependency bundled in Apache Solr 8.11.0.

## Testing

Testing techniques were matched to each vulnerability class rather than applied uniformly. Static analysis and code review were paired with the deserialisation flaw, sanitisers and fuzzing with the memory-safety defect, software-composition analysis with the supply-chain case, and integration testing with the authorisation flaw. A tool finding was treated as confirmed only once a proof of concept demonstrated it, and every retest reran that proof of concept alongside a regression test.

The tools used, with their versions and known limitations, are set out in full in the report. The main ones are Semgrep and Bandit for static analysis, OWASP ZAP for dynamic testing, AddressSanitizer and libFuzzer for the memory-safety work, OSV-Scanner, Syft and Grype for software composition, and `ysoserial` to generate the deserialisation payload.

The exception to this is the supply chain failure lab, where the readme contains instructions to set up the same experimental setup used during the lab.

VM snapshots were taken for the supply chain and java deserialisation case studies, but couldn't be uploaded due to file upload restrictions. 

## Running the Examples

The three local labs were developed in VS Code and run directly. The deserialisation and Solr labs run inside their isolated virtual machines, with the Solr environment reproduced through Docker. Each lab folder contains its own vulnerable and mitigated builds, so the before-and-after behaviour can be observed in place, along with the exploit and retest scripts referenced in the report.

## A Note on Safety

The exploits here are working proofs of concept against deliberately vulnerable targets. They are intended for study within the isolated environments described above and should not be run against anything else.