#pragma once

#include <memory>
#include <string>
#include <vector>

class ASTVisitor;

struct Position {
    std::string filename;
    int line;
    int column;
    
    Position(std::string file = "", int l = 1, int c = 1) 
        : filename(std::move(file)), line(l), column(c) {}
};

class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
    
    Position getPosition() const { return position; }
    void setPosition(const Position& pos) { position = pos; }
    
protected:
    Position position;
};

class Expression : public ASTNode {
public:
    virtual ~Expression() = default;
};

class Statement : public ASTNode {
public:
    virtual ~Statement() = default;
};

class ProgramNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> declarations;
    
    void accept(ASTVisitor& visitor) override;
};

class Parameter {
public:
    std::string name;
    std::string type;
    Position position;
    
    Parameter(std::string n, std::string t, Position pos = Position()) 
        : name(std::move(n)), type(std::move(t)), position(pos) {}
};

class FunctionDecl : public ASTNode {
public:
    std::string name;
    std::vector<Parameter> parameters;
    std::string returnType;
    std::unique_ptr<Expression> body;
    
    void accept(ASTVisitor& visitor) override;
};

class VarDecl : public Statement {
public:
    std::string name;
    std::string declaredType;
    std::unique_ptr<Expression> initializer;
    bool isConst;
    
    VarDecl(std::string n, bool constant = false) 
        : name(std::move(n)), isConst(constant) {}
    
    void accept(ASTVisitor& visitor) override;
};

class BinaryOp : public Expression {
public:
    std::unique_ptr<Expression> left;
    std::string operator_;
    std::unique_ptr<Expression> right;
    
    void accept(ASTVisitor& visitor) override;
};

class FunctionCall : public Expression {
public:
    std::string functionName;
    std::vector<std::unique_ptr<Expression>> arguments;
    
    void accept(ASTVisitor& visitor) override;
};

class Identifier : public Expression {
public:
    std::string name;
    
    explicit Identifier(std::string n) : name(std::move(n)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class NumberLiteral : public Expression {
public:
    std::string value;
    bool isFloat;
    
    NumberLiteral(std::string v, bool isF = false) 
        : value(std::move(v)), isFloat(isF) {}
    
    void accept(ASTVisitor& visitor) override;
};

class StringLiteral : public Expression {
public:
    std::string value;
    
    explicit StringLiteral(std::string v) : value(std::move(v)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class IncludeDirective : public ASTNode {
public:
    std::string filename;
    
    explicit IncludeDirective(std::string file) : filename(std::move(file)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class ImportStatement : public ASTNode {
public:
    std::string moduleName;
    std::string importedName;
    
    ImportStatement(std::string module, std::string imported) 
        : moduleName(std::move(module)), importedName(std::move(imported)) {}
    
    void accept(ASTVisitor& visitor) override;
};

class SelectiveImport : public ASTNode {
public:
    std::vector<std::string> importedNames;
    std::string moduleName;
    
    void accept(ASTVisitor& visitor) override;
};

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(FunctionDecl& node) = 0;
    virtual void visit(VarDecl& node) = 0;
    virtual void visit(BinaryOp& node) = 0;
    virtual void visit(FunctionCall& node) = 0;
    virtual void visit(Identifier& node) = 0;
    virtual void visit(NumberLiteral& node) = 0;
    virtual void visit(StringLiteral& node) = 0;
    virtual void visit(IncludeDirective& node) = 0;
    virtual void visit(ImportStatement& node) = 0;
    virtual void visit(SelectiveImport& node) = 0;
};