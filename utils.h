#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>

// trim from start
std::string &ltrim(std::string &s);

// trim from end
std::string &rtrim(std::string &s);

// trim from both ends
std::string &trim(std::string &s);

// display vectors
template<typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &v) {
    out << "[";
    size_t last = v.size() - 1;
    for (size_t i = 0; i < v.size(); ++i) {
        out << v[i];
        if (i != last)
            out << ", ";
    }
    out << "]";
    return out;
}

// display pairs
template<typename T, typename U>
std::ostream &operator<<(std::ostream &out, const std::pair<T, U> &v) {
    out << "<" << v.first << "," << v.second << ">";
    return out;
}

#endif