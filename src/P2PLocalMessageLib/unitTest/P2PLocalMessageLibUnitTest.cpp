/**
 * @file P2PLocalMessageLibUnitTest.cpp
 * @author Axel Fortun
 * @brief The Unitary Test to validate Communication Service
 * @details Test the P2PLocalMessageLib.
 */


// CommunicationService includes
#include <P2PLocalMessageLib.hpp>
// Third-Party includes
#include <zmq.hpp>
#include <gtest/gtest.h>

#ifdef unix

// System includes
#include <unistd.h>
#include <stdio.h>
#include <fstream>
#include <memory>

static const std::string COMMUNICATION_AREA_ONE = "ipc://CommunicationServiceTest";
static const std::string COMMUNICATION_AREA_TWO = "ipc://CommunicationServiceTest2";
static const std::string CORRUPTED_COMMUNICATION_AREA = "azedgfdffdg:z/;/CommunicationServiceTest2";
static const std::string MESSAGE_TO_TEST = "It is the message to test";
static const std::string MESSAGE_TO_TEST_WITH_HEADER = "[ABCD1] It is the message to test";
static const std::string USER_NAME_ONE = "ABCD1";
static const std::string USER_NAME_TWO = "ABCD2";
static const std::string DATABASE_NAME = "myDBP2PMSGForTest.sqlite3";

bool cleanupDatabase()
{
    std::ifstream fileToCheck(DATABASE_NAME.c_str());
    if (fileToCheck.good() == false)
    {
        // No file
        return true;
    } 
    fileToCheck.close();
    return (remove(DATABASE_NAME.c_str()) == 0);
}

TEST(CommunicationServiceTest, SendMessageToExistingService)
{
    ASSERT_TRUE(cleanupDatabase());

    P2PLocalMessage firstService(DATABASE_NAME);
    firstService.initialize(USER_NAME_ONE, COMMUNICATION_AREA_ONE);
    ASSERT_FALSE(firstService.sendMessageToService(USER_NAME_TWO, MESSAGE_TO_TEST));

    // Initialize second service
    P2PLocalMessage secondService(DATABASE_NAME);
    secondService.initialize(USER_NAME_TWO, COMMUNICATION_AREA_TWO);
    // Send one message to service two
    ASSERT_TRUE(firstService.sendMessageToService(USER_NAME_TWO, MESSAGE_TO_TEST));
    // Send one message to service one
    ASSERT_TRUE(firstService.sendMessageToService(USER_NAME_ONE, MESSAGE_TO_TEST));

    // Sleep to wait for the message reception by secondService
    usleep(50000);

    std::vector<std::string> listOfMessages = secondService.collectReceivedMessages();
    ASSERT_EQ(listOfMessages.size(), 1);
    EXPECT_EQ(listOfMessages[0], MESSAGE_TO_TEST_WITH_HEADER);
}

TEST(CommunicationServiceTest, ExceptionOnServiceInitialization)
{
    std::unique_ptr<P2PLocalMessage> myServiceCorrupted;
    EXPECT_ANY_THROW(myServiceCorrupted.reset(new P2PLocalMessage(CORRUPTED_COMMUNICATION_AREA)));
}

#endif

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
