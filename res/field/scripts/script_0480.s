    .include "macros/scrcmd.inc"

    .data

    .long _000A-.-4
    .long _017A-.-4
    .short 0xFD13

_000A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_068
    CheckFlag 0x137
    GoToIf 1, _0114
    CheckFlag 0xAAE
    GoToIf 1, _016F
    ScrCmd_277 0x8000
    ScrCmd_0D5 0, 0x8000
    ScrCmd_177 0x8002
    SetVar 0x8003, 0
_003B:
    ScrCmd_278 0x8001, 0x8003
    CompareVarToVar 0x8000, 0x8001
    GoToIf 1, _006D
    AddVar 0x8003, 1
    SubVar 0x8002, 1
    CompareVarToValue 0x8002, 0
    GoToIf 5, _003B
    GoTo _0164

_006D:
    CompareVarToValue 0x4108, 0
    CallIf 1, _00ED
    CompareVarToValue 0x4108, 1
    CallIf 1, _00F5
    CompareVarToValue 0x4108, 2
    CallIf 1, _00FD
    ScrCmd_0D0 1, 0x8003
    ScrCmd_0D1 2, 0x8004
    ScrCmd_02C 1
    SetVar 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0105
    GoTo _00C2

_00C2:
    ScrCmd_014 0x7E0
    ClearFlag 0x137
    SetFlag 0xAAE
    AddVar 0x4108, 1
    CompareVarToValue 0x4108, 3
    GoToIf 0, _00E7
    SetVar 0x4108, 0
_00E7:
    ScrCmd_034
    ScrCmd_061
    End

_00ED:
    SetVar 0x8004, 241
    Return

_00F5:
    SetVar 0x8004, 0x10C
    Return

_00FD:
    SetVar 0x8004, 0x113
    Return

_0105:
    SetFlag 0x137
    ScrCmd_02C 2
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_0114:
    CompareVarToValue 0x4108, 0
    CallIf 1, _00ED
    CompareVarToValue 0x4108, 1
    CallIf 1, _00F5
    CompareVarToValue 0x4108, 2
    CallIf 1, _00FD
    ScrCmd_0D1 2, 0x8004
    ScrCmd_02C 3
    SetVar 0x8005, 1
    ScrCmd_07D 0x8004, 0x8005, 0x800C
    CompareVarToValue 0x800C, 0
    GoToIf 1, _0105
    GoTo _00C2

_0164:
    ScrCmd_02C 0
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_016F:
    ScrCmd_02C 4
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

_017A:
    ScrCmd_049 0x5DC
    ScrCmd_060
    ScrCmd_02C 5
    ScrCmd_031
    ScrCmd_034
    ScrCmd_061
    End

    .byte 0
