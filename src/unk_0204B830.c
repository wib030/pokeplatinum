#include <nitro.h>
#include <string.h>

#include "inlines.h"
#include "constants/species.h"

#include "strbuf.h"
#include "trainer_info.h"
#include "struct_decls/struct_020507E4_decl.h"
#include "pokemon.h"
#include "struct_decls/struct_party_decl.h"
#include "struct_decls/struct_0207D3C0_decl.h"

#include "struct_defs/struct_0202610C.h"
#include "field/field_system.h"
#include "struct_defs/union_0204C4D0.h"

#include "string_template.h"
#include "unk_02017038.h"
#include "heap.h"
#include "unk_0201D15C.h"
#include "strbuf.h"
#include "unk_02025E08.h"
#include "trainer_info.h"
#include "unk_0202602C.h"
#include "unk_0202854C.h"
#include "unk_020298BC.h"
#include "unk_0202C9F4.h"
#include "unk_0202D778.h"
#include "unk_0202DAB4.h"
#include "unk_0202F180.h"
#include "field_script_context.h"
#include "unk_0203E880.h"
#include "unk_0204B830.h"
#include "unk_020507CC.h"
#include "unk_02054884.h"
#include "poketch_data.h"
#include "unk_0206AFE0.h"
#include "pokemon.h"
#include "party.h"
#include "unk_0207D3B8.h"
#include "unk_020923C0.h"
#include "unk_02092494.h"

typedef struct {
    FieldSystem * fieldSystem;
    StringTemplate * unk_04;
    void * unk_08;
} UnkStruct_0204B830;

typedef BOOL (* UnkFuncPtr_020EBE94)(FieldSystem *, void *);
typedef void (* UnkFuncPtr_020EBE94_1)(FieldSystem *, void *);
typedef void (* UnkFuncPtr_020EBE94_2)(UnkStruct_0204B830 *, u16 *, u16 *);
typedef void (* UnkFuncPtr_020EBE94_3)(UnkStruct_0204B830 *, u16 *, u16 *);

typedef struct {
    UnkFuncPtr_020EBE94 unk_00;
    UnkFuncPtr_020EBE94_1 unk_04;
    UnkFuncPtr_020EBE94_2 unk_08;
    UnkFuncPtr_020EBE94_3 unk_0C;
} UnkStruct_020EBE94;

static const UnkStruct_020EBE94 Unk_020EBE94[13];

static void sub_0204B830 (UnkStruct_0204B830 * param0, FieldSystem * fieldSystem, StringTemplate * param2, void * param3)
{
    param0->fieldSystem = fieldSystem;
    param0->unk_04 = param2;
    param0->unk_08 = param3;
}

static int sub_0204B838 (FieldSystem * fieldSystem)
{
    return sub_0202DF40(sub_0202DF18());
}

static void * sub_0204B844 (FieldSystem * fieldSystem)
{
    return sub_0202DF5C(sub_0202DF18());
}

static void sub_0204B850 (FieldSystem * fieldSystem)
{
    sub_0202DF78(sub_0202DF18());
}

