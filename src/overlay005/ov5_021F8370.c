#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_02027860_decl.h"
#include "overlay005/struct_ov5_021E1890_decl.h"

#include "field/field_system.h"
#include "field/field_system_sub2_t.h"
#include "struct_defs/struct_02071C34.h"

#include "heap.h"
#include "savedata_misc.h"
#include "unk_02027F50.h"
#include "field_system.h"
#include "unk_0203E880.h"
#include "unk_020507CC.h"
#include "unk_0206A8DC.h"
#include "overlay005/ov5_021E15F4.h"
#include "overlay005/ov5_021F8370.h"

typedef struct UnkStruct_ov5_021F8480_t UnkStruct_ov5_021F8480;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
} UnkStruct_ov5_021F83D4;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    VecFx32 unk_04;
    UnkStruct_ov5_021F83D4 unk_10;
} UnkStruct_ov5_0220192C;

typedef struct {
    u32 unk_00;
} UnkStruct_ov5_022018DC;

typedef struct {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    UnkStruct_ov5_021E1890 * unk_08;
} UnkStruct_ov5_021F8480_sub1;

struct UnkStruct_ov5_021F8480_t {
    FieldSystem * fieldSystem;
    UnkStruct_02071C34 * unk_04;
    UnkStruct_ov5_021F8480_sub1 unk_08[23];
};

static void ov5_021F8480(UnkStruct_ov5_021F8480 * param0, const u32 param1);
static BOOL ov5_021F8508(FieldSystem * fieldSystem, u32 param1);
static BOOL ov5_021F851C(int param0, int param1, const UnkStruct_ov5_0220192C * param2, FieldSystem * fieldSystem);

static const UnkStruct_ov5_0220192C Unk_ov5_0220192C[23];
static const u32 Unk_ov5_0220188C[20];
static const UnkStruct_ov5_022018DC Unk_ov5_022018DC[20];

void ov5_021F8370 (FieldSystem * fieldSystem)
{
    int v0;
    UnkStruct_02027860 * v1;
    UnkStruct_02071C34 * v2;
    UnkStruct_ov5_021F8480 * v3;

    v1 = sub_02027860(FieldSystem_SaveData(fieldSystem));
    v2 = sub_02027F6C(v1, 10);
    v3 = Heap_AllocFromHeap(4, sizeof(UnkStruct_ov5_021F8480));

    memset(v3, 0, sizeof(UnkStruct_ov5_021F8480));

    v3->fieldSystem = fieldSystem;
    v3->unk_04 = v2;

    fieldSystem->unk_04->unk_24 = v3;

    {
        int v4;

        for (v4 = 0; v4 < 20; v4++) {
            if (ov5_021F8508(fieldSystem, v4) == 1) {
                ov5_021F8480(v3, v4);
            }
        }
    }
}

void ov5_021F83C0 (FieldSystem * fieldSystem)
{
    UnkStruct_ov5_021F8480 * v0 = fieldSystem->unk_04->unk_24;

    Heap_FreeToHeap(v0);
    fieldSystem->unk_04->unk_24 = NULL;
}

BOOL ov5_021F83D4 (FieldSystem * fieldSystem, const int param1, const int param2, const fx32 param3, BOOL * param4)
{
    int v0;
    const UnkStruct_ov5_021F83D4 * v1;
    const UnkStruct_ov5_0220192C * v2 = Unk_ov5_0220192C;

    for (v0 = 0; v0 < 23; v0++, v2++) {
        if (ov5_021F851C(param1, param2, v2, fieldSystem) == 1) {
            *param4 = 1;
            return 1;
        }
    }

    *param4 = 0;
    return 0;
}

BOOL ov5_021F8410 (FieldSystem * fieldSystem, const int param1, const int param2, const int param3)
{
    UnkStruct_02027860 * v0 = sub_02027860(FieldSystem_SaveData(fieldSystem));

    if (sub_02027F80(v0) == 10) {
        int v1;
        const UnkStruct_ov5_021F83D4 * v2;
        const UnkStruct_ov5_0220192C * v3 = Unk_ov5_0220192C;

        for (v1 = 0; v1 < 23; v1++, v3++) {
            if (ov5_021F851C(param1, param2, v3, fieldSystem) == 1) {
                const UnkStruct_ov5_022018DC * v4 = &Unk_ov5_022018DC[v3->unk_00];

                if (v4->unk_00 == 10100) {
                    if (param3 != 0) {
                        continue;
                    }
                }

                sub_0203E880(fieldSystem, v4->unk_00, NULL);
                return 1;
            }
        }
    }

    return 0;
}

static void ov5_021F8480 (UnkStruct_ov5_021F8480 * param0, const u32 param1)
{
    int v0, v1;
    UnkStruct_ov5_021F8480_sub1 * v2;
    VecFx32 v3 = {0, 0, 0};
    int v4 = Unk_ov5_0220188C[param1];
    const UnkStruct_ov5_0220192C * v5 = Unk_ov5_0220192C;
    FieldSystem * fieldSystem = param0->fieldSystem;

    for (v0 = 0; v0 < 23; v0++, v5++) {
        if (v5->unk_00 == param1) {
            v1 = 0;
            v2 = param0->unk_08;

            do {
                if (v2->unk_00 == 0) {
                    v2->unk_00 = 1;
                    v2->unk_04 = v4;
                    v2->unk_02 = ov5_021E19CC(fieldSystem->unk_A4, fieldSystem->unk_30, v4, &v5->unk_04, &v3, fieldSystem->unk_50);
                    v2->unk_08 = ov5_021E18CC(fieldSystem->unk_A4, v4);
                    break;
                }

                v1++;
                v2++;
            } while (v1 < 23);
        }
    }
}

