#pragma once

#include "Ast.hpp"
#include "VariableReplacer.hpp"
#include "VariableUsageCounter.hpp"

class RedundantVariableRemover
{
public:
    RedundantVariableRemover(CodeBlockNode* programBody_) : programBody(programBody_), totalRemoved(0) { }
    
    bool removeRedundantVariables()
    {
        VariableUsageCounter counter(programBody);
        auto count = counter.countVarUses();
        bool success = false;
        
        for(auto var : count)
        {
            bool inputNode = dynamic_cast<InputIntNode*>(var.first->definitionNode->rightSide);
            
            if(count[var.first] == 1 && !inputNode)
            {
                VariableReplacer replacer(programBody, var.first, var.first->definitionNode->rightSide);
                bool worked = replacer.replaceVars();
                
                if(worked)
                {
                    ++totalRemoved;
                    success = true;
                }
            }
        }
        
        return success;
    }
    
    void printStats()
    {
        printf("Total redundant vars removed: %d\n", totalRemoved);
    }
    
private:
    CodeBlockNode* programBody;
    int totalRemoved;
};

