#include <nitro.h>
#include <string.h>

#include "core_sys.h"

#include "struct_decls/struct_02018340_decl.h"
#include "field/field_system_sub1_decl.h"
#include "savedata.h"
#include "overlay025/poketch_system.h"

#include "overlay005/const_ov5_021F89B0.h"
#include "overlay077/const_ov77_021D742C.h"

#include "struct_defs/struct_0203CC84.h"
#include "field/field_system.h"
#include "field/field_system_sub2_t.h"
#include "overlay005/struct_ov5_021D1CAC.h"

#include "unk_02000C88.h"
#include "game_overlay.h"
#include "overlay_manager.h"
#include "heap.h"
#include "communication_system.h"
#include "unk_02039C80.h"
#include "unk_0203A378.h"
#include "unk_0203A6DC.h"
#include "field_system.h"
#include "unk_020508D4.h"
#include "unk_020530C8.h"
#include "comm_player_manager.h"
#include "unk_0205F180.h"
#include "pokeradar.h"
#include "unk_0207D3B8.h"
#include "unk_0209ACBC.h"
#include "unk_0209C370.h"
#include "overlay005/ov5_021D1C30.h"
#include "overlay005/ov5_021DD6FC.h"
#include "overlay005/ov5_021DFB54.h"
#include "overlay005/ov5_021E1B08.h"
#include "overlay005/ov5_021EA714.h"

FS_EXTERN_OVERLAY(overlay5);
FS_EXTERN_OVERLAY(overlay77);

typedef struct FieldSystem_sub1_t {
    OverlayManager * unk_00;
    OverlayManager * unk_04;
    BOOL unk_08;
    BOOL unk_0C;
};

static FieldSystem * FieldSystem_Init(OverlayManager * param0);
static BOOL FieldSystem_Run(FieldSystem * fieldSystem);
static void sub_0203CE6C(OverlayManager * param0);
static void FieldSystem_Control(FieldSystem * fieldSystem);

static FieldSystem * sFieldSystem;

static int FieldSystem_InitContinue (OverlayManager * overlayMan, int * param1)
{
    UnkStruct_0203CC84 * v0 = OverlayManager_Args(overlayMan);

    sFieldSystem = FieldSystem_Init(overlayMan);

    if (v0->unk_04) {
        sub_02053808(sFieldSystem);
    } else {
        sub_02053704(sFieldSystem);
    }

    v0->unk_04 = 0;
    return 1;
}

static int FieldSystem_InitNewGame (OverlayManager * overlayMan, int * param1)
{
    sFieldSystem = FieldSystem_Init(overlayMan);
    sub_020535CC(sFieldSystem);
    return 1;
}

static int FieldSystem_Main (OverlayManager * overlayMan, int * param1)
{
    FieldSystem * fieldSystem = OverlayManager_Data(overlayMan);

    if (FieldSystem_Run(fieldSystem)) {
        return 1;
    } else {
        return 0;
    }
}

static int FieldSystem_Exit (OverlayManager * overlayMan, int * param1)
{
    sub_0203CE6C(overlayMan);
    sub_02000EC4(FS_OVERLAY_ID(overlay77), &Unk_ov77_021D742C);
    return 1;
}

const OverlayManagerTemplate gFieldSystemNewGameTemplate = {
    FieldSystem_InitNewGame,
    FieldSystem_Main,
    FieldSystem_Exit,
    0xffffffff
};

const OverlayManagerTemplate gFieldSystemContinueTemplate = {
    FieldSystem_InitContinue,
    FieldSystem_Main,
    FieldSystem_Exit,
    0xffffffff
};

void sub_0203CD00 (FieldSystem * fieldSystem)
{
    GF_ASSERT(fieldSystem->unk_00->unk_04 == NULL);
    GF_ASSERT(fieldSystem->unk_00->unk_00 == NULL);
    Overlay_LoadByID(FS_OVERLAY_ID(overlay5), 2);

    fieldSystem->unk_68 = 0;
    fieldSystem->unk_00->unk_08 = 0;
    fieldSystem->unk_00->unk_00 = OverlayManager_New(&gFieldMapTemplate, fieldSystem, 11);
}

void sub_0203CD44 (FieldSystem * fieldSystem)
{
    fieldSystem->unk_68 = 0;
}

