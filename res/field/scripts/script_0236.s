    .include "macros/scrcmd.inc"

    .data

    .long _0022-.-4
    .long _0107-.-4
    .long _0154-.-4
    .long _0508-.-4
    .long _0249-.-4
    .long _0210-.-4
    .long _0223-.-4
    .long _0236-.-4
    .short 0xFD13

_0022:
    SetFlag 0x9C7
    Call _00C7
    Call _0062
    ScrCmd_14D 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _0052
    CompareVarToValue 0x4000, 1
    GoToIf 1, _005A
    End

_0052:
    SetVar 0x4020, 97
    End

_005A:
    SetVar 0x4020, 0
    End

_0062:
    ScrCmd_166 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _00C5
    ScrCmd_22D 2, 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _00C5
    ScrCmd_07E 0x1C7, 1, 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _00C5
    ScrCmd_28B 2, 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _00C5
    CheckFlag 0x11E
    GoToIf 1, _00C5
    SetVar 0x4118, 1
    GoTo _00C5
    End

_00C5:
    Return

_00C7:
    ScrCmd_1F9 0x4098
    CompareVarToValue 0x4098, 0
    GoToIf 1, _0101
    CompareVarToValue 0x4098, 1
    GoToIf 1, _0101
    CompareVarToValue 0x4098, 2
    GoToIf 1, _0101
    CompareVarToValue 0x4098, 3
    GoToIf 1, _0101
    Return

_0101:
    SetFlag 0x1C5
    Return

_0107:
    End

_0109:
    ScrCmd_246 0x800C
    SetVar 0x8004, 0x8005
    CompareVarToValue 0x800C, 10
    GoToIf 1, _0133
    CompareVarToValue 0x800C, 12
    GoToIf 1, _0133
    SetVar 0x8004, 0x8006
_0133:
    Return

    .byte 77
    .byte 1
    .byte 12
    .byte 128
    .byte 41
    .byte 0
    .byte 4
    .byte 128
    .byte 5
    .byte 128
    .byte 17
    .byte 0
    .byte 12
    .byte 128
    .byte 1
    .byte 0
    .byte 28
    .byte 0
    .byte 1
    .byte 6
    .byte 0
    .byte 0
    .byte 0
    .byte 41
    .byte 0
    .byte 4
    .byte 128
    .byte 6
    .byte 128
    .byte 27
    .byte 0

_0154:
    ScrCmd_060
    ApplyMovement 0, _01E4
    ApplyMovement 3, _01F4
    WaitMovement
    ScrCmd_0EE 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 5, _01A6
    GoTo _0181
    End

_0181:
    ScrCmd_02C 1
    ScrCmd_034
    ApplyMovement 0xFF, _0204
    WaitMovement
    ApplyMovement 0, _01EC
    ApplyMovement 3, _01FC
    WaitMovement
    ScrCmd_061
    End

_01A6:
    Call _01CA
    CompareVarToValue 0x800C, 0
    GoToIf 1, _01DB
    SetVar 0x4098, 1
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_01CA:
    ScrCmd_02C 0
    ScrCmd_034
    ScrCmd_0E5 0x209, 0x20F
    ScrCmd_0EC 0x800C
    Return

_01DB:
    ScrCmd_0EB
    ScrCmd_061
    End

    .balign 4, 0
_01E4:
    MoveAction_023
    EndMovement

    .balign 4, 0
_01EC:
    MoveAction_021
    EndMovement

    .balign 4, 0
_01F4:
    MoveAction_022
    EndMovement

    .balign 4, 0
_01FC:
    MoveAction_021
    EndMovement

    .balign 4, 0
_0204:
    MoveAction_03E 5
    MoveAction_00D
    EndMovement

