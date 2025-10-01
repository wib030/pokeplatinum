#ifndef POKEPLATINUM_STRUCT_OV16_0225BFFC_T_H
#define POKEPLATINUM_STRUCT_OV16_0225BFFC_T_H

#include "struct_decls/struct_02006C24_decl.h"
#include "struct_decls/sprite_decl.h"
#include "struct_decls/cell_actor_data.h"
#include "struct_decls/sys_task.h"
#include "struct_defs/struct_0205AA50.h"
#include "overlay012/struct_ov12_02223764.h"
#include "overlay012/struct_ball_rotation_decl.h"
#include "battle/struct_ov16_0225BFFC_sub1.h"
#include "battle/struct_ov16_022674C4.h"
#include "battle/struct_ov16_0226C378.h"

#define DATA_BUF_SIZE 256

struct BattlerData {
    UnkStruct_ov16_0225BFFC_sub1 unk_00;  // user interface
    CellActorData * unk_18;               // cell actor pointer
    Sprite * unk_1C;                      // trainer soft sprite
    Sprite * unk_20;                      // pokemon soft sprite
    Window * unk_24;                      // gamefreak background layer bitmap - window
    Healthbar healthbar;                  // gauge
    UnkStruct_ov16_0226C378 unk_7B;       // cursor_save
    BallRotation * unk_84;                // BMS pointer (ball mechanic system?)
    UnkStruct_ov12_02223764 * unk_88;     // OAM Drop System
    void * unk_8C;                        // time icon
    u8 data[DATA_BUF_SIZE];               // battler buffer
    u8 battler;                           // battler
    u8 battlerType;                       // battler type
    u8 bootState;                         // battler boot state
    u8 unk_193;                           // bip flag
    SysTask * unk_194;                    // server input trainer command button popinter
    u16 unk_198;                          // server input sec
    int unk_19C;                          // substitute flag
    NARC * unk_1A0;                       // narc handle
    u8 unk_1A4;                           // battler buffer flag
    u8 unk_1A5[3];                        // dummy
}; // client_param

#endif // POKEPLATINUM_STRUCT_OV16_0225BFFC_T_H
