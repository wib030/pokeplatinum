#include <nitro.h>
#include <string.h>

#include "strbuf.h"
#include "trainer_info.h"
#include "struct_decls/struct_02026218_decl.h"
#include "struct_decls/struct_02026224_decl.h"
#include "struct_decls/struct_02026310_decl.h"
#include "struct_defs/chatot_cry.h"
#include "struct_decls/struct_0202CD88_decl.h"
#include "pokemon.h"
#include "struct_decls/struct_party_decl.h"
#include "savedata.h"

#include "field/field_system.h"

#include "message.h"
#include "message_util.h"
#include "string_template.h"
#include "unk_02017038.h"
#include "heap.h"
#include "unk_0201D15C.h"
#include "strbuf.h"
#include "unk_02025E08.h"
#include "trainer_info.h"
#include "unk_020261E4.h"
#include "unk_0202CC64.h"
#include "unk_0202CD50.h"
#include "unk_020559DC.h"
#include "constants/species.h"
#include "pokemon.h"
#include "party.h"
#include "item.h"
#include "unk_02092494.h"
#include "overlay005/ov5_021E622C.h"

#include "overlay005/rodata_ov5_021F9FA2.h"

typedef struct {
    int unk_00[4];
    int unk_10[4];
    int unk_20[4];
    u16 unk_30[50];
    u16 unk_94[16];
} UnkStruct_ov5_021E6948;

void ov5_021E6DE8(Pokemon * param0, u16 param1, UnkStruct_02026310 * param2, u32 param3, u8 param4);
void sub_020262C0(UnkStruct_02026224 * param0);
static u8 ov5_021E70FC(UnkStruct_02026310 * param0);
static int ov5_021E6F6C(Party * param0);
static u8 ov5_021E6FF0(BoxPokemon ** param0);
void ov5_021E6B40(UnkStruct_02026310 * param0);
int ov5_021E6630(UnkStruct_02026310 * param0, u8 param1, StringTemplate * param2);
u8 ov5_021E6640(UnkStruct_02026310 * param0, int param1, StringTemplate * param2);
u16 ov5_021E73A0(Party * param0, int param1, StringTemplate * param2);
u8 ov5_021E73C8(UnkStruct_02026310 * param0);
void ov5_021E72BC(UnkStruct_02026310 * param0, StringTemplate * param1);
static void ov5_021E62C4(Party * param0, int param1, UnkStruct_02026218 * param2, SaveData * param3);
static int ov5_021E7110(FieldSystem * fieldSystem);

static BoxPokemon * ov5_021E622C (UnkStruct_02026310 * param0, int param1)
{
    return sub_02026220(sub_02026218(param0, param1));
}

static UnkStruct_02026310 * Unk_ov5_02202124;

u8 ov5_021E6238 (UnkStruct_02026310 * param0)
{
    u8 v0, v1;
    BoxPokemon * v2;

    v0 = 0;

    for (v1 = 0; v1 < 2; v1++) {
        v2 = sub_02026220(sub_02026218(param0, v1));

        if (BoxPokemon_GetValue(v2, MON_DATA_SPECIES, NULL) != 0) {
            v0++;
        }
    }

    return v0;
}

int ov5_021E6270 (UnkStruct_02026310 * param0)
{
    u8 v0;
    BoxPokemon * v1;

    Unk_ov5_02202124 = param0;

    for (v0 = 0; v0 < 2; v0++) {
        v1 = sub_02026220(sub_02026218(param0, v0));

        if (BoxPokemon_GetValue(v1, MON_DATA_SPECIES, NULL) == 0) {
            return v0;
        }
    }

    return -1;
}

static int ov5_021E62B0 (BoxPokemon * param0)
{
    int v0 = BoxPokemon_GetValue(param0, MON_DATA_HELD_ITEM, NULL);
    return Item_IsMail(v0);
}

static void ov5_021E62C4 (Party * param0, int param1, UnkStruct_02026218 * param2, SaveData * param3)
{
    int v0;
    Pokemon * v1 = Party_GetPokemonBySlotIndex(param0, param1);
    const u16 * v2;
    u16 v3[10 + 1];
    UnkStruct_02026224 * v4 = sub_02026224(param2);
    BoxPokemon * v5 = sub_02026220(param2);
    TrainerInfo * v6 = SaveData_GetTrainerInfo(param3);

    v2 = TrainerInfo_Name(v6);
    Pokemon_GetValue(v1, MON_DATA_NICKNAME, v3);

    if (ov5_021E62B0(Pokemon_GetBoxPokemon(v1))) {
        Pokemon_GetValue(v1, MON_DATA_170, sub_02026230(v4));
    }

    BoxPokemon_FromPokemon(v1, v5);
    BoxPokemon_SetShayminForm(v5, 0);
    sub_02026258(param2, 0);
    Party_RemovePokemonBySlotIndex(param0, param1);

    if (Party_HasSpecies(param0, 441) == 0) {
        ChatotCry * v7 = GetChatotCryDataFromSave(param3);
        ResetChatotCryDataStatus(v7);
    }
}

void ov5_021E6358 (Party * param0, int param1, UnkStruct_02026310 * param2, SaveData * param3)
{
    int v0;
    UnkStruct_0202CD88 * v1 = sub_0202CD88(param3);

    sub_0202CF28(v1, 1 + 39);
    v0 = ov5_021E6270(param2);
    ov5_021E62C4(param0, param1, sub_02026218(param2, v0), param3);
}

