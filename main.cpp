#include <iostream>
#include "CE303/Client.h"


int main(int argc, char** argv)
{
    if (argc < 2)
    {
        //std::cerr << "Usage: " << argv[0] << " decode <encoded_value>" << std::endl;
        return 1;
    }

    Client client("localhost", 8080);
}