#include <iostream>
#include <iomanip>
#include <vector>
#include <list>

#include "Allocator.hpp"

int main()
{  
    //Global allocator, which is used for few allocations.
    Allocator<int, TrackingAllocPolicy<int> > global_alloc;

    
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
    
    std::cout
    << "Global allocator: " << std::setbase(16) << global_alloc.Address() << std::endl
    << "Total allocations: " << global_alloc.TotalAllocations() << std::endl
    << "Peak allocations: " << global_alloc.PeakAllocations() << std::endl
    << "Current allocations: " << global_alloc.CurrentAllocations() << std::endl
    << "Local allocator (array): " << std::setbase(16) << array.get_allocator().Address() << std::endl
    << "Total allocations: " << array.get_allocator().TotalAllocations() << std::endl
    << "Peak allocations: " << array.get_allocator().PeakAllocations() << std::endl
    << "Current allocations: " << array.get_allocator().CurrentAllocations() << std::endl
    << "Local allocator (list): " << std::setbase(16) << list.get_allocator().Address() << std::endl
    << "Total allocations: " << list.get_allocator().TotalAllocations() << std::endl
    << "Peak allocations: " << list.get_allocator().PeakAllocations() << std::endl
    << "Current allocations: " << list.get_allocator().CurrentAllocations() << std::endl;

    return 0;
}