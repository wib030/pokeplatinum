#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_02018340_decl.h"
#include "struct_decls/sys_task.h"
#include "overlay025/struct_ov25_02255224_decl.h"
#include "overlay025/struct_ov25_022555E8_decl.h"
#include "overlay025/struct_ov25_022558C4_decl.h"
#include "overlay044/struct_ov44_022565BC_decl.h"

#include "overlay025/struct_ov25_0225517C.h"
#include "overlay025/struct_ov25_02255810.h"
#include "overlay025/struct_ov25_02255958.h"
#include "overlay044/struct_ov44_022565BC_1.h"
#include "overlay097/struct_ov97_0222DB78.h"

#include "unk_02006E3C.h"
#include "heap.h"
#include "unk_02018340.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "overlay025/poketch_system.h"
#include "overlay025/ov25_02254560.h"
#include "overlay025/ov25_02255090.h"
#include "overlay025/ov25_02255540.h"
#include "overlay044/ov44_022565BC.h"

struct UnkStruct_ov44_022565BC_t {
    const UnkStruct_ov44_022565BC_1 * unk_00;
    BGL * unk_04;
    u32 unk_08[6];
    UnkStruct_ov25_022555E8 * unk_20;
    UnkStruct_ov25_022558C4 * unk_24[6];
    UnkStruct_ov25_02255958 unk_3C;
    UnkStruct_ov25_02255958 unk_50;
    u32 unk_64;
    BOOL unk_68;
    const int * unk_6C;
    u32 unk_70;
    u32 unk_74;
    fx32 unk_78;
    fx32 unk_7C;
    fx32 unk_80;
    u32 unk_84;
};

static void ov44_0225660C(UnkStruct_ov44_022565BC * param0, const UnkStruct_ov44_022565BC_1 * param1);
static void ov44_02256718(UnkStruct_ov44_022565BC * param0);
static void ov44_02256780(UnkStruct_ov25_02255224 * param0);
static void ov44_02256794(SysTask * param0, void * param1);
static void ov44_02256828(SysTask * param0, void * param1);
static void ov44_0225686C(SysTask * param0, void * param1);
static void ov44_02256898(SysTask * param0, void * param1);
static void ov44_022568BC(SysTask * param0, void * param1);
static void ov44_02256908(SysTask * param0, void * param1);
static void ov44_02256954(UnkStruct_ov44_022565BC * param0, u32 param1, u32 param2, u16 param3, u16 param4);
static void ov44_022569AC(UnkStruct_ov44_022565BC * param0);
static void ov44_022569E4(SysTask * param0, void * param1);
static void ov44_02256A50(UnkStruct_ov44_022565BC * param0, const UnkStruct_ov44_022565BC_1 * param1);
static BOOL ov44_02256AC8(UnkStruct_ov44_022565BC * param0, const UnkStruct_ov44_022565BC_1 * param1);

BOOL ov44_022565BC (UnkStruct_ov44_022565BC ** param0, const UnkStruct_ov44_022565BC_1 * param1, BGL * param2)
{
    UnkStruct_ov44_022565BC * v0 = (UnkStruct_ov44_022565BC *)Heap_AllocFromHeap(HEAP_ID_POKETCH_APP, sizeof(UnkStruct_ov44_022565BC));

    if (v0 != NULL) {
        ov25_02255090(v0->unk_08, 4);
        v0->unk_00 = param1;
        v0->unk_04 = ov25_02254674();
        v0->unk_20 = ov25_02254664();
        ov44_0225660C(v0, param1);
        *param0 = v0;

        return 1;
    }

    return 0;
}

void ov44_022565F8 (UnkStruct_ov44_022565BC * param0)
{
    if (param0 != NULL) {
        ov44_02256718(param0);
        Heap_FreeToHeap(param0);
    }
}

