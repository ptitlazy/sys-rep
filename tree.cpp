#include <iostream>
#include <sstream>
#include <vector>

#include "tree.h"

void Tree::addChild(Tree child) {
    this->children.push_back(child);
}

void Tree::display(std::ostream &out, int indent) const {
    out << std::string(indent * 4, ' ') << "|- " << this->name << std::endl;

    std::vector<Tree>::const_iterator first = this->children.begin();
    std::vector<Tree>::const_iterator last = this->children.end();

    while (first != last) {
        first->display(out, indent + 1);
        ++first;
    }
}

std::ostream &operator<<(std::ostream &out, const Tree t) {
    t.display(out, 0);
    return out;
}
