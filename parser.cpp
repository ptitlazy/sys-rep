#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.h"

void parseFile(RuleMap &rules, std::string fileName) {
	std::ifstream myFile(fileName);
	std::string lineRaw;

	Rule *currentRule = new Rule();
	//std::pair<std::string, std::vector<std::string>> currentRule;

	if (myFile.is_open()) {
		while (getline(myFile, lineRaw)) {
			std::stringstream line(lineRaw);
			std::string word;

			line >> word;
			bool newRule = false;
			if (word.back() != ':') {
				std::string next;
				line >> next;

				if (next.front() == ':') {
					newRule = true;
				}
			} else {
				newRule = true;
				word = word.substr(0, word.length() - 1);
			}

			if (newRule) {
				if (currentRule->getName().length() > 0) {
					rules[currentRule->getName()] = currentRule;
					currentRule = new Rule();
				}

				currentRule->setName(word);

				while (line >> word) {
					currentRule->getDependencies().push_back(word);
				}
			} else {
				if (currentRule->getName().length() > 0) {
					currentRule->setCmd(currentRule->getCmd().append(trim(lineRaw)));
				}
			}
		}

		if (currentRule->getName().length() > 0) {
			rules[currentRule->getName()] = currentRule;
		} else {
			delete currentRule;
		}

		myFile.close();
	} else {
		std::ostringstream buf;
		buf << "Unable to open " << fileName;
		throw buf.str();
	}
}

void createTree(Tree *treeRoot, RuleMap &rules, std::string target) {
    Tree *targetRule = rules[target];
    treeRoot->addChild(targetRule);
    targetRule->addParent(treeRoot);

    createTreeRecurse(targetRule, rules);
}

void createTreeRecurse(Tree *tree, RuleMap &rules) {
    if (rules.find(tree->getName()) == rules.end()) {
        if (file_exists(tree->getName())) {
            Rule* rule = new Rule(tree->getName(), "", std::vector<std::string>());
            rule->setExecuted(true);
            rules[tree->getName()] = rule;
        } else {
            std::ostringstream buf;
            buf << "No rule " << tree->getName();
            throw buf.str();
        }
    }

    if (!tree->isParsed()) {
        tree->setParsed(true);

        std::vector<std::string>::iterator first = tree->getDependencies().begin();
        std::vector<std::string>::iterator last = tree->getDependencies().end();

        for(std::vector<std::string>::iterator it = tree->getDependencies().begin(); it != tree->getDependencies().end(); ++it) {
            Tree *child = rules[(*it)];
            tree->addChild(child);
            child->addParent(tree);
            createTreeRecurse(child, rules);
            ++first;
        }
    }

}

#if 0
void createTree(Tree *tree, RuleMap &rules, std::string target) {
	if (rules.find(target) == rules.end()) {
		if (file_exists(target)) {
			Rule* rule = new Rule(target, "", std::vector<std::string>());
			rule->setExecuted(true);
			rules[target] = rule;
		} else {
			std::ostringstream buf;
			buf << "No rule " << target;
			throw buf.str();
		}
	}

	Tree *subTree = rules[target];
	tree->addChild(subTree);

	if (subTree->isParsed()) {
		return;
	}
	subTree->setParsed(true);

	subTree->addParent(tree);

	Rule *rule = rules[target];

	std::vector<std::string>::iterator first = rule->getDependencies().begin();
	std::vector<std::string>::iterator last = rule->getDependencies().end();

	while (first != last) {
		createTree(subTree, rules, *first);
		++first;
	}
}
#endif
