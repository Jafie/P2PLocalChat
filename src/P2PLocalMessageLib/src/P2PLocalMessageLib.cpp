/**
 * @file P2PLocalMessageLib.cpp
 * @author Axel Fortun
 * @brief The P2PLocalMessageLib application. Based on DatabaseService and CommunicationService
 * @details 
 */

#include <P2PLocalMessageLib.hpp>

// Local Includes
#include <CommunicationService.hpp>
#include <DatabaseService.hpp>
// ZMQ Includes
#include <zmq.hpp>
// System Includes
#include <iostream>
#include <sstream>

P2PLocalMessage::P2PLocalMessage(const std::string &databaseName) :
    theCommunicationService(new CommunicationService()),
    isInitialized(false)
{
    if (databaseName.empty())
    {
        theDatabaseService.reset(new DatabaseService());
    }
    else
    {
        theDatabaseService.reset(new DatabaseService(databaseName));
    }
}

P2PLocalMessage::~P2PLocalMessage()
{
    theDatabaseService->removeService(userName);
}

void P2PLocalMessage::initialize(const std::string &userName, const std::string &networkInfo)
{
    if (isInitialized == true)
    {
        throw std::string("Service is already initialized");
    }

    try
    {
        theCommunicationService->connectReceiver(networkInfo);
        theDatabaseService->addOrReplaceService(userName, networkInfo);
    }
    catch (const zmq::error_t& communicationException)
    {
        std::cout << "Cannot initialize the P2PLocalMessage (Comm error: " << communicationException.what() << ")" << std::endl;
        throw;
    }
    catch (const std::string& databaseException)
    {
        std::cout << "Cannot initialize the P2PLocalMessage (DB error: " << databaseException << ")" << std::endl;
        // Close for a new communication service
        theCommunicationService.reset(new CommunicationService());
        throw;
    }

    this->userName = userName;
    isInitialized = true;
}

std::vector<std::string> P2PLocalMessage::collectReceivedMessages() const
{
    if (isInitialized == false)
    {
        return std::vector<std::string>();
    }

    return theCommunicationService->collectReceivedMessages();
}

bool P2PLocalMessage::sendMessageToService(const std::string &userNameTarget, const std::string &messageToSend)
{
    if (isInitialized == false)
    {
        return false;
    }

    std::string messageToSendWithHeader = addUserNameHeaderToMessage(messageToSend);
    // Send directly the message if the user is stored in the Communication Service.
    if (theCommunicationService->isServiceKnown(userNameTarget))
    {
        return theCommunicationService->sendMessage(userNameTarget, messageToSendWithHeader);
    }

    // Else, check the database and create a new communication service.
    std::string networkInfo = theDatabaseService->getNetworkInfoFromuserName(userNameTarget);
    if (networkInfo == "")
    {
        std::cout << "The service " << userNameTarget << " is unknown, the message is aborted" << std::endl;
        return false;
    }

    try
    {
        theCommunicationService->addOrReplaceMessageSender(userNameTarget, networkInfo);
    }
    catch(const zmq::error_t& communicationException)
    {
        std::cout << "Unable to connect to " << userNameTarget << "(localisation: " << networkInfo << std::endl;
        return false;
    }

    return theCommunicationService->sendMessage(userNameTarget, messageToSendWithHeader);
}

std::string P2PLocalMessage::addUserNameHeaderToMessage(const std::string &messageToSend) const
{
    std::stringstream messageWithHeader;
    std::string messageWithHeaderStr;
    messageWithHeader << "[" << userName << "]" << " " << messageToSend;
    messageWithHeaderStr = messageWithHeader.str();

    return messageWithHeaderStr;
}
