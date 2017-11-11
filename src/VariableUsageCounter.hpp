#pragma once

#include <map>

#include "Ast.hpp"
#include "AstVisitor.hpp"

class VariableUsageCounter : AstVisitor
{
public:
    VariableUsageCounter(CodeBlockNode* programBody_) : programBody(programBody_) { }
    
    std::map<SsaIntLValueNode*, int> countVarUses()
    {
        useCounter.clear();
        programBody->acceptRecursive(*this);
        return useCounter;
    }
    
private:
    void visit(SsaIntVarFactor* node)
    {
        ++useCounter[node->ssaLValue];
    }
    
    void visit(PhiNode* node)
    {
        for(auto joinNode : node->joinNodes)
            ++useCounter[joinNode];
    }
    
    CodeBlockNode* programBody;
    std::map<SsaIntLValueNode*, int> useCounter;
};