BOOL ScrCmd_23E (ScriptContext * param0)
{
    switch (ScriptContext_ReadHalfWord(param0)) {
    case 0:
        sub_0202DEE4(param0->fieldSystem->saveData, 32);
        break;
    case 7:
        sub_0202DF04(param0->fieldSystem->saveData, 0);
        break;
    case 8:
        sub_0202DF04(param0->fieldSystem->saveData, 1);
        break;
    case 1:
    {
        u16 * v0 = ScriptContext_GetVarPointer(param0);

        if (sub_0204B838(param0->fieldSystem) != 0) {
            *v0 = 1;
        } else {
            *v0 = 0;
        }
    }
    break;
    case 2:
    {
        u16 * v1 = ScriptContext_GetVarPointer(param0);
        *v1 = sub_0204B838(param0->fieldSystem);
    }
    break;
    case 3:
    {
        u16 * v2 = ScriptContext_GetVarPointer(param0);
        const UnkStruct_020EBE94 * v3 = &Unk_020EBE94[sub_0204B838(param0->fieldSystem) - 1];

        *v2 = v3->unk_00(param0->fieldSystem, sub_0204B844(param0->fieldSystem));
    }
    break;
    case 4:
    {
        const UnkStruct_020EBE94 * v4 = &Unk_020EBE94[sub_0204B838(param0->fieldSystem) - 1];

        v4->unk_04(param0->fieldSystem, sub_0204B844(param0->fieldSystem));
        sub_0204B850(param0->fieldSystem);
    }
    break;
    case 5:
    {
        UnkStruct_0204B830 v5;
        const UnkStruct_020EBE94 * v6 = &Unk_020EBE94[sub_0204B838(param0->fieldSystem) - 1];
        StringTemplate ** v7 = sub_0203F098(param0->fieldSystem, 15);
        u16 * v8 = ScriptContext_GetVarPointer(param0);
        u16 * v9 = ScriptContext_GetVarPointer(param0);

        sub_0204B830(&v5, param0->fieldSystem, *v7, sub_0204B844(param0->fieldSystem));
        v6->unk_08(&v5, v8, v9);
    }
    break;
    case 6:
    {
        UnkStruct_0204B830 v10;
        const UnkStruct_020EBE94 * v11 = &Unk_020EBE94[sub_0204B838(param0->fieldSystem) - 1];
        StringTemplate ** v12 = sub_0203F098(param0->fieldSystem, 15);
        u16 * v13 = ScriptContext_GetVarPointer(param0);
        u16 * v14 = ScriptContext_GetVarPointer(param0);

        sub_0204B830(&v10, param0->fieldSystem, *v12, sub_0204B844(param0->fieldSystem));
        v11->unk_0C(&v10, v13, v14);
    }
    break;
    }

    return 0;
}

static BOOL sub_0204BA50 (FieldSystem * fieldSystem, void * param1)
{
    Party * v0 = Party_GetFromSavedata(fieldSystem->saveData);

    if (Party_GetCurrentCount(v0) < 6) {
        return 1;
    } else {
        return 0;
    }
}

static void sub_0204BA68 (FieldSystem * fieldSystem, void * param1)
{
    sub_02054930(32, fieldSystem->saveData, 490, 1, 2, 1);
}

static void sub_0204BA88 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    *param1 = 379;
    *param2 = 13;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
}

