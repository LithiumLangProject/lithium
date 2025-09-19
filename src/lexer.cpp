#include "lexar.hpp"
#include <unordered_map>

// Static keyword mapping for efficient lookup
static const std::unordered_map<std::string, TokenType> keywords = {
    {"fn", TokenType::FN},
    {"let", TokenType::LET},
    {"const", TokenType::CONST},
    {"include", TokenType::INCLUDE},
    {"from", TokenType::FROM},
    {"int", TokenType::INT_TYPE},
    {"float", TokenType::FLOAT_TYPE},
    {"string", TokenType::STRING_TYPE},
    {"bool", TokenType::BOOL_TYPE},
    {"any", TokenType::ANY_TYPE},
    {"void", TokenType::VOID_TYPE}
};

// Token utility functions implementation
Token Token::createKeyword(const std::string& keyword, const Position& pos) {
    auto it = keywords.find(keyword);
    if (it != keywords.end()) {
        return Token(it->second, keyword, pos);
    }
    // If not a keyword, treat as identifier
    return Token(TokenType::IDENTIFIER, keyword, pos);
}

Token Token::createIdentifier(const std::string& name, const Position& pos) {
    return Token(TokenType::IDENTIFIER, name, pos);
}

Token Token::createNumber(const std::string& value, const Position& pos) {
    return Token(TokenType::NUMBER, value, pos);
}

Token Token::createString(const std::string& value, const Position& pos) {
    return Token(TokenType::STRING, value, pos);
}

Token Token::createOperator(TokenType type, const std::string& op, const Position& pos) {
    return Token(type, op, pos);
}

Token Token::createDelimiter(TokenType type, const Position& pos) {
    std::string value;
    switch (type) {
        case TokenType::LPAREN: value = "("; break;
        case TokenType::RPAREN: value = ")"; break;
        case TokenType::LBRACE: value = "{"; break;
        case TokenType::RBRACE: value = "}"; break;
        case TokenType::COLON: value = ":"; break;
        case TokenType::COMMA: value = ","; break;
        default: value = ""; break;
    }
    return Token(type, value, pos);
}

Token Token::createNewline(const Position& pos) {
    return Token(TokenType::NEWLINE, "\\n", pos);
}

Token Token::createEOF(const Position& pos) {
    return Token(TokenType::EOF_TOKEN, "", pos);
}

Token Token::createComment(const std::string& comment, const Position& pos) {
    return Token(TokenType::COMMENT, comment, pos);
}

// Comparison operators
bool Token::operator==(const Token& other) const {
    return type == other.type && value == other.value;
}

bool Token::operator!=(const Token& other) const {
    return !(*this == other);
}

// Utility methods
bool Token::isKeyword() const {
    return isKeywordToken(type);
}

bool Token::isLiteral() const {
    return isLiteralToken(type);
}

bool Token::isOperator() const {
    return isOperatorToken(type);
}

bool Token::isDelimiter() const {
    return isDelimiterToken(type);
}

std::string Token::toString() const {
    return tokenTypeToString(type) + "(" + value + ") at " + 
           position.filename + ":" + std::to_string(position.line) + ":" + std::to_string(position.column);
}

// Global utility functions
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::STRING: return "STRING";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::FN: return "FN";
        case TokenType::LET: return "LET";
        case TokenType::CONST: return "CONST";
        case TokenType::INCLUDE: return "INCLUDE";
        case TokenType::FROM: return "FROM";
        case TokenType::INT_TYPE: return "INT_TYPE";
        case TokenType::FLOAT_TYPE: return "FLOAT_TYPE";
        case TokenType::STRING_TYPE: return "STRING_TYPE";
        case TokenType::BOOL_TYPE: return "BOOL_TYPE";
        case TokenType::ANY_TYPE: return "ANY_TYPE";
        case TokenType::VOID_TYPE: return "VOID_TYPE";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::ASSIGN: return "ASSIGN";
        case TokenType::ARROW: return "ARROW";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::COLON: return "COLON";
        case TokenType::COMMA: return "COMMA";
        case TokenType::NEWLINE: return "NEWLINE";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::COMMENT: return "COMMENT";
        default: return "UNKNOWN";
    }
}

