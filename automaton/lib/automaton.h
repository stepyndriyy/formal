#pragma once
#include<algorithm>
#include<vector>
#include<iostream>
#include<queue>
#include<set>
#include<utility>
#include<map>

struct Edge {
    int from = -1;
    int to = -1;
    char letter = '-';    
    
    Edge() {}
    Edge(const Edge& other) : from(other.from), to(other.to), letter(other.letter) {}
    Edge(int _f, int _t, char _l) : from(_f), to(_t), letter(_l) {}
    
    bool operator<(const Edge& other) const {
        return ((from < other.from) ||
            ((from == other.from) && (to < other.to)) || 
                ((from == other.from) && (to == other.to) && (letter < other.letter)));
    }
};

class Automaton {
private:
    const int ALP_SZ = 'z' - 'a' + 1;
    int VERTEX_CNT = 0;
    std::vector<Edge> edges;
    std::vector<int> terminate_v;
    
    Automaton& normalize_vertexes();

public:
        
    Automaton(const std::vector<Edge>& _edges, 
            const std::vector<int>& _terminate);
    int get_size() const {return VERTEX_CNT;} 
    std::vector<Edge> get_edges() const {return edges;}
    std::vector<int> get_terminates() const {return terminate_v;}
 
    std::vector<int> find_reachable() const; 
    std::vector<std::vector<int> > build_table() const;
    std::vector<std::vector<std::vector<int> > > 
        form_letter_to(bool reversed) const;
    Automaton determinizate() const;
    Automaton minimalize() const;
};



std::vector<std::vector<int> > Automaton::build_table() const {
    auto gr_r = this->form_letter_to(true);
    std::vector<int> terminal(VERTEX_CNT);
    for (int i = 0; i < terminate_v.size(); ++i) 
        terminal[terminate_v[i]] = 1;
    std::queue<std::pair<int, int> > Q;
    std::vector<std::vector<int> > marked(VERTEX_CNT, 
            std::vector<int> (VERTEX_CNT, 0)); 
    
    
    for (int i = 0; i < VERTEX_CNT; ++i) {
        for (int j = 0; j < VERTEX_CNT; ++j) {
            if (!marked[i][j] && terminal[i] != terminal[j]) {
                marked[i][j] = marked[j][i] = 1;
                Q.push(std::make_pair(i,j));
            }
        }
    }

    while (!Q.empty()) {
        int u = Q.front().first;
        int v = Q.front().second;
        Q.pop();
        for (int c = 0; c < ALP_SZ; ++c) {
            for (int i = 0; i < gr_r[v][c].size(); ++i) {
                for (int j = 0; j < gr_r[u][c].size(); ++j) {
                    int r = gr_r[u][c][j];
                    int s = gr_r[v][c][i];
                    if (!marked[r][s]) {
                        marked[r][s] = marked[s][r] = 1;
                        Q.push(std::make_pair(r, s)); 
                    } 
                }
            }
        }
    }
    return marked;
} 

std::vector<int> Automaton::find_reachable() const {
    auto gr = this->form_letter_to(false);
    std::queue<int> Q;
    std::vector<int> used(VERTEX_CNT, 0);
    Q.push(0);
    used[0] = 1;
    while (!Q.empty()) {
        int v = Q.front();
        Q.pop();
        for (int c = 0; c < ALP_SZ; ++c) {
            for (int i = 0; i < gr[v][c].size(); ++i) {
                int u = gr[v][c][i];
                if (used[u]) continue;
                used[u] = 1;
                Q.push(u);
            }
        }
    }
    return used;
}

