#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "JoinNodeRemover.hpp"

#include <typeinfo>

class StatementKiller : AstVisitor
{
public:
    StatementKiller(CodeBlockNode* programBody_, std::set<StatementNode*>& liveStatements_, std::set<StatementNode*>& killedStatements_)
        : programBody(programBody_),
        liveStatements(liveStatements_),
        killedStatements(killedStatements_),
        totalKilledStatements(0)
        { }
        
    bool killDeadStatements()
    {
        totalKilledStatements = 0;
        success = false;
        programBody->acceptRecursive(*this);
        return success;
    }
    
    int getTotalKilledStatements()
    {
        return totalKilledStatements;
    }
    
private:
    void visit(StatementNode* node)
    {
        if(dynamic_cast<CodeBlockNode*>(node))
            return;
        
        if(node->markedAsDead)
            return;
        
        if(liveStatements.count(node) == 0 || killedStatements.count(node) != 0)
        {
            node->markAsDead();
            success = true;
            ++totalKilledStatements;
        }
    }
    
    void visit(BasicBlockNode* node)
    {
        if(killedStatements.count(node) == 0 || node->markedAsDead)
            return;
        
        node->markAsDead();
        
        for(StatementNode* s : node->statements)
        {
            killedStatements.insert(s);
            s->accept(*this);
        }
    }
    
    void visit(LetStatementNode* node)
    {
        if(node->markedAsDead)
            return;
        
        // Special case: don't kill input nodes
        if(dynamic_cast<InputIntNode*>(node->rightSide) && killedStatements.count(node) == 0)
            return;
        
        if(liveStatements.count(node) == 0 || killedStatements.count(node) != 0)
        {
            if(auto lValue = dynamic_cast<SsaIntLValueNode*>(node->leftSide))
            {
                JoinNodeRemover remover(programBody, lValue);
                remover.removeFromJoinNodes();
            }
            
            node->markAsDead();
            success = true;
            ++totalKilledStatements;
        }
    }
    
    CodeBlockNode* programBody;
    std::set<StatementNode*>& liveStatements;
    std::set<StatementNode*>& killedStatements;
    bool success;
    int totalKilledStatements;
};

