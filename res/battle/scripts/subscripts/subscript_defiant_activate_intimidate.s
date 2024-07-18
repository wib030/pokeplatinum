    .include "macros/btlcmd.inc"

    .data

_000:
	CompareMonDataToValue OPCODE_NEQ, BTLSCR_SIDE_EFFECT_MON, BATTLEMON_ATTACK_STAGE, 12, _011
	CompareMonDataToValue OPCODE_EQU, BTLSCR_SIDE_EFFECT_MON, BATTLEMON_ATTACK_STAGE, 12, _046

_011:
    // {0}’s {1} activated!
    PrintMessage pl_msg_00000368_01279, TAG_NICKNAME_ABILITY_STAT, BTLSCR_SIDE_EFFECT_MON, BTLSCR_SIDE_EFFECT_MON, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
	
	UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_PARAM, MOVE_SUBSCRIPT_PTR_ATTACK_UP_2_STAGES
    UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_TYPE, SIDE_EFFECT_TYPE_INDIRECT
    Call BATTLE_SUBSCRIPT_UPDATE_STAT_STAGE
	End

_046:
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 30
    // {0}’s stats won’t go any higher!
    PrintMessage pl_msg_00000368_00768, TAG_NICKNAME, BTLSCR_SIDE_EFFECT_MON
    Wait 
    WaitButtonABTime 30
    End
