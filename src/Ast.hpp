#pragma once

#include <string>
#include <vector>

#include "Token.hpp"

struct AstNode
{
    AstNode* nextNode;
    virtual ~AstNode() { }
};

struct ExpressionNode : AstNode
{
    virtual int tryEvaluate() { throw "Can't evaluate expression"; }
    
    virtual ~ExpressionNode() { }
};

struct FactorNode : ExpressionNode
{
    
};

struct IntegerNode : FactorNode
{
    IntegerNode(int value_) : value(value_) { }
    
    int tryEvaluate() { return value; }
    
    int value;
};

struct BinaryOpNode : ExpressionNode
{
    BinaryOpNode(ExpressionNode* left_, TokenType op_, ExpressionNode* right_)
        : left(left_), op(op_), right(right_) { }
    
    int tryEvaluate()
    {
        int a = left->tryEvaluate();
        int b = right->tryEvaluate();
        
        switch(op)
        {
            case TOK_ADD: return a + b;
            case TOK_SUB: return a - b;
            case TOK_MUL: return a * b;
            case TOK_DIV: return a / b;
            case TOK_MOD: return a % b;
            default: break;
        }
        
        throw "Can't evaluate op";
    }
    
    ExpressionNode* left;
    TokenType op;
    ExpressionNode* right;
};

struct UnaryOpNode : ExpressionNode
{
    UnaryOpNode(ExpressionNode* value_, TokenType op_) : value(value_), op(op_) { }
    
    int tryEvaluate()
    {
        if(op == TOK_ADD)
            return value->tryEvaluate();
        
        if(op == TOK_SUB)
            return -value->tryEvaluate();
        
        throw "Invalid unary op";
    }
    
    ExpressionNode* value;
    TokenType op;
};

class Ast
{
public:
    IntegerNode* newIntegerNode(int value)
    {
        IntegerNode* newNode = new IntegerNode(value);
        addNode(newNode);
        return newNode;
    }
    
    BinaryOpNode* newBinaryOpNode(ExpressionNode* left, TokenType op, ExpressionNode* right)
    {
        BinaryOpNode* newNode = new BinaryOpNode(left, op, right);
        addNode(newNode);
        return newNode;
    }
    
    UnaryOpNode* newUnaryOpNode(ExpressionNode* value, TokenType op)
    {
        UnaryOpNode* newNode = new UnaryOpNode(value, op);
        addNode(newNode);
        return newNode;
    }
    
    void addNode(AstNode* node)
    {
        nodes.push_back(node);
    }
    
    ~Ast()
    {
        for(AstNode* node : nodes)
            delete node;
    }
    
private:
    std::vector<AstNode*> nodes;
};

