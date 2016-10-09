#!/bin/bash

. ./TestConfig.sh

OpcUaClient \
 -Command Connect 	-Session "TestSession" -EndpointUrl ${EndpointUrl} \
 -Command WriteH	-NodeId "ns=1;i=210" \
			-Value "UInt32:100" -SourceTimestamp "20161010T000000" \
                        -Value "UInt32:200" -SourceTimestamp "20161010T000001" \
			-Value "UInt32:300" -SourceTimestamp "20161010T000002" \
			-Value "UInt32:400" -SourceTimestamp "20161010T000003" \
			-Value "UInt32:500" -SourceTimestamp "20161010T000004" \
 -Command Disconnect 
