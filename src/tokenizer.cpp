#include<vector>
#include<string>
#include<cctype>
using namespace std;
using ll = long long;

bool alnum(char c){
    if((c >='a' && c <='z' || c>='A' && c<= 'Z')){
        return true;
    }else if(c>='0' && c<='9'){
        return true;
    }else{
        return false;
    }
}

vector<string> tokenizer(string &text){
    ll n = (ll)text.size();
    string current_word = "";
    vector<string> tokens;
    for(ll i =0;i<n;i++){
        if(alnum(text[i])){
            current_word += tolower(text[i]);
        }else{
            if(current_word.size() != 0){
                tokens.push_back(current_word);
                current_word = "";
            }
        }
    }
    if(current_word.size() !=0){
        tokens.push_back(current_word);
    }

    return tokens;
}


/*
Tokenizer :
TODO (Future Improvements):

1. Replace ASCII-only alnum logic with full UTF-8 / Unicode-aware tokenization.
2. Preserve meaningful tokens like "C++", "e-mail", "don't", "3.14".
3. Add optional stopword filtering (the, is, and, of, etc.).
4. Add stemming / lemmatization (running -> run, dogs -> dog).
5. Convert to streaming architecture (process large files without loading entire document).
6. Benchmark and optimize allocations (reserve, string_view, memory pooling).
7. Consider parallel tokenization for very large datasets.

Current version: ASCII-based, lowercase normalization, Θ(N) time, O(N) space.

*/
