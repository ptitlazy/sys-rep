#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.h"

void parseFile(RuleMap &rules, std::string fileName) {
    std::ifstream myFile(fileName);
    std::string lineRaw;
    std::string currentRuleName;
    std::pair<std::string, std::vector<std::string>> currentRule;

    if (myFile.is_open()) {
        while (getline(myFile, lineRaw)) {
            std::stringstream line(lineRaw);
            std::string word;

            line >> word;
            if (word.back() == ':') {
                if (currentRuleName.length() > 0) {
                    rules[currentRuleName] = currentRule;
                }

                currentRuleName = word.substr(0, word.length() - 1);
                currentRule = std::make_pair(std::string(), std::vector<std::string>());

                while (line >> word) {
                    currentRule.second.push_back(word);
                }
            } else {
                if (currentRuleName.length() > 0) {
                    currentRule.first.append(trim(lineRaw));
                }
            }
        }
        if (currentRuleName.length() > 0) {
            rules[currentRuleName] = currentRule;
        }

        myFile.close();
    } else {
        std::ostringstream buf;
        buf << "Unable to open " << fileName;
        throw buf.str();
    }
}

void createTree(Tree &tree, RuleMap &rules, std::string target) {
    if (rules.find(target) == rules.end()) {
        std::ostringstream buf;
        buf << "No rule " << target;
        throw buf.str();
    }

    Tree subTree(target);

    std::pair<std::string, std::vector<std::string>> rule = rules[target];

    std::vector<std::string>::iterator first = rule.second.begin();
    std::vector<std::string>::iterator last = rule.second.end();

    while (first != last) {
        createTree(subTree, rules, *first);
        ++first;
    }

    tree.addChild(subTree);
}
