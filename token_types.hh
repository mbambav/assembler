#ifndef TOKEN_TYPES_HH
#define TOKEN_TYPES_HH
#include <csetjmp>
#include <functional>
#include <string>
#include <regex>
#include <string_view>
namespace assembler::lexer {

    
enum class TokenType {
    LOAD,
    STORE,
    MOVE,
    PUSH,
    POP,
    SWAP,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    INC,
    DEC,
    AND,
    OR,
    XOR,
    NOT,
    SHL,
    SHR,
    CMP,
    EQ,
    NE,
    JMP,
    JE,
    JNE,
    JG,
    JGE,
    JL,
    JLE,
    WHILE,
    CALL,
    RET,
    ENTER,
    LEAVE,
    NOP,
    STOP,
    DEBUG,
    SYS,
    LITERAL,
    NEWLINE,
    REGISTER,
    STRING,
    COMMENT,
    LABEL,
    UNKNOWN
};

struct Token {
    const std::string_view data;
    const std::string_view file;
    size_t line;
    size_t column;
    TokenType type;
};


//define literals
const std::regex literals[] = {
    std::regex("[0-9]+"),
    std::regex("0x[0-9a-fA-F]+"),
    std::regex("0b[01]+"),
};

//define newlines
const std::regex newlines[] = {
    std::regex("\n")
};

//define registers
const std::regex registers[] = {
    std::regex("r[0-9]+")
};

//define strings
const std::regex strings[] = {
    std::regex("\".*\"")
};

//define labels
const std::regex labels[] = {
    std::regex("[a-zA-Z_][a-zA-Z0-9_]*:")
};

//define comments using //
const std::regex comments[] = {
    std::regex("//.*")
};

//define unknown
const std::regex unknown[] = {
    std::regex(".+")
};


} // namespace assembler

#endif // !TOKEN_TYPES_HH
