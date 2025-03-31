    .include "macros/btlcmd.inc"

    .data

_000:
    // The snow around your team was cleared!
	UpdateVar OPCODE_FLAG_OFF, BTLVAR_SIDE_CONDITIONS_ATTACKER, SIDE_CONDITION_DEEP_SNOW
    PrintMessage pl_msg_00000368_01338, TAG_NONE_SIDE_CONSCIOUS, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    End 
