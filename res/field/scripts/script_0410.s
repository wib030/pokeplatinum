    .include "macros/scrcmd.inc"

    .data

    .long _0006-.-4
    .short 0xFD13

_0006:
    ScrCmd_060
    ScrCmd_1B7 0x8000, 5
    SetVar 0x8008, 0x8000
    CompareVarToValue 0x8008, 0
    GoToIf 1, _0057
    CompareVarToValue 0x8008, 1
    GoToIf 1, _0071
    CompareVarToValue 0x8008, 2
    GoToIf 1, _008B
    CompareVarToValue 0x8008, 3
    GoToIf 1, _00A5
    CompareVarToValue 0x8008, 4
    GoToIf 1, _00BF
    End

_0057:
    ApplyMovement 0xFF, _0148
    ApplyMovement 0, _01F0
    WaitMovement
    GoTo _00D9
    End

_0071:
    ApplyMovement 0xFF, _0154
    ApplyMovement 0, _0200
    WaitMovement
    GoTo _00D9
    End

_008B:
    ApplyMovement 0xFF, _0160
    ApplyMovement 0, _0210
    WaitMovement
    GoTo _00D9
    End

_00A5:
    ApplyMovement 0xFF, _016C
    ApplyMovement 0, _0220
    WaitMovement
    GoTo _00D9
    End

_00BF:
    ApplyMovement 0xFF, _0178
    ApplyMovement 0, _0230
    WaitMovement
    GoTo _00D9
    End

_00D9:
    ScrCmd_02C 0
    ScrCmd_030
    ScrCmd_034
    ApplyMovement 0xFF, _0184
    ApplyMovement 0, _0240
    WaitMovement
    ScrCmd_049 0x64F
    ApplyMovement 0xFF, _01E8
    WaitMovement
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    SetVar 0x4003, 1
    ScrCmd_0BE 0x405C, 0, 5, 2, 1
    ScrCmd_1F8
    ScrCmd_0B3 0x800C
    SetVar 0x8004, 0x800C
    ScrCmd_2F7 0x8004
    ScrCmd_0A1
    SetVar 0x4080, 1
    ScrCmd_1B2 0xFF
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    End

    .balign 4, 0
_0148:
    MoveAction_00C 7
    MoveAction_00E 11
    EndMovement

    .balign 4, 0
_0154:
    MoveAction_00C 7
    MoveAction_00E 9
    EndMovement

    .balign 4, 0
_0160:
    MoveAction_00C 7
    MoveAction_00E 7
    EndMovement

    .balign 4, 0
_016C:
    MoveAction_00C 7
    MoveAction_00E 5
    EndMovement

    .balign 4, 0
_0178:
    MoveAction_00C 7
    MoveAction_00E 3
    EndMovement

    .balign 4, 0
_0184:
    MoveAction_00C
    MoveAction_002
    MoveAction_03D
    MoveAction_001
    MoveAction_03D
    MoveAction_003
    MoveAction_03D
    MoveAction_000
    MoveAction_03D
    MoveAction_002
    MoveAction_03D
    MoveAction_001
    MoveAction_03D
    MoveAction_003
    MoveAction_03D
    MoveAction_000
    MoveAction_03D
    MoveAction_002
    MoveAction_03D
    MoveAction_001
    MoveAction_03D
    MoveAction_003
    MoveAction_03D
    MoveAction_000
    EndMovement

    .balign 4, 0
_01E8:
    MoveAction_043
    EndMovement

    .balign 4, 0
_01F0:
    MoveAction_00C 6
    MoveAction_00E 12
    MoveAction_023
    EndMovement

    .balign 4, 0
_0200:
    MoveAction_00C 6
    MoveAction_00E 10
    MoveAction_023
    EndMovement

    .balign 4, 0
_0210:
    MoveAction_00C 6
    MoveAction_00E 8
    MoveAction_023
    EndMovement

    .balign 4, 0
_0220:
    MoveAction_00C 6
    MoveAction_00E 6
    MoveAction_023
    EndMovement

    .balign 4, 0
_0230:
    MoveAction_00C 6
    MoveAction_00E 4
    MoveAction_023
    EndMovement

    .balign 4, 0
_0240:
    MoveAction_020
    EndMovement
