    .include "macros/scrcmd.inc"

    .data

    .long _0079-.-4
    .long _030C-.-4
    .long _031F-.-4
    .long _0379-.-4
    .long _0390-.-4
    .long _03A7-.-4
    .long _001E-.-4
    .short 0xFD13

_001E:
    ScrCmd_1B6 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _0065
    CompareVarToValue 0x4000, 1
    GoToIf 1, _006F
    CompareVarToValue 0x4000, 2
    GoToIf 1, _006F
    CompareVarToValue 0x4000, 3
    GoToIf 1, _006F
    CompareVarToValue 0x4000, 4
    GoToIf 1, _006F
    End

_0065:
    ClearFlag 0x271
    SetFlag 0x270
    End

_006F:
    ClearFlag 0x270
    SetFlag 0x271
    End

_0079:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_07E 0x1D0, 1, 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _00A1
    ScrCmd_02C 0
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00A1:
    ScrCmd_02C 1
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _00CA
    CompareVarToValue 0x800C, 1
    GoToIf 1, _00C4
    End

_00C4:
    ScrCmd_034
    ScrCmd_061
    End

_00CA:
    ScrCmd_0CD 0
    ScrCmd_0D1 1, 0x1D0
    ScrCmd_02C 2
    ScrCmd_034
    ScrCmd_04C 54, 0
    ScrCmd_04D
    ApplyMovement 27, _024C
    ApplyMovement 28, _024C
    ApplyMovement 26, _024C
    ApplyMovement 19, _024C
    WaitMovement
    ApplyMovement 27, _0258
    ApplyMovement 28, _026C
    ApplyMovement 26, _0280
    ApplyMovement 19, _02B4
    WaitMovement
    ScrCmd_065 27
    ScrCmd_065 28
    ScrCmd_065 26
    ScrCmd_065 19
    ScrCmd_003 45, 0x800C
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8004, 0x230
    GoToIf 1, _015B
    CompareVarToValue 0x8004, 0x231
    GoToIf 1, _016F
    End

_015B:
    ScrCmd_186 20, 0x230, 0x254
    ScrCmd_188 20, 14
    GoTo _0183

_016F:
    ScrCmd_186 20, 0x231, 0x254
    ScrCmd_188 20, 14
    GoTo _0183

_0183:
    ClearFlag 0x1B1
    ScrCmd_064 20
    ScrCmd_062 20
    ApplyMovement 20, _02C4
    WaitMovement
    ApplyMovement 0xFF, _02DC
    WaitMovement
    ScrCmd_02C 3
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _01E9
    CompareVarToValue 0x800C, 1
    GoToIf 1, _01C6
    End

_01C6:
    ScrCmd_02C 5
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _01E9
    CompareVarToValue 0x800C, 1
    GoToIf 1, _01C6
    End

_01E9:
    SetVar 0x8004, 0x1B7
    SetVar 0x8005, 1
    ScrCmd_014 0x7FC
    ScrCmd_02C 4
    ScrCmd_034
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8004, 0x230
    GoToIf 1, _0220
    CompareVarToValue 0x8004, 0x231
    GoToIf 1, _0230
    End

_0220:
    ApplyMovement 20, _02CC
    WaitMovement
    GoTo _0240

_0230:
    ApplyMovement 20, _02D4
    WaitMovement
    GoTo _0240

_0240:
    ScrCmd_065 20
    SetFlag 0x107
    ScrCmd_061
    End

    .balign 4, 0
_024C:
    MoveAction_04B
    MoveAction_03F 4
    EndMovement

    .balign 4, 0
_0258:
    MoveAction_00C
    MoveAction_021
    MoveAction_03F 8
    MoveAction_00C 8
    EndMovement

    .balign 4, 0
_026C:
    MoveAction_00C 2
    MoveAction_021 2
    MoveAction_03F 6
    MoveAction_00C 8
    EndMovement

    .balign 4, 0
_0280:
    MoveAction_023
    MoveAction_022
    MoveAction_023
    MoveAction_03F 2
    MoveAction_020
    MoveAction_010 2
    MoveAction_03F 2
    MoveAction_00E 3
    MoveAction_023
    MoveAction_04B
    MoveAction_013 3
    MoveAction_010 8
    EndMovement

    .balign 4, 0
_02B4:
    MoveAction_00C 2
    MoveAction_03F 8
    MoveAction_00C 8
    EndMovement

    .balign 4, 0
_02C4:
    MoveAction_00C 7
    EndMovement

    .balign 4, 0
_02CC:
    MoveAction_00D 9
    EndMovement

    .balign 4, 0
_02D4:
    MoveAction_00D 9
    EndMovement

    .balign 4, 0
_02DC:
    MoveAction_021
    EndMovement

    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 32
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 34
    .byte 0
    .byte 1
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 32
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

_030C:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_031F:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 199
    GoToIf 1, _0364
    ScrCmd_02C 7
    SetVar 0x8004, 0x17A
    SetVar 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _036F
    ScrCmd_014 0x7FC
    SetFlag 199
    GoTo _0364

_0364:
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_036F:
    ScrCmd_014 0x7E1
    ScrCmd_034
    ScrCmd_061
    End

_0379:
    ScrCmd_036 10, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_0390:
    ScrCmd_036 11, 1, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_03A7:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .byte 0
    .byte 0
