    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _0014-.-4
    .short 0xFD13

_000A:
    SetFlag 0x9D2
    SetFlag 0x28E
    End

_0014:
    ScrCmd_060
    ApplyMovement 0, _0158
    WaitMovement
    ScrCmd_02C 0
    ScrCmd_034
    ApplyMovement 0, _0160
    WaitMovement
    ScrCmd_0E5 0x39E, 0
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0150
    ScrCmd_02C 1
    ScrCmd_034
    ApplyMovement 0, _0168
    WaitMovement
    ApplyMovement 4, _01BC
    WaitMovement
    ScrCmd_02C 2
    ScrCmd_034
    ApplyMovement 4, _01CC
    WaitMovement
    ScrCmd_0E5 0x39F, 0
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0150
    ScrCmd_02C 3
    ScrCmd_034
    ScrCmd_003 15, 0x800C
    ApplyMovement 0, _0174
    ApplyMovement 4, _01F4
    WaitMovement
    ScrCmd_02C 4
    ApplyMovement 4, _01D4
    WaitMovement
    ScrCmd_02C 5
    ScrCmd_034
    ApplyMovement 0, _017C
    WaitMovement
    ScrCmd_02C 6
    ScrCmd_034
    ApplyMovement 0, _0190
    ApplyMovement 0xFF, _01A4
    WaitMovement
    ScrCmd_049 0x603
    ScrCmd_04B 0x603
    ApplyMovement 4, _01DC
    WaitMovement
    ScrCmd_02C 7
    ScrCmd_034
    ApplyMovement 4, _01E4
    WaitMovement
    ScrCmd_049 0x603
    ScrCmd_04B 0x603
    ApplyMovement 14, _0200
    ApplyMovement 0xFF, _01B4
    WaitMovement
    ScrCmd_02C 8
    ScrCmd_02C 9
    ScrCmd_034
    ApplyMovement 14, _0208
    ApplyMovement 15, _0210
    ApplyMovement 16, _021C
    WaitMovement
    ScrCmd_065 14
    ScrCmd_065 0
    ScrCmd_065 4
    ScrCmd_065 15
    ScrCmd_065 16
    SetVar 0x40AD, 1
    ScrCmd_061
    End

_0150:
    ScrCmd_0EB
    ScrCmd_061
    End

    .balign 4, 0
_0158:
    MoveAction_021
    EndMovement

    .balign 4, 0
_0160:
    MoveAction_00D
    EndMovement

    .balign 4, 0
_0168:
    MoveAction_00C
    MoveAction_021
    EndMovement

    .balign 4, 0
_0174:
    MoveAction_04B
    EndMovement

    .balign 4, 0
_017C:
    MoveAction_020
    MoveAction_03F 2
    MoveAction_021
    MoveAction_03F 2
    EndMovement

    .balign 4, 0
_0190:
    MoveAction_00D 2
    MoveAction_03F
    MoveAction_00D
    MoveAction_045
    EndMovement

    .balign 4, 0
_01A4:
    MoveAction_03F 2
    MoveAction_00E
    MoveAction_023
    EndMovement

    .balign 4, 0
_01B4:
    MoveAction_020
    EndMovement

    .balign 4, 0
_01BC:
    MoveAction_022
    MoveAction_03F 2
    MoveAction_021
    EndMovement

    .balign 4, 0
_01CC:
    MoveAction_00D
    EndMovement

    .balign 4, 0
_01D4:
    MoveAction_020
    EndMovement

    .balign 4, 0
_01DC:
    MoveAction_023
    EndMovement

    .balign 4, 0
_01E4:
    MoveAction_00E
    MoveAction_00D 2
    MoveAction_045
    EndMovement

    .balign 4, 0
_01F4:
    MoveAction_03F
    MoveAction_020
    EndMovement

    .balign 4, 0
_0200:
    MoveAction_021
    EndMovement

    .balign 4, 0
_0208:
    MoveAction_00C 7
    EndMovement

    .balign 4, 0
_0210:
    MoveAction_00F
    MoveAction_00C 7
    EndMovement

    .balign 4, 0
_021C:
    MoveAction_03F
    MoveAction_00E
    MoveAction_00C 7
    EndMovement
