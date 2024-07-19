    .include "macros/btlcmd.inc"

    .data

_000:
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 30
    // It doesn’t affect {0}...
    PrintMessage pl_msg_00000368_00027, TAG_NICKNAME, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    End 
