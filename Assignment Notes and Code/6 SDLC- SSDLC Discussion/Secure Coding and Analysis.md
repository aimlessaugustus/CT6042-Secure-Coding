# Secure Coding and Analysis

Secure code review is a core activity within a Secure Software Development Lifecycle because it strengthens an application before release and reduces the cost of fixing issues later. A significant proportion of modern security breaches originate from weaknesses in web applications, which makes early identification of flaws essential. Secure code review focuses on verifying and validating source code so that vulnerabilities can be detected and removed before deployment. When developers review code proactively during implementation, issues are resolved more quickly, fewer defects reach production, and release schedules remain stable. This contributes to long‑term resilience and supports compliance with organisational and regulatory security standards.

Secure code review can be performed manually or through automated tooling. Manual review involves an expert examining the code line by line, often in collaboration with developers to understand the application’s logic, intended behaviour and architectural decisions. This approach is thorough but time‑consuming, and is most effective when used to validate critical components or complex logic.

## Automated Approaches

### SAST

Automated approaches fall into two broad categories: static analysis and dynamic analysis. Static analysis evaluates source code without executing it. Static Application Security Testing (SAST) is a white‑box technique that scans code before compilation to identify insecure patterns, logic flaws and implementation‑level vulnerabilities. SAST is well suited to early development stages, where it can prevent insecure code from entering the codebase. Software Composition Analysis (SCA) complements SAST by examining third‑party libraries and dependencies for known vulnerabilities and licence risks. As modern applications rely heavily on open‑source components, SCA has become an essential part of secure development.

### DAST

Dynamic analysis examines an application while it is running. Dynamic Application Security Testing (DAST) is a black‑box technique that simulates external attacks to identify runtime vulnerabilities. Because DAST does not require access to source code, it is typically used in staging or pre‑production environments to validate how the application behaves under hostile conditions. It is particularly effective at identifying issues that only appear when the application is executed, such as authentication weaknesses, input validation failures or misconfigurations.

### IAST

Interactive Application Security Testing (IAST) combines elements of SAST and DAST using a grey‑box approach. IAST agents run alongside the application and analyse behaviour in real time. This allows IAST to trace data flows, identify the specific line of code responsible for a vulnerability, and provide immediate feedback to developers. It is especially useful during functional testing, where realistic user interactions can reveal subtle security issues.

### RASP

Runtime Application Self‑Protection (RASP) extends security into production environments. RASP monitors inbound and outbound traffic, analyses behavioural patterns and blocks malicious activity automatically. It provides continuous protection after deployment and helps organisations respond quickly to emerging threats.

A mature secure development process uses multiple techniques together. SAST identifies vulnerabilities during coding, SCA secures dependencies, DAST and IAST validate runtime behaviour, and RASP protects live systems. Integrating these tools into developer workflows and providing appropriate training ensures they enhance productivity rather than obstruct it, while significantly reducing the likelihood of vulnerabilities reaching production.
