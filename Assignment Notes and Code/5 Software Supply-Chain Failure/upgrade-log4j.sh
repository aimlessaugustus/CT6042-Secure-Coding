#!/usr/bin/env bash
set -euo pipefail
MODS="log4j-api log4j-core log4j-slf4j-impl log4j-1.2-api log4j-web log4j-layout-template-json"
LAB="$HOME/log4shell-lab"
VH="$LAB/vulhub/log4j/CVE-2021-44228"

# 1) Download the 2.17.1 jars
mkdir -p "$LAB/log4j-2.17.1" && cd "$LAB/log4j-2.17.1"
for m in $MODS; do
  curl -sSfLO "https://repo1.maven.org/maven2/org/apache/logging/log4j/$m/2.17.1/$m-2.17.1.jar"
done
echo "Downloaded:"; ls -1 "$LAB/log4j-2.17.1"

# 2) Swap them into the container
cd "$VH"
for m in $MODS; do
  docker compose exec --user root solr rm -f "/opt/solr/server/lib/ext/$m-2.14.1.jar"
  docker compose cp "$LAB/log4j-2.17.1/$m-2.17.1.jar" \
    "solr:/opt/solr/server/lib/ext/$m-2.17.1.jar"
done