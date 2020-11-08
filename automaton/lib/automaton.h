#pragma once
#include<stdexcept>
#include<stack>
#include<algorithm>
#include<iterator>
#include<ostream>
#include<vector>
#include<iostream>
#include<queue>
#include<set>
#include<utility>
#include<map>
#include<string>

static const int ALPHABET_SIZE = 'z' - 'a' + 1;

struct Edge {
    int from = -1;
    int to = -1;
    char letter = '-';    
    
    Edge() {}
    Edge(const Edge& other) : from(other.from), to(other.to), letter(other.letter) {}
    Edge(int _f, int _t, char _l) : from(_f), to(_t), letter(_l) {}
    Edge(int _f, int _t) : from(_f), to(_t) {}

    bool operator<(const Edge& other) const {
        return ((from < other.from) ||
            ((from == other.from) && (to < other.to)) || 
                ((from == other.from) && (to == other.to) && (letter < other.letter)));
    }
};

typedef std::vector<std::vector<std::vector<int> > > VVVI ;

class Automaton {
private:
    int VERTEX_CNT = 0;
    std::vector<Edge> edges;
    std::vector<int> terminate_v;

    Automaton(); 
    Automaton(Edge _edge);    
    
    Automaton& normalize_vertexes();
    void find_achievable_vertexes(VVVI& gr, int v, std::vector<int>& used /* used[i] == 2 => dummy*/) const;
    std::vector<int> find_reachable() const; 
    std::vector<Edge> find_incoming_edges(const int vertex_num) const; 
    std::vector<std::vector<int> > build_table_not_equal_states() const;
    int max_letter_num() const;
public:
    static char NumberToLetter(int num) {return static_cast<char>(num + 'a');}
    static int LetterToNumber(char letter_number) {return static_cast<int>(letter_number - 'a');}

    //todo
    //add && constructors
    Automaton(const std::vector<Edge>& _edges, const std::vector<int>& _terminate);
    Automaton(const Automaton& other); 
    Automaton& operator=(const Automaton& other);
    Automaton(const std::string regular_exp_polish_form);

    int get_size() const {return VERTEX_CNT;} 
    std::vector<Edge> get_edges() const {return edges;}
    std::vector<int> get_terminates() const {return terminate_v;}
    
    Automaton& operator+=(const Automaton& other); // sum of language
    Automaton& operator^=(const Automaton& other); // concatenation of language (AB)
    Automaton operator+(const Automaton& other) const;
    Automaton operator^(const Automaton& other) const;
    Automaton looped() const; // applying of star Klini 
    
    Automaton delete_dummy_vertexes() const;
    std::vector<int> addition_terminates() const;
    Automaton& new_terminates(const std::vector<int>& _new);

    VVVI form_letter_to(bool reversed) const;
    
    Automaton to_full(int letter_cnt/*number of letters from 'a'*/) const;
    Automaton determinizate() const;
    Automaton minimalize() const; 

    friend std::ostream& operator<<(std::ostream &stream, const Automaton &A); 
};

void Automaton::find_achievable_vertexes(VVVI& gr, int vertex, std::vector<int>& used) const {
    used[vertex] = 1;
    for (int terminal : terminate_v) {
        if (vertex == terminal) {
            used[vertex] = 2;
            break;
        }
    }
    for (int letter_number = 0; letter_number < ALPHABET_SIZE; ++letter_number) {
        for (int to : gr[vertex][letter_number]) {
            if (!used[to]) {
                find_achievable_vertexes(gr, to, used);
            }
            if (used[to] == 2) {
                used[vertex] = 2;
            }
        }
    } 
}

Automaton Automaton::delete_dummy_vertexes() const {
    VVVI gr = form_letter_to(false);
    std::vector<int> used(VERTEX_CNT, 0);
    find_achievable_vertexes(gr, 0, used);
    std::vector<Edge> new_edges;
    for (Edge edge : edges) {
        bool bad = false;
        for (int vertex = 0; vertex < VERTEX_CNT; ++vertex) {
            if ((used[vertex] == 1 || used[vertex] == 0) && (edge.from == vertex || edge.to == vertex)) {
                bad = true;            
            }
        }
        if (!bad) {
            new_edges.push_back(edge); 
        }
    } 
    return Automaton(new_edges, terminate_v);
}


