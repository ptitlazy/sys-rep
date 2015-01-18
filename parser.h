#ifndef PARSER_H
#define PARSER_H

#include <unordered_map>
#include <iomanip>

#include "tree.h"
#include "utils.h"

/**
* ...
* [rule1] => <command, [dep1, dep2, dep3...]>
* ...
*/
// typedef std::pair<std::string, std::vector<std::string>> RULE;

typedef Tree Rule;
typedef std::unordered_map<std::string, Rule *> RuleMap;

void parseFile(RuleMap &rules, std::string fileName);

void createTree(Tree *tree, RuleMap &rules, std::string target);

template<typename T, typename U>
std::ostream &operator<<(std::ostream &out, const std::unordered_map<T, U> &m) {
	RuleMap::const_iterator first = m.begin();
	RuleMap::const_iterator last = m.end();

	while (first != last) {
		std::cout << "[" << std::setw(15) << std::left << first->first << "] " << first->second << std::endl;
		++first;
	}

	return out;
}

#endif
