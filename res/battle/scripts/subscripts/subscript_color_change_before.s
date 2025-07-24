    .include "macros/btlcmd.inc"

    .data

_000:
	PlaySound BTLSCR_DEFENDER, 1980
    SetMosaic BTLSCR_DEFENDER, 8, 1
    Wait 
    PlaySound BTLSCR_DEFENDER, 1984
    SetMosaic BTLSCR_DEFENDER, 0, 1
    Wait 
    // {0}’s {1} made it the {2} type!
    PrintMessage pl_msg_00000368_00641, TAG_NICKNAME_ABILITY_TYPE, BTLSCR_DEFENDER, BTLSCR_DEFENDER, BTLSCR_MSG_TEMP
    Wait 
    WaitButtonABTime 30
    End
