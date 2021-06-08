/**
 * @file MessageSender.cpp
 * @author Axel Fortun
 * @brief The sender of ZMQ messages. Connect to another socket.
 * @details 
 */

#include "MessageSender.hpp"

#include <iostream>

using namespace std;

MessageSender::MessageSender(const std::string &networkInformation, zmq::context_t &zmqContext)
{
    senderSocket.reset(new zmq::socket_t(zmqContext, zmq::socket_type::push));
    senderSocket->connect(networkInformation);

    // Set linger time to 0 (NO WAIT).
    senderSocket->set(zmq::sockopt::linger, 0);
}

MessageSender::~MessageSender()
{
}

bool MessageSender::sendMessage(const std::string &messageToSend)
{
    zmq::const_buffer bufferToTransmit = zmq::buffer(messageToSend);
    // Send the message to the user through ZMQ socket 
    zmq::send_result_t numberOfBytesSent = senderSocket->send(bufferToTransmit, zmq::send_flags::none);

    if (numberOfBytesSent.has_value() == false)
    {
        cout << "Message send failed..." << endl;
        return false;
    }

    return true;
}
