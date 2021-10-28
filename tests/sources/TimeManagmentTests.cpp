#include "catch.hpp"
#include "mocks/IGetTimeEndpointMock.hpp"
#include "mocks/INetworkEndpointMock.hpp"
#include "TimeManager.hpp"

SCENARIO("Timemanager gets messages from network and answer with time")
{
    GIVEN("Networking and time endpoints")
    {
        std::shared_ptr<IGetTimeEndpointMock> getTime = std::make_shared<IGetTimeEndpointMock>();
        std::shared_ptr<INetworkEndpointMock> network = std::make_shared<INetworkEndpointMock>();

        GIVEN("Manager created and started")
        {
            TimeManager sut;
            sut.start(getTime,network);
            GIVEN("Fixed timestamp")
            {
                getTime->getNowResult.first = "Wed Oct 27 2021 04:28:46 GMT+0000";
                getTime->getNowResult.second = true;
                getTime->getKnownTimeZonesResult = {{"BST", 7}};
                WHEN("Readed message with known timezone")
                {
                    network->executeRead(1,"BST");
                    THEN("Should write time to network")
                    {
                        REQUIRE(network->isWrited == true);
                        REQUIRE(network->writed.second == "Wed Oct 27 2021 04:28:46 GMT+0000\n");
                    }
                    THEN("Should answer to correct client")
                    {
                        REQUIRE(network->writed.first == 1);
                    }
                }
                WHEN("Readed message with unknown timezone")
                {
                    getTime->getNowResult.first = "";
                    getTime->getNowResult.second = false;
                    network->executeRead(2,"OLOLOLOLO");
                    THEN("Should write known timezones to network")
                    {
                        REQUIRE(network->isWrited == true);
                        std::string require =  "I don't know inputed timezone\n";
                        require += "I know these timezones:\n";
                        require += std::string("BST") + ": UTC " + std::to_string(7) + '\n';
                        REQUIRE(network->writed.second == require);

                    }
                    THEN("Should answer to correct client")
                    {
                        REQUIRE(network->writed.first == 2);
                    }
                }
            }
        }
    }
}