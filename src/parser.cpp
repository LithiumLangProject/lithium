#include "parser.hpp"

Parser::Parser(std::vector<Token> tokenList, ErrorReporter& reporter) 
    : tokens(std::move(tokenList)), currentToken(0), errorReporter(reporter) {}

std::unique_ptr<ProgramNode> Parser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();
    
    // TODO: Implement
    
    return program;
}

const Token& Parser::peek(size_t offset) const {
    size_t index = currentToken + offset;
    if (index >= tokens.size()) {
        return tokens.back(); 
    }
    return tokens[index];
}

const Token& Parser::consume() {
    if (currentToken < tokens.size()) {
        return tokens[currentToken++];
    }
    return tokens.back(); 
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        consume();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::EOF_TOKEN;
}

void Parser::synchronize() {
    while (!isAtEnd()) {
        if (peek().type == TokenType::NEWLINE) {
            consume();
            return;
        }
        consume();
    }
}

void Parser::reportError(const std::string& message) {
    errorReporter.reportSyntaxError(peek().position, message);
}