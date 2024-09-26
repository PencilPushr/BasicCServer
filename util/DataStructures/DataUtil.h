#pragma once

// SFINAE -> function template is discarded if the unamed template parameters fail.
template<typename Container, typename Index,
    typename = std::enable_if_t<std::is_integral_v<Index>>, // Check if 2nd parameter is integral
    typename = decltype(std::declval<Container>().size())>  // Check if the Container has a .size() method
inline void RangeCheck(const Container& c, Index n) {
    if (n < 0 || static_cast<typename Container::size_type>(n) >= c.size()) {
        throw std::out_of_range("index out of range");
    }
}