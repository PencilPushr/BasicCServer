#pragma once

#include "DataUtil.h"

#include <memory>
#include <stdexcept>

#ifdef THREAD
    #include <mutex>
    #include <atomic>
#endif

/*
buff_size is the length of the chunk
*/
template <class T, size_t buff_size>
struct chunk {
    T* cur;
    T* first;    
    T* last;      
    T** node;    // pointer to the map
};

template<typename T, size_t buff_size = 0>
class deque {
public:
    using chunk = chunk<T, buff_size>;
    using INITIAL_MAP_SIZE = 8;

    // Explanation:
    // 1. (buff_size != 0): Check if buff_size is non-zero
    // 2. If true, use buff_size
    // 3. If false, use (512 / sizeof(T))
    using CHUNK_SIZE = (buff_size != 0) ? buff_size : (512 / sizeof(T));

public:
    Deque();
    ~Deque();

    void PushFront();
    void PushBack();
    T PopFront();
    T PopBack();

    bool Empty() const;
    size_t Size() const;

private:
    void CreateFirstChunk();
    void RangeCheck(size_t n) const;
    void CentreChunks();

protected:

    // allocate memory for the chunk 
    using dataAllocator = allocator<T>;
    // allocate memory for map 
    using mapAllocatior = allocator<T>;

private:

    chunk   start;
    chunk   finish;

    T**     map;
    size_t  map_size = INITIAL_MAP_SIZE;
    size_t  elements_count;
};

// Functions

template<typename T, size_t buff_size>
inline deque<T, buff_size>::Deque() : map{ nullptr }, map_size{ 0 }, elements_count{ 0 }
{
    CreateFirstChunk();
}

template<typename T, size_t buff_size>
inline deque<T, buff_size>::~Deque()
{
}

template<typename T, size_t buff_size>
inline void deque<T, buff_size>::PushFront()
{
}

template<typename T, size_t buff_size>
inline void deque<T, buff_size>::PushBack()
{
}

template<typename T, size_t buff_size>
inline T deque<T, buff_size>::PopFront()
{
}

template<typename T, size_t buff_size>
inline T deque<T, buff_size>::PopBack()
{
}

template<typename T, size_t buff_size>
inline bool deque<T, buff_size>::Empty() const
{
    return false;
}

template<typename T, size_t buff_size>
inline size_t deque<T, buff_size>::Size() const
{
    return size_t();
}

template<typename T, size_t buff_size>
inline void deque<T, buff_size>::CreateFirstChunk()
{
}

template<typename T, size_t buff_size>
inline void deque<T, buff_size>::RangeCheck(size_t n) const
{
    if (n >= size()) {
        throw std::out_of_range("deque index out of range");
    }
}

template<typename T, size_t buff_size>
inline void deque<T, buff_size>::CentreChunks()
{
}
