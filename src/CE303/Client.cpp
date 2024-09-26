#include "Client.h"

#include <thread>


Client::Client(const std::string& server_address, const int server_port, std::ostream * client_writer, 
    std::istream * server_reader) : 
    m_server_address{ server_address }, m_server_port{ server_port }, m_socket{ INVALID_SOCKET },
    m_client_writer{ client_writer }, m_server_reader{ server_reader }
{

}

Client::~Client()
{
    Client::Close();
}

void Client::Connect()
{
    InitSockets();
    CreateSocket();


    // Get server info 
    // alt:
    /*
    HOSTENT* host = gethostbyname(szHost);
    if (host == nullptr)
    {
        printf("Client: Failed to get host info");
    }
    */
    SOCKADDR_IN client_service;
    client_service.sin_family = AF_INET;                           // AF_INET is the Internet address family.
    client_service.sin_addr.s_addr = inet_addr(m_server_address.c_str());  // bind IP address to socket
    client_service.sin_port = htons(m_server_port);                // bind port number to socket
    /*
    memcpy(&sin.sin_addr.S_un.S_addr, host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr))
    */

    // 3. Connect to server
    if (connect(m_socket, (SOCKADDR*)&client_service, sizeof(client_service)) == SOCKET_ERROR)
    {
        printf("Client: connect() - Failed to connect %ld\n", WSAGetLastError());
        WSACleanup();
        Client::Close();
    }

    //if (!Authenticate()) 
    //{
    //    printf("Authentication failed. Exiting.");
    //    Close();
    //    return;
    //}

    // Spawn new thread that listens to the server to prevent blocking when reading and writing
    m_sl = ServerListener(this, 4096);
    m_sl_thread = std::thread(ServerListener::SelfCall, &m_sl);
    m_sl_thread.detach();

    // 4. Send and recieve data
    std::string input;
    while (std::getline(std::cin, input)) 
    {
        send(m_socket, input.c_str(), input.length(), 0);
        if (input == "exit") 
        {
            break;
        }
    }

    Close();
}

socket_t Client::GetSocket()
{
    return &m_socket;
}

void Client::InitSockets()
{
#ifdef _WIN32
    // 1. Init winsock
    m_wVersionRequested = MAKEWORD(2, 2);
    m_wsaerr = WSAStartup(m_wVersionRequested, &m_wsaData);
    if (m_wsaerr != 0)
    {
        /* Tell the user that we could not find a usable */
        /* WinSock DLL.*/
        printf("The Winsock dll not found!\n");
        Client::Close();
    }
    else
    {
        printf("The Winsock dll found!\n");
        printf("The status: %s.\n", m_wsaData.szSystemStatus);
    }
#endif
}

void Client::CreateSocket()
{
#ifdef _WIN32
    // 2. Create a socket and validate it
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
        printf("Client: socket() - Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        Client::Close();
    }
#endif

}

bool Client::Authenticate()
{
    // Receive the nonce challenge from the server
    std::string nonce = m_server_reader.getline();
    
    // Calculate the hash with the shared secret and salt
    const char * secret_with_salt = m_authkey + m_salt;
    const char * hash_input = secret_with_salt + nonce;
    std::string* hashedResponse = CalculateHash(hash_input);
    
    // Send the hashed response back to the server
    clientWriter.println(hashedResponse);
    
    // The server will verify the response
    
    return true; // Authentication successful
    
    return 0;

}

std::string* Client::CalculateHash(const std::string& hash_input)
{
    //MessageDigest digest = MessageDigest.getInstance("SHA-256"); // Set the encrypt algo.
    //byte [] hash = digest.digest(hash_input.getBytes()); // Generating the message digest as an array of bytes
    //return Base64.getEncoder().encodeToString(hash);
    return new std::string("my code");
}

void Client::Close()
{

    if (m_client_writer != NULL) {
        m_client_writer->write("exit", 4);
    }
    if (m_socket != INVALID_SOCKET) {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    if (m_sl_thread.joinable()) {
        m_sl_thread.join();
    }

#ifdef _WIN32
    CLOSE_SOCKET();
    WSACleanup();
#endif

    // Inform the user that we have closed.
    printf("Client closed.");
}

void Client::Shutdown()
{
    //std::atexit();
}
