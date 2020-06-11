#ifndef __ARM_GENERATOR_H__
#define __ARM_GENERATOR_H__

#include "header.h"
#include "symbolTable.h"

void openFile();

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
void visitExprNode(AST_NODE *exprNode);
void visitVariableRValue(exprRelatedNode);
void retrieveVariableOffset(AST_NODE *idNode, AST_NODE *offsetNode);
void visitBinaryExprNode(AST_NODE *exprNode);
void visitUnaryExprNode(AST_NODE *exprNode);
void visitWhileStmt(AST_NODE *whileNode);
#endif
