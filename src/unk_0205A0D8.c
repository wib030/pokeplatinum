#include <nitro.h>
#include <string.h>

#include "core_sys.h"

#include "message.h"
#include "struct_decls/struct_020149F0_decl.h"
#include "strbuf.h"
#include "trainer_info.h"
#include "struct_decls/struct_020508D4_decl.h"
#include "pokemon.h"
#include "struct_decls/struct_party_decl.h"
#include "savedata.h"

#include "constdata/const_020F1E88.h"
#include "constdata/const_020F410C.h"

#include "field/field_system.h"
#include "struct_defs/struct_0205AA50.h"
#include "functypes/funcptr_0205AB10.h"
#include "struct_defs/struct_02072014.h"
#include "struct_defs/struct_02098C44.h"
#include "struct_defs/pokemon_summary.h"

#include "unk_02005474.h"
#include "message.h"
#include "string_template.h"
#include "unk_0200DA60.h"
#include "unk_0200F174.h"
#include "unk_020149F0.h"
#include "heap.h"
#include "unk_02018340.h"
#include "unk_0201D670.h"
#include "strbuf.h"
#include "unk_02025E08.h"
#include "unk_0202602C.h"
#include "unk_0202D778.h"
#include "communication_information.h"
#include "communication_system.h"
#include "unk_020363E8.h"
#include "unk_020366A0.h"
#include "field_system.h"
#include "unk_0203D1B8.h"
#include "unk_020508D4.h"
#include "comm_player_manager.h"
#include "field_comm_manager.h"
#include "unk_0205A0D8.h"
#include "unk_0205D8CC.h"
#include "player_avatar.h"
#include "map_object.h"
#include "unk_020655F4.h"
#include "pokemon.h"
#include "party.h"
#include "unk_0207A274.h"
#include "unk_0207D3B8.h"
#include "pokemon_summary_app.h"
#include "overlay005/ov5_021D0D80.h"

typedef struct {
    PokemonSummary * unk_00;
    PartyManagementData * unk_04;
    UnkFuncPtr_0205AB10 * unk_08;
    Strbuf* unk_0C;
    Strbuf* unk_10;
    Window unk_14;
    FieldSystem * fieldSystem;
    StringTemplate * unk_28;
    MessageLoader * unk_2C;
    int unk_30;
    int unk_34;
    int unk_38;
    u8 unk_3C;
    u8 unk_3D[6];
    u8 unk_43;
    u8 unk_44;
    u8 * unk_48;
    u8 * unk_4C;
    Party * unk_50;
    Window unk_54;
    Window unk_64;
    TrainerInfo * unk_74;
    UnkStruct_020149F0 * unk_78;
    Window * unk_7C;
    u8 unk_80;
    s8 unk_81;
    u8 unk_82;
    u8 unk_83;
    u8 unk_84;
    u8 unk_85;
    u16 unk_86;
    u8 unk_88;
    u8 unk_89;
} UnkStruct_0205A0D8;

typedef struct {
    u32 unk_00;
} UnkStruct_0205AD20;

typedef struct {
    Strbuf* unk_00;
    Strbuf* unk_04;
    Window unk_08;
    StringTemplate * unk_18;
    MessageLoader * unk_1C;
    int unk_20;
    int unk_24;
    int unk_28;
} UnkStruct_0205B2D4;

static void sub_0205AC28(UnkStruct_0205A0D8 * param0);
static void sub_0205AC80(UnkStruct_0205A0D8 * param0, BOOL param1);
static BOOL sub_0205ACC8(UnkStruct_0205A0D8 * param0);
static BOOL sub_0205AD10(UnkStruct_0205A0D8 * param0);
static void sub_0205ADF8(UnkStruct_0205A0D8 * param0, int param1);
static int sub_0205AFE4(UnkStruct_0205A0D8 * param0);
static void sub_0205B0B4(UnkStruct_0205A0D8 * param0);
static BOOL sub_0205AD34(UnkStruct_0205A0D8 * param0);
static BOOL sub_0205AD70(UnkStruct_0205A0D8 * param0);
static void sub_0205ADAC(UnkStruct_0205A0D8 * param0);
static void sub_0205AD80(UnkStruct_0205A0D8 * param0);
static int sub_0205AA50(UnkStruct_0205A0D8 * param0, const Strbuf *param1);
static void sub_0205AAA0(UnkStruct_0205A0D8 * param0, BOOL param1);
static void sub_0205AF18(UnkStruct_0205A0D8 * param0, int param1);
static BOOL sub_0205AD20(UnkStruct_0205A0D8 * param0);

