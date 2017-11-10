#pragma once

#include <vector>
#include <cstdio>

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
    virtual void visit(BasicBlockNode*) { }
    virtual void visit(ForLoopNode*) { }
    virtual void visit(LabelNode*) { }
    virtual void visit(GotoNode*) { }
    virtual void visit(WhileLoopNode*) { }
    virtual void visit(IfNode*) { }
    virtual void visit(PrintNode*) { }
    virtual void visit(PromptNode*) { }
    virtual void visit(InputNode*) { }
    virtual void visit(EndNode*) { }
    virtual void visit(RemNode*) { }
    virtual void visit(FactorNode*) { }
    virtual void visit(PhiNode*) { }
    
    virtual void visitVars(std::vector<VarDeclNode*>&) { }
    
    virtual void enterNode(AstNode* node) { nodeStack.push_back(node); }
    virtual void exitNode(AstNode* node) { nodeStack.pop_back(); }
    virtual AstNode* lastNode() { return *nodeStack.rbegin(); }
    virtual void replaceNode(AstNode* node) { nodeStack[nodeStack.size() - 1] = node; }
    
    std::vector<AstNode*> nodeStack;
    
    virtual ~AstVisitor()
    {
        if(nodeStack.size() != 0)
            fprintf(stderr, "Node stack has non-zero size\n");
    }
};
