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
    
    v.enterNode(index);
    index->acceptRecursive(v);
    index = dynamic_cast<ExpressionNode*>(v.lastNode());
    v.exitNode(index);
}

void BinaryOpNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void BinaryOpNode::acceptRecursive(AstVisitor& v)
{
    v.enterNode(left);
    left->acceptRecursive(v);
    left = dynamic_cast<ExpressionNode*>(v.lastNode());
    v.exitNode(left);
    
    v.enterNode(right);
    right->acceptRecursive(v);
    right = dynamic_cast<ExpressionNode*>(v.lastNode());
    v.exitNode(right);
    
    v.visit(this);
}

void UnaryOpNode::accept(AstVisitor& visitor)
{
    visitor.visit(this);
}

void UnaryOpNode::acceptRecursive(AstVisitor& v)
{
    v.enterNode(value);
    value->acceptRecursive(v);
    value = dynamic_cast<ExpressionNode*>(v.lastNode());
    v.exitNode(value);
    
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
    visitor.enterNode(leftSide);
    leftSide->acceptRecursive(visitor);
    leftSide = dynamic_cast<LValueNode*>(visitor.lastNode());
    visitor.exitNode(leftSide);
    
    visitor.enterNode(rightSide);
    rightSide->acceptRecursive(visitor);
    rightSide = dynamic_cast<ExpressionNode*>(visitor.lastNode());
    visitor.exitNode(rightSide);
    
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
    visitor.enterNode(condition);
    condition->acceptRecursive(visitor);
    condition = dynamic_cast<ExpressionNode*>(condition);
    visitor.exitNode(condition);
    
    visitor.enterNode(body);
    body->acceptRecursive(visitor);
    body = dynamic_cast<StatementNode*>(body);
    visitor.exitNode(body);
    
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
    v.visit(this);
    
    for(auto& node : statements)
    {
        v.enterNode(node);
        node->acceptRecursive(v);
        node = dynamic_cast<StatementNode*>(v.lastNode());
        v.exitNode(node);
    }
}

void BasicBlockNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
    
    for(auto& node : statements)
    {
        v.enterNode(node);
        node->acceptRecursive(v);
        node = dynamic_cast<StatementNode*>(v.lastNode());
        v.exitNode(node);
    }
}

void IntLValueNode::acceptRecursive(AstVisitor& visitor)
{
    visitor.visit(this);
}

void OneDimensionalListLValueNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
    
    v.enterNode(index);
    index->acceptRecursive(v);
    index = dynamic_cast<ExpressionNode*>(v.lastNode());
    v.exitNode(index);
}

void PromptNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}

void PhiNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}

void PhiNode::accept(AstVisitor& v)
{
    v.visit(this);
}

void SsaIntVarFactor::accept(AstVisitor& v)
{
    v.visit(this);
}

void SsaIntVarFactor::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}




