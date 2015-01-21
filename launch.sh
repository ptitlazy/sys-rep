#!/bin/bash

ORIGINAL_DIR=$(pwd)
WORKING_DIR=$(mktemp -d)
cp -R ./* "$WORKING_DIR"
cd "$WORKING_DIR"
cp /user/2/darricat/hosts hosts

# Get hostnames list
echo 'Construct workers list...'
echo 'ensipcserveur.imag.fr slots=1 max-slots=1' > hosts.clean
taktuk -o output='"$line\n"' -o status -o error -o connector -o taktuk -o info -s -f hosts broadcast exec { hostname } | grep -v "Connection failed" >> hosts_workers.clean
cat hosts_workers.clean >> hosts.clean

# Deploying
echo "Deploy job..."
taktuk -s -f hosts_workers.clean broadcast exec { rm -rf /tmp/ }
taktuk -s -f hosts_workers.clean broadcast put [ "$WORKING_DIR" ] [ "$WORKING_DIR" ]

# Execution
echo 'Execute job...'
mpirun --hostfile hosts.clean sys_rep Makefile "$1"

cp "$1" "$ORIGINAL_DIR"/"$1"

rm -Rf "$WORKING_DIR"
