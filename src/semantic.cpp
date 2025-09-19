#include "semantic.hpp"

SymbolTable::SymbolTable() {
    enterScope();
}

void SymbolTable::enterScope() {
    scopes.emplace_back();
}

void SymbolTable::exitScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

bool SymbolTable::declareSymbol(const std::string& name, std::unique_ptr<Type> type, 
                               bool isConst, const Position& position) {
    if (scopes.empty()) return false;
    
    auto& currentScope = scopes.back();
    if (currentScope.find(name) != currentScope.end()) {
        return false;
    }
    
    currentScope[name] = std::make_unique<Symbol>(name, std::move(type), isConst, position);
    return true;
}

Symbol* SymbolTable::lookupSymbol(const std::string& name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return found->second.get();
        }
    }
    return nullptr;
}

bool SymbolTable::isSymbolInCurrentScope(const std::string& name) {
    if (scopes.empty()) return false;
    return scopes.back().find(name) != scopes.back().end();
}

SemanticAnalyzer::SemanticAnalyzer(ErrorReporter& reporter) 
    : typeChecker(reporter), errorReporter(reporter) {}

bool SemanticAnalyzer::analyze(ProgramNode* program) {
    // TODO: Implement actual semantic analysis
    if (program) {
        program->accept(*this);
    }
    return !errorReporter.hasAnyErrors();
}

void SemanticAnalyzer::visit(ProgramNode& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(FunctionDecl& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(VarDecl& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(BinaryOp& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(FunctionCall& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(Identifier& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(NumberLiteral& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(StringLiteral& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(IncludeDirective& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(ImportStatement& node) {
    // TODO: Implement 
}

void SemanticAnalyzer::visit(SelectiveImport& node) {
    // TODO: Implement 
}