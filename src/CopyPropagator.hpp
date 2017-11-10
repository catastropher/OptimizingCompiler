#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "VariableReplacer.hpp"

class CopyPropagator : AstVisitor
{
public:
    CopyPropagator(CodeBlockNode* programBody_, Ast& ast_) : programBody(programBody_), ast(ast_) { }
    
    bool propagateCopies()
    {
        success = false;
        programBody->acceptRecursive(*this);
        return success;
    }
    
private:
    void visit(PhiNode* node)
    {        
        if(node->joinNodes.size() == 1)
        {
            replaceNode(ast.addSsaIntVarFactorNode(*node->joinNodes.begin()));
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
            printf("Replace %s with %s\n", lValue->var->name.c_str(), var->var->name.c_str());
            VariableReplacer replacer(programBody, lValue, var);
            replacer.replaceVars();
            success = true;
            node->markAsDead();
        }
    }
    
    CodeBlockNode* programBody;
    bool success;
    Ast& ast;
};

