    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _0045-.-4
    .short 0xFD13

_000A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_134 11, 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _003A
    ScrCmd_02C 0
    SetVar 0x8004, 11
    ScrCmd_014 0x7D9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_003A:
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0045:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04B 0x5DC
    ScrCmd_04C 0x1B8, 0
    ScrCmd_02C 2
    ScrCmd_04D
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End
