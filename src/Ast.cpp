#include "Ast.hpp"
#include "AstVisitor.hpp"
#include "BasicBlockBuilder.hpp"
#include "VariableUsageCounter.hpp"

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
    if(visitor.visitLetBefore)
    {
        visitor.visit(this);
        visitor.visit((StatementNode*)this);
    }
    
    visitor.enterNode(leftSide);
    leftSide->acceptRecursive(visitor);
    leftSide = dynamic_cast<LValueNode*>(visitor.lastNode());
    visitor.exitNode(leftSide);
    
    visitor.enterNode(rightSide);
    rightSide->acceptRecursive(visitor);
    rightSide = dynamic_cast<ExpressionNode*>(visitor.lastNode());
    visitor.exitNode(rightSide);
    
    if(!visitor.visitLetBefore)
    {
        visitor.visit(this);
        visitor.visit((StatementNode*)this);
    }
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
    visitor.visit((StatementNode*)this);
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
    visitor.visit(this);
    visitor.visit((StatementNode*)this);
    
    visitor.enterNode(condition);
    condition->acceptRecursive(visitor);
    condition = dynamic_cast<ExpressionNode*>(visitor.lastNode());
    visitor.exitNode(condition);
    
    visitor.enterNode(body);
    body->acceptRecursive(visitor);
    body = dynamic_cast<StatementNode*>(visitor.lastNode());
    visitor.exitNode(body);
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
    v.visit((StatementNode*)this);
    
    for(auto& node : statements)
    {
        if(node->markedAsDead)
            continue;
        
        v.enterNode(node);
        node->acceptRecursive(v);
        node = dynamic_cast<StatementNode*>(v.lastNode());
        v.exitNode(node);
    }
}

void BasicBlockNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
    v.visit((StatementNode*)this);
    
    for(auto& node : statements)
    {
        if(node->markedAsDead)
            continue;
        
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
    v.visit((StatementNode*)this);
}

void PrintNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
    v.visit((StatementNode*)this);
    
    v.enterNode(value);
    value->acceptRecursive(v);
    value = dynamic_cast<ExpressionNode*>(v.lastNode());
    v.exitNode(value);
}

void InputNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
    v.visit((StatementNode*)this);
    
    v.enterNode(var);
    var->acceptRecursive(v);
    var = dynamic_cast<LValueNode*>(v.lastNode());
    v.exitNode(var);
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

void SsaIntLValueNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}

void InputIntNode::accept(AstVisitor& v)
{
    v.visit(this);
}

void InputIntNode::acceptRecursive(AstVisitor& v)
{
    v.visit(this);
}

void Ast::eliminateUnusedVars()
{
    VariableUsageCounter counter(body);
    auto count = counter.countVarUses();
    std::map<IntDeclNode*, int> intVarCounts;
    
    for(auto var : count)
    {
        intVarCounts[var.first->var] += var.second;
    }
    
    for(auto var : vars)
    {
        if(auto intVar = dynamic_cast<IntDeclNode*>(var))
        {
            if(intVarCounts[intVar] == 0)
            {
                printf("Eliminated variable %s\n", intVar->name.c_str());
                intVar->eliminated = true;
            }
        }
    }
}

void Ast::defaultInitializeVars()
{
    std::vector<StatementNode*> varInit;
    
    for(auto var : vars)
    {
        if(auto intVar = dynamic_cast<IntDeclNode*>(var))
        {
            varInit.push_back
            (
                addLetStatementNode(addIntLValue(intVar), newIntegerNode(0))
            );
        }
    }
    
    if(varInit.size() != 0)
    {
        varInit.insert(varInit.begin(), addRemNode("--- Begin default variable initializations ---"));
        varInit.push_back(addRemNode("--- End default variable initializations ---"));
        body->statements.insert(body->statements.begin(), varInit.begin(), varInit.end());
    }
}

FactorNode* IntLValueNode::getFactorNode(Ast& ast)
{
    return ast.addIntVarFactor(var);
}

FactorNode* OneDimensionalListLValueNode::getFactorNode(Ast& ast)
{
    return ast.addOneDimensionalListFactor(var, index);
}

