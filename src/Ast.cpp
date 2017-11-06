#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "BasicBlockBuilder.hpp"

void Ast::accept(AstVisitor& v)
{
    body->accept(v);
}

void Ast::accepVars(AstVisitor& v)
{
    v.visitVars(vars);
}

void Ast::splitIntoBasicBlocks()
{
    BasicBlockBuilder builder(*this);
    setBody(builder.buildBasicBlocks());
}

void IntegerNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void IntegerNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}

void IntVarFactor::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void IntVarFactor::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}

void OneDimensionalListFactor::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void OneDimensionalListFactor::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}

void BinaryOpNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void BinaryOpNode::acceptRecursive(AstVisitor& v)
{
    left->acceptRecursive(v);
    right->acceptRecursive(v);
    v.visit(this);
}

void UnaryOpNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void UnaryOpNode::acceptRecursive(AstVisitor& v)
{
    value->acceptRecursive(v);
    v.visit(this);
}

void IntLValueNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void OneDimensionalListLValueNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void EndNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}


void LetStatementNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void LetStatementNode::acceptRecursive(AstVisitor& visitor)
{
    leftSide->acceptRecursive(visitor);
    rightSide->acceptRecursive(visitor);
    
    visitor.visit(this);
}

void CodeBlockNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
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

void GotoNode::acceptRecursive(AstVisitor& visitor)
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

void IfNode::acceptRecursive(AstVisitor& visitor)
{
    condition->acceptRecursive(visitor);
    body->acceptRecursive(visitor);
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

void RemNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void CodeBlockNode::acceptRecursive(AstVisitor& v)
{
    for(auto node : statements)
        node->acceptRecursive(v);
        
    v.visit(this);
}

void BasicBlockNode::acceptRecursive(AstVisitor& v)
{
    for(auto node : statements)
        node->acceptRecursive(v);
        
    v.visit(this);
}

void IntLValueNode::acceptRecursive(AstVisitor& visitor)
{
    visitor.visit(this);
}

void OneDimensionalListLValueNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}




