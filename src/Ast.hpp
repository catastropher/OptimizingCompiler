#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <set>

#include "Token.hpp"

struct AstVisitor;

struct AstNode
{
    virtual void accept(AstVisitor& v) { }
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
    void accept(AstVisitor& v);
    
    int value;
};

struct IntDeclNode;

struct IntVarFactor : FactorNode
{
    IntVarFactor(IntDeclNode* var_) : var(var_) { }
    void accept(AstVisitor& v);
    
    IntDeclNode* var;
};

struct OneDimensionalListDecl;

struct OneDimensionalListFactor : FactorNode
{
    OneDimensionalListFactor(OneDimensionalListDecl* var_, ExpressionNode* index_)
        : var(var_), index(index_) { }
    
    void accept(AstVisitor& v);
    
    OneDimensionalListDecl* var;
    ExpressionNode* index;
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
    
    void accept(AstVisitor& v);
    
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
    
    void accept(AstVisitor& v);
    
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

struct EndNode : StatementNode
{
    void accept(AstVisitor& v);
};

struct LValueNode : AstNode
{
    virtual FactorNode* getFactorNode() = 0;
};

struct IntLValueNode : LValueNode
{
    IntLValueNode(IntDeclNode* var_) : var(var_) { }
    
    void accept(AstVisitor& v);
    
    FactorNode* getFactorNode()
    {
        return new IntVarFactor(var);
    }
    
    IntDeclNode* var;
};

struct OneDimensionalListLValueNode : LValueNode
{
    OneDimensionalListLValueNode(OneDimensionalListDecl* var_, ExpressionNode* index_)
        : var(var_), index(index_) { }
        
    void accept(AstVisitor& v);
    
    FactorNode* getFactorNode()
    {
        return new OneDimensionalListFactor(var, index);
    }
    
    OneDimensionalListDecl* var;
    ExpressionNode* index;
};

struct LetStatementNode : StatementNode
{
    LetStatementNode(LValueNode* leftSide_, ExpressionNode* rightSide_) : leftSide(leftSide_), rightSide(rightSide_) { }
    
    void accept(AstVisitor& v);
    
    LValueNode* leftSide;
    ExpressionNode* rightSide;
};

struct CodeBlockNode : StatementNode
{
    CodeBlockNode() : needCurlyBraces(true) { }
    
    void addStatement(StatementNode* node)
    {
        statements.push_back(node);
    }
    
    void disableCurlyBraces()
    {
        needCurlyBraces = false;
    }
    
    void accept(AstVisitor& v);
    
    std::vector<StatementNode*> statements;
    bool needCurlyBraces;
};

struct ForLoopNode : StatementNode
{
    ForLoopNode(LValueNode* var_, ExpressionNode* lower, ExpressionNode* upper, ExpressionNode* inc, CodeBlockNode* body_)
        : var(var_), lowerBound(lower), upperBound(upper), increment(inc), body(body_) { }
    
    void accept(AstVisitor& v);
    
    LValueNode* var;
    ExpressionNode* lowerBound;
    ExpressionNode* upperBound;
    ExpressionNode* increment;
    CodeBlockNode* body;
};

struct LabelNode : StatementNode
{
    LabelNode(std::string name_, int line_, int col_)
        : name(name_), line(line_), col(col_) { }
    
    void accept(AstVisitor& v);
    
    std::string name;
    int line;
    int col;
};

struct BasicBlockNode;

struct GotoNode : StatementNode
{
    GotoNode(std::string labelName_, int line_, int col_)
        : labelName(labelName_), line(line_), col(col_), targetBlock(nullptr) { }
    
    void accept(AstVisitor& v);
    
    std::string labelName;
    int line;
    int col;
    BasicBlockNode* targetBlock;
};

struct WhileLoopNode : StatementNode
{
    WhileLoopNode(ExpressionNode* condition_, CodeBlockNode* body_)
        : condition(condition_), body(body_) { }
    
    void accept(AstVisitor& v);
    
    ExpressionNode* condition;
    CodeBlockNode* body;
};

struct IfNode : StatementNode
{
    IfNode(ExpressionNode* condition_, StatementNode* body_)
        : condition(condition_), body(body_) { }
        
    void accept(AstVisitor& v);;
    
    ExpressionNode* condition;
    StatementNode* body;
};

struct PrintNode : StatementNode
{
    PrintNode(ExpressionNode* value_) : value(value_) { }
    
    void accept(AstVisitor& v);;
    
