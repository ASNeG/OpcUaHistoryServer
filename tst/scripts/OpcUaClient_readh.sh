#!/bin/bash

. ./TestConfig.sh

OpcUaClient \
 -Command Connect 	-Session "TestSession" -EndpointUrl ${EndpointUrl} \
 -Command ReadH		-NodeId "ns=1;i=205" -StartTime "20001010T000000" -EndTime "23000101T000000" -TimestampsToReturn "Source" \
 -Command Disconnect 
