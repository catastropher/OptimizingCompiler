#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"

class ForLoopNormalizer : AstVisitor
{
public:
    ForLoopNormalizer(ForLoopNode* forNode_, Ast& ast_) : forNode(forNode_), ast(ast_)
    {
        printf("start\n");
        
        auto varLValue = dynamic_cast<IntLValueNode*>(forNode->var);
        if(!varLValue)
            return;
        
        auto var = varLValue->var;
        
        int upper, lower, stride;
        
        try
        {
            lower = forNode->lowerBound->tryEvaluate();
            upper = forNode->upperBound->tryEvaluate();
            stride = forNode->increment->tryEvaluate();
        }
        catch(...)
        {
            return;
        }
        
        printf("HERE\n");
        
        bool normalized = true;
        
        replaceExpression = ast.addIntVarFactor(var);
        
        if(stride == 0)
            return;
        
        if(stride != 1)
        {
            replaceExpression = ast.newBinaryOpNode(replaceExpression, TOK_MUL, ast.newIntegerNode(stride));
            normalized = false;
        }
        
        if(lower != 0)
        {
            replaceExpression = ast.newBinaryOpNode(replaceExpression, TOK_ADD, ast.newIntegerNode(lower));
            normalized = false;
        }
        
        if(normalized)
            return;
        
        forNode->upperBound = ast.newIntegerNode((upper - lower) / stride);
        forNode->lowerBound = ast.newIntegerNode(0);
        forNode->increment = ast.newIntegerNode(1);
        
        varToReplace = var;
        forNode->body->acceptRecursive(*this);
    }
    
private:
    void visit(IntVarFactor* node)
    {
        if(node->var != varToReplace)
            return;
        
        replaceNode(replaceExpression);
    }
    
    ForLoopNode* forNode;
    Ast& ast;
    IntDeclNode* varToReplace;
    ExpressionNode* replaceExpression;
};

