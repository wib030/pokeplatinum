#ifndef POKEPLATINUM_OV5_021E622C_H
#define POKEPLATINUM_OV5_021E622C_H

#include "string_template.h"
#include "trainer_info.h"
#include "struct_decls/struct_02026218_decl.h"
#include "struct_decls/struct_02026310_decl.h"
#include "field/field_system_decl.h"
#include "pokemon.h"
#include "struct_decls/struct_party_decl.h"
#include "savedata.h"

u8 ov5_021E6238(UnkStruct_02026310 * param0);
int ov5_021E6270(UnkStruct_02026310 * param0);
void ov5_021E6358(Party * param0, int param1, UnkStruct_02026310 * param2, SaveData * param3);
u16 ov5_021E64F8(Party * param0, StringTemplate * param1, UnkStruct_02026310 * param2, u8 param3);
int ov5_021E6520(BoxPokemon * param0, u32 param1);
int ov5_021E6568(UnkStruct_02026218 * param0);
int ov5_021E6590(UnkStruct_02026218 * param0);
u8 ov5_021E65B0(UnkStruct_02026218 * param0, StringTemplate * param1);
int ov5_021E65EC(UnkStruct_02026218 * param0, StringTemplate * param1);
int ov5_021E6630(UnkStruct_02026310 * param0, u8 param1, StringTemplate * param2);
u8 ov5_021E6640(UnkStruct_02026310 * param0, int param1, StringTemplate * param2);
void ov5_021E6720(UnkStruct_02026310 * param0);
void ov5_021E6B40(UnkStruct_02026310 * param0);
void ov5_021E6CF0(Pokemon * param0, u16 param1, u8 param2, TrainerInfo * param3, int param4, int param5);
void ov5_021E6DE8(Pokemon * param0, u16 param1, UnkStruct_02026310 * param2, u32 param3, u8 param4);
void ov5_021E6EA8(UnkStruct_02026310 * param0, Party * param1, TrainerInfo * param2);
BOOL ov5_021E7154(UnkStruct_02026310 * param0, Party * param1, FieldSystem * fieldSystem);
Pokemon * ov5_021E7278(Party * param0);
void ov5_021E72BC(UnkStruct_02026310 * param0, StringTemplate * param1);
void ov5_021E7308(UnkStruct_02026310 * param0, u32 param1, u32 param2, u32 param3, u8 param4, StringTemplate * param5);
u16 ov5_021E73A0(Party * param0, int param1, StringTemplate * param2);
u8 ov5_021E73C8(UnkStruct_02026310 * param0);
u8 ov5_021E73F0(u32 param0);
u32 ov5_021E7420(UnkStruct_02026310 * param0);
void ov5_021E771C(Pokemon * param0, int param1);
u32 ov5_021E7790(BoxPokemon ** param0);

#endif // POKEPLATINUM_OV5_021E622C_H
