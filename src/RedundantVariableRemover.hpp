#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "VariableReplacer.hpp"
#include "VariableUsageCounter.hpp"

class RedundantVariableRemover : AstVisitor
{
public:
    RedundantVariableRemover(CodeBlockNode* programBody_) : programBody(programBody_), totalRemoved(0) { }
    
    bool removeRedundantVariables()
    {
        VariableUsageCounter counter(programBody, true, false, false);
        auto count = counter.countVarUses();
        bool success = false;
        
        declNodes.clear();
        
        for(auto var : count)
        {
            declNodes[var.first->var] += var.second;
        }
        
        for(auto var : count)
        {
            bool inputNode = dynamic_cast<InputIntNode*>(var.first->definitionNode->rightSide);
            
            // Only one definition of var
            if(declNodes[var.first->var] == 1 && !inputNode)
            {
                hasAllSingleDefVariables = true;
                var.first->definitionNode->rightSide->acceptRecursive(*this);
                
                if(!hasAllSingleDefVariables)
                    continue;
                
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
    void visit(SsaIntVarFactor* node)
    {
        if(declNodes[node->var] > 1)
            hasAllSingleDefVariables = false;
    }
    
    void visit(OneDimensionalListFactor* node)
    {
        // Can't perform analysis on arrays
        hasAllSingleDefVariables = false;
    }
    
    void visit(TwoDimensionalListFactor* node)
    {
        // Can't perform analysis on arrays
        hasAllSingleDefVariables = false;
    }
    
    void visit(ThreeDimensionalListFactor* node)
    {
        // Can't perform analysis on arrays
        hasAllSingleDefVariables = false;
    }
    
    std::map<IntDeclNode*, int> declNodes;
    CodeBlockNode* programBody;
    int totalRemoved;
    bool hasAllSingleDefVariables;
};

