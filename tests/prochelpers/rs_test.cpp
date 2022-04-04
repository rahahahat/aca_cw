#include "catch.h"
#include "isa.h"
#include "prochelpers.h"
#include "events.h"
#include "backward.hpp"

TEST_CASE("Reservation Station instantiation")
{
    Scoreboard* sb = new Scoreboard(true);
    rs::ReservationStation *rs = new rs::ReservationStation(sb, true);
    delete rs;
    delete sb;
}

TEST_CASE("Reservation Station reserves entry correctly")
{
    Scoreboard* sb = new Scoreboard(true);
    rs::ReservationStation *rs = new rs::ReservationStation(sb, true);
    Instructions::Instruction *instr = new Instructions::Instruction();
    instr->rd = $r10;
    instr->rs = $r11;
    instr->rt = $r13;
    instr->type = RType;
    rs->reserve(instr);
    auto rse = rs->getEntry(instr->tag);
    REQUIRE(rse->isReserved == true);
    delete rs;
    delete sb;
}

TEST_CASE("Reservation Station reserves multiple entrires")
{
    Scoreboard* sb = new Scoreboard(true);
    rs::ReservationStation *rs = new rs::ReservationStation(sb, true);
    Instructions::Instruction *instr = new Instructions::Instruction();
    instr->rd = $r10;
    instr->rs = $r11;
    instr->rt = $r13;
    instr->type = RType;
    Instructions::Instruction *instr1 = new Instructions::Instruction();
    instr1->rd = $r14;
    instr1->rs = $r15;
    instr1->rt = $r10;
    instr1->type = RType;
    rs->reserve(instr);
    rs->reserve(instr1);
    REQUIRE(rs->getEntry(instr->tag)->isReserved == true);
    REQUIRE(rs->getEntry(instr1->tag)->isReserved == true);
    REQUIRE(rs->getEntry(instr1->tag)->valid_two == 0);
    REQUIRE(rs->getEntry(instr1->tag)->valid_one == 1);
    REQUIRE(rs->getEntry(instr1->tag)->tag_two.compare("tag01") == 0);
}

TEST_CASE("Reservation Station populates properly")
{
    Scoreboard* sb = new Scoreboard(true);
    rs::ReservationStation *rs = new rs::ReservationStation(sb, true);
    Instructions::Instruction *instr = new Instructions::Instruction();
    instr->rd = $r10;
    instr->rs = $r11;
    instr->rt = $r13;
    instr->type = RType;
    Instructions::Instruction *instr1 = new Instructions::Instruction();
    instr1->rd = $r14;
    instr1->rs = $r15;
    instr1->rt = $r10;
    instr1->type = RType;
    rs->reserve(instr);
    rs->reserve(instr1);
    Event<rsEventData> event = Event<rsEventData>();
    rsEventData pl;
    event.set("tag01");
    pl.tag_name = "tag01";
    pl.value = 10;
    event.payload = pl;
    auto rse = rs->getEntry("tag02");
    REQUIRE(rse->valid_one == 1);
    REQUIRE(rse->val_one == 0);
    rs->dispatch(event);
    rse = rs->getEntry("tag02");
    REQUIRE(rse->isReserved == true);
    REQUIRE(rse->valid_two == 1);
    REQUIRE(rse->val_two == 10);
}