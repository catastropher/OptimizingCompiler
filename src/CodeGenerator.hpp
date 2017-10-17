#pragma once

#include <stack>

#include "AstVisitor.hpp"

struct CodeGenerator : AstVisitor
{
    CodeGenerator() : currentIndent(0)
    {
        
    }
    
    void visitVars(std::vector<VarDeclNode*>& vars)
    {
        for(auto var : vars)
        {
            if(auto intVar = dynamic_cast<IntDeclNode*>(var))
            {
                addLine("int " + intVar->name + ";");
            }
            else if(auto listVar = dynamic_cast<OneDimensionalListDecl*>(var))
            {
                addLine("int " + listVar->name + + "[" + std::to_string(listVar->totalElements) + "];");
            }
        }
        
        addLine("");
    }
    
    void genCode(Ast& ast)
    {
        if(ast.getTitle() == "")
            ast.setTitle("Untitled");
        
        std::string title = ast.getTitle();
        title[0] = toupper(title[0]);
        
        addLine("// " + title);
        
        addLine("#include <stdio.h>");
        addLine("#include <stdlib.h>");
        
        addLine("");
        ast.accepVars(*this);
        
        addLine("int main()");
        ast.accept(*this);
    }
    
    void visit(IntegerNode* node)
    {
        push(std::to_string(node->value));
    }
    
    void visit(IntVarFactor* node)
    {
        push(node->var->name);
    }
    
    void visit(OneDimensionalListFactor* node)
    {
        node->index->accept(*this);
        
        std::string index = pop();
        push(node->var->name + "[" + index + "]");
    }
    
    void visit(IntLValueNode* node)
    {
        push(node->var->name);
    }
    
    void visit(OneDimensionalListLValueNode* node)
    {
        node->index->accept(*this);
        
        std::string index = pop();
        push(node->var->name + "[" + index + "]");
    }
    
    void visit(ExpressionNode* node)
    {
        throw "Bad expression type";
    }
    
    void visit(BinaryOpNode* node)
    {
        node->left->accept(*this);
        node->right->accept(*this);
        
        std::string op = Token::getTokenName(node->op);
        
        std::string right = pop();
        std::string left = pop();
        
        push("(" + left + " " + op + " " + right + ")");
    }
    
    void visit(UnaryOpNode* node)
    {
        node->value->accept(*this);
        push(Token::getTokenName(node->op) + "(" + pop() + ")");
    }
    
    void visit(LetStatementNode* node)
    {
        node->rightSide->accept(*this);
        node->leftSide->accept(*this);
        
        std::string left = pop();
        std::string right = pop();
        
        addLine(left + " = " + right + ";");
    }
    
    void visit(CodeBlockNode* node)
    {
        if(node->needCurlyBraces)
        {
            addLine("{");
            ++currentIndent;
        }
        
        for(StatementNode* s : node->statements)
            s->accept(*this);
        
        if(node->needCurlyBraces)
        {
            --currentIndent;
            addLine("}");
            addLine("");
        }
    }
    
    void visit(IfNode* node)
    {
        node->condition->accept(*this);
        
        std::string condition = pop();
        
        addLine("if(" + condition + ")");
        
        ++currentIndent;
        node->body->accept(*this);
        --currentIndent;
        
        addLine("");
    }
    
    void visit(WhileLoopNode* node)
    {
        node->condition->accept(*this);
        
        std::string condition = pop();
        
        addLine("while(" + condition + ")");
        
        node->body->accept(*this);
    }
    
    void visit(ForLoopNode* node)
    {
        node->var->accept(*this);
        node->lowerBound->accept(*this);
        node->upperBound->accept(*this);
        node->increment->accept(*this);
        
        std::string inc = pop();
        std::string upper = pop();
        std::string lower = pop();
        std::string var = pop();
        
        addLine("for(" + var + " = " + lower + "; " + var + " <= " + upper + "; " + var + " += " + inc + ")");
        node->body->accept(*this);
    }
    
    void visit(PrintNode* node)
    {
        node->value->accept(*this);
        addLine("printf(\"%d\", " + pop() + ");");
    }
    
    void visit(InputNode* node)
    {
        node->var->accept(*this);
        addLine("scanf(\"%d\", &" + pop() + ");");
    }
    
    void visit(PromptNode* node)
    {
        addLine("printf(\"%s\", " + node->str + ");");
    }
    
    void visit(EndNode* node)
    {
        addLine("return 0;");
    }
    
    void visit(LabelNode* node)
    {
        addLine(node->name + ":", false);
    }
    
    void visit(GotoNode* node)
    {
        addLine("goto " + node->labelName + ";");
    }
    
    std::string pop()
    {
        std::string top = expStack.top();
        expStack.pop();
        return top;
    }
    
    void push(std::string str)
    {
        expStack.push(str);
    }
    
    void addLine(std::string line, bool useIndent = true)
    {
        std::string indent;
        
        if(useIndent)
        {
            for(int i = 0; i < currentIndent; ++i)
                indent += "    ";
        }
        
        output.push_back(indent + line);
    }

    std::vector<std::string> output;
    int currentIndent;
    
    std::stack<std::string> expStack;
};