static BOOL ov5_021F8508 (FieldSystem * fieldSystem, u32 param1)
{
    return sub_0206AF6C(SaveData_Events(fieldSystem->saveData), 2, param1);
}

static BOOL ov5_021F851C (int param0, int param1, const UnkStruct_ov5_0220192C * param2, FieldSystem * fieldSystem)
{
    const UnkStruct_ov5_021F83D4 * v0 = &param2->unk_10;

    if ((param2->unk_02 == 1) && (ov5_021F8508(fieldSystem, param2->unk_00) == 1)) {
        if ((param1 >= v0->unk_02) && (param1 <= v0->unk_06) && (param0 >= v0->unk_00) && (param0 <= v0->unk_04)) {
            return 1;
        }
    }

    return 0;
}

static const UnkStruct_ov5_0220192C Unk_ov5_0220192C[23] = {
    {
        0x0,
        0x1,
        {(((0xc << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x7 << 4) * FX32_ONE))},
        {0xB, 0x5, 0xD, 0x8}
    },
    {
        0x1,
        0x1,
        {(((0xf << 4) * FX32_ONE)), (((0 << 4) * FX32_ONE)), (((0x7 << 4) * FX32_ONE))},
        {0xE, 0x5, 0xf, 0x8}
    },
    {
        0x2,
        0x1,
        {(((0xd << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0xa << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0xB, 0x9, 0xD, 0xA}
    },
    {
        0x3,
        0x1,
        {(((0x12 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x8 << 4) * FX32_ONE))},
        {0x12, 0x6, 0x14, 0x9}
    },
    {
        0x4,
        0x1,
        {(((0x14 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x7 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x11, 0x6, 0x11, 0x7}
    },
    {
        0x5,
        0x1,
        {(((0xc << 4) * FX32_ONE)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0xB, 0x3, 0xC, 0x3}
    },
    {
        0x6,
        0x1,
        {(((0xe << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0xD, 0x3, 0xf, 0x3}
    },
    {
        0x7,
        0x1,
        {(((0x11 << 4) * FX32_ONE)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x10, 0x3, 0x11, 0x3}
    },
    {
        0x8,
        0x1,
        {(((0x13 << 4) * FX32_ONE)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x12, 0x3, 0x13, 0x3}
    },
    {
        0x9,
        0x1,
        {(((0x1 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x1, 0x3, 0x1, 0x3}
    },
    {
        0x9,
        0x1,
        {(((0x14 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x14, 0x3, 0x14, 0x3}
    },
    {
        0x9,
        0x1,
        {(((0x1 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0xb << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x1, 0xB, 0x1, 0xB}
    },
    {
        0x9,
        0x1,
        {(((0x14 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0xb << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x14, 0xB, 0x14, 0xB}
    },
    {
        0xA,
        0x1,
        {(((0x2 << 4) * FX32_ONE)), (((0 << 4) * FX32_ONE)), (((0x8 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x1, 0x7, 0x2, 0x9}
    },
    {
        0xB,
        0x0,
        {(((0x12 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x0, 0x0, 0x0, 0x0}
    },
    {
        0xC,
        0x1,
        {(((0x2 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x2, 0x3, 0x2, 0x3}
    },
    {
        0xD,
        0x1,
        {(((0x6 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x3 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x6, 0x3, 0x6, 0x3}
    },
    {
        0xE,
        0x1,
        {(((0x2 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x5 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x1, 0x4, 0x2, 0x6}
    },
    {
        0xF,
        0x1,
        {(((0x6 << 4) * FX32_ONE)), (((0 << 4) * FX32_ONE)), (((0xa << 4) * FX32_ONE))},
        {0x4, 0x9, 0x7, 0xA}
    },
    {
        0x10,
        0x0,
        {(((0xb << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x1 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x0, 0x0, 0x0, 0x0}
    },
    {
        0x11,
        0x0,
        {(((0x8 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x1 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x0, 0x0, 0x0, 0x0}
    },
    {
        0x12,
        0x0,
        {(((0x6 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0xa << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x0, 0x0, 0x0, 0x0}
    },
    {
        0x13,
        0x0,
        {(((0x7 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1)), (((0 << 4) * FX32_ONE)), (((0x6 << 4) * FX32_ONE) + ((16 * FX32_ONE) >> 1))},
        {0x0, 0x0, 0x0, 0x0}
    }
};

static const u32 Unk_ov5_0220188C[20] = {
    0x22F,
    0x230,
    0x231,
    0x232,
    0x233,
    0x234,
    0x235,
    0x236,
    0x237,
    0x238,
    0x239,
    0x23A,
    0x23B,
    0x23C,
    0x23D,
    0x23E,
    0x23F,
    0x240,
    0x241,
    0x242
};

static const UnkStruct_ov5_022018DC Unk_ov5_022018DC[20] = {
    {0x15},
    {0x16},
    {0x17},
    {0x18},
    {0x19},
    {0x2774},
    {0x1B},
    {0x1C},
    {0x1D},
    {0x1E},
    {0x1F},
    {0x20},
    {0x21},
    {0x22},
    {0x23},
    {0x24},
    {0x25},
    {0x26},
    {0x27},
    {0x28}
};
