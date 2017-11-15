#pragma once

#include <map>

#include "Ast.hpp"
#include "AstVisitor.hpp"

class VariableUsageCounter : AstVisitor
{
public:
    VariableUsageCounter(CodeBlockNode* programBody_, bool includeLValues_ = false, bool includeFactors_ = true, bool includePhiNodes_ = true)
        : programBody(programBody_), includeLValues(includeLValues_), includeFactors(includeFactors_), includePhiNodes(includePhiNodes_) { }
    
    std::map<SsaIntLValueNode*, int> countVarUses()
    {
        useCounter.clear();
        programBody->acceptRecursive(*this);
        return useCounter;
    }
    
private:
    void visit(SsaIntVarFactor* node)
    {
        if(!includeFactors)
            return;
        
        ++useCounter[node->ssaLValue];
    }
    
    void visit(PhiNode* node)
    {
        if(!includeLValues || !includePhiNodes)
            return;
        
        for(auto joinNode : node->joinNodes)
            ++useCounter[joinNode];
    }
    
    void visit(SsaIntLValueNode* node)
    {
        if(!includeLValues)
            return;
        
        ++useCounter[node];
    }
    
    CodeBlockNode* programBody;
    std::map<SsaIntLValueNode*, int> useCounter;
    bool includeLValues;
    bool includeFactors;
    bool includePhiNodes;
};

