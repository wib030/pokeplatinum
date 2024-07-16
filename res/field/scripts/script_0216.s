    .include "macros/scrcmd.inc"

    .data

    .long _0024-.-4
    .long _008A-.-4
    .long _00CF-.-4
    .long _0150-.-4
    .long _01A8-.-4
    .long _01BF-.-4
    .long _0022-.-4
    .long _0201-.-4
    .short 0xFD13

_0022:
    End

_0024:
    CompareVarToValue 0x40CF, 2
    CallIf 1, _0082
    CheckFlag 0x10F
    CallIf 1, _00BB
    CheckFlag 0x10F
    CallIf 0, _00C5
    CompareVarToValue 0x4089, 2
    GoToIf 0, _007C
    CheckFlag 0xAA8
    GoToIf 1, _007C
    ScrCmd_234 0x4000
    CompareVarToValue 0x4000, 5
    GoToIf 5, _007C
    GoTo _0076

_0076:
    ClearFlag 0x20B
    End

_007C:
    SetFlag 0x20B
    End

_0082:
    SetVar 0x40CF, 3
    Return

_008A:
    CheckFlag 0x10F
    CallIf 1, _00BB
    CheckFlag 0x10F
    CallIf 0, _00C5
    CheckFlag 142
    GoToIf 1, _00AD
    End

_00AD:
    SetFlag 0x20B
    ScrCmd_065 4
    ClearFlag 142
    End

_00BB:
    ScrCmd_18B 1, 243, 0x28A
    Return

_00C5:
    ScrCmd_18A 0, 243, 0x28A
    Return

_00CF:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 0
    ScrCmd_034
    ScrCmd_0E5 0x34B, 0
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0133
    ScrCmd_02C 1
    ScrCmd_034
    ApplyMovement 0, _013C
    WaitMovement
    ScrCmd_168 7, 20, 19, 14, 77
    ScrCmd_16B 77
    ScrCmd_169 77
    ApplyMovement 0, _0144
    WaitMovement
    ScrCmd_16C 77
    ScrCmd_169 77
    ScrCmd_16A 77
    ScrCmd_02C 4
    ScrCmd_065 0
    ScrCmd_034
    ScrCmd_061
    End

_0133:
    ScrCmd_0EB
    ScrCmd_061
    End

    .balign 4, 0
_013C:
    MoveAction_020
    EndMovement

    .balign 4, 0
_0144:
    MoveAction_00C
    MoveAction_045
    EndMovement

_0150:
    ScrCmd_049 0x5DC
    ScrCmd_060
    CheckFlag 159
    GoToIf 1, _016C
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_016C:
    ScrCmd_0D1 0, 0x1B6
    ScrCmd_02C 6
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _01A2
    SetFlag 0x10F
    Call _00BB
    ScrCmd_18A 0, 243, 0x28E
    ScrCmd_02C 7
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_01A2:
    ScrCmd_034
    ScrCmd_061
    End

_01A8:
    ScrCmd_036 9, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_01BF:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04C 0x1A9, 0
    ScrCmd_02C 8
    ScrCmd_034
    ScrCmd_04D
    SetFlag 142
    ScrCmd_2BD 0x1A9, 15
    ClearFlag 142
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _01FB
    SetFlag 0xAA8
    ScrCmd_061
    End

_01FB:
    ScrCmd_0EB
    ScrCmd_061
    End

_0201:
    ScrCmd_060
    ApplyMovement 6, _02D8
    ApplyMovement 0xFF, _02AC
    WaitMovement
    ScrCmd_02C 2
    ScrCmd_034
    ApplyMovement 0xFF, _02BC
    ApplyMovement 6, _02EC
    WaitMovement
    ScrCmd_168 7, 20, 19, 14, 77
    ScrCmd_16B 77
    ScrCmd_169 77
    ApplyMovement 6, _0300
    WaitMovement
    ScrCmd_16C 77
    ScrCmd_169 77
    ScrCmd_16A 77
    ScrCmd_003 120, 0x800C
    ScrCmd_168 7, 20, 19, 14, 77
    ScrCmd_16B 77
    ScrCmd_169 77
    ApplyMovement 6, _0310
    WaitMovement
    ScrCmd_16C 77
    ScrCmd_169 77
    ScrCmd_16A 77
    ApplyMovement 6, _031C
    WaitMovement
    ScrCmd_02C 3
    ScrCmd_034
    ApplyMovement 0xFF, _02C8
    ApplyMovement 6, _0324
    WaitMovement
    ScrCmd_065 6
    SetVar 0x411E, 2
    ScrCmd_061
    End

    .balign 4, 0
_02AC:
    MoveAction_03F 5
    MoveAction_03E
    MoveAction_022
    EndMovement

    .balign 4, 0
_02BC:
    MoveAction_00D
    MoveAction_020
    EndMovement

    .balign 4, 0
_02C8:
    MoveAction_03F
    MoveAction_022
    MoveAction_021
    EndMovement

    .balign 4, 0
_02D8:
    MoveAction_04B
    MoveAction_03F
    MoveAction_010 4
    MoveAction_027
    EndMovement

    .balign 4, 0
_02EC:
    MoveAction_03F
    MoveAction_03E
    MoveAction_00F
    MoveAction_020
    EndMovement

    .balign 4, 0
_0300:
    MoveAction_00C
    MoveAction_045
    MoveAction_001
    EndMovement

    .balign 4, 0
_0310:
    MoveAction_046
    MoveAction_011
    EndMovement

    .balign 4, 0
_031C:
    MoveAction_025
    EndMovement

    .balign 4, 0
_0324:
    MoveAction_012
    MoveAction_011
    MoveAction_011 6
    MoveAction_012 9
    EndMovement
