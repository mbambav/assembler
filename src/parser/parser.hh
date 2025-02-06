#ifndef ASSEMBLY_PARSER_H
#define ASSEMBLY_PARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <optional>
#include <ostream>

#include "../lexer/lexer.hh"
#include "iterator.hh"

namespace assembler{
namespace parser{
// Represents a single instruction in your assembly language
struct Instruction {
    lexer::Token opcode;                // The operation code (e.g., "ADD", "LOAD")
    std::vector<lexer::Token> operands;
    int line; // Operands (e.g., "R1", "R2", "#5")

    // //default constructor
    // Instruction() = default;
    // //copy constructor
    // Instruction(const Instruction& instruction) = default;
    // //copy assignment
    // Instruction& operator=(const Instruction& instruction) = default;
    // //move assignment
    // Instruction& operator=(Instruction&& instruction) = default;
    // //move constructor
    // Instruction(Instruction&& instruction) = default;
};

using InstructionSet = std::vector<Instruction>;

struct LabelTable {
    lexer::Token label;
    InstructionSet code;
};

using Tables = std::vector<LabelTable>;

// Represents a parsed assembly program
class AssemblyParser {
public:
    // Parses a raw assembly program from a string
    // Input: The raw assembly code as a string
    // Output: A vector of parsed instructions
    InstructionSet parse(const lexer::TokenList &assemblyCode);

    Instruction get_operands(const lexer::Token &tkn,const int line, iterator::TokenIterator &gen);

    LabelTable get_label_code(const lexer::Token &tkn, int &line, iterator::TokenIterator &gen);

    // Resolves labels in the parsed program
    // Input: A vector of instructions with potential labels
    // Output: Updates label positions and removes labels from instructions
    std::unordered_map<std::string, int> resolveLabels(const std::vector<Instruction>& instructions);

    // Validates the syntax of a given instruction
    // Input: A single instruction
    // Output: Throws an exception if the instruction is invalid
    void validateInstruction(const Instruction& instruction) const;

private:
    // Checks if a string represents a valid opcode
    // Input: The opcode string
    // Output: True if valid, false otherwise
    bool isValidOpcode(const std::string& opcode) const;

    // Symbol table for labels
};
}// namespace parser
}// namespace assembler
#endif // ASSEMBLY_PARSER_H
