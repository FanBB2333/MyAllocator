#ifndef MYALLOCATOR_ALLOMEMPOOL_H
#define MYALLOCATOR_ALLOMEMPOOL_H
#include <iostream>
#include <cstdio>
#include <cstddef>
namespace MemMgr{
    const uint32_t page_size = 4096;
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

        allocator():free_block(0), total_block(0)
        {
            free_head = nullptr;
            free_tail = nullptr;
        }// default ctor
        allocator(const allocator& t)
        {
            free_head = nullptr;
            free_tail = nullptr;
        } // copy ctor
        ~allocator() {} // dtor

        pointer address(reference _Val) const {
            return &_Val;
        }

        const_pointer address( const_reference x ) const;
        void deallocate(pointer p, size_type n)
        {
            if (p != nullptr) {
                std::cout << "p: " << static_cast<void*>(p) << "  Free head: " << static_cast<void*>(free_head) <<std::endl;

                reinterpret_cast<mem_block*>(p)->next = free_head;
                free_head = reinterpret_cast<mem_block*>(p);
            }
            else{
                std::cout << "NULLPTR----allocator::deallocate(pointer p, size_type n)  "<< std::endl;

            }
            std::cout << "allocator::deallocate(pointer p, size_type n)  " << n << std::endl;
        }
//        void deallocate( T* p, std::size_t n );
//        T* allocate( std::size_t n, const void * hint = 0);

        T* allocate( std::size_t n, const void * hint = 0 )
        {
//            T* _ret = static_cast<T*>(::operator new(n * sizeof(T)));
            T* _ret = (T*)free_head;
            // If free_head is nullptr

//            if (free_head != nullptr) {
//                pointer result = reinterpret_cast<pointer>(free_head);
//                free_head = free_head->next;
//                return result;
//            }
//            else {
//                if (free_head == nullptr)
//                    free_head = reinterpret_cast<mem_block*>(allocate_mem());
//                return reinterpret_cast<pointer>(free_head++);
//            }

            if(!_ret){
                 _ret = (T*)(allocate_mem());
//                 free_head = reinterpret_cast<mem_block*>(_ret);
//                 free_head = free_head->next;
                 std::cout << "Reallocate a block!!" << static_cast<void*>(_ret)<<std::endl;
            }
            else{
                _ret = reinterpret_cast<T*>(free_head);
                for(size_t i = 0; i < n; i++){
                    free_head = free_head->next;
                }
            }

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
        template< class U , class... Args >
        void construct( U* p, Args&&... args )
        {
            ::new((void *)p) U(std::forward<Args>(args)...);
            std::cout << "construct at:  " << static_cast<void*>(p) << std::endl;

            std::cout << "allocator::construct( U* p, Args&&... args )  " << std::endl;

        }



    private:
        struct mem_block{
            value_type pData;
            mem_block* next = nullptr;
        };
        mem_block* total_head;
        mem_block* free_head;
        mem_block* free_tail;
        uint32_t free_block{};
        uint32_t total_block{};


        mem_block* allocate_mem(size_t n = page_size)
        {
//            mem_block* _ptr = static_cast<T*>(::operator new(n * sizeof(mem_block)));
            mem_block* _ptr = static_cast<mem_block*>(::operator new(n * sizeof(mem_block)));
            if(!_ptr){
                std::cerr << "Out of Memory!" << std::endl;
            }
            mem_block* tmp_tail = free_head;
            free_head = _ptr;
            mem_block* p = _ptr;
            for(int i = 0; i < n - 1; i++){
                p->next = p + 1;
//                std::cout << static_cast<const void *>(p) << std::endl;
//                printf("i:%d#pData:%d\n", i, p->pData);
                p = p->next;

            }
            free_tail = p;
            std::cout << "allocator::allocate_mem( size_t n )  " << n << std::endl;
            return _ptr;
        }

    };




}

#endif //MYALLOCATOR_ALLOMEMPOOL_H
