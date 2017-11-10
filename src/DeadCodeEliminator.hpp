#pragma once

#include <set>
#include <queue>

#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "IoStatementFinder.hpp"
#include "StatementKiller.hpp"

class DeadCodeEliminator : AstVisitor
{
public:
    DeadCodeEliminator(CodeBlockNode* programBody_) : programBody(programBody_) { }
    
    bool eliminateDeadCode()
    {
        success = false;
        liveStatements.clear();
        
        IoStatementFinder finder(programBody);
        liveStatements = finder.findIoStatements();
        
        for(auto s : liveStatements)
            workQueue.push(s);
        
        while(workQueue.size() > 0)
        {
            auto s = workQueue.front();
            workQueue.pop();
            s->acceptRecursive(*this);
        }
        
        StatementKiller killer(programBody, liveStatements);
        return killer.killDeadStatements() || success;
    }
    
    void scheduleNode(StatementNode* node)
    {
        if(liveStatements.count(node) != 0)
            return;
        
        liveStatements.insert(node);
        workQueue.push(node);
    }
    
    void visit(SsaIntLValueNode* node)
    {
        scheduleNode(node->definitionNode);
    }
    
    void visit(SsaIntVarFactor* node)
    {
        scheduleNode(node->ssaLValue->definitionNode);
    }
    
    void visit(PhiNode* node)
    {
        for(auto joinNode : node->joinNodes)
            scheduleNode(joinNode->definitionNode);
    }
    
    void visit(IfNode* node)
    {
        if(auto intNode = dynamic_cast<IntegerNode*>(node->condition))
        {
            if(intNode->value == 0)
            {
                node->markAsDead();
                success = true;
            }
        }
    }
    
private:
    CodeBlockNode* programBody;
    std::set<StatementNode*> liveStatements;
    std::queue<StatementNode*> workQueue;
    bool success;
};
