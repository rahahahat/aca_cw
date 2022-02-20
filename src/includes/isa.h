#include <map>

#ifndef _ISA_INCLUDED_
#define _ISA_INCLUDED_

enum InstructionType {
        RType, IType, JType
};

enum Register: int {
    $r0, $r1, $r2, $r3, $r4, $r5, $r6, $r7, 
    $r8, $r9, $r10, $r11, $r12, $r13, $r14, $r15, 
    $r16, $r17, $r18, $r19, $r20, $r21, $r22, $r23, 
    $r24, $r25, $r26, $r27, $r28, $r29, $r30, $r31,
    $noreg = -1,
};

enum Opcodes {
    ADD, ADDI, ADDIU, ADDU, SUB, SUBU, MULT, MULTU, 
    DIV, DIVU, AND, ANDI, OR, ORI, NOR, XOR, LW, 
    SW, BEQ, BLEZ, BNE, BGTZ, J, JAL, JR
};

static std::map<std::string, Register> RegisterMap = 
{
    {"r0",  $r0},
    {"r1",  $r1},
    {"r2",  $r2},
    {"r3",  $r3},
    {"r4",  $r4},
    {"r5",  $r5},
    {"r6",  $r6},
    {"r7",  $r7},
    {"r8",  $r8},
    {"r9",  $r9},
    {"r10", $r10},
    {"r11", $r11},
    {"r12", $r12},
    {"r13", $r13},
    {"r14", $r14},
    {"r15", $r15},
    {"r16", $r16},
    {"r17", $r17},
    {"r18", $r18},
    {"r19", $r19},
    {"r20", $r20},
    {"r21", $r21},
    {"r22", $r22},
    {"r23", $r23},
    {"r24", $r24},
    {"r25", $r25},
    {"r26", $r26},
    {"r27", $r27},
    {"r28", $r28},
    {"r29", $r29},
    {"r30", $r30},
    {"r31", $r31},
};

static std::map<std::string, std::pair<Opcodes, InstructionType>> InstructionPairMap = 
{
    {"add",   std::pair<Opcodes, InstructionType>(ADD, RType)},
    {"addi",  std::pair<Opcodes, InstructionType>(ADDIU, IType)},
    {"addiu", std::pair<Opcodes, InstructionType>(ADDIU, IType)},
    {"addu",  std::pair<Opcodes, InstructionType>(ADDU, RType)},
    {"sub",   std::pair<Opcodes, InstructionType>(SUB, RType)},
    {"subu",  std::pair<Opcodes, InstructionType>(SUBU, RType)},
    {"mult",  std::pair<Opcodes, InstructionType>(MULT, RType)},
    {"multu", std::pair<Opcodes, InstructionType>(MULTU, RType)},
    {"and",   std::pair<Opcodes, InstructionType>(AND, RType)},
    {"andi",  std::pair<Opcodes, InstructionType>(ANDI, IType)},
    {"or",    std::pair<Opcodes, InstructionType>(OR, RType)},
    {"ori",   std::pair<Opcodes, InstructionType>(ORI, IType)},
    {"nor",   std::pair<Opcodes, InstructionType>(NOR, RType)},
    {"xor",   std::pair<Opcodes, InstructionType>(XOR, RType)},
    {"lw",    std::pair<Opcodes, InstructionType>(LW, IType)},
    {"sw",    std::pair<Opcodes, InstructionType>(SW, IType)},
    {"beq",   std::pair<Opcodes, InstructionType>(BEQ, IType)},
    {"blez",  std::pair<Opcodes, InstructionType>(BLEZ, IType)},
    {"bne",   std::pair<Opcodes, InstructionType>(BNE, IType)},
    {"bgtz",  std::pair<Opcodes, InstructionType>(BGTZ, IType)},
    {"div",   std::pair<Opcodes, InstructionType>(DIV, RType)},
    {"divu",  std::pair<Opcodes, InstructionType>(DIVU, RType)},
    {"j",     std::pair<Opcodes, InstructionType>(J, JType)},
    {"jal",   std::pair<Opcodes, InstructionType>(JAL, JType)},
    {"jr",    std::pair<Opcodes, InstructionType>(JR, RType)}
};

#endif