static void sub_0204BAAC (FieldSystem * fieldSystem, void * param1)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(fieldSystem);
    TrainerInfo * v1 = SaveData_GetTrainerInfo(fieldSystem->saveData);
    UnkStruct_020507E4 * v2 = SaveData_Events(fieldSystem->saveData);
    Party * v3;
    Pokemon * v4;
    BOOL v5;
    Pokemon * v6 = NULL;
    u32 v7, v8, v9, v10;
    u32 v11;
    int v12;
    u8 * v13;
    u8 * v14;

    v4 = (Pokemon *)&v0->val1.unk_04;
    v14 = (u8 *)&v0->val1.unk_F0;
    v12 = Pokemon_GetValue(v4, MON_DATA_MET_LOCATION, NULL);
    v8 = Pokemon_GetValue(v4, MON_DATA_PERSONALITY, NULL);
    v7 = Pokemon_GetValue(v4, MON_DATA_OT_ID, NULL);
    v9 = ARNG_Next((u32)OS_GetTick());

    if (v8 == 0x0) {
        (void)0;
    } else if (v8 == 0x1) {
        while (Pokemon_IsPersonalityShiny(v7, v9)) {
            v9 = ARNG_Next(v9);
        }
    } else {
        v9 = v8;
    }

    sub_020780C4(v4, v9);
    v10 = Pokemon_GetGender(v4);

    Pokemon_SetValue(v4, 111, (u8 *)&v10);
    v10 = Pokemon_GetValue(v4, MON_DATA_HP_IV, 0) + Pokemon_GetValue(v4, MON_DATA_ATK_IV, 0) + Pokemon_GetValue(v4, MON_DATA_DEF_IV, 0) + Pokemon_GetValue(v4, MON_DATA_SPEED_IV, 0) + Pokemon_GetValue(v4, MON_DATA_SPATK_IV, 0) + Pokemon_GetValue(v4, MON_DATA_SPDEF_IV, 0);

    if (v10 == 0) {
        v8 = LCRNG_Next();
        v9 = (v8 & (0x1F << 0)) >> 0;

        Pokemon_SetValue(v4, 70, (u8 *)&v9);

        v9 = (v8 & (0x1F << 5)) >> 5;
        Pokemon_SetValue(v4, 71, (u8 *)&v9);

        v9 = (v8 & (0x1F << 10)) >> 10;
        Pokemon_SetValue(v4, 72, (u8 *)&v9);

        v8 = LCRNG_Next();
        v9 = (v8 & (0x1F << 0)) >> 0;
        Pokemon_SetValue(v4, 73, (u8 *)&v9);

        v9 = (v8 & (0x1F << 5)) >> 5;
        Pokemon_SetValue(v4, 74, (u8 *)&v9);

        v9 = (v8 & (0x1F << 10)) >> 10;
        Pokemon_SetValue(v4, 75, (u8 *)&v9);
    }

    v13 = sub_0202D79C(fieldSystem->saveData);

    if (Pokemon_GetValue(v4, MON_DATA_SINNOH_RED_RIBBON, 0)) {
        v13[sub_02092444(73)] = v14[0];
    }

    if (Pokemon_GetValue(v4, MON_DATA_SINNOH_GREEN_RIBBON, 0)) {
        v13[sub_02092444(74)] = v14[1];
    }

    if (Pokemon_GetValue(v4, MON_DATA_SINNOH_BLUE_RIBBON, 0)) {
        v13[sub_02092444(75)] = v14[2];
    }

    if (Pokemon_GetValue(v4, MON_DATA_SINNOH_FESTIVAL_RIBBON, 0)) {
        v13[sub_02092444(76)] = v14[3];
    }

    if (Pokemon_GetValue(v4, MON_DATA_SINNOH_CARNIVAL_RIBBON, 0)) {
        v13[sub_02092444(77)] = v14[4];
    }

    if (Pokemon_GetValue(v4, MON_DATA_SINNOH_CLASSIC_RIBBON, 0)) {
        v13[sub_02092444(78)] = v14[5];
    }

    if (Pokemon_GetValue(v4, MON_DATA_SINNOH_PREMIER_RIBBON, 0)) {
        v13[sub_02092444(79)] = v14[6];
    }

    if (Pokemon_GetValue(v4, MON_DATA_HOENN_MARINE_RIBBON, 0)) {
        v13[sub_02092444(25)] = v14[7];
    }

    if (Pokemon_GetValue(v4, MON_DATA_HOENN_LAND_RIBBON, 0)) {
        v13[sub_02092444(26)] = v14[8];
    }

    if (Pokemon_GetValue(v4, MON_DATA_HOENN_SKY_RIBBON, 0)) {
        v13[sub_02092444(27)] = v14[9];
    }

    if (v0->val1.unk_00 == 0) {
        Strbuf* v15 = TrainerInfo_NameNewStrbuf(v1, 32);
        u32 v16 = TrainerInfo_ID(v1);
        u32 v17 = TrainerInfo_Gender(v1);

        v6 = Pokemon_New(32);

        Pokemon_Copy(v4, v6);
        Pokemon_SetValue(v6, 145, v15);
        Pokemon_SetValue(v6, 7, &v16);
        Pokemon_SetValue(v6, 157, &v17);

        v4 = v6;
        Strbuf_Free(v15);
    }

    sub_0209304C(v4, v1, 4, sub_02017070(2, v12), 32);

    if (Pokemon_GetValue(v4, MON_DATA_SPECIES, NULL) == SPECIES_ARCEUS) {
        if (Pokemon_GetValue(v4, MON_DATA_FATEFUL_ENCOUNTER, NULL) == 1) {
            if (sub_0206B5F8(v2) == 0) {
                sub_0206B608(v2, 1);
            }
        }
    }

    Pokemon_CalcLevelAndStats(v4);

    v3 = Party_GetFromSavedata(fieldSystem->saveData);
    v5 = Party_AddPokemon(v3, v4);

    if (v5) {
        sub_0202F180(fieldSystem->saveData, v4);
    }

    if (v6) {
        Heap_FreeToHeap(v6);
    }
}

