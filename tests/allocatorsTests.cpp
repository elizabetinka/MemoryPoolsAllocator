#include <lib/MemoryPoolsAllocator.h>
#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <ctime>
#include <list>

TEST(MemoryPoolsAllocatorTestSuite, EmptyTest) {
    std::vector<int , MemoryPoolsAllocator<int,32,2>> v1;
    unsigned int start_time1 =  clock();
    for (uint64_t i=0;i<32;i++){
        v1.push_back(i);
    }
    unsigned int end_time1 = clock(); // конечное время
    unsigned int search_time1 = end_time1 - start_time1; // искомое время


    std::vector<int> v2;
    unsigned int start_time2 =  clock();
    for (uint64_t i=0;i<32;i++){
        v2.push_back(i);
    }
    unsigned int end_time2 = clock(); // конечное время
    unsigned int search_time2 = end_time2 - start_time2; // искомое время

    std::cout<<"Время работы: "<<search_time1<<" "<<search_time2<<std::endl;
    ASSERT_TRUE(true);
}

TEST(MemoryPoolsAllocatorTestSuite, ListTest) {
    std::list<int, MemoryPoolsAllocator<int ,1,10000>> list1;
    unsigned int start_time1 =  clock();
    for (uint64_t i=0;i<10000;i++){
        list1.push_back(i);
        if (i%10==0){
            list1.pop_front();
        }
    }
    unsigned int end_time1 = clock(); // конечное время
    unsigned int search_time1 = end_time1 - start_time1; // искомое время

    std::list<std::string,std::allocator<std::string>> list2;
    unsigned int start_time2 =  clock();
    for (uint64_t i=0;i<10000;i++){
        list2.push_back("Liza");
        if (i%10==0){
            list2.pop_front();
        }
    }
    unsigned int end_time2 = clock(); // конечное время
    unsigned int search_time2 = end_time2 - start_time2; // искомое время

    std::cout<<"Время работы: "<<search_time1<<" "<<search_time2<<std::endl;

    ASSERT_TRUE(true);
}
