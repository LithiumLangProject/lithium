#include "codegen.hpp"
#include <fstream>

std::string Instruction::toString() const {
    std::string result = opcode;
    for (const auto& operand : operands) {
        result += " " + operand;
    }
    if (!comment.empty()) {
        result += " ; " + comment;
    }
    return result;
}

std::string CodeGenContext::generateLabel(const std::string& prefix) {
    return prefix + std::to_string(labelCounter++);
}

std::string CodeGenContext::generateTemp(const std::string& prefix) {
    return prefix + std::to_string(tempCounter++);
}

void CodeGenContext::emitInstruction(const std::string& opcode, 
                                   const std::vector<std::string>& operands,
                                   const std::string& comment) {
    instructions.emplace_back(opcode, operands, comment);
}

void CodeGenContext::emitLabel(const std::string& label) {
    instructions.emplace_back(label + ":", std::vector<std::string>(), "");
}

void CodeGenContext::emitComment(const std::string& comment) {
    instructions.emplace_back("", std::vector<std::string>(), comment);
}

// CodeGenerator implementation
CodeGenerator::CodeGenerator(Target tgt, ErrorReporter& reporter) 
    : target(std::move(tgt)), errorReporter(reporter) {}

bool CodeGenerator::generate(ProgramNode* program, const std::string& outputFile) {
    // TODO: Implement actual code generation
    if (program) {
        program->accept(*this);
    }
    
    return writeOutputFile(outputFile);
}

void CodeGenerator::visit(ProgramNode& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(FunctionDecl& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(VarDecl& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(BinaryOp& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(FunctionCall& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(Identifier& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(NumberLiteral& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(StringLiteral& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(IncludeDirective& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(ImportStatement& node) {
    // TODO: Implement 
}

void CodeGenerator::visit(SelectiveImport& node) {
    // TODO: Implement 
}

bool CodeGenerator::writeOutputFile(const std::string& outputFile) {
    std::ofstream file(outputFile);
    if (!file.is_open()) {
        errorReporter.reportFileError(Position(), "Could not open output file: " + outputFile);
        return false;
    }
    
    file << "# TODO: Implement actual code generation\n";
    
    return true;
}