#include <iostream>
#include <vector>
#include "Frame.hpp"

int main() {
    
    Frame f(1920, 1080, RGB);
    
    std::cout << "Frame created with width: " << f.width() 
              << ", height: " << f.height() 
              << ", format: " << (f.format() == RGB ? "RGB" : "YUV420P") 
              << ", size: " << f.size() << " bytes" << std::endl
              << "First byte address: " << (void*)f.data() << "\n";

    return 0;
}