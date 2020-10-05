#include "automaton.h"
#include<cstdio>
#include<iostream>
using namespace std;
int main() {
    
    freopen("hw4/n1.txt", "r", stdin);

    int vertex_cnt, edges_cnt;
    int start;
    cin >> vertex_cnt >> edges_cnt;

    vector<Edge> edges(edges_cnt); 
    for (int i = 0; i < edges_cnt; ++i) {
        int from, to;
        char letter;
        cin >> from >> to >> letter;
        edges[i] = (Edge(from, to, letter));
    } 
    int cnt_ends;
    cin >> cnt_ends;
    vector<int> terminates(cnt_ends);
    for(int i = 0; i < cnt_ends; ++i) 
        cin >> terminates[i]; 

    Automaton x = Automaton(edges, terminates).minimalize();
    
    cout << x;

}
