    .include "macros/btlcmd.inc"

    .data

_000:
    TryFeint _008
    UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_FLAGS_INDIRECT, MOVE_SIDE_EFFECT_ON_HIT|MOVE_SUBSCRIPT_PTR_FEINT
	UpdateVar OPCODE_SET, BTLVAR_POWER_MULTI, 20
    CalcCrit 
    CalcDamage 
    End 

_008:
    CalcCrit 
    CalcDamage
    End 