static void ov5_021E638C (UnkStruct_02026310 * param0)
{
    UnkStruct_02026218 * v0, * v1;
    BoxPokemon * v2, * v3;

    v0 = sub_02026218(param0, 0);
    v1 = sub_02026218(param0, 1);
    v2 = sub_02026220(v0);
    v3 = sub_02026220(v1);

    if (BoxPokemon_GetValue(v2, MON_DATA_SPECIES, NULL) == 0) {
        if (BoxPokemon_GetValue(v3, MON_DATA_SPECIES, NULL) != 0) {
            sub_020262A8(v0, v1);
            sub_020262F4(v1);
        }
    }
}

static void ov5_021E63E0 (Pokemon * param0)
{
    int v0, v1 = 0, v2;
    u16 v3;
    u16 v4;

    for (v0 = 0; v0 < 100; v0++) {
        if (Pokemon_ShouldLevelUp(param0)) {
            v1 = 0;

            while ((v4 = Pokemon_LevelUpMove(param0, &v1, &v3)) != 0) {
                if (v4 == 0xffff) {
                    Pokemon_ReplaceMove(param0, v3);
                }
            }
        } else {
            break;
        }
    }

    Pokemon_CalcLevelAndStats(param0);
}

static int ov5_021E6444 (Party * param0, UnkStruct_02026218 * param1, StringTemplate * param2)
{
    Pokemon * v0 = Pokemon_New(4);
    BoxPokemon * v1 = sub_02026220(param1);
    UnkStruct_02026224 * v2 = sub_02026224(param1);
    u32 v3; // exp
    u16 v4; // species number

    StringTemplate_SetNickname(param2, 0, v1);
    v4 = BoxPokemon_GetValue(v1, MON_DATA_SPECIES, NULL);
    Pokemon_FromBoxPokemon(v1, v0);

    if (Pokemon_GetValue(v0, MON_DATA_LEVEL, NULL) != 100) {
        v3 = Pokemon_GetValue(v0, MON_DATA_EXP, NULL);
        v3 += sub_02026228(param1);
        Pokemon_SetValue(v0, 8, (u8 *)&v3);
        ov5_021E63E0(v0);
    }

    if (ov5_021E62B0(v1)) {
        Pokemon_SetValue(v0, 170, sub_02026230(v2));
    }

    Party_AddPokemon(param0, v0);
    BoxPokemon_Init(v1);
    sub_02026258(param1, 0);
    Heap_FreeToHeap(v0);

    return v4;
}

u16 ov5_021E64F8 (Party * param0, StringTemplate * param1, UnkStruct_02026310 * param2, u8 param3)
{
    u16 v0;
    UnkStruct_02026218 * v1 = sub_02026218(param2, param3);

    v0 = ov5_021E6444(param0, v1, param1);
    ov5_021E638C(param2);

    return v0;
}

int ov5_021E6520 (BoxPokemon * param0, u32 param1)
{
    Pokemon * v0 = Pokemon_New(4);
    BoxPokemon * v1 = Pokemon_GetBoxPokemon(v0);
    int v2;
    u32 v3;

    BoxPokemon_Copy(param0, v1);

    v3 = BoxPokemon_GetValue(v1, MON_DATA_EXP, NULL);
    v3 += param1;

    BoxPokemon_SetValue(v1, 8, (u8 *)&v3);
    v2 = BoxPokemon_GetLevel(v1);
    Heap_FreeToHeap(v0);

    return v2;
}

int ov5_021E6568 (UnkStruct_02026218 * param0)
{
    u8 v0, v1;
    BoxPokemon * v2;

    v2 = sub_02026220(param0);
    v0 = BoxPokemon_GetLevel(v2);
    v1 = ov5_021E6520(v2, sub_02026228(param0));

    return v1 - v0;
}

int ov5_021E6590 (UnkStruct_02026218 * param0)
{
    u8 v0;
    BoxPokemon * v1;

    v1 = sub_02026220(param0);
    v0 = ov5_021E6520(v1, sub_02026228(param0));

    return v0;
}

u8 ov5_021E65B0 (UnkStruct_02026218 * param0, StringTemplate * param1)
{
    int v0;
    Strbuf* v1;
    u16 v2[10 + 1];
    BoxPokemon * v3 = sub_02026220(param0);

    v0 = ov5_021E6568(param0);

    StringTemplate_SetNumber(param1, 1, v0, 3, 0, 1);
    StringTemplate_SetNickname(param1, 0, v3);

    return v0;
}

int ov5_021E65EC (UnkStruct_02026218 * param0, StringTemplate * param1)
{
    u16 v0;
    BoxPokemon * v1 = sub_02026220(param0);

    v0 = ov5_021E6568(param0);
    StringTemplate_SetNickname(param1, 0, v1);

    v0 = v0 * 100 + 100;
    StringTemplate_SetNumber(param1, 1, v0, 5, 0, 1);

    return v0;
}

int ov5_021E6630 (UnkStruct_02026310 * param0, u8 param1, StringTemplate * param2)
{
    UnkStruct_02026218 * v0;

    v0 = sub_02026218(param0, param1);
    return ov5_021E65EC(v0, param2);
}

u8 ov5_021E6640 (UnkStruct_02026310 * param0, int param1, StringTemplate * param2)
{
    UnkStruct_02026218 * v0 = sub_02026218(param0, param1);
    BoxPokemon * v1 = sub_02026220(v0);

    if (BoxPokemon_GetValue(v1, MON_DATA_SPECIES, NULL)) {
        return ov5_021E65B0(v0, param2);
    }

    return 0;
}