_0210:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0223:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 44
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0236:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 43
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0249:
    ScrCmd_060
    Call _036F
    ScrCmd_02C 3
    ScrCmd_02C 4
    ScrCmd_034
    Call _02DC
    ScrCmd_0CE 0
    ScrCmd_02C 5
    ScrCmd_02C 7
    ScrCmd_02C 8
    ScrCmd_02C 9
    ScrCmd_034
    Call _0424
    ScrCmd_2A0 0x8004, 0x210, 0x197
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _02D0
    Call _0456
    ScrCmd_0CE 0
    ScrCmd_0CD 1
    ScrCmd_02C 10
    ScrCmd_04E 0x48E
    ScrCmd_04F
    ScrCmd_14E
    ScrCmd_02C 11
    ScrCmd_02C 12
    ScrCmd_034
    ScrCmd_18C 0xFF, 1
    ApplyMovement 5, _0500
    WaitMovement
    ScrCmd_065 5
    SetVar 0x4098, 2
    GoTo _0508
    End

_02D0:
    SetVar 0x4098, 1
    ScrCmd_0EB
    ScrCmd_061
    End

_02DC:
    ClearFlag 0x1C5
    ScrCmd_069 0x8004, 0x8005
    SetVar 0x8008, 0x8004
    CompareVarToValue 0x8008, 30
    GoToIf 1, _0315
    CompareVarToValue 0x8008, 31
    GoToIf 1, _0333
    CompareVarToValue 0x8008, 32
    GoToIf 1, _0351
    Return

_0315:
    ScrCmd_186 5, 31, 40
    ScrCmd_064 5
    ApplyMovement 5, _04F4
    WaitMovement
    ScrCmd_18C 5, 3
    Return

_0333:
    ScrCmd_186 5, 30, 40
    ScrCmd_064 5
    ApplyMovement 5, _04F4
    WaitMovement
    ScrCmd_18C 5, 2
    Return

_0351:
    ScrCmd_186 5, 31, 40
    ScrCmd_064 5
    ApplyMovement 5, _04F4
    WaitMovement
    ScrCmd_18C 5, 2
    Return

_036F:
    ScrCmd_069 0x8004, 0x8005
    SetVar 0x8008, 0x8004
    CompareVarToValue 0x8008, 30
    GoToIf 1, _03A4
    CompareVarToValue 0x8008, 31
    GoToIf 1, _03BE
    CompareVarToValue 0x8008, 32
    GoToIf 1, _03D8
    Return

_03A4:
    ScrCmd_18C 0xFF, 2
    ApplyMovement 4, _03F4
    ApplyMovement 2, _03FC
    WaitMovement
    Return

_03BE:
    ScrCmd_18C 0xFF, 3
    ApplyMovement 4, _0404
    ApplyMovement 2, _040C
    WaitMovement
    Return

_03D8:
    ScrCmd_18C 0xFF, 3
    ApplyMovement 4, _0414
    ApplyMovement 2, _041C
    WaitMovement
    Return

    .balign 4, 0
_03F4:
    MoveAction_023
    EndMovement

    .balign 4, 0
_03FC:
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0404:
    MoveAction_023
    EndMovement

    .balign 4, 0
_040C:
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0414:
    MoveAction_00F
    EndMovement

    .balign 4, 0
_041C:
    MoveAction_022
    EndMovement

_0424:
    ScrCmd_0DE 0x800C
    SetVar 0x8004, 0x26C
    CompareVarToValue 0x800C, 0x186
    GoToIf 1, _0454
    SetVar 0x8004, 0x26B
    CompareVarToValue 0x800C, 0x183
    GoToIf 1, _0454
    SetVar 0x8004, 0x25F
_0454:
    Return

_0456:
    ScrCmd_069 0x8004, 0x8005
    SetVar 0x8008, 0x8004
    CompareVarToValue 0x8008, 30
    GoToIf 1, _048B
    CompareVarToValue 0x8008, 31
    GoToIf 1, _0499
    CompareVarToValue 0x8008, 32
    GoToIf 1, _04A7
    Return

_048B:
    ScrCmd_18C 0xFF, 3
    ScrCmd_18C 5, 2
    Return

