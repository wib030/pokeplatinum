    .include "macros/scrcmd.inc"

    .data

    .long _0006-.-4
    .short 0xFD13

_0006:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_1E5 48
    ScrCmd_235 0, 0x800C
    ScrCmd_1F9 0x800C
    SetVar 0x8008, 0x800C
    CompareVarToValue 0x8008, 0
    GoToIf 1, _005C
    CompareVarToValue 0x8008, 1
    GoToIf 1, _0074
    CompareVarToValue 0x8008, 2
    GoToIf 1, _0092
    CompareVarToValue 0x8008, 3
    GoToIf 1, _00AA
    GoTo _005C
    End

_005C:
    ScrCmd_235 4, 0x8004, 0x8005
    ScrCmd_1FB 0x8004, 0x8005
    ScrCmd_031
    GoTo _0129
    End

_0074:
    ScrCmd_235 1, 2, 0x8004, 0x8005
    ScrCmd_1FB 0x8004, 0x8005
    ScrCmd_031
    ScrCmd_235 2
    GoTo _0129
    End

_0092:
    ScrCmd_235 1, 0, 0x8004, 0x8005
    ScrCmd_1FB 0x8004, 0x8005
    GoTo _00AA
    End

_00AA:
    ScrCmd_235 6, 0x8006
    CompareVarToValue 0x8006, 0
    GoToIf 5, _00EB
    GoTo _00C5
    End

_00C5:
    ScrCmd_235 1, 3, 0x8004, 0x8005
    ScrCmd_1F9 0x8004
    ScrCmd_1F9 0x8005
    ScrCmd_1FB 0x8004, 0x8005
    ScrCmd_235 2
    ScrCmd_031
    GoTo _0129
    End

_00EB:
    ScrCmd_235 1, 1, 0x8004, 0x8005
    ScrCmd_1F9 0x8004
    ScrCmd_1F9 0x8005
    ScrCmd_1FB 0x8004, 0x8005
    ScrCmd_235 3, 0x8006, 0x8004, 0x8005
    ScrCmd_1F9 0x8006
    ScrCmd_1F9 0x8004
    ScrCmd_1F9 0x8005
    ScrCmd_1FB 0x8004, 0x8005
    ScrCmd_031
    GoTo _0129
    End

_0129:
    ScrCmd_034
    ScrCmd_061
    End

    .byte 0
