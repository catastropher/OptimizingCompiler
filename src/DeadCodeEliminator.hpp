#pragma once

#include <set>
#include <queue>

#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "IoStatementFinder.hpp"
#include "StatementKiller.hpp"
#include "DeadBasicBlockEliminator.hpp"

class DeadCodeEliminator : AstVisitor
{
public:
    DeadCodeEliminator(CodeBlockNode* programBody_) : programBody(programBody_) { }
    
    bool eliminateDeadCode()
    {
        success = false;
        liveStatements.clear();
        killedStatements.clear();
        
        IoStatementFinder finder(programBody);
        liveStatements = finder.findIoStatements();
        
        printf("Live statements: %d\n", (int)liveStatements.size());
        
        for(auto s : liveStatements)
            workQueue.push(s);
        
        while(workQueue.size() > 0)
        {
            auto s = workQueue.front();
            workQueue.pop();
            s->acceptRecursive(*this);
        }
        
        eliminateDeadBasicBlocks();
        
        StatementKiller killer(programBody, liveStatements, killedStatements);
        return killer.killDeadStatements() || success;
    }
    
private:    
    void eliminateDeadBasicBlocks()
    {
        DeadBasicBlockEliminator basicBlockEliminator(programBody);
        auto killedBlocks = basicBlockEliminator.eliminateDeadBlocks();
        
        killedStatements.insert(killedBlocks.begin(), killedBlocks.end());
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
    
    CodeBlockNode* programBody;
    std::set<StatementNode*> liveStatements;
    std::set<StatementNode*> killedStatements;
    std::queue<StatementNode*> workQueue;
    bool success;
};

