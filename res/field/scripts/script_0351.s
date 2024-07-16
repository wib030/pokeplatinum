    .include "macros/scrcmd.inc"

    .data

    .long _001E-.-4
    .long _004E-.-4
    .long _006F-.-4
    .long _0418-.-4
    .long _044B-.-4
    .long _048A-.-4
    .long _048C-.-4
    .short 0xFD13

_001E:
    ScrCmd_14D 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _003E
    CompareVarToValue 0x4000, 1
    GoToIf 1, _0046
    End

_003E:
    SetVar 0x4020, 97
    End

_0046:
    SetVar 0x4020, 0
    End

_004E:
    CheckFlag 142
    GoToIf 1, _005B
    End

_005B:
    SetFlag 0x18F
    ScrCmd_065 3
    ScrCmd_065 2
    ClearFlag 142
    End
    End

_006F:
    ScrCmd_060
    ScrCmd_162
    ApplyMovement 5, _0298
    ApplyMovement 0xFF, _0368
    WaitMovement
    ScrCmd_0CE 0
    ScrCmd_02C 0
    ScrCmd_034
    ScrCmd_066 46, 53
    ApplyMovement 241, _01B4
    WaitMovement
    ScrCmd_003 15, 0x800C
    ScrCmd_02C 1
    ScrCmd_034
    ScrCmd_003 30, 0x800C
    ApplyMovement 4, _01E0
    ApplyMovement 241, _01C0
    WaitMovement
    ScrCmd_067
    ScrCmd_02C 2
    ScrCmd_034
    ApplyMovement 5, _02A0
    ApplyMovement 0xFF, _0370
    WaitMovement
    ApplyMovement 4, _01F0
    WaitMovement
    ScrCmd_049 0x603
    ScrCmd_065 4
    ScrCmd_003 50, 0x800C
    ApplyMovement 0xFF, _0378
    ApplyMovement 5, _02AC
    WaitMovement
    ScrCmd_0CE 0
    ScrCmd_02C 3
    ScrCmd_034
    ApplyMovement 5, _02B8
    ApplyMovement 0xFF, _0380
    WaitMovement
    ScrCmd_003 30, 0x800C
    ScrCmd_0CD 1
    ScrCmd_02C 4
    ScrCmd_04C 0x1E1, 0
    ScrCmd_02C 5
    ScrCmd_04D
    ScrCmd_034
    ApplyMovement 5, _02C0
    ApplyMovement 0xFF, _0388
    WaitMovement
    ScrCmd_003 15, 0x800C
    ApplyMovement 5, _02F0
    WaitMovement
    ScrCmd_0CE 0
    ScrCmd_0CD 1
    ScrCmd_02C 6
    ScrCmd_034
    ApplyMovement 5, _02E8
    WaitMovement
    ScrCmd_003 15, 0x800C
    ScrCmd_0CD 1
    ScrCmd_02C 7
    ScrCmd_034
    ApplyMovement 5, _02F8
    ApplyMovement 0xFF, _039C
    WaitMovement
    SetFlag 0x196
    ScrCmd_065 5
    ScrCmd_049 0x603
    GoTo _01A1
    End

_01A1:
    SetVar 0x4086, 4
    SetVar 0x4095, 1
    ScrCmd_061
    End

    .balign 4, 0
_01B4:
    MoveAction_03F
    MoveAction_00C 9
    EndMovement

    .balign 4, 0
_01C0:
    MoveAction_00D 9
    EndMovement

    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 63
    .byte 0
    .byte 2
    .byte 0
    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_01E0:
    MoveAction_00D 5
    MoveAction_00E
    MoveAction_00D 4
    EndMovement

    .balign 4, 0
