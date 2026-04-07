    #ifndef DOCMAGNITUDE_H
#define DOCMAGNITUDE_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

vector<double>doc_magn_calc(unordered_map<int, unordered_map<string, double>> &tfIdf);

#endif
