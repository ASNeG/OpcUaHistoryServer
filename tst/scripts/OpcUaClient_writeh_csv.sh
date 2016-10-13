#!/bin/bash

. ./TestConfig.sh

OpcUaClient \
 -Command Connect 	-Session "TestSession" -EndpointUrl ${EndpointUrl} \
 -Command WriteH	-NodeId "ns=1;i=210" -ValueType "UInt32" -CSV "data/Value:UInt32.csv" \
 -Command Disconnect 
