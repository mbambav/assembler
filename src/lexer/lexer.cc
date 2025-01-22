#include "lexer.hh"

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
namespace Lexer {
Lexer::Lexer(std::string_view file) {
    this->file = file;
    // read the contents of the file and store it in data
    int fd = open(file.data(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "failed to open file\n";
        std::exit(1000);
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    if (file_size == -1) {
        close(fd);
        std::cerr << "failed to get file size\n";
        std::exit(2000);
    }

    void *map = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        std::cerr << "failed to map\n";
        std::exit(3000);
    }

    close(fd);

    this->data = std::string_view(static_cast<const char *>(map), file_size);
}

TokenList Lexer::tokenize() {
    TokenList tokens;
    // check if next char is !\n
    // add to word
    // check if it is a tokentype
    // check if it is valid tokentype
    int         i    = 0;
    std::string word = "";
    for (size_t i = 0; i < this->data.size(); ++i) {
        char c = this->data[i];

        if (i + 1 < this->data.size()) {
            switch (this->data[i + 1]) {
                case ' ':
                case '\n': {
                    word += c;
                    get_token_type(word);
                    word = "";
                    i++;
                    break;
                }
                default:
                    word += c;
                    break;
            }
        } else {
            word += c;
            get_token_type(word);
            word = "";
            break;
        }
    }
}

TokenType Lexer::get_token_type(std::string word) {
    auto elm = mapping.find(word);

    if (elm != mapping.end()) {
        return elm->second;
    } else {
        for (auto &literal : literals) {
            if (std::regex_match(word, literal)) {
                return TokenType::LITERAL;
            }
        }

        for (auto &newline : newlines) {
            if (std::regex_match(word, newline)) {
                return TokenType::NEWLINE;
            }
        }

        for (auto &register_ : registers) {
            if (std::regex_match(word, register_)) {
                return TokenType::REGISTER;
            }
        }

        for (auto &string_ : strings) {
            if (std::regex_match(word, string_)) {
                return TokenType::STRING;
            }
        }

        for (auto &label : labels) {
            if (std::regex_match(word, label)) {
                return TokenType::LABEL;
            }
        }

        for (auto &comment : comments) {
            if (std::regex_match(word, comment)) {
                return TokenType::COMMENT;
            }
        }

        for (auto &unk : unknown) {
            if (std::regex_match(word, unk)) {
                return TokenType::UNKNOWN;
            }
        }
    }

    return TokenType::UNKNOWN;
}
}  // namespace Lexer

int main() {
    Lexer::Lexer lexer("/Volumes/Projects/c++/test.txt");
    lexer.tokenize();
    return 0;
}