static void sub_0205A0D8 (UnkStruct_0205A0D8 * param0, FieldSystem * fieldSystem, Party * param2, int param3, int param4, int param5)
{
    PokemonSummary * v0;
    SaveData * v1;
    static const u8 v2[] = {
        0, 1, 2, 4, 3, 5, 6, 7, 8
    };

    v1 = fieldSystem->saveData;
    v0 = Heap_AllocFromHeapAtEnd(param5, sizeof(PokemonSummary));

    MI_CpuClear8(v0, sizeof(PokemonSummary));
    PokemonSummary_SetPlayerProfile(v0, SaveData_GetTrainerInfo(fieldSystem->saveData));

    v0->dexMode = sub_0207A274(v1);
    v0->contest = PokemonSummary_ShowContestData(v1);
    v0->options = SaveData_Options(v1);
    v0->monData = param2;
    v0->dataType = 1;
    v0->pos = param3;
    v0->max = (u8)Party_GetCurrentCount(v0->monData);
    v0->move = 0;
    v0->mode = param4;
    v0->ribbons = sub_0202D79C(v1);

    PokemonSummary_FlagVisiblePages(v0, v2);
    sub_0203CD84(fieldSystem, &Unk_020F410C, v0);

    param0->unk_00 = v0;
}

static void sub_0205A164 (UnkStruct_0205A0D8 * param0, int param1)
{
    int v0;
    PartyManagementData * v1 = Heap_AllocFromHeap(param1, sizeof(PartyManagementData));

    MI_CpuClear8(v1, sizeof(PartyManagementData));

    v1->unk_0C = SaveData_Options(param0->fieldSystem->saveData);
    v1->unk_14 = (void *)param0->fieldSystem->unk_B0;
    v1->unk_00 = Party_GetFromSavedata(param0->fieldSystem->saveData);
    v1->unk_04 = sub_0207D990(param0->fieldSystem->saveData);
    v1->unk_21 = 0;
    v1->unk_20 = 2;

    if (param0->fieldSystem->unk_B0) {
        v1->unk_32_0 = sub_02026074(param0->fieldSystem->unk_B0, 1);
        v1->unk_32_4 = v1->unk_32_0;
    } else {
        v1->unk_32_0 = 3;
        v1->unk_32_4 = 3;
    }

    v1->unk_33 = 100;
    v1->unk_22 = param0->unk_3C;

    for (v0 = 0; v0 < 6; v0++) {
        v1->unk_2C[v0] = param0->unk_3D[v0];
    }

    sub_0203CD84(param0->fieldSystem, &Unk_020F1E88, v1);
    param0->unk_04 = v1;
}

static BOOL sub_0205A258 (UnkStruct_0205A0D8 * param0, FieldSystem * fieldSystem)
{
    int v0;

    if (sub_020509B4(fieldSystem)) {
        return 0;
    }

    MI_CpuCopy8(param0->unk_04->unk_2C, param0->unk_3D, 6);

    switch (param0->unk_04->unk_22) {
    case 7:
        param0->unk_38 = 0;
        break;
    case 6:
        param0->unk_38 = 1;
        break;
    default:
        param0->unk_38 = 2;
        break;
    }

    param0->unk_3C = param0->unk_04->unk_22;
    Heap_FreeToHeap(param0->unk_04);
    param0->unk_04 = NULL;

    return 1;
}

static BOOL sub_0205A2B0 (UnkStruct_0205A0D8 * param0, FieldSystem * fieldSystem)
{
    PokemonSummary * v0;

    if (sub_020509B4(fieldSystem)) {
        return 0;
    }

    param0->unk_3C = param0->unk_00->pos;
    Heap_FreeToHeap(param0->unk_00);
    param0->unk_00 = NULL;

    return 1;
}

static BOOL sub_0205A2DC (UnkStruct_0205A0D8 * param0)
{
    if (sub_020509DC(param0->fieldSystem)) {
        ov5_021D1744(1);
        CommPlayerMan_Restart();
        return 1;
    }

    return 0;
}

static BOOL sub_0205A2FC (void)
{
    int v0, v1;

    v1 = CommSys_ConnectedCount();

    for (v0 = 0; v0 < v1; v0++) {
        if (sub_02036564(v0) == 94) {
            return 1;
        }
    }

    return 0;
}