Automaton::Automaton() {
    VERTEX_CNT = 1;
    terminate_v.push_back(0);
}

Automaton::Automaton(Edge _edge) {
    VERTEX_CNT = 2;
    edges.push_back(_edge);
    terminate_v.push_back(_edge.to);
}

Automaton::Automaton(const std::string regular_exp_polish_form) {
    std::stack<Automaton> st;  
    for (char symbol : regular_exp_polish_form) {
        if (((symbol == '+' || symbol == '.') && st.size() < 2) ||
                (symbol == '*' && st.size() < 1)) {
            throw std::invalid_argument("wrong input");
        }
        if (symbol == '+') {
            Automaton A = st.top();
            st.pop();
            Automaton B = st.top();
            st.pop();
            st.push((A + B).minimalize());
        } else if (symbol == '*') {
            Automaton A = st.top();
            st.pop();
            st.push(A.looped().minimalize());
        } else if (symbol == '.') {
            Automaton A = st.top();
            st.pop();
            Automaton B = st.top();
            st.pop();
            st.push((A ^ B).minimalize());            
        } else if (symbol == '1') {
            st.push(Automaton()); 
        } else if (symbol == 'a' || symbol == 'b' || symbol == 'c') {
            st.push(Automaton(Edge(0, 1, symbol)));
        } else {
            throw std::invalid_argument("wrong input");
        }
    }
    if (st.size() != 1) {
        throw std::invalid_argument("wrong input");
    } 
    (*this) = st.top().delete_dummy_vertexes(); 
}
    
std::vector<Edge> Automaton::find_incoming_edges(const int vertex_num) const {
    std::vector<Edge> answer;
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i].to == vertex_num) {
            answer.push_back(edges[i]);
        }
    }
    return answer;
}

Automaton Automaton::looped() const {
    Automaton ans = *this;
    for (int terminal : terminate_v) {
        std::vector<Edge> prev_vertexes = find_incoming_edges(terminal);
        for (Edge edge : prev_vertexes) {
            ans.edges.push_back(Edge(edge.from, 0, edge.letter));
        }   
    }
    return ans;
}

Automaton& Automaton::operator^=(const Automaton& other) {
    for (int terminal : terminate_v) {
        std::vector<Edge> prev_vertexes = find_incoming_edges(terminal);
        for (Edge edge : prev_vertexes) {
            edges.push_back(Edge(edge.from, VERTEX_CNT, edge.letter));
        }
    }
    for (Edge edge : other.edges) {
        edge.from += VERTEX_CNT;
        edge.to += VERTEX_CNT;
        edges.push_back(edge);
    }
    terminate_v = other.terminate_v;
    for (int i = 0; i < terminate_v.size(); ++i) {
        terminate_v[i] += VERTEX_CNT;
    }
    VERTEX_CNT += other.VERTEX_CNT;
    return *this;
}

Automaton Automaton::operator^(const Automaton& other) const {
    Automaton ans = *this;
    return (ans^=other);
}

Automaton& Automaton::operator+=(const Automaton& other) {
    for (Edge edge : other.edges) {
        if (edge.from == 0) {
            edges.push_back(Edge(0, edge.to + VERTEX_CNT, edge.letter));
        }
        edge.from += VERTEX_CNT;
        edge.to += VERTEX_CNT ;
        edges.push_back(edge);
    }
    bool v0_term = false;
    for (int i = 0; i < terminate_v.size(); ++i) {
        if (terminate_v[i] == 0) {
            v0_term = true;
            break;
        }
    }
    for (int i = 0; i < other.terminate_v.size(); ++i) {
        if (other.terminate_v[i] == 0) {
            if (!v0_term) {
                terminate_v.push_back(0);
            } 
        }
        terminate_v.push_back(other.terminate_v[i] + VERTEX_CNT);
    }
    VERTEX_CNT += other.VERTEX_CNT;
    return *this; 
}

