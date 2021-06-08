/**
 * @file DatabaseServiceUnitTest.cpp
 * @author Axel Fortun
 * @brief The Unitary Test to validate DatabaseService
 */


// DatabaseService includes
#include <DatabaseService.hpp>

// Third-Party includes
#include <gtest/gtest.h>

static const unsigned int NUMBER_OF_SERVICES_TO_TEST = 100;
static const std::string IN_MEMORY_DATABASE = ":memory:";
static DatabaseService myDatabaseToTest(IN_MEMORY_DATABASE);

TEST(DatabaseServiceTest, NominalTest)
{
    myDatabaseToTest.cleanupTable();
    
    myDatabaseToTest.addOrReplaceService("Toto", "Titi");
    EXPECT_EQ("Titi", myDatabaseToTest.getNetworkInfoFromuserName("Toto"));
    myDatabaseToTest.removeService("Toto");
    EXPECT_EQ("", myDatabaseToTest.getNetworkInfoFromuserName("Toto"));
}

TEST(DatabaseServiceTest, ReplaceDataTest)
{
    myDatabaseToTest.cleanupTable();

    myDatabaseToTest.addOrReplaceService("Toto", "Titi");
    EXPECT_EQ("Titi", myDatabaseToTest.getNetworkInfoFromuserName("Toto"));
    myDatabaseToTest.addOrReplaceService("Toto", "Titi2");
    EXPECT_EQ("Titi2", myDatabaseToTest.getNetworkInfoFromuserName("Toto"));
}

TEST(DatabaseServiceTest, MultipleService)
{
    myDatabaseToTest.cleanupTable();

    for (unsigned int i = 0; i < NUMBER_OF_SERVICES_TO_TEST; ++i)
    {
        std::string valueToAdd = std::to_string(i);
        myDatabaseToTest.addOrReplaceService(valueToAdd, valueToAdd);
    }

    for (unsigned int i = 0; i < NUMBER_OF_SERVICES_TO_TEST; ++i)
    {
        std::string valueToEvaluate = std::to_string(i);
        EXPECT_EQ(valueToEvaluate, myDatabaseToTest.getNetworkInfoFromuserName(valueToEvaluate));
    }
}

TEST(DatabaseServiceTest, SpuriousElement)
{
    myDatabaseToTest.cleanupTable();

    myDatabaseToTest.addOrReplaceService("Toto;42", "Ti ti");
    EXPECT_EQ("Titi", myDatabaseToTest.getNetworkInfoFromuserName("Toto42"));

}

TEST(DatabaseServiceTest, MaximumSize)
{
    myDatabaseToTest.cleanupTable();
    std::string MaxSizeuserName(1024, 'Y');
    myDatabaseToTest.addOrReplaceService(MaxSizeuserName, "Titi");
    
    EXPECT_EQ("Titi", myDatabaseToTest.getNetworkInfoFromuserName(MaxSizeuserName));
    MaxSizeuserName.resize(DatabaseService::MAXIMUM_USER_NAME_SIZE);
    EXPECT_EQ("Titi", myDatabaseToTest.getNetworkInfoFromuserName(MaxSizeuserName));
}

TEST(DatabaseServiceTest, CorruptedDatabase)
{
    EXPECT_THROW(DatabaseService{"42root//  this is a strange path"}, std::string);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
