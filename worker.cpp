#include "worker.h"
#include <mpi.h>

void worker(int rang) {
    bool istasks = true;
    int res = 0;
    std::string target;

    while (istasks) {
        //Recv target
            //If stop, quit
            //Else,
                //Recv dependencies and cmd
                //Do the job
                //Send the response

    }
}