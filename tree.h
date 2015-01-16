#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <vector>

/*
 * Class Tree enable representing a tree of dependencies
 */
class Tree {
public:
    Tree(std::string name) : name(name) {
    }

    void addChild(Tree child);

private:
    std::string name;
    std::string cmd;
    std::string file;
    std::vector<Tree> children;

    void display(std::ostream &out, int indent) const;

    friend std::ostream &operator<<(std::ostream &out, const Tree t);
};

std::ostream &operator<<(std::ostream &out, const Tree t);

#endif