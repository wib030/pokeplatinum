    .include "macros/scrcmd.inc"

    .data

    .long _0012-.-4
    .long _002B-.-4
    .long _0041-.-4
    .long _0057-.-4
    .short 0xFD13

_0012:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 0
    ScrCmd_014 0x7E3
    ScrCmd_035
    ScrCmd_147 1
    ScrCmd_061
    End

_002B:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_014 0x7E3
    ScrCmd_035
    ScrCmd_147 1
    ScrCmd_061
    End

_0041:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_014 0x7E3
    ScrCmd_035
    ScrCmd_148 14
    ScrCmd_061
    End

_0057:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 1
    ScrCmd_268 0x800C
    CompareVarToValue 0x800C, 4
    GoToIf 0, _0113
    CompareVarToValue 0x800C, 10
    GoToIf 0, _0093
    CompareVarToValue 0x800C, 20
    GoToIf 0, _00D3
    GoTo _0113

_0093:
    CheckFlag 0x134
    GoToIf 1, _0153
    ScrCmd_02C 2
    SetVar 0x8004, 0x129
    SetVar 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0174
    ScrCmd_014 0x7FC
    SetFlag 0x134
    ScrCmd_02C 3
    GoTo _0153

_00D3:
    CheckFlag 0x135
    GoToIf 1, _015E
    ScrCmd_02C 2
    SetVar 0x8004, 240
    SetVar 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0174
    ScrCmd_014 0x7FC
    SetFlag 0x135
    ScrCmd_02C 4
    GoTo _015E

_0113:
    CheckFlag 0x136
    GoToIf 1, _0169
    ScrCmd_02C 2
    SetVar 0x8004, 0x10B
    SetVar 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0174
    ScrCmd_014 0x7FC
    SetFlag 0x136
    ScrCmd_02C 5
    GoTo _0169

_0153:
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_015E:
    ScrCmd_02C 7
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0169:
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0174:
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .byte 0
