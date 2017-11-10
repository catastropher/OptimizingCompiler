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
        for(StatementNode* s : programBody->statements)
        {
            processBasicBlock(dynamic_cast<BasicBlockNode*>(s));
        }
    }
    
private:
    void processBasicBlock(BasicBlockNode* node)
    {
        std::vector<StatementNode*> phiNodes;
        activeVars = node->varDefIn;
        
        for(auto var : node->varDefIn.defs)
        {
            if(var.second.size() > 1)
            {
                phiNodes.push_back(createTempJoin(var.first, node));
            }
        }
        
        auto it = node->statements.begin();
        while(it != node->statements.end() && dynamic_cast<LabelNode*>(*it))
            ++it;
        
        node->statements.insert(it, phiNodes.begin(), phiNodes.end());
        node->acceptRecursive(*this);
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
        auto defs = activeVars.getActiveDefsForVar(node->var);
        
        if(defs.size() != 1)
            throw "Not just one def for " + node->var->name;
        
        replaceNode(ast.addSsaIntVarFactorNode(*defs.begin()));
    }
    
    LetStatementNode* createTempJoin(IntDeclNode* var, BasicBlockNode* basicBlock)
    {
        auto lValue = ast.addSsaIntLValueNode(ast.addIntLValue(var), basicBlock, nullptr);
        
        auto letStatement = ast.addLetStatementNode
        (
            lValue,
            ast.addPhiNode(activeVars.getActiveDefsForVar(var))
        );
        
        lValue->definitionNode = letStatement;
        return letStatement;
    }
    
    CodeBlockNode* programBody;
    Ast& ast;
    VarDefSet activeVars;
};

