#include "../include/sqlite3.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;
#define nl '\n'

int main(){

    sqlite3* DB;
    char* messageError = nullptr;

    if(sqlite3_open("../testdb.db", &DB)){
        cout<<"DB open error\n";
        return 0;
    }
    string createQuery =
        "CREATE TABLE IF NOT EXISTS documents("
        "doc_id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT, content TEXT, url TEXT);";

    if(sqlite3_exec(DB, createQuery.c_str(), NULL, 0, &messageError) != SQLITE_OK){
        cout<<"Error creating table: "<<messageError<<nl;
        sqlite3_free(messageError);
        return 0;
    }

    cout<<"Table ready"<<nl;

    string begin = "BEGIN TRANSACTION;";
    string end   = "COMMIT;";

    sqlite3_exec(DB, begin.c_str(), NULL, 0, &messageError);

    string insertQuery = "INSERT INTO documents(title,content,url) VALUES(?, ?, ?);";
    sqlite3_stmt* stmt;

    if(sqlite3_prepare_v2(DB, insertQuery.c_str(), -1, &stmt, NULL) != SQLITE_OK){
        cout<<"Prepare failed\n";
        return 0;
    }

    ifstream documents("../data/wikipedia/enwiki-latest-pages-articles.xml");

    string docline;

    // Controls
    int cnt = 0;
    int totalInserted = 0;
    int batch = 0;

    const int BATCH_SIZE = 50000;
    const int LIMIT = 3000000; 

    int foundText = 0;
    string title = "";
    string txt = "";
    string url = "";

    int isRedirect = 0;
    int isTitle = 0;
    int isText = 0;

    while(getline(documents, docline)){

        if(docline.find("<page>") != string::npos){
            title = "";
            txt = "";
            url = "";
            isRedirect = 0;
            isTitle = 0;
            isText = 0;
        }

        if(docline.find("<title") != string::npos){
            int pos = docline.find(">");
            int end = docline.find("</title>");
            if(pos != string::npos && end != string::npos){
                title = docline.substr(pos+1, end - (pos+1));
                isTitle = 1;
            }
        }

        if(docline.find("#REDIRECT") != string::npos){
            isRedirect = 1;
        }

        if(docline.find("<redirect") != string::npos){
            int start = docline.find("title=\"");
            if(start != string::npos){
                start += 7;
                int end = docline.find("\"", start);
                url = docline.substr(start, end - start);
            }
        }

        if(docline.find("<text") != string::npos){
            int pos = docline.find(">");
            if(pos != string::npos){
                txt += docline.substr(pos+1);
                txt += " ";
            }
            foundText = 1;
            isText = 1;
            continue;
        }

        if(foundText){
            txt += docline;
            txt += " ";
        }

        if(docline.find("</text>") != string::npos){
            foundText = 0;
        }

        if(docline.find("</page>") != string::npos){

            if(!isText || !isTitle || isRedirect){
                continue;
            }

            if(url.empty()){
                string base = "https://en.wikipedia.org/wiki/";
                string temp = title;
                for(char &c : temp){
                    if(c == ' ') c = '_';
                }
                url = base + temp;
            }

            sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, txt.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, url.c_str(), -1, SQLITE_TRANSIENT);

            if(sqlite3_step(stmt) != SQLITE_DONE){
                cout<<"Insert failed\n";
            }

            sqlite3_reset(stmt);

            cnt++;
            totalInserted++;

            if(totalInserted % 10000 == 0){
                cout<<"Inserted: "<<totalInserted<<nl;
            }

            if(totalInserted >= LIMIT){
                cout<<"\nReached limit of "<<LIMIT<<" documents. Stopping...\n";
                break;
            }

            if(cnt == BATCH_SIZE){
                batch++;
                cout<<"Batch "<<batch<<" committed ("<<totalInserted<<" docs)"<<nl;

                if(sqlite3_exec(DB, end.c_str(), NULL, 0, &messageError) != SQLITE_OK ||
                   sqlite3_exec(DB, begin.c_str(), NULL, 0, &messageError) != SQLITE_OK){
                    cout<<"Transaction error\n";
                }

                cnt = 0;
            }
        }
    }

    if(cnt > 0){
        sqlite3_exec(DB, end.c_str(), NULL, 0, &messageError);
        cout<<"Final batch committed ("<<totalInserted<<" docs)"<<nl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(DB);

    cout<<"\n✅ Total Inserted: "<<totalInserted<<nl;

    return 0;
}
