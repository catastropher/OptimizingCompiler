#pragma once

#include "Ast.hpp"
#include "VariableReplacer.hpp"
#include "VariableUsageCounter.hpp"

class RedundantVariableRemover
{
public:
    RedundantVariableRemover(CodeBlockNode* programBody_) : programBody(programBody_) { }
    
    bool removeRedundantVariables()
    {
        VariableUsageCounter counter(programBody);
        auto count = counter.countVarUses();
        bool success = false;
        
        for(auto var : count)
        {
            if(count[var.first] == 1)
            {
                VariableReplacer replacer(programBody, var.first, var.first->definitionNode->rightSide);
                success |= replacer.replaceVars();
            }
        }
        
        return success;
    }
    
private:
    CodeBlockNode* programBody;
};

