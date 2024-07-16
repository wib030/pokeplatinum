    .include "macros/scrcmd.inc"

    .data

    .long _0593-.-4
    .long _0680-.-4
    .long _0693-.-4
    .long _06A6-.-4
    .long _06B9-.-4
    .long _06CC-.-4
    .long _06DF-.-4
    .long _0099-.-4
    .long _0530-.-4
    .long _06F2-.-4
    .long _0709-.-4
    .long _071E-.-4
    .long _0735-.-4
    .long _074C-.-4
    .long _0763-.-4
    .long _077A-.-4
    .long _0791-.-4
    .long _0850-.-4
    .long _004E-.-4
    .short 0xFD13

_004E:
    CompareVarToValue 0x40A6, 2
    CallIf 1, _0077
    CompareVarToValue 0x40A6, 3
    CallIf 1, _0077
    CompareVarToValue 0x407E, 0
    GoToIf 1, _0083
    End

_0077:
    SetFlag 0x199
    SetVar 0x40A6, 4
    Return

_0083:
    ScrCmd_186 18, 0x349, 0x316
    ScrCmd_188 18, 16
    ScrCmd_189 18, 2
    End

_0099:
    ScrCmd_060
    ApplyMovement 9, _04E8
    WaitMovement
    ClearFlag 0x255
    ScrCmd_064 20
    ScrCmd_062 20
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8004, 0x355
    GoToIf 1, _00FA
    CompareVarToValue 0x8004, 0x356
    GoToIf 1, _0114
    CompareVarToValue 0x8004, 0x357
    GoToIf 1, _012E
    CompareVarToValue 0x8004, 0x358
    GoToIf 1, _0148
    CompareVarToValue 0x8004, 0x359
    GoToIf 1, _0162
    End

_00FA:
    ApplyMovement 20, _0454
    ApplyMovement 0xFF, _04F4
    WaitMovement
    GoTo _017C
    End

_0114:
    ApplyMovement 20, _0464
    ApplyMovement 0xFF, _04F4
    WaitMovement
    GoTo _017C
    End

_012E:
    ApplyMovement 20, _0474
    ApplyMovement 0xFF, _0504
    WaitMovement
    GoTo _017C
    End

_0148:
    ApplyMovement 20, _0480
    ApplyMovement 0xFF, _050C
    WaitMovement
    GoTo _017C
    End

_0162:
    ApplyMovement 20, _0490
    ApplyMovement 0xFF, _050C
    WaitMovement
    GoTo _017C
    End

_017C:
    ScrCmd_0CE 0
    ScrCmd_0CD 1
    ScrCmd_02C 10
    ApplyMovement 20, _04A0
    WaitMovement
    ScrCmd_02C 11
    CompareVarToValue 0x8004, 0x355
    CallIf 1, _02BE
    CompareVarToValue 0x8004, 0x356
    CallIf 1, _02CA
    CompareVarToValue 0x8004, 0x357
    CallIf 1, _02D6
    CompareVarToValue 0x8004, 0x358
    CallIf 1, _02E2
    CompareVarToValue 0x8004, 0x359
    CallIf 1, _02EE
    ScrCmd_02C 12
    ScrCmd_034
    CompareVarToValue 0x8004, 0x355
    CallIf 1, _02FA
    CompareVarToValue 0x8004, 0x356
    CallIf 1, _030E
    CompareVarToValue 0x8004, 0x357
    CallIf 1, _0322
    CompareVarToValue 0x8004, 0x358
    CallIf 1, _032E
    CompareVarToValue 0x8004, 0x359
    CallIf 1, _0342
    ScrCmd_065 20
    CompareVarToValue 0x8004, 0x355
    CallIf 1, _0356
    CompareVarToValue 0x8004, 0x356
    CallIf 1, _0362
    CompareVarToValue 0x8004, 0x357
    CallIf 1, _036E
    CompareVarToValue 0x8004, 0x358
    CallIf 1, _037A
    CompareVarToValue 0x8004, 0x359
    CallIf 1, _0386
    ApplyMovement 0xFF, _0528
    WaitMovement
    ScrCmd_02C 13
    Call _0577
    ScrCmd_034
    CompareVarToValue 0x8004, 0x355
    CallIf 1, _0392
    CompareVarToValue 0x8004, 0x356
    CallIf 1, _039E
    CompareVarToValue 0x8004, 0x357
    CallIf 1, _03AA
    CompareVarToValue 0x8004, 0x358
    CallIf 1, _03B6
    CompareVarToValue 0x8004, 0x359
    CallIf 1, _03C2
    SetVar 0x407E, 3
    ScrCmd_061
    End

_02BE:
    ApplyMovement 20, _04A8
    WaitMovement
    Return

_02CA:
    ApplyMovement 20, _04A8
    WaitMovement
    Return

_02D6:
    ApplyMovement 20, _04B0
    WaitMovement
    Return

_02E2:
    ApplyMovement 20, _04B8
    WaitMovement
    Return

_02EE:
    ApplyMovement 20, _04B8
    WaitMovement
    Return

