/**
 * @file MessageSender.hpp
 * @author Axel Fortun
 * @brief The sender of ZMQ messages. Connect to another socket.
 * @details 
 */

#pragma once

// ZMQ includes
#include <zmq.hpp>
// System includes
#include <vector>
#include <string>
#include <memory>

/** @brief The sender of ZMQ messages. Connect to another socket. **/
class MessageSender
{
public:
    /**
     * @brief Contructor, connect a ZMQ socket as PUSH
     * @param networkInformation The information used to bind the ZeroMQ socket.
     */
    MessageSender(const std::string &networkInformation, zmq::context_t &zmqContext);

    /** @brief Destructor, close the ZeroMQ socket */
    ~MessageSender();

    /**
     * @brief Send a string message to ZMQ.
     * @param messageToSend The message to send as String.
     * @return True if the message had been sent. Else, false
     */
    bool sendMessage(const std::string &messageToSend);
private:
    std::unique_ptr<zmq::socket_t> senderSocket;
};