#include "catch.hpp"
#include "ChronoTimeEndpoing.hpp"

SCENARIO("ChronoTimeEndpoint returns time in timezone")
{
    GIVEN("Default format")
    {
        std::string defaultFormat = "%a %b %d %T %Z %Y";
        GIVEN("Fixed timestamp")
        {
            time_t fixedTimeStamp = 1635308926; // == Wed Oct 27 2021 04:28:46 GMT+0000
            ChronoTimeEndpoint sut(defaultFormat, [fixedTimeStamp]() {return fixedTimeStamp;});
            WHEN("Get time in known timezone(BST)")
            {
                auto result = sut.getNowInTimezone("bst");
                THEN("Should return right answer")
                {
                    REQUIRE(result.second == true);
                    REQUIRE(result.first == "Wed Oct 27 05:28:46 BST 2021");
                }
            }
            WHEN("Get time in known timezone(KRAT)")
            {
                auto result = sut.getNowInTimezone("KRAT");
                THEN("Should return right answer")
                {
                    REQUIRE(result.second == true);
                    REQUIRE(result.first == "Wed Oct 27 11:28:46 KRAT 2021");
                }
            }
            WHEN("Get time in known timezone(MSK)")
            {
                auto result = sut.getNowInTimezone("Msk");
                THEN("Should return right answer")
                {
                    REQUIRE(result.second == true);
                    REQUIRE(result.first == "Wed Oct 27 07:28:46 MSK 2021");
                }
            }
            WHEN("Get time in known timezone(PST) with shift of day back")
            {
                auto result = sut.getNowInTimezone("PST");
                THEN("Should return right answer")
                {
                    REQUIRE(result.second == true);
                    REQUIRE(result.first == "Tue Oct 26 20:28:46 PST 2021");
                }
            }
            WHEN("Get time in known timezone(TVT) with shift of day forward")
            {
                ChronoTimeEndpoint sut2(defaultFormat, []() {return 1635342164;}); // Wed Oct 27 2021 13:42:44 GMT+0000
                auto result = sut2.getNowInTimezone("TVT");
                THEN("Should return right answer")
                {
                    REQUIRE(result.second == true);
                    REQUIRE(result.first == "Thu Oct 28 01:42:44 TVT 2021");
                }
            }
            WHEN("Get time in unknown timezone")
            {
                auto result = sut.getNowInTimezone("MOTOMOTO");
                THEN("Should return failed answer")
                {
                    REQUIRE(result.second == false);
                    REQUIRE(result.first == "");
                }
            }
        }
    }
}

SCENARIO("ChronoTimeEndpoint returns time in setted format")
{
    GIVEN("Fixed timestamp")
    {
        time_t fixedTimeStamp = 1635308926; // == Wed Oct 27 2021 04:28:46 GMT+0000
        GIVEN("Default format")
        {
            std::string defaultFormat = "%a %b %d %T %Z %Y";
            ChronoTimeEndpoint sut(defaultFormat, [fixedTimeStamp]() {return fixedTimeStamp;});
            WHEN("Get time in known timezone(BST)")
            {
                auto result = sut.getNowInTimezone("bst");
                THEN("Should return right answer")
                {
                    REQUIRE(result.second == true);
                    REQUIRE(result.first == "Wed Oct 27 05:28:46 BST 2021");
                }
            }
            WHEN("Get time in unknown timezone")
            {
                auto result = sut.getNowInTimezone("MOTOMOTO");
                THEN("Should return failed answer")
                {
                    REQUIRE(result.second == false);
                    REQUIRE(result.first == "");
                }
            }
        }
        GIVEN("%A %w %H %W format")
        {
            std::string format = "%A %w %H %W";
            ChronoTimeEndpoint sut(format, [fixedTimeStamp]() {return fixedTimeStamp;});
            WHEN("Get time in known timezone(BST)")
            {
                auto result = sut.getNowInTimezone("bst");
                THEN("Should return right answer")
                {
                    REQUIRE(result.second == true);
                    REQUIRE(result.first == "Wednesday 3 05 43");
                }
            }
            WHEN("Get time in unknown timezone")
            {
                auto result = sut.getNowInTimezone("MOTOMOTO");
                THEN("Should return failed answer")
                {
                    REQUIRE(result.second == false);
                    REQUIRE(result.first == "");
                }
            }
        }
        GIVEN("%Y %y %U %a %R format")
        {
            std::string format = "%Y %y %U %a %R";
            ChronoTimeEndpoint sut(format, [fixedTimeStamp]() {return fixedTimeStamp;});
            WHEN("Get time in known timezone(BST)")
            {
                auto result = sut.getNowInTimezone("bst");
                THEN("Should return right answer")
                {
                    REQUIRE(result.second == true);
                    REQUIRE(result.first == "2021 21 43 Wed 05:28");
                }
            }
            WHEN("Get time in unknown timezone")
            {
                auto result = sut.getNowInTimezone("MOTOMOTO");
                THEN("Should return failed answer")
                {
                    REQUIRE(result.second == false);
                    REQUIRE(result.first == "");
                }
            }
        }
    }
}
