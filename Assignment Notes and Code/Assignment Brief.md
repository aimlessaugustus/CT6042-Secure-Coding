# Assignment Brief

Taken from the Moodle Page.

## General Information

Modern applications are assembled from source code, frameworks, third-party dependencies, APIs, authentication services, build systems and deployment configurations. Security failures can therefore originate from insecure code, inappropriate design decisions, vulnerable dependencies, configuration errors or weaknesses in the software build and delivery process.

Secure software development requires more than fixing vulnerabilities after they have been discovered. Developers must identify threats, implement appropriate controls, use security testing tools, evaluate the limitations of those tools and produce evidence that mitigations remain effective throughout the software development lifecycle.

In this assessment, you will review and improve a collection of controlled laboratory examples and produce a portfolio that addresses vulnerabilities (shown under vulnerability classes) by:

- Explaining and contextualising the vulnerability by explain the underlying security theory and identifying the relevant weakness using an appropriate classification, such as CWE or OWASP;

- Demonstrating how a vulnerability can be exploited with code examples

- Demonstrating how the code was tested to identify the vulnerability using appropriate techniques and tools, such as SAST, DAST, SCA, fuzzing, runtime memory-safety testing, automated unit, integration or regression testing and more.

- Demonstrating how to mitigate and what security measures were put into place with justification

- Testing again to make sure the code is resilient to the chosen attack

Your portfolio must include one case study from each of the following vulnerability classes:

1) Web application or identity security: examples include injection, cross-site scripting, broken access control, insecure session management, inappropriate OAuth 2.0 implementation or ineffective anti-automation controls.

2) Unsafe Java deserialization or software/data integrity

3) Cryptographic or secrets-management failure

4) Memory-safety failure: This may include a buffer overflow, use-after-free, integer overflow or another appropriate memory corruption vulnerability.

5) Software supply-chain or build-integrity failure: examples include a vulnerable dependency, dependency confusion, insecure package selection, an exposed secret, an unverified build artefact or an insecure CI/CD configuration.

Your portfolio should also include general discussion about:

- The importance of Security Development Life Cycle

- Product risk assessment and risk analysis

## Marking Scheme (For Reference)

| **Mark Range (%)** | **Knowledge & Understanding** | **Analysis** | **Reading & Research** | **Writing / Presentation Style** |
| --- | --- | --- | --- | --- |
| **80+** | Extraordinary and authoritative understanding of the five selected software errors, including root causes, exploit mechanisms, secure mitigations, verification, the Security Development Life Cycle and product risk. | Exceptional critical analysis of vulnerable and mitigated code. Exploitation, appropriate tool‑assisted testing and retesting are interpreted rigorously; security decisions, limitations and residual risks are evaluated convincingly. | Extensive, independent and critical research. Authoritative standards and high‑quality academic or technical sources are integrated throughout to substantiate technical claims, mitigations, lifecycle discussion and risk analysis. | Lively, precise and professional report with exceptional control of argument. Code, screenshots and tool results are clearly captioned, referenced and explained; appendices are complete; virtually error‑free. |
| **70–79** | Excellent and wide‑ranging understanding of the five software errors, their causes, exploitation, mitigation and retesting, with confident understanding of secure development lifecycle and risk principles. | Excellent and consistent analysis of practical evidence. Vulnerable and mitigated code, exploits, security‑tool findings and retests are interpreted critically, with well‑justified controls and thoughtful consideration of limitations and residual risk. | Excellent independent research using a wide range of relevant academic and authoritative technical sources. Evidence is selected critically, integrated effectively and used consistently to support claims. | Excellent, coherent and professional presentation with strong control of argument. Figures, code and test evidence are consistently captioned, referenced and discussed; source‑code appendices are complete; very few errors. |
