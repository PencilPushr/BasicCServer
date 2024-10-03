#pragma once

#include <thread>

#ifdef _WIN32
#include <WinSock2.h>
#endif

/*
*			Winsock Server side normally as follows:
*	1. Initialize WSA				    - WSAStartup().
*	2. Create a socket				    - socket().
*	3. Bind the socket to an address	- bind().
*	4. Listen for incoming connections	- listen().
*	5. Accept a connection			    - accept().
*	6. Send and receive data		    - recv(), send(), recvfrom(), sendto().
*	7. Disconnect					    - closesocket().
*
*	IPv4 uses - sockaddr_in, in_addr
*	IPv6 uses - addrinfo, sockaddr
*
*/

class Server
{
public:
	Server();
	~Server();

public:
	// Regular methods

	void Start();

private:
	bool Authenticate();
	std::string* CalculateHash();
	void Close();

private:

#ifdef _WIN32
	WORD m_wVersionRequested;
	WSADATA m_wsaData;
	int m_wsaerr;
	SOCKET m_socket;
#endif

	std::string m_server_address;
	int m_server_port;

	const char * m_authkey = "SecretKey123";
	const char * m_salt = "RandomSalt";

};