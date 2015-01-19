#include "master.h"

void Finalize(int taille);

void master(Tree *tree, RuleMap *rules) {
	std::cout << "Master started" << std::endl;

	char hostname[MPI_MAX_PROCESSOR_NAME] = {};
	//Récupération des infos MPI
	int taille, rang, hostlen;
	MPI_Comm_size(MPI_COMM_WORLD, &taille);
	MPI_Comm_rank(MPI_COMM_WORLD, &rang);
	MPI_Get_processor_name(hostname, &hostlen);

	//Init List Tâches (Feuilles de l'arbre)
	std::set<Tree const *> tasks_set = tree->getLeafs();
	std::vector<Tree const *> tasks(tasks_set.begin(), tasks_set.end());

	//Init List Workers
	std::vector<int> idleWorkers(taille - 1);
	for (int i; i = 1; i < taille) {
		idleWorkers.push_back(i);
	}

	while (1) {
		//TODO : Envoi des tâches aux workers idle. (1 more while)

		//TODO : Attente d'un message d'un worker.

		//TODO : testParent();

		//TODO : test terminaison?

		//On laisse la première boucle en haut réasigner les tâches en réajoutant le worker dans les idle.

		break;
	}

	Finalize(taille);
}

/**
* Fonction testant les parents d'un enfant venant d'être effectué
* afin de voir si toutes ses dépendances sont réalisées et s'il peut l'être à son tour.
*/
//TODO : Editer retour de la fonction. On renverra directement le parent.
bool testParent(Tree *child) {
	//TODO
	return false;
}

/**
* Termine la communication en envoyant un message de fin à tous les workers
* Prend en paramètre la taille de l'univers MPI
*/
void Finalize(int taille) {
	std::string blank = "STOP";
	for (int i = 1; i < taille ; i++) {
		MPI_Send((void *) blank.c_str(), (int) blank.length(), MPI_CHAR, i, 1, MPI_COMM_WORLD);
	}
}


