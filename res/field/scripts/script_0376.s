    .include "macros/scrcmd.inc"

    .data

    .long _0059-.-4
    .long _05B0-.-4
    .long _05E9-.-4
    .long _05FA-.-4
    .long _066E-.-4
    .long _0075-.-4
    .long _0698-.-4
    .long _06AB-.-4
    .long _06BE-.-4
    .long _06D1-.-4
    .long _06E4-.-4
    .long _06F7-.-4
    .long _070A-.-4
    .long _003A-.-4
    .short 0xFD13

_003A:
    ScrCmd_238 19, 0x4000
    CompareVarToValue 0x4000, 0
    GoToIf 1, _0053
    ClearFlag 0x2C3
    End

_0053:
    SetFlag 0x2C3
    End

_0059:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    SetVar 0x4003, 0
    SetVar 0x4004, 0
    GoTo _0091
    End

_0075:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    SetVar 0x4003, 0
    SetVar 0x4004, 1
    GoTo _0091
    End

_0091:
    ScrCmd_313 0
    CompareVarToValue 0x4004, 0
    CallIf 1, _0570
    CompareVarToValue 0x4004, 1
    CallIf 1, _0575
    GoTo _00B7
    End

_00B7:
    CompareVarToValue 0x4004, 0
    CallIf 1, _057A
    CompareVarToValue 0x4004, 1
    CallIf 1, _0592
    ScrCmd_042 19, 2
    ScrCmd_042 20, 3
    ScrCmd_043
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0152
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0160
    CompareVarToValue 0x800C, 2
    GoToIf 1, _0117
    CompareVarToValue 0x800C, 4
    GoToIf 1, _016E
    GoTo _0139
    End

_0117:
    CompareVarToValue 0x4004, 0
    CallIf 1, _05A6
    CompareVarToValue 0x4004, 1
    CallIf 1, _05AB
    GoTo _00B7
    End

_0139:
    GoTo _0141
    End

_0141:
    SetVar 0x40B7, 0
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0152:
    SetVar 0x40B8, 0
    GoTo _017C
    End

_0160:
    SetVar 0x40B8, 1
    GoTo _017C
    End

_016E:
    SetVar 0x40B8, 2
    GoTo _017C
    End

_017C:
    ScrCmd_02C 7
    ScrCmd_041 31, 11, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 21, 0
    ScrCmd_042 22, 1
    ScrCmd_042 23, 2
    ScrCmd_043
    CompareVarToValue 0x800C, 0
    GoToIf 1, _01BA
    CompareVarToValue 0x800C, 1
    GoToIf 1, _01C8
    GoTo _0139
    End

_01BA:
    SetVar 0x40B9, 0
    GoTo _01D6
    End

_01C8:
    SetVar 0x40B9, 1
    GoTo _01D6
    End

_01D6:
    GoTo _01DE
    End

_01DE:
    CompareVarToValue 0x40B8, 0
    CallIf 1, _0386
    CompareVarToValue 0x40B8, 1
    CallIf 1, _0386
    SetVar 0x4000, 0
    ScrCmd_14E
    ScrCmd_014 0x7D6
    SetVar 0x800C, 0x4000
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0139
    CompareVarToValue 0x40B8, 2
    GoToIf 1, _022C
    GoTo _038E
    End

_022C:
    ScrCmd_02C 24
    ScrCmd_040 30, 1, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 13, 0
    ScrCmd_042 14, 1
    ScrCmd_042 5, 2
    ScrCmd_043
    SetVar 0x8008, 0x800C
    CompareVarToValue 0x8008, 0
    GoToIf 1, _0270
    CompareVarToValue 0x8008, 1
    GoToIf 1, _02EC
    GoTo _0139
    End

_0270:
    ScrCmd_02C 25
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _022C
    ScrCmd_034
    CompareVarToValue 0x40B9, 0
    CallIf 1, _02C2
    CompareVarToValue 0x40B9, 1
    CallIf 1, _02CE
    CompareVarToValue 0x800C, 1
    GoToIf 1, _02DA
    CompareVarToValue 0x800C, 3
    GoToIf 1, _02E2
    GoTo _0368
    End

_02C2:
    ScrCmd_0F2 27, 0, 0, 0x800C
    Return

_02CE:
    ScrCmd_0F2 28, 0, 0, 0x800C
    Return

_02DA:
    GoTo _022C
    End

_02E2:
    ScrCmd_150
    GoTo _022C
    End

_02EC:
    ScrCmd_02C 25
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 1
    GoToIf 1, _022C
    ScrCmd_034
    CompareVarToValue 0x40B9, 0
    CallIf 1, _033E
    CompareVarToValue 0x40B9, 1
    CallIf 1, _034A
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0356
    CompareVarToValue 0x800C, 3
    GoToIf 1, _035E
    GoTo _0368
    End

_033E:
    ScrCmd_0F3 27, 0, 0, 0x800C
    Return

_034A:
    ScrCmd_0F3 28, 0, 0, 0x800C
    Return

_0356:
    GoTo _022C
    End

_035E:
    ScrCmd_150
    GoTo _022C
    End

_0368:
    ScrCmd_02C 26
    CompareVarToValue 0x40B8, 2
    CallIf 1, _0386
    Call _05D7
    GoTo _038E
    End

_0386:
    SetVar 0x40B7, 0xFF
    Return

_038E:
    CompareVarToValue 0x40B8, 0
    CallIf 1, _042E
    CompareVarToValue 0x40B8, 1
    CallIf 1, _0449
    CompareVarToValue 0x40B8, 2
    CallIf 1, _0464
    ScrCmd_049 0x603
    GoTo _03C1
    End

