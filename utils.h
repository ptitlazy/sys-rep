#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <set>

// trim from start
std::string &ltrim(std::string &s);

// trim from end
std::string &rtrim(std::string &s);

// trim from both ends
std::string &trim(std::string &s);

std::string getCurrentDir();

// display vectors
template<typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
	out << "[";

	typename std::vector<T>::const_iterator first = v.cbegin();
	typename std::vector<T>::const_iterator last = v.cend();

	while (first != last) {
		out << *first;
		++first;

		if (first != last) {
			out << ", ";
		}
	}

	out << "]";
	return out;
}

// display set
template<typename T>
std::ostream &operator<<(std::ostream &out, const std::set<T> &v) {
	out << "{";

	typename std::set<T>::const_iterator first = v.cbegin();
	typename std::set<T>::const_iterator last = v.cend();

	while (first != last) {
		out << *first;
		++first;

		if (first != last) {
			out << ", ";
		}
	}

	out << "}";
	return out;
}

// display pairs
template<typename T, typename U>
std::ostream &operator<<(std::ostream &out, const std::pair<T, U> &v) {
	out << "<" << v.first << "," << v.second << ">";
	return out;
}

std::string ssystem(std::string cmds);

#endif
