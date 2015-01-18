#!/bin/bash

rm -f distrib.in

for (( c=1; c<=1000; c++ )); do
	echo "put test_$(($c * $c)) value" >> distrib.in
done
echo "stat" >> distrib.in

./dht_client < distrib.in
