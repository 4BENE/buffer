#include <iostream>
#include "../lib/buffer.h"
int main(){
    //std::cout<<"HELLO WORLD";
    CCircularBuffer<int> buffer(10);
    buffer.resize(10);
    CCircularBuffer<int> buffer2(10, 5);
    //std::cout<<buffer2[0]<<buffer2[1]<<buffer2[2];
    return 0;
}