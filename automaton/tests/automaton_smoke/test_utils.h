#include<cstdio>
#include<iostream>
#include<vector>
#include<fstream>
#include "automaton.h"

struct AutomatonStates {
    int vertex_cnt; 
    int term_cnt;
    int a_cnt;
    int b_cnt;
    AutomatonStates() : vertex_cnt(0), term_cnt(0), a_cnt(0), b_cnt(0) {}
};


Automaton read_automaton(const char* file) {
    using namespace std;
    ifstream fin(file);

    int vertex_cnt, edges_cnt;
    fin >> vertex_cnt >> edges_cnt;

    vector<Edge> edges(edges_cnt); 
    for (int i = 0; i < edges_cnt; ++i) {
        int from, to;
        char letter;
        fin >> from >> to >> letter;
        edges[i] = (Edge(from, to, letter));
    } 
    int cnt_ends;
    fin >> cnt_ends;
    vector<int> terminates(cnt_ends);
    for(int i = 0; i < cnt_ends; ++i) 
        fin >> terminates[i]; 
    fin.close();
    return Automaton(edges, terminates);
}

void get_real_values(const Automaton& x, AutomatonStates& ans) {
    ans.vertex_cnt = x.get_size();
    ans.term_cnt = x.get_terminates().size();
    ans.a_cnt = 0;
    ans.b_cnt = 0;
    auto edges = x.get_edges();
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i].letter == 'a') ans.a_cnt++;
        if (edges[i].letter == 'b') ans.b_cnt++;
    }
}

void read_answer(const char* file, AutomatonStates &inp) {
    std::ifstream fin(file);
    fin >> inp.vertex_cnt >> inp.term_cnt >> inp.a_cnt >> inp.b_cnt;
    fin.close();
}
