// System Headers
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

// User defined Headers
#include "../include/query.h"
#include "../include/deserialization.h"

using namespace std;
#define nl '\n'

/* Global Variables */

unordered_map<string,vector<pair<int,int>>> inverted_index;
unordered_map<string,int> df;
int total_docs = 0;

/*-------------------------------------------------------------------------*/

int main() {

    cout<<"Loading all segments..."<<nl;

    load_all_segments(inverted_index, df, total_docs);

    cout<<"Total Docs: "<<total_docs<<nl;
    cout<<"Unique Words: "<<df.size()<<nl;

    query();

    return 0;
}