#!/bin/bash

# Variables
HOST_FILE="/user/2/darricat/hosts"
BENCH_DIR="/user/2/darricat/sys-rep-benchs"
MAKEFILES="1 2 3"
NB_ITERATIONS=10
BASE_DIR="/user/2/darricat/Documents/3A/sys-rep/makefiles"
NB_MAX_WORKERS=40

# premier
CMD[1]="premier"
CIBLE[1]="list.txt"
BASE_TIME[1]=$((22*60)) # 22 minutes

# blender_2.49
CIBLE[2]="cube.mpg"
CMD[2]="blender_2.49"
BASE_TIME[2]=$((2))

# blender_2.59
CMD[3]="blender_2.59"
CIBLE[3]="out.avi"
BASE_TIME[3]=$((2))

# Initialisation
rm -rf "$BENCH_DIR" 2>/dev/null
mkdir $BENCH_DIR

LIST_START=$(date +%s%N)
	# Get hostnames list
	echo 'Construct available workers list...'
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_START LIST START"
	taktuk -o output='"$line\n"' -o status -o error -o connector -o taktuk -o info -s -f "$HOST_FILE" broadcast exec { hostname } | grep -v "Connection failed" > "$BENCH_DIR/hosts_workers.clean"

	NB_MAX_WORKERS_REAL=$(cat "$BENCH_DIR/hosts_workers.clean" | wc -l)

	echo "$NB_MAX_WORKERS_REAL workers available"
LIST_END=$(date +%s%N)
LIST_DURATION=$(($LIST_END - $LIST_START))
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_END LIST END"
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_DURATION LIST DURATION"

# Cleaning /tmp everywhere
echo 'Cleaning /tmp...'
taktuk -o output -o status -o error -o connector -o taktuk -o info -s -f "$BENCH_DIR/hosts_workers.clean" broadcast exec { rm -rf /tmp/ 2>/dev/null }
rm /tmp/ 2>/dev/null

if [[ $NB_MAX_WORKERS_REAL < $NB_MAX_WORKERS ]]
then
	NB_MAX_WORKERS=$NB_MAX_WORKERS_REAL
fi

# Benchs
echo 'Starting Benchs...'
mkdir "$BENCH_DIR"/res

for JOB in $MAKEFILES
do
	MAKEFILE=${CMD[$JOB]}
	CIBLE=${CIBLE[$JOB]}
	BASE_TIME=${BASE_TIME[$JOB]}

	mkdir "$BENCH_DIR"/res/"$MAKEFILE"

	for NB_WORKERS in $(seq 1 $NB_MAX_WORKERS)
	do
		mkdir "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"

		# 1 worker par process
		NB_PROCESS_1=$(($NB_WORKERS))

		# 4 worker par process
		NB_PROCESS_2=$(($NB_WORKERS * 4))

		for NB_PROCESS in $NB_PROCESS_1 $NB_PROCESS_2
		do
			mkdir "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"/"$NB_PROCESS"
			NB_PROCESS_REAL=$(($NB_PROCESS_REAL + 1))

			for ITERATION in $(seq 1 $NB_ITERATIONS)
			do
				# mkdir "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"/"$NB_PROCESS"/"$ITERATION"

				echo "Iteration $ITERATION for $MAKEFILE avec $NB_PROCESS process sur $NB_WORKERS worker(s)"

				WORKING_DIR=$(mktemp -d)
				cp -R "$BASE_DIR/$MAKEFILE"/* "$WORKING_DIR"
				cd "$WORKING_DIR"

				echo 'ensipcserveur.imag.fr slots=1 max-slots=1' > hosts.clean
				cat "$BENCH_DIR/hosts_workers.clean" | head -n$NB_WORKERS >> hosts_workers.clean
				cat hosts_workers.clean >> hosts.clean
				cat hosts.clean

				# Deploying
				DEPL_START=$(date +%s%N)
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
					# kill après (BASE_TIME / NB_PROCESS) * 2
					END_TIME=$(((BASE_TIME / NB_PROCESS) * 2))
					( cmdpid=$BASHPID; (sleep $END_TIME; kill $cmdpid >/dev/null 2>&1 && touch "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"/"$NB_PROCESS"/"$ITERATION".erreur || touch "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"/"$NB_PROCESS"/"$ITERATION".ok) & (exec mpirun -n $NB_PROCESS_REAL --map-by node --hostfile hosts.clean sys_rep Makefile "$CIBLE" > "$BENCH_DIR"/res/"$MAKEFILE"/"$NB_WORKERS"/"$NB_PROCESS"/"$ITERATION") )
				MPI_END=$(date +%s%N)
				MPI_DURATION=$(($MPI_END - $MPI_START))
					echo -e  "\033[22;44m\033[37m BCH \033[0m $MPI_END MPIRUN END"
					echo -e  "\033[22;44m\033[37m BCH \033[0m $MPI_DURATION MPIRUN DURATION"

				cd "$BENCH_DIR"
				rm -Rf "$WORKING_DIR"
			done
		done
	done
done
