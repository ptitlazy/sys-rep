#!/bin/bash

cd ./sys-rep-benchs.results-1/res/blender_2.49/
rm ../../../workerprocess.R
#début. Variable x.
echo -en "x<-c(" >> ../../../workerprocess.R
for num in $(seq 1 1 13)
do
	echo -en $num"," >> ../../../workerprocess.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../workerprocess.R)
	rm ../../../workerprocess.R
	echo -en $a >> ../../../workerprocess.R

#fin variable x et début y
echo -en ");\ny<-c(" >> ../../../workerprocess.R

for num in $(seq 1 1 13)
do
	
	#core de y : moyennes.
	echo -en "mean(c(" >> ../../../workerprocess.R
	find ./$num/$num/ -name "*.ok" | while read; do
		x=${REPLY%.*}
#		echo $x
		y=$(grep "MAKE DURATION" $x | sed "s/ MAKE DURATION//" | sed "s/.* //")
		echo -en $y"," >> ../../../workerprocess.R
	done

	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../workerprocess.R)
	rm ../../../workerprocess.R
	echo -en $a >> ../../../workerprocess.R

	#fin élément core : passage à la moyenne suivante.
	echo -en "))," >> ../../../workerprocess.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../workerprocess.R)
	rm ../../../workerprocess.R
	echo -en $a >> ../../../workerprocess.R

#conclusion.
echo -en ");\n" >> ../../../workerprocess.R
echo "" >> ../../../workerprocess.R
#FIN PARTIE 1

#PARTIE 2 : MULTICORE
#début. Variable x.
echo -en "x2<-c(" >> ../../../workerprocess.R
for num in $(seq 1 1 13)
do
	echo -en $num"," >> ../../../workerprocess.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../workerprocess.R)
	rm ../../../workerprocess.R
	echo -en $a >> ../../../workerprocess.R

#fin variable x et début y
echo -en ");\ny2<-c(" >> ../../../workerprocess.R

for num in $(seq 1 1 13)
do
	#core de y : moyennes.
	echo -en "mean(c(" >> ../../../workerprocess.R
	find ./$num/$(($num*4))/ -name "*.ok" | while read; do
		x=${REPLY%.*}
#		echo $x
		y=$(grep "MAKE DURATION" $x | sed "s/ MAKE DURATION//" | sed "s/.* //")
		echo -en $y"," >> ../../../workerprocess.R
	done

	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../workerprocess.R)
	rm ../../../workerprocess.R
	echo -en $a >> ../../../workerprocess.R
	
	#fin élément core : passage à la moyenne suivante.
	echo -en "))," >> ../../../workerprocess.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../workerprocess.R)
	rm ../../../workerprocess.R
	echo -en $a >> ../../../workerprocess.R
#conclusion.
echo -en ");\n" >> ../../../workerprocess.R

