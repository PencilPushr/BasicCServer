#pragma once

#include <thread>

#ifdef _WIN32
#include <WinSock2.h>
#endif

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