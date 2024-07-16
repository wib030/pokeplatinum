#ifndef POKEPLATINUM_UNK_0201E86C_H
#define POKEPLATINUM_UNK_0201E86C_H

#include "struct_defs/struct_0200A328.h"
#include "struct_defs/sprite_manager_allocation.h"
#include "overlay022/struct_ov22_022559F8.h"

#include <nitro/gx.h>
#include <nnsys.h>

void sub_0201E86C(const UnkStruct_ov22_022559F8 * param0);
void sub_0201E88C(const UnkStruct_ov22_022559F8 * param0, GXOBJVRamModeChar param1, GXOBJVRamModeChar param2);
void sub_0201E958(void);
void sub_0201E994(void);
void sub_0201E9C0(u32 param0, u32 param1, u32 param2);
BOOL sub_0201EA24(const UnkStruct_0200A328 * param0);
BOOL sub_0201EA7C(const UnkStruct_0200A328 * param0);
BOOL sub_0201EAD8(int param0);
void sub_0201EB08(int param0, NNSG2dCharacterData * param1);
void sub_0201EB50(int param0);
void sub_0201EBA0(void);
NNSG2dImageProxy * sub_0201EBDC(int param0);
NNSG2dImageProxy * sub_0201EC00(int param0, u32 param1);
NNSG2dImageProxy * sub_0201EC84(const NNSG2dImageProxy * param0);
void sub_0201ED1C(const NNSG2dImageProxy * param0);
BOOL sub_0201ED94(int param0, int param1, int param2, SpriteManagerAllocation * param3);
void sub_0201EE28(SpriteManagerAllocation * param0);
void * sub_0201EE9C(void);
void sub_0201EEB8(void * param0);
void sub_0201F460(void);
int sub_0201F6F4(int param0);

#endif // POKEPLATINUM_UNK_0201E86C_H
