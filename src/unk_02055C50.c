#include <nitro.h>
#include <string.h>

#include "core_sys.h"

#include "struct_decls/sys_task.h"
#include "struct_decls/struct_02027854_decl.h"
#include "struct_decls/struct_020508D4_decl.h"
#include "struct_decls/struct_02055CBC_decl.h"
#include "struct_decls/struct_02061AB4_decl.h"
#include "overlay005/struct_ov5_021DF47C_decl.h"

#include "struct_defs/struct_02027BF4.h"
#include "field/field_system.h"
#include "field/field_system_sub2_t.h"
#include "overlay005/struct_ov5_021F8E3C.h"

#include "heap.h"
#include "unk_0201C970.h"
#include "unk_0201CED8.h"
#include "savedata_misc.h"
#include "unk_02027B70.h"
#include "unk_020508D4.h"
#include "unk_02054D00.h"
#include "unk_02055C50.h"
#include "player_avatar.h"
#include "map_object.h"
#include "unk_020655F4.h"
#include "unk_020677F4.h"
#include "unk_0206CCB0.h"
#include "unk_0207D3B8.h"
#include "overlay005/ov5_021DF440.h"
#include "overlay005/ov5_021DFB54.h"

struct UnkStruct_02055CBC_t {
    int unk_00;
    UnkStruct_02027BF4 * unk_04;
    NNSG3dRenderObj unk_08;
    NNSG3dResMdl * unk_5C;
    NNSG3dResFileHeader * unk_60;
};

typedef struct {
    int unk_00;
    int unk_04;
    int unk_08;
    SysTask * unk_0C;
} UnkStruct_020562AC;

static void sub_02055D14(FieldSystem * fieldSystem, UnkStruct_02055CBC * param1);
static void sub_02055D48(UnkStruct_02055CBC * param0);

static u16 sub_02055C50 (int param0)
{
    if (param0 == 0) {
        return 0;
    }

    return param0 + 149 - 1;
}

static u16 sub_02055C60 (int param0)
{
    if (param0 == 0) {
        return 0;
    }

    return param0 - 149 + 1;
}

static u16 sub_02055C70 (int param0)
{
    if (param0 == 0) {
        return 0;
    }

    return param0 + 95 - 1;
}

static int sub_02055C80 (int param0)
{
    if (param0 == 0) {
        return 0;
    }

    return param0 - 95 + 1;
}

UnkStruct_02055CBC * sub_02055C8C (FieldSystem * fieldSystem, int param1)
{
    UnkStruct_02055CBC * v0;

    v0 = Heap_AllocFromHeap(param1, sizeof(UnkStruct_02055CBC));
    MI_CpuClear8(v0, sizeof(UnkStruct_02055CBC));

    v0->unk_00 = param1;
    v0->unk_04 = sub_02027BF4(param1);

    sub_02055D14(fieldSystem, v0);
    return v0;
}

void sub_02055CBC (UnkStruct_02055CBC * param0)
{
    sub_02055D48(param0);
    Heap_FreeToHeap(param0->unk_04);
    Heap_FreeToHeap(param0);
}

void sub_02055CD4 (FieldSystem * fieldSystem, int param1)
{
    UnkStruct_02027854 * v0;
    UnkStruct_02027BF4 * v1;

    if (fieldSystem->unk_04 == NULL) {
        v1 = sub_02027BF4(11);
        v0 = sub_02027854(fieldSystem->saveData);
        sub_02027EAC(v0, v1, param1);
        Heap_FreeToHeap(v1);
    } else {
        v1 = fieldSystem->unk_04->unk_18->unk_04;
        v0 = sub_02027854(fieldSystem->saveData);
        sub_02027EAC(v0, v1, param1);
    }
}

static void sub_02055D14 (FieldSystem * fieldSystem, UnkStruct_02055CBC * param1)
{
    UnkStruct_ov5_021DF47C * v0 = fieldSystem->unk_40;
    u32 v1 = ov5_021DF5A8(v0, 17);

    param1->unk_60 = Heap_AllocFromHeap(param1->unk_00, v1);

    ov5_021DF5B4(v0, 17, param1->unk_60);
    sub_0201CA3C(&param1->unk_08, &param1->unk_5C, &param1->unk_60);
}

static void sub_02055D48 (UnkStruct_02055CBC * param0)
{
    ov5_021DF554(param0->unk_60);
}

static BOOL sub_02055D54 (FieldSystem * fieldSystem, const VecFx32 * param1)
{
    const VecFx32 v0 = {FX32_ONE, FX32_ONE, FX32_ONE};
    MtxFx33 v1;

    MTX_Identity33(&v1);

    if (sub_0201CED8(fieldSystem->unk_04->unk_18->unk_5C, param1, &v1, &v0) != 0) {
        return 1;
    } else {
        return 0;
    }
}

