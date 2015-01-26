#!/bin/bash

cd ./sys-rep-benchs.results-1/res/blender_2.49/

for num in $(seq 1 1 13)
do
	find ./$num/*/ -name "*.ok" | while read; do
		x=${REPLY%.*}
		echo $x >> ../../../data.txt
		grep "MAKE DURATION" $x | sed "s/ MAKE DURATION//" | sed "s/.* //" >> ../../../data.txt
	done
done

