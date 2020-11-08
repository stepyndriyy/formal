#pragma once
#include<gtest/gtest.h>
#include "utils.h"
#include "practice1_variant5.h"

class PracticeTest : public ::testing::Test {
public:
    PracticeTest() {}
    ~PracticeTest() {}
    void SetUp() {}
    void TearDown() {}
};

TEST_F(PracticeTest, check_prefix_tests) {
    Automaton automaton = read_automaton("src/automaton2.txt");  
    VVVI gr = automaton.form_letter_to(false);
    ASSERT_EQ(1,
            check_prefix(gr, 0, 'a', 100));
    ASSERT_EQ(1,
            check_prefix(gr, 2, 'c', 1));
    ASSERT_EQ(0,
            check_prefix(gr, 0, 'c', 2));
}

TEST_F(PracticeTest, is_suffix_belong_regular_tests) {
    std::string s = "aab.*.1a+."; // (1+a)(ab)*a
    ASSERT_EQ(true,
            is_suffix_belong_regular(s, 'a', 2));
    ASSERT_EQ(false,
            is_suffix_belong_regular(s, 'a', 3));

    ASSERT_ANY_THROW(
            is_suffix_belong_regular("a+", 'a', 0));
    ASSERT_ANY_THROW(
            is_suffix_belong_regular("ab%", 'a', 0));
    ASSERT_ANY_THROW(
            is_suffix_belong_regular("aab.", 'a', 0));
}


