#pragma once

#include <set>

#include "Ast.hpp"
#include "AstVisitor.hpp"

#include <typeinfo>

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
    
private:
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
        forceAddCurrentStatement();
    }
    
    void visit(TwoDimensionalListFactor* node)
    {
        forceAddCurrentStatement();
    }
    
    void visit(ThreeDimensionalListFactor* node)
    {
        forceAddCurrentStatement();
    }
    
    void visit(OneDimensionalListLValueNode* node)
    {
        forceAddCurrentStatement();
    }
    
    void visit(TwoDimensionalListLValueNode* node)
    {
        forceAddCurrentStatement();
    }
   
   void visit(ThreeDimensionalListLValueNode* node)
    {
        forceAddCurrentStatement();
    }
   
    void visit(InputIntNode* node)
    {
        forceAddCurrentStatement();
    }
    
    void forceAddCurrentStatement()
    {
        if(!currentStatement)
            return;
        
        ioStatements.insert(currentStatement);
    }
    
    CodeBlockNode* programBody;
    std::set<StatementNode*> ioStatements;
    StatementNode* currentStatement;
};

