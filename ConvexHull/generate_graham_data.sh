#!/bin/bash
rm graham_time.txt
for i in {100..200000..100};
do
	echo $i
	/usr/bin/time -f "%U" --append --output="graham_time.txt" ./graham $i > /dev/null
done
