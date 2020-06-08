#ifndef __FRAME_REGISTER_MANAGER__
#define __FRAME_REGISTER_MANAGER__

// ARGUMENTS and RETURN REGISTER
#define REGISTER0_BEGIN 0
#define REGISTER0_END 8
// TEMPORARY REGISTER
#define REGISTER1_BEGIN 8
#define REGISTER1_END 19
// CALLEE SAVED REGISTER
#define REGISTER2_BEGIN 19
#define REGISTER2_END 29
// SPECIAL REGISTER
#define FRAME_POINTER 29
#define LINK_REGISTER 30
#define SP 31
#define REGISTER_SIZE 32

int allocated[REGISTER_SIZE];

void registerManagerInit() {
    for (int i = 0; i < REGISTER_SIZE; i++)
        allocated = 0;
}

int allocRegion0Register() {
    for (int i = REGISTER0_BEGIN; i < REGISTER0_END; i++)
        if (!allocated[i]) {
            allocated[i] = 1;
            return i;
        }
}

int allocRegion1Register() {
    for (int i = REGISTER1_BEGIN; i < REGISTER1_END; i++)
        if (!allocated[i]) {
            allocated[i] = 1;
            return i;
        }
}

int allocRegion2Register() {
    for (int i = REGISTER2_BEGIN; i < REGISTER2_END; i++)
        if (!allocated[i]) {
            allocated[i] = 1;
            return i;
        }
}

void free(int i) { allocated[i] = 0; }

#endif
