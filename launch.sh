#!/bin/bash

LIST_START=$(date +%s%N)
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_START LIST START"

	NB_MACHINES=15
	if [[ $2 ]]
	then
		NB_MACHINES=$2
	fi

	NB_WORKERS=$(($NB_MACHINES * 4))
	if [[ $3 ]]
	then
		NB_WORKERS=$2
	fi

	NB_WORKERS=$(($NB_WORKERS + 1))

	ORIGINAL_DIR=$(pwd)
	WORKING_DIR=$(mktemp -d)

	cp -R ./* "$WORKING_DIR"
	cd "$WORKING_DIR"
	cp ~/hosts hosts

	# Get hostnames list
	echo 'Construct workers list...'
	echo 'ensipcserveur.imag.fr slots=1 max-slots=1' > hosts.clean
	taktuk -o output='"$line\n"' -o status -o error -o connector -o taktuk -o info -s -f hosts broadcast exec { hostname } | grep -v "Connection failed" | head -n $NB_MACHINES >> hosts_workers.clean
	cat hosts_workers.clean >> hosts.clean

LIST_END=$(date +%s%N)
LIST_DURATION=$(($LIST_END - $LIST_START))
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_END LIST END"
	echo -e  "\033[22;44m\033[37m BCH \033[0m $LIST_DURATION LIST DURATION"


# Deploying
DELP_START=$(date +%s%N)
	echo -e  "\033[22;44m\033[37m BCH \033[0m $DELP_START DEPL START"
	echo "Deploy job..."
	taktuk -s -f hosts_workers.clean broadcast exec { rm -rf /tmp/ }
	taktuk -s -f hosts_workers.clean broadcast put [ "$WORKING_DIR" ] [ "$WORKING_DIR" ]
DEPL_END=$(date +%s%N)
DEPL_DURATION=$(($DEPL_END - $DEPL_START))
	echo -e  "\033[22;44m\033[37m BCH \033[0m $DEPL_END DEPL END"
	echo -e  "\033[22;44m\033[37m BCH \033[0m $DEPL_DURATION DEPL DURATION"

# Execution
echo 'Execute job...'
MPI_START=$(date +%s%N)
	echo -e  "\033[22;44m\033[37m BCH \033[0m $MPI_START MPIRUN START"
	mpirun -n $NB_WORKERS --map-by node --hostfile hosts.clean sys_rep Makefile "$1"
MPI_END=$(date +%s%N)
MPI_DURATION=$(($MPI_END - $MPI_START))
	echo -e  "\033[22;44m\033[37m BCH \033[0m $MPI_END MPIRUN END"
	echo -e  "\033[22;44m\033[37m BCH \033[0m $MPI_DURATION MPIRUN DURATION"

cp "$1" "$ORIGINAL_DIR"/"$1"

rm -Rf "$WORKING_DIR"
