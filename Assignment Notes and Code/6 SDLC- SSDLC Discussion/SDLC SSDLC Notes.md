# SDLC and SSDLC Notes + Product Risk Assessment Notes

These are the raw notes on the Software Development Lifecycle/Secure Software Development Lifecycle (SDLC/SSDLC) that I will process into a section of the assignment.

## Software Development Lifecycle (SDLC)

The Software Development Lifecycle (SDLC) is a structured framework for building software efficiently and predictably. It divides development into measurable phases to improve quality, reduce cost, and ensure projects meet user expectations. Typical stages include Planning, Requirements, Design, Development, Testing, Deployment, and Maintenance.  

The SDLC standardises tasks within each phase, enabling monitoring, repeatability, and continuous improvement. Organisations may merge or split phases depending on their chosen model, such as Waterfall, Agile, or DevOps.

### Phases

#### 1. Planning

The planning phase defines the system’s purpose, scope, and feasibility. It establishes project boundaries, identifies the core problem, and outlines the resources required for development. Effective planning enables early detection of issues that could hinder progress and ensures the project remains aligned with its intended goals. This stage is essential when working to fixed market deadlines, as it provides a structured foundation for scheduling, cost estimation, and resource allocation.

#### 2. Requirements Definition

Requirements definition involves gathering all functional and technical needs of the proposed system. This includes listing prototype requirements, evaluating alternatives, and analysing end‑user expectations. Clear requirements determine what the application must achieve and what resources are necessary to build it. Developers often formalise this information in a Software Requirements Specification (SRS), ensuring the system’s purpose, features, and dependencies (such as hardware for specialised manufacturing software) are fully understood before design begins.A Risk Assessment evaluates the likelihood and impact of threats being exploited within a system. It is a core component of secure software development because vulnerabilities, design flaws and poorly reviewed code increase the probability of harmful outcomes. Risk management within the SDLC aims to identify, reduce or eliminate these risks through appropriate controls.

The process begins by assuming the software will be attacked and analysing factors that motivate a threat actor. These include the value of the data handled by the system, the security maturity of dependent services, the scale of software distribution and the expectations of clients. Organisations must define an acceptable level of risk and communicate trade‑offs clearly. For example, the financial impact of data loss may outweigh the cost of fixing security defects, and reputational damage may exceed both.

Risk evaluation considers worst‑case scenarios, the value of assets at risk, the complexity of potential attacks, the number of users affected and the accessibility of the target. High‑impact or easily exploitable risks require mitigation. Understanding whether an attacker can access production systems or only isolated environments is essential for prioritisation.

Different assessment methods exist. A Qualitative Risk Assessment classifies risk as low, medium or high using the relationship between severity and likelihood. A Quantitative Risk Assessment uses numerical values, often based on business criticality, to calculate exposure. Metrics such as Annual Rate of Occurrence and Annual Loss Expectancy help determine whether security controls are cost‑effective.

Risk assessments are most effective early in the SDLC, where they guide design decisions and support security‑by‑design practices.

#### 3. Design and Prototyping

The design phase converts requirements into a structured technical blueprint. Developers define user interfaces, system interfaces, network needs, database instances, communication methods, and security measures. SRS content is translated into logical designs and architectural decisions, including coding standards, templates, and operational plans. Teams typically produce an Architecture Design Review (ADR) to ensure consistent understanding across front‑end, back‑end, and authentication components. This phase establishes how the system will function, interact, and be maintained throughout its lifecycle.

#### 4. Software Development

Software development involves writing code according to the previously defined design documents. Developers follow organisational coding guidelines and use tools such as compilers, debuggers, and interpreters to implement functionality. Documentation, such as playbooks and implementation guides, support clarity and consistency. Incorporating secure coding practices, hygiene, and best‑practice standards strengthens code quality and ensures the application is built reliably and safely.

#### 5. Testing

Testing ensures the application meets the quality standards defined earlier in the SDLC. Developers and QA engineers use automated tools, source‑code scanners, and structured test processes to validate functionality and security. Testing follows its own lifecycle, including test case design, test environment setup, and test execution. Test cases must be simple, repeatable, and aligned with core requirements, enabling reliable regression testing as new features are added. A controlled test environment replicates real‑world conditions, including hardware, software, configurations, and user profiles. During execution, testers run functional and non‑functional tests, report defects, and measure performance. Automation is essential for achieving sufficient coverage and maintaining developer velocity.