static void ov5_021E6668 (UnkStruct_02026310 * param0, BoxPokemon * param1[])
{
    param1[0] = ov5_021E622C(param0, 0);
    param1[1] = ov5_021E622C(param0, 1);
}

static int ov5_021E6684 (UnkStruct_02026310 * param0)
{
    int v0;
    int v1[2], v2 = -1, v3, mateLevel[2];
    BoxPokemon * v4[2];

    ov5_021E6668(param0, v4);

    for (v0 = 0; v0 < 2; v0++) {
        if (BoxPokemon_GetGender(v4[v0]) == 1) {
            v2 = v0;
        }
    }
	
	if (BoxPokemon_GetGender(v4[0]) == BoxPokemon_GetGender(v4[1]))
	{
        mateLevel[0] = BoxPokemon_GetLevel(v4[0]);
		mateLevel[1] = BoxPokemon_GetLevel(v4[1]);
		
		if (mateLevel[0] < mateLevel[1])
		{
			v2 = 0;
		}
		else
		{
			v2 = 1;
		}
		
		if (mateLevel[0] == mateLevel[1])
		{
			if (LCRNG_Next() >= (0xffff / 2))
			{
				v2 = 0;
			}
			else
			{
				v2 = 1;
			}
		}
    }

    for (v3 = 0, v0 = 0; v0 < 2; v0++) {
        if ((v1[v0] = BoxPokemon_GetValue(v4[v0], MON_DATA_SPECIES, NULL)) == 132) {
            v3++;
            v2 = v0;
        }
    }

    if (v3 == 2) {
        if (LCRNG_Next() >= (0xffff / 2)) {
            v2 = 0;
        } else {
            v2 = 1;
        }
        for (v0 = 0; v0 < 2; v0++) { // 229 == ITEM_EVERSTONE
            if (BoxPokemon_GetValue(v4[v0], MON_DATA_HELD_ITEM, NULL) == 229) {
                v2 = v0;
            }
        }
    }
    // 229 == ITEM_EVERSTONE
    if (BoxPokemon_GetValue(v4[v2], MON_DATA_HELD_ITEM, NULL) == 229) {
        //Don't return -1
    } else {
        return -1;
    }

    return v2;
}

void ov5_021E6720 (UnkStruct_02026310 * param0)
{
    u32 v0 = 0, v1;
    int v2, v3;
    int v4 = 0;

    if ((v2 = ov5_021E6684(param0)) < 0) {
        sub_02026270(param0, MTRNG_Next());
    } else {
        BoxPokemon * v5 = ov5_021E622C(param0, v2);

        v0 = BoxPokemon_GetValue(v5, MON_DATA_PERSONALITY, NULL);
        v3 = Pokemon_GetNatureOf(v0);

        while (TRUE) {
            v1 = MTRNG_Next();

            if ((v3 == Pokemon_GetNatureOf(v1)) && (v1 != 0)) {
                break;
            }

            if (++v4 > 2400) {
                break;
            }
        }

        sub_02026270(param0, v1);
    }
}

static void ov5_021E6778 (u8 * param0, u8 param1)
{
    int v0, v1;
    u8 v2[6];

    param0[param1] = 0xff;

    for (v0 = 0; v0 < 6; v0++) {
        v2[v0] = param0[v0];
    }

    v1 = 0;

    for (v0 = 0; v0 < 6; v0++) {
        if (v2[v0] != 0xff) {
            param0[v1++] = v2[v0];
        }
    }
}

static void ov5_021E67B0 (Pokemon * param0, UnkStruct_02026310 * param1)
{
    u8 v0[3], v1, v2[6], v3[3], v4;

    for (v1 = 0; v1 < 6; v1++) {
        v2[v1] = v1;
    }

    for (v1 = 0; v1 < 3; v1++) {
        v0[v1] = v2[LCRNG_Next() % (6 - v1)];
        ov5_021E6778(v2, v1);
    }

    for (v1 = 0; v1 < 3; v1++) {
        v3[v1] = LCRNG_Next() % 2;
    }

    for (v1 = 0; v1 < 3; v1++) {
        BoxPokemon * v5 = ov5_021E622C(param1, v3[v1]);

        switch (v0[v1]) {
        case 0:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_HP_IV, NULL);
            Pokemon_SetValue(param0, 70, (u8 *)&v4);
            break;
        case 1:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_ATK_IV, NULL);
            Pokemon_SetValue(param0, 71, (u8 *)&v4);
            break;
        case 2:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_DEF_IV, NULL);
            Pokemon_SetValue(param0, 72, (u8 *)&v4);
            break;
        case 3:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_SPEED_IV, NULL);
            Pokemon_SetValue(param0, 73, (u8 *)&v4);
            break;
        case 4:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_SPATK_IV, NULL);
            Pokemon_SetValue(param0, 74, (u8 *)&v4);
            break;
        case 5:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_SPDEF_IV, NULL);
            Pokemon_SetValue(param0, 75, (u8 *)&v4);
            break;
        }
    }
}

static u8 ov5_021E68D8 (Pokemon * param0, u16 * param1)
{
    u16 v0, v1, v2, v3;
	int eggMovesSize = sizeof(EggMove_Table);

    v2 = 0;
    v1 = 0;
    v0 = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);

    for (v3 = 0; v3 < eggMovesSize; v3++) {
        if (EggMove_Table[v3] == (20000 + v0)) {
            v1 = v3 + 1;
            break;
        }
    }

    for (v3 = 0; v3 < 16; v3++) {
        if (EggMove_Table[v1 + v3] > 20000) {
            break;
        } else {
            param1[v3] = EggMove_Table[v1 + v3];
            v2++;
        }
    }

    return v2;
}