static BOOL sub_0205A324 (TaskManager * param0)
{
    UnkStruct_0205A0D8 * v0 = TaskManager_Environment(param0);
    FieldSystem * fieldSystem = TaskManager_FieldSystem(param0);

    switch (v0->unk_34) {
    case 0:
        v0->unk_43--;

        if (v0->unk_43 == 0) {
            v0->unk_34 = 1;
            CommPlayer_SetBattleDir();
        }
        break;
    case 1:
        MessageLoader_GetStrbuf(v0->unk_2C, 1, v0->unk_0C);
        v0->unk_30 = sub_0205AA50(v0, v0->unk_0C);
        v0->unk_34 = 2;
        break;
    case 2:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            CommTiming_StartSync(93);
            v0->unk_34 = 3;
        }
        break;
    case 3:
        if (CommTiming_IsSyncState(93)) {
            v0->unk_34 = 7;
            sub_02062C30(v0->fieldSystem->mapObjMan);
            v0->unk_08(1, v0->unk_50);
        } else if (gCoreSys.pressedKeys & PAD_BUTTON_B) {
            v0->unk_34 = 4;
            CommTiming_StartSync(92);
            v0->unk_43 = 5;
        }
        break;
    case 4:
        if (CommTiming_IsSyncState(93)) {
            v0->unk_34 = 7;
            sub_02062C30(v0->fieldSystem->mapObjMan);
            v0->unk_08(1, v0->unk_50);
        }

        v0->unk_43--;

        if (v0->unk_43 == 0) {
            v0->unk_34 = 8;
        }
        break;
    case 7:
        sub_0205AC28(v0);
        Heap_FreeToHeap(v0);
        return 1;
    case 5:
        sub_0205AC28(v0);
        Heap_FreeToHeap(v0);
        sub_02059514();
        return 1;
    case 8:
        if (CommTiming_IsSyncState(93)) {
            v0->unk_34 = 5;
            v0->unk_08(1, v0->unk_50);
        } else {
            v0->unk_08(0, v0->unk_50);
            v0->unk_34 = 5;
        }
        break;
    case 9:
        v0->unk_34 = 10;
        v0->unk_44 = 5;
        break;
    case 10:
        if (v0->unk_44 != 0) {
            v0->unk_44--;
        } else {
            if (LocalMapObj_CheckAnimationFinished(Player_MapObject(fieldSystem->playerAvatar))) {
                v0->unk_34 = 11;
            }
        }
        break;
    case 11:
        CommPlayer_SetBattleDir();
        MessageLoader_GetStrbuf(v0->unk_2C, 13, v0->unk_0C);

        v0->unk_30 = sub_0205AA50(v0, v0->unk_0C);
        v0->unk_34 = 12;
        break;
    case 12:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            v0->unk_34 = 13;
        }
        break;
    case 13:
        ov5_021D1744(0);
        v0->unk_34 = 14;
        break;
    case 14:
        v0->unk_43--;

        if (v0->unk_43 == 0) {
            CommPlayer_SetBattleDir();
            v0->unk_34 = 15;
        }
        break;
    case 15:
        sub_0205AAA0(v0, 0);
        sub_0205A164(v0, 11);
        v0->unk_34 = 16;
        break;
    case 16:
        if (sub_0205A258(v0, v0->fieldSystem)) {
            switch (v0->unk_38) {
            case 0:
                v0->unk_34 = 20;
                break;
            case 1:
                v0->unk_34 = 19;
                break;
            case 2:
                v0->unk_34 = 17;
                break;
            }
        }
        break;
    case 17:
        sub_0205A0D8(v0, v0->fieldSystem, Party_GetFromSavedata(v0->fieldSystem->saveData), v0->unk_3C, 0, 11);
        v0->unk_34 = 18;
        break;
    case 18:
        if (sub_0205A2B0(v0, v0->fieldSystem)) {
            v0->unk_34 = 15;
        }
        break;
    case 19:
        sub_020509D4(v0->fieldSystem);

        if (v0->unk_88 != 3) {
            v0->unk_43 = 5;
            v0->unk_34 = 21;
        } else {
            v0->unk_34 = 26;
        }
        break;
    case 21:
        sub_0203898C(v0->unk_3D);

        if (sub_0205A2DC(v0)) {
            v0->unk_43 = 5;
            v0->unk_34 = 0;
        }
        break;
    case 20:
        sub_020509D4(v0->fieldSystem);

        if (v0->unk_88 != 3) {
            v0->unk_34 = 22;
        } else {
            v0->unk_34 = 26;
        }
        break;
    case 22:
        if (sub_0205A2DC(v0)) {
            v0->unk_34 = 8;
        }
        break;
    case 23:
        v0->unk_44--;

        if (v0->unk_44 == 0) {
            v0->unk_34 = 24;
        }
        break;
    case 24:
        CommPlayer_SetBattleDir();
        MessageLoader_GetStrbuf(v0->unk_2C, 19, v0->unk_0C);
        v0->unk_30 = sub_0205AA50(v0, v0->unk_0C);
        v0->unk_34 = 25;
        break;
    case 25:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            v0->unk_34 = 13;
        }
        break;
    case 26:
        if (sub_0205A2DC(v0)) {
            if (sub_0205A2FC()) {
                v0->unk_34 = 5;
            } else {
                v0->unk_82 = (v0->unk_38 != 0);
                sub_0205AC80(v0, v0->unk_82);
                CommTiming_StartSync(0);
                StringTemplate_SetPlayerName(v0->unk_28, 0, v0->unk_74);
                MessageLoader_GetStrbuf(v0->unk_2C, 14, v0->unk_0C);
                StringTemplate_Format(v0->unk_28, v0->unk_10, v0->unk_0C);
                v0->unk_30 = sub_0205AA50(v0, v0->unk_10);
                v0->unk_34 = 27;
            }
        }
        break;
    case 27:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            if (sub_0205A2FC()) {
                v0->unk_34 = 5;
            } else if (CommTiming_IsSyncState(0)) {
                sub_0205ACC8(v0);
                v0->unk_34 = 28;
            }
        }
        break;
    case 28:
        if (sub_0205AD10(v0)) {
            CommTiming_StartSync(1);
            v0->unk_34 = 29;
        }
        break;
    case 29:
        if (CommTiming_IsSyncState(1)) {
            v0->unk_83 = sub_0205AD20(v0);

            if (v0->unk_82 && v0->unk_83) {
                MessageLoader_GetStrbuf(v0->unk_2C, 20, v0->unk_0C);
                v0->unk_30 = sub_0205AA50(v0, v0->unk_0C);
                v0->unk_34 = 30;
            } else {
                v0->unk_34 = 42;
            }
        }
        break;
    case 30:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            v0->unk_89 = 0;
            MessageLoader_GetStrbuf(v0->unk_2C, 17, v0->unk_0C);
            v0->unk_30 = sub_0205AA50(v0, v0->unk_0C);
            v0->unk_84 = 0;
            v0->unk_34 = 31;
        }
        break;
    case 31:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            sub_0205AD80(v0);
            sub_0205ADF8(v0, v0->unk_84);
            v0->unk_34 = 32;
        }
        break;
    case 32:
        switch (sub_0205AFE4(v0)) {
        case 1:
            sub_0205B0B4(v0);
            v0->unk_84 = v0->unk_81;
            v0->unk_34 = 36;
            break;
        case 2:
            sub_0205B0B4(v0);
            v0->unk_84 = 255;
            MessageLoader_GetStrbuf(v0->unk_2C, 15, v0->unk_0C);
            v0->unk_30 = sub_0205AA50(v0, v0->unk_0C);
            CommTiming_StartSync(2);
            v0->unk_34 = 39;
            break;
        }
        break;
    case 36:
        StringTemplate_SetSpeciesName(v0->unk_28, 1, Pokemon_GetBoxPokemon(Party_GetPokemonBySlotIndex(v0->unk_50, v0->unk_84)));
        MessageLoader_GetStrbuf(v0->unk_2C, 18, v0->unk_0C);
        StringTemplate_Format(v0->unk_28, v0->unk_10, v0->unk_0C);
        v0->unk_30 = sub_0205AA50(v0, v0->unk_10);
        v0->unk_34 = 37;
        break;
    case 37:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            sub_0205AF18(v0, 0);
            v0->unk_34 = 38;
        }
        break;
    case 38:
        switch (sub_0205AFE4(v0)) {
        case 2:
            sub_0205B0B4(v0);
            MessageLoader_GetStrbuf(v0->unk_2C, 17, v0->unk_0C);
            v0->unk_30 = sub_0205AA50(v0, v0->unk_0C);
            v0->unk_34 = 31;
            break;
        case 1:
            if (v0->unk_81 == 1) {
                sub_0205B0B4(v0);
                MessageLoader_GetStrbuf(v0->unk_2C, 14, v0->unk_0C);
                StringTemplate_Format(v0->unk_28, v0->unk_10, v0->unk_0C);
                v0->unk_30 = sub_0205AA50(v0, v0->unk_10);
                CommTiming_StartSync(2);
                v0->unk_34 = 39;
            } else {
                ov5_021D1744(0);
                v0->unk_34 = 33;
            }
            break;
        }
        break;
    case 33:
        if (ScreenWipe_Done()) {
            sub_0205AAA0(v0, 0);
            sub_0205A0D8(v0, v0->fieldSystem, v0->unk_50, v0->unk_84, 1, 11);
            v0->unk_34 = 34;
        }
        break;
    case 34:
        if (sub_0205A2B0(v0, v0->fieldSystem)) {
            sub_020509D4(v0->fieldSystem);
            v0->unk_34 = 35;
        }
        break;
    case 35:
        if (sub_0205A2DC(v0)) {
            v0->unk_34 = 36;
        }
        break;
    case 39:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            if (CommTiming_IsSyncState(2)) {
                sub_0205AD34(v0);
                v0->unk_34 = 41;
            }
        }
        break;
    case 41:
        if (sub_0205AD70(v0)) {
            if (v0->unk_84 == 255) {
                CommTiming_StartSync(4);
                v0->unk_34 = 44;
            } else if (v0->unk_85 == 255) {
                v0->unk_34 = 42;
            } else {
                sub_0205ADAC(v0);
                CommTiming_StartSync(93);
                v0->unk_34 = 2;
            }
        }
        break;
    case 42:
        MessageLoader_GetStrbuf(v0->unk_2C, 15, v0->unk_0C);
        v0->unk_30 = sub_0205AA50(v0, v0->unk_0C);
        v0->unk_43 = 0;
        v0->unk_34 = 43;
        break;
    case 43:
        if (FieldMessage_FinishedPrinting(v0->unk_30)) {
            if (++(v0->unk_43) > 60) {
                CommTiming_StartSync(4);
                v0->unk_34 = 44;
            }
        }
        break;
    case 44:
        if (CommTiming_IsSyncState(4)) {
            sub_0200E084(&(v0->unk_14), 0);
            v0->unk_08(0, NULL);
            v0->unk_34 = 5;
        }
        break;
    }

    return 0;
}

