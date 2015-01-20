#include "worker.h"
#include "utils.h"
#include <vector>
#include <sstream>
#include <fstream>

void worker(int rang) {
	bool istasks = true;
	int res = 0;
	MPI_Status status;
	std::string fileNameSCP = "DMAKE_SCP_OK";

	//TODO : recevoir le hostname et le dossier dans lequel le master travail

	while (istasks) {
		/*
		Reception du message sous forme de string serialisée
		 */
		std::string message = recv_string(&status);

		//Si la cible est "STOP", on arrête tout
		if (message == "STOP") {
			break;
		}

		workerRule rule = deserialize(message);

		/*
		Do the job
		 */
		//TODO : 1. récupérer les fichiers
		//if (not fichier) {
		//	scp master:/dossier/fichier .
		//}

		//TODO : 2. executer la tâche
		executer(rule.cmd);

		/*
		Envoi de la réponse
		 */
		//TODO : 1. Envoyer le fichier créé
		// if sur machine master, cp ; else scp

		//TODO : 2. send the "ok, done"

		//TODO : a suppr, debug
		istasks = false;
	}
}

void executer(std::string cmd) {
	std::string s = ssystem(cmd);

	std::istringstream iss(s);
	std::string line;
	while (std::getline(iss, line)) {
		line = trim(line);
		if (line.length() > 0) {
			std::cout << "\033[22;104m\033[97m" << " OUT " << "\033[0m" << " " << line << std::endl;
		}
	}
}

workerRule deserialize(std::string s) {
	std::istringstream iss(s);
	std::string temp;
	std::string name;
	std::vector<std::string> dependencies;
	std::string cmds;

	// name
	std::getline(iss, name, '|');

	// dependencies
	std::getline(iss, temp, '|');
	std::istringstream iss_temp(s);
	while (std::getline(iss_temp, temp, ';')) {
		dependencies.push_back(temp);
	}

	// cmds
	std::getline(iss, cmds, '|');

	std::cout << name << std::endl;
	std::cout << dependencies << std::endl;
	std::cout << cmds << std::endl;

	workerRule res;
	res.cmd = cmds;
	res.target = name;
	res.liste_dep = dependencies;

	return res;
}
