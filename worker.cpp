#include "worker.h"
#include <vector>

void worker(int rang) {
	bool istasks = true;
	int res = 0;
	MPI_Status status;

	while (istasks) {
		/*
		Reception de la target
		 */
		//Maintenant qu'on connait la longeur de la cha�ne, on r�cup�re celle-ci
		std::string target = recv_string(status);

		//Si la cible est "STOP", on arr�te tout
		if (target == "STOP") {
			break;
		}

		/*
		R�cup�ration de la commande
		 */
		std::string cmd = recv_string(status);

		/*
		R�cup�ration des d�pendances
		 */

		//Nombre de d�pendances
		int nb_dep;
		MPI_Recv(&nb_dep, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

		//Liste des dependances
		std::vector<std::string> liste_dep;

		for (int i = 1; i <= nb_dep; i++) {
			liste_dep.push_back(recv_string(status));
		}


		/*
		Do the job
		 */
		//TODO : 1. r�cup�rer les fichiers
		//TODO : 2. executer la t�che

		/*
		Send the response
		 */
		//TODO

		//TODO : a suppr, debug
		istasks = false;
	}
}

std::string recv_string(MPI_Status status) {
	MPI_Probe(0, 1, MPI_COMM_WORLD, &status);
	int length;
	MPI_Get_count(&status, MPI_CHAR, &length);
	char *buf = new char[length];
	MPI_Recv(buf, length, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
	return std::string(buf, length);
}
