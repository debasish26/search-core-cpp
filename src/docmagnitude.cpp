#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

vector<double>doc_magn_calc(unordered_map<int, unordered_map<string, double>> &tfIdf){
    vector<double>doc_norms;
    doc_norms.resize(tfIdf.size());
    for(auto &doc:tfIdf){
        int doc_id = doc.first;
        double sum = 0;
        for(auto &term: doc.second){
            double w = term.second;
            sum += w*w;
        }
        doc_norms[doc_id] = sqrt(sum);
    }

    return doc_norms;
}
