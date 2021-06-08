/**
 * @file CommunicationService.hpp
 * @author Axel Fortun
 * @brief The facade used to communicate to other processes - P2P mode (ZeroMQ)
 * @details The communication service links the MessageSender and MessageReceiver. \n
 * It uses a single MessageReceiver and multiple MessageSender. \n
 * There are one MessageSender per UserName registered in a map. \n
 * Moreover, the constructor will create a ZeroMQ context.
 */

#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>

class MessageSender;
class MessageReceiver;

namespace zmq
{
    class context_t;
}

/**
 * @brief The facade used to communicate to other processes. Create a ZMQ context.
 */
class CommunicationService
{
public:

    /** @brief Constructor, create a ZMQ context */
    CommunicationService();

    /** @brief Destructor, do nothing */
    ~CommunicationService();

    /**
     * @brief Bind a connection on the networkInfo.
     * @param networkInfo The information of your network (format: [type]://[IP]:[port]) Example: tcp://localhost:54132
     * @throw Throw a zmq::error_t in case of failure during binding.
     */
    void connectReceiver(const std::string &networkInfo);

    /**
     * @brief Add or replace a message sender.
     * @param userName The name used to identify the receiver targeted.
     * @param networkInfo The network location of the receiver targeted.
     * @throw Throw a zmq::error_t in case of failure during connection.
     */
    void addOrReplaceMessageSender(const std::string &userName, const std::string &networkInfo);

    /**
     * @brief Send a message to a known message sender.
     * @param userName The name used to identify the receiver targeted.
     * @param networkInfo The network location of the receiver targeted.
     * @return True if the message had been sent. Else, false.
     */
    bool sendMessage(const std::string &userName, const std::string &messageToSend) const;

    /**
     * @brief Collect all  messages received by the CommunicationService
     * @return A vector of messages received
     */
    std::vector<std::string> collectReceivedMessages() const;

    /**
     * @brief Collect all the message received by the CommunicationService.
     * @param userName The user name to find.
     * @return A vector of message received.
     */
    bool isServiceKnown(const std::string &userName) const;

private:
    typedef std::map<const std::string, std::unique_ptr<MessageSender>> userNamePerSenderMap;
    userNamePerSenderMap mapOfMessageSenders;
    std::unique_ptr<MessageReceiver> theMessageReceiver;
    std::unique_ptr<zmq::context_t> zmqContext;
};