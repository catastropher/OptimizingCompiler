#pragma once

#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "PolynomialBuilder.hpp"

class PolynomialSimplifier : AstVisitor
{
public:
    PolynomialSimplifier(Ast& ast_, CodeBlockNode* programBody) : ast(ast_)
    {
        programBody->acceptRecursive(*this);
    }
    
private:
    void visit(ExpressionNode* node)
    {
        try
        {
            PolynomialBuilder builder;
            Polynomial p = builder.toPolynomial(node);
            //replaceNode(ast.addPolynomialNode(p));
        }
        catch(...)
        {
        }
    }
    
    Ast& ast;
};
