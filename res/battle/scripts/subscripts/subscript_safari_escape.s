    .include "macros/btlcmd.inc"

    .data

_000:
	CompareMonDataToValue OPCODE_EQU, BTLSCR_DEFENDER, BATTLEMON_CUR_HP, 0, _010
	CompareMonDataToValue OPCODE_FLAG_SET, BTLSCR_DEFENDER, BATTLEMON_STATUS, MON_CONDITION_SLEEP, _015
    // {0} is watching carefully!
    PrintGlobalMessage pl_msg_00000368_00849, TAG_NICKNAME, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
	
_010:
	End

_015:
	// {0} is fast asleep.
    PrintMessage pl_msg_00000368_00299, TAG_NICKNAME, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    PlayBattleAnimation BTLSCR_DEFENDER, BATTLE_ANIMATION_ASLEEP
    Wait 
    End 