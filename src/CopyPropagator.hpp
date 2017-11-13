#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "VariableReplacer.hpp"

class CopyPropagator : AstVisitor
{
public:
    CopyPropagator(CodeBlockNode* programBody_, Ast& ast_) : programBody(programBody_), ast(ast_), totalPropagated(0) { }
    
    bool propagateCopies()
    {
        success = false;
        programBody->acceptRecursive(*this);
        return success;
    }
    
    void printStats()
    {
        printf("Total variables propogated: %d\n", totalPropagated);
    }
    
private:
    void visit(PhiNode* node)
    {        
        if(node->joinNodes.size() == 1)
        {
            replaceNode(ast.addSsaIntVarFactorNode(*node->joinNodes.begin()));
            ++totalPropagated;
            success = true;
        }
    }
    
    void visit(LetStatementNode* node)
    {
        SsaIntLValueNode* lValue = dynamic_cast<SsaIntLValueNode*>(node->leftSide);
        if(!lValue)
            return;
        
        if(auto var = dynamic_cast<SsaIntVarFactor*>(node->rightSide))
        {
            VariableReplacer replacer(programBody, lValue, var);
            success |= replacer.replaceVars();
            ++totalPropagated;
            //node->markAsDead();
        }
    }
    
    CodeBlockNode* programBody;
    bool success;
    Ast& ast;
    int totalPropagated;
};