void sub_02055D94 (FieldSystem * fieldSystem)
{
    int v0 = 0;
    MapObject * v1;
    UnkStruct_02027854 * v2 = sub_02027854(fieldSystem->saveData);

    while (sub_020625B0(fieldSystem->mapObjMan, &v1, &v0, (1 << 0)) == 1) {
        if (sub_020677F4(sub_02062920(v1)) == 1) {
            if (sub_02055D54(fieldSystem, MapObject_PosVector(v1))) {
                int v3 = sub_020629D8(v1, 0);
                sub_02027D90(v2, v3, 1);
            }
        }
    }
}

BOOL sub_02055E00 (FieldSystem * fieldSystem, MapObject * param1)
{
    int v0, v1, v2;
    UnkStruct_02027854 * v3 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    v2 = sub_02027D10(v3, v0);
    v1 = sub_02027DB4(v3, v0);

    sub_0206D914(fieldSystem, sub_02055C50(v2), sub_02027D34(v3, v0), v1);
    sub_02027DC0(v3, v0);
    sub_02067834(param1);

    return sub_0207D570(sub_0207D990(fieldSystem->saveData), sub_02055C50(v2), v1, 4);
}

void sub_02055E80 (FieldSystem * fieldSystem, MapObject * param1, u16 param2)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    sub_02027DA8(v1, v0, sub_02055C80(param2));
}

void sub_02055EAC (FieldSystem * fieldSystem, MapObject * param1, u16 param2)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    sub_02027D40(v1, v0, fieldSystem->unk_04->unk_18->unk_04, sub_02055C60(param2));
}

void sub_02055EE0 (FieldSystem * fieldSystem, MapObject * param1)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    sub_02027D78(v1, v0);
}

int sub_02055F00 (const FieldSystem * fieldSystem, const MapObject * param1)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    return sub_02027D04(v1, v0);
}

int sub_02055F20 (const FieldSystem * fieldSystem, const MapObject * param1)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    return sub_02027D10(v1, v0);
}

u16 sub_02055F40 (const FieldSystem * fieldSystem, const MapObject * param1)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    return sub_02055C50(sub_02027D10(v1, v0));
}

u16 sub_02055F64 (const FieldSystem * fieldSystem, const MapObject * param1)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    return sub_02055C70(sub_02027D9C(v1, v0));
}

int sub_02055F88 (const FieldSystem * fieldSystem, const MapObject * param1)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    return sub_02027D18(v1, v0);
}

int sub_02055FA8 (const FieldSystem * fieldSystem, const MapObject * param1)
{
    int v0;
    UnkStruct_02027854 * v1 = sub_02027854(fieldSystem->saveData);

    v0 = sub_020629D8(param1, 0);
    return sub_02027DB4(v1, v0);
}

u32 sub_02055FC8 (const FieldSystem * fieldSystem, const MapObject * param1)
{
    u32 v0 = 0;

    if ((param1 == NULL) || (sub_02062920(param1) != 0x64)) {
        return 0x0;
    }

    switch (sub_02055F00(fieldSystem, param1)) {
    case 0:
        v0 |= 0x1;

        if (sub_02055F64(fieldSystem, param1) == 0) {
            v0 |= 0x2;
        }
        break;
    default:
        v0 |= 0x4;
        break;
    }

    return v0;
}

static const UnkStruct_ov5_021F8E3C Unk_020EC51C[] = {
    {0xA, 0x1},
    {0xfe, 0x0}
};

static const UnkStruct_ov5_021F8E3C Unk_020EC524[] = {
    {0xB, 0x1},
    {0xfe, 0x0}
};

static BOOL sub_02056010 (FieldSystem * fieldSystem, UnkStruct_020562AC * param1, int param2)
{
    int v0, v1;

    v0 = Player_GetXPos(fieldSystem->playerAvatar);
    v1 = Player_GetZPos(fieldSystem->playerAvatar);

    if (param2 == 2) {
        v0--;
    } else if (param2 == 3) {
        v0++;
    } else if (param2 == 0) {
        v1--;
    } else if (param2 == 1) {
        v1++;
    } else {
        GF_ASSERT(0);
    }

    if (FieldSystem_CheckCollision(fieldSystem, v0, v1)) {
        return 1;
    }

    return sub_0206326C(fieldSystem->mapObjMan, v0, v1, 0) != NULL;
}

static MapObject * sub_02056074 (FieldSystem * fieldSystem, int param1)
{
    int v0, v1;

    v0 = Player_GetXPos(fieldSystem->playerAvatar);
    v1 = Player_GetZPos(fieldSystem->playerAvatar);
    v1 -= 1;

    if (param1 == 2) {
        v0 -= 1;
    } else if (param1 == 3) {
        v0 += 1;
    }

    return sub_0206326C(fieldSystem->mapObjMan, v0, v1, 0);
}

