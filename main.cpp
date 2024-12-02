#ifdef _WIN32 // Windows only undefined selection
#define _WIN32_WINNT 0x0A00
#endif 

#include <iostream>
#include "LOGGER.h"
#include "Buffer_Cleaner.h"
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/thread.hpp>
#include <thread>

using asioContext = boost::asio::io_context;
using asioError = boost::system::error_code;
using tcpEndPoint = boost::asio::ip::tcp::endpoint;
using tcpSocket = boost::asio::ip::tcp::socket;

struct noPrintable {
    bool alfa { false };
    
};

int main() {

    // LOGGER object to log to console
    LOGGER console;
    // Buffer Cleaner class to make the buffer printable
    Buffer_Cleaner buffCleaner; 
    // Error handler class for Asio
    asioError ec;
    // Initiation object
    asioContext context;
    // Add address of somewhere we wish to connect to port 3000
    tcpEndPoint endPoint(boost::asio::ip::make_address("EndPoint string value", ec), "Port Number int value");
    // Create a socket with the "context" object
    tcpSocket socket(context);
    // Tell the socket to connect to the end point
    socket.connect(endPoint, ec);

    {
        std::jthread t([&]() {
            if (!ec) { 
                char buffer[1024];
                size_t byteRead = socket.read_some(boost::asio::buffer(buffer), ec);
                console.log("Connection is successfully made"); 
                std::string result = buffCleaner(buffer); 
                console.log(result);
            }
            else { 
                console.log("Error! Connection failed", ec.message()); 
            }
            });
        t.join();
    }

    // console.log tests
    noPrintable noPri;

    int tester{ 6 };
    float num{ 3.1f };
    std::string text = "text";
    bool isBool { true };

    console.log(2, "This is typed in", tester, num, text, isBool, noPri);
   

    
    return 0;
}
