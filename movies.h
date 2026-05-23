#ifndef MOVIES_H
#define MOVIES_H

#include <string>

struct Movie {
    std::string name;
    double rating;

    Movie(std::string n, double r);
    bool operator<(const Movie& other) const;
    bool operator>(const Movie& other) const;
};

struct CompareAlphabetically {
    bool operator()(const Movie& a, const Movie& b) const;
};

#endif