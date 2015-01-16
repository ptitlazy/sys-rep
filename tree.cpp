#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include "tree.h"
#include "utils.h"

int current = 0;
int total = -1;

void Tree::addChild(Tree* child) {
    this->children.push_back(child);
}

void Tree::display(std::ostream &out, int indent) const {
    out << std::string(indent * 4, ' ') << "|- " << this->name << std::endl;

    std::vector<Tree*>::const_iterator first = this->children.begin();
    std::vector<Tree*>::const_iterator last = this->children.end();

    while (first != last) {
        (*first)->display(out, indent + 1);
        ++first;
    }
}

void Tree::process() const {
    if (total == -1) {
        total = this->size();
    }

    std::vector<Tree*>::const_iterator first = this->children.begin();
    std::vector<Tree*>::const_iterator last = this->children.end();

    //std::cout << "\033[32;0m" << "[" << "%] Building " << this->name << "\033[0m" << std::endl;
    //std::cout << "\033[36;0m" << "Dependencies" << "\033[0m" << std::endl;

    while (first != last) {
        (*first)->process();
        ++first;
    }

    current++;

    std::cout << "[" << std::setw(3) << std::right << (current*100)/total << std::left << "%]" << "\033[32;2m" << " Building " << this->name << "\033[0m" << std::endl;
    //std::cout << "\033[36;0m" << "Execution" << "\033[0m" << std::endl;

    this->execute();

    if (current == total) {
        current = 0;
        total = -1;
    }
}

std::string ssystem (std::string cmds) {
    char tmpname [L_tmpnam];
    tmpnam ( tmpname );

    std::istringstream iss_cmd(cmds);
    std::string cmd;
    while(getline(iss_cmd, cmd, ';')) {
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

void Tree::execute() const {
        std::string s = ssystem(this->cmd);

        std::istringstream iss(s);
        std::string line;
        while (std::getline(iss, line)) {
            line = trim(line);
            if (line.length() > 0) {
                std::cout << "\033[42;2m" << " OUT " << "\033[0m" << " " << line << std::endl;
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

    std::vector<Tree*>::const_iterator first = this->children.begin();
    std::vector<Tree*>::const_iterator last = this->children.end();

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
