# Cryptographic Failure Notes

## To Do

Check out this site to learn more about ECB Penguin attacks[^ecbpenguingithub].

I want to develop an example that is similar where AES-ECB is shown both statically (A poor implementation and dynamically), and can be decrypted through repeated byte strings.

This maps to CVE-2022-28382, which can be referenced within the report[^CVE202228382].

### Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

AES requires the input for each block to be of fixed size for each round of encryption. For the last block to be encrypted, padding is used.

### Demonstration of Exploitability

### Identifying the Vulnerability Using Techniques and Tools

### How to Mitigate the Vulnerability

### Retesting For Resilience

## Marking Scheme (For Reference)

| **Mark Range (%)** | **Knowledge & Understanding** | **Analysis** | **Reading & Research** | **Writing / Presentation Style** |
| --- | --- | --- | --- | --- |
| **80+** | Extraordinary and authoritative understanding of the five selected software errors, including root causes, exploit mechanisms, secure mitigations, verification, the Security Development Life Cycle and product risk. | Exceptional critical analysis of vulnerable and mitigated code. Exploitation, appropriate tool‑assisted testing and retesting are interpreted rigorously; security decisions, limitations and residual risks are evaluated convincingly. | Extensive, independent and critical research. Authoritative standards and high‑quality academic or technical sources are integrated throughout to substantiate technical claims, mitigations, lifecycle discussion and risk analysis. | Lively, precise and professional report with exceptional control of argument. Code, screenshots and tool results are clearly captioned, referenced and explained; appendices are complete; virtually error‑free. |
| **70–79** | Excellent and wide‑ranging understanding of the five software errors, their causes, exploitation, mitigation and retesting, with confident understanding of secure development lifecycle and risk principles. | Excellent and consistent analysis of practical evidence. Vulnerable and mitigated code, exploits, security‑tool findings and retests are interpreted critically, with well‑justified controls and thoughtful consideration of limitations and residual risk. | Excellent independent research using a wide range of relevant academic and authoritative technical sources. Evidence is selected critically, integrated effectively and used consistently to support claims. | Excellent, coherent and professional presentation with strong control of argument. Figures, code and test evidence are consistently captioned, referenced and discussed; source‑code appendices are complete; very few errors. |

[^ecbpenguingithub]: <https://github.com/robertdavidgraham/ecb-penguin>

[^CVE202228382]: <https://nvd.nist.gov/vuln/detail/cve-2022-28382>
