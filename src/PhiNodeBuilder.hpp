#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"

#include <typeinfo>

class PhiNodeBuilder : AstVisitor
{
public:
    PhiNodeBuilder(CodeBlockNode* programBody_, Ast& ast_) : programBody(programBody_), ast(ast_) { }
    
    void buildPhiNodes()
    {
        programBody->acceptRecursive(*this);
    }
    
private:
    void visit(BasicBlockNode* node)
    {
        activeVars = node->varDefIn;
    }
    
    void visit(LetStatementNode* node)
    {
        SsaIntLValueNode* ssaLValue = dynamic_cast<SsaIntLValueNode*>(node->leftSide);
        if(!ssaLValue)
            return;
            
        activeVars.replaceDefinition(ssaLValue);
    }
    
    void visit(IntVarFactor* node)
    {
        replaceNode(ast.addPhiNode(activeVars.getActiveDefsForVar(node->var)));
    }
    
    CodeBlockNode* programBody;
    Ast& ast;
    VarDefSet activeVars;
};

