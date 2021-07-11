#ifndef MYALLOCATOR_ALLOMEMPOOL_H
#define MYALLOCATOR_ALLOMEMPOOL_H
#include <iostream>
#include <cstdio>
#include <cstddef>
#include <list>
#include <map>
#include <queue>

namespace MemMgr{
    const uint32_t block_size = 4096;
    const uint32_t block_in_pool = 128;
    const uint32_t expand_max = 65536; // max expand size for heap allocator

    class mem_ptr_list{
        // class of list pointer, points to specific address which means block
    public:
        void* pData = nullptr;
        size_t id;
        mem_ptr_list() = default;
        mem_ptr_list(void* p, int i): pData(p), id(i){}
        ~mem_ptr_list()= default;
    };
    static std::list<mem_ptr_list> valid_block; // valid blocks to assign
    static std::list<mem_ptr_list> all_block; // all the blocks
    // class of linked list allocator
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

        static uint32_t max_id ;

        typedef mem_ptr_list *block_ptr;
        allocator_list() = default; // default ctor
        allocator_list(const allocator_list& t)
        {} // copy ctor
        ~allocator_list() = default;// dtor

        pointer address(reference _Val) const {
            return &_Val;
        }

        const_pointer address( const_reference x ) const;
        void deallocate(pointer p, size_type n)
        {
            if(n >= block_size){
                ::operator delete(p);
                return;
            }
            // Find the block with the specific id and push back to free_list
            if (p != nullptr) {
                valid_block.template emplace_back(p, max_id++); // Add back to valid ones
            }
            else{
                std::cout << "NULLPTR----allocator_list::deallocate(pointer p, size_type n)  "<< std::endl;

            }
        }

        T* allocate( std::size_t n, const void * hint = 0 )
        {
            if(n * sizeof(T) > block_size){
                // If larger, just allocate from new
//                std::cout << "Allocate from ::operator new!!" <<std::endl;
                return reinterpret_cast<T*>(::operator new(n * sizeof(T)) );
            }
            if(valid_block.empty()){
                allocate_mem();
            }

            // Assign a block of memory from the memory pool
            mem_ptr_list out = valid_block.front();
            valid_block.pop_front();
//            std::cout << valid_block.size() << " block left!" <<std::endl;
            return reinterpret_cast<T*>(out.pData);

        }

        template< class U >
        void destroy( U* p )
        {
            p->~U();

        }
        template< class U , class... Args >
        void construct( U* p, Args&&... args )
        {
            // Construct elements
            ::new((void *)p) U(std::forward<Args>(args)...);
        }

    private:

        void* allocate_mem(size_t n = block_size * block_in_pool)
        {
            void* _new_mem = static_cast<void*>(::operator new(n));
            void* cyc = _new_mem;
            for(auto i = 0; i < block_in_pool; i++){
                // push the assigned valid block of memory back to list
                valid_block.template emplace_back(cyc, max_id);
                all_block.template emplace_back(cyc, max_id);
                max_id++;
                // note that we need to divide the memory block into smaller pieces.
                cyc = reinterpret_cast<void *>(reinterpret_cast<uint64_t>(cyc) + block_size);
            }

            return nullptr;

        }

    };
    template<class T>
    uint32_t allocator_list<T>::max_id = 0;

    class mem_ptr_heap{
    public:
        void* pData = nullptr;
        size_t id;
        uint64_t size;
        mem_ptr_heap() = default;
        mem_ptr_heap(void* p, int i, uint64_t s): pData(p), id(i), size(s) {}
        ~mem_ptr_heap()= default;
        bool operator<(const mem_ptr_heap& that) const{
            return size < that.size; //max-heap
        }
    };
    static std::priority_queue<mem_ptr_heap> valid_block_heap;
    static std::map<void*, uint64_t> all_block_heap; // To store whether the memory is assigned by pool or operator new for further delete

    template<class T>
    class allocator_heap{
    public:
        typedef void _Not_user_specialized;
        typedef T value_type;
        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        static uint32_t max_id ;
        static uint64_t current_blocksize;

        typedef mem_ptr_heap *block_ptr;
        allocator_heap()
        {}// default ctor
        allocator_heap(const allocator_heap& t)
        {} // copy ctor
        ~allocator_heap() {} // dtor

        pointer address(reference _Val) const {
            return &_Val;
        }

        const_pointer address( const_reference x ) const;
        void deallocate(pointer p, size_type n)
        {
            // Find the block with the specific id and push back to free_list
            if (p != nullptr) {
                void* t = reinterpret_cast<void*>(p);
                if(all_block_heap.count(t) == 1){
                    // If the memory is allocated by pool
                    valid_block_heap.template emplace(p, max_id++, all_block_heap[t]);
                }
                else{
                    // If the memory is allocated by operator new
                    ::operator delete(p);
                    return;
                }
            }
            else{
                std::cout << "NULLPTR----allocator_heap::deallocate(pointer p, size_type n)  "<< std::endl;

            }
        }


        T* allocate( std::size_t n, const void * hint = 0 )
        {
            if(valid_block_heap.size() <= 1){
                allocate_mem();
            }
            while ( n * sizeof(T) > valid_block_heap.top().size){
                if(current_blocksize <= expand_max){
                    allocate_mem();
                    if(valid_block_heap.size() != 0){
//                        std::cout << n * sizeof(T) << " bytes needed, but the max block is "<<valid_block_heap.top().size<<std::endl;
                    }
                }
                else{
//                    std::cout << "No more expansion, the max block size is " << expand_max << std::endl;
//                    std::cout << "Allocate from ::operator new!!" <<std::endl;
                    return reinterpret_cast<T*>(::operator new(n * sizeof(T)) );

                }

            }
            if(n * sizeof(T) > valid_block_heap.top().size){
//                std::cout << "Allocate from ::operator new!!" <<std::endl;
                return reinterpret_cast<T*>(::operator new(n * sizeof(T)) );
            }


            // Assign a block of memory from the memory pool
            mem_ptr_heap out = valid_block_heap.top();
            valid_block_heap.pop();
            return reinterpret_cast<T*>(out.pData);

        }

        template< class U >
        void destroy( U* p )
        {
            p->~U();
        }
        template< class U , class... Args >
        void construct( U* p, Args&&... args )
        {
            ::new((void *)p) U(std::forward<Args>(args)...);
        }



    private:
        void* allocate_mem(size_t n = current_blocksize * block_in_pool)
        {
            void* _new_mem = static_cast<void*>(::operator new(n));
            void* cyc = _new_mem;
            for(auto i = 0; i < block_in_pool; i++){
                // push the assigned valid block of memory back to heap
                valid_block_heap.template emplace(cyc, max_id, current_blocksize);
                all_block_heap.insert(std::pair<void*, uint64_t>(cyc, current_blocksize)); // sign as allocated from pool
                max_id++;
                // note that we need to divide the memory block into smaller pieces.
                cyc = reinterpret_cast<void *>(reinterpret_cast<uint64_t>(cyc) + current_blocksize);
            }
//            std::cout << "Block size expand from " << current_blocksize/2 << " to " << current_blocksize  << std::endl;
            current_blocksize *= 2; // expand block size
            return nullptr;

        }

    };
    template<class T>
    uint32_t allocator_heap<T>::max_id = 0;
    template<class T>
    uint64_t allocator_heap<T>::current_blocksize = 512; // Initial block size

}

#endif //MYALLOCATOR_ALLOMEMPOOL_H
