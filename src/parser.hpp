#pragma once

#include <memory>
#include <vector>
#include "lexar.hpp"
#include "ast.hpp"
#include "error.hpp"

class Parser {
private:
    std::vector<Token> tokens;
    size_t currentToken;
    ErrorReporter& errorReporter;
    
public:
    Parser(std::vector<Token> tokenList, ErrorReporter& reporter);
    
    std::unique_ptr<ProgramNode> parseProgram();
    
private:
    const Token& peek(size_t offset = 0) const;
    const Token& consume();
    bool match(TokenType type);
    bool check(TokenType type) const;
    bool isAtEnd() const;
    
    void synchronize();
    void reportError(const std::string& message);
    
    std::unique_ptr<ASTNode> parseTopLevelDeclaration();
    std::unique_ptr<FunctionDecl> parseFunction();
    std::unique_ptr<VarDecl> parseVarDecl();
    std::unique_ptr<IncludeDirective> parseInclude();
    std::unique_ptr<ImportStatement> parseImport();
    std::unique_ptr<SelectiveImport> parseSelectiveImport();
    
    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<Expression> parseBinaryOp(int minPrec = 0);
    std::unique_ptr<Expression> parsePrimary();
    std::unique_ptr<Expression> parseFunctionCall();
    std::unique_ptr<Expression> parseIdentifier();
    std::unique_ptr<Expression> parseLiteral();
    
    std::unique_ptr<Statement> parseStatement();
    
    std::vector<Parameter> parseParameterList();
    std::vector<std::unique_ptr<Expression>> parseArgumentList();
    std::string parseType();
    
    int getOperatorPrecedence(TokenType type) const;
    bool isRightAssociative(TokenType type) const;
};