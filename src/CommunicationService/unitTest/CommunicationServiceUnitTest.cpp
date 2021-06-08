/**
 * @file CommunicationServiceUnitTest.cpp
 * @author Axel Fortun
 * @brief The Unitary Test to validate Communication Service
 * @details Test only compatible on UNIX systems.
 */


// CommunicationService includes
#include <CommunicationService.hpp>
// Third-Party includes
#include <zmq.hpp>
#include <gtest/gtest.h>

#ifdef unix

// System includes
#include <unistd.h>

static const std::string COMMUNICATION_AREA_ONE = "ipc://CommunicationServiceTest";
static const std::string COMMUNICATION_AREA_TWO = "ipc://CommunicationServiceTest2";
static const std::string CORRUPTED_COMMUNICATION_AREA = "I am an invalid communication area";
static const std::string MESSAGE_TO_TEST = "It is the message to test";
static const std::string USER_NAME = "ABCD";

TEST(CommunicationServiceTest, NominalTest)
{
    CommunicationService firstService;
    CommunicationService secondService;

    firstService.connectReceiver(COMMUNICATION_AREA_ONE);
    secondService.connectReceiver(COMMUNICATION_AREA_TWO);

    firstService.addOrReplaceMessageSender(USER_NAME, COMMUNICATION_AREA_TWO);
    ASSERT_TRUE(firstService.sendMessage(USER_NAME, MESSAGE_TO_TEST));
    
    // Sleep to wait for the message reception by secondService
    usleep(50000);

    // The message should be received by the second service.
    auto messageReceived = secondService.collectReceivedMessages();
    ASSERT_EQ(messageReceived.size(), 1);
    EXPECT_EQ(messageReceived[0], MESSAGE_TO_TEST);
}

TEST(CommunicationServiceTest, InvalidAreaOfCommunication)
{
    CommunicationService firstService;

    EXPECT_THROW(firstService.connectReceiver(CORRUPTED_COMMUNICATION_AREA), zmq::error_t);
    EXPECT_THROW(firstService.addOrReplaceMessageSender(USER_NAME, CORRUPTED_COMMUNICATION_AREA), zmq::error_t);

}

TEST(CommunicationServiceTest, CheckExistingService)
{
    CommunicationService firstService;
    CommunicationService secondService;

    firstService.connectReceiver(COMMUNICATION_AREA_ONE);
    secondService.connectReceiver(COMMUNICATION_AREA_TWO);

    // Service doesn't exist
    EXPECT_FALSE(firstService.isServiceKnown(USER_NAME));
    EXPECT_FALSE(firstService.sendMessage(USER_NAME, MESSAGE_TO_TEST));

    // Service added
    firstService.addOrReplaceMessageSender(USER_NAME, COMMUNICATION_AREA_TWO);
    EXPECT_TRUE(firstService.isServiceKnown(USER_NAME));
    EXPECT_TRUE(firstService.sendMessage(USER_NAME, MESSAGE_TO_TEST));

    // Sleep to wait for the message reception by secondService
    usleep(50000);

    // The message should be received by the second service.
    auto messageReceived = secondService.collectReceivedMessages();
    ASSERT_EQ(messageReceived.size(), 1);
}

#endif

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
