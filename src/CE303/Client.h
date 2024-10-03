#pragma once

#include <iostream>
#include <thread>
#include <string>

/*
*			Winsock Client side normally as follows:
*	1. Initialize WSA			- WSAStartup().     Done in SocketType.h/cpp
*	2. Create a socket			- socket().         Done in SocketType.h/cpp
*	3. Connect to the server	- connect().
*	4. Send and receive data	- recv(), send(), recvfrom(), sendto().
*	5. Disconnect				- closesocket().    Done in SocketType.h/cpp
*	
*	IPV4 uses - sockaddr_in, in_addr
*	IPV6 uses - addrinfo, sockaddr
* 
*/

#include "SocketType/SocketType.h"
#include "Client/ServerListener.h"


class Client
{
public:
	Client(
		const std::string& server_address, 
		const int server_port
	);
	~Client();

public:
	// Regular methods
	void Connect();
	socket_t GetSocket();

private:
	bool Authenticate();
	std::string* CalculateHash(const std::string& hash_input);
	void Close();
	void Shutdown();

private:

	std::string m_server_address;
	int m_server_port;
	socket_t m_socket;
	
	const char* m_authkey = "SecretKey123";
	const char* m_salt = "RandomSalt";

	std::ostream* m_client_writer;
	std::istream* m_server_reader;

	std::thread m_sl_thread;
	ServerListener m_sl;
};