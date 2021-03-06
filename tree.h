#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <set>

/*
 * Class Tree enable representing a tree of dependencies
 */
class Tree {
public:
	Tree() {
	}

	Tree(std::string name, std::string cmd, std::vector<std::string> dependencies)
			: name(name), cmd(cmd), dependencies(dependencies) {
	}

	std::string serialize(int avancement = 50) const;

	void addChild(Tree *child);

	void addParent(Tree *parent);

	void process() const;

	void execute() const;

	int size() const;

	std::set<Tree *> getLeafs();

	bool isExecuted() {
		return executed;
	}

	void setExecuted(bool e) {
		executed = e;
	}

	bool isParsed() {
		return parsed;
	}

	void setParsed(bool e) {
		parsed = e;
	}

	bool isQueued() {
		return queued;
	}

	void setQueued(bool e) {
		queued = e;
	}

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

	std::vector<Tree *> &getParents(){
		return parents;
	}

	std::vector<Tree *> &getChildren(){
		return children;
	}

private:
	bool parsed;
	bool queued;
	bool executed;
	std::string name;
	std::string cmd;
	std::vector<std::string> dependencies;
	std::vector<Tree *> children;
	std::vector<Tree *> parents;

	void display(std::ostream &out, int indent) const;

	friend std::ostream &operator<<(std::ostream &out, const Tree *t);
};

std::ostream &operator<<(std::ostream &out, const Tree *t);

#endif
