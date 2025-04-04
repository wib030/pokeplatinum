    .include "macros/btlcmd.inc"

    .data

_000:
    SetMultiHit 2, SYSCTL_MULTI_HIT_MOVE, 2
    UpdateVar OPCODE_SET, BTLVAR_AFTER_MOVE_MESSAGE_TYPE, AFTER_MOVE_MESSAGE_MULTI_HIT
    CalcCrit 
    CalcDamage 
    End 
