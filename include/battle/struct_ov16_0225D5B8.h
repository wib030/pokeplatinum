#ifndef POKEPLATINUM_STRUCT_OV16_0225D5B8_H
#define POKEPLATINUM_STRUCT_OV16_0225D5B8_H

#include "struct_decls/battle_system.h"

typedef struct {
    BattleSystem * unk_00;  // BattleSystem * battleSys
    void * unk_04;          // PartyGauge
    u8 unk_08;              // command_code
    u8 unk_09;              // battler
    u8 unk_0A;              // sequence number (not sure what that is)
    s8 unk_0B;              // wait
    int unk_0C;             // tp_ret (AI move selected action?)
    u8 unk_10[2][6];        // mon status array for both sides (for party)
    u8 unk_1C[6];           // mon motivation (for party)
    u8 unk_22;              // message index
    u8 unk_23;              // selected mons number
    u16 unk_24[4];          // move number (array of moves?)
    u8 unk_2C[4];           // move pp (array of move pp)
    u8 unk_30[4];           // move pp max
    u8 unk_34;              // battler type
    u8 unk_35;              // index
    s16 unk_36;             // hp icon (? not sure what this is - probably health bar)
    u16 unk_38;             // hp max icon
    u8 unk_3A;              // icon status
    u8 unk_3B;              // no switch battler (possibly party slot number for switch?)
} UnkStruct_ov16_0225D5B8;  // TCB_COMMAND_SELECT

#endif // POKEPLATINUM_STRUCT_OV16_0225D5B8_H
