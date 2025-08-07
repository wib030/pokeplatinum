    .include "macros/btlcmd.inc"

    .data

_000:
	CompareMonDataToValue OPCODE_FLAG_NOT, BTLSCR_DEFENDER, BATTLEMON_STATUS, MON_CONDITION_ANY, _010
	UpdateVar OPCODE_SET, BTLVAR_POWER_MULTI, 20
	
_010:
    CalcCrit 
    CalcDamage 
    End 
