/**
 * @file DatabaseService.hpp
 * @author Axel Fortun
 * @brief Manage a SQLite3 database to identify all processes running with network information (IPAddress:PORT).
 * @details Connect to a local database in order to identify all processes launched. \n
 * You can also define another database by using a path and a db name. \n
 * This new database will be your "Domain of communication".
 */

#pragma once

// System includes
#include <string>

struct sqlite3;


/** @brief Manage a SQLite3 database to identify all service running with network information (ZMQ format). */
class DatabaseService
{
public:
    /** @brief The SQLITE3 database default name */
    static const char* DATABASE_DEFAULT_NAME;

    /** @brief The maximum size of a userName */
    static const size_t MAXIMUM_USER_NAME_SIZE;

    /**
     * @brief Open (and create if not exist) the sqlite3 database
     * @param databaseLocation The database to use to locate the different services started. Default DB if empty. 
     * @throw Throw a string if the database cannot be opened
     */
    DatabaseService(const std::string &databaseLocation = DATABASE_DEFAULT_NAME);

    /** @brief Close the SQLITE3 database */
    ~DatabaseService();

    /**
     * @brief Add a service to the SQLITE3 Database. userName is a unique ID.
     * @param userName The uniqueID of the network.
     * @param networkInfo The network information where the service is located.
     * @details Add a service to the SQLITE3 Database. userName is a unique ID. \n
     * If the user name already exists, the networkInfo will be replaced with \n
     * the one specified in networkInfo.
     */
    void addOrReplaceService(const std::string &userName, const std::string &networkInfo);


    /**
     * @brief Remove a service from the SQLITE3 Database by using the userName
     * @param userName The uniqueID of the network.
     */
    void removeService(const std::string &userName);

    /**
     * @brief Get the network information by using a user name.
     * @param userName The uniqueID of the network.
     * @return The network informations linked to the userName. If the service doesn't exist, empty string.
     */
    const std::string getNetworkInfoFromuserName(const std::string &userName) const;

    /** @brief Cleanup the database table */
    void cleanupTable();
    
private:
    void executeSQLITECommand(const char* commandToLaunch);
    std::string eraseForbidenCharacters(std::string stringToModify) const;
    mutable sqlite3 *database;
    const std::string databaseLocation;
};
