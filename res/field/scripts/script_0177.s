    .include "macros/scrcmd.inc"

    .data

    .long _0016-.-4
    .long _0022-.-4
    .long _0035-.-4
    .long _0048-.-4
    .long _0067-.-4
    .short 0xFD13

_0016:
    SetVar 0x8007, 0
    ScrCmd_014 0x7D2
    End

_0022:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0035:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 3
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0048:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04B 0x5DC
    ScrCmd_04C 54, 0
    ScrCmd_02C 4
    ScrCmd_04D
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0067:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_15B 6, 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _00EC
    ScrCmd_1BD 0x8000
    CompareVarToValue 0x8000, 0
    CallIf 1, _00C8
    CompareVarToValue 0x8000, 1
    CallIf 1, _00C8
    CompareVarToValue 0x8000, 2
    CallIf 1, _00D4
    CompareVarToValue 0x8000, 3
    CallIf 1, _00E0
    ScrCmd_0CD 0
    ScrCmd_02C 0
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00C8:
    ApplyMovement 4, _00FC
    WaitMovement
    Return

_00D4:
    ApplyMovement 4, _0104
    WaitMovement
    Return

_00E0:
    ApplyMovement 4, _010C
    WaitMovement
    Return

_00EC:
    ScrCmd_0CD 0
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .balign 4, 0
_00FC:
    MoveAction_029 2
    EndMovement

    .balign 4, 0
_0104:
    MoveAction_02B 2
    EndMovement

    .balign 4, 0
_010C:
    MoveAction_02A 2
    EndMovement
