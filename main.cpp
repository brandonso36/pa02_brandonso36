// Spring'26
// Instructor: Diba Mirza
// Student name: Brandon So
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <cstring>
#include <algorithm>
#include <limits.h>
#include <iomanip>
#include <set>
#include <queue>
#include <unordered_map>
#include <sstream>
using namespace std;

#include "utilities.h"
#include "movies.h"

bool parseLine(string &line, string &movieName, double &movieRating);

int main(int argc, char** argv){
    if (argc < 2){
        cerr << "Not enough arguments provided (need at least 1 argument)." << endl;
        cerr << "Usage: " << argv[ 0 ] << " moviesFilename prefixFilename " << endl;
        exit(1);
    }

    ifstream movieFile (argv[1]);
 
    if (movieFile.fail()){
        cerr << "Could not open file " << argv[1];
        exit(1);
    }
  
    // Create an object of a STL data-structure to store all the movies

    string line, movieName;
    double movieRating;
    set<Movie, CompareAlphabetically> movieList;
    // Read each file and store the name and rating
    while (getline (movieFile, line) && parseLine(line, movieName, movieRating)){
            // Use std::string movieName and double movieRating
            // to construct your Movie objects
            // cout << movieName << " has rating " << movieRating << endl;
            // insert elements into your data structure
            movieList.insert({movieName, movieRating});
    }

    movieFile.close();

    if (argc == 2){
        for (auto const& m : movieList) {
            cout << m.name << ", " << m.rating << endl;
        }

        return 0;
    }

    ifstream prefixFile (argv[2]);

    if (prefixFile.fail()) {
        cerr << "Could not open file " << argv[2];
        exit(1);
    }

    vector<string> prefixes;
    while (getline (prefixFile, line)) {
        if (!line.empty()) {
            prefixes.push_back(line);
        }
    }

    //  For each prefix,
    //  Find all movies that have that prefix and store them in an appropriate data structure
    //  If no movie with that prefix exists print the following message
    for (string s : prefixes){
        Movie dummy(s, 0.0);

        auto it = lower_bound(movieList.begin(), movieList.end(), dummy, CompareAlphabetically()); // gemini suggested use of lower_bound to get first instance of match


        set<Movie> matches;
        while (it != movieList.end() && it->name.rfind(s, 0) == 0) {
            matches.insert(*it); // Add the matching movie to our list
            it++;                   // Move to the next movie in the database
        }
        
        if (matches.empty()) cout << "No movies found with prefix "<< s << endl;

        else{
            for (const auto& m : matches) {
                cout << m.name << ", " << m.rating << endl;
            }

            cout << endl;

            auto bestMovie = *matches.begin();
            cout << "Best movie with prefix " << s << " is: " 
                << bestMovie.name << " with rating " << bestMovie.rating << endl;
        }
    }
    
    return 0;
}

/* Add your run time analysis for part 3 of the assignment here as commented block

3a. My part 2 algorithm runs in O(m * (k * log(k) + log(n))) time. The for loop repeats for every prefix in 
the array which is m. The lower bound algorithm finds the first instance of a match in worst case
O(log(n)) time. The while loop runs until a match is found which means it would run k times for 
O(k) time. Insertion of k elements into the set would take logarithmic time for each instance, but 
overall would take O(k * log(k)) time. Overall this gives O(k * log(k) + log(n) + k) time complexity.
This simplifies to O(k * log(k) + log(n)) time. For the entire algorithm, this takes O(m * (k * log(k) + log(n)))
time complexity.

Actual Running Times: (using prefix_large.txt)
input_20_random.csv: 21 milliseconds
input_100_random.csv: 29 milliseconds
input_1000_random.csv: 199 milliseconds
input_76920_random.csv: 66105 milliseconds

3b. My part 2 algorithm runs in O(k * l) worst-case auxiliary space complexity. 
The loop runs m times, but because the std::set is local, its memory is freed 
each iteration and never accumulates. A dummy object takes O(l) space, and 
inserting k matches into the set takes O(k * l) space to store the titles. This 
simplifies to O(k * l) space. In the absolute worst case where every movie 
matches the prefix, k equals n, resulting in O(n * l) space.

3c. I designed my algorithm to focus on both low time complexity and low space 
complexity. My target complexities were O(m * (log(n) + k * log(k))) for time 
and O(n) total space (with an extra runtime footprint of O(k * l)).

I was able to achieve both. By keeping the database in a std::set, I kept total 
space at O(n) while automatically keeping the data sorted. At the same time, I 
got a fast runtime because the set allows me to instantly jump to the first 
matching prefix section in O(log(n)) time using tree traversal, and then I used 
a while loop to grab and sort the k actual matches.

Low time complexity was definitely harder to get. Low space complexity was 
pretty straightforward because storing the data once naturally keeps memory low. 
The real challenge was making sure the search loop ran fast for thousands of 
prefixes without accidentally adding slow operations, like duplicating data or 
creating heavy string copies inside the loop. Making sure the code cleared the 
performance benchmarks made time optimization the tougher task.

*/

bool parseLine(string &line, string &movieName, double &movieRating) {
    int commaIndex = line.find_last_of(",");
    movieName = line.substr(0, commaIndex);
    movieRating = stod(line.substr(commaIndex+1));
    if (movieName[0] == '\"') {
        movieName = movieName.substr(1, movieName.length() - 2);
    }
    return true;
}