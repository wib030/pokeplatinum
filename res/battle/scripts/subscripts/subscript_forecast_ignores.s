    .include "macros/btlcmd.inc"

    .data

_000:
	// {0}'s {1} was blocked by Forecast!
    PrintMessage pl_msg_00000368_01316, TAG_NICKNAME_ABILITY, BTLSCR_MSG_TEMP, BTLSCR_MSG_BATTLER_TEMP
	Wait 
    WaitButtonABTime 30
	End