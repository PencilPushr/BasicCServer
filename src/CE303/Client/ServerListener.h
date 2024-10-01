#pragma once

#include <vector>

#include "datastructs/SocketType/SocketType.h"

class Client;

class ServerListener
{
public:
	ServerListener() = default;
	ServerListener(Client* parent_thread, size_t buffer_size);
	~ServerListener();

public:
	static void SelfCall(ServerListener* self);

private:
	void Run();

private:
	Client* m_parent_thread;
	size_t m_buffer_size;
	std::vector<char> m_input_buffer = { 0 };
};