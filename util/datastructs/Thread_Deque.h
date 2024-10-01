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
struct Chunk
{
    T* cur;
    T* first;    
    T* last;      
    T** node;    // pointer to the map
};

template<typename T, size_t buff_size = 0>
class Deque
{
public:
    using CHUNK = Chunk<T, buff_size>;
    static constexpr int INITIAL_MAP_SIZE = 8;
    static constexpr int CHUNK_SIZE = (buff_size != 0) ? buff_size : (512 / sizeof(T));
    // Explanation:
    // 1. (buff_size != 0): Check if buff_size is non-zero
    // 2. If true, use buff_size
    // 3. If false, use (512 / sizeof(T))

public:
    Deque();
    ~Deque();

    void PushFront(const T& value);
    void PushBack(const T& value);
    T PopFront();
    T PopBack();

    bool Empty() const;
    size_t Size() const;

private:
    void CreateFirstChunk();
    void RangeCheck(size_t n) const;
    void CentreChunks();
    void CreateChunkAtFront();
    void CreateChunkAtBack();
    void DestroyChunkAtFront();
    void DestroyChunkAtBack();
    void ReallocateMap(size_t new_map_size);

protected:

    // allocate memory for the chunk 
    using data_allocator = std::allocator<T>;
    // allocate memory for map 
    using map_allocator = std::allocator<T*>;

private:

    CHUNK   m_start;
    CHUNK   m_finish;

    T**     m_map;
    size_t  m_map_size;
    size_t  m_elements_count;
};

// Functions

template<typename T, size_t buff_size>
inline Deque<T, buff_size>::Deque()
    : m_map{ nullptr }
    , m_map_size{ INITIAL_MAP_SIZE }
    , m_elements_count{ 0 }
{
    CreateFirstChunk();
}

template<typename T, size_t buff_size>
inline Deque<T, buff_size>::~Deque()
{
    for (int i = 0; i < m_map_size; ++i)
    {
        data_allocator::deallocate(m_map[i], CHUNK_SIZE);
    }
    map_allocator::deallocate(m_map, m_map_size);
}

template<typename T, size_t buff_size>
inline void Deque<T, buff_size>::PushFront(const T& value)
{
    if (m_start.cur == m_start.first)
    {
        CentreChunks();
        CreateChunkAtFront();
    }
    --m_start.cur;
    data_allocator::construct(m_start.cur, value);
    ++m_elements_count;
}

template<typename T, size_t buff_size>
inline void Deque<T, buff_size>::PushBack(const T& value)
{
    if (m_start.cur == m_start.first)
    {
        CentreChunks();
        CreateChunkAtBack();
    }
    ++m_finish.cur;
    data_allocator::construct(m_start.cur, value);
    ++m_elements_count;
}

template<typename T, size_t buff_size>
inline T Deque<T, buff_size>::PopFront()
{
    if (Empty()) {
        throw std::out_of_range("Deque is empty");
    }
    T value = *m_start.cur;
    data_allocator::destroy(m_start.cur);
    ++m_start.cur;
    --m_elements_count;
    if (m_start.cur == m_start.last) {
        DestroyChunkAtFront();
    }
    return value;
}

template<typename T, size_t buff_size>
inline T Deque<T, buff_size>::PopBack()
{
    if (Empty())
    {
        throw std::out_of_range("Deque is empty");
    }
    --m_finish.cur;
    T value = *m_finish.cur;
    data_allocator::destroy(m_finish.cur);
    --m_elements_count;
    if (m_finish.cur == m_finish.first) {
        DestroyChunkAtBack();
    }
    return value;
}

template<typename T, size_t buff_size>
inline bool Deque<T, buff_size>::Empty() const
{
    return (m_elements_count == 0);
}

template<typename T, size_t buff_size>
inline size_t Deque<T, buff_size>::Size() const
{
    return m_elements_count;
}

