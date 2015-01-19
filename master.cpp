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
	MPI_Status status;

	//Init List Tâches (Feuilles de l'arbre)
	std::set<Tree const *> tasks_set = tree->getLeafs();
	std::vector<Tree const *> tasks(tasks_set.begin(), tasks_set.end());

	//Init List Workers
	std::vector<int> idleWorkers(taille - 1);
	for (int i = 1; i < taille ; i++) {
		idleWorkers.push_back(i);
	}
	
	//Tracker des tâches courantes
	Tree const *tracker[taille-1];
	for(int i=0; i<(taille-1); i++){
		tracker[i]=NULL;
	}

	while (1) {
		//Envoi des tâches aux workers idle.
		while(!tasks.empty() && !idleWorkers.empty()){
			//Récupération & mise à jour des données.
			int worker = idleWorkers.back();
			idleWorkers.pop_back();
			tracker[worker-1] = tasks.back();
			tasks.pop_back();
			
			//TODO: Envoi message. (on a toutes les données nécessaires)
		}

		//Attente puis traitement d'un message d'un worker.
		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		Tree *finished = (Tree *)tracker[status.MPI_SOURCE];
		finished->setExecuted(true);
		tracker[status.MPI_SOURCE]=NULL;
		idleWorkers.push_back(status.MPI_SOURCE);
		
		//testParents de Tree *finished.
		//S'il s'agit de la racine, ou non.
		if(finished==tree){
			bool end=true;
			for(std::vector<Tree *>::iterator it = finished->getChildren().begin(); it != finished->getChildren().end(); ++it){
				if(!(*it)->isExecuted()){
					end = false;
					break;
				}
			}
			if(end){
				break; //fin.
			}
		} else {
			//Pour chaque parent
			for(std::vector<Tree *>::iterator it = finished->getParents().begin(); it != finished->getParents().end(); ++it) {
				//Pour chaque dépendance de chaque parent.
				bool dependenciesOk = true;
				for(std::vector<Tree *>::iterator it2 = (*it)->getChildren().begin(); it2 != (*it)->getChildren().end(); ++it2) {
					if(!(*it2)->isExecuted()){
					dependenciesOk = false;
					break;
					}
				}
				//Si les dépendances sont OK
				if(dependenciesOk){
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


