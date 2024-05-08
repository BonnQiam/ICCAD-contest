#include <iostream>

#include "Fast_Median.hpp"

int main(){
    std::vector<int> vec = {1, 2, 3, 4, 5, 6};
    std::cout << find_Median(vec) << '\n';
    return 0;
}