bool isKeywordToken(TokenType type) {
    return type == TokenType::FN || type == TokenType::LET || type == TokenType::CONST ||
           type == TokenType::INCLUDE || type == TokenType::FROM ||
           type == TokenType::INT_TYPE || type == TokenType::FLOAT_TYPE ||
           type == TokenType::STRING_TYPE || type == TokenType::BOOL_TYPE ||
           type == TokenType::ANY_TYPE || type == TokenType::VOID_TYPE;
}

bool isLiteralToken(TokenType type) {
    return type == TokenType::NUMBER || type == TokenType::STRING || type == TokenType::IDENTIFIER;
}

bool isOperatorToken(TokenType type) {
    return type == TokenType::PLUS || type == TokenType::MINUS ||
           type == TokenType::MULTIPLY || type == TokenType::DIVIDE ||
           type == TokenType::ASSIGN || type == TokenType::ARROW;
}

bool isDelimiterToken(TokenType type) {
    return type == TokenType::LPAREN || type == TokenType::RPAREN ||
           type == TokenType::LBRACE || type == TokenType::RBRACE ||
           type == TokenType::COLON || type == TokenType::COMMA;
}

// Lexer implementation
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (!isAtEnd()) {
        // Skip whitespace but preserve newlines
        if (std::isspace(peek().value()) && peek().value() != '\n') {
            skipWhitespace();
            continue;
        }
        
        char c = peek().value();
        Position tokenPos = getCurrentPosition();
        
        // Handle newlines
        if (c == '\n') {
            tokens.push_back(Token::createNewline(tokenPos));
            consume();
            continue;
        }
        
        // Handle identifiers and keywords
        if (isAlpha(c)) {
            tokens.push_back(scanIdentifierOrKeyword());
            continue;
        }
        
        // Handle numbers
        if (isDigit(c)) {
            tokens.push_back(scanNumber());
            continue;
        }
        
        // Handle strings
        if (c == '"') {
            tokens.push_back(scanString());
            continue;
        }
        
        // Handle comments
        if (c == '/' && peekNext().has_value() && peekNext().value() == '/') {
            tokens.push_back(scanComment());
            continue;
        }
        
        // Handle operators
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || 
            (c == '-' && peekNext().has_value() && peekNext().value() == '>')) {
            tokens.push_back(scanOperator());
            continue;
        }
        
        // Handle delimiters
        if (c == '(' || c == ')' || c == '{' || c == '}' || c == ':' || c == ',') {
            tokens.push_back(scanDelimiter());
            continue;
        }
        
        // Unknown character
        std::cerr << "Error: Unexpected character '" << c << "' at " 
                  << m_filename << ":" << m_line << ":" << m_column << std::endl;
        consume(); // Skip unknown character
    }
    
    tokens.push_back(Token::createEOF(getCurrentPosition()));
    return tokens;
}

std::optional<char> Lexer::peek(const size_t offset) const {
    if (m_idx + offset >= m_src.length()) {
        return {};
    }
    return m_src.at(m_idx + offset);
}

char Lexer::consume() {
    if (isAtEnd()) return '\0';
    
    char c = m_src.at(m_idx++);
    updatePosition(c);
    return c;
}

void Lexer::skipWhitespace() {
    while (peek().has_value() && std::isspace(peek().value()) && peek().value() != '\n') {
        consume();
    }
}

Token Lexer::scanIdentifierOrKeyword() {
    Position startPos = getCurrentPosition();
    std::string value;
    
    // First character must be alpha
    value += consume();
    
    // Continue with alphanumeric characters
    while (peek().has_value() && isAlphaNumeric(peek().value())) {
        value += consume();
    }
    
    return Token::createKeyword(value, startPos);
}

