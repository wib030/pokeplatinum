    .include "macros/scrcmd.inc"

    .data

    .long _001A-.-4
    .long _0022-.-4
    .long _0198-.-4
    .long _01B7-.-4
    .long _01D6-.-4
    .long _01FF-.-4
    .short 0xFD13

_001A:
    ScrCmd_2CD
    End

    .byte 205
    .byte 2
    .byte 2
    .byte 0

_0022:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ApplyMovement 2, _013C
    WaitMovement
    ScrCmd_02C 0
    ScrCmd_034
    ScrCmd_0E5 0x196, 0
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0135
    ScrCmd_02C 1
    ScrCmd_034
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_065 0
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_003 15, 0x800C
    ScrCmd_1BD 0x8004
    CompareVarToValue 0x8004, 0
    GoToIf 1, _00A4
    CompareVarToValue 0x8004, 2
    GoToIf 1, _00BE
    CompareVarToValue 0x8004, 3
    GoToIf 1, _00D0
    End

_00A4:
    ApplyMovement 2, _0144
    ApplyMovement 0xFF, _0174
    WaitMovement
    GoTo _00E2
    End

_00BE:
    ApplyMovement 2, _015C
    WaitMovement
    GoTo _00E2
    End

_00D0:
    ApplyMovement 2, _0168
    WaitMovement
    GoTo _00E2
    End

_00E2:
    ScrCmd_02C 4
    ScrCmd_034
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    SetFlag 0x1FD
    SetFlag 0x1BB
    SetFlag 0x1C2
    SetFlag 129
    ClearFlag 0x192
    ClearFlag 0x200
    ClearFlag 0x1FE
    ScrCmd_065 2
    ScrCmd_065 1
    ScrCmd_065 3
    SetVar 0x407A, 3
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    SetFlag 0x987
    ScrCmd_061
    End

_0135:
    ScrCmd_0EB
    ScrCmd_061
    End

    .balign 4, 0
_013C:
    MoveAction_020
    EndMovement

    .balign 4, 0
_0144:
    MoveAction_00C
    EndMovement

    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 15
    .byte 0
    .byte 2
    .byte 0
    .byte 32
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_015C:
    MoveAction_00C 3
    MoveAction_023
    EndMovement

    .balign 4, 0
_0168:
    MoveAction_00C 3
    MoveAction_022
    EndMovement

    .balign 4, 0
_0174:
    MoveAction_03F
    MoveAction_021
    EndMovement

    .byte 34
    .byte 0
    .byte 1
    .byte 0
    .byte 62
    .byte 0
    .byte 1
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 34
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

_0198:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04B 0x5DC
    ScrCmd_04C 35, 0
    ScrCmd_02C 2
    ScrCmd_04D
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_01B7:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_04B 0x5DC
    ScrCmd_04C 0x1AB, 0
    ScrCmd_02C 5
    ScrCmd_04D
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_01D6:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 129
    GoToIf 1, _01F4
    ScrCmd_02C 3
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_01F4:
    ScrCmd_02C 4
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_01FF:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End