Automaton Automaton::operator+(const Automaton& other) const {
    Automaton ans = (*this);
    return (ans += other);
}

Automaton& Automaton::operator=(const Automaton &other) {
    edges = other.edges;
    terminate_v = other.terminate_v;
    VERTEX_CNT = other.VERTEX_CNT;
    return *this;
}

Automaton::Automaton(const Automaton& other) {
    edges = other.edges; 
    terminate_v = other.terminate_v; 
    VERTEX_CNT = other.VERTEX_CNT;
}

std::ostream& operator<<(std::ostream &stream, const Automaton &A) {
    stream <<"edges: \n";
    for (int i = 0; i < A.edges.size(); ++i) {
        stream << A.edges[i].from << " " << A.edges[i].to << " " << A.edges[i].letter << '\n';
    }
    stream << "terminate vertexes:\n";
    for (int i = 0; i < A.terminate_v.size(); ++i) {
        stream << A.terminate_v[i] << " ";
    } 
    stream << '\n';
    return stream;
}

std::vector<int> Automaton::addition_terminates() const {
    std::vector<int> used(VERTEX_CNT, 0), answer;
    for (int i = 0; i < terminate_v.size(); ++i) {
        used[terminate_v[i]] = 1;
    }
    for (int i = 0; i < VERTEX_CNT; ++i) {
        if (!used[i]) answer.push_back(i);
    }
    return answer;
}

Automaton& Automaton::new_terminates(const std::vector<int>& _new) {
    terminate_v = _new;
    return *this;
}

Automaton Automaton::to_full(int letter_cnt) const {
    auto graph = this->form_letter_to(false);
    int flag = 0;
    auto new_edges = edges;

    for (int v = 0; v < graph.size(); ++v) {
        for (int letter_number = 0; letter_number < letter_cnt; ++letter_number) {
            if (graph[v][letter_number].size() == 0) {
                flag = 1;    
                new_edges.push_back(Edge(v, VERTEX_CNT, NumberToLetter(letter_number)));
            }
        }
    }
    if (flag) {
        for (int letter_number = 0; letter_number < letter_cnt; ++letter_number) {
            new_edges.push_back(Edge(VERTEX_CNT, VERTEX_CNT, NumberToLetter(letter_number)));
        }
    }
    return Automaton(new_edges, this->terminate_v);

}

std::vector<std::vector<int> > Automaton::build_table_not_equal_states() const {
    auto gr_r = this->form_letter_to(true);
    std::vector<int> terminal(VERTEX_CNT);
    for (int i = 0; i < terminate_v.size(); ++i) 
        terminal[terminate_v[i]] = 1;
    std::queue<Edge> next_edges;
    std::vector<std::vector<int> > not_equal_state(VERTEX_CNT, 
            std::vector<int> (VERTEX_CNT, 0)); 
    
    for (int i = 0; i < VERTEX_CNT; ++i) {
        for (int j = 0; j < VERTEX_CNT; ++j) {
            if (!not_equal_state[i][j] && terminal[i] != terminal[j]) {
                not_equal_state[i][j] = not_equal_state[j][i] = 1;
                next_edges.push(Edge(i,j));
            }
        }
    }
    
    while (!next_edges.empty()) {
        int u = next_edges.front().from;
        int v = next_edges.front().to;
        next_edges.pop();
        for (int letter_number = 0; letter_number < ALPHABET_SIZE; ++letter_number) {
            for (int i = 0; i < gr_r[u][letter_number].size(); ++i) {
                for (int j = 0; j < gr_r[v][letter_number].size(); ++j) {
                    int next_state_u = gr_r[u][letter_number][i];
                    int next_state_v = gr_r[v][letter_number][j];
                    if (!not_equal_state[next_state_u][next_state_v]) {
                        not_equal_state[next_state_u][next_state_v] = 
                            not_equal_state[next_state_v][next_state_u] = 1;
                        next_edges.push(Edge(next_state_u, next_state_v)); 
                    } 
                }
            }
        }
    }
    return not_equal_state;
} 