static void ov5_021E6948 (Pokemon * param0, BoxPokemon * param1, BoxPokemon * param2)
{
    u16 v0, v1, v2, v3, v4, v5, v6;
    UnkStruct_ov5_021E6948 * v7 = Heap_AllocFromHeap(4, sizeof(UnkStruct_ov5_021E6948));

    v2 = 0;

    MI_CpuClearFast(v7, sizeof(UnkStruct_ov5_021E6948));

    v3 = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);
    v6 = Pokemon_GetValue(param0, MON_DATA_FORM, NULL);
    v4 = Pokemon_LoadLevelUpMoveIdsOf(v3, v6, v7->unk_30);

    for (v0 = 0; v0 < 4; v0++) {
        v7->unk_00[v0] = BoxPokemon_GetValue(param1, 54 + v0, NULL);
        v7->unk_20[v0] = BoxPokemon_GetValue(param2, 54 + v0, NULL);
    }

    v5 = ov5_021E68D8(param0, v7->unk_94);

    for (v0 = 0; v0 < 4; v0++) {
        if (v7->unk_00[v0] != 0) {
            for (v1 = 0; v1 < v5; v1++) {
                if (v7->unk_00[v0] == v7->unk_94[v1]) {
                    if (Pokemon_AddMove(param0, v7->unk_00[v0]) == 0xffff) {
                        Pokemon_ReplaceMove(param0, v7->unk_00[v0]);
                    }
                    break;
                }
            }
        } else {
            break;
        }
    }

    for (v0 = 0; v0 < 4; v0++) {
        if (v7->unk_00[v0] != 0) {
            for (v1 = 0; v1 < 100; v1++) {
                if (v7->unk_00[v0] == Item_MoveForTMHM(328 + v1)) {
                    if (CanPokemonFormLearnTM(v3, v6, v1)) {
                        if (Pokemon_AddMove(param0, v7->unk_00[v0]) == 0xffff) {
                            Pokemon_ReplaceMove(param0, v7->unk_00[v0]);
                        }
                    }
                }
            }
        }
    }

    for (v0 = 0; v0 < 4; v0++) {
        if (v7->unk_00[v0] == 0) {
            break;
        }

        for (v1 = 0; v1 < 4; v1++) {
            if ((v7->unk_00[v0] == v7->unk_20[v1]) && (v7->unk_00[v0] != 0)) {
                v7->unk_10[v2++] = v7->unk_00[v0];
            }
        }
    }

    for (v0 = 0; v0 < 4; v0++) {
        if (v7->unk_10[v0] == 0) {
            break;
        }

        for (v1 = 0; v1 < v4; v1++) {
            if (v7->unk_30[v1] != 0) {
                if (v7->unk_10[v0] == v7->unk_30[v1]) {
                    if (Pokemon_AddMove(param0, v7->unk_10[v0]) == 0xffff) {
                        Pokemon_ReplaceMove(param0, v7->unk_10[v0]);
                    }
                    break;
                }
            }
        }
    }

    Heap_FreeToHeap(v7);
}

void ov5_021E6B40 (UnkStruct_02026310 * param0)
{
    sub_02026270(param0, 0);
    sub_02026278(param0, 0);
}

static const u16 Unk_ov5_021F9F6C[9][3] = {
    {0x168, 0xFF, 0xCA},
    {0x12A, 0xFE, 0xB7},
    {0x1B7, 0x13A, 0x7A},
    {0x1B6, 0x13B, 0xB9},
    {0x1BE, 0x13C, 0x8F},
    {0x1CA, 0x13D, 0xE2},
    {0x196, 0x13E, 0x13B},
    {0x1B8, 0x13F, 0x71},
    {0x1B1, 0x140, 0x166}
};

static u16 ov5_021E6B54 (u16 param0, UnkStruct_02026310 * param1)
{
    u16 v0, v1, v2, v3;
    BoxPokemon * v4[2];

    ov5_021E6668(param1, v4);

    for (v3 = 0; v3 < 9; v3++) {
        if (param0 == Unk_ov5_021F9F6C[v3][0]) {
            v2 = v3;
            break;
        }
    }

    if (v3 == 9) {
        return param0;
    }

    v0 = BoxPokemon_GetValue(v4[0], MON_DATA_HELD_ITEM, NULL);
    v1 = BoxPokemon_GetValue(v4[1], MON_DATA_HELD_ITEM, NULL);

    if ((v0 != Unk_ov5_021F9F6C[v2][1]) && (v1 != Unk_ov5_021F9F6C[v2][1])) {
        param0 = Unk_ov5_021F9F6C[v2][2];
    }

    return param0;
}

static void ov5_021E6BD0 (Pokemon * param0, UnkStruct_02026310 * param1)
{
    int v0, v1;
    BoxPokemon * v2[2];

    ov5_021E6668(param1, v2);

    v0 = BoxPokemon_GetValue(v2[0], MON_DATA_HELD_ITEM, NULL);
    v1 = BoxPokemon_GetValue(v2[1], MON_DATA_HELD_ITEM, NULL);

    if ((v0 == 236) || (v1 == 236)) {
        if (Pokemon_AddMove(param0, 344) == 0xffff) {
            Pokemon_ReplaceMove(param0, 344);
        }
    }
}

