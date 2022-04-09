
#include <iostream>
#include <unordered_set>
#include <vector>
#include <queue>
#include <unordered_map>
#include "wikiscraper.h"
#include <algorithm>

using std::cout;            using std::endl;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;




/*
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */
int similarcount(const unordered_set<string> &page_set,const unordered_set<string> &end_set);

vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    WikiScraper Scraper;
    auto end_set=Scraper.getLinkSet(end_page);
    auto cmpF = [&Scraper,&end_set](vector<string> ladder1,vector<string> ladder2){
        string page1=ladder1.back();
        string page2=ladder2.back();
        auto page1_set=Scraper.getLinkSet(page1);
        auto page2_set=Scraper.getLinkSet(page2);
        int num1=similarcount(page1_set,end_set);
        int num2=similarcount(page2_set,end_set);
        return num1<num2;
    };
    std::priority_queue<vector<string>,vector<vector<string>>,decltype(cmpF)> ladderQueue(cmpF);
    vector<string> ladder;
    ladder.push_back(start_page);
    ladderQueue.push(ladder);
    while(!ladderQueue.empty()){
        vector<string> currentladder=ladderQueue.top();
        ladderQueue.pop();
        unordered_set<string> used_set;
        unordered_set<string> current_set=Scraper.getLinkSet(currentladder.back());
        if(current_set.find(end_page)!=current_set.end()){
            cout<<"we find a ladder!"<<endl;
            currentladder.push_back(end_page);
            return currentladder;
        }
        for(auto page:current_set){
            if(used_set.find(page)==used_set.end()){
                vector<string> newladder=currentladder;
                newladder.push_back(page);
                ladderQueue.push(newladder);
                used_set.insert(page);
            }
        }
    }
    cout<<"find no way"<<endl;
    return {};
}

int similarcount(const unordered_set<string> &page_set,const unordered_set<string> &end_set){
    int num=0;
    for(auto link:end_set){
        num+=std::count(page_set.begin(),page_set.end(),link);
    }
    return num;
}

int main() {
    auto ladder = findWikiLadder("Milkshake", "Gene");
    cout << endl;

    if(ladder.empty()) {
        cout << "No ladder found!" << endl;
    } else {
        cout << "Ladder found:" << endl;
        cout << "\t"<<"{";
        for(auto link:ladder){
            cout<<link<<",";
        }
        cout<<"}"; // Print the ladder here!
    }

    return 0;
}




