# SDLC and SSDLC Notes

These are the raw notes on the Software Development Lifecycle/Secure Software Development Lifecycle (SDLC/SSDLC) that I will process into a section of the assignment.

## Software Development Lifecycle

The Software Development Lifecycle (SDLC) is a structured framework for building software efficiently and predictably. It divides development into measurable phases to improve quality, reduce cost, and ensure projects meet user expectations. Typical stages include Planning, Requirements, Design, Development, Testing, Deployment, and Maintenance.  

The SDLC standardises tasks within each phase, enabling monitoring, repeatability, and continuous improvement. Organisations may merge or split phases depending on their chosen model, such as Waterfall, Agile, or DevOps.

### Phases

#### 1. Planning

The planning phase defines the system’s purpose, scope, and feasibility. It establishes project boundaries, identifies the core problem, and outlines the resources required for development. Effective planning enables early detection of issues that could hinder progress and ensures the project remains aligned with its intended goals. This stage is essential when working to fixed market deadlines, as it provides a structured foundation for scheduling, cost estimation, and resource allocation.

#### 2. Requirements Definition

Requirements definition involves gathering all functional and technical needs of the proposed system. This includes listing prototype requirements, evaluating alternatives, and analysing end‑user expectations. Clear requirements determine what the application must achieve and what resources are necessary to build it. Developers often formalise this information in a Software Requirements Specification (SRS), ensuring the system’s purpose, features, and dependencies (such as hardware for specialised manufacturing software) are fully understood before design begins.

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
