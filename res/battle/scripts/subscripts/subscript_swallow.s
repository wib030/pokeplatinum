    .include "macros/btlcmd.inc"

    .data

_000:
    PrintAttackMessage 
    Wait
	CompareMonDataToValue OPCODE_NEQ, BTLSCR_ATTACKER, BATTLEMON_HEAL_INVERSION_TURNS, 0, _010
    Call BATTLE_SUBSCRIPT_RECOVER_HP
    // {0}’s stockpiled effect wore off!
    PrintMessage pl_msg_00000368_00994, TAG_NICKNAME, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    End
	
_010:
    UpdateVar OPCODE_MUL, BTLVAR_HP_CALC_TEMP, -1
    Call BATTLE_SUBSCRIPT_RECOVER_HP
    // {0}’s stockpiled effect wore off!
    PrintMessage pl_msg_00000368_00994, TAG_NICKNAME, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    End
