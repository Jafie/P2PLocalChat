/**
 * @file CommunicationService.cpp
 * @author Axel Fortun
 * @brief The facade used to communicate to other processes - P2P mode (ZeroMQ)
 */

#include <CommunicationService.hpp>

// Local includes
#include "MessageReceiver.hpp"
#include "MessageSender.hpp"
// ZMQ includes
#include <zmq.hpp>

using namespace std;

CommunicationService::CommunicationService() :
    zmqContext(new zmq::context_t)
{
}

CommunicationService::~CommunicationService()
{
    // Close at first the "Receiver"
    theMessageReceiver.reset();
    // Delete the "Senders"
    mapOfMessageSenders.clear();
}

void CommunicationService::connectReceiver(const string &networkInfo)
{
    theMessageReceiver.reset(new MessageReceiver(networkInfo, *zmqContext));
}

void CommunicationService::addOrReplaceMessageSender(const string &userName, const string &networkInfo)
{
    mapOfMessageSenders[userName] = unique_ptr<MessageSender>(new MessageSender(networkInfo, *zmqContext));
}

bool CommunicationService::sendMessage(const string &userName, const string &messageToSend) const
{
    auto it = mapOfMessageSenders.find(userName);

    if (it != mapOfMessageSenders.end())
    {
        return it->second->sendMessage(messageToSend);
    }

    return false;
}

vector<string> CommunicationService::collectReceivedMessages() const
{
    return theMessageReceiver->collectMessage();
}


bool CommunicationService::isServiceKnown(const std::string &userName) const
{
    auto it = mapOfMessageSenders.find(userName);

    return (it != mapOfMessageSenders.end());
}