static void ov44_0225660C (UnkStruct_ov44_022565BC * param0, const UnkStruct_ov44_022565BC_1 * param1)
{
    static const UnkStruct_ov25_02255810 v0[] = {
        {
            {(112 << FX32_SHIFT), (148 << FX32_SHIFT)},
            9,
            0,
            2,
            0,
            0
        },
        {
            {(112 << FX32_SHIFT), (32 << FX32_SHIFT)},
            0,
            0,
            2,
            1,
            0
        },
        {
            {(48 << FX32_SHIFT), (88 << FX32_SHIFT)},
            5,
            0,
            2,
            0,
            0
        },
        {
            {(176 << FX32_SHIFT), (88 << FX32_SHIFT)},
            6,
            0,
            2,
            0,
            0
        },
        {
            {(48 << FX32_SHIFT), (140 << FX32_SHIFT)},
            5,
            0,
            2,
            0,
            1
        },
        {
            {(176 << FX32_SHIFT), (140 << FX32_SHIFT)},
            4,
            0,
            2,
            0,
            1
        }
    };
    int v1;
    u32 v2;

    ov25_02255360(1);

    v2 = sub_02006EC0(12, 74, 1, 0, 0, 1, 8);
    v2 /= 20;
    param0->unk_64 = v2;

    ov25_022553A0(v2, param1->unk_00, param1->unk_30, 0);
    ov25_02255958(&param0->unk_3C, 12, 72, 73, 8);
    ov25_02255958(&param0->unk_50, 12, 5, 6, 8);

    for (v1 = 0; v1 < 4; v1++) {
        param0->unk_24[v1] = ov25_02255810(param0->unk_20, &v0[v1], &param0->unk_3C);
    }

    for (; v1 < 6; v1++) {
        param0->unk_24[v1] = ov25_02255810(param0->unk_20, &v0[v1], &param0->unk_50);
    }

    ov44_02256954(param0, 4, param1->unk_32, param1->unk_18[param1->unk_32], param1->unk_24[param1->unk_32]);
    ov44_02256954(param0, 5, param1->unk_33, param1->unk_18[param1->unk_33], param1->unk_24[param1->unk_33]);

    if (param1->unk_30 <= 1) {
        ov25_02255914(param0->unk_24[5], 1);
        ov25_02255914(param0->unk_24[3], 1);
        ov25_022558C4(param0->unk_24[0], 10);
    }
}

static void ov44_02256718 (UnkStruct_ov44_022565BC * param0)
{
    int v0;

    ov25_022559B0(&param0->unk_50);
    ov25_022559B0(&param0->unk_3C);

    for (v0 = 0; v0 < 6; v0++) {
        if (param0->unk_24[v0]) {
            ov25_022558B0(param0->unk_20, param0->unk_24[v0]);
        }
    }
}

static const UnkStruct_ov25_0225517C Unk_ov44_02256D5C[] = {
    {0x0, ov44_02256794, 0x0},
    {0x1, ov44_02256828, 0x0},
    {0x2, ov44_0225686C, 0x0},
    {0x3, ov44_02256898, 0x0},
    {0x4, ov44_022568BC, 0x0},
    {0x5, ov44_02256908, 0x0},
    {0x6, ov44_022569E4, 0x0},
    {0x0, NULL, 0x0}
};

void ov44_02256744 (UnkStruct_ov44_022565BC * param0, u32 param1)
{
    ov25_0225517C(Unk_ov44_02256D5C, param1, param0, param0->unk_00, param0->unk_08, 2, 8);
}

BOOL ov44_02256768 (UnkStruct_ov44_022565BC * param0, u32 param1)
{
    return ov25_02255130(param0->unk_08, param1);
}

BOOL ov44_02256774 (UnkStruct_ov44_022565BC * param0)
{
    return ov25_02255154(param0->unk_08);
}

static void ov44_02256780 (UnkStruct_ov25_02255224 * param0)
{
    UnkStruct_ov44_022565BC * v0 = ov25_0225523C(param0);
    ov25_02255224(v0->unk_08, param0);
}

static void ov44_02256794 (SysTask * param0, void * param1)
{
    static const UnkStruct_ov97_0222DB78 v0 = {
        0,
        0,
        0x800,
        0,
        1,
        GX_BG_COLORMODE_16,
        GX_BG_SCRBASE_0x7000,
        GX_BG_CHARBASE_0x00000,
        GX_BG_EXTPLTT_01,
        2,
        0,
        0,
        0
    };
    GXSDispCnt v1;
    UnkStruct_ov44_022565BC * v2;
    const UnkStruct_ov44_022565BC_1 * v3;
    void * v4;
    NNSG2dPaletteData * v5;

    v2 = ov25_0225523C(param1);
    v3 = ov25_02255240(param1);

    sub_020183C4(v2->unk_04, 6, &v0, 0);
    sub_02006E3C(12, 71, v2->unk_04, 6, 0, 0, 1, 8);
    sub_02006E60(12, 70, v2->unk_04, 6, 0, 0, 1, 8);

    ov25_022546B8(0, 0);
    sub_02019448(v2->unk_04, 6);

    v1 = GXS_GetDispCnt();
    GXS_SetVisiblePlane(v1.visiblePlane | GX_PLANEMASK_BG2);

    ov44_02256780(param1);
}

