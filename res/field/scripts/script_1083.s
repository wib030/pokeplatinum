    .include "macros/scrcmd.inc"

    .data

    .long _000E-.-4
    .long _02BB-.-4
    .long _02CE-.-4
    .short 0xFD13

_000E:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 220
    GoToIf 1, _0216
    CheckFlag 0xAB1
    GoToIf 1, _02B0
    CheckFlag 216
    GoToIf 0, _004A
    CompareVarToValue 0x40E5, 0
    GoToIf 1, _0057
    GoTo _0081

_004A:
    SetFlag 216
    ScrCmd_02C 0
    GoTo _0060

_0057:
    ScrCmd_02C 1
    GoTo _0060

_0060:
    ScrCmd_218 0x800C
    SetVar 0x40E5, 0x800C
    ScrCmd_219 1
    ScrCmd_0DA 0, 0x40E5, 0, 0
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0081:
    ScrCmd_02C 3
    ScrCmd_21A 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _029F
    ScrCmd_1C0 0x800C, 0x40E5
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0283
    GoTo _00AE

_00AE:
    ScrCmd_0DA 0, 0x40E5, 0, 0
    ScrCmd_02C 4
    ScrCmd_1B7 0x800C, 12
    CompareVarToValue 0x800C, 0
    CallIf 1, _0189
    CompareVarToValue 0x800C, 1
    CallIf 1, _0191
    CompareVarToValue 0x800C, 2
    CallIf 1, _0199
    CompareVarToValue 0x800C, 3
    CallIf 1, _01A1
    CompareVarToValue 0x800C, 4
    CallIf 1, _01A9
    CompareVarToValue 0x800C, 5
    CallIf 1, _01B1
    CompareVarToValue 0x800C, 6
    CallIf 1, _01B9
    CompareVarToValue 0x800C, 7
    CallIf 1, _01C1
    CompareVarToValue 0x800C, 8
    CallIf 1, _01C9
    CompareVarToValue 0x800C, 9
    CallIf 1, _01D1
    CompareVarToValue 0x800C, 10
    CallIf 1, _01D9
    CompareVarToValue 0x800C, 11
    CallIf 1, _01E1
    SetVar 0x8005, 3
    ScrCmd_07D 93, 1, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 5, _01E9
    SetFlag 0xAB1
    CompareVarToValue 0x800C, 0
    GoToIf 1, _023F
    End

_0189:
    SetVar 0x8004, 2
    Return

_0191:
    SetVar 0x8004, 3
    Return

_0199:
    SetVar 0x8004, 4
    Return

_01A1:
    SetVar 0x8004, 6
    Return

_01A9:
    SetVar 0x8004, 7
    Return

_01B1:
    SetVar 0x8004, 8
    Return

_01B9:
    SetVar 0x8004, 9
    Return

_01C1:
    SetVar 0x8004, 10
    Return

_01C9:
    SetVar 0x8004, 11
    Return

_01D1:
    SetVar 0x8004, 13
    Return

_01D9:
    SetVar 0x8004, 14
    Return

_01E1:
    SetVar 0x8004, 15
    Return

_01E9:
    ScrCmd_014 0x7FC
    SetVar 0x8004, 93
    SetVar 0x8005, 1
    ScrCmd_014 0x7FC
    ClearFlag 220
    SetVar 0x40E5, 0
    SetFlag 0xAB1
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0216:
    ScrCmd_07D 93, 1, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0251
    SetVar 0x8004, 0x4117
    SetVar 0x8005, 3
    GoTo _025C
    End

_023F:
    SetVar 0x4117, 0x8004
    SetFlag 220
    GoTo _0251
    End

_0251:
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_025C:
    ScrCmd_02C 7
    ClearFlag 220
    SetVar 0x40E5, 0
    ScrCmd_014 0x7FC
    SetVar 0x8004, 93
    SetVar 0x8005, 1
    ScrCmd_014 0x7E0
    ScrCmd_034
    ScrCmd_061
    End

_0283:
    ScrCmd_0DA 0, 0x40E5, 0, 0
    ScrCmd_21A 0x800C
    ScrCmd_0D5 1, 0x800C
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_029F:
    ScrCmd_02C 9
    SetVar 0x40E5, 0
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_02B0:
    ScrCmd_02C 10
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_02BB:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 11
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_02CE:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_02C 12
    ScrCmd_02C 13
    ScrCmd_040 1, 1, 0, 1, 0x800C
    ScrCmd_29D 0x107, 0
    ScrCmd_29D 0x108, 1
    ScrCmd_29D 0x109, 2
    ScrCmd_29D 0x10B, 3
    ScrCmd_29D 0x10A, 4
    ScrCmd_043
    SetVar 0x8008, 0x800C
    CompareVarToValue 0x8008, 0
    GoToIf 1, _0344
    CompareVarToValue 0x8008, 1
    GoToIf 1, _034F
    CompareVarToValue 0x8008, 2
    GoToIf 1, _035A
    CompareVarToValue 0x8008, 3
    GoToIf 1, _0365
    GoTo _0370
    End

_0344:
    ScrCmd_02C 14
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_034F:
    ScrCmd_02C 15
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_035A:
    ScrCmd_02C 16
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0365:
    ScrCmd_02C 17
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0370:
    ScrCmd_034
    ScrCmd_061
    End

    .byte 0
    .byte 0
