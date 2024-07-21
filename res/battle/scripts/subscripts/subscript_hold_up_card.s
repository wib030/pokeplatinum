    .include "macros/btlcmd.inc"

    .data

_000:
	PlayBattleAnimation BTLSCR_DEFENDER, BATTLE_ANIMATION_HELD_ITEM
    Wait 
    WaitButtonABTime 15
	// {0}’s {1} activated!
    PrintMessage pl_msg_00000368_01279, TAG_NICKNAME_ITEM, BTLSCR_DEFENDER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
	RemoveItem BTLSCR_DEFENDER
	UpdateVarFromVar OPCODE_SET, BTLVAR_DEFENDER, BTLVAR_ATTACKER
	Call BATTLE_SUBSCRIPT_FORCE_TARGET_TO_SWITCH_OR_FLEE
	End
