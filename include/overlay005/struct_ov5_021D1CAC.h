#ifndef POKEPLATINUM_STRUCT_OV5_021D1CAC_H
#define POKEPLATINUM_STRUCT_OV5_021D1CAC_H

typedef struct {
    u16 unk_00_0 : 1;    // TalkCheck
    u16 unk_00_1 : 1;    // StepCheck
    u16 unk_00_2 : 1;    // MenuOpen
    u16 unk_00_3 : 1;    // CnvButton
    u16 unk_00_4 : 1;    // MatCheck
    u16 unk_00_5 : 1;    // PushCheck
    u16 unk_00_6 : 1;    // MoveCheck
    u16 unk_00_7 : 1;    // FloatCheck
    u16 unk_00_8 : 1;    // DebugMenu
    u16 unk_00_9 : 1;    // DebugBattle
    u16 unk_00_10 : 1;   // DebugHook
    u16 unk_00_11 : 1;   // DebugKeyPush
    u16 : 4;
    u8 unk_02;           // Site
    s8 unk_03;           // PushSite
    u16 unk_04;          // trg (target)
    u16 unk_06;          // cont
} UnkStruct_ov5_021D1CAC; // EV_REQUEST (event request)

#endif // POKEPLATINUM_STRUCT_OV5_021D1CAC_H
