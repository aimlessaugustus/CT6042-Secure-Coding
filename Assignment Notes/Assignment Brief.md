# Assignment Information

Taken From:

Modern applications are assembled from source code, frameworks, third-party dependencies, APIs, authentication services, build systems and deployment configurations. Security failures can therefore originate from insecure code, inappropriate design decisions, vulnerable dependencies, configuration errors or weaknesses in the software build and delivery process. 

Secure software development requires more than fixing vulnerabilities after they have been discovered. Developers must identify threats, implement appropriate controls, use security testing tools, evaluate the limitations of those tools and produce evidence that mitigations remain effective throughout the software development lifecycle. 

In this assessment, you will review and improve a collection of controlled laboratory examples and produce a portfolio that addresses vulnerabilities (shown under vulnerability classes) by: 

- Explaining and contextualising the vulnerability by explain the underlying security theory and identifying the relevant weakness using an appropriate classification, such as CWE or OWASP; 

- Demonstrating how a vulnerability can be exploited with code examples 

- Demonstrating how the code was tested to identify the vulnerability using appropriate techniques and tools, such as SAST, DAST, SCA, fuzzing, runtime memory-safety testing, automated unit, integration or regression testing and more. 

- Demonstrating how to mitigate and what security measures were put into place with justification 

- Testing again to make sure the code is resilient to the chosen attack 

Your portfolio must include **one case study from each of the following vulnerability classes**: 

- Web application or identity security: examples include injection, cross-site scripting, broken access control, insecure session management, inappropriate OAuth 2.0 implementation or ineffective anti-automation controls. 

- Unsafe Java deserialization or software/data integrity 

- Cryptographic or secrets-management failure 

- Memory-safety failure: This may include a buffer overflow, use-after-free, integer overflow or another appropriate memory corruption vulnerability. 

- Software supply-chain or build-integrity failure: examples include a vulnerable dependency, dependency confusion, insecure package selection, an exposed secret, an unverified build artefact or an insecure CI/CD configuration. 

Your portfolio should also include general discussion about: 

- The importance of Security Development Life Cycle 

- Product risk assessment and risk analysis 