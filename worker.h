#ifndef WORKER_H
#define WORKER_H

#include <string>
#include <mpi.h>

void worker(int rang);

std::string recv_string(MPI_Status status);

void executer(std::string cmd);


#endif