#include "movies.h"
// In movies.h

Movie::Movie(std::string n, double r) : name(n), rating(r) {}
    
bool Movie::operator<(const Movie& other) const {
    if (rating != other.rating) {
        return rating > other.rating;
    }
    return name < other.name;
}

bool Movie::operator>(const Movie& other) const {
    return other < *this;
}

bool CompareAlphabetically::operator()(const Movie& a, const Movie& b) const {
    return a.name < b.name;
}