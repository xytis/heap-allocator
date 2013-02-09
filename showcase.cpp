#include <iostream>
#include <vector>

#include "Allocator.hpp"

int main()
{
    std::vector< int, Allocator<int, TrackingAllocPolicy<int> > > array;
    
    array.resize(50);
    
    for (int i = 0; i < array.size(); ++i) {
      array[i] = -1;
    }
    
    std::cout
    << "Total allocations: " << array.get_allocator->TotalAllocations() << std::endl
    << "Peak allocations: " << array.get_allocator->PeakAllocations() << std::endl
    << "Current allocations: " << array.get_allocator->CurrentAllocations() << std::endl;

    return 0;
}