    .include "macros/scrcmd.inc"

    .data

    .long _000E-.-4
    .long _0398-.-4
    .long _040F-.-4
    .short 0xFD13

_000E:
    SetFlag 0x183
    SetFlag 0x184
    CheckFlag 0x97F
    GoToIf 0, _0029
    Call _002B
    End

_0029:
    End

_002B:
    ScrCmd_234 0x4000
    CompareVarToValue 0x4000, 0
    CallIf 1, _008C
    CompareVarToValue 0x4000, 1
    CallIf 1, _00ED
    CompareVarToValue 0x4000, 2
    CallIf 1, _014E
    CompareVarToValue 0x4000, 3
    CallIf 1, _01AF
    CompareVarToValue 0x4000, 4
    CallIf 1, _0203
    CompareVarToValue 0x4000, 5
    CallIf 1, _0264
    CompareVarToValue 0x4000, 6
    CallIf 1, _02C5
    Return

_008C:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 168
    CallIf 1, _0326
    CompareVarToValue 0x4001, 189
    CallIf 1, _0332
    CompareVarToValue 0x4001, 189
    CallIf 1, _0348
    CompareVarToValue 0x4001, 101
    CallIf 1, _035E
    CompareVarToValue 0x4001, 69
    CallIf 1, _036A
    CompareVarToValue 0x4001, 0x1CB
    CallIf 1, _0380
    CompareVarToValue 0x4001, 123
    CallIf 1, _038C
    Return

_00ED:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 123
    CallIf 1, _0326
    CompareVarToValue 0x4001, 6
    CallIf 1, _0332
    CompareVarToValue 0x4001, 6
    CallIf 1, _0348
    CompareVarToValue 0x4001, 0x1BB
    CallIf 1, _035E
    CompareVarToValue 0x4001, 69
    CallIf 1, _036A
    CompareVarToValue 0x4001, 0x1AC
    CallIf 1, _0380
    CompareVarToValue 0x4001, 36
    CallIf 1, _038C
    Return

_014E:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 48
    CallIf 1, _0326
    CompareVarToValue 0x4001, 101
    CallIf 1, _0332
    CompareVarToValue 0x4001, 101
    CallIf 1, _0348
    CompareVarToValue 0x4001, 123
    CallIf 1, _035E
    CompareVarToValue 0x4001, 69
    CallIf 1, _036A
    CompareVarToValue 0x4001, 0x1B3
    CallIf 1, _0380
    CompareVarToValue 0x4001, 36
    CallIf 1, _038C
    Return

_01AF:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _0332
    CompareVarToValue 0x4001, 6
    CallIf 1, _0348
    CompareVarToValue 0x4001, 0x1BB
    CallIf 1, _035E
    CompareVarToValue 0x4001, 134
    CallIf 1, _036A
    CompareVarToValue 0x4001, 0x1CB
    CallIf 1, _0380
    CompareVarToValue 0x4001, 123
    CallIf 1, _038C
    Return

_0203:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 48
    CallIf 1, _0326
    CompareVarToValue 0x4001, 6
    CallIf 1, _0332
    CompareVarToValue 0x4001, 6
    CallIf 1, _0348
    CompareVarToValue 0x4001, 123
    CallIf 1, _035E
    CompareVarToValue 0x4001, 134
    CallIf 1, _036A
    CompareVarToValue 0x4001, 0x1AC
    CallIf 1, _0380
    CompareVarToValue 0x4001, 151
    CallIf 1, _038C
    Return

_0264:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 123
    CallIf 1, _0326
    CompareVarToValue 0x4001, 101
    CallIf 1, _0332
    CompareVarToValue 0x4001, 101
    CallIf 1, _0348
    CompareVarToValue 0x4001, 0x1BB
    CallIf 1, _035E
    CompareVarToValue 0x4001, 134
    CallIf 1, _036A
    CompareVarToValue 0x4001, 0x1B3
    CallIf 1, _0380
    CompareVarToValue 0x4001, 151
    CallIf 1, _038C
    Return

