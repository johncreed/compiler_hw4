#ifndef __REGISTER_MANAGER__
#define __REGISTER_MANAGER__
#include "header.h"

// ARGUMENTS and RETURN REGISTER
#define REGISTER0_BEGIN 0
#define REGISTER0_END 8
// TEMPORARY REGISTER
#define REGISTER1_BEGIN 8
#define REGISTER1_END 19
// CALLEE SAVED REGISTER
#define REGISTER2_BEGIN 8
#define REGISTER2_END 29
// SPECIAL REGISTER
#define FRAME_POINTER 29
#define LINK_REGISTER 30
#define SP 31
#define REGISTER_SIZE 32

#define DEBUG_REGISTER 0

void initRegister();

int getR2Size();

void allocR0Register(AST_NODE *node, int i, REGISTER_TYPE type);
void allocR1Register(AST_NODE *node, REGISTER_TYPE type);
void allocR2Register(AST_NODE *node, REGISTER_TYPE type);

void StoW(AST_NODE *node);
void WtoS(AST_NODE *node);

// int allocR2();
// int freeR2(int i);
void freeRegister(AST_NODE *node);
int hasAllocRegister(AST_NODE *node);
#endif
