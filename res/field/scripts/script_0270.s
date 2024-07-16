    .include "macros/scrcmd.inc"

    .data

    .long _0027-.-4
    .long _0190-.-4
    .long _02E4-.-4
    .long _0012-.-4
    .short 0xFD13

_0012:
    CheckFlag 230
    GoToIf 0, _001F
    End

_001F:
    SetVar 0x4090, 0
    End

_0027:
    ScrCmd_060
    ScrCmd_0C8 0
    ApplyMovement 28, _0140
    WaitMovement
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8005, 55
    GoToIf 1, _008C
    CompareVarToValue 0x8005, 56
    GoToIf 1, _009C
    CompareVarToValue 0x8005, 57
    GoToIf 1, _00AC
    CompareVarToValue 0x8005, 58
    GoToIf 1, _00BC
    CompareVarToValue 0x8005, 59
    GoToIf 1, _00CC
    CompareVarToValue 0x8005, 60
    GoToIf 1, _00DC
    End

_008C:
    ApplyMovement 28, _014C
    WaitMovement
    GoTo _00EC

_009C:
    ApplyMovement 28, _0158
    WaitMovement
    GoTo _00EC

_00AC:
    ApplyMovement 28, _0164
    WaitMovement
    GoTo _00EC

_00BC:
    ApplyMovement 28, _0170
    WaitMovement
    GoTo _00EC

_00CC:
    ApplyMovement 28, _0178
    WaitMovement
    GoTo _00EC

_00DC:
    ApplyMovement 28, _0184
    WaitMovement
    GoTo _00EC

_00EC:
    ScrCmd_0CD 0
    CheckFlag 223
    CallIf 0, _0134
    CheckFlag 223
    CallIf 1, _0139
    ScrCmd_0CD 0
    ScrCmd_04E 0x481
    ScrCmd_02C 1
    ScrCmd_04F
    SetFlag 223
    SetVar 0x4090, 1
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    SetVar 0x403F, 0x262
    ScrCmd_161
    ScrCmd_06D 28, 48
    ScrCmd_061
    End

_0134:
    ScrCmd_02C 0
    Return

_0139:
    ScrCmd_02C 3
    Return

    .balign 4, 0
_0140:
    MoveAction_022
    MoveAction_04B
    EndMovement

    .balign 4, 0
_014C:
    MoveAction_00C 3
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0158:
    MoveAction_00C 2
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0164:
    MoveAction_00C
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0170:
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0178:
    MoveAction_00D
    MoveAction_00E
    EndMovement

    .balign 4, 0
_0184:
    MoveAction_00D 2
    MoveAction_00E
    EndMovement

_0190:
    ScrCmd_060
    ApplyMovement 0xFF, _0274
    ApplyMovement 28, _02DC
    WaitMovement
    ScrCmd_02C 4
    ScrCmd_034
    SetVar 0x4090, 0
    ScrCmd_162
    ScrCmd_06D 28, 14
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8005, 55
    GoToIf 1, _020D
    CompareVarToValue 0x8005, 56
    GoToIf 1, _021D
    CompareVarToValue 0x8005, 57
    GoToIf 1, _022D
    CompareVarToValue 0x8005, 58
    GoToIf 1, _023D
    CompareVarToValue 0x8005, 59
    GoToIf 1, _024D
    CompareVarToValue 0x8005, 60
    GoToIf 1, _025D
    End

_020D:
    ApplyMovement 28, _0284
    WaitMovement
    GoTo _026D

_021D:
    ApplyMovement 28, _0294
    WaitMovement
    GoTo _026D

_022D:
    ApplyMovement 28, _02A4
    WaitMovement
    GoTo _026D

_023D:
    ApplyMovement 28, _02B4
    WaitMovement
    GoTo _026D

_024D:
    ApplyMovement 28, _02BC
    WaitMovement
    GoTo _026D

_025D:
    ApplyMovement 28, _02CC
    WaitMovement
    GoTo _026D

_026D:
    ScrCmd_061
    End

    .balign 4, 0
_0274:
    MoveAction_023
    EndMovement

    .byte 12
    .byte 0
    .byte 1
    .byte 0
    .byte 254
    .byte 0
    .byte 0
    .byte 0

    .balign 4, 0
_0284:
    MoveAction_00F 2
    MoveAction_00D 3
    MoveAction_023
    EndMovement

    .balign 4, 0
_0294:
    MoveAction_00F 2
    MoveAction_00D 2
    MoveAction_023
    EndMovement

    .balign 4, 0
_02A4:
    MoveAction_00F 2
    MoveAction_00D
    MoveAction_023
    EndMovement

    .balign 4, 0
_02B4:
    MoveAction_00F 2
    EndMovement

    .balign 4, 0
_02BC:
    MoveAction_00F 2
    MoveAction_00C
    MoveAction_023
    EndMovement

    .balign 4, 0
_02CC:
    MoveAction_00F 2
    MoveAction_00C 2
    MoveAction_023
    EndMovement

    .balign 4, 0
_02DC:
    MoveAction_002
    EndMovement

_02E4:
    ScrCmd_060
    ScrCmd_162
    ScrCmd_06D 28, 14
    ApplyMovement 0xFF, _03B4
    ApplyMovement 28, _03E0
    WaitMovement
    ScrCmd_02C 5
    ScrCmd_034
    ScrCmd_069 0x8004, 0x8005
    CompareVarToValue 0x8004, 51
    GoToIf 1, _0341
    CompareVarToValue 0x8004, 52
    GoToIf 1, _0359
    CompareVarToValue 0x8004, 53
    GoToIf 1, _0371
    CompareVarToValue 0x8004, 54
    GoToIf 1, _0389
    End

_0341:
    ApplyMovement 0xFF, _03C0
    ApplyMovement 28, _03EC
    WaitMovement
    GoTo _03A1

_0359:
    ApplyMovement 0xFF, _03C0
    ApplyMovement 28, _0400
    WaitMovement
    GoTo _03A1

_0371:
    ApplyMovement 0xFF, _03C0
    ApplyMovement 28, _0414
    WaitMovement
    GoTo _03A1

_0389:
    ApplyMovement 0xFF, _03D0
    ApplyMovement 28, _0428
    WaitMovement
    GoTo _03A1

_03A1:
    ScrCmd_065 28
    SetFlag 230
    SetVar 0x4090, 2
    ScrCmd_061
    End

    .balign 4, 0
_03B4:
    MoveAction_03F
    MoveAction_021
    EndMovement

    .balign 4, 0
_03C0:
    MoveAction_03F
    MoveAction_023
    MoveAction_020
    EndMovement

    .balign 4, 0
_03D0:
    MoveAction_03F
    MoveAction_022
    MoveAction_020
    EndMovement

    .balign 4, 0
_03E0:
    MoveAction_020
    MoveAction_04B
    EndMovement

    .balign 4, 0
_03EC:
    MoveAction_00F
    MoveAction_00C 7
    MoveAction_00F 4
    MoveAction_023
    EndMovement

    .balign 4, 0
_0400:
    MoveAction_00F
    MoveAction_00C 7
    MoveAction_00F 3
    MoveAction_023
    EndMovement

    .balign 4, 0
_0414:
    MoveAction_00F
    MoveAction_00C 7
    MoveAction_00F 2
    MoveAction_023
    EndMovement

    .balign 4, 0
_0428:
    MoveAction_00E
    MoveAction_00C 7
    MoveAction_00F 3
    MoveAction_023
    EndMovement