_02C5:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 168
    CallIf 1, _0326
    CompareVarToValue 0x4001, 189
    CallIf 1, _0332
    CompareVarToValue 0x4001, 189
    CallIf 1, _0348
    CompareVarToValue 0x4001, 101
    CallIf 1, _035E
    CompareVarToValue 0x4001, 69
    CallIf 1, _036A
    CompareVarToValue 0x4001, 0x1CB
    CallIf 1, _0380
    CompareVarToValue 0x4001, 123
    CallIf 1, _038C
    Return

_0326:
    ClearFlag 0x183
    SetVar 0x402A, 35
    Return

_0332:
    ClearFlag 0x183
    ClearFlag 0x184
    SetVar 0x402A, 22
    SetVar 0x402B, 23
    Return

_0348:
    ClearFlag 0x183
    ClearFlag 0x184
    SetVar 0x402A, 22
    SetVar 0x402B, 23
    Return

_035E:
    ClearFlag 0x183
    SetVar 0x402A, 43
    Return

_036A:
    ClearFlag 0x183
    ClearFlag 0x184
    SetVar 0x402A, 22
    SetVar 0x402B, 23
    Return

_0380:
    ClearFlag 0x183
    SetVar 0x402A, 71
    Return

_038C:
    ClearFlag 0x183
    SetVar 0x402A, 10
    Return

_0398:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    SetVar 0x8004, 0
    SetVar 0x8006, 0
    CompareVarToValue 0x402A, 35
    CallIf 1, _1331
    CompareVarToValue 0x402A, 22
    CallIf 1, _133E
    CompareVarToValue 0x402A, 43
    CallIf 1, _13A3
    CompareVarToValue 0x402A, 22
    CallIf 1, _13B0
    CompareVarToValue 0x402A, 71
    CallIf 1, _13DD
    CompareVarToValue 0x402A, 10
    CallIf 1, _13EA
    CompareVarToValue 0x8006, 1
    GoToIf 1, _06A8
    GoTo _0486
    End

_040F:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    SetVar 0x8004, 1
    SetVar 0x8006, 0
    CompareVarToValue 0x402B, 35
    CallIf 1, _1331
    CompareVarToValue 0x402B, 23
    CallIf 1, _1378
    CompareVarToValue 0x402B, 43
    CallIf 1, _13A3
    CompareVarToValue 0x402B, 22
    CallIf 1, _13B0
    CompareVarToValue 0x402B, 71
    CallIf 1, _13DD
    CompareVarToValue 0x402B, 10
    CallIf 1, _13EA
    CompareVarToValue 0x8006, 1
    GoToIf 1, _06A8
    GoTo _0486
    End

_0486:
    SetVar 0x8000, 1
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _05B9
    CompareVarToValue 0x4001, 134
    CallIf 1, _05B9
    CompareVarToValue 0x8000, 0
    GoToIf 1, _0590
    CompareVarToValue 0x8004, 1
    GoToIf 1, _051A
    CompareVarToValue 0x402A, 35
    CallIf 1, _0828
    CompareVarToValue 0x402A, 22
    CallIf 1, _0864
    CompareVarToValue 0x402A, 43
    CallIf 1, _08F6
    CompareVarToValue 0x402A, 22
    CallIf 1, _0932
    CompareVarToValue 0x402A, 71
    CallIf 1, _0986
    CompareVarToValue 0x402A, 10
    CallIf 1, _09C2
    GoTo _0570
    End

_051A:
    CompareVarToValue 0x402B, 35
    CallIf 1, _0828
    CompareVarToValue 0x402B, 23
    CallIf 1, _08A0
    CompareVarToValue 0x402B, 43
    CallIf 1, _08F6
    CompareVarToValue 0x402B, 22
    CallIf 1, _095C
    CompareVarToValue 0x402B, 71
    CallIf 1, _0986
    CompareVarToValue 0x402B, 10
    CallIf 1, _09C2
    GoTo _0570
    End

