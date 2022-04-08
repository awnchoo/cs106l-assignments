#include <iostream>
#include <fstream>
#include <unordered_set>
#include <algorithm>

using std::cout;            using std::endl;
using std::string;          using std::unordered_set;

unordered_set<string> findWikiLinks(const string& page_html);
int main() {
    string filename;
    getline(std::cin,filename);

    std::ifstream input(filename,std::ifstream::in);
    string page_html="";
    string line;
    while(getline(input,line)){
        page_html+=line;
    }

    unordered_set<string> validlinks=findWikiLinks(page_html);
    for(auto link=validlinks.begin();link!=validlinks.end();link++){
        cout<<*link<<endl;
    }
    return 0;
}
unordered_set<string> findWikiLinks(const string& page_html){
    unordered_set<string> WikiLinks;

    const string generallinkstart="<a href=\"/wiki/";
    auto start=page_html.begin();
    auto end=page_html.end();
    while(start!=end){
        auto linkstart=std::search(start,end,generallinkstart.begin(),generallinkstart.end());
        if(linkstart==end){
            break;
        }
        auto linkend=std::find(linkstart+generallinkstart.size(),end,'"');
        string link(linkstart+generallinkstart.size(),linkend);
        if(std::all_of(link.begin(),link.end(),[](char c){return c!=':'&&c!='#';})){
                WikiLinks.insert(link);
        }
        start=linkend+1;
    }
    return WikiLinks;
};
