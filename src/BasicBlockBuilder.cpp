#include <list>

#include "BasicBlockBuilder.hpp"
#include "Error.hpp"

CodeBlockNode* BasicBlockBuilder::buildBasicBlocks()
{
    createBlocksForLabels();
    currentBlock = blocks.insert(blocks.begin(), ast.addBasicBlockNode());
    addCodeBlock(ast.getBody());
    labelBlocksIds();
    
    return putBasicBlocksIntoCodeBlock();
    
}

void BasicBlockBuilder::createBlocksForLabels()
{
    for(std::string name : ast.getAllLabelNames())
        labelBlocks[name] = ast.addBasicBlockNode();
}

void BasicBlockBuilder::beginLabelBlock(LabelNode* node)
{
    insertBlockAfterCurrentBlock(labelBlocks[node->name]);
}

void BasicBlockBuilder::splitCurrentBlock()
{
    insertBlockAfterCurrentBlock(ast.addBasicBlockNode());
}

void BasicBlockBuilder::insertBlockAfterCurrentBlock(BasicBlockNode* node)
{
    auto nextBlock = currentBlock;
    ++nextBlock;
    currentBlock = blocks.insert(nextBlock, node);
}

void BasicBlockBuilder::addCodeBlock(CodeBlockNode* node)
{
    for(auto s : node->statements)
    {
        if(CodeBlockNode* blockNode = dynamic_cast<CodeBlockNode*>(s))
        {
            addCodeBlock(blockNode);
            continue;
        }
        
        if(LabelNode* labelNode = dynamic_cast<LabelNode*>(s))
        {
            beginLabelBlock(labelNode);
        }
        
        (*currentBlock)->addStatement(s);
        
        if(GotoNode* gotoNode = dynamic_cast<GotoNode*>(s))
        {
            if(labelBlocks.count(gotoNode->labelName) == 0)
            {
                throw CompileError("No such label: " + gotoNode->labelName, "Basic block partitioning", gotoNode->line, gotoNode->col);
            }
            
            addGotoNodeTargetBlock(gotoNode);
            
            splitCurrentBlock();
            continue;
        }
        
        if(IfNode* ifNode = dynamic_cast<IfNode*>(s))
        {
            if(GotoNode* gotoNode = dynamic_cast<GotoNode*>(ifNode->body))
            {
                addGotoNodeTargetBlock(gotoNode);
            }
            else
            {
                throw CompileError("If contains statement other than goto", "Basic block partitioning", -1, -1);
            }
            
            splitCurrentBlock();
            continue;
        }
    }
}

void BasicBlockBuilder::addGotoNodeTargetBlock(GotoNode* gotoNode)
{
    gotoNode->targetBlock = labelBlocks[gotoNode->labelName];
    gotoNode->targetBlock->addPredecessor(*currentBlock);
    (*currentBlock)->addSuccessor(gotoNode->targetBlock);
}

CodeBlockNode* BasicBlockBuilder::putBasicBlocksIntoCodeBlock()
{
    CodeBlockNode* node = ast.addCodeBlockNode();
    
    for(auto block : blocks)
    {
        if(!block->deleted)
            node->addStatement(block);
    }
    
    return node;
}

void BasicBlockBuilder::labelBlocksIds()
{
    int id = 0;
    
    auto prev = blocks.end();
    
    for(auto block = blocks.begin(); block != blocks.end(); ++block)
    {
        if(prev != blocks.end() && !(*prev)->blockEndsInGoto())
        {
            (*block)->addPredecessor(*prev);
            (*prev)->addSuccessor(*block);
        }
        
        if((*block)->predecessors.size() == 0 && id != 0)
            (*block)->deleted = true;
            
        if(!(*block)->deleted)
        {
            (*block)->setId(id++);
            prev = block;
        }
    }
}