_0570:
    ScrCmd_03E 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _05CE
    CompareVarToValue 0x800C, 1
    GoToIf 1, _0761
    End

_0590:
    CompareVarToValue 0x8004, 1
    GoToIf 1, _05AB
    Call _100C
    GoTo _081A
    End

_05AB:
    Call _1036
    GoTo _081A
    End

_05B9:
    CompareVarToValue 0x402A, 22
    GoToIf 1, _05C8
    Return

_05C8:
    ScrCmd_0EE 0x8000
    Return

_05CE:
    CompareVarToValue 0x8004, 1
    GoToIf 1, _0631
    CompareVarToValue 0x402A, 35
    CallIf 1, _09FE
    CompareVarToValue 0x402A, 22
    CallIf 1, _0A40
    CompareVarToValue 0x402A, 43
    CallIf 1, _0AF6
    CompareVarToValue 0x402A, 22
    CallIf 1, _0B38
    CompareVarToValue 0x402A, 71
    CallIf 1, _0BA4
    CompareVarToValue 0x402A, 10
    CallIf 1, _0BE6
    GoTo _0687
    End

_0631:
    CompareVarToValue 0x402B, 35
    CallIf 1, _09FE
    CompareVarToValue 0x402B, 23
    CallIf 1, _0A8E
    CompareVarToValue 0x402B, 43
    CallIf 1, _0AF6
    CompareVarToValue 0x402B, 22
    CallIf 1, _0B6E
    CompareVarToValue 0x402B, 71
    CallIf 1, _0BA4
    CompareVarToValue 0x402B, 10
    CallIf 1, _0BE6
    GoTo _0687
    End

_0687:
    ScrCmd_034
    ScrCmd_0E5 0x8005, 0
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0822
    GoTo _06A8
    End

_06A8:
    CompareVarToValue 0x8004, 1
    GoToIf 1, _070B
    CompareVarToValue 0x402A, 35
    CallIf 1, _0DFE
    CompareVarToValue 0x402A, 22
    CallIf 1, _0E3E
    CompareVarToValue 0x402A, 43
    CallIf 1, _0EE8
    CompareVarToValue 0x402A, 22
    CallIf 1, _0F28
    CompareVarToValue 0x402A, 71
    CallIf 1, _0F8C
    CompareVarToValue 0x402A, 10
    CallIf 1, _0FCC
    GoTo _081A
    End

_070B:
    CompareVarToValue 0x402B, 35
    CallIf 1, _0DFE
    CompareVarToValue 0x402B, 23
    CallIf 1, _0E86
    CompareVarToValue 0x402B, 43
    CallIf 1, _0EE8
    CompareVarToValue 0x402B, 22
    CallIf 1, _0F5A
    CompareVarToValue 0x402B, 71
    CallIf 1, _0F8C
    CompareVarToValue 0x402B, 10
    CallIf 1, _0FCC
    GoTo _081A
    End

_0761:
    CompareVarToValue 0x8004, 1
    GoToIf 1, _07C4
    CompareVarToValue 0x402A, 35
    CallIf 1, _0C28
    CompareVarToValue 0x402A, 22
    CallIf 1, _0C64
    CompareVarToValue 0x402A, 43
    CallIf 1, _0CF6
    CompareVarToValue 0x402A, 22
    CallIf 1, _0D32
    CompareVarToValue 0x402A, 71
    CallIf 1, _0D86
    CompareVarToValue 0x402A, 10
    CallIf 1, _0DC2
    GoTo _081A
    End

_07C4:
    CompareVarToValue 0x402B, 35
    CallIf 1, _0C28
    CompareVarToValue 0x402B, 23
    CallIf 1, _0CA0
    CompareVarToValue 0x402B, 43
    CallIf 1, _0CF6
    CompareVarToValue 0x402B, 22
    CallIf 1, _0D5C
    CompareVarToValue 0x402B, 71
    CallIf 1, _0D86
    CompareVarToValue 0x402B, 10
    CallIf 1, _0DC2
    GoTo _081A
    End

