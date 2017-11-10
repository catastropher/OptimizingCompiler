#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"

class JoinNodeRemover : AstVisitor
{
public:
    JoinNodeRemover(CodeBlockNode* programBody_, SsaIntLValueNode* nodeToRemove_)
        : programBody(programBody_), nodeToRemove(nodeToRemove_) { }
        
    bool removeFromJoinNodes()
    {
        success = false;
        programBody->acceptRecursive(*this);
        return success;
    }
    
private:
    void visit(PhiNode* node)
    {
        if(node->joinNodes.count(nodeToRemove) == 0)
            return;
        
        node->joinNodes.erase(nodeToRemove);
        success = true;
    }
    
    CodeBlockNode* programBody;
    SsaIntLValueNode* nodeToRemove;
    bool success;
};
