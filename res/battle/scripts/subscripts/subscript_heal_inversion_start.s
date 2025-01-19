    .include "macros/btlcmd.inc"

    .data

_000:
    CheckSubstitute BTLSCR_DEFENDER, _028
    CompareMonDataToValue OPCODE_NEQ, BTLSCR_DEFENDER, BATTLEMON_HEAL_INVERSION_TURNS, 0, _028
    Call BATTLE_SUBSCRIPT_ATTACK_MESSAGE_AND_ANIMATION
    UpdateMonData OPCODE_SET, BTLSCR_DEFENDER, BATTLEMON_HEAL_INVERSION_TURNS, 5
    // {0}'s healing was inverted!
    PrintMessage pl_msg_00000368_01333, TAG_NICKNAME, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    End 

_028:
    PrintAttackMessage 
    Wait 
    WaitButtonABTime 15
    // It failed to affect {0}!
    PrintMessage pl_msg_00000368_01235, TAG_NICKNAME, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    UpdateVar OPCODE_FLAG_ON, BTLVAR_MOVE_STATUS_FLAGS, MOVE_STATUS_NO_MORE_WORK
    End 
