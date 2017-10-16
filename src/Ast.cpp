#include "Ast.hpp"
#include "AstVisitor.hpp"

void IntegerNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void IntVarFactor::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void OneDimensionalListFactor::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void BinaryOpNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void UnaryOpNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void IntLValueNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void OneDimensionalListLValueNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void LetStatementNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void CodeBlockNode::accept(AstVisitor& visitor)
{
    for(StatementNode* node : statements)
        node->accept(visitor);
}

void ForLoopNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void LabelNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void GotoNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void WhileLoopNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void IfNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void PrintNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void PromptNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void InputNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