static int sub_0205AA50 (UnkStruct_0205A0D8 * param0, const Strbuf *param1)
{
    Window * v0 = &(param0->unk_14);

    if (BGL_WindowAdded(v0) == 0) {
        FieldMessage_AddWindow(param0->fieldSystem->unk_08, v0, 3);
        FieldMessage_DrawWindow(v0, SaveData_Options(param0->fieldSystem->saveData));
    } else {
        sub_0205D988(v0);
    }

    return FieldMessage_Print(v0, (Strbuf *)param1, SaveData_Options(param0->fieldSystem->saveData), 1);
}

static void sub_0205AAA0 (UnkStruct_0205A0D8 * param0, BOOL param1)
{
    if (BGL_WindowAdded(&(param0->unk_14))) {
        if (param1) {
            sub_0200E084(&param0->unk_14, 0);
            sub_0201ACF4(&param0->unk_14);
        }

        BGL_DeleteWindow(&param0->unk_14);
        Window_Init(&param0->unk_14);
    }

    if (BGL_WindowAdded(&(param0->unk_54))) {
        BGL_DeleteWindow(&param0->unk_54);
        Window_Init(&param0->unk_54);
    }

    if (BGL_WindowAdded(&(param0->unk_64))) {
        BGL_DeleteWindow(&param0->unk_64);
        Window_Init(&param0->unk_64);
    }
}