std::vector<int> Automaton::find_reachable() const {
    auto graph = this->form_letter_to(false);
    std::queue<int> next_vertex;
    std::vector<int> used(VERTEX_CNT, 0);
    next_vertex.push(0);
    used[0] = 1;
    while (!next_vertex.empty()) {
        int from = next_vertex.front();
        next_vertex.pop();
        for (int letter_number = 0; letter_number < ALPHABET_SIZE; ++letter_number) {
            for (int i = 0; i < graph[from][letter_number].size(); ++i) {
                int to = graph[from][letter_number][i];
                if (used[to]) continue;
                used[to] = 1;
                next_vertex.push(to);
            }
        }
    }
    return used;
}

int Automaton::max_letter_num() const {
    int ans = 0;
    for (Edge edge : edges) {
        int cur = LetterToNumber(edge.letter);
        ans = (ans < cur) ? cur : ans;
    }
    return ans; 
}

Automaton Automaton::minimalize() const {
    Automaton DFA = this->determinizate();
    DFA = DFA.to_full(DFA.max_letter_num() + 1);
    auto not_equal_state = DFA.build_table_not_equal_states();
    auto reachable = DFA.find_reachable();
    int n = DFA.get_size();
    std::vector<int> component(n, -1);
    
    for (int i = 0; i < n; ++i) {
        if (!not_equal_state[0][i])
            component[i] = 0;
    }
    int components_cnt = 0;
    for (int i = 0; i < n; ++i) {
        if (!reachable[i]) continue;
        if (component[i] != -1) continue;
        
        components_cnt++;
        component[i] = components_cnt;
        for (int j = i + 1; j < n; ++j) {
            if (!not_equal_state[i][j]) {
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

VVVI Automaton::form_letter_to(bool reversed = false) const {
    VVVI graph(VERTEX_CNT, 
        std::vector<std::vector<int> > (ALPHABET_SIZE, std::vector<int>(0)));
    for (int i = 0; i < edges.size(); ++i) {
        if (!reversed) { 
            graph[edges[i].from][LetterToNumber(edges[i].letter)].
                push_back(edges[i].to);
        } else {
            graph[edges[i].to][LetterToNumber(edges[i].letter)].
                push_back(edges[i].from);
        }
    }
    return graph;
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

//using bitset mask to calculate next vetrex
Automaton Automaton::determinizate() const {
    const int MAX_BIT_IN_INT = 32;
    auto graph = this->form_letter_to();
    std::set<int> used;
    std::queue<int> process_next;
    std::vector<Edge> answer; 
    std::vector<int> new_vertexes;
    process_next.push(1);
    used.insert(1);
    new_vertexes.push_back(1);
    while (!process_next.empty()) {
        int current_vertex = process_next.front();
        process_next.pop();
        
        for (int letter_number = 0; letter_number < ALPHABET_SIZE; ++letter_number) {
            int next_vertex = 0;
            for (int vertex = 0; vertex < MAX_BIT_IN_INT; vertex++) {
                int bit_responds = (1<<vertex);
                if ((bit_responds & current_vertex) == 0) {
                    continue;
                }
                for (int j = 0; j < graph[vertex][letter_number].size(); ++j) {
                    int bit_responds_j = (1<<graph[vertex][letter_number][j]);
                    next_vertex |= bit_responds_j;
                }
                    
            }
            if (next_vertex > 0) {
                answer.push_back(Edge(current_vertex, next_vertex, NumberToLetter(letter_number)));
                if (used.find(next_vertex) == used.end()) {
                    used.insert(next_vertex);
                    process_next.push(next_vertex);
                    new_vertexes.push_back(next_vertex);
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

