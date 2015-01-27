#!/bin/bash

cd sys-rep-benchs-blender_2.59-2/res/blender_2.59/
rm ../../../WorPro2.59.R
#début. Variable x.
echo -en "x<-c(" >> ../../../WorPro2.59.R
for num in $(seq 1 1 13)
do
	echo -en $num"," >> ../../../WorPro2.59.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../WorPro2.59.R)
	rm ../../../WorPro2.59.R
	echo -en $a >> ../../../WorPro2.59.R

#fin variable x et début y
echo -en ");\ny<-c(" >> ../../../WorPro2.59.R

for num in $(seq 1 1 13)
do
	
	#core de y : moyennes.
	echo -en "mean(c(" >> ../../../WorPro2.59.R
	find ./$num/$num/ -name "*.ok" | while read; do
		x=${REPLY%.*}
#		echo $x
		y=$(grep "MAKE DURATION" $x | sed "s/ MAKE DURATION//" | sed "s/.* //")
		echo -en $y"," >> ../../../WorPro2.59.R
	done

	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../WorPro2.59.R)
	rm ../../../WorPro2.59.R
	echo -en $a >> ../../../WorPro2.59.R

	#fin élément core : passage à la moyenne suivante.
	echo -en "))," >> ../../../WorPro2.59.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../WorPro2.59.R)
	rm ../../../WorPro2.59.R
	echo -en $a >> ../../../WorPro2.59.R

#conclusion.
echo -en ");\n" >> ../../../WorPro2.59.R
echo "" >> ../../../WorPro2.59.R
#FIN PARTIE 1

#PARTIE 2 : MULTICORE
#début. Variable x.
echo -en "x2<-c(" >> ../../../WorPro2.59.R
for num in $(seq 1 1 13)
do
	echo -en $num"," >> ../../../WorPro2.59.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../WorPro2.59.R)
	rm ../../../WorPro2.59.R
	echo -en $a >> ../../../WorPro2.59.R

#fin variable x et début y
echo -en ");\ny2<-c(" >> ../../../WorPro2.59.R

for num in $(seq 1 1 13)
do
	#core de y : moyennes.
	echo -en "mean(c(" >> ../../../WorPro2.59.R
	find ./$num/$(($num*4))/ -name "*.ok" | while read; do
		x=${REPLY%.*}
#		echo $x
		y=$(grep "MAKE DURATION" $x | sed "s/ MAKE DURATION//" | sed "s/.* //")
		echo -en $y"," >> ../../../WorPro2.59.R
	done

	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../WorPro2.59.R)
	rm ../../../WorPro2.59.R
	echo -en $a >> ../../../WorPro2.59.R
	
	#fin élément core : passage à la moyenne suivante.
	echo -en "))," >> ../../../WorPro2.59.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../WorPro2.59.R)
	rm ../../../WorPro2.59.R
	echo -en $a >> ../../../WorPro2.59.R
#conclusion.
echo -en ");\n" >> ../../../WorPro2.59.R

