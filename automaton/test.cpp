#include "test_utils.h"
#include<gtest/gtest.h>

class Tests : public ::testing::Test {
public:
    Tests() { }
    ~Tests() {  }
    void SetUp() {  }
    void TearDown() {  }

protected:
    
};



TEST_F(Tests, TEST_DETERMIN) {
    Automaton x = read_automaton("tests/test_determ1.txt");
    AutomatonStates ans, inp;
    read_answer("tests/test_determ1_ans.txt", ans);
    x = x.determinizate(); 
    get_real_values(x, inp);

    ASSERT_EQ(inp.vertex_cnt, ans.vertex_cnt);
    ASSERT_EQ(inp.term_cnt, ans.term_cnt);
    ASSERT_EQ(inp.a_cnt, ans.a_cnt);
    ASSERT_EQ(inp.b_cnt, ans.b_cnt);
}

TEST_F(Tests, TEST_MINIMALIZE) {
    Automaton x = read_automaton("tests/test_min1.txt");
    AutomatonStates ans, inp;

    read_answer("tests/test_min1_ans.txt", ans);
    x = x.minimalize();
    
    get_real_values(x, inp);
    
    ASSERT_EQ(inp.vertex_cnt, ans.vertex_cnt);
    ASSERT_EQ(inp.term_cnt, ans.term_cnt);
    ASSERT_EQ(inp.a_cnt, ans.a_cnt);
    ASSERT_EQ(inp.b_cnt, ans.b_cnt);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
