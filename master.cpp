#include "master.h"
#include "worker.h"

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
	std::vector<int> idleWorkers;
	for (int i = 1; i < taille ; i++) {
		idleWorkers.push_back(i);
	}
	std::cout << idleWorkers << std::endl;

	debug("Master: Init tracker");
	//Tracker des tâches courantes
	Tree* tracker[taille-1];
	for(int i=0; i<(taille-1); i++){
		tracker[i]=NULL;
	}

	int total = tree->size() - 1;
	int current = 0;

	debug("Master: begin endless loop");
	try {
		while (1) {
			//Envoi des tâches aux workers idle.
			while (!tasks.empty() && !idleWorkers.empty()) {
				//Récupération & mise à jour des données.
				int worker = idleWorkers.back();

				debug("Master: idle worker: " + to_string(worker));

				idleWorkers.pop_back();
				tracker[worker-1] = (Tree*) tasks.back();
				tasks.pop_back();

				debug("Master: Send task " + tracker[worker - 1]->getName() + " to worker " + to_string(worker));

				current++;

				std::string message = tracker[worker - 1]->serialize((current * 100) / total);
				MPI_Send(message.c_str(), message.length(), MPI_CHAR, worker, 1, MPI_COMM_WORLD);

				for (int i=0 ; i<tracker[worker-1]->getDependencies().size() ; i++) {

					debug("Master: Send file " + tracker[worker-1]->getDependencies()[i] + " to worker " + to_string(worker));

					send_file(worker, tracker[worker-1]->getDependencies()[i]);

					debug("Master: file sent to worker " + to_string(worker));
				}

				debug("Master: task sent to worker " + to_string(worker));
			}

			debug("Master: waiting for a worker...");
			//Attente puis traitement d'un message d'un worker.
			std::string name = recv_file(MPI_ANY_SOURCE, &status);

			if (name == "ERROR") {
				break;
			}

			Tree *finished = tracker[status.MPI_SOURCE - 1];
			finished->setExecuted(true);

			debug("Master: Message received: " + finished->getName() + " executed");

			tracker[status.MPI_SOURCE - 1] = NULL;

			debug("Master: set " + to_string(status.MPI_SOURCE) + " as idle");

			idleWorkers.push_back(status.MPI_SOURCE);

			bool end = false;
			//Pour chaque parent
			for(std::vector<Tree *>::iterator it = finished->getParents().begin(); it != finished->getParents().end(); ++it) {
				if(testChildren(*it)) {
					debug("Master: " + (*it)->getName() + " available");
					// On ne veut pas exécuter root
					if(*it == tree){
						if(testChildren(finished)){
							end = true;
							break;
						}
					}

					tasks.push_back((*it));
				}
			}

			if (end) {
				break;
			}
			//On laisse la première boucle en haut réasigner les tâches en réajoutant le worker dans les idle.
		}
	} catch (std::string &s) {
		debug("Master: " + s);
		error(s);
	}

	debug("Master: Endless loop ended");
	Finalize(taille);
}


/**
* Termine la communication en envoyant un message de fin à tous les workers
* Prend en paramètre la taille de l'univers MPI
*/
void Finalize(int taille) {
	std::string blank = "STOP";
	for (int i = 1; i < taille ; i++) {
		debug("Master: send STOP to worker " + to_string(i));
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

