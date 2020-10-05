#include "automaton.h"
#include<cstdio>
#include<iostream>
#include<vector>
#include <gtest/gtest.h>
 
Automaton read_automaton(const char* file) {
    using namespace std;
    freopen(file, "r", stdin);
    
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
    
    return Automaton(edges, terminates);
}

void get_real_values(const Automaton& x, int& v, int& term, int& a_cnt, int& b_cnt) {
    v = x.get_size();
    term = x.get_terminates().size();
    a_cnt = 0;
    b_cnt = 0;
    auto edges = x.get_edges();
    for (int i = 0; i < edges.size(); ++i) {
        if (edges[i].letter == 'a') a_cnt++;
        if (edges[i].letter == 'b') b_cnt++;
    }
}

void read_answer(const char* file, int& v, int& term, int& a, int &b) {
    freopen(file, "r", stdin);
    std::cin >> v >> term >> a >> b;
}

class Tests : public ::testing::Test {
public:
    Tests() { /* init protected members here */ }
    ~Tests() { /* free protected members here */ }
    void SetUp() { /* called before every test */ }
    void TearDown() { /* called after every test */ }

protected:
    /* none yet */
};



TEST_F(Tests, TEST_DETERMIN) {
    Automaton x = read_automaton("tests/test_determ1.txt");
    int v_ans, term_ans, a_ans, b_ans;
    read_answer("tests/test_determ1_ans.txt", v_ans, term_ans, a_ans, b_ans);
    x = x.determinizate();
    int v, a_cnt, b_cnt, term_cnt;
    get_real_values(x, v, term_cnt, a_cnt, b_cnt);
    ASSERT_EQ(v, v_ans);
    ASSERT_EQ(term_cnt, term_ans);
    ASSERT_EQ(a_cnt, a_ans);
    ASSERT_EQ(b_cnt, b_ans);
}

TEST_F(Tests, TEST_MINIMALIZE) {
    Automaton x = read_automaton("tests/test_min1.txt");
    int v_ans, term_ans, a_ans, b_ans;
    read_answer("tests/test_min1_ans.txt", v_ans, term_ans, a_ans, b_ans);
    x = x.minimalize();
    int v, a_cnt, b_cnt, term_cnt;
    get_real_values(x, v, term_cnt, a_cnt, b_cnt);
    ASSERT_EQ(v, v_ans);
    ASSERT_EQ(term_cnt, term_ans);
    ASSERT_EQ(a_cnt, a_ans);
    ASSERT_EQ(b_cnt, b_ans);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
