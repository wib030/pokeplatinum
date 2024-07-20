    .include "macros/btlcmd.inc"

    .data

_000:
    // {0} floats in the air with its {1}!
    PrintMessage pl_msg_00000368_01294, TAG_NICKNAME_ITEM, BTLSCR_MSG_BATTLER_TEMP, BTLSCR_MSG_BATTLER_TEMP
	Wait
	WaitButtonABTime 30
    End 
