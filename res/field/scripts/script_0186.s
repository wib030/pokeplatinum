    .include "macros/scrcmd.inc"

    .data

    .long _006D-.-4
    .long _0128-.-4
    .long _013E-.-4
    .long _0154-.-4
    .long _0022-.-4
    .long _0310-.-4
    .long _0323-.-4
    .long _0336-.-4
    .short 0xFD13

_0022:
    SetFlag 0x9F4
    SetFlag 0x282
    SetFlag 0x283
    SetFlag 0x284
    SetFlag 0x285
    ClearFlag 0x286
    ClearFlag 0x287
    ClearFlag 0x288
    ClearFlag 0x289
    ClearFlag 176
    ClearFlag 177
    ClearFlag 178
    ClearFlag 179
    CheckFlag 175
    GoToIf 1, _0063
    End

_0063:
    ScrCmd_186 0, 12, 3
    End

_006D:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 175
    GoToIf 1, _00F7
    ScrCmd_02C 2
    ScrCmd_034
    ScrCmd_04E 0x489
    ScrCmd_04F
    ScrCmd_1BD 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _00B8
    CompareVarToValue 0x800C, 2
    GoToIf 1, _00C8
    CompareVarToValue 0x800C, 3
    GoToIf 1, _00D8
    End

_00B8:
    ApplyMovement 0, _0104
    WaitMovement
    GoTo _00E8

_00C8:
    ApplyMovement 0, _0110
    WaitMovement
    GoTo _00E8

_00D8:
    ApplyMovement 0, _011C
    WaitMovement
    GoTo _00E8

_00E8:
    SetFlag 175
    ScrCmd_02C 3
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_00F7:
    ScrCmd_02C 4
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .balign 4, 0
_0104:
    MoveAction_00F
    MoveAction_021
    EndMovement

    .balign 4, 0
_0110:
    MoveAction_00E
    MoveAction_023
    EndMovement

    .balign 4, 0
_011C:
    MoveAction_00F
    MoveAction_022
    EndMovement

_0128:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_014 0x7E3
    ScrCmd_035
    ScrCmd_147 1
    ScrCmd_061
    End

_013E:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_014 0x7E3
    ScrCmd_035
    ScrCmd_148 18
    ScrCmd_061
    End

_0154:
    ScrCmd_060
    ClearFlag 0x21D
    ScrCmd_064 5
    ScrCmd_014 0x7FA
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8004, 10
    GoToIf 1, _0191
    CompareVarToValue 0x8004, 11
    GoToIf 1, _01A1
    CompareVarToValue 0x8004, 12
    GoToIf 1, _01B1
    End

_0191:
    ApplyMovement 5, _02C0
    WaitMovement
    GoTo _01C1

_01A1:
    ApplyMovement 5, _02D0
    WaitMovement
    GoTo _01C1

_01B1:
    ApplyMovement 5, _02D8
    WaitMovement
    GoTo _01C1

_01C1:
    ApplyMovement 0xFF, _02B8
    WaitMovement
    ScrCmd_0CE 0
    ScrCmd_02C 0
    ScrCmd_034
    ScrCmd_0DE 0x800C
    CompareVarToValue 0x800C, 0x183
    GoToIf 1, _0203
    CompareVarToValue 0x800C, 0x186
    GoToIf 1, _020F
    GoTo _01F7

_01F7:
    ScrCmd_0E5 0x1DF, 0
    GoTo _021B

_0203:
    ScrCmd_0E5 0x1E0, 0
    GoTo _021B

_020F:
    ScrCmd_0E5 0x1E1, 0
    GoTo _021B

_021B:
    ScrCmd_0EC 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _02AC
    ScrCmd_0CE 0
    ScrCmd_0CD 1
    ScrCmd_02C 1
    ScrCmd_034
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8004, 10
    GoToIf 1, _0266
    CompareVarToValue 0x8004, 11
    GoToIf 1, _0276
    CompareVarToValue 0x8004, 12
    GoToIf 1, _0286
    End

_0266:
    ApplyMovement 5, _02E8
    WaitMovement
    GoTo _0296

_0276:
    ApplyMovement 5, _02F8
    WaitMovement
    GoTo _0296

_0286:
    ApplyMovement 5, _0300
    WaitMovement
    GoTo _0296

_0296:
    ScrCmd_049 0x603
    ScrCmd_065 5
    ScrCmd_04B 0x603
    SetVar 0x40EF, 1
    ScrCmd_061
    End

_02AC:
    SetFlag 0x21D
    ScrCmd_0EB
    ScrCmd_061
    End

    .balign 4, 0
_02B8:
    MoveAction_025
    EndMovement

    .balign 4, 0
_02C0:
    MoveAction_010 3
    MoveAction_012
    MoveAction_010 3
    EndMovement

    .balign 4, 0
_02D0:
    MoveAction_010 6
    EndMovement

    .balign 4, 0
_02D8:
    MoveAction_010 3
    MoveAction_013
    MoveAction_010 3
    EndMovement

    .balign 4, 0
_02E8:
    MoveAction_011 3
    MoveAction_013
    MoveAction_011 3
    EndMovement

    .balign 4, 0
_02F8:
    MoveAction_011 6
    EndMovement

    .balign 4, 0
_0300:
    MoveAction_011 3
    MoveAction_012
    MoveAction_011 3
    EndMovement

_0310:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0323:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    ScrCmd_02C 6
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0336:
    SetVar 0x8007, 3
    ScrCmd_014 0x7D2
    End

    .byte 0
    .byte 0
