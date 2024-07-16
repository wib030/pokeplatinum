#ifndef POKEPLATINUM_STRUCT_020322F8_H
#define POKEPLATINUM_STRUCT_020322F8_H

#include "comm_ring.h"
#include "struct_defs/struct_020322D8.h"
#include "struct_defs/struct_02032318.h"

typedef struct {
    UnkStruct_02032318 unk_00;
    UnkStruct_02032318 unk_08;
    UnkStruct_020322D8 * unk_10;
    CommRing * unk_14;
    void * unk_18;
    int unk_1C;
} CommQueueMan;

#endif // POKEPLATINUM_STRUCT_020322F8_H
