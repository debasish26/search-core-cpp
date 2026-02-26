#include<vector>
#include<string>

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
        if(alnum(text[i]) == 1){
            current_word += text[i];
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
