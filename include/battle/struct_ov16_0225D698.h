#ifndef POKEPLATINUM_STRUCT_OV16_0225D698_H
#define POKEPLATINUM_STRUCT_OV16_0225D698_H

#include "struct_decls/battle_system.h"

typedef struct {
    BattleSystem * unk_00;  // BattleSystem
    void * unk_04;          // gauge
    int unk_08;             // tp_ret (AI move selected action?)
    u16 unk_0C[4];          // move
    u8 unk_14[4];           // move pp
    u8 unk_18[4];           // move pp max
    u8 unk_1C;              // command_code
    u8 unk_1D;              // battler
    u8 unk_1E;              // battler type
    u8 unk_1F;              // selected mons number (party slot?)
    u8 unk_20;              // sequence number
    u8 unk_21;              // dummy
    u16 unk_22;             // move bit
} UnkStruct_ov16_0225D698; // TCB_MOVE_SELECT

#endif // POKEPLATINUM_STRUCT_OV16_0225D698_H
