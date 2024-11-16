    .include "macros/btlcmd.inc"

    .data

_000:
    CompareMonDataToValue OPCODE_EQU, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_UNOWN_ENERGY_STRONG_FLAG, 1, _010
	CompareMonDataToValue OPCODE_EQU, BTLSCR_MSG_BATTLER_TEMP, BATTLEMON_UNOWN_ENERGY_WEAK_FLAG, 1, _020
	GoTo _030
	
_010:
    // {0}'s {1} increased the incoming damage!
    PrintMessage pl_msg_00000368_01325, TAG_NICKNAME_ABILITY, BTLSCR_MSG_BATTLER_TEMP, BTLSCR_MSG_BATTLER_TEMP
    Wait 
    WaitButtonABTime 30
	GoTo _030

_020:
    // {0}'s {1} reduced the incoming damage!
    PrintMessage pl_msg_00000368_01322, TAG_NICKNAME_ABILITY, BTLSCR_MSG_BATTLER_TEMP, BTLSCR_MSG_BATTLER_TEMP
    Wait 
    WaitButtonABTime 30

_030:
    End 
