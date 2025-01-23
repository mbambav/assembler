#ifndef TOKEN_TYPES_HH
#define TOKEN_TYPES_HH

#include <map>
#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include <fcntl.h>



#ifdef _WIN32
#include <handleapi.h>
#include <io.h>
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

#include <cstdio>


namespace Lexer {
struct Token;
using TokenList = std::vector<Token>;

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
    REGISTER,
    COMMA,
    STRING,
    COMMENT,
    LABEL,
    LABEL_CALL,
    UNKNOWN
};

std::map<std::string, TokenType> mapping{
    {"load", TokenType::LOAD},
    {"store", TokenType::STORE},
    {"move", TokenType::MOVE},
    {"push", TokenType::PUSH},
    {"pop", TokenType::POP},
    {"swap", TokenType::SWAP},
    {"add", TokenType::ADD},
    {"sub", TokenType::SUB},
    {"mul", TokenType::MUL},
    {"div", TokenType::DIV},
    {"mod", TokenType::MOD},
    {"inc", TokenType::INC},
    {"dec", TokenType::DEC},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"xor", TokenType::XOR},
    {"not", TokenType::NOT},
    {"shl", TokenType::SHL},
    {"shr", TokenType::SHR},
    {"cmp", TokenType::CMP},
    {"eq", TokenType::EQ},
    {"ne", TokenType::NE},
    {"jmp", TokenType::JMP},
    {"je", TokenType::JE},
    {"jne", TokenType::JNE},
    {"jg", TokenType::JG},
    {"jge", TokenType::JGE},
    {"jl", TokenType::JL},
    {"jle", TokenType::JLE},
    {"while", TokenType::WHILE},
    {"call", TokenType::CALL},
    {"ret", TokenType::RET},
    {"enter", TokenType::ENTER},
    {"leave", TokenType::LEAVE},
    {"nop", TokenType::NOP},
    {"stop", TokenType::STOP},
    {"debug", TokenType::DEBUG},
    {"sys", TokenType::SYS},
    {"literal", TokenType::LITERAL},
    {"register", TokenType::REGISTER},
    {"string", TokenType::STRING},
    {"comma", TokenType::COMMA},
    {"comment", TokenType::COMMENT},
    {"label", TokenType::LABEL},
    {"label_call", TokenType::LABEL_CALL},
    {"unknown", TokenType::UNKNOWN},
};


struct Token {
    const std::string data;
    const std::string_view file;
    size_t                 line;
    size_t                 column;
    TokenType              type;
};

class Lexer {
  private:
    std::string_view file;
    std::string_view data;

    size_t line;
    size_t column;
    size_t offset;

  public:
    Lexer(std::string_view file);
    TokenList tokenize();
    Token get_token_type(const std::string& word);
};

// define literals
const std::regex literals[] = {
    std::regex("[0-9]+"),
    std::regex("0x[0-9a-fA-F]+"),
    std::regex("0b[01]+"),
};

// define registers
const std::regex registers[] = {std::regex("r[0-9]+")};

// define commas
const std::regex commas[] = {std::regex(",")};

// define strings
const std::regex strings[] = {std::regex("\".*\"")};

// define labels
const std::regex labels[] = {std::regex("[a-zA-Z_][a-zA-Z0-9_]*:")};

//define label calls. they start with a .
const std::regex label_calls[] = {std::regex("\\.[a-zA-Z_][a-zA-Z0-9_]*")};

// define comments using //
const std::regex comments[] = {std::regex("//.*")};

}  // namespace Lexer
#endif  // !TOKEN_TYPES_HH
