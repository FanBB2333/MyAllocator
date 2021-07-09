#ifndef MYALLOCATOR_ALLOMEMPOOL_H
#define MYALLOCATOR_ALLOMEMPOOL_H
#include <iostream>
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

        allocator() {};// default ctor
        allocator(const allocator& t); // copy ctor
        ~allocator() {}; // dtor

        pointer address(reference _Val) const;
        const_pointer address( const_reference x ) const;
        void deallocate(pointer p, size_type n)
        {
            ::operator delete(p);
            std::cout << "allocator::deallocate(pointer p, size_type n)  " << n << std::endl;
        }
//        void deallocate( T* p, std::size_t n );
//        T* allocate( std::size_t n, const void * hint = 0);

        T* allocate( std::size_t n )
        {
            T* _ret = static_cast<T*>(::operator new(n * sizeof(T)));
            if(!_ret){
                std::cerr << "Out of Memory!" << std::endl;
            }
            std::cout << "allocator::allocate( std::size_t n )  " << n << std::endl;
            return _ret;
        }

        template< class U >
        void destroy( U* p )
        {
            p->~U();
            std::cout << "allocator::destroy( U* p )  " << std::endl;

        }
        template< class U, class... Args >
        void construct( U* p, Args&&... args )
        {
            ::new((void *)p) U(std::forward<Args>(args)...);
            std::cout << "allocator::construct( U* p, Args&&... args )  " << std::endl;

        }

    private:
        pointer free_head;

        void allocate_mem()
        {
            
        }

    };




}

#endif //MYALLOCATOR_ALLOMEMPOOL_H
