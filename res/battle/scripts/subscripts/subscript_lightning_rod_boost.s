    .include "macros/btlcmd.inc"

    .data

_000:
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 15
	
	CompareMonDataToValue OPCODE_EQU, BTLSCR_DEFENDER, BATTLEMON_SP_ATTACK_STAGE, 12, _032
    PlayBattleAnimation BTLSCR_DEFENDER, BATTLE_ANIMATION_STAT_BOOST
    Wait 
    UpdateMonData OPCODE_ADD, BTLSCR_DEFENDER, BATTLEMON_SP_ATTACK_STAGE, 1
    UpdateVar OPCODE_SET, BTLVAR_MSG_TEMP, 4
    // {0}’s {1} raised its {2}!
    PrintMessage pl_msg_00000368_00622, TAG_NICKNAME_ABILITY_STAT, BTLSCR_DEFENDER, BTLSCR_DEFENDER, BTLSCR_MSG_TEMP
	Wait 
    WaitButtonABTime 30
    End
	
_032:
	// {0}’s {1} took the attack!
    // PrintMessage pl_msg_00000368_00724, TAG_NICKNAME_ABILITY, BTLSCR_DEFENDER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
	End