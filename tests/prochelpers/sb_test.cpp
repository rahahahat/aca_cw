#include "catch.h"
#include "isa.h"
#include "prochelpers.h"

TEST_CASE("All scoreboard entries are valid by default")
{
    Scoreboard sb = Scoreboard();
    REQUIRE(sb.isValid($r0) == 1);
    REQUIRE(sb.isValid($r1) == 1);
    REQUIRE(sb.isValid($r2) == 1);
    REQUIRE(sb.isValid($r3) == 1);
    REQUIRE(sb.isValid($r4) == 1);
    REQUIRE(sb.isValid($r5) == 1);
    REQUIRE(sb.isValid($r6) == 1);
    REQUIRE(sb.isValid($r7) == 1);
    REQUIRE(sb.isValid($r8) == 1);
    REQUIRE(sb.isValid($r9) == 1);
    REQUIRE(sb.isValid($r10) == 1);
    REQUIRE(sb.isValid($r11) == 1);
    REQUIRE(sb.isValid($r12) == 1);
    REQUIRE(sb.isValid($r13) == 1);
    REQUIRE(sb.isValid($r14) == 1);
    REQUIRE(sb.isValid($r15) == 1);
    REQUIRE(sb.isValid($r16) == 1);
    REQUIRE(sb.isValid($r17) == 1);
    REQUIRE(sb.isValid($r18) == 1);
    REQUIRE(sb.isValid($r19) == 1);
    REQUIRE(sb.isValid($r20) == 1);
    REQUIRE(sb.isValid($r21) == 1);
    REQUIRE(sb.isValid($r22) == 1);
    REQUIRE(sb.isValid($r23) == 1);
    REQUIRE(sb.isValid($r24) == 1);
    REQUIRE(sb.isValid($r25) == 1);
    REQUIRE(sb.isValid($r26) == 1);
    REQUIRE(sb.isValid($r27) == 1);
    REQUIRE(sb.isValid($r28) == 1);
    REQUIRE(sb.isValid($r29) == 1);
    REQUIRE(sb.isValid($r30) == 1);
    REQUIRE(sb.isValid($r31) == 1);
}

TEST_CASE("Invalidating a register entry in scoreboard")
{
    Scoreboard sb = Scoreboard();
    REQUIRE(sb.isValid($r0) == 1);
    sb.inValidate($r0);
    REQUIRE(sb.isValid($r0) == 0);
}

TEST_CASE("Validating a register entry in the scoreboard")
{
    Scoreboard sb = Scoreboard();
    sb.inValidate($r1);
    REQUIRE(sb.isValid($r1) == 0);
    sb.validate($r1);
    REQUIRE(sb.isValid($r1) == 1);
}



