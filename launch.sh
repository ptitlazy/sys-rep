#!/bin/bash

WORKING_DIR=$(mktemp -d)
cp -R ./* "$WORKING_DIR"
cd "$WORKING_DIR"

# Get hostnames list
echo 'ensipcserveur.imag.fr slots=1 max-slots=1' > hosts.clean
taktuk -o output='"$line\n"' -o status -o error -o connector -o taktuk -o info -s -f hosts broadcast exec { hostname } | grep -v "Connection failed" >> hosts.clean

# Deploying
taktuk -s -f hosts.clean broadcast put [ "$WORKING_DIR" ] [ "$WORKING_DIR" ]

# Execution
# mpirun --hostfile hosts.clean sys-rep Makefile $1
