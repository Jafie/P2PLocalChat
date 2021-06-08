/**
 * @file main.cxx
 * @author Axel Fortun
 * @brief Launch the P2PLocalMessage application
 * @details 
 */

// Local includes
#include "P2PLocalMessageUI.hpp"
// System includes
#include <iostream>

int main(int argc, char **argv)
{
    std::string databaseToUse = "";
    if (argc > 1)
    {
        databaseToUse = argv[1];
        std::cout << "Using database: " << databaseToUse << std::endl;
    }

    P2PLocalMessageUI myLocalMessageUI(databaseToUse);
    myLocalMessageUI.runP2PLocalMessageUI();

    return 0;
}