static MapObject * sub_020560A8 (FieldSystem * fieldSystem, UnkStruct_020562AC * param1)
{
    int v0, v1;

    v0 = Player_GetXPos(fieldSystem->playerAvatar);
    v1 = Player_GetZPos(fieldSystem->playerAvatar);

    if (param1->unk_04 == 0) {
        v1 -= 1;
    } else if (param1->unk_04 == 1) {
        v1 += 1;
    } else {
        GF_ASSERT(0);
    }

    return sub_0206326C(fieldSystem->mapObjMan, v0, v1, 0);
}

static BOOL sub_020560E4 (MapObject * mapObj)
{
    return sub_02062920(mapObj) == 0x64;
}

static void sub_020560F8 (FieldSystem * fieldSystem, UnkStruct_020562AC * param1)
{
    MapObject * v0;

    v0 = sub_020560A8(fieldSystem, param1);

    if (v0 != NULL) {
        sub_02055EE0(fieldSystem, v0);
    }
}

static void sub_0205610C (FieldSystem * fieldSystem, UnkStruct_020562AC * param1, const UnkStruct_ov5_021F8E3C * param2)
{
    MapObject * v0 = Player_MapObject(fieldSystem->playerAvatar);
    param1->unk_0C = sub_02065700(v0, param2);
}

static BOOL sub_02056124 (TaskManager * taskMan)
{
    FieldSystem * v0 = TaskManager_FieldSystem(taskMan);
    UnkStruct_020562AC * v1 = TaskManager_Environment(taskMan);

    switch (v1->unk_00) {
    case 0:
        PlayerAvatar_SetRequestStateBit(v0->playerAvatar, (1 << 4));
        PlayerAvatar_RequestChangeState(v0->playerAvatar);
        sub_02062DDC(Player_MapObject(v0->playerAvatar));
        v1->unk_00 = 1;
        break;
    case 1:
        sub_020560F8(v0, v1);
        v1->unk_08 = 0;
        v1->unk_00 = 2;
    case 2:
        if (gCoreSys.heldKeys & PAD_KEY_LEFT) {
            MapObject * v2 = sub_02056074(v0, 2);

            if ((v2 == NULL) || !sub_020560E4(v2)) {
                v1->unk_00 = 4;
                break;
            } else if (!sub_02056010(v0, v1, 2)) {
                sub_0205610C(v0, v1, Unk_020EC51C);
                v1->unk_00 = 3;
                break;
            }
        } else if (gCoreSys.heldKeys & PAD_KEY_RIGHT) {
            MapObject * v2 = sub_02056074(v0, 3);

            if ((v2 == NULL) || !sub_020560E4(v2)) {
                v1->unk_00 = 4;
                break;
            } else if (!sub_02056010(v0, v1, 3)) {
                sub_0205610C(v0, v1, Unk_020EC524);
                v1->unk_00 = 3;
                break;
            }
        } else if ((gCoreSys.heldKeys & PAD_KEY_UP) && (v1->unk_04 == 1)) {
            Player_SetDir(v0->playerAvatar, 0);
            v1->unk_00 = 4;
            break;
        } else if ((gCoreSys.heldKeys & PAD_KEY_DOWN) && (v1->unk_04 == 0)) {
            v1->unk_00 = 4;
            break;
        }

        v1->unk_08++;

        if (v1->unk_08 > 30 * 3) {
            Player_SetDir(v0->playerAvatar, v1->unk_04);
            v1->unk_00 = 4;
        }
        break;
    case 3:
        if (sub_0206574C(v1->unk_0C)) {
            MapObject * v3;

            sub_02065758(v1->unk_0C);
            v3 = sub_020560A8(v0, v1);

            if ((v3 != NULL) && sub_020560E4(v3)) {
                v1->unk_00 = 1;
            } else {
                Player_SetDir(v0->playerAvatar, v1->unk_04);
                v1->unk_00 = 4;
            }
        }
        break;
    case 4:
        Player_SetDir(v0->playerAvatar, v1->unk_04);
        sub_02062DD0(Player_MapObject(v0->playerAvatar));
        Heap_FreeToHeap(v1);
        return 1;
    }

    return 0;
}

void sub_020562AC (FieldSystem * fieldSystem)
{
    UnkStruct_020562AC * v0;

    v0 = Heap_AllocFromHeap(32, sizeof(UnkStruct_020562AC));

    v0->unk_00 = 0;
    v0->unk_0C = NULL;
    v0->unk_04 = PlayerAvatar_GetDir(fieldSystem->playerAvatar);

    sub_02050944(fieldSystem->unk_10, sub_02056124, v0);
}

void sub_020562D8 (FieldSystem * fieldSystem)
{
    int v0 = PlayerAvatar_GetPlayerState(fieldSystem->playerAvatar);
    u32 v1 = sub_0205EED8(v0);

    PlayerAvatar_SetRequestStateBit(fieldSystem->playerAvatar, v1);
    PlayerAvatar_RequestChangeState(fieldSystem->playerAvatar);
}
