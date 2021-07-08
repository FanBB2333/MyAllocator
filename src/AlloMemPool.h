#ifndef MYALLOCATOR_ALLOMEMPOOL_H
#define MYALLOCATOR_ALLOMEMPOOL_H

#include <cstdio>
#include <cstddef>
namespace MemMgr{
    template <class T>
    class allocator{
    public:
        typedef void _Not_user_specialized;
        typedef T value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;


        pointer address(reference _Val) const;
        const_pointer address( const_reference x ) const;
        void deallocate( T* p, std::size_t n );
        T* allocate( std::size_t n, const void * hint);
        template< class U >
        void destroy( U* p );
        template< class U, class... Args >
        void construct( U* p, Args&&... args );
    };

}

#endif //MYALLOCATOR_ALLOMEMPOOL_H
