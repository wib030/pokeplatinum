    .include "macros/scrcmd.inc"

    .data

    .long _001E-.-4
    .long _0071-.-4
    .long _0084-.-4
    .long _009B-.-4
    .long _00B2-.-4
    .long _00C7-.-4
    .long _04C4-.-4
    .short 0xFD13

_001E:
    CompareVarToValue 0x4087, 2
    CallIf 4, _005B
    ScrCmd_14D 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _004B
    CompareVarToValue 0x4000, 1
    GoToIf 1, _0053
    End

_004B:
    SetVar 0x4020, 97
    End

_0053:
    SetVar 0x4020, 0
    End

_005B:
    ScrCmd_186 3, 186, 0x331
    ScrCmd_189 3, 1
    ScrCmd_188 3, 15
    Return

_0071:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 10
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0084:
    ScrCmd_036 11, 1, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_009B:
    ScrCmd_036 12, 1, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_00B2:
    ScrCmd_037 3, 0
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03A 13, 0x800C
    ScrCmd_014 0x7D0
    End

_00C7:
    ScrCmd_060
    ApplyMovement 3, _03C4
    ApplyMovement 0xFF, _04A0
    WaitMovement
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8005, 0x339
    GoToIf 1, _0124
    CompareVarToValue 0x8005, 0x33A
    GoToIf 1, _0134
    CompareVarToValue 0x8005, 0x33B
    GoToIf 1, _0144
    CompareVarToValue 0x8005, 0x33C
    GoToIf 1, _0154
    CompareVarToValue 0x8005, 0x33D
    GoToIf 1, _0164
    End

_0124:
    ApplyMovement 3, _03D4
    WaitMovement
    GoTo _0174

_0134:
    ApplyMovement 3, _03E4
    WaitMovement
    GoTo _0174

_0144:
    ApplyMovement 3, _03F4
    WaitMovement
    GoTo _0174

_0154:
    ApplyMovement 3, _0400
    WaitMovement
    GoTo _0174

_0164:
    ApplyMovement 3, _0410
    WaitMovement
    GoTo _0174

_0174:
    CheckFlag 143
    GoToIf 0, _027C
    ScrCmd_14D 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _019F
    CompareVarToValue 0x800C, 1
    GoToIf 1, _01B2
    End

_019F:
    ScrCmd_014 0x7F8
    ScrCmd_0CF 0
    ScrCmd_0CD 1
    ScrCmd_02C 2
    GoTo _01C5

_01B2:
    ScrCmd_014 0x7F8
    ScrCmd_0CF 0
    ScrCmd_0CD 1
    ScrCmd_02C 7
    GoTo _01C5

_01C5:
    ScrCmd_034
    ApplyMovement 3, _0488
    ApplyMovement 0xFF, _04BC
    WaitMovement
    ScrCmd_126
    ApplyMovement 3, _0490
    WaitMovement
    ScrCmd_14D 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0205
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0211
    End

_0205:
    ScrCmd_0CD 0
    ScrCmd_02C 3
    GoTo _021D

_0211:
    ScrCmd_0CD 0
    ScrCmd_02C 8
    GoTo _021D

_021D:
    SetVar 0x8004, 4
    SetVar 0x8005, 5
    ScrCmd_014 0x7FC
    ScrCmd_14D 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _024D
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0259
    End

_024D:
    ScrCmd_0CD 0
    ScrCmd_02C 4
    GoTo _0262

_0259:
    ScrCmd_02C 9
    GoTo _0262

_0262:
    ScrCmd_034
    ApplyMovement 3, _0498
    WaitMovement
    ScrCmd_065 3
    SetVar 0x4087, 1
    ScrCmd_061
    End

_027C:
    ScrCmd_14D 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _029C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _02C8
    End

_029C:
    ScrCmd_0CF 0
    ScrCmd_0CD 1
    CheckFlag 0x115
    CallIf 0, _02BE
    CheckFlag 0x115
    CallIf 1, _02C3
    GoTo _02F4

