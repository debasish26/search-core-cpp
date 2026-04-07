#ifndef DESERIALIZATION_H
#define DESERIALIZATION_H
#include <unordered_map>
#include <vector>
#include<string>
using namespace std;
void deserialize(unordered_map<string,vector<pair<int,int>>> &inverted_index);
void load_meta(unordered_map<string,int> &df, int &total_docs);
void load_all_segments(
    unordered_map<string,vector<pair<int,int>>> &inverted_index,
    unordered_map<string,int> &df,
    int &total_docs
);
#endif
