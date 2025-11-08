#include <iostream>
#include <map>
#include <utility> 

#include "MyContainer.hpp" 
#include "MyAllocator.hpp"

long long factorial(int n) {
    long long res = 1;
    for (int i = 2; i <= n; ++i) {
        res *= i;
    }
    return res;
}

int main() {

    std::cout << "Creating std::map (default allocator)" << std::endl;
    std::map<int, int> mapDefault;
    for (int i = 0; i < 10; ++i) {
        mapDefault[i] = factorial(i);
    }
    std::cout << "Filled 10 elements (factorials)" << std::endl;


    using MyMapAllocator = MyAllocator<std::pair<const int, int>, 10>;
    using MyMap = std::map<int, int, std::less<int>, MyMapAllocator>;

    std::cout << "Creating std::map (MyAllocator<..., 10>)" << std::endl;
    MyMap mapCustom;
    
    std::cout << "Filling with 10 elements (factorials)" << std::endl;
    for (int i = 0; i < 10; ++i) {
        mapCustom[i] = factorial(i);
    }

    std::cout << "Outputting mapCustom contents:" << std::endl;
    for (const auto& pair : mapCustom) {
        std::cout << " " << pair.first << " " << pair.second << std::endl;
    }


    std::cout << "Creating MyContainer (default allocator)" << std::endl;
    MyContainer<int> containerDefault;
    
    std::cout << "Filling with 10 elements (0-9)" << std::endl;
    for (int i = 0; i < 10; ++i) {
        containerDefault.push_back(i);
    }



    std::cout << "Creating MyContainer (MyAllocator<int, 10>)" << std::endl;
    MyContainer<int, MyAllocator<int, 10>> containerCustom;
    
    std::cout << "Filling with 10 elements (0-9)" << std::endl;
    for (int i = 0; i < 10; ++i) {
        containerCustom.push_back(i);
    }

    std::cout << "Outputting containerCustom contents:" << std::endl;
    std::cout << " ";
    for (int value : containerCustom) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    std::cout << "(Container and allocator operations complete, memory will be released in destructors)" << std::endl;
    
    return 0;
}