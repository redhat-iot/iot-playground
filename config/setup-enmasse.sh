#!/bin/sh

# Step 1: Generate cert
#openssl req -new -x509 -batch -nodes -out server-cert.pem -keyout server-key.pem

# Step 2: Deploy EnMasse
curl -o /tmp/enmasse-deploy.sh https://github.com/EnMasseProject/enmasse/releases/download/0.5/enmasse-deploy.sh
bash /tmp/enmasse-deploy.sh -u iot -p enmasse -s server-cert.pem -k server-key.pem -c https://awesome.iot-playground.org -t https://github.com/EnMasseProject/enmasse/releases/download/0.5/tls-enmasse-template.yaml