#### 6. Deployment

Deployment integrates all tested modules into the main codebase and releases the software to end‑users. Once testing is complete, the application is considered ready for market. Many organisations automate deployment using software deployment tools to streamline rollouts, manage configurations, and support actions such as device reboots. Automation also enables controlled releases across selected machines and provides rollback capability if issues arise. Deployment may involve releasing new website features or distributing updated mobile application versions. The phase focuses on reliable delivery, ensuring the final product reaches users consistently and safely.

#### 7. Operations & Maintenance

After release, developers shift to maintaining stability, uptime, and user satisfaction. They address residual bugs, resolve newly reported issues, and implement required changes. Operations teams support developer velocity by enabling self‑service tooling, standardising processes, and providing secure, compliant environments. Collaboration between development and operations encourages automation of repeatable tasks such as incident resolution, system updates, and infrastructure scaling, improving consistency and monitoring. As automation increases, X‑as‑code practices become standard, requiring operational code to be versioned, secured, and maintained. Tools like Vagrant and Ansible exemplify this shift toward codified, traceable operations.

### The CALMS Framework

The CALMS Framework evaluates an organisation’s readiness to adopt DevOps by examining five pillars: Culture, Automation, Lean, Measurement, and Sharing.
DevOps requires a significant cultural shift, replacing traditional waterfall releases with smaller, iterative tasks delivered through collaborative sprints. This shift must be embraced across development, QA, product management, and operations. Automation is central to DevOps maturity, ensuring reliable, repeatable integration and delivery. As teams progress, automation extends to configuration‑as‑code, enabling environment‑specific builds and reducing manual error.

Lean principles encourage delivering minimal, functional versions early, allowing continuous improvement and user feedback to shape future development. Measurement provides essential insight into process effectiveness, enabling teams to refine workflows and sustain improvement. Finally, Sharing emphasises collective responsibility across all teams. When development and operations collaborate, knowledge, tooling, and accountability are distributed, resulting in more resilient systems and higher‑quality outcomes.

### DevOps Metrics

DevOps relies on clear, actionable metrics to understand performance, identify bottlenecks, and support collaboration between engineering and security teams. These measures help determine where processes require refinement and provide evidence for improvement over time. Core metrics include Mean Time to Production (MTTP), which tracks how long newly committed code takes to reach a deployed state, and Deployment Frequency, which reflects how often production releases occur. Deployment Speed (time for a release to be deployed in production) is combined with Deployment Frequency to measure Deployment Agility.

Quality‑focused metrics include the Change Failure Rate, measuring how often production releases require remediation, and Mean Time to Recovery (MTTR), which evaluates how quickly teams restore service after an incident. Automation, small batch work, and continuous feedback loops help improve these metrics by reducing errors and accelerating recovery.

Understanding how different teams perceive risk is essential for DevSecOps adoption. Security teams view risk through vulnerabilities and impact, whereas DevOps teams focus on production stability and failure rates. Aligning these perspectives builds empathy, strengthens collaboration, and supports the introduction of security controls without hindering delivery performance.

## The Secure Software Development Cycle (S-SDLC)

Within the SDLC, testing is introduced very late in the cycle. This makes bugs and flaws more expensive and time-consuming to fix. The S-SDLC model aims to circumvent this by introducing security at every stage of the SDLC. By detecting and resolving vulnerabilities early, costs are reduced and security awareness at an organisational level is increased.

### Implementing a Secure SDLC

A Secure SDLC introduces security activities into every phase of software development. Security requirements, testing and assurance evolve alongside functional design. Effective implementation begins with understanding the organisation’s security posture. This involves performing a gap analysis to identify existing policies and procedures, establishing Software Security Initiatives with measurable goals, formalising processes so that teams can adopt them gradually, and investing in early security training and tooling to support operational use.

Once the posture is understood, security processes are integrated into the SDLC. A Risk Assessment identifies security considerations during planning and requirements gathering, which promotes security by design. Threat Modelling follows during the design phase and focuses on what must not occur, ensuring safeguards are defined. During development, Code Scanning and Review detects vulnerabilities early through manual and automated testing. In later stages, Security Assessments such as vulnerability assessments and penetration testing validate security controls and identify exploitable flaws.

