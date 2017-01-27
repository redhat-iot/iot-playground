#!/bin/sh
# This script can be run for setting up the cloud components in a playground
# It takes 4 arguments: 
# - the username doing the deployment
# - the project name for deploying the infrastructure
# - path to a certificate private key (PEM)
# - path to a certificate public key (PEM)

# Step 1: Setup EnMasse
curl -o /tmp/enmasse-deploy.sh https://raw.githubusercontent.com/EnMasseProject/openshift-configuration/master/scripts/enmasse-deploy.sh 
bash /tmp/enmasse-deploy.sh -u $1 -p $2 -k $3 -s $4
