#include "ServerListener.h"

#include "CE303/Client.h"

ServerListener::ServerListener(Client* parent_thread, size_t buffer_size)
	: m_parent_thread(parent_thread), m_buffer_size(buffer_size)
{
	// preserve geometric growth of vector (.reserve() will not)
	m_input_buffer.resize(m_input_buffer.size() + buffer_size);
}

ServerListener::~ServerListener()
{
}

void ServerListener::Run()
{
    std::vector<char> buffer(m_buffer_size);

    while (true) {
        int bytes_received = recv(m_parent_thread->GetSocket(), buffer.data(), buffer.size(), 0);
        if (bytes_received > 0) 
        {
            std::cout.write(buffer.data(), bytes_received);
            std::cout << std::endl;
        }
        else if (bytes_received == 0) 
        {
            std::cout << "Server closed the connection." << std::endl;
            break;
        }
        else 
        {
            std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
            break;
        }
    }
}

void ServerListener::SelfCall(ServerListener* self)
{
	self->Run();
}