static void ov44_02256828 (SysTask * param0, void * param1)
{
    UnkStruct_ov44_022565BC * v0 = ov25_0225523C(param1);

    switch (ov25_02255248(param1)) {
    case 0:
        v0->unk_68 = 1;
        ov25_0225524C(param1);
        break;
    case 1:
        if (ov44_02256768(v0, 6)) {
            sub_02019044(v0->unk_04, 6);
            ov44_02256780(param1);
        }
        break;
    }
}

static void ov44_0225686C (SysTask * param0, void * param1)
{
    UnkStruct_ov44_022565BC * v0;
    const UnkStruct_ov44_022565BC_1 * v1;

    v0 = ov25_0225523C(param1);
    v1 = ov25_02255240(param1);

    ov25_02254424(1641);
    ov25_022558C4(v0->unk_24[0], 10);
    ov44_02256780(param1);
}

static void ov44_02256898 (SysTask * param0, void * param1)
{
    UnkStruct_ov44_022565BC * v0;
    const UnkStruct_ov44_022565BC_1 * v1;

    v0 = ov25_0225523C(param1);
    v1 = ov25_02255240(param1);

    ov25_022558C4(v0->unk_24[0], 9);
    ov44_02256780(param1);
}

static void ov44_022568BC (SysTask * param0, void * param1)
{
    UnkStruct_ov44_022565BC * v0 = ov25_0225523C(param1);
    const UnkStruct_ov44_022565BC_1 * v1 = ov25_02255240(param1);
    int v2;

    v2 = v1->unk_32;

    ov25_02254444(v1->unk_18[v2], v1->unk_24[v2]);
    ov44_02256954(v0, 4, v2, v1->unk_18[v2], v1->unk_24[v2]);
    ov44_022569AC(v0);
    ov44_02256780(param1);
}

static void ov44_02256908 (SysTask * param0, void * param1)
{
    UnkStruct_ov44_022565BC * v0 = ov25_0225523C(param1);
    const UnkStruct_ov44_022565BC_1 * v1 = ov25_02255240(param1);
    int v2 = v1->unk_33;

    ov25_02254444(v1->unk_18[v2], v1->unk_24[v2]);
    ov44_02256954(v0, 5, v2, v1->unk_18[v2], v1->unk_24[v2]);
    ov44_022569AC(v0);
    ov44_02256780(param1);
}

static void ov44_02256954 (UnkStruct_ov44_022565BC * param0, u32 param1, u32 param2, u16 param3, u16 param4)
{
    UnkStruct_ov25_022558C4 * v0 = param0->unk_24[param1];
    u32 v1;

    if (param1 == 5) {
        v1 = 4;
    } else {
        if (PokemonPersonalData_GetFormValue(param3, param4, 28)) {
            v1 = 4;
        } else {
            v1 = 5;
        }
    }

    ov25_022558C4(v0, v1);
    ov25_02255940(v0, param0->unk_64 + param2 * 16);
    ov25_02255938(v0, 1 + PokeIconPaletteIndex(param3, param4, 0));
}

static void ov44_022569AC (UnkStruct_ov44_022565BC * param0)
{
    ov25_02255900(param0->unk_24[2], (48 << FX32_SHIFT), (88 << FX32_SHIFT));
    ov25_02255900(param0->unk_24[3], (176 << FX32_SHIFT), (88 << FX32_SHIFT));
    ov25_022558C4(param0->unk_24[2], 5);
    ov25_022558C4(param0->unk_24[3], 6);
    ov25_022558C4(param0->unk_24[1], 0);
}

static void ov44_022569E4 (SysTask * param0, void * param1)
{
    UnkStruct_ov44_022565BC * v0 = ov25_0225523C(param1);
    const UnkStruct_ov44_022565BC_1 * v1 = ov25_02255240(param1);
    u32 v2 = ov25_02255248(param1);

    if ((v2 != 0) && v0->unk_68) {
        ov44_02256780(param1);
        return;
    }

    switch (ov25_02255248(param1)) {
    case 0:
        v0->unk_68 = 0;
        ov44_022569AC(v0);
        ov44_02256A50(v0, v1);
        ov25_0225524C(param1);
        break;
    case 1:
        if (ov44_02256AC8(v0, v1)) {
            ov44_02256780(param1);
        }
        break;
    }
}

static const int Unk_ov44_02256D2C[] = {
    -2,
    0x10,
    0x10,
    -4,
    0x10,
    -6,
    -5,
    0x66C,
    -3,
    0x10,
    0x10,
    -1
};

