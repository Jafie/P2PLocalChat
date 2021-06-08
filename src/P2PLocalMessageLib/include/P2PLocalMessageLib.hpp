/**
 * @file P2PLocalMessageLib.hpp
 * @author Axel Fortun
 * @brief The P2PLocalMessageLib application. Based on DatabaseService and CommunicationService
 * @details The P2PLocalMessageLib provides all the nescessaries to use the P2PMESSAGE system. \n
 * In the Constructor, you can define a sqlite3 database to define your own domain of communication \n
 * Only one initialization can be applied. A succeed initialization will complete the database to \n
 * register your network informations. \n
 * Then, you can simply "Send a message" or "Collect the message received".
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>

class CommunicationService;
class DatabaseService;

/** @brief The P2PLocalMessageLib application. Based on DatabaseService and CommunicationService */
class P2PLocalMessage
{
public:
    /**
     * @brief Constructor, connect to the database. The database correspond to the domain of communication
     * @param databaseName The database to connect. If empty, connect to a default database.
     */
    P2PLocalMessage(const std::string &databaseName = "");

    /** @brief Destructor, close database and handle */
    ~P2PLocalMessage();

    /**
     * @brief Add or replace a message sender.
     * @param userName The name used to identify the receiver.
     * @param networkInfo The network location of the receiver.
     * @throw Throw a zmq::error_t in case of failure during connection. Throw a string if the database failed
     */
    void initialize(const std::string &userName, const std::string &networkInfo);

    /**
     * @brief Collect all the message received by the CommunicationService.
     * @return A vector of message received.
     * @details After this call, the ZMQ receiver queue is cleaned.
     */
    std::vector<std::string> collectReceivedMessages() const;
    
    /**
     * @brief Send a message to a new known message sender.
     * @param userName The name used to identify the receiver.
     * @param networkInfo The network location of the receiver.
     * @return True if the message had been sent. Else, false.
     * @throw Throw a string in case of database failure.
     */
    bool sendMessageToService(const std::string &userName, const std::string &messageToSend);

private:
    std::unique_ptr<CommunicationService> theCommunicationService;
    std::unique_ptr<DatabaseService> theDatabaseService;
    std::atomic_bool isInitialized;
    std::string userName;
    std::string addUserNameHeaderToMessage(const std::string &messageToSend) const;
};
