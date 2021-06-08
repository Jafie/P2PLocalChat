/**
 * @file DatabaseService.cpp
 * @author Axel Fortun
 * @brief Manage a SQLite3 database to identify all processes running with network information (IPAddress:PORT).
 */


#include <DatabaseService.hpp>

// Third-Party includes
#include <sqlite3.h>
// System includes
#include <algorithm>
#include <iostream>
#include <unistd.h>

using namespace std;

/** @brief The command to create the SQLITE3 table */
static const char* CREATE_TABLE_COMMAND = "CREATE TABLE IF NOT EXISTS DatabaseService (UserName TEXT PRIMARY KEY, NetworkInfo TEXT);";

/** @brief The command to add a DatabaseService element */
static const char* ADD_DATABASE_SERVICE_COMMAND = "REPLACE INTO DatabaseService VALUES (\"%s\",\"%s\");";

/** @brief The command to remove a DatabaseService element */
static const char* REMOVE_DATABASE_SERVICE_COMMAND = "DELETE FROM DatabaseService WHERE UserName = \"%s\";";

/** @brief The command to remove a DatabaseService element */
static const char* GET_DATABASE_SERVICE_COMMAND = "SELECT NetworkInfo from DatabaseService WHERE UserName = \"%s\";";

/** @brief The delete table command */
static const char* DELETE_TABLE_COMMAND = "DROP TABLE DatabaseService";

/** @brief Maximum size of a SQLITE command in characters */
static const size_t MAXIMUM_SQLITE_COMMAND_LEN = 512;

/** @brief SQLITE timeout value in ms */
static const int SQLITE_TIMEOUT_IN_MS = 5000;

const char* DatabaseService::DATABASE_DEFAULT_NAME = "P2PMessageDB.sqlite3";
const size_t DatabaseService::MAXIMUM_USER_NAME_SIZE = 64;

DatabaseService::DatabaseService(const std::string &databaseLocation) :
    databaseLocation(databaseLocation)
{
    int SQliteRetunValue = sqlite3_open(databaseLocation.c_str(), &database);

    if (SQliteRetunValue != SQLITE_OK)
    {
        cout << "Unable to open the SQLITE3 database: " << databaseLocation << " (Code: " <<  sqlite3_errmsg(database) << ")" << endl;
        sqlite3_close(database);
        throw string("Unable to open the database");
    }

    SQliteRetunValue = sqlite3_busy_timeout(database, SQLITE_TIMEOUT_IN_MS);
    if (SQliteRetunValue != SQLITE_OK)
    {
        cout << "Unable to set timeout the SQLITE3 database: " << databaseLocation << " (Code: " <<  sqlite3_errmsg(database) << ")" << endl;
        sqlite3_close(database);
        throw string("Unable to set the database timeout");
    }

    executeSQLITECommand(CREATE_TABLE_COMMAND);
    cout << "DatabaseService ready to be used!" << endl;
}

DatabaseService::~DatabaseService()
{
    sqlite3_close(database);
}

void DatabaseService::executeSQLITECommand(const char* commandToLaunch)
{
    char* errorMessage = NULL;
    int SQliteRetunValue = sqlite3_exec(database, commandToLaunch, NULL, NULL, &errorMessage);

    if (SQliteRetunValue != SQLITE_OK)
    {
        cout << "SQLITE Command refused: " <<  commandToLaunch << "(code = " << errorMessage << ")" << endl;
        throw string("Unable to insert an entry");
    }
}

void DatabaseService::addOrReplaceService(const std::string &userName, const std::string &networkInfo)
{
    char sqlCommandToInject[MAXIMUM_SQLITE_COMMAND_LEN];

    // Protect against injection
    std::string userNameProtected = eraseForbidenCharacters(userName);
    std::string networkInfoProtected = eraseForbidenCharacters(networkInfo);

    snprintf(sqlCommandToInject, MAXIMUM_SQLITE_COMMAND_LEN, ADD_DATABASE_SERVICE_COMMAND, userNameProtected.c_str(), networkInfoProtected.c_str());

    executeSQLITECommand(sqlCommandToInject);
}

std::string DatabaseService::eraseForbidenCharacters(std::string stringToModify) const
{
    stringToModify.erase(std::remove_if(
        stringToModify.begin(),
        stringToModify.end(),
    [](char c) { return (std::isspace(c) || (c == ';')); } ),
    stringToModify.end());

    stringToModify.resize(MAXIMUM_USER_NAME_SIZE);
    return stringToModify;
}

void DatabaseService::removeService(const std::string &userName)
{
    char sqlCommandToInject[MAXIMUM_SQLITE_COMMAND_LEN];

    // Protect against injection
    std::string userNameProtected = eraseForbidenCharacters(userName);

    snprintf(sqlCommandToInject, MAXIMUM_SQLITE_COMMAND_LEN, REMOVE_DATABASE_SERVICE_COMMAND, userNameProtected.c_str());
    executeSQLITECommand(sqlCommandToInject);
}

const std::string DatabaseService::getNetworkInfoFromuserName(const std::string &userName) const
{
    char sqlCommandToInject[MAXIMUM_SQLITE_COMMAND_LEN];
    sqlite3_stmt *result = NULL;

    // Protect against injection
    std::string userNameProtected = eraseForbidenCharacters(userName);

    snprintf(sqlCommandToInject, MAXIMUM_SQLITE_COMMAND_LEN, GET_DATABASE_SERVICE_COMMAND, userNameProtected.c_str());

    int SQliteRetunValue = sqlite3_prepare_v2(database, sqlCommandToInject, -1, &result, 0);
    if (SQliteRetunValue != SQLITE_OK)
    {
        cout << "SQLITE Command refused during prepare: " <<  sqlCommandToInject << endl;
        return "";
    }

    std::string stringToReturn = "";
    SQliteRetunValue = sqlite3_step(result);

    if (SQliteRetunValue == SQLITE_ROW)
    {
        stringToReturn = reinterpret_cast<const char*>(sqlite3_column_text(result, 0));
    }
    
    SQliteRetunValue = sqlite3_finalize(result);
    if (SQliteRetunValue != SQLITE_OK)
    {
        cout << "SQLITE refused finalize instruction: " <<  sqlCommandToInject << endl;
    }

    return stringToReturn;
}

void DatabaseService::cleanupTable()
{
    executeSQLITECommand(DELETE_TABLE_COMMAND);
    executeSQLITECommand(CREATE_TABLE_COMMAND);
}
