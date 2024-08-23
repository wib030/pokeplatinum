    .include "macros/btlcmd.inc"

    .data

_000:
    // {0}’s {1} activated!
    PrintMessage pl_msg_00000368_01279, TAG_NICKNAME_ABILITY, BTLSCR_SIDE_EFFECT_MON, BTLSCR_SIDE_EFFECT_MON
    Wait 
    WaitButtonABTime 30
	UpdateMonDataFromVar OPCODE_SET, BTLSCR_SIDE_EFFECT_MON, BATTLEMON_ABILITY, BTLVAR_CALC_TEMP
	// {0} acquired {1}!
    PrintMessage pl_msg_00000368_01021, TAG_NICKNAME_ABILITY, BTLSCR_SIDE_EFFECT_MON, BTLSCR_SIDE_EFFECT_MON
    Wait 
    WaitButtonABTime 30
    End 
