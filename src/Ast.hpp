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

struct VarDeclNode : AstNode
{
    VarDeclNode(std::string name_, int line_, int col_)
        : name(name_), line(line_), col(col_) { }
    
    std::string name;
    int line;
    int col;
};

struct IntDeclNode : VarDeclNode
{
    IntDeclNode(std::string name_, int line_, int col_)
        : VarDeclNode(name_, line_, col_) { }
};

struct OneDimensionalListDecl : VarDeclNode
{
    OneDimensionalListDecl(std::string name_, int line_, int col_, int totalElements_)
        : VarDeclNode(name_, line_, col_),
        totalElements(totalElements_) { }
    
    int totalElements;
};

struct StatementNode : AstNode
{
    
};

struct LValueNode : AstNode
{
    
};

struct IntLValueNode : LValueNode
{
    IntLValueNode(IntDeclNode* var_) : var(var_) { }
    
    IntDeclNode* var;
};

struct LetStatementNode : StatementNode
{
    LetStatementNode(LValueNode* leftSide_, ExpressionNode* rightSide_) : leftSide(leftSide_), rightSide(rightSide_) { }
    
    LValueNode* leftSide;
    ExpressionNode* rightSide;
};

struct CodeBlockNode : AstNode
{
    void addStatement(StatementNode* node)
    {
        statements.push_back(node);
    }
    
    std::vector<StatementNode*> statements;
};

struct ForLoopNode : StatementNode
{
    ForLoopNode(LValueNode* var_, ExpressionNode* lower, ExpressionNode* upper, ExpressionNode* inc, CodeBlockNode* body_)
        : var(var_), lowerBound(lower), upperBound(upper), increment(inc), body(body_) { }
    
    LValueNode* var;
    ExpressionNode* lowerBound;
    ExpressionNode* upperBound;
    ExpressionNode* increment;
    CodeBlockNode* body;
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
    
    IntDeclNode* addIntegerVar(std::string name, int line, int col)
    {
        IntDeclNode* newNode = new IntDeclNode(name, line, col);
        vars.push_back(newNode);
        printf("Added new int var: %s\n", name.c_str());
        return newNode;
    }
    
    OneDimensionalListDecl* add1DListVar(std::string name, int line, int col, int totalElements)
    {
        OneDimensionalListDecl* newNode = new OneDimensionalListDecl(name, line, col, totalElements);
        vars.push_back(newNode);
        printf("Added new int 1D list of size %d: %s\n", totalElements, name.c_str());
        return newNode;
    }
    
    IntLValueNode* addIntLValue(IntDeclNode* varDecl)
    {
        IntLValueNode* newNode = new IntLValueNode(varDecl);
        addNode(newNode);
        return newNode;
    }
    
    LetStatementNode* addLetStatementNode(LValueNode* leftSide, ExpressionNode* rightSide)
    {
        LetStatementNode* newNode = new LetStatementNode(leftSide, rightSide);
        addNode(newNode);
        return newNode;
    }
    
    CodeBlockNode* addCodeBlockNode()
    {
        CodeBlockNode* newNode = new CodeBlockNode;
        addNode(newNode);
        return newNode;
    }
    
    ForLoopNode* addForLoopNode(LValueNode* var, ExpressionNode* lower, ExpressionNode* upper, ExpressionNode* inc, CodeBlockNode* body_)
    {
        ForLoopNode* newNode = new ForLoopNode(var, lower, upper, inc, body_);
        addNode(newNode);
        return newNode;
    }
    
    VarDeclNode* getVarByName(std::string name)
    {
        for(VarDeclNode* var : vars)
        {
            if(var->name == name)
                return var;
        }
        
        return nullptr;
    }
    
    void setBody(CodeBlockNode* body_)
    {
        body = body_;
    }
    
    ~Ast()
    {
        for(AstNode* node : nodes)
            delete node;
        
        for(VarDeclNode* node : vars)
            delete node;
    }
    
private:
    void addNode(AstNode* node)
    {
        nodes.push_back(node);
    }
    
    std::vector<AstNode*> nodes;
    std::vector<VarDeclNode*> vars;
    CodeBlockNode* body;
};

