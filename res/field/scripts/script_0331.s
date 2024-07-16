    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _0035-.-4
    .short 0xFD13

_000A:
    CheckFlag 0x11D
    GoToIf 1, _0029
    ScrCmd_22D 2, 0x4000
    CompareVarToValue 0x4000, 1
    CallIf 1, _002F
    End

_0029:
    SetFlag 0x24D
    End

_002F:
    ClearFlag 0x24D
    Return

_0035:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 0
    SetVar 0x8004, 233
    SetVar 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _00A8
    ScrCmd_014 0x7FC
    SetFlag 0x11D
    ScrCmd_02C 1
    ScrCmd_034
    ScrCmd_1BD 0x8004
    CompareVarToValue 0x8004, 2
    GoToIf 1, _00B2
    CompareVarToValue 0x8004, 3
    GoToIf 1, _00D0
    CompareVarToValue 0x8004, 0
    GoToIf 1, _00B2
    CompareVarToValue 0x8004, 1
    GoToIf 1, _00D0
    End

_00A8:
    ScrCmd_014 0x7E1
    ScrCmd_034
    ScrCmd_061
    End

_00B2:
    ApplyMovement 0, _00F0
    WaitMovement
    ScrCmd_049 0x603
    ScrCmd_065 0
    ScrCmd_04B 0x603
    ScrCmd_04A 0x603
    ScrCmd_061
    End

_00D0:
    ApplyMovement 0, _00FC
    WaitMovement
    ScrCmd_049 0x603
    ScrCmd_065 0
    ScrCmd_04B 0x603
    ScrCmd_04A 0x603
    ScrCmd_061
    End

    .balign 4, 0
_00F0:
    MoveAction_00E
    MoveAction_00D 3
    EndMovement

    .balign 4, 0
_00FC:
    MoveAction_00D 2
    MoveAction_00E
    MoveAction_00D
    EndMovement
