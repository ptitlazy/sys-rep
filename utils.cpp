#include "utils.h"

#include "stdio.h"

#include <algorithm>
#ifdef WINDOWS
	#include <windows.h>
    #include <direct.h>
    #define getcwd _getcwd
#else
	#include <unistd.h>
#endif

#include <sstream>
#include <fstream>
#include <mpi.h>
#include <chrono>

// trim from start
std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

std::string getCurrentDir() {
	char currentPath[FILENAME_MAX];

	if (!getcwd(currentPath, sizeof(currentPath)))
	{
		throw "Cannot get current directory";
	}

	return std::string(currentPath);
}

std::string ssystem(std::string cmds) {
	char tmpname[L_tmpnam];
	tmpnam(tmpname);

	std::istringstream iss_cmd(cmds);
	std::string cmd;
	while (getline(iss_cmd, cmd, ';')) {
		debug("Executing " + cmd);

		if (cmd.find(">") == std::string::npos) {
			cmd = cmd + " >>" + tmpname + " 2>&1";
		}

		system(cmd.c_str());
	}

	/*std::ofstream file_temp(tmpname, std::ios::out);
	file_temp << cmd;
	cmd = "sh " + std::string(tmpname) + " > " + std::string(tmpname) + " 2> " + std::string(tmpname)
	system(cmd.c_str());*/

	std::ifstream file(tmpname, std::ios::in);
	std::string result;
	if (file) {
		while (!file.eof()) result.push_back(file.get());
		file.close();
	}

	remove(tmpname);
	return result;
}

std::string recv_string(int src, MPI_Status *status) {
	MPI_Probe(src, 1, MPI_COMM_WORLD, status);

	if (src == MPI_ANY_SOURCE) {
		src = status->MPI_SOURCE;
	}

	int length;
	MPI_Get_count(status, MPI_CHAR, &length);
	char *buf = new char[length];
	MPI_Recv(buf, length, MPI_CHAR, src, 1, MPI_COMM_WORLD, status);
	std::string res(buf, length);
	delete buf;
	return res;
}

std::string recv_file(int src, MPI_Status *status) {
	std::string name = recv_string(src, status);

	if (src == MPI_ANY_SOURCE) {
		src = status->MPI_SOURCE;
	}

	debug("Received file name: " + name + " from " + to_string(src));
	if (name == "ERROR") {
		return name;
	}

	debug("Receiving file content for: " + name + " from " + to_string(src));

	std::string temp_name = "__temp." + name + ".tmp";
	std::ofstream fl(temp_name, std::ios::out | std::ios::binary | std::ios::trunc );

	int taille;
	char *buf;

	MPI_Probe(src, 1, MPI_COMM_WORLD, status);
	MPI_Get_count(status, MPI_CHAR, &taille);
	buf = new char[taille];
	MPI_Recv(buf, taille, MPI_BYTE, src, 1, MPI_COMM_WORLD, status);

	fl.write(buf, taille);
	fl.close();
	delete buf;

	rename(temp_name.c_str(), name.c_str());

	debug("Received file: " + name + " from " + to_string(src));

	return name;
}

std::string to_string(int i) {
	std::string s;
	std::stringstream out;
	out << i;
	s = out.str();
	return s;
}

void debug(std::string msg) {
	std::chrono::time_point<std::chrono::high_resolution_clock> time = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::duration dtn = time.time_since_epoch();

	//std::cout << dtn.count() << "\033[22;43m\033[93m" << " DBG " << "\033[0m" << " " << msg << std::endl;
}

void time(std::string msg) {
	std::chrono::time_point<std::chrono::high_resolution_clock> time = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::duration dtn = time.time_since_epoch();

	timet(msg, dtn);
}

void timet(std::string msg, std::chrono::high_resolution_clock::duration dtn) {
	std::cout << "\033[22;44m\033[37m" << " BCH " << "\033[0m" << " " << dtn.count() << " " << msg << std::endl;
}

void bench(std::string msg, long value) {
	std::cout << "\033[22;44m\033[37m" << " BCH " << "\033[0m" << " " << value << " " << msg << std::endl;
}

void error(std::string msg) {
	std::cerr << "\033[22;41m\033[91m" << " ERR " << "\033[0m" << " " << "\033[31;1m" << msg << "\033[0m" << std::endl;
}

void send_file(int dest, std::string file_name) {
	std::ifstream fl(file_name);

	if(!fl.is_open()) {
		throw "file '" + file_name + "' not available (sending to " + to_string(dest) + ")";
	}

	debug("Sending file name for: " + file_name + " to " + to_string(dest));
	MPI_Ssend(file_name.c_str(), file_name.length(), MPI_CHAR, dest, 1, MPI_COMM_WORLD);
	bench("SEND", file_name.length());

	debug("Reading file size for: " + file_name  + " to " + to_string(dest));
	//Récupération du flux d'octets
	fl.seekg( 0, std::ios::end );
	size_t len = fl.tellg();

	debug("Reading file content for: " + file_name + " to " + to_string(dest));
	char *ret = new char[len];
	fl.seekg(0, std::ios::beg);
	fl.read(ret, len);
	fl.close();

	debug("Sending file content for: " + file_name + " to " + to_string(dest));

	MPI_Ssend(ret, len, MPI_BYTE, dest, 1, MPI_COMM_WORLD);
	bench("SEND", len);
	debug("Sent file content for: " + file_name + " to " + to_string(dest));

	delete ret;
}

int m_stoi(std::string s) {
	int numb;
	std::istringstream ( s ) >> numb;
	return numb;
}
