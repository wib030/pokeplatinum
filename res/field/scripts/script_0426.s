    .include "macros/scrcmd.inc"

    .data

    .long _0008-.-4
    .short 0xFD13
    End

_0008:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 0
    GoTo _001B
    End

_001B:
    ScrCmd_02C 1
    ScrCmd_041 1, 1, 0, 1, 0x800C
    ScrCmd_042 9, 0
    ScrCmd_042 10, 1
    ScrCmd_042 11, 2
    ScrCmd_042 12, 3
    ScrCmd_043
    SetVar 0x8008, 0x800C
    CompareVarToValue 0x8008, 0
    GoToIf 1, _00D0
    CompareVarToValue 0x8008, 1
    GoToIf 1, _0121
    CompareVarToValue 0x8008, 2
    GoToIf 1, _0078
    GoTo _006D
    End

_006D:
    ScrCmd_02C 33
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0078:
    ScrCmd_02C 32
    ScrCmd_031
    GoTo _001B
    End

_0085:
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_034
    ScrCmd_061
    Return

_0097:
    ScrCmd_0A1
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    Return

_00AF:
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00BA:
    ScrCmd_02C 6
    GoTo _006D
    End

_00C5:
    ScrCmd_02C 7
    GoTo _006D
    End

_00D0:
    ScrCmd_07E 0x1C1, 1, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _00AF
    ScrCmd_1D8 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _00BA
    CompareVarToValue 0x800C, 2
    GoToIf 1, _00C5
    ScrCmd_003 30, 0x800C
    Call _0085
    ScrCmd_1D7 0
    Call _0097
    GoTo _006D
    End

_0121:
    ScrCmd_07E 0x1C1, 1, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _00AF
    ScrCmd_1D8 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _00BA
    CompareVarToValue 0x800C, 2
    GoToIf 1, _00C5
    ScrCmd_014 0x7D6
    SetVar 0x800C, 0x4000
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0173
    GoTo _006D
    End

_0173:
    ScrCmd_02C 13
    ScrCmd_040 30, 1, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 13, 0
    ScrCmd_042 14, 1
    ScrCmd_042 5, 2
    ScrCmd_043
    SetVar 0x8008, 0x800C
    CompareVarToValue 0x8008, 0
    GoToIf 1, _01B7
    CompareVarToValue 0x8008, 1
    GoToIf 1, _020D
    GoTo _006D
    End

_01B7:
    ScrCmd_02C 2
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0173
    ScrCmd_034
    ScrCmd_0F2 6, 0, 0, 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _01F9
    CompareVarToValue 0x800C, 3
    GoToIf 1, _0203
    GoTo _0263
    End

_01F9:
    ScrCmd_150
    GoTo _0173
    End

_0203:
    ScrCmd_150
    GoTo _0173
    End

_020D:
    ScrCmd_02C 2
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0173
    ScrCmd_034
    ScrCmd_0F3 6, 0, 0, 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _024F
    CompareVarToValue 0x800C, 3
    GoToIf 1, _0259
    GoTo _0263
    End

_024F:
    ScrCmd_150
    GoTo _0173
    End

_0259:
    ScrCmd_150
    GoTo _0173
    End

_0263:
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_061
    ScrCmd_1D7 1
    ScrCmd_150
    Call _0097
    GoTo _006D

    .byte 2
    .byte 0
    .byte 0
    .byte 0
    .byte 0
