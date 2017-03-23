#!/bin/sh
curl -X PUT --data-binary @addresses.json -H "content-type: application/json" http://api.awesome.iot-playground.org/v3/address
