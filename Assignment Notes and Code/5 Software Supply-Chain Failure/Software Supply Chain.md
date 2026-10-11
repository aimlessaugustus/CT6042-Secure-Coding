### Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

### Demonstration of Exploitability

### Identifying the Vulnerability Using Techniques and Tools

### How to Mitigate the Vulnerability

#### Detecting and Mitigating Supply Chain Attacks (Broadly)

Organisations can use Security Information and Event Management (SIEM) platforms to identify indicators of supply chain compromise by analysing logs for unusual or unauthorised activity. For example, unexpected connections to cryptocurrency-related services or known malicious domains may indicate a compromised third-party dependency and should be investigated or blocked.

Key defensive measures include:

- **Monitoring Web Traffic:** Analyse web and application logs for unexpected requests originating from third-party libraries, particularly communications with unknown external domains or suspicious endpoints. However, some malicious JavaScript-based supply chain attacks may execute on the client side and leave little or no evidence in server logs.

- **Tracking Dependency Updates:** Maintain records of dependency and package version changes. Updates to widely used libraries should be reviewed against official release notes and verified sources to identify potentially malicious modifications.

- **Configuring Security Alerts:** Implement automated alerts for abnormal behaviours, such as unauthorised outbound connections, unexpected API calls, or interactions with cryptocurrency services that could suggest a compromise.

- **Restricting Access and Permissions:** Protect software distribution and development accounts by enforcing multi-factor authentication (MFA), limiting privileged access, regularly rotating credentials, and securing publishing tokens.

- **Using Trusted Sources:** Obtain software packages only from reputable registries, official vendors, or internally approved repositories to reduce exposure to malicious or tampered dependencies.

- **Implementing Dependency Management Controls:** Deploy dependency monitoring and integrity-checking tools to detect unauthorised code changes, suspicious package updates, and other indicators of supply chain compromise throughout the software development lifecycle.

### Retesting For Resilience

## Marking Scheme (For Reference)

| **Mark Range (%)** | **Knowledge & Understanding** | **Analysis** | **Reading & Research** | **Writing / Presentation Style** |
| --- | --- | --- | --- | --- |
| **80+** | Extraordinary and authoritative understanding of the five selected software errors, including root causes, exploit mechanisms, secure mitigations, verification, the Security Development Life Cycle and product risk. | Exceptional critical analysis of vulnerable and mitigated code. Exploitation, appropriate tool‑assisted testing and retesting are interpreted rigorously; security decisions, limitations and residual risks are evaluated convincingly. | Extensive, independent and critical research. Authoritative standards and high‑quality academic or technical sources are integrated throughout to substantiate technical claims, mitigations, lifecycle discussion and risk analysis. | Lively, precise and professional report with exceptional control of argument. Code, screenshots and tool results are clearly captioned, referenced and explained; appendices are complete; virtually error‑free. |
| **70–79** | Excellent and wide‑ranging understanding of the five software errors, their causes, exploitation, mitigation and retesting, with confident understanding of secure development lifecycle and risk principles. | Excellent and consistent analysis of practical evidence. Vulnerable and mitigated code, exploits, security‑tool findings and retests are interpreted critically, with well‑justified controls and thoughtful consideration of limitations and residual risk. | Excellent independent research using a wide range of relevant academic and authoritative technical sources. Evidence is selected critically, integrated effectively and used consistently to support claims. | Excellent, coherent and professional presentation with strong control of argument. Figures, code and test evidence are consistently captioned, referenced and discussed; source‑code appendices are complete; very few errors. |