Automaton Automaton::minimalize() const {
    Automaton DFA = this->determinizate();
    auto marked = DFA.build_table();
    auto reachable = DFA.find_reachable();
    int n = DFA.get_size();
    std::vector<int> component(n, -1);
    
    for (int i = 0; i < n; ++i) {
        if (!marked[0][i])
            component[i] = 0;
    }
    int components_cnt = 0;
    for (int i = 0; i < n; ++i) {
        if (!reachable[i]) continue;
        if (component[i] != -1) continue;
        
        components_cnt++;
        component[i] = components_cnt;
        for (int j = i + 1; j < n; ++j) {
            if (!marked[i][j]) {
                component[j] = components_cnt;
            }
        }      
    }
        
    std::set<Edge> new_edges;
    auto dfa_edges = DFA.get_edges();
    for (int i = 0; i < dfa_edges.size(); ++i) {
        int from = dfa_edges[i].from;
        int to = dfa_edges[i].to;
        char letter = dfa_edges[i].letter;
        new_edges.insert(Edge(component[from], component[to], letter));
    }
    std::vector<Edge> answer;
    for (auto it = new_edges.begin(); it != new_edges.end(); ++it) {
        answer.push_back(*it);
    }

    std::set<int> new_terminate;
    std::vector<int> terminate_answer;
    auto dfa_terminate = DFA.get_terminates();
    for (int i = 0; i < dfa_terminate.size(); ++i) {
        int v = dfa_terminate[i];
        new_terminate.insert(component[v]);
    }
    for (auto it = new_terminate.begin(); it != new_terminate.end(); ++it) {
        terminate_answer.push_back(*it);
    }
    return Automaton(answer, terminate_answer);
}

std::vector<std::vector<std::vector<int> > > 
Automaton::form_letter_to(bool reversed = false) const {
    std::vector<std::vector<std::vector<int> > > gr(edges.size(), 
        std::vector<std::vector<int> > ('z' - 'a' + 1, std::vector<int>(0)));
    for (int i = 0; i < edges.size(); ++i) {
        if (!reversed) 
            gr[edges[i].from][edges[i].letter - 'a'].
                push_back(edges[i].to);
        else
            gr[edges[i].to][edges[i].letter - 'a'].
                push_back(edges[i].from);

    }
    return gr;
}

Automaton& Automaton::normalize_vertexes() {
    std::map<int, int> reform;
    int cnt = 1;
    int start = edges[0].from;
    for (int i = 0; i < edges.size(); ++i) 
        start = std::min(std::min(edges[i].from, edges[i].to), start);
    reform[start] = 0;
    for (int i = 0; i < edges.size(); ++i) {
        if (reform.find(edges[i].from) == reform.end()) 
            reform[edges[i].from] = cnt++;
        if (reform.find(edges[i].to) == reform.end()) 
            reform[edges[i].to] = cnt++;
        edges[i].from = reform[edges[i].from];
        edges[i].to = reform[edges[i].to];        
    }
    for (int i = 0; i < terminate_v.size(); ++i) {
        terminate_v[i] = reform[terminate_v[i]];
    }
    return *this;
}

Automaton::Automaton(const std::vector<Edge>& _edges, const std::vector<int>& _terminate) {
    edges = _edges;
    terminate_v = _terminate;
    
    normalize_vertexes();
    for (int i = 0; i < edges.size(); ++i) {
        VERTEX_CNT = std::max(std::max(edges[i].from, 
                    edges[i].to) + 1, VERTEX_CNT);
    }
}

Automaton Automaton::determinizate() const {
    auto gr = this->form_letter_to();
    std::set<int> used;
    std::queue<int> q;
    std::vector<Edge> answer; 
    std::vector<int> new_vertexes;
    q.push(1);
    used.insert(1);
    new_vertexes.push_back(1);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        
        for (int c = 0; c <= ('z' - 'a'); ++c) {
            int next = 0;
            for (int i = 0; i <= 30; i++) {
                
                if (((1<<i) & v) == 0) continue;
                
                int cur = i;
                for (int j = 0; j < gr[cur][c].size(); ++j) {
                    next |= (1<<gr[cur][c][j]);
                }
                    
            }
            if (next > 0) {
                answer.push_back(Edge(v, next, (char)(c + 'a')));
                if (used.find(next) == used.end()) {
                    used.insert(next);
                    q.push(next);
                    new_vertexes.push_back(next);
                }
            }    
        }   
    }
    std::vector<int> pointed(new_vertexes.size(), 0), lasts;
    for (int i = 0; i < terminate_v.size(); ++i) {
        for (int j = 0; j < new_vertexes.size(); ++j) {
            if (((1<<terminate_v[i]) & new_vertexes[j]) != 0) {
                pointed[j] = 1;
            }
        } 
    }
    for (int i = 0; i < pointed.size(); ++i) {
        if (pointed[i])
            lasts.push_back(new_vertexes[i]);
    }

    return Automaton(answer, lasts);
}





