#include "scanner.hpp"

#include <stdexcept>
#include <string>

std::string stripComments(const std::string& source) {
    enum class State { Code, String, LineComment, BlockComment };
    State state = State::Code;
    std::string result = source;
    std::size_t line = 1;
    std::size_t column = 1;
    std::size_t commentLine = 1;
    std::size_t commentColumn = 1;
    bool escaped = false;

    for (std::size_t index = 0; index < source.size(); ++index) {
        const char current = source[index];
        const char next = index + 1 < source.size() ? source[index + 1] : '\0';

        if (state == State::Code) {
            if (current == '"') {
                state = State::String;
                escaped = false;
            } else if (current == '/' && (next == '/' || next == '*')) {
                state = next == '/' ? State::LineComment : State::BlockComment;
                commentLine = line;
                commentColumn = column;
                result[index] = ' ';
                result[index + 1] = ' ';
                ++index;
                ++column;
            }
        } else if (state == State::String) {
            if (escaped) {
                escaped = false;
            } else if (current == '\\') {
                escaped = true;
            } else if (current == '"') {
                state = State::Code;
            }
        } else if (state == State::LineComment) {
            if (current == '\n') {
                state = State::Code;
            } else if (current != '\r') {
                result[index] = ' ';
            }
        } else {
            if (current == '*' && next == '/') {
                result[index] = ' ';
                result[index + 1] = ' ';
                ++index;
                ++column;
                state = State::Code;
            } else if (current != '\n' && current != '\r') {
                result[index] = ' ';
            }
        }

        if (current == '\n') {
            ++line;
            column = 1;
        } else {
            ++column;
        }
    }

    if (state == State::BlockComment) {
        throw std::runtime_error("Unterminated block comment at " +
                                 std::to_string(commentLine) + ':' +
                                 std::to_string(commentColumn));
    }
    return result;
}
