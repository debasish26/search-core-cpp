#ifndef TFIDFCALC_H
#define TFIDFCALC_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;
unordered_map<string,double>idfCalc(unordered_map<string,vector<pair<int,int>>> &inverted_index,int &n);
unordered_map<int, unordered_map<string, double>>tfIdfCalc(unordered_map<int, unordered_map<string, int>> &frequencies);

#endif