static u16 ov5_021E6C20 (UnkStruct_02026310 * param0, u8 param1[])
{
    u16 v0[2], v1, v2, v3, v4, v5;
    BoxPokemon * v6[2];
	int mateLevel[2];

    ov5_021E6668(param0, v6);

    v2 = 0;

    for (v1 = 0; v1 < 2; v1++) {
        if ((v0[v1] = BoxPokemon_GetValue(v6[v1], MON_DATA_SPECIES, NULL)) == 132) {
            param1[0] = v1 ^ 1;
            param1[1] = v1;
        } else if (BoxPokemon_GetGender(v6[v1]) == 1) {
            param1[0] = v1;
            param1[1] = v1 ^ 1;
        }
    }

    v3 = v0[param1[0]];
    v4 = sub_02076F84(v3);

    if (v4 == 29) {
        if (sub_02026248(param0) & 0x8000) {
            v4 = 32;
        } else {
            v4 = 29;
        }
    }

    if (v4 == 314) {
        if (sub_02026248(param0) & 0x8000) {
            v4 = 313;
        } else {
            v4 = 314;
        }
    }

    if (v4 == 490) {
        v4 = 489;
    }

    if ((v0[param1[1]] == 132) && (BoxPokemon_GetGender(v6[param1[0]]) != 1)) {
        v5 = param1[1];
        param1[1] = param1[0];
        param1[0] = v5;
    }

	if (BoxPokemon_GetGender(v6[0]) == BoxPokemon_GetGender(v6[1]))
	{
        mateLevel[0] = BoxPokemon_GetLevel(v6[0]);
		mateLevel[1] = BoxPokemon_GetLevel(v6[1]);
		
		if (mateLevel[0] < mateLevel[1])
		{
			v4 = BoxPokemon_GetValue(v6[0], MON_DATA_SPECIES, NULL);
		}
		else
		{
			v4 = BoxPokemon_GetValue(v6[1], MON_DATA_SPECIES, NULL);
		}
		
		if (mateLevel[0] == mateLevel[1])
		{
			if (LCRNG_Next() >= (0xffff / 2))
			{
				v4 = BoxPokemon_GetValue(v6[0], MON_DATA_SPECIES, NULL);
			}
			else
			{
				v4 = BoxPokemon_GetValue(v6[1], MON_DATA_SPECIES, NULL);
			}
		}
    }

    return v4;
}

void ov5_021E6CF0 (Pokemon * param0, u16 param1, u8 param2, TrainerInfo * param3, int param4, int param5)
{
    u8 v0, v1, v2;
    u16 v3;
    u8 v4 = PokemonPersonalData_GetSpeciesValue(param1, 19);
    Strbuf* v5;

    Pokemon_InitWith(param0, param1, 1, 32, 0, 0, 0, 0);

    v0 = 0;
    v3 = 4;

    Pokemon_SetValue(param0, 155, &v3);
    Pokemon_SetValue(param0, 9, &v4);
    Pokemon_SetValue(param0, 156, &v0);

    if (param2) {
        Pokemon_SetValue(param0, 152, &param2);
    }

    v2 = 1;
    Pokemon_SetValue(param0, 76, &v2);

    v5 = MessageUtil_SpeciesName(SPECIES_EGG, 4);
    Pokemon_SetValue(param0, 119, v5);
    Strbuf_Free(v5);

    if (param4 == 4) {
        u32 v6 = TrainerInfo_ID(param3);
        u32 v7 = TrainerInfo_Gender(param3);
        Strbuf* v8 = TrainerInfo_NameNewStrbuf(param3, 32);

        Pokemon_SetValue(param0, 145, v8);
        Pokemon_SetValue(param0, 7, &v6);
        Pokemon_SetValue(param0, 157, &v7);
        Strbuf_Free(v8);
    }

    sub_0209304C(param0, param3, param4, param5, 0);
}

void ov5_021E6DE8 (Pokemon * param0, u16 param1, UnkStruct_02026310 * param2, u32 param3, u8 param4)
{
    u8 v0;
    u16 v1;
    u32 v2;
    Strbuf* v3;
    u8 v4 = PokemonPersonalData_GetSpeciesValue(param1, 19);

    v2 = sub_02026248(param2);

    if (sub_02026280(param2)) {
        int v5;

        if (Pokemon_IsPersonalityShiny(param3, v2) == 0) {
            for (v5 = 0; v5 < 4; v5++) {
                v2 = ARNG_Next(v2);

                if (Pokemon_IsPersonalityShiny(param3, v2)) {
                    break;
                }
            }
        } else {
            (void)0;
        }
    }

    Pokemon_InitWith(param0, param1, 1, 32, 1, v2, 0, 0);

    v0 = 0;
    v1 = 4;

    Pokemon_SetValue(param0, 155, &v1);
    Pokemon_SetValue(param0, 9, &v4);
    Pokemon_SetValue(param0, 156, &v0);
    Pokemon_SetValue(param0, 112, &param4);

    v3 = MessageUtil_SpeciesName(SPECIES_EGG, 4);

    Pokemon_SetValue(param0, 119, v3);
    Strbuf_Free(v3);
}

