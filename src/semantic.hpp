#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "ast.hpp"
#include "types.hpp"
#include "error.hpp"

class Symbol {
public:
    std::string name;
    std::unique_ptr<Type> type;
    bool isConst;
    Position declarationPos;
    
    Symbol(std::string n, std::unique_ptr<Type> t, bool constant, Position pos)
        : name(std::move(n)), type(std::move(t)), isConst(constant), declarationPos(pos) {}
};

class SymbolTable {
private:
    std::vector<std::unordered_map<std::string, std::unique_ptr<Symbol>>> scopes;
    
public:
    SymbolTable();
    
    void enterScope();
    void exitScope();
    
    bool declareSymbol(const std::string& name, std::unique_ptr<Type> type, 
                      bool isConst, const Position& position);
    
    Symbol* lookupSymbol(const std::string& name);
    bool isSymbolInCurrentScope(const std::string& name);
    
    size_t getCurrentScopeLevel() const { return scopes.size(); }
};

class TypeChecker {
private:
    ErrorReporter& errorReporter;
    
public:
    explicit TypeChecker(ErrorReporter& reporter) : errorReporter(reporter) {}
    
    std::unique_ptr<Type> inferType(Expression* expr, SymbolTable& symbolTable);
    bool checkTypeCompatibility(const Type& expected, const Type& actual, const Position& position);
    std::unique_ptr<Type> checkBinaryOperation(const std::string& op, const Type& left, const Type& right, const Position& position);
    bool validateFunctionCall(const std::string& functionName, const std::vector<std::unique_ptr<Expression>>& args, 
                             SymbolTable& symbolTable, const Position& position);
};

class SemanticAnalyzer : public ASTVisitor {
private:
    SymbolTable symbolTable;
    TypeChecker typeChecker;
    ErrorReporter& errorReporter;
    std::vector<std::unique_ptr<Type>> functionReturnTypes;
    
public:
    explicit SemanticAnalyzer(ErrorReporter& reporter);
    
    bool analyze(ProgramNode* program);
    
    void visit(ProgramNode& node) override;
    void visit(FunctionDecl& node) override;
    void visit(VarDecl& node) override;
    void visit(BinaryOp& node) override;
    void visit(FunctionCall& node) override;
    void visit(Identifier& node) override;
    void visit(NumberLiteral& node) override;
    void visit(StringLiteral& node) override;
    void visit(IncludeDirective& node) override;
    void visit(ImportStatement& node) override;
    void visit(SelectiveImport& node) override;
    
private:
    void validateIncludeFile(const std::string& filename, const Position& position);
    std::unique_ptr<Type> parseTypeString(const std::string& typeStr);
};