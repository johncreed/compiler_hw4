#include "armGenerator.h"
#include "registerManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int DEBUG = 1;

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

void MOV_RC(REGISTER_INFO *reg, int val) {
    char R1[3];
    getRegister(reg, R1);
    printf("\tmov %s, %d\n", R1, val);
}

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

void LSL(AST_NODE *node, int size) {
    printf("\tlsl w%d, w%d, %d\n", node->registerNumber, node->registerNumber,
           size);
}

// Control flow Related instruction

void CMP_RC(REGISTER_INFO *reg, int val){
    char R[3];
    getRegister(reg, R);
    printf("\tcmp %s, %d\n", R, val);
}

void CMP_RR(REGISTER_INFO *reg1, REGISTER_INFO *reg2){
    char R1[3], R2[3];
    getRegister(reg1, R1);
    getRegister(reg2, R2);
    printf("\tcmp %s, %s\n", R1, R2);
}

void CSET_R(REGISTER_INFO *reg1, char *flag){
    char R1[3];
    getRegister(reg1, R1);
    printf("\tcset %s, %s\n", R1, flag);
}

void B_L(char *label){
    printf("\tb %s\n", label);
}

void CBZ_RL(REGISTER_INFO *reg1, char *label){
    char R1[3];
    getRegister(reg1, R1);
    printf("\tcbz %s, %s\n", R1, label);
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


// Old A64 instruction

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

void getBranchLabel(char* buffer){
    sprintf(buffer, ".L%d", BRANCH_LABEL_COUNTER);
    BRANCH_LABEL_COUNTER ++;
}

void getCONSTLabel(char* buffer){
    sprintf(buffer, ".LC%d", CONST_LABEL_COUNTER);
    CONST_LABEL_COUNTER ++;
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
            processExprRelatedNode(traverseChildren);
            if (traverseChildren->dataType == ERROR_TYPE) {
                node->dataType = ERROR_TYPE;
            }
            traverseChildren = traverseChildren->rightSibling;
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
    switch (constValueNode->semantic_value.const1->const_type) {
    case INTEGERC:
        MOV_RC(getRegisterInfo(constValueNode),
               constValueNode->semantic_value.exprSemanticValue.constEvalValue
                   .iValue);
        break;
    /*case FLOATC:
        constValueNode->dataType = FLOAT_TYPE;
        constValueNode->semantic_value.exprSemanticValue.constEvalValue.fValue =
            constValueNode->semantic_value.const1->const_u.fval;
        break;
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

void visitExprNode(AST_NODE *exprNode) {
    if (exprNode->semantic_value.exprSemanticValue.kind == BINARY_OPERATION) {
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
            break;
        case BINARY_OP_OR:
            break;
        }

        freeRegister(leftOp);
        freeRegister(rightOp);
    } else {
        // case UNARY_OPERATION:
        AST_NODE *operand = exprNode->child;
        visitExprRelatedNode(operand);
    }
}

void visitVariableRValue(AST_NODE *idNode){
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
    /*
    case STMT_NODE:
        // function call
        checkFunctionCall(exprRelatedNode);
        break;
        */
    case IDENTIFIER_NODE:
        //processVariableRValue(exprRelatedNode);
        visitVariableRValue(exprRelatedNode);
        break;
    case CONST_VALUE_NODE:
        visitConstValueNode(exprRelatedNode);
        break;
        /*
    default:
        printf("Unhandle case in void processExprRelatedNode(AST_NODE* "
               "exprRelatedNode)\n");
        exprRelatedNode->dataType = ERROR_TYPE;
        break;
        */
    }
}

void retrieveVariableOffset(AST_NODE *idNode, AST_NODE *offsetNode){
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
        // TODO printf("\tstr w%d [sp, %d]\n", registerNumber, offset);
        break;
    default:
        fprintf(stderr,
                "visitAssignmentStmt leftOP dataType is not supported\n");
        exit(1);
        break;
    }
}

