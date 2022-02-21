#include "catch.h"
#include "prochelpers.h"
#include "isa.h"

TEST_CASE( "Adding value to RF", "[rf]" )
{
    ResultForwarder rf = ResultForwarder();
    WHEN("register entry is empty")
    {
        rf.addValue($r0, 1);
        REQUIRE(rf.getSize() == 1);
    }

    WHEN("register entry is not empty (overwrite)")
    {
        rf.addValue($r0, 1);
        rf.addValue($r0, 2);
        REQUIRE(rf.getSize() == 1);
    }
}


TEST_CASE( "Removing value from RF", "[rf]" )
{
    ResultForwarder rf = ResultForwarder();
    WHEN("register entry is not empty")
    {
        rf.addValue($r0, 1);
        rf.addValue($r1, 2);
        REQUIRE(rf.getSize() == 2);
        rf.removeValue($r1);
        REQUIRE(rf.getSize() == 1);
        rf.removeValue($r0);
        REQUIRE(rf.getSize() == 0);
    }
}

TEST_CASE("Getting value from RF")
{
    ResultForwarder rf = ResultForwarder();
    SECTION("should be invalid")
    {
        WHEN("register entry is empty")
        {
            std::pair<int, int> res = rf.getValue($r0);
            REQUIRE(res.first == 0);
            REQUIRE(res.second == 0);
        }
    }

    SECTION("should not increase map size")
    {
        WHEN("value is being overwritten")
        {
            rf.addValue($r1, 10);
            rf.addValue($r1, 20);
            REQUIRE(rf.getSize() == 1);
        }
    }

    SECTION("should be valid and return correct value")
    {
        WHEN("register is not being overwritten")
        {
            rf.addValue($r3, 10);
            std::pair<int, int> res = rf.getValue($r3);
            REQUIRE(res.first == 1);
            REQUIRE(res.second == 10);
        }
        WHEN("register is being overwritten")
        {
            rf.addValue($r2, 10);
            rf.addValue($r2, 3);
            std::pair<int, int> res = rf.getValue($r2);
            REQUIRE(res.first == 1);
            REQUIRE(res.second == 3);
        }
    }
}