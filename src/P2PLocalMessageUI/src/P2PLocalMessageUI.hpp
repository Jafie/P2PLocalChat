/**
 * @file P2PLocalMessageUI.hpp
 * @author Axel Fortun
 * @brief User interface (terminal) based on the P2PLocalMessage library.
 * @details 
 */

#include <string>
#include <memory>

class P2PLocalMessage;

class P2PLocalMessageUI
{
public:
    /**
     * @brief Constructor, do nothing
     * @param databaseLocation The location of your database. If nothing, use default database.
     */
    P2PLocalMessageUI(const std::string &databaseLocation = "");

    /** @brief Destructor, call delete */
    ~P2PLocalMessageUI();

    /** @brief Blocking method, run the UI */
    void runP2PLocalMessageUI();

private:
    enum P2PLocalMessageIUIActions
    {
        EXIT_REQUESTED = 0,
        SEND_MESSAGE,
        COLLECT_MESSAGE,
        INVALID_ENTRY
    };

    std::unique_ptr<P2PLocalMessage> theP2PLocalMessageSystem;
    std::string databaseLocation;

    void firstConfiguration();
    void selectActionLoop();
    void sendMessageAction();
    void collectMessageAction();

    P2PLocalMessageIUIActions selectSingleAction();
};