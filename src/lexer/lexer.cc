#include "lexer.hh"
#include <fcntl.h>
#include <handleapi.h>
#include <filesystem> 

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#endif

#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>
namespace Lexer {
Lexer::Lexer(std::string_view file) {
    this->file = file;
    std::filesystem::path pathobj(file);
    std::string file_name = pathobj.filename().string();
    std::wstring widestr = std::wstring(file_name.begin(),file_name.end());
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
            widestr.c_str(),
            GENERIC_READ,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );

        if (file_handle == INVALID_HANDLE_VALUE){
            std::cerr << "Failed to open file\n";
            std::exit(100);
        }

        LARGE_INTEGER size;
        if (!GetFileSizeEx(file_handle, &size)){
            std::cerr << "could not get file size\n";
            CloseHandle(file_handle);
            std::exit(200);
        }

        size_t file_size = static_cast<size_t>(size.QuadPart);
        
        HANDLE file_mapping = CreateFileMapping(
            file_handle,
            nullptr,
            PAGE_READONLY,
            0,
            0,
            nullptr
        );

        if (!file_mapping){
            std::cerr << "could not create file mapping\n";
            CloseHandle(file_handle);
            std::exit(300);
        }

        void* map = MapViewOfFile(
            file_mapping,
            FILE_MAP_READ,
            0,
            0,
            0
        );

        if(!map){
            std::cerr << "failed to map view of file\n";
            CloseHandle(file_handle);
            CloseHandle(file_mapping);
            std::exit(400);
        }

        CloseHandle(file_handle);
        CloseHandle(file_mapping);


    #else
        std::cerr<<"unknow operating system\n";
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
    int         i    = 0;
    std::string word = "";
    for (size_t i = 0; i < this->data.size(); ++i) {
        char c = this->data[i];

        if (i + 1 < this->data.size()) {
            switch (this->data[i + 1]) {
                case ' ':
                case '\n':
                    word += c;
                    tokens.emplace_back(get_token_type(word));
                    word = "";
                    i++;
                    break;
                case '/':
                    if (((i + 2) < this->data.size()) && (this->data[i + 2] == '/' || this->data[i]=='/')) {
                        size_t j = i;
                        while (j < this->data.size() && this->data[j] != '\n') {
                            c = this->data[j];
                            word += c;
                            j++;
                        }
                        i = j;
                    }
                    tokens.emplace_back(get_token_type(word));
                    word = "";
                    break;
                default:
                    word += c;
                    break;
            }
        } else {
            word += c;
            tokens.emplace_back(get_token_type(word));
            word = "";
            break;
        }
    }

    std::cout << "comment type: " << std::to_string(int(mapping["comment"])) << "\n";
    std::cout << "comment type: " << std::to_string(int(mapping["comment"])) << "\n";
    for (auto &i : tokens) {
        std::cout << "tok: " << std::to_string(int(i.type)) << " \"" << std::string(i.data) << "\"\n";
    }
    
}

Token Lexer::get_token_type(const std::string& word) {
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

        for (auto &newline : newlines) {
            if (std::regex_match(word, newline)) {
                return {.data   = word,
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::NEWLINE};
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

        for (auto &string_ : strings) {
            if (std::regex_match(word, string_)) {
                return {.data   = word,
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::STRING};
            }
        }

        for (auto &label : labels) {
            if (std::regex_match(word, label)) {
                return {.data   = word,
                        .file   = file,
                        .line   = line,
                        .column = col,
                        .type   = TokenType::LABEL};
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