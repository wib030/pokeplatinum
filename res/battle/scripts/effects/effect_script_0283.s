    .include "macros/btlcmd.inc"

    .data

_000:
    UpdateVar OPCODE_SET, BTLVAR_SIDE_EFFECT_FLAGS_INDIRECT, MOVE_SIDE_EFFECT_ON_HIT|MOVE_SUBSCRIPT_PTR_LIGHT_SCREEN_HIT
	CalcCrit
	CalcDamage
    End 
