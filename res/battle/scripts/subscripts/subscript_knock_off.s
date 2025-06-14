    .include "macros/btlcmd.inc"

    .data

_000:
    CheckSubstitute BTLSCR_DEFENDER, _032
    CompareMonDataToValue OPCODE_EQU, BTLSCR_DEFENDER, BATTLEMON_ABILITY, ABILITY_MULTITYPE, _032
    CompareMonDataToValue OPCODE_EQU, BTLSCR_DEFENDER, BATTLEMON_HELD_ITEM, ITEM_GRISEOUS_ORB, _032
    CompareMonDataToValue OPCODE_NEQ, BTLSCR_DEFENDER, BATTLEMON_QUICK_CLAW, 0, _032
    CompareMonDataToValue OPCODE_NEQ, BTLSCR_DEFENDER, BATTLEMON_CUSTAP_BERRY, 0, _032
	CompareMonDataToValue OPCODE_EQU, BTLSCR_DEFENDER, BATTLEMON_HELD_ITEM, ITEM_TOXIC_ORB, _052
    TryKnockOff _032
    PrintBufferedMessage 
    Wait 
    WaitButtonABTime 30

_032:
    End 
	
_052:
    TryKnockOff _032
    PrintBufferedMessage
	Wait
	WaitButtonABTime 30
	CompareMonDataToValue OPCODE_FLAG_NOT, BTLSCR_DEFENDER, BATTLEMON_STATUS, MON_CONDITION_ANY_POISON, _032
    // {0}’s poisoning faded away.
    PrintMessage pl_msg_00000368_01340, TAG_NICKNAME, BTLSCR_DEFENDER
    UpdateMonData OPCODE_SET, BTLSCR_DEFENDER, BATTLEMON_STATUS, MON_CONDITION_NONE
	SetHealthbarStatus BTLSCR_DEFENDER, BATTLE_ANIMATION_NONE
    Wait 
    WaitButtonABTime 30
	End