BOOL sub_0203CD4C (FieldSystem * fieldSystem)
{
    return fieldSystem->unk_00->unk_00 != NULL;
}

BOOL sub_0203CD5C (FieldSystem * fieldSystem)
{
    if ((fieldSystem->unk_00->unk_00 != NULL) && fieldSystem->unk_68) {
        return 1;
    } else {
        return 0;
    }
}

BOOL sub_0203CD74 (FieldSystem * fieldSystem)
{
    return fieldSystem->unk_00->unk_04 != NULL;
}

void sub_0203CD84 (FieldSystem * fieldSystem, const OverlayManagerTemplate * param1, void * param2)
{
    GF_ASSERT(fieldSystem->unk_00->unk_04 == NULL);
    sub_0203CD44(fieldSystem);
    fieldSystem->unk_00->unk_04 = OverlayManager_New(param1, param2, 11);
}

static FieldSystem * FieldSystem_Init (OverlayManager * overlayMan)
{
    UnkStruct_0203CC84 * v0;
    FieldSystem * fieldSystem;

    Heap_Create(3, 11, 0x1c000);
    Heap_Create(3, 32, 0x4000);
    Heap_Create(0, 91, 0x300);

    fieldSystem = OverlayManager_NewData(overlayMan, sizeof(FieldSystem), 11);
    MI_CpuClear8(fieldSystem, sizeof(FieldSystem));

    fieldSystem->unk_00 = Heap_AllocFromHeap(11, sizeof(FieldSystem_sub1));
    fieldSystem->unk_00->unk_00 = NULL;
    fieldSystem->unk_00->unk_04 = NULL;
    fieldSystem->unk_00->unk_08 = 0;
    fieldSystem->unk_00->unk_0C = 0;

    v0 = OverlayManager_Args(overlayMan);

    fieldSystem->saveData = v0->unk_08;
    fieldSystem->unk_10 = NULL;
    fieldSystem->unk_1C = sub_0203A720(sub_0203A790(fieldSystem->saveData));
    fieldSystem->unk_2C = sub_02039D6C();

    sub_0203A378(fieldSystem, 11);

    fieldSystem->unk_98 = sub_0207D99C(11);
    fieldSystem->chain = RadarChain_Init(11);

    RadarChain_Clear(fieldSystem->chain);

    fieldSystem->unk_B4 = sub_0209ACBC(11);
    fieldSystem->unk_BC = sub_0209C370(11);

    return fieldSystem;
}

static void sub_0203CE6C (OverlayManager * overlayMan)
{
    FieldSystem * v0 = OverlayManager_Data(overlayMan);

    sub_02039DE4(v0->unk_2C);
    sub_0203A398(v0);
    Heap_FreeToHeap(v0->unk_98);
    RadarChain_Free(v0->chain);
    sub_0209ACDC(v0->unk_B4);
    sub_0209C388(v0->unk_BC);

    Heap_FreeToHeap(v0->unk_00);
    OverlayManager_FreeData(overlayMan);
    Heap_Destroy(91);
    Heap_Destroy(11);
    Heap_Destroy(32);
}

static void sub_0203CECC (OverlayManager ** overlayMan)
{
    if (*overlayMan) {
        if (OverlayManager_Exec(*overlayMan)) {
            OverlayManager_Free(*overlayMan);
            *overlayMan = NULL;
        }
    }
}

BOOL FieldSystem_Run (FieldSystem * fieldSystem)
{
    BOOL v0;

    FieldSystem_Control(fieldSystem);
    v0 = sub_02050958(fieldSystem);

    if ((v0 == 1) && (fieldSystem->unk_04 != NULL)) {
        ov5_021EA714(fieldSystem, 0, 0);
    }

    if (fieldSystem->unk_00->unk_00) {
        sub_0203CECC(&fieldSystem->unk_00->unk_00);

        if (fieldSystem->unk_00->unk_00 == NULL) {
            Overlay_UnloadByID(FS_OVERLAY_ID(overlay5));
        }
    } else if (fieldSystem->unk_00->unk_04) {
        sub_0203CECC(&fieldSystem->unk_00->unk_04);
    }

    if (fieldSystem->unk_00->unk_0C && !fieldSystem->unk_10 && !fieldSystem->unk_00->unk_00 && !fieldSystem->unk_00->unk_04) {
        return 1;
    }

    return 0;
}