Understanding how teams perceive risk helps DevSecOps engineers build empathy and secure buy‑in. This alignment supports the introduction of security controls without hindering delivery performance or operational stability.

#### Risk Assessments

A Risk Assessment evaluates the likelihood and impact of threats being exploited within a system. It is a core component of secure software development because vulnerabilities, design flaws and poorly reviewed code increase the probability of harmful outcomes. Risk management within the SDLC aims to identify, reduce or eliminate these risks through appropriate controls.

The process begins by assuming the software will be attacked and analysing factors that motivate a threat actor. These include the value of the data handled by the system, the security maturity of dependent services, the scale of software distribution and the expectations of clients. Organisations must define an acceptable level of risk and communicate trade‑offs clearly. For example, the financial impact of data loss may outweigh the cost of fixing security defects, and reputational damage may exceed both.

Risk evaluation considers worst‑case scenarios, the value of assets at risk, the complexity of potential attacks, the number of users affected and the accessibility of the target. High‑impact or easily exploitable risks require mitigation. Understanding whether an attacker can access production systems or only isolated environments is essential for prioritisation.

Different assessment methods exist. A Qualitative Risk Assessment classifies risk as low, medium or high using the relationship between severity and likelihood. A Quantitative Risk Assessment uses numerical values, often based on business criticality, to calculate exposure. Metrics such as Annual Rate of Occurrence and Annual Loss Expectancy help determine whether security controls are cost‑effective.

Qualitative Risk Assessment Formula: Risk = Severity x Likelihood

Risk assessments are most effective early in the SDLC, where they guide design decisions and support security‑by‑design practices.

#### Threat Modelling

Threat Modelling is a structured technique used during the design phase of the SDLC to identify potential security threats before any code is written. It focuses on understanding how an attacker might compromise a system and prioritising safeguards that protect assets identified as high‑value or high‑risk during the risk assessment stage. Introducing threat modelling early allows teams to detect design issues before implementation, reducing remediation costs and strengthening security by design.

Different methodologies exist, each suited to specific organisational needs. STRIDE is a widely adopted model developed by Microsoft. It evaluates system design using data‑flow diagrams and categorises threats into Spoofing, Tampering, Repudiation, Information Disclosure, Denial of Service and Elevation of Privilege. STRIDE aligns with the confidentiality, integrity and availability principles and helps engineers answer the question: “What could go wrong with this system”.

DREAD provides a scoring‑based approach that ranks threats according to Damage, Reproducibility, Exploitability, Affected Users and Discoverability. Each factor is scored from 0 to 10, allowing teams to prioritise threats based on severity and likelihood.

PASTA offers a more strategic, business‑aligned methodology. It consists of seven stages, including defining objectives, mapping technical scope, analysing trust boundaries, identifying threats, assessing vulnerabilities, modelling attack paths and evaluating risk impact. PASTA is particularly effective for organisations that require alignment between technical risks and business goals.

Threat modelling complements risk assessment by focusing on what must not occur within a system. When integrated consistently, it provides early visibility of weaknesses, supports informed design decisions and strengthens the overall security posture of the software.

### Security Assessments

Security assessments are a key component of a secure software development lifecycle. They evaluate systems, applications and infrastructure for weaknesses, misconfigurations and potential attack paths. Although security considerations should be present throughout development, full assessments are typically performed once a system is complete and deployed, allowing testers to examine the application from an end‑to‑end perspective. Two primary assessment types are used: vulnerability assessments and penetration testing.

#### Vulnerability Assessment

A vulnerability assessment identifies potential weaknesses without attempting to exploit them. Automated scanners analyse systems, services and network assets to detect outdated software versions, exposed ports, insecure configurations and known vulnerabilities. Findings are usually categorised by severity, enabling teams to prioritise remediation. Vulnerability assessments are efficient, cost‑effective and suitable for organisations that need broad visibility of potential risks. However, they do not determine whether a vulnerability is practically exploitable, and they may produce large volumes of findings that require further investigation. Examples of tools: are OpenVAS, Nessus (Tenable), and ISS Scanner.

