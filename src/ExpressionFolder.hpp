#pragma once

#include "AstVisitor.hpp"

class ExpressionFolder : AstVisitor
{
public:
    ExpressionFolder(CodeBlockNode* programBody_, Ast& ast_) : programBody(programBody_), ast(ast_) { }
    
    bool foldExpressions()
    {
        success = false;
        programBody->acceptRecursive(*this);
        return success;
    }
    
    void visit(ExpressionNode* node)
    {
        bool alreadySimplified = dynamic_cast<IntegerNode*>(node);
        if(alreadySimplified)
            return;
        
        try
        {
            int value = node->tryEvaluate();
            replaceNode(ast.newIntegerNode(value));
            success = true;
        }
        catch(...)
        {
            
        }
    }
    
    void visit(LetStatementNode* node)
    {
        SsaIntLValueNode* lValue = dynamic_cast<SsaIntLValueNode*>(node->leftSide);
        if(!lValue)
            return;
        
        if(lValue->hasConstantValue)
            return;
        
        if(IntegerNode* intNode = dynamic_cast<IntegerNode*>(node->rightSide))
        {
            lValue->setConstant(intNode->value);
            success = true;
        }
    }
    
    void visit(SsaIntVarFactor* node)
    {
        if(node->ssaLValue->hasConstantValue)
        {
            replaceNode(ast.newIntegerNode(node->ssaLValue->value));
            success = true;
        }
    }
    
private:
    CodeBlockNode* programBody;
    Ast& ast;
    bool success;
};
