#ifndef SERIALIZATION_H
#define SERIALIZATION_H
#include <unordered_map>
#include <vector>
#include<string>
using namespace std;
void serialize(unordered_map<string,vector<pair<int,int>>> &inverted_index);
#endif
