#include <nitro.h>
#include <string.h>

#include "overlay099/const_ov99_021D4CAC.h"

#include "struct_defs/struct_0200D0F4.h"
#include "overlay099/struct_ov99_021D2CB0.h"
#include "overlay099/struct_ov99_021D340C.h"
#include "overlay099/struct_ov99_021D35E8.h"
#include "overlay099/struct_ov99_021D3A40.h"

#include "unk_02002F38.h"
#include "unk_0200A9DC.h"
#include "unk_0200C6E4.h"
#include "unk_0201D15C.h"
#include "overlay099/ov99_021D340C.h"
#include "overlay099/ov99_021D4134.h"

typedef struct {
    s16 unk_00;
    s16 unk_02;
    fx32 unk_04;
    fx32 unk_08;
    fx32 unk_0C;
    fx32 unk_10;
    s32 unk_14;
    u16 unk_18;
} UnkStruct_ov99_021D4C90;

typedef struct {
    s16 unk_00;
    s16 unk_02;
} UnkStruct_ov99_021D4C84;

static void ov99_021D3588(UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1);
static void ov99_021D35C8(UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1);
static void ov99_021D35E8(UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1, CellActorData * param2, int param3);
static void ov99_021D3930(UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1);
static void ov99_021D36B0(UnkStruct_ov99_021D2CB0 * param0);
static void ov99_021D36D4(UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1);
static void ov99_021D372C(UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1, int param2);
static void ov99_021D37E0(UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1);

static const UnkStruct_ov99_021D4C90 Unk_ov99_021D4C90[] = {
    {0x130, -10, -0xe00, 0x600, 0x8000, 0xC000, 0x96, 0x14}
};

static const UnkStruct_ov99_021D4C84 Unk_ov99_021D4C84[] = {
    {0xC8, 0x0},
    {0x90, 0x0},
    {0xB4, 0x0}
};

__attribute__((aligned(4))) const u16 Unk_ov99_021D4CAC[] = {
    0x0,
    0x0,
    0x0,
    0x0,
    0x0,
    0x1,
    0x0,
    0x0,
    0x1,
    0x0,
    0x0,
    0x1,
    0x1,
    0x0,
    0x0,
    0x1,
    0x0,
    0x1,
    0x0,
    0x1,
    0x1,
    0x1,
    0x1,
    0x0,
    0x1,
    0x1,
    0x1,
    0x1
};

BOOL ov99_021D340C (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D3A40 * param1)
{
    UnkStruct_ov99_021D340C * v0 = &param1->unk_08_val3;
    s32 v1;

    switch (param1->unk_00) {
    case 0:
    {
        int v2;

        for (v2 = 0; v2 < 16; v2++) {
            MI_CpuClear8(&param0->unk_6C[0][v2].unk_00, sizeof(NNSG3dRenderObj));

            if (v2 % 8 == 0) {
                v1 = 3;
            } else if (v2 >= NELEMS(Unk_ov99_021D4CAC)) {
                v1 = 1;
            } else {
                v1 = Unk_ov99_021D4CAC[v2];
            }

            NNS_G3dRenderObjInit(&param0->unk_6C[0][v2].unk_00, param0->unk_2C[v1].unk_08);
        }

        v0->unk_120 = 16;
    }
        ov99_021D3588(param0, v0);
        ov99_021D36B0(param0);
        sub_0200AAE0(24, 0, -16, (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD), 3);
        param1->unk_00++;
        break;
    case 1:
        if (sub_0200AC1C(3) == 1) {
            G2_SetBlendAlpha(0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ, 31, 0);
            param1->unk_00++;
        }
        break;
    case 2:
        if (param0->unk_10FC >= 7980) {
            sub_0200AAE0(24, -16, 0, (GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_OBJ | GX_BLEND_PLANEMASK_BD), 3);
            param1->unk_00++;
        }
        break;
    case 3:
        if (sub_0200AC1C(3) == 1) {
            return 1;
        }
        break;
    }

    if (v0->unk_120 % 8 == 0) {
        v1 = 3;
    } else if (v0->unk_120 >= NELEMS(Unk_ov99_021D4CAC)) {
        v1 = 1;
    } else {
        v1 = Unk_ov99_021D4CAC[v0->unk_120];
    }

    if (ov99_021D425C(param0, v1) == 1) {
        v0->unk_120++;
    }

    ov99_021D44CC(param0, param0->unk_10E0[0]);
    ov99_021D35C8(param0, v0);
    ov99_021D36D4(param0, v0);
    ov99_021D3930(param0, v0);

    return 0;
}