_03C1:
    ScrCmd_0BC 6, 1, 0, 0
    ScrCmd_0BD
    CompareVarToValue 0x40B8, 0
    CallIf 1, _048A
    CompareVarToValue 0x40B8, 1
    CallIf 1, _049E
    CompareVarToValue 0x40B8, 2
    CallIf 1, _04B2
    ScrCmd_1E5 58
    ScrCmd_1CD 37, 0, 0, 0, 0
    ScrCmd_1F8
    ScrCmd_2C4 3
    CompareVarToValue 0x40B8, 2
    CallIf 1, _042A
    ScrCmd_0A1
    ScrCmd_0BC 6, 1, 1, 0
    ScrCmd_0BD
    ScrCmd_313 1
    End

_042A:
    ScrCmd_150
    Return

_042E:
    ScrCmd_02C 9
    ScrCmd_030
    ScrCmd_034
    ApplyMovement 0xFF, _04C8
    ApplyMovement 0x800D, _04F0
    WaitMovement
    Return

_0449:
    ScrCmd_02C 9
    ScrCmd_030
    ScrCmd_034
    ApplyMovement 0xFF, _04C8
    ApplyMovement 0x800D, _04F0
    WaitMovement
    Return

_0464:
    ScrCmd_02E 9
    ScrCmd_003 15, 0x800C
    ScrCmd_136
    ScrCmd_135 169
    ScrCmd_034
    ApplyMovement 0xFF, _04DC
    ApplyMovement 0x800D, _0504
    WaitMovement
    Return

_048A:
    ApplyMovement 0xFF, _0518
    ApplyMovement 0x800D, _0548
    WaitMovement
    Return

_049E:
    ApplyMovement 0xFF, _0518
    ApplyMovement 0x800D, _0548
    WaitMovement
    Return

_04B2:
    ApplyMovement 0xFF, _0530
    ApplyMovement 0x800D, _055C
    WaitMovement
    Return

    .balign 4, 0
_04C8:
    MoveAction_00C 3
    MoveAction_00F
    MoveAction_00C 4
    MoveAction_045
    EndMovement

    .balign 4, 0
_04DC:
    MoveAction_00C 3
    MoveAction_00E
    MoveAction_00C 4
    MoveAction_045
    EndMovement

    .balign 4, 0
_04F0:
    MoveAction_00C 2
    MoveAction_00F
    MoveAction_00C 4
    MoveAction_045
    EndMovement

    .balign 4, 0
_0504:
    MoveAction_00C 2
    MoveAction_00E
    MoveAction_00C 4
    MoveAction_045
    EndMovement

    .balign 4, 0
_0518:
    MoveAction_015 3
    MoveAction_016
    MoveAction_015 4
    MoveAction_000
    MoveAction_046
    EndMovement

    .balign 4, 0
_0530:
    MoveAction_015 3
    MoveAction_017
    MoveAction_015 4
    MoveAction_000
    MoveAction_046
    EndMovement

    .balign 4, 0
_0548:
    MoveAction_015 2
    MoveAction_016
    MoveAction_015 4
    MoveAction_046
    EndMovement

    .balign 4, 0
_055C:
    MoveAction_015 2
    MoveAction_017
    MoveAction_015 4
    MoveAction_046
    EndMovement

_0570:
    ScrCmd_02C 0
    Return

_0575:
    ScrCmd_02C 3
    Return

_057A:
    ScrCmd_041 31, 9, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 16, 0
    ScrCmd_042 17, 1
    ScrCmd_02C 1
    Return

_0592:
    ScrCmd_041 31, 11, 0, 1, 0x800C
    ScrCmd_33A 1
    ScrCmd_042 18, 4
    ScrCmd_02C 4
    Return

_05A6:
    ScrCmd_02C 2
    Return

_05AB:
    ScrCmd_02C 5
    Return

_05B0:
    ScrCmd_313 0
    SetVar 0x4003, 1
    SetVar 0x40B7, 0
    ScrCmd_02C 11
    Call _0386
    Call _05D7
    GoTo _038E
    End

_05D7:
    ScrCmd_18D
    ScrCmd_12D 0x800C
    ScrCmd_18E
    ScrCmd_049 0x61B
    ScrCmd_04B 0x61B
    Return

_05E9:
    ScrCmd_02C 12
    ScrCmd_2C5 0x40B8, 0x40B9
    GoTo _0139
    End

_05FA:
    CompareVarToValue 0x40B8, 0
    CallIf 1, _0636
    CompareVarToValue 0x40B8, 1
    CallIf 1, _0636
    CompareVarToValue 0x4050, 1
    CallIf 1, _063C
    CompareVarToValue 0x4050, 3
    CallIf 1, _0653
    GoTo _0139
    End

_0636:
    ScrCmd_30A 38
    Return

_063C:
    ScrCmd_02C 13
    ScrCmd_0CD 0
    ScrCmd_02C 15
    ScrCmd_04E 0x486
    ScrCmd_04F
    SetVar 0x4050, 2
    Return

_0653:
    ScrCmd_02C 13
    ScrCmd_0CD 0
    ScrCmd_02C 14
    ScrCmd_04E 0x486
    ScrCmd_04F
    SetVar 0x4050, 4
    ScrCmd_014 0x806
    Return

_066E:
    GoTo _0139

    .byte 2
    .byte 0
    .byte 0
    .byte 0
    .byte 12
    .byte 0
    .byte 2
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 12
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0
    .byte 12
    .byte 0
    .byte 2
    .byte 0
    .byte 63
    .byte 0
    .byte 1
    .byte 0
    .byte 12
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

_0698:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 27
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06AB:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 28
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06BE:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 29
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06D1:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 30
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06E4:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 31
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_06F7:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 32
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_070A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 33
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .byte 0
    .byte 0
    .byte 0
