#include <mpi.h>

#include "tree.h"
#include "parser.h"
#include "worker.h"
#include "master.h"

using namespace std;

int main(int argc, char **argv) {
#if 0
    int taille, rang, hostlen;
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

    MPI_Finalize();
#endif
	int taille, rang, hostlen;
	char hostname[MPI_MAX_PROCESSOR_NAME] = {};
	int TAG = 1;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &taille);
	MPI_Comm_rank(MPI_COMM_WORLD, &rang);
	MPI_Get_processor_name(hostname, &hostlen);

	if (rang == 0) {
		//Création de l'arbre
		RuleMap rules;
		Tree *tree = new Tree("Name", "", std::vector<std::string>());

		try {
			parseFile(rules, string(argv[1]));
			//cout << rules;
			createTree(tree, rules, string(argv[2]));
			//cout << tree;
			//tree->process();
		} catch (string &s) {
			cerr << "\033[22;41m\033[91m" << " ERR " << "\033[0m" << " " << "\033[31;1m" << s << "\033[0m" << endl;
		}

		if (taille == 1) {
			tree->process();
		} else {
			//Fonction master
			master(tree, &rules);
		}
	} else {
		cout << "<<< Hi! Here is " << rang << " on " << hostname << ">>>" << endl;
		cout << getCurrentDir() << endl;
		worker(rang);
	}

	MPI_Finalize();
}
