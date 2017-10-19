#pragma once

#include <map>
#include <list>

#include "Ast.hpp"

class BasicBlockBuilder
{
public:
    BasicBlockBuilder(Ast& ast_) : ast(ast_) { }
    
    CodeBlockNode* buildBasicBlocks();
    
private:
    void createBlocksForLabels();
    void splitCurrentBlock();
    void insertBlockAfterCurrentBlock(BasicBlockNode* node);
    void beginLabelBlock(LabelNode* node);
    void addCodeBlock(CodeBlockNode* node);
    
    CodeBlockNode* putBasicBlocksIntoCodeBlock();
    
    Ast& ast;
    std::map<std::string, BasicBlockNode*> labelBlocks;
    std::list<BasicBlockNode*> blocks;
    std::list<BasicBlockNode*>::iterator currentBlock;
};

