    .include "macros/btlcmd.inc"

    .data

_000:
    // {0} threw mud at the {1}!
    PrintGlobalMessage pl_msg_00000368_00854, TAG_TRNAME_NICKNAME, BTLSCR_ATTACKER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
	
    UpdateVar OPCODE_FLAG_OFF, BTLVAR_BATTLE_CTX_STATUS, SYSCTL_PLAYED_MOVE_ANIMATION
	UpdateVar OPCODE_SET, BTLVAR_MSG_MOVE_TEMP, MOVE_FLING
    PlayMoveAnimationOnMons BTLSCR_MSG_TEMP, BTLSCR_PLAYER, BTLSCR_DEFENDER
	Wait
	UpdateVar OPCODE_FLAG_OFF, BTLVAR_BATTLE_CTX_STATUS, SYSCTL_PLAYED_MOVE_ANIMATION
	
	UpdateVar OPCODE_FLAG_OFF, BTLVAR_BATTLE_CTX_STATUS, SYSCTL_PLAYED_MOVE_ANIMATION
	UpdateVar OPCODE_SET, BTLVAR_MSG_MOVE_TEMP, MOVE_SELFDESTRUCT
    PlayMoveAnimationOnMons BTLSCR_MSG_TEMP, BTLSCR_DEFENDER, BTLSCR_DEFENDER
	Wait
	UpdateVar OPCODE_FLAG_OFF, BTLVAR_BATTLE_CTX_STATUS, SYSCTL_PLAYED_MOVE_ANIMATION
	
	CalcGrenadeDamage
    UpdateVarFromVar OPCODE_SET, BTLVAR_MSG_BATTLER_TEMP, BTLVAR_DEFENDER
    Call BATTLE_SUBSCRIPT_UPDATE_HP
	
	CompareMonDataToValue OPCODE_EQU, BTLSCR_DEFENDER, BATTLEMON_CUR_HP, 0, _040
	CompareMonDataToValue OPCODE_FLAG_NOT, BTLSCR_DEFENDER, BATTLEMON_STATUS, MON_CONDITION_SLEEP, _010
	// {0} woke up!
    PrintMessage pl_msg_00000368_00302, TAG_NICKNAME, BTLSCR_DEFENDER
    Wait 
    SetHealthbarStatus BTLSCR_DEFENDER, BATTLE_ANIMATION_NONE
    WaitButtonABTime 30
    UpdateMonData OPCODE_FLAG_OFF, BTLSCR_DEFENDER, BATTLEMON_STATUS, MON_CONDITION_SLEEP
    UpdateMonData OPCODE_FLAG_OFF, BTLSCR_DEFENDER, BATTLEMON_VOLATILE_STATUS, VOLATILE_CONDITION_NIGHTMARE
	ClearSleepClauseFlag BTLSCR_DEFENDER

_010:
	PlayBattleAnimation BTLSCR_ENEMY, BATTLE_ANIMATION_ANGRY
    Wait 
    CompareVarToValue OPCODE_EQU, BTLVAR_SCRIPT_TEMP, 0, _026
    // {0} is angry!
    PrintGlobalMessage pl_msg_00000368_00855, TAG_NICKNAME, BTLSCR_DEFENDER
    GoTo _030

_026:
    // {0} is beside itself with anger!
    PrintGlobalMessage pl_msg_00000368_00856, TAG_NICKNAME, BTLSCR_DEFENDER

_030:
    Wait 
    WaitButtonABTime 30
	
_040:
    End 
