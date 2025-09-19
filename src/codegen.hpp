#pragma once

#include <string>
#include <vector>
#include <memory>
#include "ast.hpp"
#include "error.hpp"

// donno if im doing iR
enum class TargetType {
    EXECUTABLE,
    INTERMEDIATE,
    ASSEMBLY
};

class Target {
public:
    TargetType type;
    std::string outputPath;
    
    Target(TargetType t, std::string path) : type(t), outputPath(std::move(path)) {}
};

class Instruction {
public:
    std::string opcode;
    std::vector<std::string> operands;
    std::string comment;
    
    Instruction(std::string op, std::vector<std::string> ops = {}, std::string cmt = "")
        : opcode(std::move(op)), operands(std::move(ops)), comment(std::move(cmt)) {}
    
    std::string toString() const;
};

class CodeGenContext {
public:
    std::vector<Instruction> instructions;
    std::vector<std::string> dataSection;
    std::vector<std::string> textSection;
    int labelCounter;
    int tempCounter;
    
    CodeGenContext() : labelCounter(0), tempCounter(0) {}
    
    std::string generateLabel(const std::string& prefix = "L");
    std::string generateTemp(const std::string& prefix = "t");
    
    void emitInstruction(const std::string& opcode, 
                        const std::vector<std::string>& operands = {},
                        const std::string& comment = "");
    
    void emitLabel(const std::string& label);
    void emitComment(const std::string& comment);
};

class CodeGenerator : public ASTVisitor {
private:
    Target target;
    CodeGenContext context;
    ErrorReporter& errorReporter;
    std::string currentFunction;
    
public:
    CodeGenerator(Target tgt, ErrorReporter& reporter);
    
    bool generate(ProgramNode* program, const std::string& outputFile);
    
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
    void generatePrologue();
    void generateEpilogue();
    void generateFunctionPrologue(const std::string& functionName);
    void generateFunctionEpilogue(const std::string& functionName);
    
    bool writeOutputFile(const std::string& outputFile);
    std::string generateExecutable();
    std::string generateIntermediate();
    std::string generateAssembly();
};