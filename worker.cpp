#include "worker.h"
#include "utils.h"

#include <sstream>
#include <fstream>
#include <iomanip>

void worker(int rang) {
	MPI_Status status;

	debug("Worker " + to_string(rang) + ": started");
	while (1) {
		/*
		Reception du message sous forme de string serialisée
		 */
		std::string message = recv_string(0, &status);

		debug("Worker " + to_string(rang) + ": received " + message);
		//Si la cible est "STOP", on arrête tout
		if (message == "STOP") {
			break;
		}

		workerRule rule = deserialize(message);

		std::cout << "[" << std::setw(3) << rule.avancement << std::left << "%]" << "\033[32;2m" << " Building " << rule.target << "\033[0m" << std::endl;

		/*
		Do the job
		 */
		debug("Worker " + to_string(rang) + ": receiving dependencies...");
		//Récupération des fichiers de dépendances
		for (int i=0 ; i<rule.liste_dep.size(); ++i) {
			recv_file(0, &status);
		}

		debug("Worker " + to_string(rang) + ": executing...");
		//Exécution de la tâche
		executer(rule.cmd);

		/*
		Envoi de la réponse
		 */
		debug("Worker " + to_string(rang) + ": sending results...");
		try {
			send_file(0, rule.target);
		} catch (std::string &s) {
			debug("Worker " + to_string(rang) + ": " + s);
			error(s);
			std::string error = "ERROR";
			MPI_Send(error.c_str(), error.length(), MPI_CHAR, 0, 1, MPI_COMM_WORLD);
		}

		debug("Worker " + to_string(rang) + ": task done");
	}

	debug("Worker " + to_string(rang) + ": ended");
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
	std::string temp1;
	std::string name;
	std::vector<std::string> dependencies;
	std::string cmds;
	std::string avancement;

	// name
	std::getline(iss, name, '|');

	// dependencies
	std::getline(iss, temp, '|');
	std::istringstream iss_temp(temp);
	while (std::getline(iss_temp, temp1, ';')) {
		dependencies.push_back(temp1);
	}

	// cmds
	std::getline(iss, cmds, '|');

	// avancement
	std::getline(iss, avancement, '|');

	workerRule res;
	res.cmd = cmds;
	res.target = name;
	res.liste_dep = dependencies;
	res.avancement = m_stoi(avancement);

	return res;
}
