#pragma once
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
    int c_cnt;
    AutomatonStates() : vertex_cnt(0), term_cnt(0), a_cnt(0), b_cnt(0) {}
    AutomatonStates(const Automaton &_a) {
        vertex_cnt = _a.get_size();
        term_cnt = _a.get_terminates().size();
        a_cnt = 0;
        b_cnt = 0;
        c_cnt = 0;
        for (Edge edge : _a.get_edges()) {
            if (edge.letter == 'a') a_cnt++;
            if (edge.letter == 'b') b_cnt++;
            if (edge.letter == 'c') c_cnt++;
        }   
    }
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

void read_answer(const char* file, AutomatonStates &inp) {
    std::ifstream fin(file);
    fin >> inp.vertex_cnt >> inp.term_cnt >> inp.a_cnt >> inp.b_cnt >> inp.c_cnt;
    fin.close();
}
