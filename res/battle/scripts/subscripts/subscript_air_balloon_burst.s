    .include "macros/btlcmd.inc"

    .data

_000:
    // {0}’s {1} popped!
    PrintMessage pl_msg_00000368_01297, TAG_NICKNAME_ITEM, BTLSCR_DEFENDER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 15
	RemoveItem BTLSCR_DEFENDER
	End