    .include "macros/scrcmd.inc"

    .data

    .long _0020-.-4
    .long _0022-.-4
    .long _01CC-.-4
    .long _0298-.-4
    .long _02BF-.-4
    .long _0300-.-4
    .long _001E-.-4
    .short 0xFD13

_001E:
    End

_0020:
    End

_0022:
    ScrCmd_060
    ApplyMovement 0xFF, _0194
    WaitMovement
    ScrCmd_069 0x4000, 0x4001
    CompareVarToValue 0x4000, 5
    CallIf 1, _017D
    ScrCmd_072 20, 2
    ScrCmd_02C 0
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _006A
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0156
    End

_006A:
    ScrCmd_252 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 5, _008C
    ScrCmd_177 0x800C
    CompareVarToValue 0x800C, 6
    GoToIf 1, _0168
_008C:
    ScrCmd_02C 1
    ScrCmd_071 0x800C, 0x1F4
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0141
    ScrCmd_049 0x644
    ScrCmd_070 0x1F4
    ScrCmd_074
    ScrCmd_02C 2
    ScrCmd_0CD 0
    ScrCmd_02C 3
    ScrCmd_02C 4
    ScrCmd_034
    ScrCmd_073
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8004, 4
    GoToIf 1, _00E2
    CompareVarToValue 0x8004, 5
    GoToIf 1, _00F2
    End

_00E2:
    ApplyMovement 0xFF, _019C
    WaitMovement
    GoTo _0102

_00F2:
    ApplyMovement 0xFF, _01B4
    WaitMovement
    GoTo _0102

_0102:
    SetVar 0x40DA, 1
    ScrCmd_202 0
    ScrCmd_11B 125, 2, 5, 2, 1
    ScrCmd_049 0x603
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_0BE 0x1FD, 0, 68, 116, 0
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    End

_0141:
    ScrCmd_02C 6
    ScrCmd_034
    ScrCmd_073
    ApplyMovement 0xFF, _01C4
    WaitMovement
    ScrCmd_061
    End

_0156:
    ScrCmd_034
    ScrCmd_073
    ApplyMovement 0xFF, _01C4
    WaitMovement
    ScrCmd_061
    End

_0168:
    ScrCmd_073
    ScrCmd_02C 7
    ScrCmd_034
    ApplyMovement 0xFF, _01C4
    WaitMovement
    ScrCmd_061
    End

_017D:
    ApplyMovement 0xFF, _018C
    WaitMovement
    Return

    .balign 4, 0
_018C:
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0194:
    MoveAction_022
    EndMovement

    .balign 4, 0
_019C:
    MoveAction_020
    MoveAction_00C 3
    MoveAction_00F
    MoveAction_00C
    MoveAction_045
    EndMovement

    .balign 4, 0
_01B4:
    MoveAction_020
    MoveAction_00C 4
    MoveAction_045
    EndMovement

    .balign 4, 0
_01C4:
    MoveAction_00D
    EndMovement

_01CC:
    ScrCmd_060
    ScrCmd_02C 9
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _01F1
    CompareVarToValue 0x800C, 1
    GoToIf 1, _024E
    End

_01F1:
    ScrCmd_02C 10
    ScrCmd_034
    ApplyMovement 0xFF, _0280
    WaitMovement
    SetVar 0x40DA, 0
    ScrCmd_202 1
    ScrCmd_31B 0x4002
    CompareVarToValue 0x4002, 5
    GoToIf 4, _021E
    ScrCmd_061
    End

_021E:
    CheckFlag 163
    GoToIf 1, _024A
    ApplyMovement 2, _0334
    WaitMovement
    ScrCmd_02C 18
    SetVar 0x8004, 22
    ScrCmd_014 0x7D9
    SetFlag 163
    ScrCmd_034
    ScrCmd_061
    End

_024A:
    ScrCmd_061
    End

_024E:
    ScrCmd_034
    ApplyMovement 0xFF, _0288
    WaitMovement
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_0BE 0x1FD, 0, 68, 116, 0
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    End

    .balign 4, 0
_0280:
    MoveAction_00D 5
    EndMovement

    .balign 4, 0
_0288:
    MoveAction_000
    MoveAction_00C
    MoveAction_045
    EndMovement

_0298:
    ScrCmd_060
    ApplyMovement 0xFF, _0280
    WaitMovement
    SetVar 0x40DA, 0
    ScrCmd_31B 0x4002
    CompareVarToValue 0x4002, 5
    GoToIf 4, _021E
    ScrCmd_061
    End

_02BF:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 12
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _02EA
    CompareVarToValue 0x800C, 1
    GoToIf 1, _02F5
    End

_02EA:
    ScrCmd_02C 13
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_02F5:
    ScrCmd_02C 14
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0300:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 163
    GoToIf 1, _031E
    ScrCmd_02C 15
    GoTo _0329
    End

_031E:
    ScrCmd_02C 19
    GoTo _0329
    End

_0329:
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .balign 4, 0
_0334:
    MoveAction_00F 4
    MoveAction_020
    EndMovement

    .byte 14
    .byte 0
    .byte 1
    .byte 0
    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 15
    .byte 0
    .byte 1
    .byte 0
    .byte 34
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