_081A:
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0822:
    ScrCmd_0EB
    ScrCmd_061
    End

_0828:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 48
    CallIf 1, _0855
    CompareVarToValue 0x4001, 123
    CallIf 1, _085A
    CompareVarToValue 0x4001, 168
    CallIf 1, _085F
    Return

_0855:
    ScrCmd_02C 16
    Return

_085A:
    ScrCmd_02C 38
    Return

_085F:
    ScrCmd_02C 64
    Return

_0864:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _0891
    CompareVarToValue 0x4001, 101
    CallIf 1, _0896
    CompareVarToValue 0x4001, 189
    CallIf 1, _089B
    Return

_0891:
    ScrCmd_02C 0
    Return

_0896:
    ScrCmd_02C 30
    Return

_089B:
    ScrCmd_02C 68
    Return

_08A0:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _08E7
    CompareVarToValue 0x4001, 101
    CallIf 1, _08EC
    CompareVarToValue 0x4001, 189
    CallIf 1, _08F1
    CompareVarToValue 0x4001, 69
    CallIf 1, _097C
    CompareVarToValue 0x4001, 134
    CallIf 1, _0981
    Return

_08E7:
    ScrCmd_02C 4
    Return

_08EC:
    ScrCmd_02C 34
    Return

_08F1:
    ScrCmd_02C 72
    Return

_08F6:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 101
    CallIf 1, _0923
    CompareVarToValue 0x4001, 123
    CallIf 1, _0928
    CompareVarToValue 0x4001, 0x1BB
    CallIf 1, _092D
    Return

_0923:
    ScrCmd_02C 8
    Return

_0928:
    ScrCmd_02C 42
    Return

_092D:
    ScrCmd_02C 84
    Return

_0932:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _0952
    CompareVarToValue 0x4001, 134
    CallIf 1, _0957
    Return

_0952:
    ScrCmd_02C 26
    Return

_0957:
    ScrCmd_02C 56
    Return

_095C:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _097C
    CompareVarToValue 0x4001, 134
    CallIf 1, _0981
    Return

_097C:
    ScrCmd_02C 21
    Return

_0981:
    ScrCmd_02C 51
    Return

_0986:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 0x1AC
    CallIf 1, _09B3
    CompareVarToValue 0x4001, 0x1B3
    CallIf 1, _09B8
    CompareVarToValue 0x4001, 0x1CB
    CallIf 1, _09BD
    Return

_09B3:
    ScrCmd_02C 76
    Return

_09B8:
    ScrCmd_02C 80
    Return

_09BD:
    ScrCmd_02C 88
    Return

_09C2:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 36
    CallIf 1, _09EF
    CompareVarToValue 0x4001, 123
    CallIf 1, _09F4
    CompareVarToValue 0x4001, 151
    CallIf 1, _09F9
    Return

_09EF:
    ScrCmd_02C 12
    Return

_09F4:
    ScrCmd_02C 46
    Return

_09F9:
    ScrCmd_02C 60
    Return

_09FE:
    Call _1060
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 48
    CallIf 1, _0A31
    CompareVarToValue 0x4001, 123
    CallIf 1, _0A36
    CompareVarToValue 0x4001, 168
    CallIf 1, _0A3B
    Return

_0A31:
    ScrCmd_02C 17
    Return

_0A36:
    ScrCmd_02C 39
    Return

_0A3B:
    ScrCmd_02C 65
    Return

_0A40:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _0A6D
    CompareVarToValue 0x4001, 101
    CallIf 1, _0A78
    CompareVarToValue 0x4001, 189
    CallIf 1, _0A83
    Return

_0A6D:
    Call _10C7
    ScrCmd_02C 1
    Return

_0A78:
    Call _10C7
    ScrCmd_02C 31
    Return