static void sub_0204BDEC (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    Pokemon * v1;

    *param1 = 379;
    *param2 = 7;

    v1 = (Pokemon *)&v0->val1.unk_04;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetSpeciesNameWithArticle(param0->unk_04, 1, Pokemon_GetBoxPokemon(v1));
}

static void sub_0204BE2C (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    *param1 = 379;
    *param2 = 4;
}

static void sub_0204BE3C (FieldSystem * fieldSystem, void * param1)
{
    sub_0204BAAC(fieldSystem, param1);
}

static void sub_0204BE44 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    Pokemon * v1;

    *param1 = 379;
    *param2 = 8;

    v1 = (Pokemon *)v0->val2.unk_04;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetSpeciesName(param0->unk_04, 1, Pokemon_GetBoxPokemon(v1));
}

static BOOL sub_0204BE84 (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);

    return sub_0207D55C(v0, v1->val3.unk_00, 1, 32);
}

static void sub_0204BEAC (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);
    u16 v2;
    u16 v3 = v1->val3.unk_00;

    sub_0207D570(v0, v3, 1, 32);
}

static void sub_0204BED4 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    u16 v1 = v0->val3.unk_00;

    *param1 = 379;
    *param2 = 9;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetItemName(param0->unk_04, 1, v1);
}

static void sub_0204BF14 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(param0->fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(param0->fieldSystem);
    u16 v2 = v1->val3.unk_00;

    *param1 = 379;
    *param2 = 5;

    StringTemplate_SetItemName(param0->unk_04, 0, v2);
}

static BOOL sub_0204BF48 (FieldSystem * fieldSystem, void * param1)
{
    return 1;
}

static void sub_0204BF4C (FieldSystem * fieldSystem, void * param1)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(fieldSystem);
    const BattleRegulation * v1 = (const BattleRegulation *)v0;

    sub_0202613C(fieldSystem->saveData, v1);
}

static void sub_0204BF60 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    const BattleRegulation * v1 = (const BattleRegulation *)v0;
    Strbuf* v2;

    *param1 = 379;
    *param2 = 10;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));

    v2 = sub_0202605C(v1, 32);
    StringTemplate_SetStrbuf(param0->unk_04, 1, v2, 0, 1, GAME_LANGUAGE);
    Strbuf_Free(v2);
}

static void sub_0204BFB8 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    *param1 = 379;
    *param2 = 6;
}

static BOOL sub_0204BFC8 (FieldSystem * fieldSystem, void * param1)
{
    int v0 = sub_020289A0(sub_020298B0(fieldSystem->saveData));

    if (v0 < 200) {
        return 1;
    }

    return 0;
}

static void sub_0204BFE0 (FieldSystem * fieldSystem, void * param1)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(fieldSystem);
    int v1 = v0->val4.unk_00;

    sub_0202895C(sub_020298B0(fieldSystem->saveData), v1);
}

static void sub_0204BFF8 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    int v1 = v0->val4.unk_00;

    *param1 = 379;
    *param2 = 11;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetUndergroundGoodsName(param0->unk_04, 1, v1);
}

