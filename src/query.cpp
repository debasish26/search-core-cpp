#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include <chrono>

#include "../include/tokenizer.h"
#include "../include/frequency.h"
#include "../include/heapOptmization.h"
#include "../include/sqlite3.h"

using namespace std;
#define nl '\n'

extern unordered_map<string,vector<pair<int,int>>> inverted_index;
extern unordered_map<string,int> df;
extern int total_docs;

string get_title(sqlite3* DB, int doc_id){

    string title = "";
    string query = "SELECT title FROM documents WHERE doc_id = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, doc_id);

    if(sqlite3_step(stmt) == SQLITE_ROW){
        title = (const char*)sqlite3_column_text(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return title;
}

void fetch_doc(sqlite3* DB, int doc_id){

    string query = "SELECT title, url FROM documents WHERE doc_id = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, doc_id);

    if(sqlite3_step(stmt) == SQLITE_ROW){
        string title = (const char*)sqlite3_column_text(stmt, 0);
        string url = (const char*)sqlite3_column_text(stmt, 1);

        cout<<"Title  : "<<title<<"\n";
        cout<<"URL    : "<<url<<"\n";
    }

    sqlite3_finalize(stmt);
}

string toLower(string s){
    for(char &c : s){
        c = tolower(c);
    }
    return s;
}

void query(){

    sqlite3* DB;
    if(sqlite3_open("../testdb.db", &DB)){
        cout<<"DB open error\n";
        return;
    }

    while(true){

        cout<<"\nEnter Query(Type exit2 for exit): ";
        string q;
        getline(cin,q);

        if(q == "exit2") break;
        if(q.empty()) continue;

        auto start = chrono::high_resolution_clock::now();

        vector<string> tokens = tokenizer(q);
        unordered_map<string,int> q_freq = frequency(tokens);

        unordered_map<int,double> scores;

        string full_query = toLower(q);

        double query_mag = 0.0;

        for(auto &term : q_freq){
            if(!df.count(term.first)) continue;

            double idf_val = log((double)total_docs / df[term.first]);
            double val = term.second * idf_val;

            query_mag += val * val;
        }

        query_mag = sqrt(query_mag);

        unordered_map<int,double> doc_mag;

        for(auto &term : q_freq){

            if(!inverted_index.count(term.first)) continue;

            double idf_val = log((double)total_docs / df[term.first]);

            for(auto &doc : inverted_index[term.first]){

                int doc_id = doc.first;
                int d_tf   = doc.second;

                double weight = d_tf * idf_val;

                scores[doc_id] += (term.second * weight);
                doc_mag[doc_id] += weight * weight;
            }
        }

        for(auto &s : scores){
            double d_mag = sqrt(doc_mag[s.first]);

            if(d_mag != 0 && query_mag != 0){
                s.second /= (d_mag * query_mag);
            }
        }

        for(auto &s : scores){

            string title = get_title(DB, s.first);
            string title_lower = toLower(title);
            for(auto &term : tokens){
                string term_lower = toLower(term);

                if(title_lower.find(term_lower) != string::npos){
                    s.second += 2.0;
                }
            }

            if(title_lower == full_query){
                s.second += 15.0;
            }

            if(title_lower.find(full_query) != string::npos){
                s.second += 5.0;
            }
        }

        vector<pair<double,int>> sc;
        for(auto &s : scores){
            sc.push_back({s.second, s.first});
        }

        int k = min(5, (int)sc.size());
        vector<pair<double,int>> sortedScore = heapOptmization(sc, k);

        sort(sortedScore.begin(), sortedScore.end(),
            [](const pair<double,int>& a, const pair<double,int>& b){
                if(a.first == b.first)
                    return a.second < b.second;
                return a.first > b.first;
            }
        );

        auto end = chrono::high_resolution_clock::now();
        double time_taken = chrono::duration<double>(end - start).count();

        /* ---------- OUTPUT ---------- */
        cout<<"\n========================================\n";
        cout<<"SEARCH RESULTS\n";
        cout<<"Query        : \""<<q<<"\"\n";
        cout<<"Docs Indexed : "<<total_docs<<"\n";
        cout<<"Results Found: "<<scores.size()<<"\n";
        cout<<"Time Taken   : "<<time_taken<<" sec\n";
        cout<<"========================================\n";

        int rank = 1;
        for(auto &s : sortedScore){

            cout<<"\n----------------------------------------\n";
            cout<<"Rank #" << rank << "\n";
            cout<<"Score  : " << s.first << "\n";

            fetch_doc(DB, s.second);

            rank++;
        }

        cout<<"\n========================================\n";
    }

    sqlite3_close(DB);
}