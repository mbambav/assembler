/*
Parsing: Convert tokens into Instruction objects.
First Pass (Labels): Identify and store label addresses.
Second Pass (Translation): Translate instructions into machine code using mappings and resolve
labels. Output: Write the machine code to a file.
*/

#include "parser.hh"

#include <iostream>
#include <optional>
#include <ostream>
#include <vector>

namespace assembler {
namespace parser {

    InstructionSet AssemblyParser::parse(const lexer::TokenList &assemblyCode) {
        InstructionSet          instructions;
        Tables                  table;
        iterator::TokenIterator gen(assemblyCode);
        lexer::Token            token;
        int                     linecount = 1;
        lexer::Token            temp_token;
        // first pass to add all labels into the table and any code between semi colons to the label
        // struct. resolve any labels within labels,count the number of semicolons using a stack to
        // make sure that its fully resolved. instantiate a new struct if there is more than one
        // label inside of each other. go through the tokens
        while (gen.peek() != std::nullopt) {
            if (token.type == lexer::TokenType::LABEL) {
                ...
            }
            if (token.opcode == true) {
                instructions.emplace_back(get_operands(token, linecount, gen));
            }
            if (token.type == lexer::TokenType::NEWLINE) {
                linecount++;
            }
        }

        return instructions;
    }

    Instruction AssemblyParser::get_operands(const lexer::Token     &tkn,
                                             const int               line,
                                             iterator::TokenIterator gen) {
        lexer::Token              opcode = tkn;
        std::vector<lexer::Token> operands;
        while (gen.peek() != std::nullopt) {
            auto token = gen.get();
            if (tkn.type == lexer::TokenType::NEWLINE) {
                break;
            }
            operands.emplace_back(*token);
        }
        return {.opcode = opcode, .operands = operands, .line = line};
    }

    Instruction AssemblyParser::get_label_code(const lexer::Token     &tkn,
                                               int                     line,
                                               iterator::TokenIterator gen) {
                                                
                                               }

}  // namespace parser
}  // namespace assembler