static const int Unk_ov44_02256CCC[] = {
    -2,
    0x10,
    0x10,
    -5,
    0x66B,
    -1
};

static const int Unk_ov44_02256D00[] = {
    -2,
    0x10,
    0x10,
    -5,
    0x66B,
    -2,
    0x10,
    0x10,
    -5,
    0x66B,
    -1
};

static const int Unk_ov44_02256E1C[] = {
    -2,
    0x10,
    0x10,
    -5,
    0x66B,
    -2,
    0x10,
    0x10,
    -5,
    0x66B,
    -2,
    0x10,
    0x10,
    -5,
    0x66B,
    -4,
    0x10,
    -5,
    0x66D,
    -7,
    0x2,
    0x7,
    -7,
    0x3,
    0x8,
    -7,
    0x1,
    0x4,
    -4,
    0x10,
    -1
};

static void ov44_02256A50 (UnkStruct_ov44_022565BC * param0, const UnkStruct_ov44_022565BC_1 * param1)
{
    param0->unk_70 = 0;
    param0->unk_74 = 0;
    param0->unk_78 = 0;
    param0->unk_84 = 0;

    switch (param1->unk_31) {
    case 3:
        param0->unk_6C = Unk_ov44_02256D2C;
        break;
    case 2:
        param0->unk_6C = Unk_ov44_02256CCC;
        ov25_022558C4(param0->unk_24[1], 1);
        break;
    case 1:
        param0->unk_6C = Unk_ov44_02256D00;
        ov25_022558C4(param0->unk_24[1], 2);
        break;
    case 0:
        param0->unk_6C = Unk_ov44_02256E1C;
        ov25_022558C4(param0->unk_24[1], 3);
        break;
    }

    ov44_02256AC8(param0, param1);
}

static BOOL ov44_02256AC8 (UnkStruct_ov44_022565BC * param0, const UnkStruct_ov44_022565BC_1 * param1)
{
 func_start:
    switch (param0->unk_70) {
    case 0:
        while (param0->unk_70 == 0) {
            switch (param0->unk_6C[param0->unk_74++]) {
            case -1:
                return 1;
            case -2:
                param0->unk_84 = param0->unk_6C[(param0->unk_74)++];
                param0->unk_7C = param0->unk_6C[(param0->unk_74)++];
                param0->unk_7C *= FX32_ONE;
                param0->unk_80 = param0->unk_78 + param0->unk_7C;
                param0->unk_7C /= param0->unk_84;
                param0->unk_70 = 1;
                break;
            case -3:
                param0->unk_84 = param0->unk_6C[(param0->unk_74)++];
                param0->unk_7C = param0->unk_6C[(param0->unk_74)++];
                param0->unk_7C *= FX32_ONE;
                param0->unk_7C *= -1;
                param0->unk_80 = param0->unk_78 + param0->unk_7C;
                param0->unk_7C /= param0->unk_84;
                param0->unk_70 = 1;
                break;
            case -4:
                param0->unk_84 = param0->unk_6C[(param0->unk_74)++];
                param0->unk_70 = 2;
                break;
            case -5:
                ov25_02254424(param0->unk_6C[(param0->unk_74)++]);
                break;
            case -6:
                ov25_022558C4(param0->unk_24[2], 6);
                ov25_022558C4(param0->unk_24[3], 5);
                break;
            case -7:
            {
                u32 v0 = param0->unk_6C[(param0->unk_74)++];
                u32 v1 = param0->unk_6C[(param0->unk_74)++];
                ov25_022558C4(param0->unk_24[v0], v1);
            }
            break;
            }
        }
        break;
    case 1:
        if (param0->unk_84) {
            param0->unk_84--;

            if (param0->unk_84) {
                param0->unk_78 += param0->unk_7C;
            } else {
                param0->unk_78 = param0->unk_80;
            }

            ov25_02255900(param0->unk_24[2], (48 << FX32_SHIFT) + param0->unk_78, (88 << FX32_SHIFT));
            ov25_02255900(param0->unk_24[3], (176 << FX32_SHIFT) - param0->unk_78, (88 << FX32_SHIFT));
        }

        if (param0->unk_84 == 0) {
            param0->unk_70 = 0;
            goto func_start;
        }
        break;
    case 2:
        if (param0->unk_84) {
            param0->unk_84--;
        } else {
            param0->unk_70 = 0;
            goto func_start;
        }

        break;
    }

    return 0;
}
