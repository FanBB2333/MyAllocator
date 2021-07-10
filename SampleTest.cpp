#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include "./src/AlloMemPool.h"

//#define DEBUG
// include header of your allocator here
template<class T>
//using MyAllocator = std::allocator<T>; // replace the std::allocator with your allocator
using MyAllocator = MemMgr::allocator<T>;
using Point2D = std::pair<int, int>;

const int TestSize = 10;
const int PickSize = 1000;

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, TestSize);
    // vector creation
    using IntVec = std::vector<int, MyAllocator<int>>;
#ifdef DEBUG
    clock_t start;
//    start = clock();
//    std::vector<int, std::allocator<int> > ori;
//    ori.push_back(1);
//    for(int i = 0; i < 100; i++){
//        ori.push_back(i);
//        std::cout << "Push : " << i << std::endl;
//    }
//    std::cout << ori.at(0) << std::endl;
//    ori.pop_back();
//    std::cout << "Default Allocator Time: ";
//    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";

    start = clock();
    std::vector<int, MyAllocator<int>> abc;


    for(int i = 0; i < 65; i++){
        abc.push_back(i);
//        std::cout << "Push : " << i << std::endl;
    }
    for(int i = 0; i < abc.size(); i++){
        printf("%d : %d\n", i, abc.at(i));
    }
    return 0;

//    std::cout << abc.at(0) << std::endl;


//    abc.push_back(6);
//    abc.push_back(7);
//    abc.push_back(8);
//    abc.push_back(9);

//    for(int i = 0; i < 5; i++){
//        abc.push_back(i);
//        std::cout << "Push : " << i << std::endl;
//    }

    return 0;
    for(int i = 0; i < 5; i++){
        abc.push_back(i);
        std::cout << "Push : " << i << std::endl;
    }

    abc.pop_back();
    std::cout << "My Allocator Time: ";
    std::cout << (((double)clock() - start) / CLOCKS_PER_SEC) << "\n\n";
    return 0;
#endif
    std::vector<IntVec, MyAllocator<IntVec>> vecints(TestSize);

    for (int i = 0; i < TestSize; i++){
        vecints[i].resize(dis(gen));
    }

    using PointVec = std::vector<Point2D, MyAllocator<Point2D>>;
    std::vector<PointVec, MyAllocator<PointVec>> vecpts(TestSize);
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

    return 0;
}

