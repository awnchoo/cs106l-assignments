/*
 * TODO: complete this file comment.
 */
#include <iostream>
#include <fstream>
#include "SimpleGraph.h"
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>
const double kPi = 3.14159265358979323;

using namespace std;

void Welcome();
void creategraph(SimpleGraph& graph);
void initnodes(ifstream& input,SimpleGraph& graph);
void initedges(ifstream& input,SimpleGraph& graph);
bool getInteger(ifstream& input,int & n);
double runtime();
void computeforces(SimpleGraph& graph);
// Main method
int main() {
    Welcome();
    SimpleGraph graph;
    creategraph(graph);
    InitGraphVisualizer(graph);
    double needtime=runtime();
    time_t starttime=time(NULL);
    double elapsedtime=0;
    while(elapsedtime<needtime){
        computeforces(graph);
        DrawGraph(graph);
        elapsedtime = difftime(time(NULL),starttime);
    }
    DrawGraph(graph);
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}
void creategraph(SimpleGraph& graph){
    cout<<"which file to read?";
    string filename;
    getline(cin,filename);
    ifstream input(filename);
    while(!input.is_open()){
        cout<<"unable to read that file. Try again.\n";
        getline(cin,filename);
        input.open(filename);
    }
    initnodes(input,graph);
    initedges(input,graph);
}
void initnodes(ifstream& input,SimpleGraph& graph){
    int n=0;
    getInteger(input,n);
    for(int i=0;i<n;i++){
       struct Node nodei={cos(2*kPi*i/n),sin(2*kPi*i/n)};
       graph.nodes.push_back(nodei);
    }

};
void initedges(ifstream& input,SimpleGraph& graph){
    int startu,endu;
    unsigned start,end;
    while(getInteger(input,startu)&&getInteger(input,endu)){
        start=startu;
        end=endu;
        struct Edge edgei={start,end};
        graph.edges.push_back(edgei);
     }

};
bool getInteger(ifstream& input,int & n){
    string num;
    stringstream instring;
    input>>num;
    instring<<num;
    if(instring>>n){
        return true;
    }else{
        return false;
    }
};
double runtime(){
    cout<<"how long you want this program to run?";
    string time_s;
    double time;
    stringstream instring;
    cin>>time_s;
    instring<<time_s;
    while(!(instring>>time)){
        cout<<"Enter a double number";
        cin.clear();
        cin.ignore(99,'\n');
        cin>>time_s;
        instring<<time_s;
    }
    return time;
};
void computeforces(SimpleGraph& graph){
    double deltaxy[graph.nodes.size()][2];
    const double k=10e-3;
    for(unsigned i =0;i<graph.nodes.size();i++){
        for(int j=0;j<2;j++){
            deltaxy[i][j]=0;
        }
    }
    for(unsigned i =0;i<graph.nodes.size();i++){
        for(unsigned j =0;j<graph.nodes.size();j++){
            if(j!=i){
                double frepel = 0.5*k/sqrt(pow((graph.nodes[i].y-graph.nodes[j].y),2)+
                                           pow((graph.nodes[i].x-graph.nodes[j].x),2));
                double theta = atan2((graph.nodes[i].y-graph.nodes[j].y),(graph.nodes[i].x-graph.nodes[j].x));
                deltaxy[i][0]+=frepel*cos(theta);
                deltaxy[i][1]+=frepel*sin(theta);
                deltaxy[j][0]-=frepel*cos(theta);
                deltaxy[j][1]-=frepel*sin(theta);
            }
         }
     }
     for(unsigned i =0;i<graph.edges.size();i++){
         unsigned start,end;
         start=graph.edges[i].start;
         end=graph.edges[i].end;
         double fattract = k*(pow((graph.nodes[start].x-graph.nodes[end].x),2)+
                              pow((graph.nodes[start].y-graph.nodes[end].y),2));
         double theta = atan2((graph.nodes[start].y-graph.nodes[end].y),
                              (graph.nodes[start].x-graph.nodes[end].x));
         deltaxy[start][0] -=fattract*cos(theta);
         deltaxy[start][1] -=fattract*sin(theta);
         deltaxy[end][0] +=fattract*cos(theta);
         deltaxy[end][1] +=fattract*sin(theta);
     }
     for(unsigned i =0;i<graph.nodes.size();i++){
         graph.nodes[i].x+=deltaxy[i][0];
         graph.nodes[i].y+=deltaxy[i][1];
     }
};

