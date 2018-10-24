#!/bin/bash
rm triangle_time.txt
for i in {10..200..10};
do
	echo $i
	/usr/bin/time -f "%U" --append --output="triangle_time.txt" ./triangle $i > /dev/null
done
