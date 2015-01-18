#!/bin/bash

if [ $# -lt 1 ]; then
	echo "Usage $0 VM_number"
	exit 1
fi

INTERFACE="h$1-eth0"

ifconfig $INTERFACE mtu 1500
ethtool -K $INTERFACE gso off; ethtool -K $INTERFACE tso off
tc qdisc add dev $INTERFACE root handle 1: tbf rate 100Mbit burst 10000 latency 10ms
tc qdisc add dev $INTERFACE parent 1:1 handle 10: netem delay 5ms

memcached -u mininet &
echo "All configured"
