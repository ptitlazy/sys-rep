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
    Tree() {
    }

    Tree(std::string name, std::string cmd, std::vector<std::string> dependencies) : name(name), cmd(cmd), dependencies(dependencies) {
    }

    void addChild(Tree *child);

    void process() const;
    void execute() const;
    int size() const;

    std::string &getName() {
        return name;
    }

    void setName(std::string &name) {
        Tree::name = name;
    }

    std::string &getCmd() {
        return cmd;
    }

    void setCmd(std::string &cmd) {
        Tree::cmd = cmd;
    }

    std::vector<std::string> &getDependencies() {
        return dependencies;
    }

    void setDependencies(std::vector<std::string> &dependencies) {
        Tree::dependencies = dependencies;
    }

private:
    std::string name;
    std::string cmd;
    std::vector<std::string> dependencies;
    std::vector<Tree*> children;

    void display(std::ostream &out, int indent) const;

    friend std::ostream &operator<<(std::ostream &out, const Tree *t);
};

std::ostream &operator<<(std::ostream &out, const Tree *t);

#endif
