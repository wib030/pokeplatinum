    .include "macros/btlcmd.inc"

    .data

_000:
	CompareVarToValue OPCODE_FLAG_SET, BTLVAR_SIDE_CONDITIONS_DEFENDER, SIDE_CONDITION_DEEP_SNOW, _018
    Call BATTLE_SUBSCRIPT_ATTACK_MESSAGE_AND_ANIMATION
	// {0}’s {1} activated!
    PrintMessage pl_msg_00000368_01279, TAG_NICKNAME_ABILITY, BTLSCR_ATTACKER, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    // Snow piled up around the foe's team!
    PrintMessage pl_msg_00000368_01336, TAG_NONE_SIDE_CONSCIOUS, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    UpdateVar OPCODE_FLAG_ON, BTLVAR_SIDE_CONDITIONS_DEFENDER, SIDE_CONDITION_DEEP_SNOW
    End 

_018:
    End 
