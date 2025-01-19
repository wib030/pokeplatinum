    .include "macros/btlcmd.inc"

    .data

_000:
    PlayBattleAnimation BTLSCR_MSG_TEMP, BATTLE_ANIMATION_HELD_ITEM
    Wait 
    UpdateVar OPCODE_FLAG_ON, BTLVAR_BATTLE_CTX_STATUS, SYSCTL_SKIP_SPRITE_BLINK
	CompareMonDataToValue OPCODE_NEQ, BTLSCR_MSG_TEMP, BATTLEMON_HEAL_INVERSION_TURNS, 0, _010
    Call BATTLE_SUBSCRIPT_UPDATE_HP
    // {0} restored its health using its {1}!
    PrintMessage pl_msg_00000368_00899, TAG_NICKNAME_ITEM, BTLSCR_MSG_TEMP, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    Call BATTLE_SUBSCRIPT_PLUCK_CHECK
    End
	
_010:
	UpdateVar OPCODE_MUL, BTLVAR_HP_CALC_TEMP, -1
	Call BATTLE_SUBSCRIPT_UPDATE_HP
    // {0} restored its health using its {1}!
    PrintMessage pl_msg_00000368_00899, TAG_NICKNAME_ITEM, BTLSCR_MSG_TEMP, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    Call BATTLE_SUBSCRIPT_PLUCK_CHECK
    End