template<typename T, size_t buff_size>
inline void Deque<T, buff_size>::CreateFirstChunk()
{
    m_map = map_allocator::allocate(INITIAL_MAP_SIZE); // T**
    // m_map_size = INITIAL_MAP_SIZE; We already did this in the constructor

    T* chunk = data_allocator::allocate(CHUNK_SIZE);
    m_map[m_map_size / 2] = chunk; // Assign the chunk to the middle of the map

    m_start.node = m_map + m_map_size / 2; // The address of the map (at index 0) + half the map size
    m_finish.node = m_start.node;

    // Set up the members to point to the right blocks internally
    m_start.first = *m_start.node;
    m_start.last = m_start.first + CHUNK_SIZE;
    m_start.cur = m_start.first;

    // Set m_finish to m_start, as the first block m_start and m_finish pointers sit on the same chunk
    m_finish = m_start;
}

template<typename T, size_t buff_size>
inline void Deque<T, buff_size>::RangeCheck(size_t n) const
{
    if (n >= size()) {
        throw std::out_of_range("Deque index out of range");
    }
}

template<typename T, size_t buff_size>
inline void Deque<T, buff_size>::CentreChunks()
{
    int used_chunks = (m_finish.node - m_start.node) + 1; // Get the difference which is equivalent to the range
    if (used_chunks < m_map_size / 2) // If the range of used_chunks is less than the middle of the map (or map_size / 2)
    {
        int new_start = (m_map_size - used_chunks) / 2;
        std::copy(m_start.node, m_finish.node + 1, m_map + new_start);
        for (int i = 0; i < used_chunks; ++i)
        {
            m_map
        }
        m_start.node = m_map + new_start;
        m_finish.node = m_start.node + used_chunks - 1;
    }
}

template<typename T, size_t buff_size>
void Deque<T, buff_size>::CreateChunkAtFront()
{
    if (m_start.node == m_map) {
        ReallocateMap(m_map_size * 2);
    }
    *(--m_start.node) = data_allocator::allocate(CHUNK_SIZE);
    m_start.cur = m_start.last = *m_start.node + CHUNK_SIZE;
    m_start.first = *m_start.node;
}

template<typename T, size_t buff_size>
void Deque<T, buff_size>::CreateChunkAtBack()
{
    if (m_finish.node == m_map + m_map_size - 1) {
        ReallocateMap(m_map_size * 2);
    }
    *(++m_finish.node) = data_allocator::allocate(CHUNK_SIZE);
    m_finish.first = *m_finish.node;
    m_finish.cur = m_finish.last = *m_finish.node + CHUNK_SIZE;
}


template<typename T, size_t buff_size>
void Deque<T, buff_size>::DestroyChunkAtFront()
{
    data_allocator::deallocate(*m_start.node, CHUNK_SIZE);
    ++m_start.node;
    m_start.cur = m_start.first = *m_start.node;
    m_start.last = m_start.first + CHUNK_SIZE;
}

template<typename T, size_t buff_size>
void Deque<T, buff_size>::DestroyChunkAtBack()
{
    data_allocator::deallocate(*m_finish.node, CHUNK_SIZE);
    --m_finish.node;
    m_finish.first = *m_finish.node;
    m_finish.cur = m_finish.last = m_finish.first + CHUNK_SIZE;
}

template<typename T, size_t buff_size>
void Deque<T, buff_size>::ReallocateMap(size_t new_map_size)
{
    T** new_map = map_allocator::allocate(new_map_size);

    size_t old_chunks = m_finish.node - m_start.node + 1;
    size_t new_start = (new_map_size - old_chunks) / 2;
    std::copy(m_start.node, m_finish.node + 1, new_map + new_start);

    int range = (m_finish.node - m_start.node) + 1;
    for (int i = 0; i < range; ++i)
    {

    }

    map_allocator::deallocate(m_map, m_map_size);
    m_map = new_map;
    m_map_size = new_map_size;
    m_start.node = m_map + new_start;
    m_finish.node = m_start.node + old_chunks - 1;
}