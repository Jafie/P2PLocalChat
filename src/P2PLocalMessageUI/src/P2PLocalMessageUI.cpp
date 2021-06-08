/**
 * @file P2PLocalMessageUI.cpp
 * @author Axel Fortun
 * @brief User interface (terminal) based on the P2PLocalMessage library.
 * @details 
 */

#include "P2PLocalMessageUI.hpp"

// Local includes
#include <P2PLocalMessageLib.hpp>
// System includes
#include <iostream>

using namespace std;

#define P2PLOCALMESSAGEUI_SEPARATORS " ================== "
#define RED_INFO "\x1B[31m"
#define GREEN_INFO "\x1B[32m"
#define DEFAULT_INFO "\x1B[0m"

P2PLocalMessageUI::P2PLocalMessageUI(const std::string &databaseLocation)
    : databaseLocation(databaseLocation)
{
}

P2PLocalMessageUI::~P2PLocalMessageUI()
{
}

void P2PLocalMessageUI::runP2PLocalMessageUI()
{
    // Header
    cout << endl <<P2PLOCALMESSAGEUI_SEPARATORS << endl << " Welcome to P2PLocalMessage ";
    cout << endl << P2PLOCALMESSAGEUI_SEPARATORS << endl;

    firstConfiguration();
    selectActionLoop();

    // Foot
    cout << endl << P2PLOCALMESSAGEUI_SEPARATORS << endl << " Exiting P2PLocalMessage ";
    cout << endl << P2PLOCALMESSAGEUI_SEPARATORS << endl;
}

void P2PLocalMessageUI::firstConfiguration()
{
    std::string userName;
    std::string networkInformations;
    std::string zmqNetworkInfo = "tcp://";
    cout << "Please, select a User Name" << endl;
    getline(cin, userName);
    cout << "Welcome " << userName << ", please select your IP and PORT" <<  endl;
    cout << "User the following format: IP:port (TCP will be used)" << endl;
    cout << "Example: 127.0.0.1:25475" << endl;
    cout << "Example: 192.168.0.5:26874" << endl;
    getline(cin, networkInformations);

    zmqNetworkInfo += networkInformations;
    cout << "Connecting to " << zmqNetworkInfo << endl;

    theP2PLocalMessageSystem.reset(new P2PLocalMessage(databaseLocation));
    theP2PLocalMessageSystem->initialize(userName, zmqNetworkInfo);
    cout << "You are now registered as " << userName << ", enjoy!" << endl;
}

void P2PLocalMessageUI::selectActionLoop()
{
    bool exitRequested = false;
    do
    {
        switch(selectSingleAction())
        {
            case P2PLocalMessageUI::SEND_MESSAGE:
                sendMessageAction();
                break;

            case P2PLocalMessageIUIActions::COLLECT_MESSAGE:
                collectMessageAction();
                break;

            case P2PLocalMessageIUIActions::EXIT_REQUESTED:
                exitRequested = true;
                break;

            default:
                cout << "Invalid action" << endl;
                break;
        }
    }while (exitRequested == false);
}

P2PLocalMessageUI::P2PLocalMessageIUIActions P2PLocalMessageUI::selectSingleAction()
{
    std::string actionSelected = "";
    cout << "Please, select your action:" << endl;
    cout << "1 - Send a message" << endl;
    cout << "2 - Read all received messages" << endl;
    cout << "3 - Quit application" << endl;
    getline(cin, actionSelected);

    if (actionSelected == "1")
    {
        return P2PLocalMessageUI::SEND_MESSAGE;
    }
    else if (actionSelected == "2")
    {
        return P2PLocalMessageUI::COLLECT_MESSAGE;
    }
    else if (actionSelected == "3")
    {
        cout << "Exiting application..." << endl;
        return P2PLocalMessageUI::EXIT_REQUESTED;
    }

    return P2PLocalMessageUI::INVALID_ENTRY;
}

void P2PLocalMessageUI::sendMessageAction()
{
    std::string userNameTarget;
    std::string messageToSend;
    cout << "Who will receive the message?" << endl;
    getline(cin, userNameTarget);
    cout << "Please, type your message for " << userNameTarget << endl;
    getline(cin, messageToSend);

    if (theP2PLocalMessageSystem->sendMessageToService(userNameTarget, messageToSend))
    {
        cout << endl << GREEN_INFO << "Message sent !" << DEFAULT_INFO << endl << endl;
    }
    else
    {
        cout << endl << RED_INFO << "Message sending failed, maybe " << userNameTarget << " is not connected" << DEFAULT_INFO << endl << endl;
    }
}

void P2PLocalMessageUI::collectMessageAction()
{
    cout << "Collecting messages..." << endl;
    auto listOfMessageReceived = theP2PLocalMessageSystem->collectReceivedMessages();

    cout << "You received " << listOfMessageReceived.size() << " messages" << endl;

    for (auto messageReceived : listOfMessageReceived)
    {
        cout << messageReceived << endl;
    }
}

#undef P2PLOCALMESSAGEUI_SEPARATORS
