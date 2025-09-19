#pragma once

#include <string>
#include <vector>
#include "ast.hpp"

enum class ErrorSeverity {
    WARNING,
    ERROR,
    FATAL
};

enum class ErrorCategory {
    LEXICAL,
    SYNTAX,
    SEMANTIC,
    TYPE,
    FILE_IO
};

class Error {
public:
    ErrorSeverity severity;
    ErrorCategory category;
    Position position;
    std::string message;
    std::string context;
    
    Error(ErrorSeverity sev, ErrorCategory cat, Position pos, 
          std::string msg, std::string ctx = "")
        : severity(sev), category(cat), position(pos), 
          message(std::move(msg)), context(std::move(ctx)) {}
    
    std::string toString() const;
    std::string getSeverityString() const;
    std::string getCategoryString() const;
};

class ErrorReporter {
private:
    std::vector<Error> errors;
    bool hasErrors;
    bool hasFatalErrors;
    
public:
    ErrorReporter() : hasErrors(false), hasFatalErrors(false) {}
    
    void reportError(ErrorSeverity severity, ErrorCategory category, 
                    const Position& position, const std::string& message,
                    const std::string& context = "");
    
    void reportLexicalError(const Position& position, const std::string& message);
    void reportSyntaxError(const Position& position, const std::string& message);
    void reportSemanticError(const Position& position, const std::string& message);
    void reportTypeError(const Position& position, const std::string& message);
    void reportFileError(const Position& position, const std::string& message);
    
    bool hasAnyErrors() const { return hasErrors; }
    bool hasFatalError() const { return hasFatalErrors; }
    
    const std::vector<Error>& getErrors() const { return errors; }
    void clearErrors();
    
    void printErrors() const;
    void printError(const Error& error) const;
};