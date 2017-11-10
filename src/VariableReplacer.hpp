#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"

class VariableReplacer : AstVisitor
{
public:
    VariableReplacer(CodeBlockNode* programBody_, SsaIntLValueNode* nodeToReplace_, SsaIntVarFactor* replacement_)
        : programBody(programBody_), nodeToReplace(nodeToReplace_), replacement(replacement_) { }    
    
    bool replaceVars()
    {
        success = false;
        programBody->acceptRecursive(*this);
        return success;
    }
    
private:
    void visit(SsaIntVarFactor* node)
    {
        if(node->ssaLValue == nodeToReplace)
        {
            replaceNode(replacement);
            success = true;
        }
    }
    
    void visit(PhiNode* node)
    {
        if(node->joinNodes.count(nodeToReplace) == 0)
            return;
        
        node->joinNodes.erase(nodeToReplace);
        success = true;
    }
    
    CodeBlockNode* programBody;
    SsaIntLValueNode* nodeToReplace;
    SsaIntVarFactor* replacement;
    bool success;
};