_0A83:
    Call _10C7
    ScrCmd_02C 69
    Return

_0A8E:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _0AD5
    CompareVarToValue 0x4001, 101
    CallIf 1, _0AE0
    CompareVarToValue 0x4001, 189
    CallIf 1, _0AEB
    CompareVarToValue 0x4001, 69
    CallIf 1, _0B8E
    CompareVarToValue 0x4001, 134
    CallIf 1, _0B99
    Return

_0AD5:
    Call _112E
    ScrCmd_02C 5
    Return

_0AE0:
    Call _112E
    ScrCmd_02C 35
    Return

_0AEB:
    Call _112E
    ScrCmd_02C 73
    Return

_0AF6:
    Call _1195
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 101
    CallIf 1, _0B29
    CompareVarToValue 0x4001, 123
    CallIf 1, _0B2E
    CompareVarToValue 0x4001, 0x1BB
    CallIf 1, _0B33
    Return

_0B29:
    ScrCmd_02C 9
    Return

_0B2E:
    ScrCmd_02C 43
    Return

_0B33:
    ScrCmd_02C 85
    Return

_0B38:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _0B58
    CompareVarToValue 0x4001, 134
    CallIf 1, _0B63
    Return

_0B58:
    Call _11FC
    ScrCmd_02C 27
    Return

_0B63:
    Call _11FC
    ScrCmd_02C 57
    Return

_0B6E:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _0B8E
    CompareVarToValue 0x4001, 134
    CallIf 1, _0B99
    Return

_0B8E:
    Call _11FC
    ScrCmd_02C 22
    Return

_0B99:
    Call _11FC
    ScrCmd_02C 52
    Return

_0BA4:
    Call _1263
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 0x1AC
    CallIf 1, _0BD7
    CompareVarToValue 0x4001, 0x1B3
    CallIf 1, _0BDC
    CompareVarToValue 0x4001, 0x1CB
    CallIf 1, _0BE1
    Return

_0BD7:
    ScrCmd_02C 77
    Return

_0BDC:
    ScrCmd_02C 81
    Return

_0BE1:
    ScrCmd_02C 89
    Return

_0BE6:
    Call _12CA
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 36
    CallIf 1, _0C19
    CompareVarToValue 0x4001, 123
    CallIf 1, _0C1E
    CompareVarToValue 0x4001, 151
    CallIf 1, _0C23
    Return

_0C19:
    ScrCmd_02C 13
    Return

_0C1E:
    ScrCmd_02C 47
    Return

_0C23:
    ScrCmd_02C 61
    Return

_0C28:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 48
    CallIf 1, _0C55
    CompareVarToValue 0x4001, 123
    CallIf 1, _0C5A
    CompareVarToValue 0x4001, 168
    CallIf 1, _0C5F
    Return

_0C55:
    ScrCmd_02C 18
    Return

_0C5A:
    ScrCmd_02C 40
    Return

_0C5F:
    ScrCmd_02C 66
    Return

_0C64:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _0C91
    CompareVarToValue 0x4001, 101
    CallIf 1, _0C96
    CompareVarToValue 0x4001, 189
    CallIf 1, _0C9B
    Return

_0C91:
    ScrCmd_02C 2
    Return

_0C96:
    ScrCmd_02C 32
    Return

_0C9B:
    ScrCmd_02C 70
    Return

_0CA0:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _0CE7
    CompareVarToValue 0x4001, 101
    CallIf 1, _0CEC
    CompareVarToValue 0x4001, 189
    CallIf 1, _0CF1
    CompareVarToValue 0x4001, 69
    CallIf 1, _0D7C
    CompareVarToValue 0x4001, 134
    CallIf 1, _0D81
    Return

_0CE7:
    ScrCmd_02C 6
    Return

_0CEC:
    ScrCmd_02C 36
    Return

_0CF1:
    ScrCmd_02C 74
    Return

