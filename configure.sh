#!/bin/bash

ifconfig lo mtu 1500
ethtool -K lo gso off; ethtool -K lo tso off
tc qdisc add dev lo root handle 1: tbf rate 100Mbit burst 10000 latency 10ms
tc qdisc add dev lo parent 1:1 handle 10: netem delay 5ms