void visitAssignOrExpr(AST_NODE *assignOrExprRelatedNode) {
    if (assignOrExprRelatedNode->nodeType == STMT_NODE) {
        if (assignOrExprRelatedNode->semantic_value.stmtSemanticValue.kind ==
            ASSIGN_STMT) {
            visitAssignmentStmt(assignOrExprRelatedNode);
        } else if (assignOrExprRelatedNode->semantic_value.stmtSemanticValue
                       .kind == FUNCTION_CALL_STMT) {
            visitFunctionCall(assignOrExprRelatedNode);
        }
    } else {
        visitExprRelatedNode(assignOrExprRelatedNode);
    }
}

void visitIfStmt(AST_NODE *ifNode) {
    char B_else[8], B_end[8];
    getBranchLabel(B_else);
    getBranchLabel(B_end);
   
    AST_NODE *boolExpression = ifNode->child;
    allocR2Register(boolExpression, R_32);
    visitAssignOrExpr(boolExpression);

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

void visitStmtNode(AST_NODE *stmtNode) {
    if (stmtNode->nodeType == NUL_NODE) {
        return;
    } else if (stmtNode->nodeType == BLOCK_NODE) {
        visitBlockNode(stmtNode);
    } else {
        switch (stmtNode->semantic_value.stmtSemanticValue.kind) {
        /*case WHILE_STMT:
            checkWhileStmt(stmtNode);
            break;
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
        /*case RETURN_STMT:
            checkReturnStmt(stmtNode);
            break;
            */
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

    // special case
    if (strcmp(functionIDNode->semantic_value.identifierSemanticValue
                   .identifierName,
               "write") == 0) {
        visitWriteFunction(functionCallNode);
        return;
    }
    /*
        SymbolTableEntry *symbolTableEntry = retrieveSymbol(
            functionIDNode->semantic_value.identifierSemanticValue.identifierName);
        functionIDNode->semantic_value.identifierSemanticValue.symbolTableEntry
       =
            symbolTableEntry;

        if (symbolTableEntry == NULL) {
            printErrorMsg(functionIDNode, SYMBOL_UNDECLARED);
            functionIDNode->dataType = ERROR_TYPE;
            functionCallNode->dataType = ERROR_TYPE;
            return;
        } else if (symbolTableEntry->attribute->attributeKind !=
                   FUNCTION_SIGNATURE) {
            printErrorMsg(functionIDNode, NOT_FUNCTION_NAME);
            functionIDNode->dataType = ERROR_TYPE;
            functionCallNode->dataType = ERROR_TYPE;
            return;
        }

        AST_NODE *actualParameterList = functionIDNode->rightSibling;
        processGeneralNode(actualParameterList);

        AST_NODE *actualParameter = actualParameterList->child;
        Parameter *formalParameter =
            symbolTableEntry->attribute->attr.functionSignature->parameterList;

        int parameterPassingError = 0;
        while (actualParameter && formalParameter) {
            if (actualParameter->dataType == ERROR_TYPE) {
                parameterPassingError = 1;
            } else {
                checkParameterPassing(formalParameter, actualParameter);
                if (actualParameter->dataType == ERROR_TYPE) {
                    parameterPassingError = 1;
                }
            }
            actualParameter = actualParameter->rightSibling;
            formalParameter = formalParameter->next;
        }

        if (parameterPassingError) {
            functionCallNode->dataType = ERROR_TYPE;
        }
        if (actualParameter != NULL) {
            printErrorMsg(functionIDNode, TOO_MANY_ARGUMENTS);
            functionCallNode->dataType = ERROR_TYPE;
        } else if (formalParameter != NULL) {
            printErrorMsg(functionIDNode, TOO_FEW_ARGUMENTS);
            functionCallNode->dataType = ERROR_TYPE;
        } else {
            functionCallNode->dataType =
                symbolTableEntry->attribute->attr.functionSignature->returnType;
        }
        */
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
        printf("\tmov x0, %lf\n",
               actualParameter->semantic_value.const1->const_u);
        printf("\tbl _write_float");
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
