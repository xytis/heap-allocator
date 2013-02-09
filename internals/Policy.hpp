/*
 * =====================================================================================
 *
 *       Filename:  Policy.hpp
 *
 *    Description:  Various alocation policies
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

#include <limits>

template<typename T>
class StandardAllocPolicy {
public :
    //    typedefs
    typedef T                           value_type;
    typedef value_type*                 pointer;
    typedef const value_type*           const_pointer;
    typedef value_type&                 reference;
    typedef const value_type&           const_reference;
    typedef std::size_t                 size_type;
    typedef std::ptrdiff_t              difference_type;

public :
    //    convert an StandardAllocPolicy<T> to StandardAllocPolicy<U>
    template<typename U>
    struct rebind {
        typedef StandardAllocPolicy<U> other;
    };

public :
    inline explicit StandardAllocPolicy() {}
    inline ~StandardAllocPolicy() {}
    inline explicit StandardAllocPolicy(StandardAllocPolicy const&) {}

    template <typename U>
    inline explicit StandardAllocPolicy(StandardAllocPolicy<U> const&) {}

    //    memory allocation
    inline pointer allocate(size_type cnt,
                            typename std::allocator<void>::const_pointer = 0) {
        return reinterpret_cast<pointer>(::operator
                                         new(cnt * sizeof (T)));
    }
    inline void deallocate(pointer p, size_type)
    {
        ::operator delete(p);
    }

    //    size
    inline size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }
};    //    end of class StandardAllocPolicy

// determines if memory from another
// allocator can be deallocated from this one
template<typename TA, typename TB>
inline bool operator==(StandardAllocPolicy<TA> const&,
                       StandardAllocPolicy<TB> const&) {
    return true;
}

template<typename TA, typename OtherAllocator>
inline bool operator==(StandardAllocPolicy<TA> const&,
                       OtherAllocator const&) {
    return false;
}

template<typename T, typename Policy = StandardAllocPolicy<T> >
class TrackingAllocPolicy : public Policy {
private : 
    typedef Policy AllocationPolicy;

public : 
    //    convert an TrackingAllocPolicy<T> to TrackingAllocPolicy<U>
    template<typename U>
    struct rebind {
        typedef TrackingAllocPolicy<U, 
           typename AllocationPolicy::rebind<U>::other> other;
    };

public : 
    inline explicit TrackingAllocPolicy():total_(0), current_(0), peak_(0) {}
    inline ~TrackingAllocPolicy() {}
    inline explicit 
        TrackingAllocPolicy(TrackingAllocPolicy const& rhs):Policy(rhs), 
        total_(rhs.total_), current_(rhs.current_), peak_(rhs.peak_) {}
    template <typename U>
    inline explicit 
       TrackingAllocPolicy(TrackingAllocPolicy<U> const& rhs):Policy(rhs), 
       total_(0), current_(0), peak_(0) {}

    //    memory allocation
    typename AllocationPolicy::pointer 
      allocate(typename AllocationPolicy::size_type cnt, 
      typename std::allocator<void>::const_pointer hint = 0) { 
        typename AllocationPolicy::pointer p = 
               AllocationPolicy::allocate(cnt, hint);
        this->total_ += cnt;
        this->current_ += cnt;
        if ( this->current_ > this->peak_ ) {
            this->peak_ = this->current_;
        }
        return p;
    }
    
    inline void deallocate(typename AllocationPolicy::pointer p, 
        typename AllocationPolicy::size_type cnt) { 
        AllocationPolicy::deallocate(p, cnt);
        this->current_ -= cnt;
    }

    // get stats
    inline typename AllocationPolicy::size_type 
          TotalAllocations() { return this->total_; }
    inline typename AllocationPolicy::size_type 
          CurrentAllocations() { return this->current_; }
    inline typename AllocationPolicy::size_type 
          PeakAllocations() { return this->peak_; }

private : 
    //    total allocations
    typename AllocationPolicy::size_type total_;    
    //    current allocations
    typename AllocationPolicy::size_type current_; 
    //    peak allocations   
    typename AllocationPolicy::size_type peak_;    
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