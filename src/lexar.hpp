#pragma once

#include <cctype>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>
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
};

class Lexer {
public:
    Lexer(std::string source, std::string filename = "") 
        : m_src(std::move(source)), m_filename(std::move(filename)) {}

    std::vector<Token> tokenize() {
    std::vector<Token> tokens;
    int lineCount = 1;
    std::string buf;

    while (peek().has_value()) {
      const char c = peek().value();
      if (std::isalpha(c)) {
        buf.push_back(consume());

        while (peek().has_value() && std::isalnum(peek().value())) {
          buf.push_back(consume());
        }

        if (buf == "fn") {
          tokens.push_back(Token(TokenType::FN, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "let") {
          tokens.push_back(Token(TokenType::LET, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "const") {
          tokens.push_back(Token(TokenType::CONST, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "include") {
          tokens.push_back(Token(TokenType::INCLUDE, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "from") {
          tokens.push_back(Token(TokenType::FROM, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "int") {
          tokens.push_back(Token(TokenType::INT_TYPE, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "float") {
          tokens.push_back(Token(TokenType::FLOAT_TYPE, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "string") {
          tokens.push_back(Token(TokenType::STRING_TYPE, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "bool") {
          tokens.push_back(Token(TokenType::BOOL_TYPE, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "any") {
          tokens.push_back(Token(TokenType::ANY_TYPE, buf, getCurrentPosition()));
          buf.clear();
        } else if (buf == "void") {
          tokens.push_back(Token(TokenType::VOID_TYPE, buf, getCurrentPosition()));
          buf.clear();
        } else {
          tokens.push_back(Token(TokenType::IDENTIFIER, buf, getCurrentPosition()));
          buf.clear();
        }
      } else if (std::isdigit(c)) {
        buf.push_back(consume());
        while (peek().has_value() && std::isdigit(peek().value())) {
          buf.push_back(consume());
        }
        tokens.push_back(Token(TokenType::NUMBER, buf, getCurrentPosition()));
        buf.clear();
      } else if (c == '\n') {
        tokens.push_back(Token(TokenType::NEWLINE, "\\n", getCurrentPosition()));
        lineCount++;
        consume();
      } else if (std::isspace(c)) {
        consume();
      } else if (c == '"') {
        consume();
        while (peek().has_value() && peek().value() != '"') {
          buf.push_back(consume());
        }
        if (peek().has_value()) {
          consume();
          tokens.push_back(Token(TokenType::STRING, buf, getCurrentPosition()));
          buf.clear();
        } else {
          std::cerr << "Error: Unterminated string literal.\n";
          exit(EXIT_FAILURE);
        }
      } else {
        TokenType type;

        consume();

        switch (c) {
        case '(':
          type = TokenType::LPAREN;
          break;
        case ')':
          type = TokenType::RPAREN;
          break;
        case '{':
          type = TokenType::LBRACE;
          break;
        case '}':
          type = TokenType::RBRACE;
          break;
        case ':':
          type = TokenType::COLON;
          break;
        case ',':
          type = TokenType::COMMA;
          break;
        case '=':
          type = TokenType::ASSIGN;
          break;
        case '+':
          type = TokenType::PLUS;
          break;
        case '*':
          type = TokenType::MULTIPLY;
          break;
        case '/':
          type = TokenType::DIVIDE;
          break;
        case '-': // if -> return arrow else js -
          if (peek().has_value() && peek().value() == '>') {
            consume(); 
            type = TokenType::ARROW;
          } else {
            type = TokenType::MINUS;
          }
          break;
        default:
          std::cerr << "Error: Line " << lineCount << " Unexpected token '" << c
                    << "'" << std::endl;
          exit(EXIT_FAILURE);
        }
        tokens.push_back(Token(type, std::string(1, c), getCurrentPosition()));
      }
    }

    tokens.push_back(Token(TokenType::EOF_TOKEN, "", getCurrentPosition()));
    
    m_idx = 0;
    return tokens;
  }

private:
  [[nodiscard]] std::optional<char> peek(const size_t offset = 0) const {
    if (m_idx + offset >= m_src.length()) {
      return {};
    }
    return m_src.at(m_idx + offset);
  }

  char consume() { return m_src.at(m_idx++); }

    const std::string m_src;
    std::string m_filename;
    int m_idx = 0;
    
    Position getCurrentPosition() const {
        int line = 1;
        int column = 1;
        for (int i = 0; i < m_idx && i < static_cast<int>(m_src.length()); ++i) {
            if (m_src[i] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
        }
        return Position(m_filename, line, column);
    }
};
