#include "../avl.hpp"
#include <iostream>

int main(void){
    avl<std::string, int> tree;

    tree["one"] = 1;
    tree["two"] = 2;
    tree["three"] = 3;

    std::cout << tree["one"] << ' ' << tree.exists("two") << std::endl;

    return 0;
}
