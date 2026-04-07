#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

#define nl '\n'


void deserialize_segment(
    unordered_map<string,vector<pair<int,int>>> &inverted_index,
    string filename
){
    ifstream in(filename, ios::binary);

    if(!in){
        return;
    }

    while(true){
        int len;

        if(!in.read((char*)&len, sizeof(len))) break;

        string word(len, ' ');
        in.read(&word[0], len);

        int count;
        in.read((char*)&count, sizeof(count));

        vector<pair<int,int>> posting(count);

        for(int i = 0; i < count; i++){
            in.read((char*)&posting[i].first, sizeof(posting[i].first));
            in.read((char*)&posting[i].second, sizeof(posting[i].second));
        }

        for(auto &p : posting){
            inverted_index[word].push_back(p);
        }
    }

    cout<<"Loaded index: "<<filename<<nl;
}


void load_meta_segment(
    unordered_map<string,int> &df,
    int &total_docs,
    string filename
){
    ifstream in(filename, ios::binary);

    if(!in){
        return;
    }

    int temp_docs;
    in.read((char*)&temp_docs, sizeof(temp_docs));

    total_docs += temp_docs; 
    int size;
    in.read((char*)&size, sizeof(size));

    for(int i = 0; i < size; i++){
        int len;
        in.read((char*)&len, sizeof(len));

        string word(len, ' ');
        in.read(&word[0], len);

        int freq;
        in.read((char*)&freq, sizeof(freq));

        df[word] += freq; // 🔥 merge df
    }

    cout<<"Loaded meta: "<<filename<<nl;
}

void load_all_segments(
    unordered_map<string,vector<pair<int,int>>> &inverted_index,
    unordered_map<string,int> &df,
    int &total_docs
){
    int segment_id = 1;
    int MAX_SEGMENTS = 9;  

    while(segment_id <= MAX_SEGMENTS){

        string idx  = "../segments/seg/segment_" + to_string(segment_id) + ".bin";
        string meta = "../segments/meta/meta_" + to_string(segment_id) + ".bin";

        ifstream test(idx);
        if(!test) break;

        deserialize_segment(inverted_index, idx);
        load_meta_segment(df, total_docs, meta);

        segment_id++;
    }

    cout<<"Loaded "<<segment_id-1<<" segments only (limited mode)"<<nl;
}