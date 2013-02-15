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

#ifndef __standard_policy_hpp__
#define __standard_policy_hpp__

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
        return reinterpret_cast<pointer>(::operator new(cnt * sizeof (T)));
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

#endif //__standard_policy_hpp__