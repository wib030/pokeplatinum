    .include "macros/btlcmd.inc"

    .data

_000:
	CompareMonDataToValue OPCODE_EQU, BTLVAR_DEFENDER, BATTLEMON_CUR_HP, 0, _010
    // {0} is watching carefully!
    PrintGlobalMessage pl_msg_00000368_00849, TAG_NICKNAME, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
	
_010:
	End