/*
 * =====================================================================================
 *
 *       Filename:  Allocator.hpp
 *
 *    Description:  Allocator class composed from two elements: Allocation policy and
 *                  Object traits. Policy is responsible for actual allocation, traits
 *                  can return the address of particular object. Specific object
 *                  implementations (like overloaded '&' operator) require modified
 *                  Trait class.
 *                  Depending on policy, allocation can be done in different flavours.
 *                  See Policy.hpp for specific options.
 *
 *        Version:  1.0
 *        Created:  02/09/2013 11:25:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Vytis Valentinavičius (), vytis.valentinavicius@gmail.com
 *        Company:  VU FF
 *
 *          Notes:  1) Note that the container always holds a copy of the allocator
 *                     object that is passed to the constructor. If you need a single
 *                     allocator object to manage all storage for a number of containers,
 *                     you must provide an allocator that maintains a reference to some
 *                     shared implementation.
 *                  o  Implement by using singleton pattern without binding to
 *                     typename T.
 *                  o  Find a way to have two different allocators with same type, but
 *                     distinct memory location.
 *
 * =====================================================================================
 */

#include "./internals/Traits.hpp"
#include "./internals/Policy.hpp"



template<typename T,
typename Policy = StandardAllocPolicy<T>,
typename Traits = StandardObjectTraits<T> >
class Allocator : public Policy, public Traits {
private :
    typedef Policy AllocationPolicy;
    typedef Traits ObjectTraits;

public :
    //  Extract typenames from Policy 
    typedef typename AllocationPolicy::size_type 		size_type;
    typedef typename AllocationPolicy::difference_type 		difference_type;
    typedef typename AllocationPolicy::pointer 			pointer;
    typedef typename AllocationPolicy::const_pointer 		const_pointer;
    typedef typename AllocationPolicy::reference 		reference;
    typedef typename AllocationPolicy::const_reference 		const_reference;
    typedef typename AllocationPolicy::value_type 		value_type;

public :
    //  Structure which allows Allocator changes from one type to another
    template<typename U>
    struct rebind {
        typedef Allocator<U, 
        typename AllocationPolicy::template rebind<U>::other,
        typename ObjectTraits::template rebind<U>::other > other;
    };

public :
    inline explicit Allocator() {}
    inline ~Allocator() {}
    inline Allocator(Allocator const& rhs): Policy(rhs), Traits(rhs) {}
    
    template <typename U>
    inline Allocator(Allocator<U> const&) {}
    
    template <typename U, typename PU, typename TrU>
    inline Allocator(Allocator<U, PU, TrU> const& rhs): Policy(rhs), Traits(rhs) {}
};    //    end of class Allocator

//  Determines if memory from another
// allocator can be deallocated from this one
// This solely depends on policy used by allocator.
template<typename T, typename P, typename Tr>
inline bool operator==(Allocator<T, P,
                       Tr> const& lhs, Allocator<T,
                       P, Tr> const& rhs) {
    return operator==(static_cast<P&>(lhs),
                      static_cast<P&>(rhs));
}

template<typename TA, typename PA, typename TrA,
         typename TB, typename PB, typename TrB>
inline bool operator==(Allocator<TA, PA, TrA> const& lhs,
                       Allocator<TB, PB, TrB> const& rhs) {
    return operator==(static_cast<PA&>(lhs),
                      static_cast<PB&>(rhs));
}

template<typename T, typename P, typename Tr, typename OtherAllocator>
inline bool operator==(Allocator<T, P, Tr> const& lhs,
                       OtherAllocator const& rhs) {
    return operator==(static_cast<P&>(lhs), rhs);
}

template<typename T, typename P, typename Tr>
inline bool operator!=(Allocator<T, P, Tr> const& lhs,
                       Allocator<T, P, Tr> const& rhs) {
    return !operator==(lhs, rhs);
}

template<typename TA, typename PA, typename TrA,
         typename TB, typename PB, typename TrB>
inline bool operator!=(Allocator<TA, PA, TrA> const& lhs,
                       Allocator<TB, PB, TrB> const& rhs) {
    return !operator==(lhs, rhs);
}

template<typename T, typename P, typename Tr, typename OtherAllocator>
inline bool operator!=(Allocator<T, P, Tr> const& lhs,
                       OtherAllocator const& rhs) {
    return !operator==(lhs, rhs);
}
