#pragma once

#include <set>
#include <queue>
#include <vector>

#include "Ast.hpp"
#include "AstVisitor.hpp"

class DeadBasicBlockEliminator
{
public:
    DeadBasicBlockEliminator(CodeBlockNode* programBody_) : programBody(programBody_) { }
    
    std::vector<BasicBlockNode*> eliminateDeadBlocks()
    {
        std::vector<BasicBlockNode*> deadBlocks;
        
        workQueue.push(dynamic_cast<BasicBlockNode*>(programBody->statements.front()));
        
        while(workQueue.size() > 0)
        {
            BasicBlockNode* node = workQueue.front();
            ++refCount[node];
            workQueue.pop();
            visitBlock(node);
        }
        
        for(auto s : programBody->statements)
        {
            BasicBlockNode* node = dynamic_cast<BasicBlockNode*>(s);
            if(refCount[node] == 0 && !node->markedAsDead)
                deadBlocks.push_back(node);
        }
        
        return deadBlocks;
    }
    
private:
    void visitBlock(BasicBlockNode* node)
    {
        if(visitedBlocks.count(node) != 0)
            return;
        
        visitedBlocks.insert(node);
        
        for(auto s : node->statements)
        {
            if(GotoNode* gotoNode = dynamic_cast<GotoNode*>(s))
            {
                workQueue.push(gotoNode->targetBlock);
                break;
            }
            else if(IfNode* ifNode = dynamic_cast<IfNode*>(s))
            {
                workQueue.push(dynamic_cast<GotoNode*>(ifNode->body)->targetBlock);
                break;
            }
        }
        
        if(!node->blockEndsInGoto() && node->directSuccessor != nullptr)
        {
            workQueue.push(node->directSuccessor);
        }
    }
    
    CodeBlockNode* programBody;
    std::set<BasicBlockNode*> visitedBlocks;
    std::queue<BasicBlockNode*> workQueue;
    std::map<BasicBlockNode*, int> refCount;
};

