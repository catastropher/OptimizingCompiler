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
    ExpressionNode* value;
    virtual ~ExpressionNode() { }
};

struct FactorNode : ExpressionNode
{
    
};

struct IntegerNode : FactorNode
{
    IntegerNode(int value_) : value(value_) { }
    
    int value;
};

struct BinaryOpNode : ExpressionNode
{
    BinaryOpNode(ExpressionNode* left_, TokenType op_, ExpressionNode* right_)
        : left(left_), op(op_), right(right_) { }
    
    ExpressionNode* left;
    TokenType op;
    ExpressionNode* right;
};

struct UnaryOpNode : ExpressionNode
{
    ExpressionNode* value;
    TokenType op;
};

class Ast
{
private:
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
    
    void addNode(AstNode* node)
    {
        nodes.push_back(node);
    }
    
    ~Ast()
    {
        for(AstNode* node : nodes)
            delete node;
    }
    
    std::vector<AstNode*> nodes;
};

