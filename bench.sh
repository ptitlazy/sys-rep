#!/bin/bash

# Variables
BENCH_DIR="/user/2/darricat/sys-rep-benchs"
MAKEFILES="premier blender_2.49 blender_2.59"
NB_ITERATIONS=10
BASE_DIR=/user/2/darricat/Documents/3A/sys-rep/makefiles
NB_MAX_WORKERS=30

# Initialisation
rm -rf "$BENCH_DIR" 2>/dev/null
mkdir $BENCH_DIR

LIST_START=$(date +%s%N)
	# Get hostnames list
	echo 'Construct available workers list...'
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_START LIST START"
	taktuk -o output='"$line\n"' -o status -o error -o connector -o taktuk -o info -s -f hosts broadcast exec { hostname } | grep -v "Connection failed" > "$BENCH_DIR/hosts_workers.clean"

	NB_MAX_WORKERS_REAL=$(wc -l "$BENCH_DIR/hosts_workers.clean")

	echo "$NB_MAX_WORKERS_REAL workers available"
LIST_END=$(date +%s%N)
LIST_DURATION=$(($LIST_END - $LIST_START))
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_END LIST END"
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_DURATION LIST DURATION"

# Cleaning /tmp everywhere
echo 'Cleaning /tmp...'
taktuk -o output -o status -o error -o connector -o taktuk -o info -s -f "$BENCH_DIR/hosts_workers.clean" broadcast exec { rm -rf /tmp/ 2>/dev/null }
rm /tmp/ 2>/dev/null

if [[ $NB_MAX_WORKERS_REAL > $NB_MAX_WORKERS ]]
then
	NB_MAX_WORKERS=$NB_MAX_WORKERS_REAL
fi

# Benchs
echo 'Starting Benchs...'
mkdir "$BENCH_DIR"/res

for MAKEFILE in $MAKEFILES
do
	mkdir "$BENCH_DIR"/res/"$MAKEFILE"
	cd "$BASE_DIR/$MAKEFILE"

	for NB_WORKERS in {1..$NB_MAX_WORKERS}
	do
		mkdir "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"

		# 1 worker par process
		NB_PROCESS_1=$(($NB_WORKERS))

		# 4 worker par process
		NB_PROCESS_2=$(($NB_WORKERS * 4))

		for NB_PROCESS in $NB_PROCESS_1 $NB_PROCESS_2
		do
			mkdir "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"/"$NB_PROCESS"

			for ITERATION in {1 .. $NB_ITERATIONS}
			do
				mkdir "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"/"$NB_PROCESS"/"$ITERATION"

				echo "Iteration $ITERATION for $MAKEFILE with $NB_PROCESS on $NB_WORKERS"

				WORKING_DIR=$(mktemp -d)
				cp -R ./* "$WORKING_DIR"
				cd "$WORKING_DIR"

				echo 'ensipcserveur.imag.fr slots=1 max-slots=1' > hosts.clean
				cat "$BENCH_DIR/hosts_workers.clean" | head -n$NB_WORKERS >> hosts_workers.clean
				cat hosts_workers.clean >> hosts.clean

				# Deploying
				DELP_START=$(date +%s%N)
					echo -e  "\033[22;44m\033[37m BCH \033[0m $DELP_START DEPL START"
					echo "Deploy job..."
				#	taktuk -s -f hosts_workers.clean broadcast put [ "$WORKING_DIR" ] [ "$WORKING_DIR" ]
				DEPL_END=$(date +%s%N)
				DEPL_DURATION=$(($DEPL_END - $DEPL_START))
					echo -e  "\033[22;44m\033[37m BCH \033[0m $DEPL_END DEPL END"
					echo -e  "\033[22;44m\033[37m BCH \033[0m $DEPL_DURATION DEPL DURATION"

				# Execution
				echo 'Execute job...'
				MPI_START=$(date +%s%N)
					echo -e  "\033[22;44m\033[37m BCH \033[0m $MPI_START MPIRUN START"
				#	mpirun -n $NB_WORKERS --map-by node --hostfile hosts.clean sys_rep Makefile "$1"
				MPI_END=$(date +%s%N)
				MPI_DURATION=$(($MPI_END - $MPI_START))
					echo -e  "\033[22;44m\033[37m BCH \033[0m $MPI_END MPIRUN END"
					echo -e  "\033[22;44m\033[37m BCH \033[0m $MPI_DURATION MPIRUN DURATION"

				rm -Rf "$WORKING_DIR"
			done
		done
	done
done