_02BE:
    ScrCmd_02C 0
    Return

_02C3:
    ScrCmd_02C 1
    Return

_02C8:
    ScrCmd_0CF 0
    ScrCmd_0CD 1
    CheckFlag 0x115
    CallIf 0, _02EA
    CheckFlag 0x115
    CallIf 1, _02EF
    GoTo _02F4

_02EA:
    ScrCmd_02C 5
    Return

_02EF:
    ScrCmd_02C 6
    Return

_02F4:
    ScrCmd_034
    CompareVarToValue 0x8005, 0x339
    GoToIf 1, _0339
    CompareVarToValue 0x8005, 0x33A
    GoToIf 1, _0351
    CompareVarToValue 0x8005, 0x33B
    GoToIf 1, _0369
    CompareVarToValue 0x8005, 0x33C
    GoToIf 1, _0381
    CompareVarToValue 0x8005, 0x33D
    GoToIf 1, _0399
    End

_0339:
    ApplyMovement 3, _0420
    ApplyMovement 0xFF, _04AC
    WaitMovement
    GoTo _03B1

_0351:
    ApplyMovement 3, _0434
    ApplyMovement 0xFF, _04AC
    WaitMovement
    GoTo _03B1

_0369:
    ApplyMovement 3, _0448
    ApplyMovement 0xFF, _04AC
    WaitMovement
    GoTo _03B1

_0381:
    ApplyMovement 3, _0458
    ApplyMovement 0xFF, _04AC
    WaitMovement
    GoTo _03B1

_0399:
    ApplyMovement 3, _046C
    ApplyMovement 0xFF, _04AC
    WaitMovement
    GoTo _03B1

_03B1:
    ApplyMovement 3, _0480
    WaitMovement
    SetFlag 0x115
    ScrCmd_061
    End

    .balign 4, 0
_03C4:
    MoveAction_023
    MoveAction_04B
    MoveAction_041
    EndMovement

    .balign 4, 0
_03D4:
    MoveAction_00F 4
    MoveAction_00C 2
    MoveAction_00F
    EndMovement

    .balign 4, 0
_03E4:
    MoveAction_00F 4
    MoveAction_00C
    MoveAction_00F
    EndMovement

    .balign 4, 0
_03F4:
    MoveAction_00F 4
    MoveAction_00F
    EndMovement

    .balign 4, 0
_0400:
    MoveAction_00F 4
    MoveAction_00D
    MoveAction_00F
    EndMovement

    .balign 4, 0
_0410:
    MoveAction_00F 4
    MoveAction_00D 2
    MoveAction_00F
    EndMovement

    .balign 4, 0
_0420:
    MoveAction_00F
    MoveAction_00E 4
    MoveAction_00E 2
    MoveAction_00D 2
    EndMovement

    .balign 4, 0
_0434:
    MoveAction_00F
    MoveAction_00E 4
    MoveAction_00E 2
    MoveAction_00D
    EndMovement

    .balign 4, 0
_0448:
    MoveAction_00F
    MoveAction_00E 4
    MoveAction_00E 2
    EndMovement

    .balign 4, 0
_0458:
    MoveAction_00F
    MoveAction_00E 4
    MoveAction_00E 2
    MoveAction_00C
    EndMovement

    .balign 4, 0
_046C:
    MoveAction_00F
    MoveAction_00E 4
    MoveAction_00E 2
    MoveAction_00C 2
    EndMovement

    .balign 4, 0
_0480:
    MoveAction_023
    EndMovement

    .balign 4, 0
_0488:
    MoveAction_00E 2
    EndMovement

    .balign 4, 0
_0490:
    MoveAction_023
    EndMovement

    .balign 4, 0
_0498:
    MoveAction_00E 12
    EndMovement

    .balign 4, 0
_04A0:
    MoveAction_03F
    MoveAction_002
    EndMovement

    .balign 4, 0
_04AC:
    MoveAction_047
    MoveAction_00F
    MoveAction_048
    EndMovement

    .balign 4, 0