void ov5_021E6EA8 (UnkStruct_02026310 * param0, Party * param1, TrainerInfo * param2)
{
    u16 v0;
    u8 v1[2], v2;
    Pokemon * v3 = Pokemon_New(4);

    v0 = ov5_021E6C20(param0, v1);
    v0 = ov5_021E6B54(v0, param0);

    {
        u32 v4 = TrainerInfo_ID(param2);
        BoxPokemon * v5 = ov5_021E622C(param0, v1[0]);
        u8 v6 = BoxPokemon_GetValue(v5, MON_DATA_FORM, NULL);

        ov5_021E6DE8(v3, v0, param0, v4, v6);
    }

    ov5_021E67B0(v3, param0);
    ov5_021E6948(v3, ov5_021E622C(param0, v1[1]), ov5_021E622C(param0, v1[0]));

    sub_0209304C(v3, param2, 3, sub_02017070(1, 0), 4);

    if (v0 == 172) {
        ov5_021E6BD0(v3, param0);
    }

    v2 = 1;
    Pokemon_SetValue(v3, 76, &v2);

    Party_AddPokemon(param1, v3);
    ov5_021E6B40(param0);
    Heap_FreeToHeap(v3);
}

static int ov5_021E6F6C (Party * param0)
{
    u8 v0;
    u8 v1;
    int v2;

    v2 = Party_GetCurrentCount(param0);

    for (v0 = 0; v0 < v2; v0++) {
        if (Pokemon_GetValue(Party_GetPokemonBySlotIndex(param0, v0), MON_DATA_EGG_EXISTS, NULL) == 0) {
            v1 = Pokemon_GetValue(Party_GetPokemonBySlotIndex(param0, v0), MON_DATA_ABILITY, NULL);

            if ((v1 == 40) || (v1 == 49)) {
                return 2;
            }
        }
    }

    return 1;
}

static u8 ov5_021E6FC0 (u16 * param0, u16 * param1)
{
    int v0, v1;

    for (v0 = 0; v0 < 2; v0++) {
        for (v1 = 0; v1 < 2; v1++) {
            if (param0[v0] == param1[v1]) {
                return 1;
            }
        }
    }

    return 0;
}

static u8 ov5_021E6FF0 (BoxPokemon ** param0)
{
    u16 v0[2][2], v1[2];
    u32 v2[2], v3[2], v4, v5;

    for (v5 = 0; v5 < 2; v5++) {
        v1[v5] = BoxPokemon_GetValue(param0[v5], MON_DATA_SPECIES, NULL);
        v2[v5] = BoxPokemon_GetValue(param0[v5], MON_DATA_OT_ID, NULL);
        v4 = BoxPokemon_GetValue(param0[v5], MON_DATA_PERSONALITY, NULL);
        v3[v5] = Pokemon_GetGenderOf(v1[v5], v4);
        v0[v5][0] = PokemonPersonalData_GetSpeciesValue(v1[v5], 22);
        v0[v5][1] = PokemonPersonalData_GetSpeciesValue(v1[v5], 23);

        if (BoxPokemon_GetValue(param0[v5], MON_DATA_HELD_ITEM, NULL) == 217) {
            return 0;
        }
    }

    if ((v0[0][0] == 15) || (v0[1][0] == 15)) {
        return 0;
    }

    if ((v0[0][0] == 13) && (v0[1][0] == 13)) {
        return 0;
    }

    if ((v0[0][0] == 13) || (v0[1][0] == 13)) {
        if (v2[0] == v2[1]) {
            return 20;
        } else {
            return 50;
        }
    }

    if (v3[0] == v3[1]) {
        return 20;
    }

    if ((v3[0] == 2) || (v3[1] == 2)) {
        return 0;
    }

    if (ov5_021E6FC0(v0[0], v0[1]) == 0) {
        return 0;
    }

    if (v1[0] == v1[1]) {
        if (v2[0] != v2[1]) {
            return 70;
        } else {
            return 50;
        }
    } else {
        if (v2[0] != v2[1]) {
            return 50;
        } else {
            return 20;
        }
    }

    return 0;
}

static u8 ov5_021E70FC (UnkStruct_02026310 * param0)
{
    BoxPokemon * v0[2];

    ov5_021E6668(param0, v0);
    return ov5_021E6FF0(v0);
}

static const u16 Unk_ov5_021F9F54[] = {
	101,    // New Years's Day
    106,    // Three Kings' Day
	112,	// Junichi Masuda's Birthday
	214,	// Valentine's Day
    227,    // Green Release
	303,	// Hinamatsuri
	401,	// School Entrance Exam Day
	405,    // Easter 2026
    414,    // Lyss Birthday
    430,    // Maisy's Birthday
	501,	// May Day
	509,    // Goku & Piccolo Day
	611,	// Pokemon Daisuki Club?
	707,	// Qixi  (Tanabata)
    807,    // Ellie's birthday
	813,	// 2009 World Championship
	907,	// American Anime Debut
	928,	// DP Anime Debut (and Red and Blue Release)
    1009,   // Yellow Release
    1031,   // Halloween
	1121,	// Ruby Sapphire Release (and Gold and Silver Relase)
	1214,	// Crystal Release
    1224,   // Christmas Eve
    1225,   // Christmas
	1231,   // New Year's Eve
};

static int ov5_021E7110 (FieldSystem * fieldSystem)
{
    int v0 = sub_02055BB8(fieldSystem) * 100 + sub_02055BC4(fieldSystem);
    int v1;

    if (sub_02055C40(fieldSystem)) {
        return 64;
    }

    for (v1 = 0; v1 < NELEMS(Unk_ov5_021F9F54); v1++) {
        if (Unk_ov5_021F9F54[v1] == v0) {
            return 32;
        }
    }

    return 64;
}

