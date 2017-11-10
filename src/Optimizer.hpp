#pragma once

#include "Ast.hpp"
#include "SsaBuilder.hpp"
#include "PhiNodeBuilder.hpp"
#include "ExpressionFolder.hpp"
#include "DeadCodeEliminator.hpp"
#include "CopyPropagator.hpp"

class Optimizer
{
public:
    Optimizer(CodeBlockNode* programBody_, Ast& ast_)
        : programBody(programBody_),
        ast(ast_),
        expressionFolder(programBody, ast),
        eliminator(programBody),
        copyPropagator(programBody, ast)
        { }
        
    void optimize()
    {
        SsaBuilder ssaBuilder(programBody, ast);
        ssaBuilder.buildSsa();
        
        PhiNodeBuilder phiNodeBuilder(programBody, ast);
        phiNodeBuilder.buildPhiNodes();
        
        while(optimizeIteration()) ;
    }
    
private:
    bool optimizeIteration()
    {
        bool success = false;
        
        success |= expressionFolder.foldExpressions();
        success |= eliminator.eliminateDeadCode();
        success |= copyPropagator.propagateCopies();
        
        return success;
    }
    
    
    CodeBlockNode* programBody;
    Ast& ast;
    ExpressionFolder expressionFolder;
    DeadCodeEliminator eliminator;
    CopyPropagator copyPropagator;
};
