#pragma once

#include <cctype>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include "ast.hpp"

enum class TokenType {
    // Literals
    NUMBER,
    STRING,
    IDENTIFIER,
    
    // Keywords
    FN,
    LET,
    CONST,
    INCLUDE,
    FROM,
    
    // Types
    INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE,
    BOOL_TYPE,
    ANY_TYPE,
    VOID_TYPE,
    
    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    ASSIGN,
    ARROW,
    
    // Delimiters
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COLON,
    COMMA,
    
    // Special
    NEWLINE,
    EOF_TOKEN,
    COMMENT
};

struct Token {
    TokenType type;
    std::string value;
    Position position;
    
    Token(TokenType t, std::string v = "", Position p = Position()) 
        : type(t), value(std::move(v)), position(p) {}
    
    // Utility functions for token creation and comparison
    static Token createKeyword(const std::string& keyword, const Position& pos);
    static Token createIdentifier(const std::string& name, const Position& pos);
    static Token createNumber(const std::string& value, const Position& pos);
    static Token createString(const std::string& value, const Position& pos);
    static Token createOperator(TokenType type, const std::string& op, const Position& pos);
    static Token createDelimiter(TokenType type, const Position& pos);
    static Token createNewline(const Position& pos);
    static Token createEOF(const Position& pos);
    static Token createComment(const std::string& comment, const Position& pos);
    
    // Comparison operators
    bool operator==(const Token& other) const;
    bool operator!=(const Token& other) const;
    
    // Utility methods
    bool isKeyword() const;
    bool isLiteral() const;
    bool isOperator() const;
    bool isDelimiter() const;
    std::string toString() const;
};

// Utility functions for token type checking
std::string tokenTypeToString(TokenType type);
bool isKeywordToken(TokenType type);
bool isLiteralToken(TokenType type);
bool isOperatorToken(TokenType type);
bool isDelimiterToken(TokenType type);

class Lexer {
public:
    Lexer(std::string source, std::string filename = "") 
        : m_src(std::move(source)), m_filename(std::move(filename)), m_line(1), m_column(1) {}

    std::vector<Token> tokenize();

private:
    // Character scanning methods with lookahead
    [[nodiscard]] std::optional<char> peek(const size_t offset = 0) const;
    [[nodiscard]] std::optional<char> peekNext() const { return peek(1); }
    char consume();
    void skipWhitespace();
    
    // Token scanning methods
    Token scanIdentifierOrKeyword();
    Token scanNumber();
    Token scanString();
    Token scanOperator();
    Token scanDelimiter();
    Token scanComment();
    
    // Position tracking
    Position getCurrentPosition() const;
    void updatePosition(char c);
    
    // Helper methods
    bool isAtEnd() const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    bool isDigit(char c) const;
    
    const std::string m_src;
    std::string m_filename;
    size_t m_idx = 0;
    int m_line;
    int m_column;
};
