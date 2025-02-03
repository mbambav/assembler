#ifndef ITERATOR_H
#define ITERATOR_H

#include <cstddef>
#include <optional>
#include "../lexer/lexer.hh"
namespace assembler {
namespace iterator {
class TokenIterator{
    private:
        lexer::TokenList::const_iterator iter;
        mutable size_t index;
        size_t max_size;
    public:
        TokenIterator(const lexer::TokenList& tokens) : iter(tokens.begin()), index(0), max_size(tokens.size()) {}
        std::optional<lexer::Token> get(ssize_t n = 0) const;
        std::optional<lexer::Token> peek(ssize_t n = 1) const;
        
        
};
} // namespace parser
} // namespace assembler

#endif