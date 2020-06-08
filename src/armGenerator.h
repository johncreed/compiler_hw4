#ifndef __ARM_GENERATOR_H__
#define __ARM_GENERATOR_H__

#include "header.h"
#include "symbolTable.h"

// Node traversal
void visitProgramNode(AST_NODE *programNode);
void visitDeclarationNode(AST_NODE *declarationNode);
void visitDeclareFunction(AST_NODE *declarationNode);
void visitGeneralNode(AST_NODE *node);
void visitStmtNode(AST_NODE *stmtNode);
void visitFunctionCall(AST_NODE *functionCallNode);
void visitWriteFunction(AST_NODE *functionCallNode);
void visitDeclareIdList(AST_NODE *declarationNode,
                   SymbolAttributeKind isVariableOrTypeAttribute,
                   int ignoreArrayFirstDimSize);
void visitAssignmentStmt(AST_NODE *assignmentNode); 
void visitVariableLValue(AST_NODE *idNode); 
void visitExprRelatedNode(AST_NODE *exprRelatedNode); 
#endif
