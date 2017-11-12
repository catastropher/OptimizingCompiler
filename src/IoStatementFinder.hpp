#pragma once

#include <set>

#include "Ast.hpp"
#include "AstVisitor.hpp"

class IoStatementFinder : AstVisitor
{
public:
    IoStatementFinder(CodeBlockNode* programBody_) : programBody(programBody_)
        { visitLetBefore = true; }
    
    std::set<StatementNode*> findIoStatements()
    {
        currentStatement = nullptr;
        ioStatements.clear();
        programBody->acceptRecursive(*this);
        return ioStatements;
    }
    
    void visit(PromptNode* node)
    {
        ioStatements.insert(node);
    }
    
    void visit(PrintNode* node)
    {
        ioStatements.insert(node);
    }
    
    void visit(InputNode* node)
    {
        ioStatements.insert(node);
    }
    
    void visit(IfNode* node)
    {
        ioStatements.insert(node);
    }
    
    void visit(GotoNode* node)
    {
        ioStatements.insert(node);
    }
    
    void visit(EndNode* node)
    {
        ioStatements.insert(node);
    }
    
    void visit(StatementNode* node)
    {
        currentStatement = node;
    }
    
    void visit(OneDimensionalListFactor* node)
    {
        if(currentStatement)
            ioStatements.insert(currentStatement);
    }
    
    void visit(OneDimensionalListLValueNode* node)
    {
        if(currentStatement)
            ioStatements.insert(currentStatement);
    }
    
private:
    CodeBlockNode* programBody;
    std::set<StatementNode*> ioStatements;
    StatementNode* currentStatement;
};