_01F0:
    MoveAction_00D 3
    MoveAction_045
    EndMovement

    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 34
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
    .byte 13
    .byte 0
    .byte 4
    .byte 0
    .byte 14
    .byte 0
    .byte 2
    .byte 0
    .byte 13
    .byte 0
    .byte 5
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 14
    .byte 0
    .byte 1
    .byte 0
    .byte 13
    .byte 0
    .byte 5
    .byte 0
    .byte 14
    .byte 0
    .byte 1
    .byte 0
    .byte 13
    .byte 0
    .byte 3
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 13
    .byte 0
    .byte 1
    .byte 0
    .byte 34
    .byte 0
    .byte 1
    .byte 0
    .byte 62
    .byte 0
    .byte 1
    .byte 0
    .byte 39
    .byte 0
    .byte 1
    .byte 0
    .byte 62
    .byte 0
    .byte 1
    .byte 0
    .byte 38
    .byte 0
    .byte 1
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 13
    .byte 0
    .byte 3
    .byte 0
    .byte 69
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 12
    .byte 0
    .byte 4
    .byte 0
    .byte 15
    .byte 0
    .byte 2
    .byte 0
    .byte 12
    .byte 0
    .byte 4
    .byte 0
    .byte 34
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 39
    .byte 0
    .byte 1
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 62
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_0298:
    MoveAction_010
    EndMovement

    .balign 4, 0
_02A0:
    MoveAction_00F
    MoveAction_022
    EndMovement

    .balign 4, 0
_02AC:
    MoveAction_00E
    MoveAction_021
    EndMovement

    .balign 4, 0
_02B8:
    MoveAction_026
    EndMovement

    .balign 4, 0
_02C0:
    MoveAction_04B
    MoveAction_010 3
    MoveAction_03F 3
    MoveAction_026
    MoveAction_03F
    MoveAction_024
    MoveAction_03F 2
    MoveAction_011 3
    MoveAction_026
    EndMovement

    .balign 4, 0
_02E8:
    MoveAction_04B
    EndMovement

    .balign 4, 0
_02F0:
    MoveAction_026 4
    EndMovement

    .balign 4, 0
_02F8:
    MoveAction_011 2
    EndMovement

    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 39
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 63
    .byte 0
    .byte 7
    .byte 0
    .byte 15
    .byte 0
    .byte 1
    .byte 0
    .byte 34
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
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
    .byte 14
    .byte 0
    .byte 1
    .byte 0
    .byte 13
    .byte 0
    .byte 1
    .byte 0
    .byte 34
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 37
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 13
    .byte 0
    .byte 1
    .byte 0
    .byte 14
    .byte 0
    .byte 1
    .byte 0
    .byte 13
    .byte 0
    .byte 3
    .byte 0
    .byte 14
    .byte 0
    .byte 1
    .byte 0
    .byte 13
    .byte 0
    .byte 6
    .byte 0
    .byte 69
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_0368:
    MoveAction_00C
    EndMovement

    .balign 4, 0
_0370:
    MoveAction_023
    EndMovement

    .balign 4, 0
_0378:
    MoveAction_021
    EndMovement

    .balign 4, 0
_0380:
    MoveAction_023
    EndMovement

    .balign 4, 0
_0388:
    MoveAction_03F 4
    MoveAction_020
    MoveAction_03F 9
    MoveAction_023
    EndMovement

    .balign 4, 0
_039C:
    MoveAction_021
    EndMovement

    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 63
    .byte 0
    .byte 5
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 63
    .byte 0
    .byte 2
    .byte 0
    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 32
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 63
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
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 32
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 33
    .byte 0
    .byte 1
    .byte 0
    .byte 13
    .byte 0
    .byte 4
    .byte 0
    .byte 14
    .byte 0
    .byte 1
    .byte 0
    .byte 13
    .byte 0
    .byte 6
    .byte 0
    .byte 69
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 16
    .byte 0
    .byte 6
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 18
    .byte 0
    .byte 7
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

_0418:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 184
    GoToIf 1, _043D
    SetFlag 184
    ScrCmd_0CD 0
    ScrCmd_02C 8
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_043D:
    ScrCmd_0CD 0
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_044B:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_14D 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _046A
    GoTo _0476

_046A:
    ScrCmd_0CD 0
    ScrCmd_02C 10
    GoTo _0482

_0476:
    ScrCmd_0CD 0
    ScrCmd_02C 11
    GoTo _0482

_0482:
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_048A:
    End

_048C:
    End

    .byte 0
    .byte 0
