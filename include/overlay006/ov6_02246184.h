#ifndef POKEPLATINUM_OV6_02246184_H
#define POKEPLATINUM_OV6_02246184_H

#include "field/field_system_decl.h"
#include "pokemon.h"
#include "overlay006/struct_ov6_02246204_decl.h"
#include "overlay006/struct_ov6_02246254.h"

UnkStruct_ov6_02246204 * ov6_02246184(u32 param0, u32 param1);
void ov6_02246204(UnkStruct_ov6_02246204 * param0);
u32 ov6_02246224(const UnkStruct_ov6_02246204 * param0);
u32 ov6_0224622C(const UnkStruct_ov6_02246204 * param0);
void ov6_02246234(FieldSystem * fieldSystem, UnkStruct_ov6_02246204 * param1, int param2);
void ov6_02246254(FieldSystem * fieldSystem, UnkStruct_ov6_02246204 * param1, int param2, UnkStruct_ov6_02246254 * param3, Pokemon * param4, Pokemon * param5);

#endif // POKEPLATINUM_OV6_02246184_H
