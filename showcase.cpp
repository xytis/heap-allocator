#include <iostream>
#include <iomanip>
#include <vector>
#include <list>

#include "Allocator.hpp"

template<typename T>
struct DebugAlloc {
  typedef Allocator<T, TrackingAllocPolicy<T> > type;
};

int main()
{  
    //Global allocator, which is used for few allocations.
    DebugAlloc<int>::type global_alloc;

    {
    //Vector which uses global allocator.
    std::vector< int, decltype(global_alloc) > array;
    //List, which uses personal allocator.
    std::list< int, decltype(global_alloc) > list(global_alloc);

    for (size_t i = 0; i < 15; ++i) {
        array.push_back(i);
        for (size_t j = 0; j < i; ++j) {
            list.push_back(i);
        }
        for (size_t j = 0; j < i/2; ++j) {
            list.pop_back();
        }
    }
}
    
    std::cout
    << "Type Tracking:"
    << std::endl;    
    GlobalTracker::report(std::cout);
    GlobalTracker::clean();
    return 0;
}