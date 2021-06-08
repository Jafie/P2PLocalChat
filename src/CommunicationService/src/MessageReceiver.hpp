/**
 * @file MessageReceiver.hpp
 * @author Axel Fortun
 * @brief The receiver of ZMQ messages.
 * @details 
 */

#pragma once

// ZMQ includes
#include <zmq.hpp>
// System includes
#include <vector>
#include <string>
#include <memory>

/** @brief The receiver of ZMQ messages. **/
class MessageReceiver
{
public:
    /**
     * @brief Contructor, launch a bind to networkInformation as PULL.
     * @param networkInformation The information used to bind the ZeroMQ socket.
     */
    MessageReceiver(const std::string &networkInformation, zmq::context_t &zmqContext);

    /** @brief Destructor, close the ZeroMQ socket */
    ~MessageReceiver();

    /**
     * @brief Collect all the messages received by the socket
     * @return A vector of message received
     */
    std::vector<std::string> collectMessage() const;
private:
    std::unique_ptr<zmq::socket_t> receiverSocket;
};