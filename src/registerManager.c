#include "registerManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getR2Size() {
    int numRegister = REGISTER2_END - REGISTER2_BEGIN;
    int x_size = numRegister * 8;
    int d_size = numRegister * 4;
    return x_size + d_size;
}

int R0_COUNT;
int R2_COUNT;
int ALLOCATED_ARRAY[REGISTER_SIZE];

void allocate(AST_NODE *node, REGISTER_TYPE type, int registerNumber) {
    ALLOCATED_ARRAY[registerNumber] = 1;
    node->register_info.registerNumber = registerNumber;
    node->registerNumber = registerNumber;
    switch (type) {
    case R_32:
        node->register_info.symbol = 'w';
        break;
    case R_64:
        node->register_info.symbol = 'x';
        break;
    case S_32:
        node->register_info.symbol = 's';
        break;
    case D_64:
        node->register_info.symbol = 'd';
        break;
    }

    if (registerNumber >= REGISTER0_BEGIN && registerNumber < REGISTER0_END)
        R0_COUNT++;

    if (registerNumber >= REGISTER2_BEGIN && registerNumber < REGISTER2_END)
        R2_COUNT++;
}

void initRegister() {
    fprintf(stderr, "Free all register\n");
    R2_COUNT = 0;
    for (int i = 0; i < REGISTER_SIZE; i++)
        ALLOCATED_ARRAY[i] = 0;
}

void allocR0Register(AST_NODE *node, int i, REGISTER_TYPE type) {
    allocate(node, type, i);
    // fprintf(stderr, "Alloc R0 {%d} usage: %d \n", i , 8);
}

void allocR1Register(AST_NODE *node, REGISTER_TYPE type) {
    for (int i = REGISTER1_BEGIN; i < REGISTER1_END; i++)
        if (ALLOCATED_ARRAY[i] == 0) {
            allocate(node, type, i);
            break;
        }
}

void allocR2Register(AST_NODE *node, REGISTER_TYPE type) {
    int i;
    for (i = REGISTER2_BEGIN; i < REGISTER2_END; i++)
        if (ALLOCATED_ARRAY[i] == 0) {
            allocate(node, type, i);
            break;
        }

    if (R2_COUNT >= 21) {
        fprintf(stderr, "R2 usage: %d\n", R2_COUNT);
        fprintf(stderr, "R2 is fulled.\n");
        exit(1);
    } else {
        fprintf(stderr, "Alloc R2 {%d} usage: %d \n", i, R2_COUNT);
    }
}

int allocR2() {
    for (int i = REGISTER2_BEGIN; i < REGISTER2_END; i++)
        if (ALLOCATED_ARRAY[i] == 0) {
            ALLOCATED_ARRAY[i] = 1;
            return i;
        }
}

int hasAllocRegister(AST_NODE *node) {
    if (node->register_info.registerNumber == -1)
        return 0;
    return 1;
}

void freeRegister(AST_NODE *node) {
    R2_COUNT--;
    fprintf(stderr, "Free R2 {%d} usage: %d \n", node->registerNumber,
            R2_COUNT);
    ALLOCATED_ARRAY[node->registerNumber] = 0;
    node->register_info.registerNumber = -1;
    node->register_info.symbol = '$';
}

int freeR2(int i) { ALLOCATED_ARRAY[i] = 0; }

void saveRegisterToSP() {
    int offset = 16;
    for (int i = REGISTER2_BEGIN; i < REGISTER2_END; i++) {
        printf("\tstr x%d, [sp, %d]\n", i, offset);
        offset += 8;
    }
    for (int i = REGISTER2_BEGIN; i < REGISTER2_END; i++) {
        printf("\tstr s%d, [sp, %d]\n", i, offset);
        offset += 4;
    }
}

void loadRegisterToSP() {
    int offset = 16;
    for (int i = REGISTER2_BEGIN; i < REGISTER2_END; i++) {
        printf("\tldr x%d, [sp, %d]\n", i, offset);
        offset += 8;
    }
    for (int i = REGISTER2_BEGIN; i < REGISTER2_END; i++) {
        printf("\tldr s%d, [sp, %d]\n", i, offset);
        offset += 4;
    }
}