BOOL ov5_021E7154 (UnkStruct_02026310 * param0, Party * param1, FieldSystem * fieldSystem)
{
    u32 v0, v1, v2, v3, v4;
    u32 v5 = 0, v6;
    int v7;
    BoxPokemon * v8[2];

    ov5_021E6668(param0, v8);

    v2 = 0;

    for (v0 = 0; v0 < 2; v0++) {
        if (BoxPokemon_GetValue(v8[v0], MON_DATA_SPECIES_EXISTS, NULL) != 0) {
            sub_02026260(sub_02026218(param0, v0), 1);
            v2++;
        }
    }

    if ((sub_02026234(param0) == 0) && (v2 == 2)) {
        if ((sub_02026228(sub_02026218(param0, 1)) & 0xff) == 0xff) {
            v3 = ov5_021E70FC(param0);
            v4 = LCRNG_Next();
            v4 = (v4 * 100) / 0xffff;

            if (v3 > v4) {
                ov5_021E6720(param0);
            }
        }
    }

    v6 = sub_02026250(param0);
    sub_02026278(param0, ++v6);

    if (v6 == ov5_021E7110(fieldSystem)) {
        sub_02026278(param0, 0);
        v7 = ov5_021E6F6C(param1);

        for (v0 = 0; v0 < Party_GetCurrentCount(param1); v0++) {
            Pokemon * v9 = Party_GetPokemonBySlotIndex(param1, v0);

            if (Pokemon_GetValue(v9, MON_DATA_IS_EGG, NULL)) {
                if (Pokemon_GetValue(v9, MON_DATA_IS_DATA_INVALID, NULL)) {
                    continue;
                }

                v1 = Pokemon_GetValue(v9, MON_DATA_FRIENDSHIP, NULL);

                if (v1 != 0) {
                    if (v1 >= v7) {
                        v1 -= v7;
                    } else {
                        v1--;
                    }

                    Pokemon_SetValue(v9, 9, (u8 *)&v1);
                } else {
                    return 1;
                }
            }
        }
    }

    return 0;
}

Pokemon * ov5_021E7278 (Party * param0)
{
    int v0;
    Pokemon * v1;
    int v2 = Party_GetCurrentCount(param0);

    for (v0 = 0; v0 < v2; v0++) {
        v1 = Party_GetPokemonBySlotIndex(param0, v0);

        if (Pokemon_GetValue(v1, MON_DATA_IS_EGG, NULL)
            && (Pokemon_GetValue(v1, MON_DATA_FRIENDSHIP, NULL) == 0)) {
            return v1;
        }
    }

    return NULL;
}

void ov5_021E72BC (UnkStruct_02026310 * param0, StringTemplate * param1)
{
    BoxPokemon * v0[2];
    u16 v1[10 + 1];

    ov5_021E6668(param0, v0);

    if (BoxPokemon_GetValue(v0[0], MON_DATA_SPECIES, NULL) != 0) {
        StringTemplate_SetNickname(param1, 0, v0[0]);
        StringTemplate_SetOTName(param1, 2, v0[0]);
    }

    if (BoxPokemon_GetValue(v0[1], MON_DATA_SPECIES, NULL) != 0) {
        StringTemplate_SetNickname(param1, 1, v0[1]);
    }
}

void ov5_021E7308 (UnkStruct_02026310 * param0, u32 param1, u32 param2, u32 param3, u8 param4, StringTemplate * param5)
{
    UnkStruct_02026218 * v0;
    BoxPokemon * v1;
    u8 v2, v3;
    u16 v4;

    v0 = sub_02026218(param0, param4);
    v1 = ov5_021E622C(param0, param4);

    StringTemplate_SetNickname(param5, param1, v1);

    v2 = ov5_021E6520(v1, sub_02026228(v0));
    StringTemplate_SetNumber(param5, param2, v2, 3, 0, 1);
    v3 = BoxPokemon_GetValue(v1, MON_DATA_GENDER, NULL);

    if (v3 != 2) {
        v4 = BoxPokemon_GetValue(v1, MON_DATA_SPECIES, NULL);

        if (((v4 == 29) || (v4 == 32)) && (BoxPokemon_GetValue(v1, MON_DATA_HAS_NICKNAME, NULL) == 0)) {
            v3 = 2;
        }
    }

    StringTemplate_SetGenderMarker(param5, param3, v3);
}

u16 ov5_021E73A0 (Party * param0, int param1, StringTemplate * param2)
{
    Pokemon * v0 = Party_GetPokemonBySlotIndex(param0, param1);

    StringTemplate_SetNickname(param2, 0, Pokemon_GetBoxPokemon(v0));
    return Pokemon_GetValue(v0, MON_DATA_SPECIES, NULL);
}

u8 ov5_021E73C8 (UnkStruct_02026310 * param0)
{
    u8 v0;

    if (sub_02026234(param0)) {
        return 1;
    }

    if ((v0 = ov5_021E6238(param0))) {
        return v0 + 1;
    }

    return 0;
}

u8 ov5_021E73F0 (u32 param0)
{
    switch (param0) {
    case 0:
        return 3;
    case 20:
        return 2;
    case 50:
        return 1;
    case 70:
        return 0;
    }

    return 0;
}

extern u32 ov5_021E7420 (UnkStruct_02026310 * param0)
{
    u8 v0, v1;

    v0 = ov5_021E70FC(param0);
    v1 = ov5_021E73F0(v0);

    return v1;
}

