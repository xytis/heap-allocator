/*
 * =====================================================================================
 *
 *       Filename:  TrackingPolicy.hpp
 *
 *    Description:  Special debug tracking policy
 *
 *        Version:  1.0
 *        Created:  02/09/2013 11:25:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Vytis Valentinavičius (), vytis.valentinavicius@gmail.com
 *        Company:  VU FF
 *
 * =====================================================================================
 */


#ifndef __tracking_policy_hpp__
#define __tracking_policy_hpp__

#include <iostream>
#include "Policy.hpp"
#include "TypeTracking.hpp"

template<typename T, typename Policy = StandardAllocPolicy<T> >
class TrackingAllocPolicy : public Policy {
private : 
    typedef Policy AllocationPolicy;

public : 
    //    convert an TrackingAllocPolicy<T> to TrackingAllocPolicy<U>
    template<typename U>
    struct rebind {
        typedef TrackingAllocPolicy<U, typename AllocationPolicy::template rebind<U>::other > other;
    };

public : 
    inline explicit TrackingAllocPolicy() {}
    inline ~TrackingAllocPolicy() {}
    
    inline explicit 
        TrackingAllocPolicy(TrackingAllocPolicy const& rhs) : Policy(rhs) {}
        
    template <typename U>
    inline explicit 
       TrackingAllocPolicy(TrackingAllocPolicy<U> const& rhs):Policy(rhs) {}

    //    memory allocation
    typename AllocationPolicy::pointer 
      allocate(typename AllocationPolicy::size_type cnt, 
      typename std::allocator<void>::const_pointer hint = 0) { 
        typename AllocationPolicy::pointer p = 
               AllocationPolicy::allocate(cnt, hint);
        TypedTrackingInfo<T>::allocated(cnt);
        return p;
    }
    
    inline void deallocate(typename AllocationPolicy::pointer p, 
        typename AllocationPolicy::size_type cnt) { 
        AllocationPolicy::deallocate(p, cnt);
        TypedTrackingInfo<T>::deallocated(cnt);
    }
};    //    end of class TrackingAllocPolicy

// determines if memory from another
// allocator can be deallocated from this one
template<typename T, typename Policy, typename T2, typename Policy2>
inline bool operator==(TrackingAllocPolicy<T, Policy> const& lhs, 
        TrackingAllocPolicy<T2, Policy2> const& rhs) { 
  return operator==(static_cast<Policy&>(lhs), 
                   static_cast<Policy&>(rhs)); 
}
template<typename T, typename Policy, typename OtherAllocator>
inline bool operator==(TrackingAllocPolicy<T, 
     Policy> const& lhs, OtherAllocator const& rhs) { 
  return operator==(static_cast<Policy&>(lhs), rhs); 
}

#endif //__tracking_policy_hpp__