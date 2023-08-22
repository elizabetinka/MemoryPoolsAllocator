//
// Created by Елизавета Кравченкова on 23.03.2023.
//
#include <functional>
#include <lib/MemoryPoolsAllocator.h>

int main(int argc, char** argv) {
    const int size_1=1024;
    const int size_2=2;
    const int capacity=1000;
    MemoryPoolsAllocator<int,size_1,size_2> q;


    unsigned int start_time1 =  clock();
    std::vector<int , MemoryPoolsAllocator<int,size_1,size_2>> v1;
    //v1.reserve(capacity);
    for (uint64_t i=0;i<capacity;i++){
        v1.push_back(i); //1 2 4 1000 2048
    }
    unsigned int end_time1 = clock(); // конечное время


    unsigned int search_time1 = end_time1 - start_time1; // искомое время

    unsigned int start_time2 =  clock();
    std::vector<int> v2(capacity);
    for (uint64_t i=0;i<capacity;i++){
        v2.push_back(i);
    }
    unsigned int end_time2 = clock(); // конечное время
    unsigned int search_time2 = end_time2 - start_time2; // искомое время

    std::cout<<"Время работы: "<<search_time1<<" "<<search_time2<<std::endl;

    //std::allocator<int> w;

    return 0;

}