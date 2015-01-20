#ifndef WORKER_H
#define WORKER_H

#include <string>
#include <mpi.h>
#include <vector>

void worker(int rang);

std::string recv_string(MPI_Status status);

void executer(std::string cmd);

typedef struct {
    std::string target;
    std::string cmd;
    std::vector<std::string> liste_dep;
} workerRule;

workerRule deserialize(std::string s);

#endif