_0CF6:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 101
    CallIf 1, _0D23
    CompareVarToValue 0x4001, 123
    CallIf 1, _0D28
    CompareVarToValue 0x4001, 0x1BB
    CallIf 1, _0D2D
    Return

_0D23:
    ScrCmd_02C 10
    Return

_0D28:
    ScrCmd_02C 44
    Return

_0D2D:
    ScrCmd_02C 86
    Return

_0D32:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _0D52
    CompareVarToValue 0x4001, 134
    CallIf 1, _0D57
    Return

_0D52:
    ScrCmd_02C 28
    Return

_0D57:
    ScrCmd_02C 58
    Return

_0D5C:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _0D7C
    CompareVarToValue 0x4001, 134
    CallIf 1, _0D81
    Return

_0D7C:
    ScrCmd_02C 23
    Return

_0D81:
    ScrCmd_02C 53
    Return

_0D86:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 0x1AC
    CallIf 1, _0DB3
    CompareVarToValue 0x4001, 0x1B3
    CallIf 1, _0DB8
    CompareVarToValue 0x4001, 0x1CB
    CallIf 1, _0DBD
    Return

_0DB3:
    ScrCmd_02C 78
    Return

_0DB8:
    ScrCmd_02C 82
    Return

_0DBD:
    ScrCmd_02C 90
    Return

_0DC2:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 36
    CallIf 1, _0DEF
    CompareVarToValue 0x4001, 123
    CallIf 1, _0DF4
    CompareVarToValue 0x4001, 151
    CallIf 1, _0DF9
    Return

_0DEF:
    ScrCmd_02C 14
    Return

_0DF4:
    ScrCmd_02C 48
    Return

_0DF9:
    ScrCmd_02C 62
    Return

_0DFE:
    SetFlag 0xABC
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 48
    CallIf 1, _0E2F
    CompareVarToValue 0x4001, 123
    CallIf 1, _0E34
    CompareVarToValue 0x4001, 168
    CallIf 1, _0E39
    Return

_0E2F:
    ScrCmd_02C 19
    Return

_0E34:
    ScrCmd_02C 41
    Return

_0E39:
    ScrCmd_02C 67
    Return

_0E3E:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _0E6B
    CompareVarToValue 0x4001, 101
    CallIf 1, _0E74
    CompareVarToValue 0x4001, 189
    CallIf 1, _0E7D
    Return

_0E6B:
    SetFlag 0xABD
    ScrCmd_02C 3
    Return

_0E74:
    SetFlag 0xABD
    ScrCmd_02C 33
    Return

_0E7D:
    SetFlag 0xABD
    ScrCmd_02C 71
    Return

_0E86:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    CallIf 1, _0ECD
    CompareVarToValue 0x4001, 101
    CallIf 1, _0ED6
    CompareVarToValue 0x4001, 189
    CallIf 1, _0EDF
    CompareVarToValue 0x4001, 69
    CallIf 1, _0F7A
    CompareVarToValue 0x4001, 134
    CallIf 1, _0F83
    Return

_0ECD:
    SetFlag 0xABE
    ScrCmd_02C 7
    Return

_0ED6:
    SetFlag 0xABE
    ScrCmd_02C 37
    Return

_0EDF:
    SetFlag 0xABE
    ScrCmd_02C 75
    Return

_0EE8:
    SetFlag 0xABF
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 101
    CallIf 1, _0F19
    CompareVarToValue 0x4001, 123
    CallIf 1, _0F1E
    CompareVarToValue 0x4001, 0x1BB
    CallIf 1, _0F23
    Return

_0F19:
    ScrCmd_02C 11
    Return

_0F1E:
    ScrCmd_02C 45
    Return

_0F23:
    ScrCmd_02C 87
    Return

_0F28:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _0F48
    CompareVarToValue 0x4001, 134
    CallIf 1, _0F51
    Return

_0F48:
    SetFlag 0xAC0
    ScrCmd_02C 29
    Return

_0F51:
    SetFlag 0xAC0
    ScrCmd_02C 59
    Return