static void sub_0204C034 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    *param1 = 379;
    *param2 = 6;
}

static BOOL sub_0204C044 (FieldSystem * fieldSystem, void * param1)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(fieldSystem);
    int v1 = v0->val5.unk_00;
    int v2 = v0->val5.unk_04;

    switch (v1) {
    case 1:
        return sub_0202CB70(sub_0202CA1C(fieldSystem->saveData), v2, 1);
    case 2:
        return 1;
    case 3:
        return 1;
    }

    return 0;
}

static void sub_0204C07C (FieldSystem * fieldSystem, void * param1)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(fieldSystem);
    int v1 = v0->val5.unk_00;
    int v2 = v0->val5.unk_04;

    switch (v1) {
    case 1:
        sub_0202CAE0(sub_0202CA1C(fieldSystem->saveData), v2, 1);
        break;
    case 2:
        sub_02029E2C(sub_02029D04(sub_0202A750(fieldSystem->saveData)), v2, 1);
        break;
    case 3:
        sub_02029EFC(sub_02029D04(sub_0202A750(fieldSystem->saveData)), v2);
        break;
    }
}

static void sub_0204C0CC (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    int v1 = v0->val5.unk_00;
    int v2 = v0->val5.unk_04;

    switch (v1) {
    case 1:
        StringTemplate_SetBallSealName(param0->unk_04, 1, v2);
        break;
    case 2:
        StringTemplate_SetContestAccessoryName(param0->unk_04, 1, v2);
        break;
    case 3:
        StringTemplate_SetContestBackdropName(param0->unk_04, 1, v2);
        break;
    }

    *param1 = 379;
    *param2 = 12;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
}

static void sub_0204C128 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    *param1 = 379;
    *param2 = 6;
}

static BOOL sub_0204C138 (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);

    return sub_0207D55C(v0, 454, 1, 32);
}

static void sub_0204C15C (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);
    UnkStruct_020507E4 * v2 = SaveData_Events(fieldSystem->saveData);
    u16 v3;
    u16 v4 = 454;

    sub_0207D570(v0, v4, 1, 32);
    sub_0206B144(v2, 0);
}

static void sub_0204C190 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    u16 v1 = 454;

    *param1 = 379;
    *param2 = 14;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetItemName(param0->unk_04, 1, v1);
}

static void sub_0204C1CC (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(param0->fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(param0->fieldSystem);
    u16 v2 = 454;

    *param1 = 379;
    *param2 = 5;

    StringTemplate_SetItemName(param0->unk_04, 0, v2);
}

static BOOL sub_0204C1FC (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);

    return sub_0207D55C(v0, 452, 1, 32);
}

static void sub_0204C220 (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);
    UnkStruct_020507E4 * v2 = SaveData_Events(fieldSystem->saveData);
    u16 v3;
    u16 v4 = 452;

    sub_0207D570(v0, v4, 1, 32);
    sub_0206B144(v2, 1);

    if (sub_0206B618(v2) == 0) {
        sub_0206B628(v2, 1);
    }
}

static void sub_0204C264 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    u16 v1 = 452;

    *param1 = 379;
    *param2 = 15;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetItemName(param0->unk_04, 1, v1);
}

static void sub_0204C2A0 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(param0->fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(param0->fieldSystem);
    u16 v2 = 452;

    *param1 = 379;
    *param2 = 5;

    StringTemplate_SetItemName(param0->unk_04, 0, v2);
}

static BOOL sub_0204C2D0 (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);

    return sub_0207D55C(v0, 467, 1, 32);
}

static void sub_0204C2F4 (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);
    UnkStruct_020507E4 * v2 = SaveData_Events(fieldSystem->saveData);
    u16 v3;
    u16 v4 = 467;

    sub_0207D570(v0, v4, 1, 32);
    sub_0206B144(v2, 3);
}

