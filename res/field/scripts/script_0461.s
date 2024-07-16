    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _0010-.-4
    .short 0xFD13

_000A:
    SetFlag 0x9ED
    End

_0010:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_247 0x8002
    ScrCmd_0D0 0, 0x8002
    ScrCmd_0CD 1
    ScrCmd_0D6 2, 0x8002
    ScrCmd_23A 0x8000, 0x8001, 0x8002
    CheckFlag 249
    GoToIf 1, _004B
    SetFlag 249
    ScrCmd_02C 0
    ScrCmd_034
    GoTo _0056

_004B:
    ScrCmd_02C 1
    ScrCmd_034
    GoTo _0056

_0056:
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    ScrCmd_003 40, 0x800C
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    Call _008A
    ScrCmd_003 20, 0x800C
    ScrCmd_1E5 116
    GoTo _0150

_008A:
    ScrCmd_069 0x8005, 0x8006
    CompareVarToValue 0x8005, 4
    GoToIf 1, _00BD
    CompareVarToValue 0x8005, 6
    GoToIf 1, _00C9
    CompareVarToValue 0x8006, 5
    GoToIf 1, _00D5
    GoTo _00E1

_00BD:
    ApplyMovement 0, _0698
    WaitMovement
    Return

_00C9:
    ApplyMovement 0, _06A8
    WaitMovement
    Return

_00D5:
    ApplyMovement 0, _06B8
    WaitMovement
    Return

_00E1:
    ApplyMovement 0, _06C8
    WaitMovement
    Return

_00ED:
    ScrCmd_069 0x8005, 0x8006
    CompareVarToValue 0x8005, 4
    GoToIf 1, _0120
    CompareVarToValue 0x8005, 6
    GoToIf 1, _012C
    CompareVarToValue 0x8006, 5
    GoToIf 1, _0138
    GoTo _0144

_0120:
    ApplyMovement 0, _06D8
    WaitMovement
    Return

_012C:
    ApplyMovement 0, _06E0
    WaitMovement
    Return

_0138:
    ApplyMovement 0, _06E8
    WaitMovement
    Return

_0144:
    ApplyMovement 0, _06F0
    WaitMovement
    Return

_0150:
    CompareVarToValue 0x8001, 1
    GoToIf 1, _018A
    CompareVarToValue 0x8001, 2
    GoToIf 1, _019D
    CompareVarToValue 0x8001, 3
    GoToIf 1, _01B0
    CompareVarToValue 0x8001, 4
    GoToIf 1, _01C3
    GoTo _0574

_018A:
    CompareVarToValue 0x8000, 1
    GoToIf 1, _01D6
    GoTo _0216

_019D:
    CompareVarToValue 0x8000, 1
    GoToIf 1, _0256
    GoTo _0296

_01B0:
    CompareVarToValue 0x8000, 1
    GoToIf 1, _02D6
    GoTo _0316

_01C3:
    CompareVarToValue 0x8000, 1
    GoToIf 1, _0356
    GoTo _0396

_01D6:
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 4, _03FA
    CompareVarToValue 0x800C, 200
    GoToIf 4, _03F1
    CompareVarToValue 0x800C, 150
    GoToIf 4, _03E8
    CompareVarToValue 0x800C, 70
    GoToIf 4, _03DF
    GoTo _03D6

_0216:
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 4, _0427
    CompareVarToValue 0x800C, 200
    GoToIf 4, _041E
    CompareVarToValue 0x800C, 150
    GoToIf 4, _0415
    CompareVarToValue 0x800C, 70
    GoToIf 4, _040C
    GoTo _0403

_0256:
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 4, _0454
    CompareVarToValue 0x800C, 200
    GoToIf 4, _044B
    CompareVarToValue 0x800C, 150
    GoToIf 4, _0442
    CompareVarToValue 0x800C, 70
    GoToIf 4, _0439
    GoTo _0430

_0296:
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 4, _0481
    CompareVarToValue 0x800C, 200
    GoToIf 4, _0478
    CompareVarToValue 0x800C, 150
    GoToIf 4, _046F
    CompareVarToValue 0x800C, 70
    GoToIf 4, _0466
    GoTo _045D

_02D6:
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 4, _04AE
    CompareVarToValue 0x800C, 200
    GoToIf 4, _04A5
    CompareVarToValue 0x800C, 150
    GoToIf 4, _049C
    CompareVarToValue 0x800C, 70
    GoToIf 4, _0493
    GoTo _048A

_0316:
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 4, _04DB
    CompareVarToValue 0x800C, 200
    GoToIf 4, _04D2
    CompareVarToValue 0x800C, 150
    GoToIf 4, _04C9
    CompareVarToValue 0x800C, 70
    GoToIf 4, _04C0
    GoTo _04B7

_0356:
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 4, _0508
    CompareVarToValue 0x800C, 200
    GoToIf 4, _04FF
    CompareVarToValue 0x800C, 150
    GoToIf 4, _04F6
    CompareVarToValue 0x800C, 70
    GoToIf 4, _04ED
    GoTo _04E4

_0396:
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 4, _0535
    CompareVarToValue 0x800C, 200
    GoToIf 4, _052C
    CompareVarToValue 0x800C, 150
    GoToIf 4, _0523
    CompareVarToValue 0x800C, 70
    GoToIf 4, _051A
    GoTo _0511

_03D6:
    ScrCmd_02C 10
    GoTo _05A5

_03DF:
    ScrCmd_02C 11
    GoTo _05A5

