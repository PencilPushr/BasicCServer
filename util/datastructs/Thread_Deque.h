#pragma once

/*
buff_size is the length of the chunk
*/
template <class T, size_t buff_size>
struct chunk {
    using iterator = __deque_iterator<T, buff_size>;

    // pointer to the chunk
    T* cur;
    T* first;     // the begin of the chunk
    T* last;      // the end of the chunk

    //because the pointer may skip to other chunk
    //so this pointer to the map
    T** node;    // pointer to the map
};

template<typename T, size_t buff_size = 0>
class deque {
public:
    using chunk<T, buff_size> iterator;

public:
    Deque();
    ~Deque();

    PushFront();
    PushBack();
    PopFront();
    PopBack();


protected:
    // allocate memory for the chunk 
    using dataAllocator = allocator<T>;
    // allocate memory for map 
    using mapAllocatior = allocator<T>;

private:

    //data members
    iterator start;
    iterator finish;

    T** map;
    size_t   map_size;
};

// Functions

template<typename T, size_t buff_size>
inline deque<T, buff_size>::Deque()
{

}

template<typename T, size_t buff_size>
inline deque<T, buff_size>::~Deque()
{
}

template<typename T, size_t buff_size>
inline deque<T, buff_size>::PushFront()
{
}

template<typename T, size_t buff_size>
inline deque<T, buff_size>::PushBack()
{
}

template<typename T, size_t buff_size>
inline deque<T, buff_size>::PopFront()
{
}

template<typename T, size_t buff_size>
inline deque<T, buff_size>::PopBack()
{
}
