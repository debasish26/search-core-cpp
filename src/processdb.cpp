// System Headers
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <string>
#include <cmath>

// User defines Headers
#include "../include/tokenizer.h"
#include "../include/frequency.h"
#include "../include/serialization.h"
#include "../include/tfIdfcalc.h"
#include "../include/docmagnitude.h"
#include "../include/benchmarks.h"
#include "../include/query.h"
#include "../include/sqlite3.h"
#include "../include/deserialization.h"

using namespace std;
#define nl '\n'

/* Global Variables */

unordered_map<string,vector<pair<int,int>>> inverted_index;
vector<string> documents;
vector<double> doc_magnitude;
unordered_map<int, unordered_map<string, int>> frequencies;
unordered_map<string,double> idf;
unordered_map<int, unordered_map<string, double>> tfIdf;
unordered_map<string,int> df;
int total_docs = 0;
int NoOfFiles;

/*-------------------------------------------------------------------------*/

void load_from_db(sqlite3* DB){

    string query = "SELECT doc_id, content FROM documents;";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL) != SQLITE_OK){
        cout<<"Query prepare failed"<<nl;
        return;
    }else{
        cout<<"Query retrieved"<<nl;
    }

    int batch_size = 50000;
    int count = 0;
    int segment_id = 1;

    while(sqlite3_step(stmt) == SQLITE_ROW){

        int doc_id = sqlite3_column_int(stmt, 0);

        const unsigned char* c = sqlite3_column_text(stmt, 1);
        string content = c ? (const char*)c : "";

        if(content.size() > 5000){
            content = content.substr(0, 5000);
        }

        vector<string> tokens = tokenizer(content);
        unordered_map<string,int> frequencie = frequency(tokens);

        total_docs++;

        if(total_docs % 1000 == 0){
            cout<<"Processed: "<<total_docs<<nl;
        }

        for(auto &word:frequencie){
            inverted_index[word.first].push_back({doc_id, word.second});
            df[word.first]++;
        }

        count++;

        if(count == batch_size){

            string idx  = "../segments/seg/segment_" + to_string(segment_id) + ".bin";
            string meta = "../segments/meta/meta_" + to_string(segment_id) + ".bin";

            serialize(inverted_index, idx);
            save_meta(df, total_docs, meta);

            cout<<"Segment "<<segment_id<<" created ("<<total_docs<<" docs)"<<nl;

            inverted_index.clear();
            df.clear();

            count = 0;
            segment_id++;
        }
    }

    if(!inverted_index.empty()){
        string idx  = "../segments/seg/segment_" + to_string(segment_id) + ".bin";
        string meta = "../segments/meta/meta_" + to_string(segment_id) + ".bin";

        serialize(inverted_index, idx);
        save_meta(df, total_docs, meta);

        cout<<"Final Segment "<<segment_id<<" created"<<nl;
    }

    sqlite3_finalize(stmt);
}
/*-------------------------------------------------------------------------*/

void processDB() {

    sqlite3* DB;
    int check = sqlite3_open("../testdb.db",&DB);

    if(check){
        cout<<"DB open error"<<nl;
        return;
    }else{
        cout<<"DB is opened"<<nl;
    }

    // ❌ OLD FILE READING (DISABLED)
    /*
    vector<string>fileNames;
    string x;

    ifstream txtFiles("../data/fileNames.txt");
    while(getline(txtFiles,x)){
        fileNames.push_back(x);
    }

    for(auto x: fileNames){
        cout<<x<<" ";
    }
    cout<<nl;

    NoOfFiles = (int)fileNames.size();

    for(int i =0;i<(int)fileNames.size();i++){
        documents.push_back(fileNames[i]);
    }

    for(int i =0;i<(int)fileNames.size();i++){
        int doc_id = i;
        const string filename = "../data/"+fileNames[i];
        ifstream file(filename);
        string s = "";
        string line;
        while(getline(file,line)){
            s += line;
            s += nl;
        }

        vector<string> tokens = tokenizer(s);
        unordered_map<string,int> frequencie = frequency(tokens);
        frequencies[doc_id] = frequencie;

        for(auto &word:frequencie){
            inverted_index[word.first].push_back({i,word.second});
        }
    }
    */

    
    cout<<"Loading from DB..."<<nl;
    
    load_from_db(DB);
    
    cout<<"Total Docs Processed: "<<total_docs<<nl;

    // ❌ OLD TF-IDF (DISABLED)
    /*
    idf = idfCalc(inverted_index,NoOfFiles);
    tfIdf = tfIdfCalc(frequencies);
    doc_magnitude = doc_magn_calc(tfIdf);
    */


    sqlite3_close(DB);
}
 

int main(){
    processDB();
    
    return 0;
}