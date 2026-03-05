// serialize index to disk - making a binary file with above entries in the  disk so that accessing it can be easy as everytime if we create the inverted_index its highly expensive in terms of cost
/*
 Entries will be :
 for each word:
 ```
 [word length]
 [word]
 [number of posting]

 [doc id]
 [frequency]

 [doc id]
 [frequency]
 ```

 */

#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
#define nl '\n'

void serialize(unordered_map<string,vector<pair<int,int>>> &inverted_index){
    ofstream out("../index.bin",ios::binary);
    for(auto &x:inverted_index){
        string word = x.first;
        auto &posting = x.second;

        int len = word.size();
        out.write((char*)&len, sizeof(len));
        out.write(word.c_str(), len);

        int count = posting.size();
        out.write((char*)&count, sizeof(count));

        for(auto &p : posting){
            out.write((char*)&p.first, sizeof(p.first));
            out.write((char*)&p.second, sizeof(p.second));
        }
    }

    cout<<"Bin created"<<nl;
}
