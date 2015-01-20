#include <iomanip>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#include "tree.h"
#include "utils.h"
#include "worker.h"

int current = 0;
int total = -1;

void Tree::addChild(Tree *child) {
	this->children.push_back(child);
}

void Tree::addParent(Tree *parent) {
	this->parents.push_back(parent);
}

void Tree::display(std::ostream &out, int indent) const {
	out << std::string(indent * 4, ' ') << "|- " << this->name << std::endl;

	std::vector<Tree *>::const_iterator first = this->children.cbegin();
	std::vector<Tree *>::const_iterator last = this->children.cend();

	while (first != last) {
		(*first)->display(out, indent + 1);
		++first;
	}
}

void Tree::process() const {
	if (total == -1) {
		total = this->size();
	}

	std::vector<Tree *>::const_iterator first = this->children.cbegin();
	std::vector<Tree *>::const_iterator last = this->children.cend();

	//std::cout << "\033[32;0m" << "[" << "%] Building " << this->name << "\033[0m" << std::endl;
	//std::cout << "\033[36;0m" << "Dependencies" << "\033[0m" << std::endl;

	while (first != last) {
		(*first)->process();
		++first;
	}

	current++;

	std::cout << "[" << std::setw(3) << std::right << (current * 100) / total << std::left << "%]" << "\033[32;2m" << " Building " << this->name << "\033[0m" << std::endl;
	//std::cout << "\033[36;0m" << "Execution" << "\033[0m" << std::endl;

	this->execute();

	if (current == total) {
		current = 0;
		total = -1;
	}
}

void Tree::execute() const {
	std::string s = ssystem("cd /tmp/toto;" + this->cmd);

	std::istringstream iss(s);
	std::string line;
	while (std::getline(iss, line)) {
		line = trim(line);
		if (line.length() > 0) {
			std::cout << "\033[22;104m\033[97m" << " OUT " << "\033[0m" << " " << line << std::endl;
		}
	}

	/*FILE* pipe = popen(this->cmd.c_str(), "r");
	if (!pipe) {
		std::cerr << "Can't execute the command" << std::endl;
	}*/



	/*char buffer[128];
	std::string result = "";
	while(!feof(pipe)) {
		if(fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	pclose(pipe);
	return result;*/

	//system(this->cmd.c_str());
}

int Tree::size() const {
	int size = 0;

	std::vector<Tree *>::const_iterator first = this->children.cbegin();
	std::vector<Tree *>::const_iterator last = this->children.cend();

	while (first != last) {
		size += (*first)->size();
		++first;
	}

	size++;

	return size;
}

std::ostream &operator<<(std::ostream &out, const Tree *t) {
	t->display(out, 0);
	return out;
}

std::set<const Tree *> Tree::getLeafs() const {
	std::set<const Tree *> leafs;

	std::vector<Tree *>::const_iterator first = this->children.cbegin();
	std::vector<Tree *>::const_iterator last = this->children.cend();

	if (first == last) {
		leafs.insert(this);
	} else {
		while (first != last) {
			std::set<const Tree *> child_leafs = (*first)->getLeafs();
			leafs.insert(child_leafs.begin(), child_leafs.end());
			++first;
		}
	}

	return leafs;
}

// <name>|<dependencies>|<cmd>
std::string Tree::serialize() const {
	std::stringstream ss;

	ss << this->name << "|";

	std::vector<Tree *>::const_iterator first = this->children.cbegin();
	std::vector<Tree *>::const_iterator last = this->children.cend();

	while (first != last) {
		ss << (*first)->name;
		++first;

		if (first != last) {
			ss << ";";
		}
	}

	ss << "|";
	ss << this->cmd;

	return ss.str();
}

#if 0
void deserialize(std::string s) {
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
}
#endif
