#include "practice1_variant5.h" 

int main() {
    std::string regular_exp;
    char letter;
    int suff_len;
    std::cin >> regular_exp >> letter >> suff_len;
    if (is_suffix_belong_regular(regular_exp, letter, suff_len)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
    return 0;
}
