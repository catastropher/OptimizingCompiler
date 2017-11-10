#pragma once

#include <vector>
#include <cstdio>

#include "Ast.hpp"

struct AstVisitor
{
    virtual void visit(StatementNode* node) { }
    virtual void visit(ExpressionNode* node) { }
    virtual void visit(IntegerNode* node) { visit((ExpressionNode*)node); }
    virtual void visit(IntVarFactor* node) { visit((ExpressionNode*)node); }
    virtual void visit(OneDimensionalListFactor* node) { }
    virtual void visit(BinaryOpNode* node) { visit((ExpressionNode*)node); }
    virtual void visit(UnaryOpNode* node) { visit((ExpressionNode*)node); }
    virtual void visit(IntLValueNode* node) { }
    virtual void visit(OneDimensionalListLValueNode* node) { }
    virtual void visit(LetStatementNode* node) { }
    virtual void visit(CodeBlockNode* node) { }
    virtual void visit(BasicBlockNode* node) { }
    virtual void visit(ForLoopNode* node) { }
    virtual void visit(LabelNode* node) { }
    virtual void visit(GotoNode* node) { }
    virtual void visit(WhileLoopNode* node) { }
    virtual void visit(IfNode* node) { }
    virtual void visit(PrintNode* node) { }
    virtual void visit(PromptNode* node) { }
    virtual void visit(InputNode* node) { }
    virtual void visit(EndNode* node) { }
    virtual void visit(RemNode* node) { }
    virtual void visit(FactorNode* node) { }
    virtual void visit(PhiNode* node) { }
    virtual void visit(SsaIntVarFactor* node) { visit((IntVarFactor*)node); }
    
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
