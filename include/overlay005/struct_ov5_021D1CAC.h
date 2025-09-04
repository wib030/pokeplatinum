#ifndef POKEPLATINUM_STRUCT_OV5_021D1CAC_H
#define POKEPLATINUM_STRUCT_OV5_021D1CAC_H

typedef struct {
    u16 unk_00_0 : 1;    // TalkCheck     interact
    u16 unk_00_1 : 1;    // StepCheck     endMovement
    u16 unk_00_2 : 1;    // MenuOpen      menu
    u16 unk_00_3 : 1;    // CnvButton     registeredItem
    u16 registeredItem2 : 1;    // CnvButton     registeredItem2
    u16 unk_00_4 : 1;    // MatCheck      sign
    u16 unk_00_5 : 1;    // PushCheck     mapTrasition
    u16 unk_00_6 : 1;    // MoveCheck     movement
    u16 unk_00_7 : 1;    // FloatCheck    dummy1
    u16 unk_00_8 : 1;    // DebugMenu     dummy2
    u16 unk_00_9 : 1;    // DebugBattle   dummy3
    u16 unk_00_10 : 1;   // DebugHook     dummy4
    u16 unk_00_11 : 1;   // DebugKeyPush  dummy5
    u16 : 4;
    u8 unk_02;           // Site          playerDir
    s8 unk_03;           // PushSite      transitionDir
    u16 unk_04;          // trg (target)  pressedKeys
    u16 unk_06;          // cont          heldKeys
} UnkStruct_ov5_021D1CAC; // EV_REQUEST (event request)      FieldInput

#endif // POKEPLATINUM_STRUCT_OV5_021D1CAC_H
