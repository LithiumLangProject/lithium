#include "error.hpp"
#include <iostream>

std::string Error::toString() const {
    return position.filename + ":" + std::to_string(position.line) + ":" + 
           std::to_string(position.column) + ": " + getSeverityString() + ": " + message;
}

std::string Error::getSeverityString() const {
    switch (severity) {
        case ErrorSeverity::WARNING: return "warning";
        case ErrorSeverity::ERROR: return "error";
        case ErrorSeverity::FATAL: return "fatal error";
        default: return "unknown";
    }
}

std::string Error::getCategoryString() const {
    switch (category) {
        case ErrorCategory::LEXICAL: return "lexical";
        case ErrorCategory::SYNTAX: return "syntax";
        case ErrorCategory::SEMANTIC: return "semantic";
        case ErrorCategory::TYPE: return "type";
        case ErrorCategory::FILE_IO: return "file I/O";
        default: return "unknown";
    }
}

void ErrorReporter::reportError(ErrorSeverity severity, ErrorCategory category, 
                               const Position& position, const std::string& message,
                               const std::string& context) {
    errors.emplace_back(severity, category, position, message, context);
    hasErrors = true;
    if (severity == ErrorSeverity::FATAL) {
        hasFatalErrors = true;
    }
}

void ErrorReporter::reportLexicalError(const Position& position, const std::string& message) {
    reportError(ErrorSeverity::ERROR, ErrorCategory::LEXICAL, position, message);
}

void ErrorReporter::reportSyntaxError(const Position& position, const std::string& message) {
    reportError(ErrorSeverity::ERROR, ErrorCategory::SYNTAX, position, message);
}

void ErrorReporter::reportSemanticError(const Position& position, const std::string& message) {
    reportError(ErrorSeverity::ERROR, ErrorCategory::SEMANTIC, position, message);
}

void ErrorReporter::reportTypeError(const Position& position, const std::string& message) {
    reportError(ErrorSeverity::ERROR, ErrorCategory::TYPE, position, message);
}

void ErrorReporter::reportFileError(const Position& position, const std::string& message) {
    reportError(ErrorSeverity::ERROR, ErrorCategory::FILE_IO, position, message);
}

void ErrorReporter::clearErrors() {
    errors.clear();
    hasErrors = false;
    hasFatalErrors = false;
}

void ErrorReporter::printErrors() const {
    for (const auto& error : errors) {
        printError(error);
    }
}

void ErrorReporter::printError(const Error& error) const {
    std::cerr << error.toString() << std::endl;
    if (!error.context.empty()) {
        std::cerr << "  " << error.context << std::endl;
    }
}