#ifndef DOCNORMS_H
#define DOCNORMS_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

vector<double>doc_norms_calc(unordered_map<int, unordered_map<string, double>> &tfIdf);

#endif
