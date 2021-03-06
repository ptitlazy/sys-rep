cd ./sys-rep-benchs.results-1/res/blender_2.49/
rm ../../../networkusage.R
#début. Variable x.
echo -en "x<-c(" >> ../../../networkusage.R
for num in $(seq 1 1 13)
do
	echo -en $num"," >> ../../../networkusage.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../networkusage.R)
	rm ../../../networkusage.R
	echo -en $a >> ../../../networkusage.R

#fin variable x et début y
echo -en ");\ny<-c(" >> ../../../networkusage.R

for num in $(seq 1 1 13)
do
	
	#core de y : moyennes.
	echo -en "mean(c(" >> ../../../networkusage.R
	find ./$num/$num/ -name "*.ok" | while read; do
		x=${REPLY%.*}
#		echo $x
		grep "SEND" $x | sed "s/ SEND//" | sed "s/.* //" | while read; do
			y=$((y+REPLY))
			echo $y > var
		done
		cat var >> ../../../networkusage.R
		echo -en "," >> ../../../networkusage.R
		rm var
	done

	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../networkusage.R)
	rm ../../../networkusage.R
	echo -en $a >> ../../../networkusage.R

	#fin élément core : passage à la moyenne suivante.
	echo -en "))," >> ../../../networkusage.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../networkusage.R)
	rm ../../../networkusage.R
	echo -en $a >> ../../../networkusage.R

#conclusion.
echo -en ");\n" >> ../../../networkusage.R
echo "" >> ../../../networkusage.R

#FIN PARTIE1
#PARTIE 2
#début. Variable x.
echo -en "x2<-c(" >> ../../../networkusage.R
for num in $(seq 1 1 13)
do
	echo -en $num"," >> ../../../networkusage.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../networkusage.R)
	rm ../../../networkusage.R
	echo -en $a >> ../../../networkusage.R

#fin variable x et début y
echo -en ");\ny2<-c(" >> ../../../networkusage.R

for num in $(seq 1 1 13)
do
	
	#core de y : moyennes.
	echo -en "mean(c(" >> ../../../networkusage.R
	find ./$num/$(($num*4))/ -name "*.ok" | while read; do
		x=${REPLY%.*}
#		echo $x
		grep "SEND" $x | sed "s/ SEND//" | sed "s/.* //" | while read; do
			y=$((y+REPLY))
			echo $y > var
		done
		cat var >> ../../../networkusage.R
		echo -en "," >> ../../../networkusage.R
		rm var
	done

	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../networkusage.R)
	rm ../../../networkusage.R
	echo -en $a >> ../../../networkusage.R

	#fin élément core : passage à la moyenne suivante.
	echo -en "))," >> ../../../networkusage.R
done
	#Suppression de la dernière virgule #yolo
	a=$(sed '$ s/.$//' ../../../networkusage.R)
	rm ../../../networkusage.R
	echo -en $a >> ../../../networkusage.R

#conclusion.
echo -en ");\n" >> ../../../networkusage.R
echo "" >> ../../../networkusage.R