void FieldSystem_Control (FieldSystem * fieldSystem)
{
    int v0;
    UnkStruct_ov5_021D1CAC v1;
    BOOL v2 = 0;

    if (!fieldSystem->unk_00->unk_08 && fieldSystem->unk_68 && (sub_020509A4(fieldSystem) == 0)) {
        v2 = 1;
    }

    if (v2) {
        sub_0205F490(fieldSystem->playerAvatar);
        ov5_021D1CAC(&v1, fieldSystem, gCoreSys.pressedKeys, gCoreSys.heldKeys);
    }

    v0 = fieldSystem->unk_70;

    if (fieldSystem->unk_1C->unk_00 == 326) {
        v0 = 0;
    }

    switch (v0) {
    case 1:
        if (v2) {
            if (sub_02058C40()) {
                if (ov5_021D213C(&v1, fieldSystem) == 1) {
                    
                }
            }
        }

        sub_0205805C(fieldSystem, v2);
        break;
    case 3:
        if (v2) {
            if (sub_02058C40()) {
                if (ov5_021D219C(&v1, fieldSystem) == 1) {
                    v2 = 0;
                }
            } else {
                v2 = 0;
            }
        }

        sub_0205805C(fieldSystem, v2);
        break;
    case 2:
        if (v2) {
            if (ov5_021D2298(&v1, fieldSystem) == 1) {
                
            } else {
                PlayerAvatar_MoveControl(fieldSystem->playerAvatar, fieldSystem->unk_28, -1, v1.unk_04, v1.unk_06, 0);
            }
        }
        break;
    case 4:
        if (v2) {
            if (ov5_021D2368(&v1, fieldSystem) == 1) {
                ov5_021DDA78(fieldSystem->unk_04->unk_08);
                ov5_021E1BCC(fieldSystem, 4);
                ov5_021E0EEC(fieldSystem->playerAvatar);
                ov5_021EA714(fieldSystem, 0, 1);
            } else {
                if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
                    ov5_021DDA78(fieldSystem->unk_04->unk_08);
                }

                {
                    BOOL v3 = 0;
                    PoketchSystem * v4 = sub_0203D158();

                    if (v4 != NULL) {
                        v3 = ov25_02253DD4(v4);
                    }

                    PlayerAvatar_MoveControl(fieldSystem->playerAvatar, fieldSystem->unk_28, -1, v1.unk_04, v1.unk_06, v3);
                }
            }
        }
        break;
    default:
        if (v2) {
            if (ov5_021D1DA4(&v1, fieldSystem) == 1) {
                ov5_021DDA78(fieldSystem->unk_04->unk_08);
                ov5_021E1BCC(fieldSystem, 4);
                sub_0205F56C(fieldSystem->playerAvatar);
                ov5_021E0EEC(fieldSystem->playerAvatar);
                ov5_021EA714(fieldSystem, 0, 1);
            } else {
                if (gCoreSys.pressedKeys & PAD_BUTTON_A) {
                    ov5_021DDA78(fieldSystem->unk_04->unk_08);
                }

                {
                    BOOL v5 = 0;
                    PoketchSystem * v6 = sub_0203D158();

                    if (v6 != NULL) {
                        v5 = ov25_02253DD4(v6);
                    }

                    PlayerAvatar_MoveControl(fieldSystem->playerAvatar, fieldSystem->unk_28, -1, v1.unk_04, v1.unk_06, v5);
                }
            }
        }
        break;
    }
}

void sub_0203D128 (void)
{
    sFieldSystem->unk_00->unk_08 = 1;
    CommSys_DisableSendMovementData();
}

void sub_0203D140 (void)
{
    sFieldSystem->unk_00->unk_08 = 0;
    CommSys_EnableSendMovementData();
}

struct PoketchSystem * sub_0203D158 (void)
{
    if (sFieldSystem->unk_04 == NULL) {
        return NULL;
    }

    return sFieldSystem->unk_04->unk_14;
}

BGL * sub_0203D170 (void * param0)
{
    FieldSystem * v0 = (FieldSystem *)param0;
    return v0->unk_08;
}

SaveData * FieldSystem_SaveData (void * param0)
{
    return ((FieldSystem *)param0)->saveData;
}
