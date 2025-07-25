    .include "macros/btlcmd.inc"

    .data

_000:
    TryTailwind BTLSCR_ATTACKER, 4, _018
    Call BATTLE_SUBSCRIPT_ATTACK_MESSAGE_AND_ANIMATION
    // The tailwind blew from behind your team!
    PrintMessage pl_msg_00000368_01230, TAG_NONE_SIDE_CONSCIOUS, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    UpdateVar OPCODE_FLAG_ON, BTLVAR_SIDE_CONDITIONS_ATTACKER, SIDE_CONDITION_TAILWIND
	// RecalcSpeed
    End 

_018:
    UpdateVar OPCODE_FLAG_ON, BTLVAR_MOVE_STATUS_FLAGS, MOVE_STATUS_FAILED
    End 
