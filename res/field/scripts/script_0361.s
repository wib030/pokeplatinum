    .include "macros/scrcmd.inc"

    .data

    .long _000E-.-4
    .long _0014-.-4
    .long _002F-.-4
    .short 0xFD13

_000E:
    SetFlag 0x9E3
    End

_0014:
    CheckFlag 142
    GoToIf 1, _0021
    End

_0021:
    SetFlag 0x1E1
    ScrCmd_065 0
    ClearFlag 142
    End

_002F:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04C 0x1E0, 0
    ScrCmd_02C 0
    ScrCmd_034
    SetFlag 142
    ScrCmd_2BD 0x1E0, 50
    ClearFlag 142
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0085
    ScrCmd_2BC 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _007A
    SetFlag 0x127
    ScrCmd_061
    End

_007A:
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0085:
    ScrCmd_0EB
    ScrCmd_061
    End

    .byte 0