    ExpressionNode* value;
};

struct PromptNode : StatementNode
{
    PromptNode(std::string str_) : str(str_) { }
    
    void accept(AstVisitor& v);
    
    std::string str;
};

struct InputNode : StatementNode
{
    InputNode(LValueNode* var_) : var(var_) { }
    
    void accept(AstVisitor& v);
    
    LValueNode* var;
};

struct BasicBlockNode : CodeBlockNode
{
    BasicBlockNode()
    {
        needCurlyBraces = false;
    }
};

struct RemNode : StatementNode
{
    RemNode(std::string text_) : text(text_) { }
    
    void accept(AstVisitor& v);
    
    std::string text;
};

class Ast
{
public:
    void accept(AstVisitor& v);
    void accepVars(AstVisitor& v);
    
    IntegerNode* newIntegerNode(int value)
    {
        IntegerNode* newNode = new IntegerNode(value);
        addNode(newNode);
        return newNode;
    }
    
    IntVarFactor* addIntVarFactor(IntDeclNode* var)
    {
        auto newNode = new IntVarFactor(var);
        addNode(newNode);
        return newNode;
    }
    
    OneDimensionalListFactor* addOneDimensionalListFactor(OneDimensionalListDecl* var, ExpressionNode* index)
    {
        auto newNode = new OneDimensionalListFactor(var, index);
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
        return newNode;
    }
    
    OneDimensionalListDecl* add1DListVar(std::string name, int line, int col, int totalElements)
    {
        OneDimensionalListDecl* newNode = new OneDimensionalListDecl(name, line, col, totalElements);
        vars.push_back(newNode);
        return newNode;
    }
    
    IntLValueNode* addIntLValue(IntDeclNode* varDecl)
    {
        IntLValueNode* newNode = new IntLValueNode(varDecl);
        addNode(newNode);
        return newNode;
    }
    
    OneDimensionalListLValueNode* addOneDimensionalListLValueNode(OneDimensionalListDecl* var, ExpressionNode* index)
    {
        auto newNode = new OneDimensionalListLValueNode(var, index);
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
    
    LabelNode* addLabelNode(std::string name, int line, int col)
    {
        LabelNode* newNode = new LabelNode(name, line, col);
        addNode(newNode);
        
        if(labelNames.count(name) != 0)
            throw "Label " + name + " already exists";
            
        labelNames.insert(name);
        return newNode;
    }
    
    GotoNode* addGotoNode(std::string targetLabel, int line, int col)
    {
        GotoNode* newNode = new GotoNode(targetLabel, line, col);
        addNode(newNode);
        return newNode;
    }
    
    WhileLoopNode* addWhileLoopNode(ExpressionNode* condition, CodeBlockNode* body_)
    {
        auto newNode = new WhileLoopNode(condition, body_);
        addNode(newNode);
        return newNode;
    }
    
    IfNode* addIfNode(ExpressionNode* condition, StatementNode* body_)
    {
        auto newNode = new IfNode(condition, body_);
        addNode(newNode);
        return newNode;
    }
    
    InputNode* addInputNode(LValueNode* var)
    {
        auto newNode = new InputNode(var);
        addNode(newNode);
        return newNode;
    }
    
    PromptNode* addPromptNode(std::string str)
    {
        auto newNode = new PromptNode(str);
        addNode(newNode);
        return newNode;
    }
    
    PrintNode* addPrintNode(ExpressionNode* value)
    {
        auto newNode = new PrintNode(value);
        addNode(newNode);
        return newNode;
    }
    
    EndNode* addEndNode()
    {
        auto newNode = new EndNode;
        addNode(newNode);
        return newNode;
    }
    
    BasicBlockNode* addBasicBlockNode()
    {
        auto newNode = new BasicBlockNode;
        addNode(newNode);
        return newNode;
    }
    
    RemNode* addRemNode(std::string text)
    {
        auto newNode = new RemNode(text);
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
    
    void setTitle(std::string title_)
    {
        title = title_;
    }
    
    std::string getTitle()
    {
        return title;
    }
    
    std::set<std::string>& getAllLabelNames()
    {
        return labelNames;
    }
    
    CodeBlockNode* getBody()
    {
        return body;
    }
    
    void splitIntoBasicBlocks();
    
    ~Ast()
    {
        std::cout << "Total nodes created in AST: " << nodes.size() << std::endl;
        
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
    std::string title;
    std::set<std::string> labelNames;
};

