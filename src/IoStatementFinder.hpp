#pragma once

#include <set>

#include "Ast.hpp"
#include "AstVisitor.hpp"

class IoStatementFinder : AstVisitor
{
public:
    IoStatementFinder(CodeBlockNode* programBody_) : programBody(programBody_) { }
    
    std::set<StatementNode*> findIoStatements()
    {
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
    
private:
    CodeBlockNode* programBody;
    std::set<StatementNode*> ioStatements;
};

