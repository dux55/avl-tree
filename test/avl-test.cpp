#include "../avl.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>

int main(void){
    int iterations = 100000; 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> values(1, iterations);
    
    avl<int, int> test;
    std::vector<int> traversal;
    for(int i = 1; i <= iterations; i++){
	test.insert(values(gen), 1);

	avl_iterator<int, int> iter(test);
	int last = 0;
	while(last < (*iter).first){
	   traversal.push_back((*iter).first);
	   last = (*iter).first;
	   iter++;
	}

        if(!std::is_sorted(traversal.begin(), traversal.end())){
	    std::cout << "TEST FAILED\n";
            return 1;
        }
	traversal.clear();

        if(i%(iterations/100) == 0) std::cout << "iteration " << i << '/' << iterations << '\n';
    }

    return 0;
}
