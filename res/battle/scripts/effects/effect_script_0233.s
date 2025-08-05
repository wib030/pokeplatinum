    .include "macros/btlcmd.inc"

    .data

_000:
    CalcFlingParams _185
    TryFling _185
	CheckFlingEffectMon _195
    UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_FLAGS_INDIRECT, MOVE_SIDE_EFFECT_ON_HIT|MOVE_SIDE_EFFECT_TO_DEFENDER|MOVE_SUBSCRIPT_PTR_FLING
    CalcCrit 
    CalcDamage 
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 30
    // {0} flung its {1}!
    PrintMessage pl_msg_00000368_01144, TAG_NICKNAME_ITEM, BTLSCR_ATTACKER, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    RemoveItem BTLSCR_ATTACKER
    End

_185:
    UpdateVar OPCODE_FLAG_ON, BTLVAR_MOVE_STATUS_FLAGS, MOVE_STATUS_FAILED
    End
	
_195:
    UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_FLAGS_INDIRECT, MOVE_SIDE_EFFECT_ON_HIT|MOVE_SIDE_EFFECT_TO_ATTACKER|MOVE_SUBSCRIPT_PTR_FLING_BEFORE
    CalcCrit 
    CalcDamage 
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 30
    // {0} flung its {1}!
    PrintMessage pl_msg_00000368_01144, TAG_NICKNAME_ITEM, BTLSCR_ATTACKER, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    RemoveItem BTLSCR_ATTACKER
    End