static void ov99_021D3588 (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1)
{
    int v0, v1 = 0;

    GF_ASSERT(NELEMS(Unk_ov99_021D4C90) == 2 - 2 + 1);

    for (v0 = 2; v0 <= 2; v0++, v1++) {
        sub_0200D6A4(param0->unk_10E0[v0], 2);
        sub_0200D500(param0->unk_10E0[v0], Unk_ov99_021D4C90[v1].unk_00, Unk_ov99_021D4C90[v1].unk_02, ((192 + 80) << FX32_SHIFT));
        param1->unk_124[v1].unk_00 = Unk_ov99_021D4C90[v1].unk_00 * FX32_ONE;
        param1->unk_124[v1].unk_04 = Unk_ov99_021D4C90[v1].unk_02 * FX32_ONE;
    }
}

static void ov99_021D35C8 (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1)
{
    int v0, v1 = 0;

    if (param0->unk_10FC < 7440) {
        return;
    }

    for (v0 = 2; v0 <= 2; v0++, v1++) {
        ov99_021D35E8(param0, param1, param0->unk_10E0[v0], v1);
    }
}

static void ov99_021D35E8 (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1, CellActorData * param2, int param3)
{
    UnkStruct_ov99_021D35E8 * v0 = &param1->unk_124[param3];
    const UnkStruct_ov99_021D4C90 * v1 = &Unk_ov99_021D4C90[param3];
    fx32 v2;

    if (v0->unk_00 < -64 * FX32_ONE) {
        return;
    }

    v0->unk_08 += v1->unk_0C;

    if (v0->unk_08 >= (360 << FX32_SHIFT)) {
        v0->unk_08 -= 360 << FX32_SHIFT;
    }

    v2 = FX_Mul(sub_0201D2B8(v0->unk_08), v1->unk_10);

    v0->unk_00 += v1->unk_04;
    v0->unk_04 += v1->unk_08;

    sub_0200D650(param2, v0->unk_00, v0->unk_04 + v2, ((192 + 80) << FX32_SHIFT));

    if (v0->unk_0E == 0) {
        sub_0200D7C0(param2, v1->unk_14);
    } else {
        sub_0200D7C0(param2, -v1->unk_14);
    }

    v0->unk_0C++;

    if (v0->unk_0C >= v1->unk_18) {
        v0->unk_0C = 0;
        v0->unk_0E ^= 1;
    }
}

static void ov99_021D36B0 (UnkStruct_ov99_021D2CB0 * param0)
{
    SpriteActor_SetOAMMode(param0->unk_10E0[3], GX_OAM_MODE_XLU);
    SpriteActor_SetOAMMode(param0->unk_10E0[4], GX_OAM_MODE_XLU);
}

static void ov99_021D36D4 (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1)
{
    if (param0->unk_10FC == 7200) {
        ov99_021D372C(param0, param1, 0);
    }

    if (param0->unk_10FC == 7380) {
        ov99_021D372C(param0, param1, 1);
    }

    if (param0->unk_10FC == 7440) {
        ov99_021D372C(param0, param1, 2);
    }

    ov99_021D37E0(param0, param1);
}

