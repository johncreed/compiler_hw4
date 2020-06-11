#include "armGenerator.h"
#include "registerManager.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int DEBUG = 1;

char POST_BUFFER[1000000];

void outputPostBuffer() {
    fprintf(stdout, "%s", POST_BUFFER);
    sprintf(POST_BUFFER, "");
}

int aligned_size(int size, int aligned) {
    if (size % aligned > 0) {
        size = (size / aligned) * aligned + aligned;
    }
    return size;
}

// ARM routine
void storeLinkerAndSPRegister(int size) {
    printf("\tstp x29, x30, [sp, -%d]!\n", aligned_size(size, 8));
    printf("\tadd x29, sp, 0\n");
}

void loadLinkerAndSPRegister(int size) {
    printf("\tldp x29, x30, [sp], %d\n", aligned_size(size, 8));
    printf("\tret\n");
}

int getRegisterNumber(REGISTER_INFO *reg) { return reg->registerNumber; }

void getRegister(REGISTER_INFO *reg, char *buffer) {
    sprintf(buffer, "%c%d", reg->symbol, getRegisterNumber(reg));
}

REGISTER_INFO *getRegisterInfo(AST_NODE *node) {
    return &(node->register_info);
}

void MOV_RR(REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char R1[3], R2[3];
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tmov %s, %s\n", R1, R2);
}

void MOV_RC(REGISTER_INFO *reg, int val) {
    char R1[3];
    getRegister(reg, R1);
    printf("\tmov %s, %d\n", R1, val);
}

void FMOV_RR(REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char R1[3], R2[3];
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tfmov %s, %s\n", R1, R2);
}

// FLOAT ADD SUB MUL DIV

