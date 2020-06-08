#ifndef __ARM_GENERATOR_H__
#define __ARM_GENERATOR_H__

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

#endif
