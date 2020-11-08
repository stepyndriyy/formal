#pragma once
#include<gtest/gtest.h>
#include "utils.h"

class OperationTest : public ::testing::Test {
public:
    OperationTest() {}
    ~OperationTest() {}
    void SetUp() {}
    void TearDown() {}
};

TEST_F(OperationTest, concatinate_test) {
    Automaton a1 = read_automaton("src/automaton1.txt");
    Automaton a2 = read_automaton("src/automaton2.txt");  
    AutomatonStates expected;
    read_answer("src/concatinate_test_ans.txt", expected);
    AutomatonStates actual(a1^a2);

    ASSERT_EQ(actual.vertex_cnt, expected.vertex_cnt);
    ASSERT_EQ(actual.term_cnt, expected.term_cnt);
    ASSERT_EQ(actual.a_cnt, expected.a_cnt);
    ASSERT_EQ(actual.b_cnt, expected.b_cnt);
    ASSERT_EQ(actual.c_cnt, expected.c_cnt);
}

TEST_F(OperationTest, plus_test) {
    Automaton a1 = read_automaton("src/automaton1.txt");
    Automaton a2 = read_automaton("src/automaton2.txt");  
    AutomatonStates expected;
    read_answer("src/plus_test_ans.txt", expected);
    AutomatonStates actual(a1+a2);

    ASSERT_EQ(actual.vertex_cnt, expected.vertex_cnt);
    ASSERT_EQ(actual.term_cnt, expected.term_cnt);
    ASSERT_EQ(actual.a_cnt, expected.a_cnt);
    ASSERT_EQ(actual.b_cnt, expected.b_cnt);
    ASSERT_EQ(actual.c_cnt, expected.c_cnt);
}

TEST_F(OperationTest, loop_test) {
    Automaton a1 = read_automaton("src/automaton1.txt");
    AutomatonStates expected;
    read_answer("src/loop_test_ans.txt", expected);
    AutomatonStates actual(a1.looped());

    ASSERT_EQ(actual.vertex_cnt, expected.vertex_cnt);
    ASSERT_EQ(actual.term_cnt, expected.term_cnt);
    ASSERT_EQ(actual.a_cnt, expected.a_cnt);
    ASSERT_EQ(actual.b_cnt, expected.b_cnt);
    ASSERT_EQ(actual.c_cnt, expected.c_cnt);
}


