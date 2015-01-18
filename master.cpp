#include "master.h"

void master(Tree *tree, RuleMap *rules) {
	char hostname[MPI_MAX_PROCESSOR_NAME] = {};
	//Récupération des infos MPI
	int taille, rang, hostlen;
	MPI_Comm_size(MPI_COMM_WORLD, &taille);
	MPI_Comm_rank(MPI_COMM_WORLD, &rang);
	MPI_Get_processor_name(hostname, &hostlen);
	
	
	
	//Init List Tâches (Feuilles de l'arbre)
	//TODO
	
	//Init List Workers
	std::vector<int> idleWorkers(taille-1);
	for(int i; i=1; i<taille){
		idleWorkers.push_back(i);
	}
	
	//TODO : Boucle + Envoi de tâches
	
}

/** 
 * Fonction testant les parents d'un enfant venant d'être effectué
 * afin de voir si toutes ses dépendances sont réalisées et s'il peut l'être à son tour.
 */
bool testParent(Tree *child){
	//TODO
	return false;
}

/**
 * Termine la communication en envoyant un message de fin à tous les workers
 * Prend en paramètre la taille de l'univers MPI
 */
void Finalize(int taille){
	for(int i; i=1; i<taille){
		//TODO : Envoi message MPI
	}
}


