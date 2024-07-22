#include "ServerListener.h"

ServerListener::ServerListener(Client* parent_thread, size_t buffer_size)
	: m_parent_thread(parent_thread)
{
	// preserve geometric growth of vector (.reserve() will not)
	m_input_buffer.resize(m_input_buffer.size() + buffer_size);
}

ServerListener::~ServerListener()
{
}

void ServerListener::Run()
{

}

void ServerListener::SelfCall(ServerListener* self)
{
	self->Run();
}