_0F5A:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _0F7A
    CompareVarToValue 0x4001, 134
    CallIf 1, _0F83
    Return

_0F7A:
    SetFlag 0xAC0
    ScrCmd_02C 24
    Return

_0F83:
    SetFlag 0xAC0
    ScrCmd_02C 54
    Return

_0F8C:
    SetFlag 0xAC1
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 0x1AC
    CallIf 1, _0FBD
    CompareVarToValue 0x4001, 0x1B3
    CallIf 1, _0FC2
    CompareVarToValue 0x4001, 0x1CB
    CallIf 1, _0FC7
    Return

_0FBD:
    ScrCmd_02C 79
    Return

_0FC2:
    ScrCmd_02C 83
    Return

_0FC7:
    ScrCmd_02C 91
    Return

_0FCC:
    SetFlag 0xAC2
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 36
    CallIf 1, _0FFD
    CompareVarToValue 0x4001, 123
    CallIf 1, _1002
    CompareVarToValue 0x4001, 151
    CallIf 1, _1007
    Return

_0FFD:
    ScrCmd_02C 15
    Return

_1002:
    ScrCmd_02C 49
    Return

_1007:
    ScrCmd_02C 63
    Return

_100C:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _102C
    CompareVarToValue 0x4001, 134
    CallIf 1, _1031
    Return

_102C:
    ScrCmd_02C 25
    Return

_1031:
    ScrCmd_02C 55
    Return

_1036:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    CallIf 1, _1056
    CompareVarToValue 0x4001, 134
    CallIf 1, _105B
    Return

_1056:
    ScrCmd_02C 20
    Return

_105B:
    ScrCmd_02C 50
    Return

_1060:
    SetVar 0x8005, 0x173
    CheckFlag 0x97F
    CallIf 1, _109F
    CheckFlag 0x980
    CallIf 1, _10A7
    CheckFlag 0x981
    CallIf 1, _10AF
    CheckFlag 0x982
    CallIf 1, _10B7
    CheckFlag 0x983
    CallIf 1, _10BF
    Return

_109F:
    SetVar 0x8005, 0x370
    Return

_10A7:
    SetVar 0x8005, 0x371
    Return

_10AF:
    SetVar 0x8005, 0x372
    Return

_10B7:
    SetVar 0x8005, 0x38C
    Return

_10BF:
    SetVar 0x8005, 0x394
    Return

_10C7:
    SetVar 0x8005, 0x172
    CheckFlag 0x97F
    CallIf 1, _1106
    CheckFlag 0x980
    CallIf 1, _110E
    CheckFlag 0x981
    CallIf 1, _1116
    CheckFlag 0x982
    CallIf 1, _111E
    CheckFlag 0x983
    CallIf 1, _1126
    Return

_1106:
    SetVar 0x8005, 0x37E
    Return

_110E:
    SetVar 0x8005, 0x37F
    Return

_1116:
    SetVar 0x8005, 0x380
    Return

_111E:
    SetVar 0x8005, 0x390
    Return

_1126:
    SetVar 0x8005, 0x398
    Return

_112E:
    SetVar 0x8005, 0x171
    CheckFlag 0x97F
    CallIf 1, _116D
    CheckFlag 0x980
    CallIf 1, _1175
    CheckFlag 0x981
    CallIf 1, _117D
    CheckFlag 0x982
    CallIf 1, _1185
    CheckFlag 0x983
    CallIf 1, _118D
    Return

_116D:
    SetVar 0x8005, 0x36A
    Return

_1175:
    SetVar 0x8005, 0x36B
    Return

_117D:
    SetVar 0x8005, 0x36C
    Return

_1185:
    SetVar 0x8005, 0x38A
    Return

_118D:
    SetVar 0x8005, 0x392
    Return

