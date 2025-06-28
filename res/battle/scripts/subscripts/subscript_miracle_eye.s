    .include "macros/btlcmd.inc"

    .data

_000:
    // {0} identified {1}!
    PrintMessage pl_msg_00000368_00432, TAG_NICKNAME_NICKNAME, BTLSCR_ATTACKER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    End 