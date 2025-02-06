/*
Parsing: Convert tokens into Instruction objects.
First Pass (Labels): Identify and store label addresses.
Second Pass (Translation): Translate instructions into machine code using mappings and resolve
labels. Output: Write the machine code to a file.
*/

#include "parser.hh"

#include <optional>

namespace assembler {
namespace parser {

    InstructionSet AssemblyParser::parse(const lexer::TokenList &assemblyCode) {
        InstructionSet          instructions;
        Tables                  table;
        iterator::TokenIterator gen(assemblyCode);
        lexer::Token            token;
        int                     linecount = 1;
        // first pass to add all labels into the table and any code between semi colons to the label
        // struct. resolve any labels within labels,count the number of semicolons using a stack to
        // make sure that its fully resolved. instantiate a new struct if there is more than one
        // label inside of each other. go through the tokens
        while (gen.peek() != std::nullopt) {
            std::cout << "first loop\n";
            token = *gen.get();
            if (token.type == lexer::TokenType::LABEL) {
                table.emplace_back(get_label_code(token, linecount, gen));
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

    Instruction AssemblyParser::get_operands(const lexer::Token      &tkn,
                                             const int                line,
                                             iterator::TokenIterator &gen) {
        lexer::Token              opcode = tkn;
        std::vector<lexer::Token> operands;
        
        while (gen.peek() != std::nullopt) {
            std::cout << "second loop\n";
            auto token = gen.get();
            if (tkn.type == lexer::TokenType::NEWLINE) {
                break;
            }
            operands.emplace_back(*token);
        }
        return {.opcode = opcode, .operands = operands, .line = line};
    }

    LabelTable AssemblyParser::get_label_code(const lexer::Token      &tkn,
                                              int                     &line,
                                              iterator::TokenIterator &gen) {
        lexer::Token   label = tkn;
        InstructionSet instruction;
        // get all the code between the label def and the semi-colon
        // call the get operands function to get the code
        // return the instruction
        // increase the line number every time a new line is encountered
        while (gen.peek() != std::nullopt) {
            std::cout << "third loop\n";
            auto token = gen.get();
            if (token->type == lexer::TokenType::SEMICOLON) {
                break;
            }
            if (token->opcode == true) {
                instruction.emplace_back(get_operands(*token, line, gen));
            }
            if (token->type == lexer::TokenType::NEWLINE) {
                line++;
            }
        }
        return {.label = label, .code = instruction};
    }

}  // namespace parser
}  // namespace assembler
