#pragma once

#include <stack>

#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "Polynomial.hpp"

class PolynomialBuilder : AstVisitor
{
public:
    Polynomial toPolynomial(ExpressionNode* node)
    {
        node->acceptRecursive(*this);
        
        if(stack.size() != 1)
            throw "Bad poly stack";
        
        return popPoly();
    }
    
private:
    void visit(AstNode* node) { throw "Bad node"; }
    
    
    void visit(IntegerNode* node)
    {
        pushPoly(Polynomial(node->value));
    }
    
    void visit(IntVarFactor* node)
    {
        pushPoly(Polynomial(node->var->name));
    }
    
    void visit(PhiNode* node)
    {
        pushPoly(Polynomial((*node->joinNodes.begin())->var->name));
    }
    
    void visit(SsaIntVarFactor* node)
    {
        pushPoly(Polynomial(node->var->name));
    }
    
    void visit(PolynomialNode* node)
    {
        pushPoly(node->poly);
    }
    
    void visit(BinaryOpNode* node)
    {
        Polynomial b = popPoly();
        Polynomial a = popPoly();
        
        if(node->op == TOK_ADD) { pushPoly(a.add(b)); return; }
        if(node->op == TOK_SUB) { pushPoly(a.sub(b)); return; }
        if(node->op == TOK_MUL) { pushPoly(a.mul(b)); return; }
        if(node->op == TOK_DIV) throw "Bad polynomial division";
        
        throw "Bad polynomial op";
    }
    
    void visit(UnaryOpNode* node)
    {
        if(node->op == TOK_ADD)
        {
            pushPoly(popPoly());
            return;
        }
        else if(node->op == TOK_SUB)
        {
            pushPoly(popPoly().neg());
            return;
        }
        
        throw "Bad polynomial op";
    }
    
    void pushPoly(Polynomial p)
    {
        stack.push(p);
    }
    
    Polynomial popPoly()
    {
        if(stack.size() == 0)
            throw "Emply poly stack";
        
        Polynomial top = stack.top();
        stack.pop();
        return top;
    }
    
    std::stack<Polynomial> stack;
};
