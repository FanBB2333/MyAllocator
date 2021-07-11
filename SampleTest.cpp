#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <cassert>
#include "./src/AlloMemPool.h"
#define timecost_test
//#define original_test
// include header of your allocator_list here
//using MyAllocator = std::allocator_list<T>; // replace the std::allocator_list with your allocator_list
template<class T>
using MyAllocator = MemMgr::allocator_list<T>;
template<class T>
using ListAllocator = MemMgr::allocator_list<T>;
template<class T>
using HeapAllocator = MemMgr::allocator_heap<T>;

using Point2D = std::pair<int, int>;


const int TestSize = 10000;
const int PickSize = 1000;
const int cycle = 100000;
int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, TestSize);
    // vector creation
    clock_t start;


#ifdef original_test
    std::cout << "------------Start of original Testcase!------------" << std::endl;
    std::cout << "Linked list allocator" << std::endl;
    using IntVec = std::vector<int, ListAllocator<int>>;
    std::vector<IntVec, ListAllocator<IntVec>> vecints(TestSize);

    for (int i = 0; i < TestSize; i++){
        vecints[i].resize(dis(gen));
    }

    using PointVec = std::vector<Point2D, ListAllocator<Point2D>>;
    std::vector<PointVec, ListAllocator<PointVec>> vecpts(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts[i].resize(dis(gen));

    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints[idx].resize(size);
        vecpts[idx].resize(size);
    }

    // vector element assignment
    {
        int val = 10;
        int idx1 = dis(gen) - 1;
        int idx2 = vecints[idx1].size() / 2;
        vecints[idx1][idx2] = val;
        if (vecints[idx1][idx2] == val)
            std::cout << "correct assignment in vecints: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
    }
    {
        Point2D val(11, 15);
        int idx1 = dis(gen) - 1;
        int idx2 = vecpts[idx1].size() / 2;
        vecpts[idx1][idx2] = val;
        if (vecpts[idx1][idx2] == val)
            std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    }


    std::cout << "Heap allocator" << std::endl;
    using IntVec2 = std::vector<int, HeapAllocator<int>>;
    std::vector<IntVec2, HeapAllocator<IntVec2>> vecints2(TestSize);

    for (int i = 0; i < TestSize; i++){
        vecints2[i].resize(dis(gen));
    }

    using PointVec2 = std::vector<Point2D, HeapAllocator<Point2D>>;
    std::vector<PointVec2, HeapAllocator<PointVec2>> vecpts2(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts2[i].resize(dis(gen));

    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints2[idx].resize(size);
        vecpts2[idx].resize(size);
    }

    // vector element assignment
    {
        int val = 10;
        int idx1 = dis(gen) - 1;
        int idx2 = vecints2[idx1].size() / 2;
        vecints2[idx1][idx2] = val;
        if (vecints2[idx1][idx2] == val)
            std::cout << "correct assignment in vecints: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
    }
    {
        Point2D val(11, 15);
        int idx1 = dis(gen) - 1;
        int idx2 = vecpts2[idx1].size() / 2;
        vecpts2[idx1][idx2] = val;
        if (vecpts2[idx1][idx2] == val)
            std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    }
    std::cout << "------------End of original Testcase!------------\n\n" << std::endl;
    return 0;
#endif
#ifdef timecost_test
    std::cout << "------------Start of time Test!------------" << std::endl;
    std::cout << "Because that a vector is expanded twice large when it is almost full, then we test with different size of push elements" << std::endl;
    std::cout << "------------Using Default Allocator!------------" << std::endl;
    int push_size = 10;
    start = clock();
    for(auto i = 0; i < cycle; i++){
        std::vector<int, std::allocator<int> > tmp;
        // Push 4 elements to test twice memory expand and copy of vector
        for(auto j = 0; j < push_size; j++){
            tmp.push_back(j);
        }
    }
    std::cout << "Default Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n";

    std::cout << "------------Using Linked List Allocator!------------" << std::endl;
    for(auto i = 0; i < cycle; i++){
        std::vector<int, ListAllocator<int> > tmp;
        // Push 4 elements to test twice memory expand and copy of vector
        for(auto j = 0; j < push_size; j++){
            tmp.push_back(j);
        }
    }
    std::cout << "Linked List Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n";
//    return 0;
    std::cout << "------------Using Heap Allocator!------------" << std::endl;

    for(auto i = 0; i < cycle; i++){
        std::vector<int, HeapAllocator<int> > tmp;
        // Push 4 elements to test twice memory expand and copy of vector
        for(auto j = 0; j < push_size; j++){
            tmp.push_back(j);
        }
    }
    std::cout << "Heap Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
#endif

#ifdef frequent_allo_test
    std::cout << "------------Frequently allocate memories test!------------\n" << std::endl;
    std::cout << "Because that each vector applies for a piece of memory when push an element," << std::endl;
    std::cout << "then we test the frequent allocate and release ability\n" << std::endl;

    std::vector<std::vector<int, ListAllocator<int> > > vecs;
    int max_vectors = 127;
    for(auto i = 0; i < max_vectors; i++){
        std::vector<int, ListAllocator<int> > t ;
        for(auto j = 0; j < 100; j++){
            t.push_back(j);
        }
        vecs.push_back(t);
    }
    int test_vec = gen() % max_vectors;
    bool correct = true;
    for(auto i = 0; i < vecs[test_vec].size(); i++){
        int element =vecs[test_vec].at(i);
        assert(element == i); // If the memory allocation has error, then it will assert.
        if( element != i){
            std::cout << "An Error occurs when frequently apply and free." <<std::endl;
            correct = false;
            break;
        }
    }
    if(correct){
        std::cout << "Test pass when frequently apply and free blocks!" <<std::endl;
    }
    std::cout << "------------End of Frequently allocate memories test!------------" << std::endl;
#endif

#ifdef largemem_test
    std::cout << "------------Allocate Extreme Large Memories Test!------------" << std::endl;
    std::cout << "uint64_t type costs 8 byte, our block has 4k byte, then test with at least 512 elements.\n" << std::endl;
    std::cout << "To construct an element larger than a block : 513 uint64_t elements" << std::endl;
    std::vector<uint64_t, ListAllocator<uint64_t> > large_test1(513);
    std::cout << std::endl;
    std::cout << "To construct an element smaller than a block : 511 uint64_t elements" << std::endl;
    std::vector<uint64_t, ListAllocator<uint64_t> > large_test2(511);
    std::cout << "------------End of Allocate Extreme Large Memories Test!------------" << std::endl;
#endif
    return 0;
}