_0499:
    ScrCmd_18C 0xFF, 2
    ScrCmd_18C 5, 3
    Return

_04A7:
    ScrCmd_18C 0xFF, 2
    ScrCmd_18C 5, 3
    Return

    .byte 94
    .byte 0
    .byte 4
    .byte 0
    .byte 15
    .byte 0
    .byte 0
    .byte 0
    .byte 94
    .byte 0
    .byte 2
    .byte 0
    .byte 27
    .byte 0
    .byte 0
    .byte 0
    .byte 95
    .byte 0
    .byte 27
    .byte 0
    .byte 0
    .byte 0
    .byte 0
    .byte 3
    .byte 0
    .byte 1
    .byte 0
    .byte 71
    .byte 0
    .byte 1
    .byte 0
    .byte 10
    .byte 0
    .byte 1
    .byte 0
    .byte 72
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 2
    .byte 0
    .byte 1
    .byte 0
    .byte 71
    .byte 0
    .byte 1
    .byte 0
    .byte 11
    .byte 0
    .byte 1
    .byte 0
    .byte 72
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_04F4:
    MoveAction_03E 2
    MoveAction_010 8
    EndMovement

    .balign 4, 0
_0500:
    MoveAction_011 8
    EndMovement

_0508:
    ApplyMovement 0xFF, _05B8
    WaitMovement
    ScrCmd_069 0x8000, 0x8001
    ScrCmd_066 0x8000, 0x8001
    Call _05C0
    WaitMovement
    SetVar 0x8005, 13
    SetVar 0x8006, 68
    Call _0109
    ScrCmd_02D 0x8004
    ScrCmd_034
    ScrCmd_054 0, 30
    ScrCmd_20D 0, 0x800C
    ScrCmd_003 10, 0x800C
    ScrCmd_049 0x5C8
    ScrCmd_003 20, 0x800C
    ScrCmd_050 0x478
    ScrCmd_05D
    GoTo _0567
    End

_0567:
    ScrCmd_20D 1, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0567
    ScrCmd_2FB
    SetFlag 0x1C8
    SetFlag 0x1C9
    SetFlag 0x1CA
    SetVar 0x4098, 3
    SetFlag 0x981
    ClearFlag 0x1C7
    SetFlag 0x132
    SetVar 0x40C3, 1
    ScrCmd_31A 0x1E3
    ScrCmd_31A 0x1E4
    ScrCmd_067
    ScrCmd_0BE 221, 0, 30, 30, 0
    End

    .balign 4, 0
_05B8:
    MoveAction_020
    EndMovement

_05C0:
    ScrCmd_069 0x8004, 0x8005
    SetVar 0x8008, 0x8004
    CompareVarToValue 0x8008, 29
    GoToIf 1, _060F
    CompareVarToValue 0x8008, 30
    GoToIf 1, _0619
    CompareVarToValue 0x8008, 31
    GoToIf 1, _0623
    CompareVarToValue 0x8008, 32
    GoToIf 1, _062D
    CompareVarToValue 0x8008, 33
    GoToIf 1, _0637
    Return

_060F:
    ApplyMovement 241, _0644
    Return

_0619:
    ApplyMovement 241, _0654
    Return

_0623:
    ApplyMovement 241, _0664
    Return

_062D:
    ApplyMovement 241, _0670
    Return

_0637:
    ApplyMovement 241, _0680
    Return

    .balign 4, 0
_0644:
    MoveAction_03F
    MoveAction_00C 6
    MoveAction_00F 2
    EndMovement

    .balign 4, 0
_0654:
    MoveAction_03F
    MoveAction_00C 6
    MoveAction_00F
    EndMovement

    .balign 4, 0
_0664:
    MoveAction_03F
    MoveAction_00C 6
    EndMovement

    .balign 4, 0
_0670:
    MoveAction_03F
    MoveAction_00C 6
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0680:
    MoveAction_03F
    MoveAction_00C 6
    MoveAction_00E 2
    EndMovement
