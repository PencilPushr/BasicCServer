#pragma once

#include <iostream>
#include <thread>
#include <string>

/*
*			Winsock Client side normally as follows:
*	1. Initialize WSA			– WSAStartup().
*	2. Create a socket			– socket().
*	3. Connect to the server	– connect().
*	4. Send and receive data	– recv(), send(), recvfrom(), sendto().
*	5. Disconnect				– closesocket().
*	
*	IPV4 uses - sockaddr_in, in_addr
*	IPV6 uses - addrinfo, sockaddr
* 
*/

#include "DataStructures/SocketType/SocketType.h"
#include "Client/ServerListener.h"


class Client
{
public:
	Client(
		const std::string& server_address, 
		const int server_port, 
		std::ostream* client_writer,
		std::istream* server_reader
	);
	~Client();

public:
	// Regular methods
	void Connect();
	socket_t GetSocket();

private:
	void InitSockets();
	void CreateSocket();
	bool Authenticate();
	std::string* CalculateHash(const std::string& hash_input);
	void Close();
	void Shutdown();

private:

#ifdef _WIN32
	WORD m_wVersionRequested;
	WSADATA m_wsaData;
	int m_wsaerr;
#endif

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