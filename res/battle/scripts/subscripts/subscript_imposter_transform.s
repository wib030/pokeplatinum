    .include "macros/btlcmd.inc"

    .data

_000:
    CompareMonDataToValue OPCODE_FLAG_SET, BTLSCR_DEFENDER, BATTLEMON_VOLATILE_STATUS, VOLATILE_CONDITION_TRANSFORM, _023
	CheckSubstitute BTLSCR_DEFENDER, _023
	UpdateVar OPCODE_SET, BTLVAR_MSG_MOVE_TEMP, MOVE_TRANSFORM
    PlayMoveAnimation BTLSCR_MSG_TEMP
	UpdateVar OPCODE_FLAG_OFF, BTLVAR_BATTLE_CTX_STATUS, SYSCTL_PLAYED_MOVE_ANIMATION
    Transform 
    // {0} transformed into {1}!
    PrintMessage pl_msg_00000368_00345, TAG_NICKNAME_POKE, BTLSCR_ATTACKER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    End 

_023:
    End 
