#pragma once

#include "Ast.hpp"

struct AstVisitor
{
    virtual void visit(StatementNode*) { }
    virtual void visit(ExpressionNode*) { }
    virtual void visit(IntegerNode*) { }
    virtual void visit(IntVarFactor*) { }
    virtual void visit(OneDimensionalListFactor*) { }
    virtual void visit(BinaryOpNode*) {  }
    virtual void visit(UnaryOpNode*) { }
    virtual void visit(IntLValueNode*) { }
    virtual void visit(OneDimensionalListLValueNode*) { }
    virtual void visit(LetStatementNode*) { }
    virtual void visit(CodeBlockNode*) { }
    virtual void visit(ForLoopNode*) { }
    virtual void visit(LabelNode*) { }
    virtual void visit(GotoNode*) { }
    virtual void visit(WhileLoopNode*) { }
    virtual void visit(IfNode*) { }
    virtual void visit(PrintNode*) { }
    virtual void visit(PromptNode*) { }
    virtual void visit(InputNode*) { }
    virtual void visit(EndNode*) { }
    
    virtual void visitVars(std::vector<VarDeclNode*>&) { }
};
