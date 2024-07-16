    .include "macros/scrcmd.inc"

    .data

    .long _0012-.-4
    .long _004E-.-4
    .long _0066-.-4
    .long _014C-.-4
    .short 0xFD13

_0012:
    CheckFlag 0x13E
    GoToIf 0, _002A
    CheckFlag 0x13E
    GoToIf 1, _003C
    End

_002A:
    ScrCmd_18A 2, 80, 0x348
    ScrCmd_18A 3, 81, 0x348
    End

_003C:
    ScrCmd_18A 1, 80, 0x348
    ScrCmd_18A 0, 81, 0x348
    End

_004E:
    CheckFlag 0x13E
    GoToIf 0, _002A
    CheckFlag 0x13E
    GoToIf 1, _003C
    End

_0066:
    ScrCmd_060
    ApplyMovement 242, _0124
    ApplyMovement 0xFF, _0144
    WaitMovement
    ScrCmd_0CE 0
    ScrCmd_02C 0
    ScrCmd_034
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8004, 80
    GoToIf 1, _00D8
    CompareVarToValue 0x8004, 81
    GoToIf 1, _00D8
    CompareVarToValue 0x8004, 82
    GoToIf 1, _00D8
    CompareVarToValue 0x8004, 83
    GoToIf 1, _00D8
    CompareVarToValue 0x8004, 84
    GoToIf 1, _00D8
    CompareVarToValue 0x8004, 85
    GoToIf 1, _00D8
    End

_00D8:
    ApplyMovement 0xFF, _0138
    ApplyMovement 242, _012C
    WaitMovement
    GoTo _00F0

_00F0:
    SetVar 0x4082, 1
    ScrCmd_061
    ScrCmd_049 0x603
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_0BE 0x137, 0, 46, 54, 0
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    End

    .balign 4, 0
_0124:
    MoveAction_020
    EndMovement

    .balign 4, 0
_012C:
    MoveAction_00C 2
    MoveAction_045
    EndMovement

    .balign 4, 0
_0138:
    MoveAction_00C
    MoveAction_045
    EndMovement

    .balign 4, 0
_0144:
    MoveAction_021
    EndMovement

_014C:
    ScrCmd_037 3, 0
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03A 3, 0x800C
    ScrCmd_014 0x7D0
    End

    .byte 0
    .byte 0
    .byte 0