_02FA:
    ApplyMovement 20, _04C0
    ApplyMovement 0xFF, _051C
    WaitMovement
    Return

_030E:
    ApplyMovement 20, _04C8
    ApplyMovement 0xFF, _051C
    WaitMovement
    Return

_0322:
    ApplyMovement 20, _04D0
    WaitMovement
    Return

_032E:
    ApplyMovement 20, _04D8
    ApplyMovement 0xFF, _051C
    WaitMovement
    Return

_0342:
    ApplyMovement 20, _04E0
    ApplyMovement 0xFF, _051C
    WaitMovement
    Return

_0356:
    ApplyMovement 9, _03D0
    WaitMovement
    Return

_0362:
    ApplyMovement 9, _03DC
    WaitMovement
    Return

_036E:
    ApplyMovement 9, _03E8
    WaitMovement
    Return

_037A:
    ApplyMovement 9, _03F4
    WaitMovement
    Return

_0386:
    ApplyMovement 9, _0400
    WaitMovement
    Return

_0392:
    ApplyMovement 9, _0408
    WaitMovement
    Return

_039E:
    ApplyMovement 9, _0418
    WaitMovement
    Return

_03AA:
    ApplyMovement 9, _0428
    WaitMovement
    Return

_03B6:
    ApplyMovement 9, _0438
    WaitMovement
    Return

_03C2:
    ApplyMovement 9, _0448
    WaitMovement
    Return

    .balign 4, 0
_03D0:
    MoveAction_00E 4
    MoveAction_00D
    EndMovement

    .balign 4, 0
_03DC:
    MoveAction_00E 3
    MoveAction_00D
    EndMovement

    .balign 4, 0
_03E8:
    MoveAction_00E 2
    MoveAction_00D
    EndMovement

    .balign 4, 0
_03F4:
    MoveAction_00E
    MoveAction_00D
    EndMovement

    .balign 4, 0
_0400:
    MoveAction_00D
    EndMovement

    .balign 4, 0
_0408:
    MoveAction_00C
    MoveAction_00F 4
    MoveAction_021
    EndMovement

    .balign 4, 0
_0418:
    MoveAction_00C
    MoveAction_00F 3
    MoveAction_021
    EndMovement

    .balign 4, 0
_0428:
    MoveAction_00C
    MoveAction_00F 2
    MoveAction_021
    EndMovement

    .balign 4, 0
_0438:
    MoveAction_00C
    MoveAction_00F
    MoveAction_021
    EndMovement

    .balign 4, 0
_0448:
    MoveAction_00C
    MoveAction_021
    EndMovement

    .balign 4, 0
_0454:
    MoveAction_03F
    MoveAction_010 4
    MoveAction_012
    EndMovement

    .balign 4, 0
_0464:
    MoveAction_03F
    MoveAction_010 4
    MoveAction_026
    EndMovement

    .balign 4, 0
_0474:
    MoveAction_03F
    MoveAction_010 3
    EndMovement

    .balign 4, 0
_0480:
    MoveAction_03F
    MoveAction_010 4
    MoveAction_027
    EndMovement

    .balign 4, 0
_0490:
    MoveAction_03F
    MoveAction_010 4
    MoveAction_013
    EndMovement

    .balign 4, 0
_04A0:
    MoveAction_025
    EndMovement

    .balign 4, 0
_04A8:
    MoveAction_026
    EndMovement

    .balign 4, 0
_04B0:
    MoveAction_024
    EndMovement

    .balign 4, 0
_04B8:
    MoveAction_027
    EndMovement

    .balign 4, 0
_04C0:
    MoveAction_011 8
    EndMovement

    .balign 4, 0
_04C8:
    MoveAction_011 8
    EndMovement

    .balign 4, 0
_04D0:
    MoveAction_011 8
    EndMovement

    .balign 4, 0
_04D8:
    MoveAction_011 8
    EndMovement

    .balign 4, 0
_04E0:
    MoveAction_011 8
    EndMovement

    .balign 4, 0
_04E8:
    MoveAction_021
    MoveAction_04B
    EndMovement

    .balign 4, 0
_04F4:
    MoveAction_021
    MoveAction_03F
    MoveAction_023
    EndMovement

    .balign 4, 0
_0504:
    MoveAction_021
    EndMovement

    .balign 4, 0
_050C:
    MoveAction_021
    MoveAction_03F 2
    MoveAction_022
    EndMovement

    .balign 4, 0
_051C:
    MoveAction_03F
    MoveAction_021
    EndMovement

    .balign 4, 0
_0528:
    MoveAction_020
    EndMovement

_0530:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 154
    GoToIf 1, _056C
    ScrCmd_02C 8
    ScrCmd_15B 7, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0561
    GoTo _056C
    End

_0561:
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_056C:
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0577:
    ScrCmd_02C 6
    SetVar 0x8004, 0x1AA
    SetVar 0x8005, 1
    ScrCmd_014 0x7FC
    SetFlag 154
    ScrCmd_02C 7
    Return