void FADD_RRR(REGISTER_INFO *regD, REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char Rd[3], R1[3], R2[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tfadd %s, %s, %s\n", Rd, R1, R2);
}

void FSUB_RRR(REGISTER_INFO *regD, REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char Rd[3], R1[3], R2[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tfsub %s, %s, %s\n", Rd, R1, R2);
}

void FMUL_RRR(REGISTER_INFO *regD, REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char Rd[3], R1[3], R2[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tfmul %s, %s, %s\n", Rd, R1, R2);
}

void FDIV_RRR(REGISTER_INFO *regD, REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char Rd[3], R1[3], R2[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tfdiv %s, %s, %s\n", Rd, R1, R2);
}

// ADD, SUB, MUL, DIV

void ADD_RRR(REGISTER_INFO *regD, REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char Rd[3], R1[3], R2[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tadd %s, %s, %s\n", Rd, R1, R2);
}

void SUB_RRR(REGISTER_INFO *regD, REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char Rd[3], R1[3], R2[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tsub %s, %s, %s\n", Rd, R1, R2);
}

void MUL_RRR(REGISTER_INFO *regD, REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char Rd[3], R1[3], R2[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tmul %s, %s, %s\n", Rd, R1, R2);
}

void DIV_RRR(REGISTER_INFO *regD, REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char Rd[3], R1[3], R2[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tsdiv %s, %s, %s\n", Rd, R1, R2);
}

void SXTW_R(REGISTER_INFO *reg) {
    reg->type = R_64;
    reg->symbol = 'x';
    printf("\tsxtw x%d, w%d\n", getRegisterNumber(reg), getRegisterNumber(reg));
}

void ADD_RRC(REGISTER_INFO *regD, REGISTER_INFO *reg1, int val) {
    char Rd[3], R1[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    printf("\tadd %s, %s, %d\n", Rd, R1, val);
}

void LSL_RRC(REGISTER_INFO *regD, REGISTER_INFO *reg1, int val) {
    char Rd[3], R1[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    printf("\tlsl %s, %s, %d\n", Rd, R1, val);
}

void FNEG_RR(REGISTER_INFO *regD, REGISTER_INFO *reg1) {
    char Rd[3], R1[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    printf("\tfneg %s, %s\n", Rd, R1);
}

void NEG_RR(REGISTER_INFO *regD, REGISTER_INFO *reg1) {
    char Rd[3], R1[3];
    getRegister(regD, Rd);
    getRegister(reg1, R1);
    printf("\tneg %s, %s\n", Rd, R1);
}

void FCMP_RR(REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char R1[3], R2[3];
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tfcmp %s, %s\n", R1, R2);
}

// Control flow Related instruction

void CMP_RC(REGISTER_INFO *reg, int val) {
    char R[3];
    getRegister(reg, R);
    printf("\tcmp %s, %d\n", R, val);
}

void CMP_RR(REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char R1[3], R2[3];
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tcmp %s, %s\n", R1, R2);
}

void CSET_R(REGISTER_INFO *reg1, char *flag) {
    char R1[3];
    getRegister(reg1, R1);
    printf("\tcset %s, %s\n", R1, flag);
}

void B_L(char *label) { printf("\tb %s\n", label); }

void BL_L(char *label) { printf("\tbl %s\n", label); }

void CBZ_RL(REGISTER_INFO *reg1, char *label) {
    char R1[3];
    getRegister(reg1, R1);
    printf("\tcbz %s, %s\n", R1, label);
}

void CBNZ_RL(REGISTER_INFO *reg1, char *label) {
    char R1[3];
    getRegister(reg1, R1);
    printf("\tcbnz %s, %s\n", R1, label);
}

// Memory access

void STR_RR(REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char R1[3], R2[3];
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tstr  %s, [sp, %s]\n", R1, R2);
}

void LDR_RR(REGISTER_INFO *reg1, REGISTER_INFO *reg2) {
    char R1[3], R2[3];
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tldr  %s, [sp, %s]\n", R1, R2);
}

void LDR_RC(REGISTER_INFO *reg1, char *val) {
    char R1[3];
    getRegister(reg1, R1);
    printf("\tldr  %s, %s\n", R1, val);
}

// Old A64 instruction

void LSL(AST_NODE *node, int size) {
    printf("\tlsl w%d, w%d, %d\n", node->registerNumber, node->registerNumber,
           size);
}

/*void MUL_RRR(AST_NODE *node_a, AST_NODE *node_b, AST_NODE *node_c) {
    printf("\tmul w%d, w%d, w%d\n", node_a->registerNumber,
           node_b->registerNumber, node_c->registerNumber);
}*/

void MOV(AST_NODE *node, int val) {
    printf("\tmov w%d, %d\n", node->registerNumber, val);
}

void STRSP(AST_NODE *node_a, AST_NODE *node_b) {
    printf("\tstr w%d, [sp, x%d]\n", node_a->registerNumber,
           node_b->registerNumber);
}

void SXTW(AST_NODE *node) {
    printf("\tsxtw x%d, w%d\n", node->registerNumber, node->registerNumber);
}

// *********************************************//
//
// GET Label

int BRANCH_LABEL_COUNTER = 4;
int CONST_LABEL_COUNTER = 4;

void getBranchLabel(char *buffer) {
    sprintf(buffer, ".L%d", BRANCH_LABEL_COUNTER);
    BRANCH_LABEL_COUNTER++;
}

void getCONSTLabel(char *buffer) {
    sprintf(buffer, ".LC%d", CONST_LABEL_COUNTER);
    CONST_LABEL_COUNTER++;
}

void floatConst(char *label, float fval) {
    int *int_ptr = (int *)(&fval);
    char buffer[128];

    sprintf(buffer, "%s:\n", label);
    sprintf(buffer, "%s\t.word\t%d\n", buffer, (*int_ptr));
    sprintf(POST_BUFFER, "%s%s", POST_BUFFER, buffer);
}

// *********************************************//

int getOffset(char *symbolName) {
    SymbolTableEntry *entry = retrieveSymbol(symbolName);

    if (DEBUG > 0) {
        if (entry == NULL) {
            fprintf(stderr, "entry  is null %s\n", symbolName);
            exit(1);
            return 0;
        } else {
            fprintf(stderr, "Get id: %s offset: %d\n", symbolName,
                    entry->attribute->offset);
            return entry->attribute->offset;
        }
    }
    return entry->attribute->offset;
}

void armGenerator(AST_NODE *root) {
    visitProgramNode(root);
    return;
}

void visitProgramNode(AST_NODE *programNode) {
    AST_NODE *traverseDeclaration = programNode->child;
    while (traverseDeclaration) {
        if (traverseDeclaration->nodeType == VARIABLE_DECL_LIST_NODE) {
            visitGeneralNode(traverseDeclaration);
        } else if (traverseDeclaration->nodeType == DECLARATION_NODE) {
            visitDeclarationNode(traverseDeclaration);
        } else {
            fprintf(stderr, "Node not match in visitProgramNode\n");
            exit(1);
        }

        if (traverseDeclaration->dataType == ERROR_TYPE) {
            programNode->dataType = ERROR_TYPE;
        }

        traverseDeclaration = traverseDeclaration->rightSibling;
    }
    return;
}

void visitDeclareIdList(AST_NODE *declarationNode,
                        SymbolAttributeKind isVariableOrTypeAttribute,
                        int ignoreArrayFirstDimSize) {
    AST_NODE *typeNode = declarationNode->child;
    AST_NODE *traverseIDList = typeNode->rightSibling;
    while (traverseIDList) {
        if (DEBUG > 0) {
            int offset =
                traverseIDList->semantic_value.identifierSemanticValue.offset;
            char *name = traverseIDList->semantic_value.identifierSemanticValue
                             .identifierName;
            getOffset(name);
        }
        switch (traverseIDList->semantic_value.identifierSemanticValue.kind) {
        case NORMAL_ID:
            break;
        case ARRAY_ID:
            break;
        case WITH_INIT_ID:
            // TODO assign value to sp + offset
            break;
        default:
            break;
        }
        traverseIDList = traverseIDList->rightSibling;
    }
}

void visitDeclarationNode(AST_NODE *declarationNode) {
    AST_NODE *typeNode = declarationNode->child;

    switch (declarationNode->semantic_value.declSemanticValue.kind) {
    case VARIABLE_DECL:
        visitDeclareIdList(declarationNode, VARIABLE_ATTRIBUTE, 0);
        break;
    /*
    case TYPE_DECL:
        declareIdList(declarationNode, TYPE_ATTRIBUTE, 0);
        break;
    */
    case FUNCTION_DECL:
        visitDeclareFunction(declarationNode);
        break;
        /*
        case FUNCTION_PARAMETER_DECL:
            declareIdList(declarationNode, VARIABLE_ATTRIBUTE, 1);
            break;
        */
    }
    return;
}

void visitDeclareFunction(AST_NODE *declarationNode) {
    AST_NODE *returnTypeNode = declarationNode->child;
    AST_NODE *functionNameID = returnTypeNode->rightSibling;
    AST_NODE *parameterListNode = functionNameID->rightSibling;

    initRegister();
    printf(".text\n");
    printf(
        "_start_%s:\n",
        functionNameID->semantic_value.identifierSemanticValue.identifierName);
    storeLinkerAndSPRegister(
        declarationNode->semantic_value.declSemanticValue.frameSize);

    /*TODO bonus
     * parse parameter
    AST_NODE *traverseParameter = parameterListNode->child;
    if (traverseParameter) {
        processDeclarationNode(traverseParameter);
    }
    while (traverseParameter) {
        processDeclarationNode(traverseParameter);
    }
    */

    AST_NODE *blockNode = parameterListNode->rightSibling;
    AST_NODE *traverseListNode = blockNode->child;
    while (traverseListNode) {
        visitGeneralNode(traverseListNode);
        traverseListNode = traverseListNode->rightSibling;
    }

    loadLinkerAndSPRegister(
        declarationNode->semantic_value.declSemanticValue.frameSize);

    outputPostBuffer();
}

void visitGeneralNode(AST_NODE *node) {
    AST_NODE *traverseChildren = node->child;
    switch (node->nodeType) {
    case VARIABLE_DECL_LIST_NODE:
        while (traverseChildren) {
            visitDeclarationNode(traverseChildren);
            traverseChildren = traverseChildren->rightSibling;
        }
        break;
    case STMT_LIST_NODE:
        while (traverseChildren) {
            visitStmtNode(traverseChildren);
            traverseChildren = traverseChildren->rightSibling;
        }
        break;
    /*
    case NONEMPTY_ASSIGN_EXPR_LIST_NODE:
        while (traverseChildren) {
            checkAssignOrExpr(traverseChildren);
            if (traverseChildren->dataType == ERROR_TYPE) {
                node->dataType = ERROR_TYPE;
            }
            traverseChildren = traverseChildren->rightSibling;
        }
        break;
    case NONEMPTY_RELOP_EXPR_LIST_NODE:
        while (traverseChildren) {
            allocR2Register(traverseChildren, R_32);
            visitExprRelatedNode(traverseChildren);
            if (traverseChildren->dataType == ERROR_TYPE) {
                node->dataType = ERROR_TYPE;
            }
            traverseChildren = traverseChildren->rightSibling;
            freeRegister(traverseChildren);
        }
        break;
    */
    case NUL_NODE:
        break;
    default:
        break;
    }
}

void visitConstValueNode(AST_NODE *constValueNode) {
    char float_label[64];
    switch (constValueNode->semantic_value.const1->const_type) {
    case INTEGERC:
        MOV_RC(getRegisterInfo(constValueNode),
               constValueNode->semantic_value.exprSemanticValue.constEvalValue
                   .iValue);
        break;
    case FLOATC:

        // Create float constant label
        getCONSTLabel(float_label);
        floatConst(float_label, constValueNode->semantic_value.exprSemanticValue
                                    .constEvalValue.fValue);

        // Load constant label
        AST_NODE tmp;
        allocR2Register(&tmp, R_32);
        LDR_RC(getRegisterInfo(&tmp), float_label);
        FMOV_RR(getRegisterInfo(constValueNode), getRegisterInfo(&tmp));
        freeRegister(&tmp);
        break;
    /*
case STRINGC:
    constValueNode->dataType = CONST_STRING_TYPE;
    break;
    */
    default:
        printf("Unhandle case in void processConstValueNode(AST_NODE* "
               "constValueNode)\n");
        constValueNode->dataType = ERROR_TYPE;
        break;
    }
}

void visitINTBinaryExprNode(AST_NODE *exprNode) {
    AST_NODE *leftOp = exprNode->child;
    AST_NODE *rightOp = leftOp->rightSibling;
    allocR2Register(leftOp, R_32);
    allocR2Register(rightOp, R_32);
    visitExprRelatedNode(leftOp);
    visitExprRelatedNode(rightOp);

    REGISTER_INFO *R1 = getRegisterInfo(leftOp);
    REGISTER_INFO *R2 = getRegisterInfo(rightOp);
    REGISTER_INFO *Rd = getRegisterInfo(exprNode);
    switch (exprNode->semantic_value.exprSemanticValue.op.binaryOp) {
    case BINARY_OP_ADD:
        ADD_RRR(Rd, R1, R2);
        break;
    case BINARY_OP_SUB:
        SUB_RRR(Rd, R1, R2);
        break;
    case BINARY_OP_MUL:
        MUL_RRR(Rd, R1, R2);
        break;
    case BINARY_OP_DIV:
        DIV_RRR(Rd, R1, R2);
        break;
    case BINARY_OP_EQ:
        CMP_RR(R1, R2);
        CSET_R(Rd, "EQ");
        break;
    case BINARY_OP_GE:
        CMP_RR(R1, R2);
        CSET_R(Rd, "GE");
        break;
    case BINARY_OP_LE:
        CMP_RR(R1, R2);
        CSET_R(Rd, "LE");
        break;
    case BINARY_OP_NE:
        CMP_RR(R1, R2);
        CSET_R(Rd, "NE");
        break;
    case BINARY_OP_GT:
        CMP_RR(R1, R2);
        CSET_R(Rd, "GT");
        break;
    case BINARY_OP_LT:
        CMP_RR(R1, R2);
        CSET_R(Rd, "LT");
        break;
    case BINARY_OP_AND:
        CMP_RC(R1, 1);
        CSET_R(R1, "EQ");
        CMP_RC(R2, 1);
        CSET_R(R2, "EQ");
        ADD_RRR(Rd, R1, R2);
        CMP_RC(Rd, 2);
        CSET_R(Rd, "EQ");
        break;
    case BINARY_OP_OR:
        CMP_RC(R1, 1);
        CSET_R(R1, "EQ");
        CMP_RC(R2, 1);
        CSET_R(R2, "EQ");
        ADD_RRR(Rd, R1, R2);
        CMP_RC(Rd, 1);
        CSET_R(Rd, "GE");
        break;
    }

    freeRegister(leftOp);
    freeRegister(rightOp);
}

void visitFLOATBinaryExprNode(AST_NODE *exprNode) {
    AST_NODE *leftOp = exprNode->child;
    AST_NODE *rightOp = leftOp->rightSibling;
    allocR2Register(leftOp, S_32);
    allocR2Register(rightOp, S_32);
    visitExprRelatedNode(leftOp);
    visitExprRelatedNode(rightOp);

    REGISTER_INFO *R1 = getRegisterInfo(leftOp);
    REGISTER_INFO *R2 = getRegisterInfo(rightOp);
    REGISTER_INFO *Rd = getRegisterInfo(exprNode);
    switch (exprNode->semantic_value.exprSemanticValue.op.binaryOp) {
    case BINARY_OP_ADD:
        FADD_RRR(Rd, R1, R2);
        break;
    case BINARY_OP_SUB:
        FSUB_RRR(Rd, R1, R2);
        break;
    case BINARY_OP_MUL:
        FMUL_RRR(Rd, R1, R2);
        break;
    case BINARY_OP_DIV:
        FDIV_RRR(Rd, R1, R2);
        break;
    case BINARY_OP_EQ:
        FCMP_RR(R1, R2);
        StoW(exprNode);
        CSET_R(Rd, "EQ");
        break;
    case BINARY_OP_GE:
        FCMP_RR(R1, R2);
        StoW(exprNode);
        CSET_R(Rd, "GE");
        break;
    case BINARY_OP_LE:
        FCMP_RR(R1, R2);
        StoW(exprNode);
        CSET_R(Rd, "LE");
        break;
    case BINARY_OP_NE:
        FCMP_RR(R1, R2);
        StoW(exprNode);
        CSET_R(Rd, "NE");
        break;
    case BINARY_OP_GT:
        FCMP_RR(R1, R2);
        StoW(exprNode);
        CSET_R(Rd, "GT");
        break;
    case BINARY_OP_LT:
        FCMP_RR(R1, R2);
        StoW(exprNode);
        CSET_R(Rd, "LT");
        break;
    case BINARY_OP_AND:
        // Leftop and Rightop should be w not d
        CMP_RC(R1, 1);
        CSET_R(R1, "EQ");
        CMP_RC(R2, 1);
        CSET_R(R2, "EQ");
        ADD_RRR(Rd, R1, R2);
        CMP_RC(Rd, 2);
        CSET_R(Rd, "EQ");
        break;
    case BINARY_OP_OR:
        // Leftop and Rightop should be w not d
        CMP_RC(R1, 1);
        CSET_R(R1, "EQ");
        CMP_RC(R2, 1);
        CSET_R(R2, "EQ");
        ADD_RRR(Rd, R1, R2);
        CMP_RC(Rd, 1);
        CSET_R(Rd, "GE");
        break;
    }

    freeRegister(leftOp);
    freeRegister(rightOp);
}

void visitBinaryExprNode(AST_NODE *exprNode) {
    AST_NODE *operand = exprNode->child;
    if (operand->dataType == INT_TYPE) {
        visitINTBinaryExprNode(exprNode);
    } else if (operand->dataType == FLOAT_TYPE) {
        visitFLOATBinaryExprNode(exprNode);
    }
}

void visitUnaryExprNode(AST_NODE *exprNode) {
    AST_NODE *operand = exprNode->child;
    if (operand->dataType == INT_TYPE) {
        allocR2Register(operand, R_32);
        visitExprRelatedNode(operand);

        REGISTER_INFO *R1 = getRegisterInfo(operand);
        REGISTER_INFO *Rd = getRegisterInfo(exprNode);
        switch (exprNode->semantic_value.exprSemanticValue.op.unaryOp) {
        case UNARY_OP_POSITIVE:
            break;
        case UNARY_OP_NEGATIVE:
            NEG_RR(Rd, R1);
            break;
        case UNARY_OP_LOGICAL_NEGATION:
            CMP_RC(R1, 0);
            CSET_R(Rd, "EQ");
            break;
        default:
            break;
        }
        freeRegister(operand);
    } else if (operand->dataType == FLOAT_TYPE) {
        allocR2Register(operand, S_32);
        visitExprRelatedNode(operand);

        REGISTER_INFO *R1 = getRegisterInfo(operand);
        REGISTER_INFO *Rd = getRegisterInfo(exprNode);
        switch (exprNode->semantic_value.exprSemanticValue.op.unaryOp) {
        case UNARY_OP_POSITIVE:
            break;
        case UNARY_OP_NEGATIVE:
            FNEG_RR(Rd, R1);
            break;
        case UNARY_OP_LOGICAL_NEGATION:
            fprintf(stderr, "Don't support float logic.\n");
            exit(1);
            break;
        default:
            break;
        }
        freeRegister(operand);
    }
}

void visitExprNode(AST_NODE *exprNode) {
    if (exprNode->semantic_value.exprSemanticValue.kind == BINARY_OPERATION) {
        visitBinaryExprNode(exprNode);
    } else {
        visitUnaryExprNode(exprNode);
    }
}

void visitVariableRValue(AST_NODE *idNode) {
    AST_NODE offsetNode;
    allocR2Register(&offsetNode, R_32);
    retrieveVariableOffset(idNode, &offsetNode);

    LDR_RR(getRegisterInfo(idNode), getRegisterInfo(&offsetNode));
    freeRegister(&offsetNode);
}

void visitExprRelatedNode(AST_NODE *exprRelatedNode) {
    switch (exprRelatedNode->nodeType) {
    case EXPR_NODE:
        visitExprNode(exprRelatedNode);
        break;
    case STMT_NODE:
        visitFunctionCall(exprRelatedNode);
        break;
    case IDENTIFIER_NODE:
        visitVariableRValue(exprRelatedNode);
        break;
    case CONST_VALUE_NODE:
        visitConstValueNode(exprRelatedNode);
        break;
    default:
        printf("Unhandle case in void processExprRelatedNode(AST_NODE* "
               "exprRelatedNode)\n");
        break;
    }
}

void retrieveVariableOffset(AST_NODE *idNode, AST_NODE *offsetNode) {
    if (DEBUG > 0) {
        fprintf(stderr, "{ start load offset\n");
    }
    idNode->accessOffset = getOffset(
        idNode->semantic_value.identifierSemanticValue.identifierName);

    switch (idNode->semantic_value.identifierSemanticValue.kind) {
    case NORMAL_ID:
        MOV_RC(getRegisterInfo(offsetNode), idNode->accessOffset);
        break;
    case ARRAY_ID:
        MOV_RC(getRegisterInfo(offsetNode), 0);
        AST_NODE *traverseDimList = idNode->child;
        while (traverseDimList) {
            allocR2Register(traverseDimList, R_32);
            visitExprRelatedNode(traverseDimList);
            /* TODO handle mult dim array
            MUL_RRR(getRegisterInfo(offsetNode), getRegisterInfo(offsetNode),
            getRegisterInfo(traverseDimList));
            */
            ADD_RRR(getRegisterInfo(offsetNode), getRegisterInfo(offsetNode),
                    getRegisterInfo(traverseDimList));
            freeRegister(traverseDimList);
            traverseDimList = traverseDimList->rightSibling;
        }
        LSL_RRC(getRegisterInfo(offsetNode), getRegisterInfo(offsetNode), 2);
        ADD_RRC(getRegisterInfo(offsetNode), getRegisterInfo(offsetNode),
                idNode->accessOffset);
        break;
    }

    SXTW_R(getRegisterInfo(offsetNode));
    if (DEBUG > 0) {
        fprintf(stderr, "} end offset\n");
    }
}

// Get variable offset
void visitVariableLValue(AST_NODE *idNode) {
    if (DEBUG > 0) {
        fprintf(stderr, "{ start load offset\n");
    }
    idNode->accessOffset = getOffset(
        idNode->semantic_value.identifierSemanticValue.identifierName);

    switch (idNode->semantic_value.identifierSemanticValue.kind) {
    case NORMAL_ID:
        MOV_RC(getRegisterInfo(idNode), idNode->accessOffset);
        break;
    case ARRAY_ID:
        MOV_RC(getRegisterInfo(idNode), 0);
        AST_NODE *traverseDimList = idNode->child;
        while (traverseDimList) {
            allocR2Register(traverseDimList, R_32);
            visitExprRelatedNode(traverseDimList);
            /* TODO handle mult dim array
            MUL_RRR(getRegisterInfo(idNode), getRegisterInfo(idNode),
            getRegisterInfo(traverseDimList));
            */
            ADD_RRR(getRegisterInfo(idNode), getRegisterInfo(idNode),
                    getRegisterInfo(traverseDimList));
            freeRegister(traverseDimList);
            traverseDimList = traverseDimList->rightSibling;
        }
        LSL_RRC(getRegisterInfo(idNode), getRegisterInfo(idNode), 2);
        ADD_RRC(getRegisterInfo(idNode), getRegisterInfo(idNode),
                idNode->accessOffset);
        break;
    }

    SXTW_R(getRegisterInfo(idNode));
    if (DEBUG > 0) {
        fprintf(stderr, "} end offset\n");
    }
}

void visitAssignmentStmt(AST_NODE *assignmentNode) {
    AST_NODE *leftOp = assignmentNode->child;
    AST_NODE *rightOp = leftOp->rightSibling;

    switch (leftOp->dataType) {
    case INT_TYPE:
        allocR2Register(leftOp, R_32);
        visitVariableLValue(leftOp);
        allocR2Register(rightOp, R_32);
        visitExprRelatedNode(rightOp);

        STR_RR(getRegisterInfo(rightOp), getRegisterInfo(leftOp));

        freeRegister(leftOp);
        freeRegister(rightOp);
        break;
    case FLOAT_TYPE:
        // Get offset of L variable
        allocR2Register(leftOp, R_32);
        visitVariableLValue(leftOp);
        // Calculate expr value
        allocR2Register(rightOp, S_32);
        visitExprRelatedNode(rightOp);

        STR_RR(getRegisterInfo(rightOp), getRegisterInfo(leftOp));

        freeRegister(leftOp);
        freeRegister(rightOp);
        break;
    default:
        fprintf(stderr,
                "visitAssignmentStmt leftOP dataType is not supported\n");
        exit(1);
        break;
    }
}

void visitIfStmt(AST_NODE *ifNode) {
    char B_else[8], B_end[8];
    getBranchLabel(B_else);
    getBranchLabel(B_end);

    AST_NODE *boolExpression = ifNode->child;
    allocR2Register(boolExpression, R_32);
    visitExprRelatedNode(boolExpression);

    CBZ_RL(getRegisterInfo(boolExpression), B_else);

    // B_if
    AST_NODE *ifBodyNode = boolExpression->rightSibling;
    visitStmtNode(ifBodyNode);
    B_L(B_end);

    // B_else
    printf("%s:\n", B_else);
    AST_NODE *elsePartNode = ifBodyNode->rightSibling;
    visitStmtNode(elsePartNode);

    // B_end
    printf("%s:\n", B_end);

    freeRegister(boolExpression);
}

void visitBlockNode(AST_NODE *blockNode) {
    openScope();

    AST_NODE *traverseListNode = blockNode->child;
    while (traverseListNode) {
        visitGeneralNode(traverseListNode);
        traverseListNode = traverseListNode->rightSibling;
    }

    closeScope();
}

void visitWhileStmt(AST_NODE *whileNode) {
    char B_block[8], B_bool[8];
    getBranchLabel(B_block);
    getBranchLabel(B_bool);

    AST_NODE *boolExpression = whileNode->child;
    AST_NODE *bodyNode = boolExpression->rightSibling;

    // jump to bool expr
    B_L(B_bool);

    // block
    printf("%s:\n", B_block);
    visitStmtNode(bodyNode);

    // bool expr
    printf("%s:\n", B_bool);
    allocR2Register(boolExpression, R_32);
    visitExprRelatedNode(boolExpression);
    CBNZ_RL(getRegisterInfo(boolExpression), B_block);
    freeRegister(boolExpression);
}

void visitReturnStmt(AST_NODE *returnNode) {

    if (returnNode->dataType == INT_TYPE) {
        AST_NODE *exprNode = returnNode->child;

        allocR0Register(returnNode, 0, R_32);
        allocR2Register(exprNode, R_32);
        visitExprRelatedNode(exprNode);

        REGISTER_INFO *R1 = getRegisterInfo(exprNode);
        REGISTER_INFO *Rd = getRegisterInfo(returnNode);

        MOV_RR(Rd, R1);

        freeRegister(exprNode);
    } else if (returnNode->dataType == FLOAT_TYPE) {
        AST_NODE *exprNode = returnNode->child;

        allocR0Register(returnNode, 0, S_32);
        allocR2Register(exprNode, S_32);
        visitExprRelatedNode(exprNode);

        REGISTER_INFO *R1 = getRegisterInfo(exprNode);
        REGISTER_INFO *Rd = getRegisterInfo(returnNode);

        FMOV_RR(Rd, R1);

        freeRegister(exprNode);
    }
}

void visitStmtNode(AST_NODE *stmtNode) {
    if (stmtNode->nodeType == NUL_NODE) {
        return;
    } else if (stmtNode->nodeType == BLOCK_NODE) {
        visitBlockNode(stmtNode);
    } else {
        switch (stmtNode->semantic_value.stmtSemanticValue.kind) {
        case WHILE_STMT:
            visitWhileStmt(stmtNode);
            break;
        /*
        case FOR_STMT:
            checkForStmt(stmtNode);
            break;
            */
        case ASSIGN_STMT:
            visitAssignmentStmt(stmtNode);
            break;
        case IF_STMT:
            visitIfStmt(stmtNode);
            break;
        case FUNCTION_CALL_STMT:
            visitFunctionCall(stmtNode);
            break;
        case RETURN_STMT:
            visitReturnStmt(stmtNode);
            break;
        default:
            printf(
                "Unhandle case in void processStmtNode(AST_NODE* stmtNode)\n");
            stmtNode->dataType = ERROR_TYPE;
            break;
        }
    }
}

void visitFunctionCall(AST_NODE *functionCallNode) {
    AST_NODE *functionIDNode = functionCallNode->child;
    char *functionName =
        functionIDNode->semantic_value.identifierSemanticValue.identifierName;

    // special case
    if (strcmp(functionName, "write") == 0) {
        visitWriteFunction(functionCallNode);
        return;
    }

    AST_NODE *actualParameterList = functionIDNode->rightSibling;

    // Allocate parameter to register
    AST_NODE *actualParameter = actualParameterList->child;
    int i = 0;
    while (actualParameter) {
        allocR0Register(actualParameter, i, R_32);
        visitExprRelatedNode(actualParameter);
        actualParameter->rightSibling;
    }

    char buffer[256];
    sprintf(buffer, "_start_%s", functionName);

    saveRegisterToSP();
    BL_L(buffer);
    loadRegisterToSP();

    actualParameter = actualParameterList->child;
    while (actualParameter) {
        freeRegister(actualParameter);
        actualParameter->rightSibling;
    }

    if (hasAllocRegister(functionCallNode)) {
        char R1[3];
        getRegister(getRegisterInfo(functionCallNode), R1);
        switch (functionCallNode->dataType) {
        case INT_TYPE:
            printf("\tmov %s, w0\n", R1);
            break;
        case FLOAT_TYPE:
            printf("\tfmov %s, s0\n", R1);
            break;
        case VOID_TYPE:
            break;
        }
    }
}

void visitWriteFunction(AST_NODE *functionCallNode) {
    AST_NODE *functionIDNode = functionCallNode->child;
    AST_NODE *actualParameterList = functionIDNode->rightSibling;
    // visitGeneralNode(actualParameterList);
    AST_NODE *actualParameter = actualParameterList->child;

    allocR2Register(actualParameter, R_32);
    visitVariableLValue(actualParameter);

    switch (actualParameter->dataType) {
    case INT_TYPE:
        printf("\tldr x0, [sp, x%d]\n", actualParameter->registerNumber);
        printf("\tbl _write_int\n");
        break;
    case FLOAT_TYPE:
        printf("\tldr s0, [sp, x%d]\n",
               getRegisterInfo(actualParameter)->registerNumber);
        printf("\tbl _write_float\n");
        break;
    case CONST_STRING_TYPE:
        printf("\tmov x0, %s\n",
               actualParameter->semantic_value.const1->const_u);
        printf("\tbl _write_str");
        break;
    default:
        break;
    }
    freeRegister(actualParameter);
}
