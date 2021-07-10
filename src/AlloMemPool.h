#ifndef MYALLOCATOR_ALLOMEMPOOL_H
#define MYALLOCATOR_ALLOMEMPOOL_H
#include <iostream>
#include <cstdio>
#include <cstddef>
#include <list>
#include <queue>

namespace MemMgr{
    const uint32_t block_size = 8192;
    const uint32_t block_in_pool = 128;
    template <class T>
    class allocator_list{
    public:
        typedef void _Not_user_specialized;
        typedef T value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;


        class mem_ptr{
        public:
            void* pData = nullptr;
            size_t id;
            mem_ptr() = default;
            mem_ptr(void* p, int i):pData(p), id(i){}
            ~mem_ptr()= default;
        };
        typedef mem_ptr *block_ptr;
        allocator_list(): free_block(0), total_block(0), max_id(0)
        {
            free_head = nullptr;
            free_tail = nullptr;
        }// default ctor
        allocator_list(const allocator_list& t)
        {
            free_head = nullptr;
            free_tail = nullptr;
        } // copy ctor
        ~allocator_list() {} // dtor

        pointer address(reference _Val) const {
            return &_Val;
        }

        const_pointer address( const_reference x ) const;
        void deallocate(pointer p, size_type n)
        {
            if (p != nullptr) {
                for(auto &iter : all_block){
                    if(iter.pData == reinterpret_cast<void*>(p)){
                        valid_block.template emplace_back(p, iter.id);
                    }
                }
            }
            else{
                std::cout << "NULLPTR----allocator_list::deallocate(pointer p, size_type n)  "<< std::endl;

            }
//            std::cout << "allocator_list::deallocate(pointer p, size_type n)  " << n << std::endl;
        }
//        void deallocate( T* p, std::size_t n );
//        T* allocate( std::size_t n, const void * hint = 0);

        T* allocate( std::size_t n, const void * hint = 0 )
        {
//            T* _ret = static_cast<T*>(::operator new(n * sizeof(T)));
//            return reinterpret_cast<T*>(::operator new(n * sizeof(T)) );

            if(n * sizeof(T) > block_size){
                std::cout << "Allocate from operator new!!" <<std::endl;
                return reinterpret_cast<T*>(::operator new(n * sizeof(T)) );
            }
            if(valid_block.empty()){
                allocate_mem();
            }

            // Assign a block of memory from the memory pool
            mem_ptr out = valid_block.front();
            valid_block.pop_front();
            std::cout << valid_block.size() << " block left!" <<std::endl;
            return reinterpret_cast<T*>(out.pData);

        }

        template< class U >
        void destroy( U* p )
        {
            p->~U();
//            std::cout << "allocator_list::destroy( U* p )  " << std::endl;

        }
        template< class U , class... Args >
        void construct( U* p, Args&&... args )
        {
            ::new((void *)p) U(std::forward<Args>(args)...);
//            std::cout << "construct at:  " << static_cast<void*>(p) << std::endl;
//
//            std::cout << "allocator_list::construct( U* p, Args&&... args )  " << std::endl;

        }



    private:
        struct mem_block{
            value_type pData;
            mem_block* next = nullptr;
        };

        std::list<mem_ptr> valid_block;
        std::list<mem_ptr> all_block;

        mem_block* total_head;
        mem_block* free_head;
        mem_block* free_tail;
        uint32_t free_block{};
        uint32_t total_block{};
        uint32_t max_id;

        void* allocate_mem(size_t n = block_size * block_in_pool)
        {
            void* _new_mem = static_cast<void*>(::operator new(n));
            void* cyc = _new_mem;
            for(auto i = 0; i < block_in_pool; i++){
                valid_block.template emplace_back(cyc, max_id);
                all_block.template emplace_back(cyc, max_id);
                max_id++;
//                std::cout << max_id << " : " << cyc << std::endl;
                cyc = reinterpret_cast<void *>(reinterpret_cast<uint64_t>(cyc) + block_size);
            }

            return nullptr;

        }

    };




}

#endif //MYALLOCATOR_ALLOMEMPOOL_H
