#include <unordered_map>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
unordered_map<string, double>i;
unordered_map<string,double>idfCalc(unordered_map<string,vector<pair<int,int>>> &inverted_index,int &n){
    unordered_map<string,double> idf;
    for(auto &word: inverted_index){
        int df = word.second.size();
        idf[word.first] = log2((double)n/df);
    }
    i = idf;
    return idf;
}


unordered_map<int, unordered_map<string, double>>tfIdfCalc(unordered_map<int, unordered_map<string, int>> &frequencies){
    unordered_map<int, unordered_map<string, double>>tfIdf;
    for(auto &doc:frequencies){
        int doc_id = doc.first;
        for(auto &term: doc.second){
            string word = term.first;
            int tf = term.second;
            tfIdf[doc_id][word] = tf*i[word];
        }
    }
    
    return tfIdf;
}
