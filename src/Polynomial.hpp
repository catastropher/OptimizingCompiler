#pragma once

#include <map>
#include <stack>

#include "Ast.hpp"
#include "AstVisitor.hpp"

struct Polynomial
{
    Polynomial(int val)
    {
        coeff["constant"] = val;
    }
    
    Polynomial(std::string var)
    {
        coeff[var] = 1;
    }
    
    bool onlyConstant()
    {
        return coeff.size() == 1 && coeff.count("constant") == 1;
    }
    
    Polynomial add(Polynomial& p)
    {
        Polynomial res = *this;
        for(auto c : p.coeff)
            res.coeff[c.first] += c.second;
        
        return res;
    }
    
    Polynomial sub(Polynomial& p)
    {
        Polynomial res = *this;
        for(auto c : p.coeff)
            res.coeff[c.first] += c.second;
        
        return res;
    }
    
    Polynomial mul(Polynomial& p)
    {
        if(!onlyConstant() && !p.onlyConstant())
            throw "Bad polynomial mul";
        
        if(p.onlyConstant())
        {
            Polynomial res = *this;
            for(auto c : coeff)
            {
                res.coeff[c.first] *= p.coeff["constant"];
            }
            
            
            return res;
        }
        else
            return p.mul(*this);
    }
    
    Polynomial neg()
    {
        Polynomial res = *this;
        for(auto c : coeff)
            res.coeff[c.first] = -res.coeff[c.first];
        
        return res;
    }
    
    void print()
    {
        bool first = true;
        
        for(auto c : coeff)
        {
            if(!first)
                printf(" + ");
            
            first = false;
            printf("%d * %s", c.second, c.first.c_str());
        }
        
        printf("\n");
    }
    
    std::map<std::string, int> coeff;
};

class PolynomialBuilder : AstVisitor
{
public:
    Polynomial toPolynomial(ExpressionNode* node)
    {
        node->acceptRecursive(*this);
        return popPoly();
    }
    
private:
    void visit(IntegerNode* node)
    {
        pushPoly(Polynomial(node->value));
    }
    
    void visit(IntVarFactor* node)
    {
        pushPoly(Polynomial(node->var->name));
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
        Polynomial top = stack.top();
        stack.pop();
        return top;
    }
    
    std::stack<Polynomial> stack;
};

