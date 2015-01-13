#include <iostream>
#include <sstream>
#include <vector>
#include <string>

/*
 * Class Tree enable representing a tree of depedencies
 */
class Tree
{
public:
        Tree(std::string name) : name(name) {
        }

        void addChild(Tree child);
private:
        std::string name;
        std::string cmd;
        std::string file;
        std::vector<Tree> children;

        void display(std::ostream& out, int indent) const;

        friend std::ostream& operator<< (std::ostream& out, const Tree t);
};

void Tree::addChild(Tree child) {
        this->children.push_back(child);
}

std::ostream& operator<< (std::ostream& out, const Tree t) {
        t.display(out, 0);
        return out;
}

void Tree::display(std::ostream& out, int indent) const {
        out << std::string(indent * 4, ' ') << "|- " << this->name << std::endl;

        std::vector<Tree>::const_iterator first = this->children.begin();
        std::vector<Tree>::const_iterator last = this->children.end();

        while (first != last) {
                first->display(out, indent + 1);
                ++first;
        }
}