_1195:
    SetVar 0x8005, 0x1CC
    CheckFlag 0x97F
    CallIf 1, _11D4
    CheckFlag 0x980
    CallIf 1, _11DC
    CheckFlag 0x981
    CallIf 1, _11E4
    CheckFlag 0x982
    CallIf 1, _11EC
    CheckFlag 0x983
    CallIf 1, _11F4
    Return

_11D4:
    SetVar 0x8005, 0x376
    Return

_11DC:
    SetVar 0x8005, 0x377
    Return

_11E4:
    SetVar 0x8005, 0x378
    Return

_11EC:
    SetVar 0x8005, 0x38E
    Return

_11F4:
    SetVar 0x8005, 0x396
    Return

_11FC:
    SetVar 0x8005, 0x170
    CheckFlag 0x97F
    CallIf 1, _123B
    CheckFlag 0x980
    CallIf 1, _1243
    CheckFlag 0x981
    CallIf 1, _124B
    CheckFlag 0x982
    CallIf 1, _1253
    CheckFlag 0x983
    CallIf 1, _125B
    Return

_123B:
    SetVar 0x8005, 0x373
    Return

_1243:
    SetVar 0x8005, 0x374
    Return

_124B:
    SetVar 0x8005, 0x375
    Return

_1253:
    SetVar 0x8005, 0x38D
    Return

_125B:
    SetVar 0x8005, 0x395
    Return

_1263:
    SetVar 0x8005, 0x1D1
    CheckFlag 0x97F
    CallIf 1, _12A2
    CheckFlag 0x980
    CallIf 1, _12AA
    CheckFlag 0x981
    CallIf 1, _12B2
    CheckFlag 0x982
    CallIf 1, _12BA
    CheckFlag 0x983
    CallIf 1, _12C2
    Return

_12A2:
    SetVar 0x8005, 0x379
    Return

_12AA:
    SetVar 0x8005, 0x37A
    Return

_12B2:
    SetVar 0x8005, 0x37B
    Return

_12BA:
    SetVar 0x8005, 0x38F
    Return

_12C2:
    SetVar 0x8005, 0x397
    Return

_12CA:
    SetVar 0x8005, 0x1AF
    CheckFlag 0x97F
    CallIf 1, _1309
    CheckFlag 0x980
    CallIf 1, _1311
    CheckFlag 0x981
    CallIf 1, _1319
    CheckFlag 0x982
    CallIf 1, _1321
    CheckFlag 0x983
    CallIf 1, _1329
    Return

_1309:
    SetVar 0x8005, 0x36D
    Return

_1311:
    SetVar 0x8005, 0x36E
    Return

_1319:
    SetVar 0x8005, 0x36F
    Return

_1321:
    SetVar 0x8005, 0x38B
    Return

_1329:
    SetVar 0x8005, 0x393
    Return

_1331:
    CheckFlag 0xABC
    GoToIf 1, _13F7
    Return

_133E:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 6
    GoToIf 1, _136B
    CompareVarToValue 0x4001, 101
    GoToIf 1, _136B
    CompareVarToValue 0x4001, 189
    GoToIf 1, _136B
    Return

_136B:
    CheckFlag 0xABD
    GoToIf 1, _13F7
    Return

_1378:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    GoToIf 1, _13B0
    CompareVarToValue 0x4001, 134
    GoToIf 1, _13B0
    CheckFlag 0xABE
    GoToIf 1, _13F7
    Return

_13A3:
    CheckFlag 0xABF
    GoToIf 1, _13F7
    Return

_13B0:
    ScrCmd_201 0x4001
    CompareVarToValue 0x4001, 69
    GoToIf 1, _13D0
    CompareVarToValue 0x4001, 134
    GoToIf 1, _13D0
    Return

_13D0:
    CheckFlag 0xAC0
    GoToIf 1, _13F7
    Return

_13DD:
    CheckFlag 0xAC1
    GoToIf 1, _13F7
    Return

_13EA:
    CheckFlag 0xAC2
    GoToIf 1, _13F7
    Return

_13F7:
    SetVar 0x8006, 1
    Return

    .byte 0
