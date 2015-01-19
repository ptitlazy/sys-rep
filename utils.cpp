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
