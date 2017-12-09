#pragma once

#include <stack>

#include "AstVisitor.hpp"
#include "Utils.hpp"

struct CodeGenerator : AstVisitor
{
    CodeGenerator() : currentIndent(0)
    {
        
    }
    
    void visitVars(std::vector<VarDeclNode*>& vars)
    {
        for(auto var : vars)
        {
            if(var->eliminated)
                continue;
            
            if(auto intVar = dynamic_cast<IntDeclNode*>(var))
            {
                addLine("int " + intVar->name + ";");
            }
            else if(auto listVar = dynamic_cast<OneDimensionalListDecl*>(var))
            {
                addLine("int " + listVar->name + "[" + std::to_string(listVar->totalElements) + "];");
            }
            else if(auto listVar = dynamic_cast<TwoDimensionalListDecl*>(var))
            {
                addLine("int " + listVar->name +
                    "[" + std::to_string(listVar->totalElements0) + "]" + 
                    "[" + std::to_string(listVar->totalElements1) + "];");
            }
            else if(auto listVar = dynamic_cast<ThreeDimensionalListDecl*>(var))
            {
                addLine("int " + listVar->name +
                    "[" + std::to_string(listVar->totalElements0) + "]" +
                    "[" + std::to_string(listVar->totalElements1) + "]" + 
                    "[" + std::to_string(listVar->totalElements2) + "];");
            }
        }
        
        addLine("");
    }
    
    void addReadIntPrototype()
    {
        addLine("int readInt();");
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
        
        addReadIntPrototype();
        
        addLine("int main()");
        ast.accept(*this);
        
        if(needsReadInt)
            addReadIntDef();
    }
    
    void addReadIntDef()
    {
        addLine("int readInt()");
        addLine("{");
        addLine("    int val;");
        addLine("    scanf(\"%d\", &val);");
        addLine("    return val;");
        addLine("}");
        addLine("");
    }
    
    void visit(PhiNode* node)
    {
        auto var = *node->joinNodes.begin();
        var->accept(*this);
        //push("{" + std::to_string(node->joinNodes.size()) + "}");
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
    
    void visit(TwoDimensionalListFactor* node)
    {
        node->index1->accept(*this);
        node->index0->accept(*this);
        
        std::string index0 = pop();
        std::string index1 = pop();
        
        push(node->var->name + "[" + index0 + "][" + index1 + "]");
    }
    
    void visit(ThreeDimensionalListFactor* node)
    {
        node->index2->accept(*this);
        node->index1->accept(*this);
        node->index0->accept(*this);
        
        std::string index0 = pop();
        std::string index1 = pop();
        std::string index2 = pop();
        
        push(node->var->name + "[" + index0 + "][" + index1 + "][" + index2 + "]");
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
    
    void visit(TwoDimensionalListLValueNode* node)
    {
        node->index1->accept(*this);
        node->index0->accept(*this);
        
        std::string index0 = pop();
        std::string index1 = pop();
        
        push(node->var->name + "[" + index0 + "][" + index1 + "]");
    }
    
    void visit(ThreeDimensionalListLValueNode* node)
    {
        node->index2->accept(*this);
        node->index1->accept(*this);
        node->index0->accept(*this);
        
        std::string index0 = pop();
        std::string index1 = pop();
        std::string index2 = pop();
        
        push(node->var->name + "[" + index0 + "][" + index1 + "][" + index2 + "]");
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
        if(dynamic_cast<PhiNode*>(node->rightSide))
            return;
        
        node->rightSide->accept(*this);
        node->leftSide->accept(*this);
        
        std::string left = pop();
        std::string right = pop();
        
        addLine(left + " = " + right + ";");
    }
    
    void visit(CodeBlockNode* node)
    {
        if(BasicBlockNode* basicBlockNode = dynamic_cast<BasicBlockNode*>(node))
        {
            addBasicBlockCommentLine(basicBlockNode);
        }
        
        if(node->needCurlyBraces)
        {
            addLine("{");
            ++currentIndent;
        }
        
        for(StatementNode* s : node->statements)
        {
            if(!s->markedAsDead)
                s->accept(*this);
        }
        
        if(node->needCurlyBraces)
        {
            --currentIndent;
            addLine("}");
            addLine("");
        }
    }
    
    void addBasicBlockCommentLine(BasicBlockNode* node)
    {
        std::string pred = containerToString(node->getPredecessorIds());
        
        if(node->id == 0)
            pred = "entry";
        
        std::string succ = containerToString(node->getSuccessorIds());
        
        if(succ == "")
            succ = "exit";
        
        addLine("// * block " + std::to_string(node->id) + " pred " + pred + " succ " + succ);
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
        
    }
    
    void visit(InputIntNode* node)
    {
        push("readInt()");
        needsReadInt = true;
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
    
    void visit(RemNode* node)
    {
        node->text[0] = toupper(node->text[0]);
        addLine("// " + node->text);
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
    bool needsReadInt;
};