static void ov5_021E742C (Pokemon * param0, int param1)
{
    u16 v0;
    u16 v1[4];
    u8 v2[4];
    u32 v3, v4;
    u8 v5[6], v6;
    u8 v7, v8, v9, v10, v11, v12, v13, v14;
    Strbuf* v15 = Strbuf_Init(7 + 1, param1);
    Pokemon * v16 = Pokemon_New(param1);

    v0 = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);

    for (v7 = 0; v7 < 4; v7++) {
        v1[v7] = Pokemon_GetValue(param0, MON_DATA_MOVE1 + v7, NULL);
        v2[v7] = Pokemon_GetValue(param0, MON_DATA_MOVE1_CUR_PP + v7, NULL);
    }

    v3 = Pokemon_GetValue(param0, MON_DATA_PERSONALITY, NULL);

    for (v7 = 0; v7 < 6; v7++) {
        v5[v7] = Pokemon_GetValue(param0, MON_DATA_HP_IV + v7, NULL);
    }

    v8 = Pokemon_GetValue(param0, MON_DATA_LANGUAGE, NULL);
    v9 = Pokemon_GetValue(param0, MON_DATA_MET_GAME, NULL);
    v10 = Pokemon_GetValue(param0, MON_DATA_MARKS, NULL);
    v6 = Pokemon_GetValue(param0, MON_DATA_POKERUS, NULL);
    v12 = Pokemon_GetValue(param0, MON_DATA_FATEFUL_ENCOUNTER, NULL);

    Pokemon_GetValue(param0, MON_DATA_OTNAME_STRBUF, v15);

    v14 = Pokemon_GetValue(param0, MON_DATA_OT_GENDER, NULL);
    v4 = Pokemon_GetValue(param0, MON_DATA_OT_ID, NULL);
    v13 = Pokemon_GetValue(param0, MON_DATA_FORM, NULL);

    if (v0 == 490) {
        int v17 = Pokemon_GetValue(param0, MON_DATA_MET_LOCATION, NULL);

        if (v17 == sub_02017070(2, 1)) {
            while (Pokemon_IsPersonalityShiny(v4, v3)) {
                v3 = ARNG_Next(v3);
            }
        }
    }

    Pokemon_InitWith(v16, v0, 1, 32, 1, v3, 0, 0);

    for (v7 = 0; v7 < 4; v7++) {
        Pokemon_SetValue(v16, 54 + v7, &(v1[v7]));
        Pokemon_SetValue(v16, 58 + v7, &(v2[v7]));
    }

    for (v7 = 0; v7 < 6; v7++) {
        Pokemon_SetValue(v16, 70 + v7, &(v5[v7]));
    }

    Pokemon_SetValue(v16, 12, &v8);
    Pokemon_SetValue(v16, 122, &v9);
    Pokemon_SetValue(v16, 11, &v10);

    v11 = 120;

    Pokemon_SetValue(v16, 9, &v11);
    Pokemon_SetValue(v16, 154, &v6);
    Pokemon_SetValue(v16, 110, &v12);
    Pokemon_SetValue(v16, 145, v15);
    Pokemon_SetValue(v16, 157, &v14);
    Pokemon_SetValue(v16, 7, &v4);
    Pokemon_SetValue(v16, 112, &v13);

    {
        u16 v18;
        u8 v19, v20, v21;

        v18 = Pokemon_GetValue(param0, MON_DATA_MET_LOCATION, NULL);
        v19 = Pokemon_GetValue(param0, MON_DATA_MET_YEAR, NULL);
        v20 = Pokemon_GetValue(param0, MON_DATA_MET_MONTH, NULL);
        v21 = Pokemon_GetValue(param0, MON_DATA_MET_DAY, NULL);

        Pokemon_SetValue(v16, 152, &v18);
        Pokemon_SetValue(v16, 146, &v19);
        Pokemon_SetValue(v16, 147, &v20);
        Pokemon_SetValue(v16, 148, &v21);

        v18 = Pokemon_GetValue(param0, MON_DATA_HATCH_LOCATION, NULL);
        v19 = Pokemon_GetValue(param0, MON_DATA_HATCH_YEAR, NULL);
        v20 = Pokemon_GetValue(param0, MON_DATA_HATCH_MONTH, NULL);
        v21 = Pokemon_GetValue(param0, MON_DATA_HATCH_DAY, NULL);

        Pokemon_SetValue(v16, 153, &v18);
        Pokemon_SetValue(v16, 149, &v19);
        Pokemon_SetValue(v16, 150, &v20);
        Pokemon_SetValue(v16, 151, &v21);
    }

    Pokemon_Copy(v16, param0);
    Strbuf_Free(v15);
    Heap_FreeToHeap(v16);
}

void ov5_021E771C (Pokemon * param0, int param1)
{
    u8 v0, v1;
    u8 v2, v3;
    u16 v4;
    u16 v5[11];

    v0 = 70;
    v1 = 0;
    v2 = 4;
    v3 = 0;

    ov5_021E742C(param0, param1);
    Pokemon_SetValue(param0, 76, &v0);

    v4 = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);

    MessageLoader_GetSpeciesName(v4, 0, v5);
    Pokemon_SetValue(param0, 117, v5);
    Pokemon_SetValue(param0, 77, &v1);
    Pokemon_SetValue(param0, 155, &v2);
    Pokemon_SetValue(param0, 156, &v3);
    Pokemon_CalcLevelAndStats(param0);
}

u32 ov5_021E7790 (BoxPokemon ** param0)
{
    return ov5_021E73F0(ov5_021E6FF0(param0));
}
