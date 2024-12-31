    .include "macros/btlcmd.inc"

    .data

_000:
    // {0}’s {1} activated!
    PrintMessage pl_msg_00000368_01279, TAG_NICKNAME_ABILITY, BTLSCR_MSG_TEMP, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
	UpdateMonDataFromVar OPCODE_SET, BTLSCR_MSG_TEMP, BATTLEMON_ABILITY, BTLVAR_CALC_TEMP
	// {0} acquired {1}!
    PrintMessage pl_msg_00000368_01021, TAG_NICKNAME_ABILITY, BTLSCR_MSG_TEMP, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 60
    End 
