#include "iterator.hh"
#include <optional>

namespace assembler {
namespace iterator {
/// gets teh token while adnvaceing the index
std::optional<lexer::Token> TokenIterator::get(ssize_t n) const {
    if ((index + n < 0) || (index + n >= max_size)) {
        return std::nullopt;
    }

    index += n+1;
    return *(iter + index);
}

std::optional<lexer::Token> TokenIterator::peek(ssize_t n) const {
    if ((index + n < 0) || (index + n >= max_size)) {
        return std::nullopt;
    }

    return *(iter + index + n);
}
} // namespace iterator
} // namespace assembler
