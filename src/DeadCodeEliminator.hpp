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
    DeadCodeEliminator(CodeBlockNode* programBody_)
        : programBody(programBody_), totalKilledStatements(0), totalKilledBlocks(0), totalKilledIfs(0) { }
    
    bool eliminateDeadCode()
    {
        success = false;
        liveStatements.clear();
        killedStatements.clear();
        
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
        
        eliminateDeadBasicBlocks();
        
        printf("Live statements: %d\n", (int)liveStatements.size());
        
        StatementKiller killer(programBody, liveStatements, killedStatements);
        bool worked = killer.killDeadStatements() || success;
        totalKilledStatements += killer.getTotalKilledStatements();
        
        return worked;
    }
    
    void printStats()
    {
        printf("Total always false if's replaced: %d\n", totalKilledIfs);
        printf("Total dead statements eliminated: %d\n", totalKilledStatements);
        printf("Total dead blocks eliminated: %d\n", totalKilledBlocks);
    }
    
private:    
    void eliminateDeadBasicBlocks()
    {
        DeadBasicBlockEliminator basicBlockEliminator(programBody);
        auto killedBlocks = basicBlockEliminator.eliminateDeadBlocks();
        
        killedStatements.insert(killedBlocks.begin(), killedBlocks.end());
        
        totalKilledBlocks += killedBlocks.size();
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
                ++totalKilledIfs;
            }
        }
    }
    
    CodeBlockNode* programBody;
    std::set<StatementNode*> liveStatements;
    std::set<StatementNode*> killedStatements;
    std::queue<StatementNode*> workQueue;
    bool success;
    int totalKilledStatements;
    int totalKilledBlocks;
    int totalKilledIfs;
};

