    .include "macros/btlcmd.inc"

    .data

_000:
    CompareMonDataToValue OPCODE_NEQ, BTLSCR_ATTACKER, BATTLEMON_HEAL_BLOCK_TURNS, 0, _028
    UpdateMonData OPCODE_FLAG_ON, BTLSCR_ATTACKER, BATTLEMON_MOVE_EFFECTS_MASK, MOVE_EFFECT_HEAL_BLOCK
    UpdateMonData OPCODE_SET, BTLSCR_ATTACKER, BATTLEMON_HEAL_BLOCK_TURNS, 5
    // {0} was prevented from healing!
    PrintMessage pl_msg_00000368_01051, TAG_NICKNAME, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    End 

_028:
    End 
