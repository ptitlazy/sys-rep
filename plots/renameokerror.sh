#!/bin/bash

cd sys-rep-benchs-blender_2.59-2/res/blender_2.59/

find ./*/*/ -name "*.ok" | while read; do
	rm $REPLY
done

find ./*/*/ -name "*.erreur" | while read; do
	x=${REPLY%.*}
	mv $REPLY $x".ok"
done
