#!/bin/bash

function pause() {
	read -p "$*"
}

# Push values
rm -f avail.in
for (( c=1; c<=100; c++ )); do
	echo "put test_c_$(($c * $c)) value" >> avail.in
done
echo "stat" >> avail.in
./dht_client < avail.in

pause 'Press [Enter] key to continue. Deactivate link'

# Perform gets and measure
rm -f avail.in
for (( c=1; c<=100; c++ )); do
	echo "get test_c_$(($c * $c))" >> avail.in
done
echo "stat" >> avail.in

./dht_client < avail.in
pause 'Press [Enter] key to continue. Deactivate link'
