    .include "macros/btlcmd.inc"

    .data

_000:
    TryPluck _024, _023
	// {0}’s {1} activated!
    PrintMessage pl_msg_00000368_01279, TAG_NICKNAME_ABILITY, BTLSCR_DEFENDER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    // {0} stole and ate its foe’s {1}!
    PrintMessage pl_msg_00000368_01141, TAG_NICKNAME_ITEM, BTLSCR_DEFENDER, BTLSCR_ATTACKER
    Wait 
    WaitButtonABTime 30
    RemoveItem BTLSCR_ATTACKER
    UpdateVarFromVar OPCODE_SET, BTLVAR_MSG_BATTLER_TEMP, BTLVAR_DEFENDER
    CompareVarToValue OPCODE_EQU, BTLVAR_SCRIPT_TEMP, 0, _023
    CallFromVar BTLVAR_SCRIPT_TEMP

_023:
    End 

_024:
    // {0}’s {1} made {2} ineffective!
    PrintMessage pl_msg_00000368_00714, TAG_NICKNAME_ABILITY_MOVE, BTLSCR_ATTACKER, BTLSCR_ATTACKER, BTLSCR_DEFENDER
    Wait 
    WaitButtonABTime 30
    End 