_04BC:
    MoveAction_00E 2
    EndMovement

_04C4:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_0CD 0
    ScrCmd_1BD 0x8002
    ScrCmd_14D 0x8000
    CompareVarToValue 0x8000, 0
    CallIf 1, _0653
    CompareVarToValue 0x8000, 1
    CallIf 1, _0658
    ScrCmd_034
    ScrCmd_050 0x47E
    ScrCmd_308 189, 0x332
    ApplyMovement 241, _06F8
    WaitMovement
    ScrCmd_309
    ApplyMovement 3, _0690
    ApplyMovement 0xFF, _06D0
    WaitMovement
    ScrCmd_003 15, 0x800C
    CompareVarToValue 0x8000, 0
    CallIf 1, _065D
    CompareVarToValue 0x8000, 1
    CallIf 1, _0662
    ScrCmd_034
    ScrCmd_003 15, 0x800C
    ScrCmd_052
    CompareVarToValue 0x8002, 0
    CallIf 1, _05D1
    CompareVarToValue 0x8002, 1
    CallIf 1, _05E5
    CompareVarToValue 0x8002, 2
    CallIf 1, _05F9
    CompareVarToValue 0x8002, 3
    CallIf 1, _0605
    CompareVarToValue 0x8000, 0
    CallIf 1, _0667
    CompareVarToValue 0x8000, 1
    CallIf 1, _066C
    ScrCmd_034
    CompareVarToValue 0x8002, 0
    CallIf 1, _0611
    CompareVarToValue 0x8002, 1
    CallIf 1, _061D
    CompareVarToValue 0x8002, 2
    CallIf 1, _0629
    CompareVarToValue 0x8002, 3
    CallIf 1, _0635
    GoTo _0641
    End

_05D1:
    ApplyMovement 3, _0698
    ApplyMovement 0xFF, _06D8
    WaitMovement
    Return

_05E5:
    ApplyMovement 3, _06A0
    ApplyMovement 0xFF, _06E0
    WaitMovement
    Return

_05F9:
    ApplyMovement 0xFF, _06E8
    WaitMovement
    Return

_0605:
    ApplyMovement 3, _06B0
    WaitMovement
    Return

_0611:
    ApplyMovement 3, _0674
    WaitMovement
    Return

_061D:
    ApplyMovement 3, _0684
    WaitMovement
    Return

_0629:
    ApplyMovement 3, _0684
    WaitMovement
    Return

_0635:
    ApplyMovement 3, _0684
    WaitMovement
    Return

_0641:
    ScrCmd_065 3
    ClearFlag 0x177
    SetVar 0x4087, 3
    ScrCmd_061
    End

_0653:
    ScrCmd_02C 14
    Return

_0658:
    ScrCmd_02C 17
    Return

_065D:
    ScrCmd_02C 15
    Return

_0662:
    ScrCmd_02C 18
    Return

_0667:
    ScrCmd_02C 16
    Return

_066C:
    ScrCmd_02C 19
    Return

    .balign 4, 0
_0674:
    MoveAction_00E
    MoveAction_00D 3
    MoveAction_00E 10
    EndMovement

    .balign 4, 0
_0684:
    MoveAction_00D 3
    MoveAction_00E 10
    EndMovement

    .balign 4, 0
_0690:
    MoveAction_003
    EndMovement

    .balign 4, 0
_0698:
    MoveAction_021
    EndMovement

    .balign 4, 0
_06A0:
    MoveAction_020
    EndMovement

    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_06B0:
    MoveAction_022
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

    .balign 4, 0
_06D0:
    MoveAction_003
    EndMovement

    .balign 4, 0
_06D8:
    MoveAction_020
    EndMovement

    .balign 4, 0
_06E0:
    MoveAction_021
    EndMovement

    .balign 4, 0
_06E8:
    MoveAction_022
    EndMovement

    .byte 35
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_06F8:
    MoveAction_00F
    EndMovement
