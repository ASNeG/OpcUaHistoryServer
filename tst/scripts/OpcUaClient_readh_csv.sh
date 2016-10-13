#!/bin/bash

. ./TestConfig.sh

OpcUaClient \
 -Command Connect 	-Session "TestSession" -EndpointUrl ${EndpointUrl} \
 -Command ReadH		-NodeId "ns=1;i=205" -StartTime "20001010T000000" -EndTime "23000101T000000" -TimestampsToReturn "Source" -MaxNumResultValuesPerNode 50 -MaxNumRequests 30 -csv "Value:Int16Array.csv"\
 -Command Disconnect 


OpcUaClient \
 -Command Connect 	-Session "TestSession" -EndpointUrl ${EndpointUrl} \
 -Command ReadH		-NodeId "ns=1;i=227" -StartTime "20001010T000000" -EndTime "23000101T000000" -TimestampsToReturn "Source" -MaxNumResultValuesPerNode 50 -MaxNumRequests 30 -csv "Value:LocalizedTextArray.csv"\
 -Command Disconnect 

OpcUaClient \
 -Command Connect 	-Session "TestSession" -EndpointUrl ${EndpointUrl} \
 -Command ReadH		-NodeId "ns=1;i=210" -StartTime "20001010T000000" -EndTime "23000101T000000" -TimestampsToReturn "Source" -MaxNumResultValuesPerNode 50 -MaxNumRequests 30 -csv "Value:UInt32.csv"\
 -Command Disconnect 