_03E8:
    ScrCmd_02C 12
    GoTo _05A5

_03F1:
    ScrCmd_02C 13
    GoTo _05A5

_03FA:
    ScrCmd_02C 14
    GoTo _053E

_0403:
    ScrCmd_02C 15
    GoTo _05C4

_040C:
    ScrCmd_02C 16
    GoTo _05C4

_0415:
    ScrCmd_02C 17
    GoTo _05C4

_041E:
    ScrCmd_02C 18
    GoTo _05C4

_0427:
    ScrCmd_02C 19
    GoTo _0559

_0430:
    ScrCmd_02C 20
    GoTo _05A5

_0439:
    ScrCmd_02C 21
    GoTo _05A5

_0442:
    ScrCmd_02C 22
    GoTo _05A5

_044B:
    ScrCmd_02C 23
    GoTo _05A5

_0454:
    ScrCmd_02C 24
    GoTo _053E

_045D:
    ScrCmd_02C 25
    GoTo _05C4

_0466:
    ScrCmd_02C 26
    GoTo _05C4

_046F:
    ScrCmd_02C 27
    GoTo _05C4

_0478:
    ScrCmd_02C 28
    GoTo _05C4

_0481:
    ScrCmd_02C 29
    GoTo _0559

_048A:
    ScrCmd_02C 30
    GoTo _05A5

_0493:
    ScrCmd_02C 31
    GoTo _05A5

_049C:
    ScrCmd_02C 32
    GoTo _05A5

_04A5:
    ScrCmd_02C 33
    GoTo _05A5

_04AE:
    ScrCmd_02C 34
    GoTo _053E

_04B7:
    ScrCmd_02C 35
    GoTo _05C4

_04C0:
    ScrCmd_02C 36
    GoTo _05C4

_04C9:
    ScrCmd_02C 37
    GoTo _05C4

_04D2:
    ScrCmd_02C 38
    GoTo _05C4

_04DB:
    ScrCmd_02C 39
    GoTo _0559

_04E4:
    ScrCmd_02C 40
    GoTo _05A5

_04ED:
    ScrCmd_02C 41
    GoTo _05A5

_04F6:
    ScrCmd_02C 42
    GoTo _05A5

_04FF:
    ScrCmd_02C 43
    GoTo _05A5

_0508:
    ScrCmd_02C 44
    GoTo _053E

_0511:
    ScrCmd_02C 45
    GoTo _05C4

_051A:
    ScrCmd_02C 46
    GoTo _05C4

_0523:
    ScrCmd_02C 47
    GoTo _05C4

_052C:
    ScrCmd_02C 48
    GoTo _05C4

_0535:
    ScrCmd_02C 49
    GoTo _0559

_053E:
    ScrCmd_230 0x800C, 0x8002, 69
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0602
    GoTo _05A5

_0559:
    ScrCmd_230 0x800C, 0x8002, 69
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0633
    GoTo _05C4

_0574:
    ScrCmd_02C 50
    ScrCmd_1B9 0x800C, 0x8002
    CompareVarToValue 0x800C, 0xFF
    GoToIf 0, _05E3
    ScrCmd_230 0x800C, 0x8002, 69
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0664
    GoTo _05E3

_05A5:
    ScrCmd_034
    ScrCmd_003 20, 0x800C
    Call _00ED
    ScrCmd_003 10, 0x800C
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_05C4:
    ScrCmd_034
    ScrCmd_003 20, 0x800C
    Call _00ED
    ScrCmd_003 10, 0x800C
    ScrCmd_02C 3
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_05E3:
    ScrCmd_034
    ScrCmd_003 20, 0x800C
    Call _00ED
    ScrCmd_003 10, 0x800C
    ScrCmd_02C 4
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0602:
    ScrCmd_034
    ScrCmd_003 20, 0x800C
    Call _00ED
    ScrCmd_003 10, 0x800C
    ScrCmd_02C 5
    ScrCmd_04E 0x483
    ScrCmd_02C 8
    ScrCmd_04F
    ScrCmd_231 0x8002, 69
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0633:
    ScrCmd_034
    ScrCmd_003 20, 0x800C
    Call _00ED
    ScrCmd_003 10, 0x800C
    ScrCmd_02C 6
    ScrCmd_04E 0x483
    ScrCmd_02C 8
    ScrCmd_04F
    ScrCmd_231 0x8002, 69
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0664:
    ScrCmd_034
    ScrCmd_003 20, 0x800C
    Call _00ED
    ScrCmd_003 10, 0x800C
    ScrCmd_02C 7
    ScrCmd_04E 0x483
    ScrCmd_02C 8
    ScrCmd_04F
    ScrCmd_231 0x8002, 69
    ScrCmd_02C 9
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .balign 4, 0
_0698:
    MoveAction_047
    MoveAction_00F
    MoveAction_048
    EndMovement

    .balign 4, 0
_06A8:
    MoveAction_047
    MoveAction_00E
    MoveAction_048
    EndMovement

    .balign 4, 0
_06B8:
    MoveAction_047
    MoveAction_00D
    MoveAction_048
    EndMovement

    .balign 4, 0
_06C8:
    MoveAction_047
    MoveAction_00C
    MoveAction_048
    EndMovement

    .balign 4, 0
_06D8:
    MoveAction_00E
    EndMovement

    .balign 4, 0
_06E0:
    MoveAction_00F
    EndMovement

    .balign 4, 0
_06E8:
    MoveAction_00C
    EndMovement

    .balign 4, 0
_06F0:
    MoveAction_00D
    EndMovement
