#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"

#include <typeinfo>

class StatementKiller : AstVisitor
{
public:
    StatementKiller(CodeBlockNode* programBody_, std::set<StatementNode*>& liveStatements_)
        : programBody(programBody_), liveStatements(liveStatements_) { }
        
    bool killDeadStatements()
    {
        success = false;
        programBody->acceptRecursive(*this);
        return success;
    }
    
    void visit(StatementNode* node)
    {
        // Currently don't kill code blocks
        if(dynamic_cast<CodeBlockNode*>(node))
            return;
        
        if(liveStatements.count(node) == 0)
        {
            node->markAsDead();
            success = true;
        }
    }
    
private:
    CodeBlockNode* programBody;
    std::set<StatementNode*>& liveStatements;
    bool success;
};

