    .include "macros/btlcmd.inc"

    .data

_000:
    CompareVarToValue OPCODE_FLAG_SET, BTLVAR_SIDE_CONDITIONS_DEFENDER, SIDE_CONDITION_STICKY_WEB, _017
    UpdateVar OPCODE_FLAG_ON, BTLVAR_SIDE_CONDITIONS_DEFENDER, SIDE_CONDITION_STICKY_WEB
    // A sticky web was laid out beneath your teams feet!
    BufferMessage pl_msg_00000368_01328, TAG_NONE_SIDE_CONSCIOUS, BTLSCR_ATTACKER_ENEMY
    UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_FLAGS_INDIRECT, MOVE_SIDE_EFFECT_ON_HIT|MOVE_SUBSCRIPT_PTR_PRINT_MESSAGE_AND_PLAY_ANIMATION
    End 

_017:
    UpdateVar OPCODE_FLAG_ON, BTLVAR_MOVE_STATUS_FLAGS, MOVE_STATUS_FAILED
    End 
