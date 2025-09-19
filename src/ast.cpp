#include "ast.hpp"

// todo: non work
void ProgramNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void FunctionDecl::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void VarDecl::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void BinaryOp::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void FunctionCall::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void Identifier::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void NumberLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void StringLiteral::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void IncludeDirective::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void ImportStatement::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

void SelectiveImport::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}