Token Lexer::scanNumber() {
    Position startPos = getCurrentPosition();
    std::string value;
    
    // Scan integer part
    while (peek().has_value() && isDigit(peek().value())) {
        value += consume();
    }
    
    // Check for decimal point
    if (peek().has_value() && peek().value() == '.' && 
        peekNext().has_value() && isDigit(peekNext().value())) {
        value += consume(); // consume '.'
        
        // Scan fractional part
        while (peek().has_value() && isDigit(peek().value())) {
            value += consume();
        }
    }
    
    return Token::createNumber(value, startPos);
}

Token Lexer::scanString() {
    Position startPos = getCurrentPosition();
    std::string value;
    
    consume(); // consume opening quote
    
    while (peek().has_value() && peek().value() != '"') {
        char c = peek().value();
        
        // Handle escape sequences
        if (c == '\\' && peekNext().has_value()) {
            consume(); // consume backslash
            char escaped = consume();
            
            switch (escaped) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                default: 
                    value += '\\';
                    value += escaped;
                    break;
            }
        } else {
            value += consume();
        }
    }
    
    if (!peek().has_value()) {
        std::cerr << "Error: Unterminated string literal at " 
                  << m_filename << ":" << startPos.line << ":" << startPos.column << std::endl;
        exit(EXIT_FAILURE);
    }
    
    consume(); // consume closing quote
    return Token::createString(value, startPos);
}

Token Lexer::scanOperator() {
    Position startPos = getCurrentPosition();
    char c = consume();
    
    switch (c) {
        case '+': return Token::createOperator(TokenType::PLUS, "+", startPos);
        case '*': return Token::createOperator(TokenType::MULTIPLY, "*", startPos);
        case '/': return Token::createOperator(TokenType::DIVIDE, "/", startPos);
        case '=': return Token::createOperator(TokenType::ASSIGN, "=", startPos);
        case '-':
            if (peek().has_value() && peek().value() == '>') {
                consume(); // consume '>'
                return Token::createOperator(TokenType::ARROW, "->", startPos);
            }
            return Token::createOperator(TokenType::MINUS, "-", startPos);
        default:
            std::cerr << "Error: Unknown operator '" << c << "' at " 
                      << m_filename << ":" << m_line << ":" << m_column << std::endl;
            return Token::createOperator(TokenType::ASSIGN, std::string(1, c), startPos);
    }
}

Token Lexer::scanDelimiter() {
    Position startPos = getCurrentPosition();
    char c = consume();
    
    switch (c) {
        case '(': return Token::createDelimiter(TokenType::LPAREN, startPos);
        case ')': return Token::createDelimiter(TokenType::RPAREN, startPos);
        case '{': return Token::createDelimiter(TokenType::LBRACE, startPos);
        case '}': return Token::createDelimiter(TokenType::RBRACE, startPos);
        case ':': return Token::createDelimiter(TokenType::COLON, startPos);
        case ',': return Token::createDelimiter(TokenType::COMMA, startPos);
        default:
            std::cerr << "Error: Unknown delimiter '" << c << "' at " 
                      << m_filename << ":" << m_line << ":" << m_column << std::endl;
            return Token::createDelimiter(TokenType::COMMA, startPos);
    }
}

Token Lexer::scanComment() {
    Position startPos = getCurrentPosition();
    std::string value;
    
    consume(); // consume first '/'
    consume(); // consume second '/'
    
    // Read until end of line
    while (peek().has_value() && peek().value() != '\n') {
        value += consume();
    }
    
    return Token::createComment(value, startPos);
}

Position Lexer::getCurrentPosition() const {
    return Position(m_filename, m_line, m_column);
}

void Lexer::updatePosition(char c) {
    if (c == '\n') {
        m_line++;
        m_column = 1;
    } else {
        m_column++;
    }
}

bool Lexer::isAtEnd() const {
    return m_idx >= m_src.length();
}

bool Lexer::isAlpha(char c) const {
    return std::isalpha(c) || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::isDigit(char c) const {
    return std::isdigit(c);
}