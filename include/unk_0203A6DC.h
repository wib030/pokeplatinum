#ifndef POKEPLATINUM_UNK_0203A6DC_H
#define POKEPLATINUM_UNK_0203A6DC_H

#include "struct_decls/struct_0203A790_decl.h"
#include "struct_decls/struct_0203A79C_decl.h"
#include "field/field_system_decl.h"
#include "struct_defs/struct_02049FA8.h"
#include "struct_defs/struct_020556C4.h"
#include "struct_defs/struct_0205EC34.h"
#include "savedata.h"

int FieldPlayerState_SaveSize(void);
int FieldOWState_SaveSize(void);
void FieldOWState_Init(UnkStruct_0203A79C * param0);
void FieldPlayerState_Init(UnkStruct_0203A790 * param0);
Location * sub_0203A720(UnkStruct_0203A790 * param0);
Location * sub_0203A724(UnkStruct_0203A790 * param0);
Location * sub_0203A728(UnkStruct_0203A790 * param0);
Location * sub_0203A72C(UnkStruct_0203A790 * param0);
Location * sub_0203A730(UnkStruct_0203A790 * param0);
void sub_0203A734(UnkStruct_0203A790 * param0, Location * param1);
u16 * sub_0203A748(UnkStruct_0203A790 * param0);
u16 sub_0203A74C(const UnkStruct_0203A790 * param0);
void sub_0203A754(UnkStruct_0203A790 * param0, u16 param1);
u16 sub_0203A75C(const UnkStruct_0203A790 * param0);
void sub_0203A764(UnkStruct_0203A790 * param0, u16 param1);
UnkStruct_020556C4 * sub_0203A76C(UnkStruct_0203A790 * param0);
int sub_0203A770(const UnkStruct_0203A790 * param0);
void sub_0203A778(UnkStruct_0203A790 * param0, int param1);
PlayerData * sub_0203A780(UnkStruct_0203A790 * param0);
u16 * sub_0203A784(UnkStruct_0203A790 * param0);
u16 * sub_0203A788(UnkStruct_0203A790 * param0);
u16 * sub_0203A78C(UnkStruct_0203A790 * param0);
UnkStruct_0203A790 * sub_0203A790(SaveData * param0);
UnkStruct_0203A79C * sub_0203A79C(SaveData * param0);
void sub_0203A7A8(FieldSystem * fieldSystem);
void sub_0203A7C0(FieldSystem * fieldSystem);

#endif // POKEPLATINUM_UNK_0203A6DC_H
