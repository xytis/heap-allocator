/*
 * =====================================================================================
 *
 *       Filename:  Traits.hpp
 *
 *    Description:  Standart object trait, which returns the address of the object using
 *                  '&' operator.
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



template<typename T>
class StandardObjectTraits {
public : 
    //    convert an StandardObjectTraits<T> to StandardObjectTraits<U>
    template<typename U>
    struct rebind {
        typedef StandardObjectTraits<U> other;
    };

public : 
    inline explicit StandardObjectTraits() {}
    inline ~StandardObjectTraits() {}
    template <typename U>
    inline explicit StandardObjectTraits(StandardObjectTraits<U> const&) {}

    //    address
    inline T* address(T& r) { return &r; }
    inline T const* address(T const& r) { return &r; }

    inline void construct(T* p, const T& t) { new(p) T(t); }
    inline void destroy(T* p) { p->~T(); }
};    //    end of class StandardObjectTraits