void sub_0205AB10 (FieldSystem * fieldSystem, UnkFuncPtr_0205AB10 * param1)
{
    UnkStruct_0205A0D8 * v0;
    TaskManager * v1 = fieldSystem->unk_10;

    if (v1) {
        return;
    }

    v0 = Heap_AllocFromHeapAtEnd(11, sizeof(UnkStruct_0205A0D8));
    MI_CpuClear8(v0, sizeof(UnkStruct_0205A0D8));

    v0->unk_43 = 5;
    v0->fieldSystem = fieldSystem;
    v0->unk_08 = param1;
    v0->unk_28 = StringTemplate_Default(11);
    v0->unk_2C = MessageLoader_Init(0, 26, 11, 11);
    v0->unk_0C = Strbuf_Init((100 * 2), 11);
    v0->unk_10 = Strbuf_Init((100 * 2), 11);

    Window_Init(&v0->unk_14);
    Window_Init(&v0->unk_54);
    Window_Init(&v0->unk_64);

    v0->unk_78 = sub_020149F0(11);
    v0->unk_88 = sub_0203895C();
    v0->unk_4C = NULL;
    v0->unk_48 = NULL;
    v0->unk_50 = NULL;
    v0->unk_89 = 0;
    v0->unk_86 = CommSys_CurNetId();
    v0->unk_74 = CommInfo_TrainerInfo(v0->unk_86 ^ 1);

    switch (v0->unk_88) {
    case 3:
    {
        u32 v2 = sub_0205B0E4();

        v0->unk_4C = Heap_AllocFromHeapAtEnd(11, v2);
        v0->unk_48 = Heap_AllocFromHeapAtEnd(11, v2);
        v0->unk_50 = Party_New(11);

        Party_InitWithCapacity(v0->unk_50, 3);

        v0->unk_44 = 5;
        v0->unk_34 = 23;
    }
    break;
    case 4:
        v0->unk_34 = 9;
        break;
    default:
        if (v0->fieldSystem->unk_B0) {
            v0->unk_34 = 9;
        } else {
            v0->unk_34 = 0;
        }
        break;
    }

    sub_02050904(fieldSystem, sub_0205A324, v0);
}

