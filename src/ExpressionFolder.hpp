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
    
private:
    CodeBlockNode* programBody;
    Ast& ast;
    bool success;
};
