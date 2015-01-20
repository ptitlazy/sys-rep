#include "worker.h"
#include "utils.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <w32api/lmaudit.h>

void worker(int rang) {
	int res = 0;
	MPI_Status status;
	std::string fileNameSCP = "DMAKE_SCP_OK";

	while (1) {
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
		//Récupération des fichiers de dépendances
		for (int i=0 ; i<rule.liste_dep.size(); ++i) {

			recv_file(0, &status);

		}

		//Exécution de la tâche
		executer(rule.cmd);

		/*
		Envoi de la réponse
		 */
		send_file(0, rule.target);
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