static void sub_0205AC28 (UnkStruct_0205A0D8 * param0)
{
    if (param0->unk_50) {
        Heap_FreeToHeap(param0->unk_50);
    }

    if (param0->unk_4C) {
        Heap_FreeToHeap(param0->unk_4C);
    }

    if (param0->unk_48) {
        Heap_FreeToHeap(param0->unk_48);
    }

    MessageLoader_Free(param0->unk_2C);
    StringTemplate_Free(param0->unk_28);
    Strbuf_Free(param0->unk_0C);
    Strbuf_Free(param0->unk_10);
    sub_02014A20(param0->unk_78);

    sub_0205AAA0(param0, 1);
}

static UnkStruct_0205A0D8 * sub_0205AC74 (FieldSystem * fieldSystem)
{
    return TaskManager_Environment(fieldSystem->unk_10);
}

static void sub_0205AC80 (UnkStruct_0205A0D8 * param0, BOOL param1)
{
    Party * v0;
    UnkStruct_0205AD20 * v1;
    u8 * v2;
    int v3, v4;

    v0 = Party_GetFromSavedata(param0->fieldSystem->saveData);
    v2 = param0->unk_4C;
    v4 = Pokemon_GetStructSize();
    v1 = (UnkStruct_0205AD20 *)(v2 + v4 * 3);
    v1->unk_00 = param1;

    if (v1->unk_00) {
        for (v3 = 0; v3 < 3; v3++) {
            MI_CpuCopy8(Party_GetPokemonBySlotIndex(v0, param0->unk_3D[v3] - 1), v2, v4);
            v2 += v4;
        }
    }
}

static BOOL sub_0205ACC8 (UnkStruct_0205A0D8 * param0)
{
    if (param0->unk_89 & 1) {
        return 1;
    } else {
        BOOL v0;
        u8 * v1;
        u32 v2;

        v1 = param0->unk_4C;
        v2 = sub_0205B0E4();

        if (param0->unk_86 == 0) {
            v0 = sub_02035A3C(106, v1, v2);
        } else {
            v0 = CommSys_SendDataHuge(106, v1, v2);
        }

        if (v0) {
            param0->unk_89 |= 1;
        }

        return v0;
    }
}

static BOOL sub_0205AD10 (UnkStruct_0205A0D8 * param0)
{
    if (param0->unk_89 == 3) {
        return 1;
    }

    return 0;
}

static BOOL sub_0205AD20 (UnkStruct_0205A0D8 * param0)
{
    UnkStruct_0205AD20 * v0;

    v0 = (UnkStruct_0205AD20 *)((u8 *)(param0->unk_48) + (Pokemon_GetStructSize() * 3));
    return v0->unk_00;
}

static BOOL sub_0205AD34 (UnkStruct_0205A0D8 * param0)
{
    BOOL v0;

    if (param0->unk_86 == 0) {
        v0 = CommSys_SendDataServer(107, &(param0->unk_84), 1);
    } else {
        v0 = CommSys_SendData(107, &(param0->unk_84), 1);
    }

    if (v0) {
        param0->unk_89 |= 1;
    }

    return v0;
}

static BOOL sub_0205AD70 (UnkStruct_0205A0D8 * param0)
{
    if (param0->unk_89 == 3) {
        return 1;
    }

    return 0;
}

static void sub_0205AD80 (UnkStruct_0205A0D8 * param0)
{
    u32 v0;
    int v1;

    v0 = Pokemon_GetStructSize();
    Party_InitWithCapacity(param0->unk_50, 3);

    for (v1 = 0; v1 < 3; v1++) {
        Party_AddPokemon(param0->unk_50, (Pokemon *)(&param0->unk_48[v1 * v0]));
    }
}

