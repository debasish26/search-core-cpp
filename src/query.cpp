#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <algorithm>

#include "../include/tokenizer.h"
#include "../include/frequency.h"

using namespace std;
#define nl '\n'

extern unordered_map<string,vector<pair<int,int>>> inverted_index;
extern unordered_map<int, unordered_map<string,double>> tfIdf;
extern unordered_map<string,double> idf;
extern vector<double> doc_magnitude;
extern vector<string> documents;

void query(){

    string q;
    cout<<"Enter The Query You Want To Search : ";
    getline(cin,q);

    vector<string> tokens = tokenizer(q);
    unordered_map<string,int> q_freq = frequency(tokens);

    unordered_map<string,double> q_tfidf;

    for(auto &term : q_freq){
        string word = term.first;
        int tf = term.second;

        if(idf.count(word))
            q_tfidf[word] = tf * idf[word];
    }

    double q_mag = 0;
    for(auto &term : q_tfidf){
      q_mag += term.second * term.second;
    }
    q_mag = sqrt(q_mag);

    unordered_map<int,double> scores;

    for(auto &term: q_tfidf){
        string word = term.first;
        double q_weight = term.second;

        if(!inverted_index.count(word)){
            continue;
        }

        for(auto &doc:inverted_index[word]){
            int doc_id = doc.first;

            double d_weight = tfIdf[doc_id][word];
            scores[doc_id] += q_weight * d_weight;
        }
    }

    for(auto &score:scores){
        int doc_id = score.first;
        scores[doc_id] =  scores[doc_id]/(doc_magnitude[doc_id] * q_mag);
    }
    vector<pair<double,int>>sc;
    for(auto &s:scores){
        sc.push_back({s.second,s.first});
    }
    
    sort(sc.rbegin(),sc.rend());

    cout << "\n===== SEARCH RESULTS =====\n";
    for(auto &doc:sc){
        cout<<documents[doc.second]<<" "<<"->"<<" "<<doc.first<<nl;
    }
}

/*
 * need to store for each document need to calculate its value then store it
 * 
 *
 *
 */
