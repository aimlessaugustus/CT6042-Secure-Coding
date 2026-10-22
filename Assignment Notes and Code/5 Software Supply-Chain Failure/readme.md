# Software Supply-Chain Dependency

## Running the Lab

This setup assumes a freshly installed Kali Linux VM. By following the installation instructions the artifacts from the report can be independently verified.

To begin with, run the following commands in the terminal on the Kali VM:

### To install the initial dependencies for testing

```bash
sudo apt update
sudo apt install -y docker.io docker-compose default-jdk maven git curl zip nmap netcat-traditional
sudo systemctl enable --now docker

# Run Docker without sudo for the rest of the lab example, then apply the group now:
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
