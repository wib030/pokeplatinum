    .include "macros/scrcmd.inc"

    .data

    .long _000E-.-4
    .long _0014-.-4
    .long _0027-.-4
    .short 0xFD13

_000E:
    SetFlag 0x9E8
    End

_0014:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 0
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0027:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 1
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .byte 0
    .byte 0
