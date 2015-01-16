#include <mpi.h>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iomanip>

#include "tree.h"
#include "parser.h"

using namespace std;

int main(int argc, char **argv) {
    /*int taille, rang, hostlen;
    char hostname[MPI_MAX_PROCESSOR_NAME] = {};
    double message = rang + 123;
    int TAG = 123456;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &taille);
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);
    MPI_Get_processor_name(hostname, &hostlen);

    if (rang == 0) {
        MPI_Send(&message, 1, MPI_DOUBLE, (rang + 1) % taille, TAG, MPI_COMM_WORLD);
        MPI_Recv(&message, 1, MPI_DOUBLE, (taille - 1), TAG, MPI_COMM_WORLD, &status);
        printf("Message %g reçu ! %d parmi %d sur %s!\n", message, rang, taille, hostname);
    } else {
        MPI_Recv(&message, 1, MPI_DOUBLE, (rang - 1), TAG, MPI_COMM_WORLD, &status);
        printf("Message %g reçu ! %d parmi %d sur %s!\n", message, rang, taille, hostname);
        MPI_Send(&message, 1, MPI_DOUBLE, (rang + 1) % taille, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();*/

    RuleMap rules;
    Tree* tree = new Tree();

    try {
        parseFile(rules, string(argv[1]));
        //cout << rules;
        createTree(tree, rules, string(argv[2]));
        //cout << tree;
        tree->process();
    } catch (string &s) {
        cerr << "\033[41;2m" << " ERR " << "\033[0m" << " " << "\033[31;1m" << s << "\033[0m" << endl;
    }
}
