    .include "macros/scrcmd.inc"

    .data

    .long _0006-.-4
    .short 0xFD13

_0006:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 0x13A
    GoToIf 1, _0055
    ScrCmd_02C 0
    SetVar 0x8004, 69
    SetVar 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _004B
    SetFlag 0x13A
    ScrCmd_014 0x7E0
    ScrCmd_034
    ScrCmd_061
    End

_004B:
    ScrCmd_014 0x7E1
    ScrCmd_034
    ScrCmd_061
    End

_0055:
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End
