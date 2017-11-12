#pragma once

#include "AstVisitor.hpp"

class ExpressionFolder : AstVisitor
{
public:
    ExpressionFolder(CodeBlockNode* programBody_, Ast& ast_)
        : programBody(programBody_),
        ast(ast_),
        totalFolded(0),
        totalConstantsPropogated(0),
        totalAlwaysTrueIfStatements(0) { }
    
    bool foldExpressions()
    {
        success = false;
        programBody->acceptRecursive(*this);
        return success;
    }
    
    void printStats()
    {
        printf("Total expressions folded: %d\n", totalFolded);
        printf("Total constants propogated: %d\n", totalConstantsPropogated);
        printf("Total always true if's replaced: %d\n", totalAlwaysTrueIfStatements);
    }
    
private:
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
            ++totalFolded;
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
            //node->markAsDead();
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
    
    void visit(IfNode* node)
    {
        if(auto intNode = dynamic_cast<IntegerNode*>(node->condition))
        {
            if(intNode->value != 0)
            {
                replaceNode(node->body);
                success = true;
                ++totalAlwaysTrueIfStatements;
            }
        }
    }
    
    void visit(PhiNode* node)
    {
        int value = (*node->joinNodes.begin())->value;
        
        for(auto joinNode : node->joinNodes)
        {
            if(!joinNode->hasConstantValue)
                return;
            
            if(joinNode->value != value)
                return;
        }
        
        // All join nodes are constant and have the same value, replace with the value
        replaceNode(ast.newIntegerNode(value));
    }
    
    CodeBlockNode* programBody;
    Ast& ast;
    bool success;
    int totalFolded;
    int totalConstantsPropogated;
    int totalAlwaysTrueIfStatements;
};