static void sub_0205ADAC (UnkStruct_0205A0D8 * param0)
{
    u32 v0;
    u8 * v1, * v2;
    int v3;

    v0 = Pokemon_GetStructSize();
    v1 = &param0->unk_4C[param0->unk_85 * v0];
    v2 = &param0->unk_48[param0->unk_84 * v0];

    MI_CpuCopy8(v2, v1, v0);

    Party_InitWithCapacity(param0->unk_50, 3);

    for (v3 = 0; v3 < 3; v3++) {
        Party_AddPokemon(param0->unk_50, (Pokemon *)(&param0->unk_4C[v3 * v0]));
    }
}

static void sub_0205ADF8 (UnkStruct_0205A0D8 * param0, int param1)
{
    Window * v0 = &(param0->unk_54);

    if (BGL_WindowAdded(v0) == 0) {
        int v1, v2, v3;
        MessageLoader * v4;

        v4 = MessageLoader_Init(1, 26, 412, 4);
        v3 = Pokemon_GetStructSize();

        BGL_AddWindow(param0->fieldSystem->unk_08, v0, 3, 21, 9, 10, 8, 13, 10);
        sub_0200DAA4(param0->fieldSystem->unk_08, 3, 1, 11, 0, 4);
        BGL_FillWindow(v0, 15);

        for (v1 = 0; v1 < 3; v1++) {
            v2 = Pokemon_GetValue((Pokemon *)(&param0->unk_48[v1 * v3]), MON_DATA_SPECIES, NULL);

            MessageLoader_GetStrbuf(v4, v2, param0->unk_0C);
            PrintStringSimple(v0, 0, param0->unk_0C, 16, v1 * 16, 0xff, NULL);
        }

        MessageLoader_GetStrbuf(param0->unk_2C, 21, param0->unk_0C);
        PrintStringSimple(v0, 0, param0->unk_0C, 16, v1 * 16, 0xff, NULL);
        MessageLoader_Free(v4);
    }

    BGL_WindowColor(v0, 15, 0, 0, 16, v0->unk_08 * 8);
    sub_02014A58(param0->unk_78, &param0->unk_54, 0, param1 * 16);
    Window_Show(&param0->unk_54, 0, 1, 11);

    param0->unk_81 = param1;
    param0->unk_80 = 3 + 1;
    param0->unk_7C = v0;
}

static void sub_0205AF18 (UnkStruct_0205A0D8 * param0, int param1)
{
    Window * v0 = &(param0->unk_64);

    if (BGL_WindowAdded(v0) == 0) {
        int v1;

        BGL_AddWindow(param0->fieldSystem->unk_08, v0, 3, 20, 11, 11, 6, 13, 90);
        sub_0200DAA4(param0->fieldSystem->unk_08, 3, 1, 11, 0, 4);
        BGL_FillWindow(v0, 15);

        for (v1 = 0; v1 < 3; v1++) {
            MessageLoader_GetStrbuf(param0->unk_2C, 22 + v1, param0->unk_0C);
            PrintStringSimple(v0, 0, param0->unk_0C, 16, v1 * 16, 0xff, NULL);
        }
    }

    param0->unk_80 = 3;
    param0->unk_7C = v0;
    param0->unk_81 = param1;

    BGL_WindowColor(v0, 15, 0, 0, 16, v0->unk_08 * 8);
    sub_02014A58(param0->unk_78, param0->unk_7C, 0, param1 * 16);
    Window_Show(param0->unk_7C, 0, 1, 11);
}

static int sub_0205AFE4 (UnkStruct_0205A0D8 * param0)
{
    do {
        if (gCoreSys.pressedKeys & PAD_KEY_UP) {
            param0->unk_81 = ((param0->unk_81 == 0) ? (param0->unk_80 - 1) : (param0->unk_81 - 1));
            break;
        }

        if (gCoreSys.pressedKeys & PAD_KEY_DOWN) {
            param0->unk_81 = (param0->unk_81 == (param0->unk_80 - 1)) ?  0 : (param0->unk_81 + 1);
            break;
        }

        if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
            Sound_PlayEffect(1500);

            if (param0->unk_81 < (param0->unk_80 - 1)) {
                return 1;
            } else {
                return 2;
            }
        }

        if (gCoreSys.pressedKeys & PAD_BUTTON_B) {
            Sound_PlayEffect(1500);
            return 2;
        }

        return 0;
    } while (0);

    Sound_PlayEffect(1500);
    BGL_WindowColor(param0->unk_7C, 15, 0, 0, 16, param0->unk_7C->unk_08 * 8);
    sub_02014A58(param0->unk_78, param0->unk_7C, 0, param0->unk_81 * 16);
    sub_0201ACCC(param0->unk_7C);

    return 0;
}

