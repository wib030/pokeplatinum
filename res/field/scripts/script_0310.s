    .include "macros/scrcmd.inc"

    .data

    .long _000E-.-4
    .long _0062-.-4
    .long _007D-.-4
    .short 0xFD13

_000E:
    SetFlag 0x9D5
    ScrCmd_22D 2, 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _005C
    ScrCmd_07E 0x1C4, 1, 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _005C
    ScrCmd_28B 1, 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _005C
    CheckFlag 0x123
    GoToIf 1, _005C
    ClearFlag 0x251
    End

_005C:
    SetFlag 0x251
    End

_0062:
    CheckFlag 142
    GoToIf 1, _006F
    End

_006F:
    SetFlag 0x251
    ScrCmd_065 0
    ClearFlag 142
    End

_007D:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04C 0x1EC, 0
    ScrCmd_02C 0
    ScrCmd_034
    SetFlag 142
    ScrCmd_318 0x1EC, 30
    ClearFlag 142
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _00D7
    ScrCmd_2BC 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _00C8
    SetFlag 0x123
    ScrCmd_061
    End

_00C8:
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ClearFlag 0x251
    ScrCmd_061
    End

_00D7:
    ScrCmd_0EB
    ClearFlag 0x251
    ScrCmd_061
    End

    .byte 0
    .byte 0
    .byte 0
