#include <iostream>
#include <list>
#include <string>

/*
 * Class Tree enable representing a tree of depedencies
 */
class Tree
{
public:
        Tree();

private:
        int id;
        std::string cmd;
        std::string file;
        std::list<Tree> children;
}
