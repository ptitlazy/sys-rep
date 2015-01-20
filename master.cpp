#include "master.h"

#include <fstream>

bool testChildren(Tree *parent);
void Finalize(int taille);

void master(Tree *tree) {
	debug("Master started");

	char hostname[MPI_MAX_PROCESSOR_NAME] = {};
	//Récupération des infos MPI
	int taille, rang, hostlen;
	MPI_Comm_size(MPI_COMM_WORLD, &taille);
	MPI_Comm_rank(MPI_COMM_WORLD, &rang);
	MPI_Get_processor_name(hostname, &hostlen);
	MPI_Status status;

	debug("Master: Init tasks vector");
	//Init List Tâches (Feuilles de l'arbre)
	std::set<Tree const *> tasks_set = tree->getLeafs();
	std::vector<Tree const *> tasks(tasks_set.begin(), tasks_set.end());

	debug("Master: Init workers list");
	//Init List Workers
	std::vector<int> idleWorkers(taille - 1);
	for (int i = 1; i < taille ; i++) {
		idleWorkers.push_back(i);
	}

	debug("Master: Init tracker");
	//Tracker des tâches courantes
	Tree* tracker[taille-1];
	for(int i=0; i<(taille-1); i++){
		tracker[i]=NULL;
	}

	debug("Master: begin endless loop");
	while (1) {
		//Envoi des tâches aux workers idle.
		while(!tasks.empty() && !idleWorkers.empty()){
			//Récupération & mise à jour des données.
			int worker = idleWorkers.back();
			idleWorkers.pop_back();
			tracker[worker-1] = (Tree*) tasks.back();
			tasks.pop_back();

			debug("Master: Send task " + tracker[worker - 1]->getName() + " to worker " + to_string(worker));

			std::string message = tracker[worker-1]->serialize();
			MPI_Send(message.c_str(), message.length(), MPI_CHAR, worker, 1, MPI_COMM_WORLD);

			for (int i=0 ; i<tracker[worker-1]->getDependencies().size() ; i++) {
				debug("Master: Send file " + tracker[worker-1]->getDependencies()[i] + " to worker " + to_string(worker));
				MPI_Send(tracker[worker-1]->getDependencies()[i].c_str(), tracker[worker-1]->getDependencies()[i].length(), MPI_CHAR, worker, 1, MPI_COMM_WORLD);

				//Récupération du flux d'octets
				std::ifstream fl(tracker[worker-1]->getDependencies()[i]);
				fl.seekg( 0, std::ios::end );
				size_t len = fl.tellg();
				char *ret = new char[len];
				fl.seekg(0, std::ios::beg);
				fl.read(ret, len);
				fl.close();

				MPI_Send(ret, len, MPI_BYTE, worker, 1, MPI_COMM_WORLD);

				delete ret;
			}
		}

		//Attente puis traitement d'un message d'un worker.
		int reponse;
		MPI_Recv(&reponse, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		Tree *finished = tracker[status.MPI_SOURCE];
		finished->setExecuted(true);
		tracker[status.MPI_SOURCE]=NULL;
		idleWorkers.push_back(status.MPI_SOURCE);

		//testParents de Tree *finished.
		//S'il s'agit de la racine, ou non.
		if(finished==tree){
			if(testChildren(finished)){
				break; //fin.
			}
		} else {
			//Pour chaque parent
			for(std::vector<Tree *>::iterator it = finished->getParents().begin(); it != finished->getParents().end(); ++it){
				if(testChildren(*it)){
					tasks.push_back((*it));
				}
			}
		}

		//On laisse la première boucle en haut réasigner les tâches en réajoutant le worker dans les idle.
	}

	Finalize(taille);
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

/**
 * Vérifie si les dépendances de l'arbre passé en argument ont été exécutées.
 */
bool testChildren(Tree *parent){
	//Pour chaque enfant de chaque parent.
	bool dependenciesOk = true;
	for(std::vector<Tree *>::iterator it = parent->getChildren().begin(); it != parent->getChildren().end(); ++it) {
		if(!(*it)->isExecuted()){
			dependenciesOk = false;
			break;
		}
	}

	return dependenciesOk;
}

