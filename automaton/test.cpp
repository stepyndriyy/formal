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
    AutomatonStates expected, actual;
    read_answer("tests/test_determ1_ans.txt", expected);
    x = x.determinizate(); 
    get_real_values(x, actual);

    ASSERT_EQ(actual.vertex_cnt, expected.vertex_cnt);
    ASSERT_EQ(actual.term_cnt, expected.term_cnt);
    ASSERT_EQ(actual.a_cnt, expected.a_cnt);
    ASSERT_EQ(actual.b_cnt, expected.b_cnt);
}

TEST_F(Tests, TEST_MINIMALIZE) {
    Automaton x = read_automaton("tests/test_min1.txt");
    AutomatonStates expected, actual;

    read_answer("tests/test_min1_ans.txt", expected);
    x = x.minimalize();
    
    get_real_values(x, actual);
    
    ASSERT_EQ(actual.vertex_cnt, expected.vertex_cnt);
    ASSERT_EQ(actual.term_cnt, expected.term_cnt);
    ASSERT_EQ(actual.a_cnt, expected.a_cnt);
    ASSERT_EQ(actual.b_cnt, expected.b_cnt);
}

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
