    .include "macros/btlcmd.inc"

    .data

_000:
    // {0} was freed of the {1}!
    PrintMessage pl_msg_00000368_01276, TAG_NICKNAME_ITEM, BTLSCR_ATTACKER, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    End