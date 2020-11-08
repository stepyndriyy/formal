#pragma once
#include "automaton.h"

static int max(int x, int y) {
    return (x > y) ? x : y; 
}

int check_prefix(VVVI& gr, int vertex, char letter, int len) {
    int answer = 0;
    if (len <= 0) {
        return 1;
    }
    for (int to : gr[vertex][Automaton::LetterToNumber(letter)]) { 
        answer = max(answer, check_prefix(gr, to, letter, len - 1));
    }
    return answer;
}

bool is_suffix_belong_regular(const std::string& regular_exp, char letter, int len) {
    Automaton automaton(regular_exp);
    VVVI gr_r = automaton.form_letter_to(true);
    for (int terminal : automaton.get_terminates()) {
        if (check_prefix(gr_r, terminal, letter, len)) {
            return true;
        }
    }
    return false;
}
