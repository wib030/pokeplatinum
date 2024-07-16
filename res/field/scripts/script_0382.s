    .include "macros/scrcmd.inc"

    .data

    .long _000E-.-4
    .long _0012-.-4
    .long _0084-.-4
    .short 0xFD13

_000E:
    ScrCmd_2F2
    End

_0012:
    ScrCmd_049 0x5DC
    ScrCmd_060
    CompareVarToValue 0x4055, 5
    GoToIf 1, _0084
    ScrCmd_317 0x8004, 0x8005, 0x8006
    CompareVarToValue 0x8005, 232
    GoToIf 1, _0059
    ApplyMovement 128, _00DC
    WaitMovement
    ScrCmd_02C 0
    ScrCmd_034
    ApplyMovement 128, _00E4
    WaitMovement
    GoTo _007A

_0059:
    ApplyMovement 128, _00F0
    WaitMovement
    ScrCmd_02C 0
    ScrCmd_034
    ApplyMovement 128, _00F8
    ApplyMovement 0xFF, _011C
    WaitMovement
_007A:
    SetVar 0x4055, 5
    ScrCmd_061
    End

_0084:
    ScrCmd_317 0x8004, 0x8005, 0x8006
    CompareVarToValue 0x8005, 231
    GoToIf 1, _00B6
    CompareVarToValue 0x8005, 232
    GoToIf 1, _00C6
    ApplyMovement 128, _0104
    WaitMovement
    GoTo _00D0

_00B6:
    ApplyMovement 128, _010C
    WaitMovement
    GoTo _00D0

_00C6:
    ApplyMovement 128, _0114
    WaitMovement
_00D0:
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .balign 4, 0
_00DC:
    MoveAction_001
    EndMovement

    .balign 4, 0
_00E4:
    MoveAction_06A
    MoveAction_002
    EndMovement

    .balign 4, 0
_00F0:
    MoveAction_003
    EndMovement

    .balign 4, 0
_00F8:
    MoveAction_06A
    MoveAction_002
    EndMovement

    .balign 4, 0
_0104:
    MoveAction_002
    EndMovement

    .balign 4, 0
_010C:
    MoveAction_003
    EndMovement

    .balign 4, 0
_0114:
    MoveAction_001
    EndMovement

    .balign 4, 0
_011C:
    MoveAction_06B
    MoveAction_000
    EndMovement