static void ov99_021D372C (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1, int param2)
{
    GF_ASSERT(param1->unk_11C == 0);
    GF_ASSERT(NELEMS(Unk_ov99_021D4C84) > param2);

    param1->unk_11C = 1;
    param1->unk_11D = 0;
    param1->unk_11E = 0;
    param1->unk_11F = 31;

    sub_0200D500(param0->unk_10E0[4], Unk_ov99_021D4C84[param2].unk_00, Unk_ov99_021D4C84[param2].unk_02, ((192 + 80) << FX32_SHIFT));
    SpriteActor_EnableObject(param0->unk_10E0[4], 1);
    SpriteActor_EnableObject(param0->unk_10E0[3], 0);

    G2S_SetBlendAlpha(0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ, param1->unk_11E, param1->unk_11F);
    G2_SetBlendAlpha(0, GX_BLEND_PLANEMASK_BG0 | GX_BLEND_PLANEMASK_BG1 | GX_BLEND_PLANEMASK_BG2 | GX_BLEND_PLANEMASK_BG3 | GX_BLEND_PLANEMASK_BD | GX_BLEND_PLANEMASK_OBJ, param1->unk_11F, param1->unk_11E);
}

static void ov99_021D37E0 (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1)
{
    s16 v0, v1;

    if (param1->unk_11C == 0) {
        return;
    }

    if (param1->unk_11D == 0) {
        param1->unk_11E += 3;
        param1->unk_11F -= 3;

        if (param1->unk_11E > 31) {
            param1->unk_11E = 31;
        }

        if (param1->unk_11F < 0) {
            param1->unk_11F = 0;
        }

        G2S_ChangeBlendAlpha(param1->unk_11E, param1->unk_11F);

        sub_0200D5E8(param0->unk_10E0[4], -0x6000, 0xd000);
        sub_0200D5A0(param0->unk_10E0[4], &v0, &v1, ((192 + 80) << FX32_SHIFT));

        if (v1 > 192 + 32) {
            SpriteActor_EnableObject(param0->unk_10E0[4], 0);
            SpriteActor_EnableObject(param0->unk_10E0[3], 1);
            sub_0200D500(param0->unk_10E0[3], v0, -32, ((192 + 80) << FX32_SHIFT));
            param1->unk_11D = 1;
        }
    } else {
        param1->unk_11F += 3;
        param1->unk_11E -= 3;

        if (param1->unk_11F > 31) {
            param1->unk_11F = 31;
        }

        if (param1->unk_11E < 0) {
            param1->unk_11E = 0;
        }

        G2_ChangeBlendAlpha(param1->unk_11E, param1->unk_11F);

        sub_0200D5E8(param0->unk_10E0[3], -0x6000, 0xd000);
        sub_0200D5A0(param0->unk_10E0[3], &v0, &v1, ((192 + 80) << FX32_SHIFT));

        if (v1 > 192 + 32) {
            SpriteActor_EnableObject(param0->unk_10E0[3], 0);
            param1->unk_11D = 0;
            param1->unk_11C = 0;
        }
    }
}

static void ov99_021D3930 (UnkStruct_ov99_021D2CB0 * param0, UnkStruct_ov99_021D340C * param1)
{
    int v0, v1;
    int v2, v3, v4;
    u16 * v5, * v6;

    switch (param1->unk_114) {
    case 0:
        if (param0->unk_10FC < 7620) {
            return;
        }

        if (param1->unk_108 >= 8) {
            return;
        }

        if (param1->unk_118 > 0 * (param1->unk_108 + 1)) {
            param1->unk_114++;
        }
        break;
    case 1:
        v2 = param1->unk_108;
        param1->unk_10C += 0x400;

        if (param1->unk_10C >= (16 << 8)) {
            v3 = 16;
            param1->unk_10C = 0;
            param1->unk_108++;
            param1->unk_114 = 0;
        } else {
            v3 = param1->unk_10C >> 8;
        }

        v4 = v2 + 1;

        if (v4 >= 8) {
            v4 = 8 - 1;
        }

        v5 = sub_0200316C(param0->unk_0C, 0);
        v6 = sub_0200316C(param0->unk_0C, 1);

        for (v1 = 0; v1 < 16; v1++) {
            sub_0200393C(&param1->unk_08[v2][v1], &v5[v1 + 16 * 1], 1, v3, param1->unk_08[v4][v1]);
            sub_0200393C(&param1->unk_08[v2][v1], &v6[v1 + 16 * 1], 1, v3, param1->unk_08[v4][v1]);
        }
        break;
    }

    param1->unk_118++;
}
