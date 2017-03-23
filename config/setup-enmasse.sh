#!/bin/sh

# Step 1: Generate cert
#openssl req -new -x509 -batch -nodes -out server-cert.pem -keyout server-key.pem

# Step 2: Deploy EnMasse
bash enmasse-deploy.sh -u iot -p enmasse -s server-cert.pem -k server-key.pem -c https://awesome.iot-playground.org -t tls-enmasse-template.yaml
