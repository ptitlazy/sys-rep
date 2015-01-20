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

void recv_file(int &taille, char *buf, MPI_Status *status) {
	MPI_Probe(0, 1, MPI_COMM_WORLD, status);
	MPI_Get_count(status, MPI_CHAR, &taille);
	buf = new char[taille];
	MPI_Recv(buf, taille, MPI_BYTE, 0, 1, MPI_COMM_WORLD, status);
}

std::string to_string(int i) {
	std::string s;
	std::stringstream out;
	out << i;
	s = out.str();
	return s;
}