#### Penetration Testing

Penetration testing extends vulnerability assessment by validating and exploiting identified weaknesses. Testers simulate real‑world attacks to understand how vulnerabilities could be used to compromise systems, escalate privileges or access sensitive data. This provides a realistic view of risk by demonstrating actual impact rather than theoretical severity. Penetration tests produce detailed reports that include exploitation steps, risk evaluation and recommended countermeasures. They require specialist expertise, careful planning and greater financial investment, making them more resource‑intensive than automated scanning.

####

| Assessment Type | Purpose | Strengths | Limitations |
| --- | --- | --- | --- |
| Vulnerability Assessment | Identify potential weaknesses across systems and services | Fast, automated, cost‑effective; broad coverage | Does not validate exploitability; may generate large volumes of findings; low‑risk issues may contribute to larger attack chains |
| Penetration Testing | Validate and exploit vulnerabilities to demonstrate real‑world impact | Provides realistic risk insight; shows how attackers could behave; produces detailed remediation guidance | Expensive; time‑consuming; requires specialist expertise and planning |

#### Choosing an Approach

Both assessment types serve different purposes. Vulnerability assessments provide rapid visibility and are suitable for regular scanning, while penetration testing offers deeper insight into real‑world exploitability and business impact. Mature security programmes typically combine both approaches to ensure comprehensive coverage. Automated scanning identifies weaknesses early, while targeted manual testing validates critical risks and supports effective prioritisation.

## SSDLC Methodologies

Secure Software Development Lifecycle (SSDLC) methodologies provide structured approaches for embedding security throughout software development. Regardless of whether a team follows Agile, DevOps, Waterfall or another model, two principles remain essential: build with security in mind and introduce security‑focused testing at every stage. Several recognised methodologies support these aims, including Microsoft’s Security Development Lifecycle (SDL), the OWASP Secure SDLC (S‑SDLC) and maturity‑based models such as SAMM and BSIMM.

### Microsoft SDL

Microsoft SDL is a comprehensive framework built around mandatory security activities aligned with traditional SDLC phases. It emphasises secure design, secure defaults, secure deployment and effective communication. SDL requires teams to define and maintain security requirements, perform threat modelling, establish design requirements, enforce cryptography standards and manage risks associated with third‑party components. It also mandates the use of approved tools, security testing techniques such as SAST, DAST and IAST, and formal security assessments. A standardised incident response process ensures organisations can react effectively to emerging threats. SDL is structured, prescriptive and designed to ensure consistent security practices across engineering teams.

### OWASP S‑SDLC

OWASP’s S‑SDLC methodology focuses on building “security quality gates” throughout the development pipeline. It adopts an Agile Security approach, where dedicated sprints address specific security concerns such as authentication, authorisation, input validation and injection risks. These gates ensure that security tasks are treated as core development activities. The methodology is heavily influenced by the Software Assurance Maturity Model (SAMM), which helps organisations evaluate their current security posture, define objectives, measure progress and build a tailored security programme. BSIMM complements this by providing real‑world benchmarking across hundreds of organisations, helping teams understand how their practices compare to industry peers.

### Comparison Table

| Methodology | Focus | Strengths | Limitations |
| --- | --- | --- | --- |
| Microsoft SDL | Mandatory security activities across SDLC phases | Strong structure; comprehensive practices; mature tooling | Can be heavy‑weight; requires significant organisational effort |
| OWASP S‑SDLC | Security gates and Agile Security sprints | Flexible; integrates well with Agile; supports iterative improvement | Requires disciplined sprint planning; maturity varies by team |
| SAMM | Maturity‑based evaluation and improvement | Tailored to organisational risk; strong measurement and governance | Does not prescribe specific technical controls |
| BSIMM | Benchmarking against industry practices | Real‑world insight; useful for posture comparison | Descriptive rather than prescriptive; does not define required actions |

### Choosing an Approach

Organisations often combine elements from multiple SSDLC methodologies. SDL provides structure and mandatory activities, OWASP S‑SDLC offers Agile‑friendly security integration, SAMM supports maturity‑based improvement and BSIMM provides benchmarking. Together, these approaches help teams embed security deeply into development processes, improve consistency and ensure that security evolves alongside functional requirements.