static void sub_0204C328 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    u16 v1 = 467;

    *param1 = 379;
    *param2 = 17;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetItemName(param0->unk_04, 1, v1);
}

static void sub_0204C364 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(param0->fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(param0->fieldSystem);
    u16 v2 = 467;

    *param1 = 379;
    *param2 = 5;

    StringTemplate_SetItemName(param0->unk_04, 0, v2);
}

static BOOL sub_0204C394 (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);

    return sub_0207D55C(v0, 455, 1, 32);
}

static void sub_0204C3B8 (FieldSystem * fieldSystem, void * param1)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);
    UnkStruct_020507E4 * v2 = SaveData_Events(fieldSystem->saveData);
    u16 v3;
    u16 v4 = 455;

    sub_0207D570(v0, v4, 1, 32);
    sub_0206B144(v2, 2);
}

static void sub_0204C3EC (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);
    u16 v1 = 455;

    *param1 = 379;
    *param2 = 16;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetItemName(param0->unk_04, 1, v1);
}

static void sub_0204C428 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkStruct_0207D3C0 * v0 = sub_0207D990(param0->fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(param0->fieldSystem);
    u16 v2 = 455;

    *param1 = 379;
    *param2 = 5;

    StringTemplate_SetItemName(param0->unk_04, 0, v2);
}

static BOOL sub_0204C458 (FieldSystem * fieldSystem, void * param1)
{
    PoketchData * v0 = SaveData_PoketchData(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);

    return PoketchData_IsEnabled(v0);
}

static void sub_0204C474 (FieldSystem * fieldSystem, void * param1)
{
    PoketchData * v0 = SaveData_PoketchData(fieldSystem->saveData);
    UnkUnion_0204C4D0 * v1 = sub_0204B844(fieldSystem);

    PoketchData_RegisterApp(v0, v1->val6.unk_00);
}

static void sub_0204C494 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);

    *param1 = 379;
    *param2 = 19;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
    StringTemplate_SetPoketchAppName(param0->unk_04, 1, v0->val6.unk_00);
}

static void sub_0204C4D0 (UnkStruct_0204B830 * param0, u16 * param1, u16 * param2)
{
    UnkUnion_0204C4D0 * v0 = sub_0204B844(param0->fieldSystem);

    *param1 = 379;
    *param2 = 20;

    StringTemplate_SetPlayerName(param0->unk_04, 0, SaveData_GetTrainerInfo(param0->fieldSystem->saveData));
}

static const UnkStruct_020EBE94 Unk_020EBE94[] = {
    {sub_0204BA50, sub_0204BAAC, sub_0204BDEC, sub_0204BE2C},
    {sub_0204BA50, sub_0204BE3C, sub_0204BE44, sub_0204BE2C},
    {sub_0204BE84, sub_0204BEAC, sub_0204BED4, sub_0204BF14},
    {sub_0204BF48, sub_0204BF4C, sub_0204BF60, sub_0204BFB8},
    {sub_0204BFC8, sub_0204BFE0, sub_0204BFF8, sub_0204C034},
    {sub_0204C044, sub_0204C07C, sub_0204C0CC, sub_0204C128},
    {sub_0204BA50, sub_0204BA68, sub_0204BA88, sub_0204BE2C},
    {sub_0204C138, sub_0204C15C, sub_0204C190, sub_0204C1CC},
    {sub_0204C1FC, sub_0204C220, sub_0204C264, sub_0204C2A0},
    {sub_0204C394, sub_0204C3B8, sub_0204C3EC, sub_0204C428},
    {sub_0204C458, sub_0204C474, sub_0204C494, sub_0204C4D0},
    {sub_0204C2D0, sub_0204C2F4, sub_0204C328, sub_0204C364},
    {sub_0204BA50, sub_0204BAAC, sub_0204BDEC, sub_0204BE2C}
};
