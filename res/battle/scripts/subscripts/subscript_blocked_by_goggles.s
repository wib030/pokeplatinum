    .include "macros/btlcmd.inc"

    .data

_000:
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 30
    // {0}’s {1} blocks {2}!
    PrintMessage pl_msg_00000368_00689, TAG_NICKNAME_ITEM_MOVE, BTLSCR_DEFENDER, BTLSCR_DEFENDER, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    End 
