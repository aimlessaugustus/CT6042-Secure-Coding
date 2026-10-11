# Java Deserialisation

## CVE-2021-44228: Solar (Log4j)

### Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

CVE-2021-44228, colloqiually known as Solar, is a vulnerability that allows attackers to achieve remote code execution by exploiting insecure deserialisation in the Apache Log4j 2 library, a widely used logging library in Java applications. This enabled attackers to execute commands on targeted hosts, disrupting infrastructure and causing supply chain failiures.

Insecure deserialisation occurs when a web application trusts serialised data without taking steps to validate it's authenticity. In Java, object serialisation is facilitated through the `Serializable` interface, allowing objects to be converted into byte streams and vice versa for network communication and data persistence.

Deserialisation errors can be identified by either analyzing the source code of a Java application for use of the `Serializable` module, or by performing black box testing using a tool such as Ysoserial.

Object injection is a vulnerability that arises from insecure data deserialisation in web applications. It occurs when untrusted data is deserialised into an object, allowing attackers to manipulate the serialised data to execute arbitrary code, leading to serious security risks.

### Demonstration of Exploitability

### Identifying the Vulnerability Using Techniques and Tools

#### Ysoserial for Java

Ysoserial is a widely recognised exploitation tool specifically crafted to test the security of Java applications against serialisation vulnerabilities. It helps generate payloads that exploit these vulnerabilities, making it an essential tool for attackers and penetration testers who aim to assess and exploit applications that use Java serialisation.

To use Ysoserial, an attacker would typically generate a payload with a command such as java -jar ysoserial.jar [payload type] '[command to execute]', where [payload type] is the type of exploit and [command to execute] is the arbitrary command they wish to run on the target system. For example, using the CommonsCollections1 payload type might look like this: java -jar ysoserial.jar CommonsCollections1 'calc.exe'. This command generates a serialised object that will execute the specified command when deserialised by a vulnerable application. Ysoserial is available for download on GitHub.

### How to Mitigate the Vulnerability

### Retesting For Resilience

## Marking Scheme (For Reference)

| **Mark Range (%)** | **Knowledge & Understanding** | **Analysis** | **Reading & Research** | **Writing / Presentation Style** |
| --- | --- | --- | --- | --- |
| **80+** | Extraordinary and authoritative understanding of the five selected software errors, including root causes, exploit mechanisms, secure mitigations, verification, the Security Development Life Cycle and product risk. | Exceptional critical analysis of vulnerable and mitigated code. Exploitation, appropriate tool‑assisted testing and retesting are interpreted rigorously; security decisions, limitations and residual risks are evaluated convincingly. | Extensive, independent and critical research. Authoritative standards and high‑quality academic or technical sources are integrated throughout to substantiate technical claims, mitigations, lifecycle discussion and risk analysis. | Lively, precise and professional report with exceptional control of argument. Code, screenshots and tool results are clearly captioned, referenced and explained; appendices are complete; virtually error‑free. |
| **70–79** | Excellent and wide‑ranging understanding of the five software errors, their causes, exploitation, mitigation and retesting, with confident understanding of secure development lifecycle and risk principles. | Excellent and consistent analysis of practical evidence. Vulnerable and mitigated code, exploits, security‑tool findings and retests are interpreted critically, with well‑justified controls and thoughtful consideration of limitations and residual risk. | Excellent independent research using a wide range of relevant academic and authoritative technical sources. Evidence is selected critically, integrated effectively and used consistently to support claims. | Excellent, coherent and professional presentation with strong control of argument. Figures, code and test evidence are consistently captioned, referenced and discussed; source‑code appendices are complete; very few errors. |