_0593:
    ScrCmd_060
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8005, 0x316
    GoToIf 1, _05B7
    CompareVarToValue 0x8005, 0x317
    GoToIf 1, _05C7
    End

_05B7:
    ApplyMovement 18, _063C
    WaitMovement
    GoTo _05D7

_05C7:
    ApplyMovement 18, _0650
    WaitMovement
    GoTo _05D7

_05D7:
    ScrCmd_02C 0
    ScrCmd_034
    ApplyMovement 18, _0668
    WaitMovement
    ScrCmd_003 15, 0x800C
    ApplyMovement 18, _0670
    WaitMovement
    ScrCmd_02C 1
    ScrCmd_034
    ScrCmd_003 15, 0x800C
    ApplyMovement 18, _0678
    WaitMovement
    ScrCmd_065 18
    ScrCmd_003 1, 0x800C
    ScrCmd_186 18, 0x34D, 0x2EC
    ScrCmd_188 18, 15
    ScrCmd_189 18, 1
    ClearFlag 0x239
    ScrCmd_064 18
    SetVar 0x407E, 1
    ScrCmd_061
    End

    .balign 4, 0
_063C:
    MoveAction_00E 3
    MoveAction_022
    MoveAction_04B
    MoveAction_00E 5
    EndMovement

    .balign 4, 0
_0650:
    MoveAction_00D
    MoveAction_00E 3
    MoveAction_022
    MoveAction_04B
    MoveAction_00E 5
    EndMovement

    .balign 4, 0
_0668:
    MoveAction_021
    EndMovement

    .balign 4, 0
_0670:
    MoveAction_022
    EndMovement

    .balign 4, 0
_0678:
    MoveAction_00F 9
    EndMovement

_0680:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 14
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0693:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 20
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06A6:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 15
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06B9:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 17
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06CC:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 18
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06DF:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 19
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06F2:
    ScrCmd_036 21, 0, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_0709:
    ScrCmd_037 3, 0
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03A 22, 0x800C
    ScrCmd_014 0x7D0
    End

_071E:
    ScrCmd_036 23, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_0735:
    ScrCmd_036 24, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_074C:
    ScrCmd_036 25, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_0763:
    ScrCmd_036 26, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_077A:
    ScrCmd_036 27, 2, 0, 0x800C
    ScrCmd_038 3
    ScrCmd_039
    ScrCmd_03B 0x800C
    ScrCmd_014 0x7D0
    End

_0791:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_014 0x7E3
    ScrCmd_035
    ScrCmd_234 0x800C
    SetVar 0x8008, 0x800C
    CompareVarToValue 0x8008, 0
    GoToIf 1, _0806
    CompareVarToValue 0x8008, 1
    GoToIf 1, _0810
    CompareVarToValue 0x8008, 2
    GoToIf 1, _081A
    CompareVarToValue 0x8008, 3
    GoToIf 1, _0824
    CompareVarToValue 0x8008, 4
    GoToIf 1, _082E
    CompareVarToValue 0x8008, 5
    GoToIf 1, _0838
    CompareVarToValue 0x8008, 6
    GoToIf 1, _0842
    End

_0806:
    ScrCmd_14A 0
    GoTo _084C

_0810:
    ScrCmd_14A 1
    GoTo _084C

_081A:
    ScrCmd_14A 2
    GoTo _084C

_0824:
    ScrCmd_14A 3
    GoTo _084C

_082E:
    ScrCmd_14A 4
    GoTo _084C

_0838:
    ScrCmd_14A 5
    GoTo _084C

_0842:
    ScrCmd_14A 6
    GoTo _084C

_084C:
    ScrCmd_061
    End

_0850:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 0x162
    GoToIf 1, _086E
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_086E:
    ScrCmd_02C 3
    ScrCmd_034
    ScrCmd_003 15, 0x800C
    ScrCmd_1BD 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _08A6
    CompareVarToValue 0x800C, 2
    GoToIf 1, _08BE
    CompareVarToValue 0x800C, 3
    GoToIf 1, _08D6
    End

_08A6:
    ApplyMovement 18, _08F8
    ApplyMovement 0xFF, _0914
    WaitMovement
    GoTo _08EE

_08BE:
    ApplyMovement 18, _0908
    ApplyMovement 0xFF, _0920
    WaitMovement
    GoTo _08EE

_08D6:
    ApplyMovement 18, _0908
    ApplyMovement 0xFF, _0914
    WaitMovement
    GoTo _08EE

_08EE:
    ScrCmd_065 18
    ScrCmd_061
    End

    .balign 4, 0
_08F8:
    MoveAction_00F
    MoveAction_00D 2
    MoveAction_00F 8
    EndMovement

    .balign 4, 0
_0908:
    MoveAction_00D 2
    MoveAction_00F 9
    EndMovement

    .balign 4, 0
_0914:
    MoveAction_03F
    MoveAction_003
    EndMovement

    .balign 4, 0
_0920:
    MoveAction_03F
    MoveAction_021
    MoveAction_023
    EndMovement
