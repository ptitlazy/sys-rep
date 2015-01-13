#include <mpi.h>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <functional>
#include <locale>

#include "tree.h"

using namespace std;

// trim from start
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

template<typename T>
ostream& operator<< (ostream& out, const vector<T>& v) {
    out << "[";
    size_t last = v.size() - 1;
    for(size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last)
            out << ", ";
    }
    out << "]";
    return out;
}

template<typename T, typename U>
ostream& operator<< (ostream& out, const pair<T, U>& v) {
    out << "<" << v.first << "," << v.second << ">";
    return out;
}

template<typename T, typename U>
ostream& operator<< (ostream& out, const unordered_map<T, U>& m) {
    unordered_map<string, pair<string, vector<string>>>::const_iterator first = m.begin();
    unordered_map<string, pair<string, vector<string>>>::const_iterator last = m.end();

    while (first != last) {
        cout << "[" << first->first << "] " << first->second << endl;
        ++first;
    }

    return out;
}

void parseFile(unordered_map<string, pair<string, vector<string>>> &rules, string fileName);

void createTree(Tree &tree, unordered_map<string, pair<string, vector<string>>> &rules, string target);

int main(int argc, char **argv) {
    /*int taille, rang, hostlen;
    char hostname[MPI_MAX_PROCESSOR_NAME] = {};
    double message = rang + 123;
    int TAG = 123456;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &taille);
    MPI_Comm_rank(MPI_COMM_WORLD, &rang);
    MPI_Get_processor_name(hostname, &hostlen);

    if (rang == 0) {
        MPI_Send(&message, 1, MPI_DOUBLE, (rang + 1) % taille, TAG, MPI_COMM_WORLD);
        MPI_Recv(&message, 1, MPI_DOUBLE, (taille - 1), TAG, MPI_COMM_WORLD, &status);
        printf("Message %g reçu ! %d parmi %d sur %s!\n", message, rang, taille, hostname);
    } else {
        MPI_Recv(&message, 1, MPI_DOUBLE, (rang - 1), TAG, MPI_COMM_WORLD, &status);
        printf("Message %g reçu ! %d parmi %d sur %s!\n", message, rang, taille, hostname);
        MPI_Send(&message, 1, MPI_DOUBLE, (rang + 1) % taille, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();*/
    /**
    * ...
    * [rule1] => <command, [dep1, dep2, dep3...]>
    * ...
    */
    unordered_map<string, pair<string, vector<string>>> rules;
    Tree tree("ROOT");

    try {
        parseFile(rules, "test.makefile");
        cout << rules;
        createTree(tree, rules, "list.txt");
        cout << tree;
    } catch (string &s) {
        cerr << s << endl;
        cout << tree;
    }
}

void parseFile(unordered_map<string, pair<string, vector<string>>> &rules, string fileName) {

    ifstream myFile (fileName);
    string lineRaw;
    string currentRuleName;
    pair<string, vector<string>> currentRule;

    if (myFile.is_open())
    {
        while ( getline (myFile, lineRaw) )
        {
            stringstream line(lineRaw);
            string word;

            line >> word;
            if (word.back() == ':') {
                if (currentRuleName.length() > 0) {
                    rules[currentRuleName] = currentRule;
                }

                currentRuleName = word.substr(0, word.length() - 1);
                currentRule = make_pair(string(), vector<string>());

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
        ostringstream  buf;
        buf << "Unable to open " << fileName;
        throw buf.str();
    }
}

void createTree(Tree &tree, unordered_map<string, pair<string, vector<string>>> &rules, string target) {
    if (rules.find(target) == rules.end()) {
        ostringstream  buf;
        buf << "No rule " << target;
        throw buf.str();
    }

    Tree subTree(target);

    pair<string, vector<string>> rule = rules[target];

    vector<string>::iterator first = rule.second.begin();
    vector<string>::iterator last = rule.second.end();

    while (first != last) {
        createTree(subTree, rules, *first);
        ++first;
    }

    tree.addChild(subTree);
}