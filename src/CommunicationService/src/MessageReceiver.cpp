/**
 * @file MessageReceiver.cpp
 * @author Axel Fortun
 * @brief The receiver of ZMQ messages.
 * @details 
 */

#include "MessageReceiver.hpp"
#include <iostream>

#define MESSAGE_RECEIVER_HEADER "[RECEIVER SOCKET]"

using namespace std;

MessageReceiver::MessageReceiver(const std::string &networkInformation, zmq::context_t &zmqContext)
{
    cout << MESSAGE_RECEIVER_HEADER" Binding " << networkInformation << " ..." << endl;
    receiverSocket.reset(new zmq::socket_t(zmqContext, zmq::socket_type::pull));
    receiverSocket->bind(networkInformation);

    // Set linger time to 0 (NO WAIT).
    receiverSocket->set(zmq::sockopt::linger, 0);
    cout << MESSAGE_RECEIVER_HEADER" Bind SUCCEED! I can now receive messages!" << endl;
}

MessageReceiver::~MessageReceiver()
{
    cout << MESSAGE_RECEIVER_HEADER" Receiver socket is now closed" << endl;
}

std::vector<std::string> MessageReceiver::collectMessage() const
{
    zmq::message_t messageReceived;
    vector<string> listOfMessageReceived;

    // Retrieve all messages in ZMQ queue. Then, place it in "listOfMessageReceived"
    while (receiverSocket->recv(messageReceived, zmq::recv_flags::dontwait).has_value())
    {
        listOfMessageReceived.push_back(messageReceived.to_string());
    }

    return listOfMessageReceived;
}

#undef MESSAGE_RECEIVER_HEADER
