    .include "macros/scrcmd.inc"

    .data

    .long _0103-.-4
    .long _0036-.-4
    .long _0049-.-4
    .long _005C-.-4
    .long _006F-.-4
    .long _0082-.-4
    .long _0095-.-4
    .long _00A8-.-4
    .long _00BB-.-4
    .long _00CE-.-4
    .long _00E1-.-4
    .long _00F2-.-4
    .long _0105-.-4
    .short 0xFD13

_0036:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 0
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0049:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_005C:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_006F:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 3
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0082:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 4
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0095:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00A8:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00BB:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 7
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00CE:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00E1:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00F2:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_02C 10
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0103:
    End

_0105:
    ScrCmd_049 0x5DC
    ScrCmd_060
    SetVar 0x8005, 2
    GoTo _0119
    End

_0119:
    ScrCmd_02C 11
    ScrCmd_041 31, 11, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 13, 0
    ScrCmd_042 14, 1
    ScrCmd_042 15, 2
    ScrCmd_043
    SetVar 0x8008, 0x800C
    CompareVarToValue 0x8008, 0
    GoToIf 1, _0163
    CompareVarToValue 0x8008, 1
    GoToIf 1, _0184
    GoTo _015D
    End

_015D:
    ScrCmd_034
    ScrCmd_061
    End

_0163:
    ScrCmd_014 0x7D6
    SetVar 0x800C, 0x4000
    CompareVarToValue 0x800C, 0
    GoToIf 1, _015D
    ScrCmd_034
    ScrCmd_014 0x802
    ScrCmd_061
    End

_0184:
    ScrCmd_02C 12
    GoTo _0119

    .byte 2
    .byte 0
    .byte 0
