#include "SocketStreambuf.h"
#include <cstring>

SocketStreambuf::SocketStreambuf(socket_t socket_fd, std::size_t buff_sz)
        : m_socket(socket_fd)
        , in_buffer_(buff_sz)
        , out_buffer_(buff_sz)
{
    // 1. Set up the input buffer pointers
    setg(in_buffer_.data(), in_buffer_.data() + in_buffer_.size(), in_buffer_.data() + in_buffer_.size());

    // 2. Set up the output buffer pointers
    setp(out_buffer_.data(), out_buffer_.data() + out_buffer_.size());
}

// Called when the input buffer is empty. It reads data from the socket into the buffer.
SocketStreambuf::int_type SocketStreambuf::underflow()
{
    // 1. Check if there's still data in the buffer
    if (gptr() < egptr())
    {
        return traits_type::to_int_type(*gptr());
    }

    // 2. We overwrite the buffer with new data
#ifdef _WIN32
    int n = recv(m_socket, in_buffer_.data(), static_cast<int>(in_buffer_.size()), 0);
#else
    ssize_t n = read(m_socket, in_buffer_.data(), in_buffer_.size());
#endif

    if (n <= 0) {
        return traits_type::eof();
    }

    // Reset buffer pointers after reading
    setg(in_buffer_.data(), in_buffer_.data(), in_buffer_.data() + n);

    return traits_type::to_int_type(*gptr());
}

// Called when the output buffer is full or when flushing is needed. It writes data from the buffer to the socket.
SocketStreambuf::int_type SocketStreambuf::overflow(int_type ch)
{
    if (!traits_type::eq_int_type(ch, traits_type::eof()))
    {
        *pptr() = traits_type::to_char_type(ch);
        pbump(1);
    }

    return sync() == 0 ? traits_type::not_eof(ch) : traits_type::eof();
}

// Flushes the output buffer.
int SocketStreambuf::sync()
{
    std::ptrdiff_t n = pptr() - pbase();
    if (n > 0) {
#ifdef _WIN32
        int sent = send(m_socket, pbase(), static_cast<int>(n), 0);
#else
        ssize_t sent = write(m_socket, pbase(), n);
#endif
        if (sent <= 0 || sent != n) {
            return -1;  // Error
        }
        pbump(-n);  // Reset the put pointer
    }
    return 0;
}