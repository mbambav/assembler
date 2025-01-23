#include "lexer.hh"

#include <filesystem>
#include <iostream>

namespace Lexer {
Lexer::Lexer(std::string_view file) {
    this->file = file;
    std::filesystem::path pathobj(file);
    std::string           file_name = pathobj.filename().string();
    std::wstring          widestr   = std::wstring(file_name.begin(), file_name.end());
#if defined(__APPLE__) || defined(__linux__) || defined(__unix__) || defined(__MACH__)
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
#elif defined(_WIN32)
    HANDLE file_handle = CreateFileW(
        widestr.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if (file_handle == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open file\n";
        std::exit(100);
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(file_handle, &size)) {
        std::cerr << "could not get file size\n";
        CloseHandle(file_handle);
        std::exit(200);
    }

    size_t file_size = static_cast<size_t>(size.QuadPart);

    HANDLE file_mapping = CreateFileMapping(file_handle, nullptr, PAGE_READONLY, 0, 0, nullptr);

    if (!file_mapping) {
        std::cerr << "could not create file mapping\n";
        CloseHandle(file_handle);
        std::exit(300);
    }

    void *map = MapViewOfFile(file_mapping, FILE_MAP_READ, 0, 0, 0);

    if (!map) {
        std::cerr << "failed to map view of file\n";
        CloseHandle(file_handle);
        CloseHandle(file_mapping);
        std::exit(400);
    }

    CloseHandle(file_handle);
    CloseHandle(file_mapping);

#else
    std::cerr << "unknow operating system\n";
    std::exit(1);
#endif

    this->data = std::string_view(static_cast<const char *>(map), file_size);
}

TokenList Lexer::tokenize() {
    TokenList tokens;
    // check if next char is !\n
    // add to word
    // check if it is a tokentype
    // check if it is valid tokentype
    size_t      i          = 0;
    size_t      max_length = data.size();
    std::string word       = "";

    for (; i < max_length; i++) {
        char cur_ch = data[i];

        switch (cur_ch) {
            case ',': {
                if (word.empty()) {
                    break;
                }
                tokens.emplace_back(get_token_type(word));
                tokens.emplace_back(get_token_type(","));
                word = "";
                break;
            }
            case ' ': {
                if (word.empty()) {
                    break;
                }
                tokens.emplace_back(get_token_type(word));
                word = "";
                break;
            }
            case '\r': {
            line_ending_handling:
                if (i + 1 > max_length || data[i + 1] != '\n') {
                    word += cur_ch;
                }
                break;
            }
            case '\n': {
                if (word.empty()) {
                    break;
                }
                tokens.emplace_back(get_token_type(word));
                word = "";
                break;
            }
            case '/': {
                if (i + 1 > max_length && data[i + 1] != '/') {
                    break;
                }
                while (i < max_length) {
                    cur_ch = data[i];
                    if (cur_ch == '\n' || cur_ch == '\r') {
                        goto line_ending_handling;
                        break;
                    }

                    word += cur_ch;
                    ++i;
                }

                tokens.emplace_back(get_token_type(word));
                word = "";
                break;
            }

            default:
                word += cur_ch;
                break;
        }
    }
    if (!word.empty()) {
        tokens.emplace_back(get_token_type(word));
    }

    std::cout << "comment type: " << std::to_string(int(mapping["comment"])) << "\n";
    std::cout << "comment type: " << std::to_string(int(mapping["comment"])) << "\n";
    for (auto &i : tokens) {
        std::cout << "tok: " << std::to_string(int(i.type)) << " \"" << std::string(i.data)
                  << "\"\n";
    }
    return tokens;
}

Token Lexer::get_token_type(const std::string &word) {
    auto             elm  = mapping.find(word);
    size_t           line = 0;
    size_t           col  = 0;
    std::string_view file = "";

    if (elm != mapping.end()) {
        return {.data = word, .file = file, .line = line, .column = col, .type = elm->second};
    } else {
        for (auto &literal : literals) {
            if (std::regex_match(word, literal)) {
                return {.data   = word,
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::LITERAL};
            }
        }

        for (auto &register_ : registers) {
            if (std::regex_match(word, register_)) {
                return {.data   = word,
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::REGISTER};
            }
        }
        for (auto &comma : commas) {
            if (std::regex_match(word, comma)) {
                return {.data   = word,
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::COMMA};
            }
        }

        for (auto &string_ : strings) {
            if (std::regex_match(word, string_)) {
                return {.data   = word.substr(1, word.length() - 2),
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::STRING};
            }
        }

        for (auto &label : labels) {
            if (std::regex_match(word, label)) {
                return {.data   = word.substr(0, word.length() - 1),
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::LABEL};
            }
        }

        for(auto &label_call : label_calls) {
            if (std::regex_match(word, label_call)) {
                return {.data   = word,
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::LABEL_CALL};
            }
        }

        for (auto &comment : comments) {
            if (std::regex_match(word, comment)) {
                return {.data   = word,
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::COMMENT};
            }
        }
    }

    return {.data = word, .file = file, .line = line, .column = col, .type = TokenType::UNKNOWN};
}
}  // namespace Lexer

int main() {
    Lexer::Lexer lexer("/Volumes/Projects/c++/test.txt");
    lexer.tokenize();
    return 0;
}