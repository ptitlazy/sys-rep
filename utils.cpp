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
		cmd = cmd + " >> " + tmpname + " 2>> " + tmpname;
		system(cmd.c_str());
	}

	std::ifstream file(tmpname, std::ios::in);
	std::string result;
	if (file) {
		while (!file.eof()) result.push_back(file.get());
		file.close();
	}

	remove(tmpname);
	return result;
}

std::string recv_string(MPI_Status *status) {
	MPI_Probe(0, 1, MPI_COMM_WORLD, status);
	int length;
	MPI_Get_count(status, MPI_CHAR, &length);
	char *buf = new char[length];
	MPI_Recv(buf, length, MPI_CHAR, 0, 1, MPI_COMM_WORLD, status);
	std::string res(buf, length);
	delete buf;
	return res;
}

std::string recv_file(int src, MPI_Status *status) {
	std::string name = recv_string(status);
	std::ofstream fl(name, std::ios::out | std::ios::binary | std::ios::trunc);

	int taille;
	char *buf;

	MPI_Probe(0, 1, MPI_COMM_WORLD, status);
	MPI_Get_count(status, MPI_CHAR, &taille);
	buf = new char[taille];
	MPI_Recv(buf, taille, MPI_BYTE, src, 1, MPI_COMM_WORLD, status);

	fl.write(buf, taille);
	fl.close();
	delete buf;

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
	std::cout << "\033[22;104m\033[97m" << " DBG " << "\033[0m" << " " << msg << std::endl;
}

void send_file(int dest, std::string file_name) {
	MPI_Send(file_name.c_str(), file_name.length(), MPI_CHAR, dest, 1, MPI_COMM_WORLD);

	//Récupération du flux d'octets
	std::ifstream fl(file_name);
	fl.seekg( 0, std::ios::end );
	size_t len = fl.tellg();
	char *ret = new char[len];
	fl.seekg(0, std::ios::beg);
	fl.read(ret, len);
	fl.close();

	MPI_Send(ret, len, MPI_BYTE, dest, 1, MPI_COMM_WORLD);

	delete ret;
}
