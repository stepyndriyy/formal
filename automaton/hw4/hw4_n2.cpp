#include "automaton.h"
#include<cstdio>

using namespace std;
int main() {
    
    freopen("hw4/n2.txt", "r", stdin);

    //input
    int vertex_cnt, edges_cnt;
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



    Automaton x = Automaton(edges, terminates).determinizate();
    x = x.to_full('b' - 'a' + 1);
    x.new_terminates(x.addition_terminates());
    x = x.minimalize();

    cout << x;
   
}
