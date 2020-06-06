#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void visitProgramNode(AST_NODE *programNode);
void visitDeclarationNode(AST_NODE *declarationNode);
void visitDeclareFunction(AST_NODE *declarationNode);

void armGenerator(AST_NODE *root) {
    visitProgramNode(root);
    return;
}

void visitProgramNode(AST_NODE *programNode) {
    AST_NODE *traverseDeclaration = programNode->child;
    while (traverseDeclaration) {
        if (traverseDeclaration->nodeType == VARIABLE_DECL_LIST_NODE) {
            // TODO visitGeneralNode(traverseDeclaration);
        } else {
            // function declaration
            visitDeclarationNode(traverseDeclaration);
        }

        if (traverseDeclaration->dataType == ERROR_TYPE) {
            programNode->dataType = ERROR_TYPE;
        }

        traverseDeclaration = traverseDeclaration->rightSibling;
    }
    return;
}

void visitDeclarationNode(AST_NODE *declarationNode) {
    AST_NODE *typeNode = declarationNode->child;
    if (typeNode->dataType == ERROR_TYPE) {
        declarationNode->dataType = ERROR_TYPE;
        return;
    }

    switch (declarationNode->semantic_value.declSemanticValue.kind) {
    /*
    case VARIABLE_DECL:
        declareIdList(declarationNode, VARIABLE_ATTRIBUTE, 0);
        break;
    case TYPE_DECL:
        declareIdList(declarationNode, TYPE_ATTRIBUTE, 0);
        break;
    */
    case FUNCTION_DECL:
        declareFunction(declarationNode);
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

    int errorOccur = 0;
    if (returnTypeNode->semantic_value.identifierSemanticValue.symbolTableEntry
            ->attribute->attr.typeDescriptor->kind == ARRAY_TYPE_DESCRIPTOR) {
        printErrorMsg(returnTypeNode, RETURN_ARRAY);
        returnTypeNode->dataType = ERROR_TYPE;
        errorOccur = 1;
    }

    AST_NODE *functionNameID = returnTypeNode->rightSibling;
    if (declaredLocally(functionNameID->semantic_value.identifierSemanticValue
                            .identifierName)) {
        printErrorMsg(functionNameID, SYMBOL_REDECLARE);
        functionNameID->dataType = ERROR_TYPE;
        errorOccur = 1;
    }

    SymbolAttribute *attribute = NULL;
    attribute = (SymbolAttribute *)malloc(sizeof(SymbolAttribute));
    attribute->attributeKind = FUNCTION_SIGNATURE;
    attribute->attr.functionSignature =
        (FunctionSignature *)malloc(sizeof(FunctionSignature));
    attribute->attr.functionSignature->returnType = returnTypeNode->dataType;
    attribute->attr.functionSignature->parameterList = NULL;

    int enterFunctionNameToSymbolTable = 0;
    if (!errorOccur) {
        enterSymbol(functionNameID->semantic_value.identifierSemanticValue
                        .identifierName,
                    attribute);
        enterFunctionNameToSymbolTable = 1;
    }

    openScope();

    AST_NODE *parameterListNode = functionNameID->rightSibling;
    AST_NODE *traverseParameter = parameterListNode->child;
    int parametersCount = 0;
    if (traverseParameter) {
        ++parametersCount;
        processDeclarationNode(traverseParameter);
        AST_NODE *parameterID = traverseParameter->child->rightSibling;
        if (traverseParameter->dataType == ERROR_TYPE) {
            errorOccur = 1;
        } else if (!errorOccur) {
            Parameter *parameter = (Parameter *)malloc(sizeof(Parameter));
            parameter->next = NULL;
            parameter->parameterName =
                parameterID->semantic_value.identifierSemanticValue
                    .identifierName;
            parameter->type =
                parameterID->semantic_value.identifierSemanticValue
                    .symbolTableEntry->attribute->attr.typeDescriptor;
            attribute->attr.functionSignature->parameterList = parameter;
        }
        traverseParameter = traverseParameter->rightSibling;
    }

    Parameter *parameterListTail =
        attribute->attr.functionSignature->parameterList;

    while (traverseParameter) {
        ++parametersCount;
        processDeclarationNode(traverseParameter);
        AST_NODE *parameterID = traverseParameter->child->rightSibling;
        if (traverseParameter->dataType == ERROR_TYPE) {
            errorOccur = 1;
        } else if (!errorOccur) {
            Parameter *parameter = (Parameter *)malloc(sizeof(Parameter));
            parameter->next = NULL;
            parameter->parameterName =
                parameterID->semantic_value.identifierSemanticValue
                    .identifierName;
            parameter->type =
                parameterID->semantic_value.identifierSemanticValue
                    .symbolTableEntry->attribute->attr.typeDescriptor;
            parameterListTail->next = parameter;
            parameterListTail = parameter;
        }
        traverseParameter = traverseParameter->rightSibling;
    }
    attribute->attr.functionSignature->parametersCount = parametersCount;

    if (errorOccur && (attribute != NULL)) {
        Parameter *traverseParameter =
            attribute->attr.functionSignature->parameterList;
        Parameter *next = NULL;
        while (traverseParameter) {
            next = traverseParameter->next;
            free(traverseParameter);
            traverseParameter = next;
        }
        free(attribute->attr.functionSignature);
        free(attribute);
    }

    if (!errorOccur) {
        AST_NODE *blockNode = parameterListNode->rightSibling;
        AST_NODE *traverseListNode = blockNode->child;
        while (traverseListNode) {
            processGeneralNode(traverseListNode);
            traverseListNode = traverseListNode->rightSibling;
        }
    }

    closeScope();

    if (errorOccur && enterFunctionNameToSymbolTable) {
        declarationNode->dataType = ERROR_TYPE;
        if (enterFunctionNameToSymbolTable) {
            removeSymbol(functionNameID->semantic_value.identifierSemanticValue
                             .identifierName);
        }
    }
}
