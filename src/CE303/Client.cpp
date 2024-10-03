#include "Client.h"

#include <thread>
#include <string>
#include <cstring>

Client::Client(
        const std::string& server_address,
        const int server_port
)
        : m_server_address{ server_address }
        , m_server_port{ server_port }
        , m_socket{ INVALID_SOCKET }
{}

Client::~Client()
{
    Client::Close();
}

void Client::Connect()
{

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

bool Client::Authenticate()
{
    // Receive the nonce challenge from the server
    std::string nonce = m_server_reader.getline();
    
    // Calculate the hash with the shared secret and salt
    const char * secret_with_salt = m_authkey + m_salt;
    const char * hash_input = secret_with_salt + nonce;
    std::string* hashedResponse = CalculateHash(hash_input);
    
    // Send the hashed response back to the server
    m_client_writer.println(hashedResponse);
    
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
