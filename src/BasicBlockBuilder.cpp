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
            
            gotoNode->targetBlock = labelBlocks[gotoNode->labelName];
            splitCurrentBlock();
            continue;
        }
        
        if(dynamic_cast<IfNode*>(s))
        {
            splitCurrentBlock();
            continue;
        }
    }
}

CodeBlockNode* BasicBlockBuilder::putBasicBlocksIntoCodeBlock()
{
    CodeBlockNode* node = ast.addCodeBlockNode();
    
    for(auto block : blocks)
    {
        if(block->statements.size() != 0)
            node->addStatement(block);
    }
    
    return node;
}

void BasicBlockBuilder::labelBlocksIds()
{
    int id = 0;
    
    for(auto block : blocks)
        block->setId(id++);
}