static void sub_0205B0B4 (UnkStruct_0205A0D8 * param0)
{
    Window_Clear(param0->unk_7C, 1);
}

void sub_0205B0C0 (int param0, int param1, void * param2, void * param3)
{
    UnkStruct_0205A0D8 * v0 = sub_0205AC74(param3);

    if (v0->unk_86 != param0) {
        v0->unk_89 |= 2;
    }
}

int sub_0205B0E4 (void)
{
    return Pokemon_GetStructSize() * 3 + sizeof(UnkStruct_0205AD20);
}

u8 * sub_0205B0F4 (int param0, void * param1, int param2)
{
    UnkStruct_0205A0D8 * v0 = sub_0205AC74(param1);

    if (v0->unk_86 != param0) {
        return v0->unk_48;
    } else {
        return NULL;
    }
}

void sub_0205B110 (int param0, int param1, void * param2, void * param3)
{
    UnkStruct_0205A0D8 * v0 = sub_0205AC74(param3);

    if (v0->unk_86 != param0) {
        v0->unk_85 = *((u8 *)param2);
        v0->unk_89 |= 2;
    }
}

static BOOL sub_0205B140 (TaskManager * param0)
{
    FieldSystem * fieldSystem = TaskManager_FieldSystem(param0);
    UnkStruct_0205B2D4 * v1 = TaskManager_Environment(param0);
    TrainerCard * v2 = (TrainerCard *)sub_02059EBC(v1->unk_24, NULL, 0);

    switch (v1->unk_28) {
    case 0:
        v1->unk_18 = StringTemplate_Default(4);
        v1->unk_1C = MessageLoader_Init(0, 26, 11, 4);
        v1->unk_00 = Strbuf_Init((100 * 2), 4);
        v1->unk_04 = Strbuf_Init((100 * 2), 4);

        MessageLoader_GetStrbuf(v1->unk_1C, 2 + v2->unk_03, v1->unk_00);
        StringTemplate_SetPlayerName(v1->unk_18, 0, CommInfo_TrainerInfo(v1->unk_24));
        StringTemplate_Format(v1->unk_18, v1->unk_04, v1->unk_00);
        FieldMessage_AddWindow(fieldSystem->unk_08, &v1->unk_08, 3);
        FieldMessage_DrawWindow(&v1->unk_08, SaveData_Options(fieldSystem->saveData));

        v1->unk_20 = FieldMessage_Print(&v1->unk_08, v1->unk_04, SaveData_Options(fieldSystem->saveData), 1);
        v1->unk_28++;
        break;
    case 1:
        if (FieldMessage_FinishedPrinting(v1->unk_20)) {
            if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
                MessageLoader_Free(v1->unk_1C);
                StringTemplate_Free(v1->unk_18);
                Strbuf_Free(v1->unk_00);
                Strbuf_Free(v1->unk_04);
                sub_0200E084(&v1->unk_08, 0);
                BGL_DeleteWindow(&v1->unk_08);
                ov5_021D1744(0);
                v1->unk_28++;
            }
        }
        break;
    case 2:
        if (ScreenWipe_Done()) {
            v1->unk_28++;
        }
        break;
    case 3:
        sub_0203E09C(fieldSystem, v2);
        v1->unk_28++;
        break;
    case 4:
        if (!sub_020509B4(fieldSystem)) {
            v1->unk_28++;
        }
        break;
    case 5:
        sub_020509D4(fieldSystem);
        v1->unk_28++;
        break;
    case 6:
        if (!sub_020509DC(fieldSystem)) {
            ov5_021D1744(1);
            CommPlayerMan_Restart();
            v1->unk_28++;
        }
        break;
    case 7:
        sub_02059514();
        Heap_FreeToHeap(v1);
        return 1;
    default:
        return 1;
    }

    return 0;
}

void sub_0205B2D4 (FieldSystem * fieldSystem)
{
    int v0;
    int v1 = CommSys_CurNetId();
    int v2 = sub_02058D88(v1);
    int v3 = sub_02058DC0(v1);

    for (v0 = 0; v0 < CommSys_ConnectedCount(); v0++) {
        if (v0 == v1) {
            continue;
        }

        if ((v2 == CommPlayer_XPos(v0)) && (v3 == CommPlayer_ZPos(v0))) {
            UnkStruct_0205B2D4 * v4 = Heap_AllocFromHeapAtEnd(11, sizeof(UnkStruct_0205B2D4));

            v4->unk_24 = v0;
            v4->unk_28 = 0;

            sub_02050904(fieldSystem, sub_0205B140, v4);
            sub_0203D128();
            break;
        }
    }
}
