#!/bin/bash


# Generate 100B value
for (( c=1; c<=10; c++ )); do
	ten=`echo -n "1234567890"`
	suta=$suta$ten
done
echo $suta


# Generate 1KB value
for (( c=1; c<=10; c++ )); do
	oneKB=$oneKB$suta
done


# Generate 10KB value
for (( c=1; c<=100; c++ )); do
	tenK=$tenK$suta
done


# Generate 100KB value
for (( c=1; c<=10; c++ )); do
	sutaK=$sutaK$tenK
done


# Generate 1MB value
for (( c=1; c<=10; c++ )); do
	oneM=$oneM$sutaK
done


rm -f hundreds.in tenks.in hundredKs.in oneM.in

# Write 1M of data using the 4 types of chunk
for (( c=1; c<=10000; c++ )); do
	echo "put key_4_$c $suta" >> hundreds.in
done

for (( c=1; c<=1000; c++ )); do
	echo "put key_6_$c $oneKB" >> oneKBs.in
done

for (( c=1; c<=100; c++ )); do
	echo "put key_1_$c $tenK" >> tenks.in
done

for (( c=1; c<=10; c++ )); do
	echo "put key_2_$c $sutaK" >> hundredKs.in
done

echo "put key_3 $oneM" >> oneM.in
