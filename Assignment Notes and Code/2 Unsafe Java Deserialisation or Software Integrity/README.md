# Java Deserealisation

## Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

This laboratory example emulates CVE-2020-9484, in which Apache Tomcat’s PersistentManager deserialises session files through an unfiltered ObjectInputStream \parencite{NIST2026CVE9484}. Native deserialisation rebuilds an arbitrary object graph, invoking callbacks such as readObject before validation \parencite{Oracle2018SerialGuide}. An attacker chains gadget classes already on the classpath, here Apache Commons Collections, into a sink such as Runtime.exec(), gaining remote code execution without any malicious class of their own. The flaw maps to CWE-502 (Deserialization of Untrusted Data) and OWASP A08:2025 – Software and Data Integrity Failures \parencite{CWE502, OWASP2021A08}. NVD assigns a CVSS 3.1 base score of 7.0, its Local vector and High complexity reflecting preconditions absent from a network-reachable sink \parencite{NIST2026CVE9484}.

## Demonstration of Exploitability

The minimal sink is a single unfiltered readObject() on a session file (Listing~X). Exploitation needs no flaw in the application’s own classes. Using ysoserial, an attacker serialises a CommonsCollections6 gadget chain that embeds an operating-system command, here touch /tmp/pwned, and writes the result as a .session file; in the lab this is placed directly, while in CVE-2020-9484 a ../ session identifier redirected the store to an uploaded file. When the application reloads that session, readObject() rebuilds the object graph and the chain executes during reconstruction, before any value is returned (Figure~X). The dropped /tmp/pwned file confirms arbitrary execution. Because the command runs with the JVM’s privileges, impact is complete: confidentiality, integrity and availability are all lost. The application never references Commons Collections; its presence on the classpath suffices.

## Identifying the Vulnerability Using Techniques and Tools

## How to Mitigate the Vulnerability

## Retesting For Resilience

---

# Supplementary example information
