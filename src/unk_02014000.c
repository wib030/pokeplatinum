#include <nitro.h>
#include <string.h>

#include "struct_decls/sys_task.h"
#include "struct_decls/struct_020203AC_decl.h"

#include "functypes/funcptr_02014014.h"
#include "struct_defs/struct_02014560.h"
#include "functypes/funcptr_020146F4.h"
#include "struct_defs/struct_020147B8.h"
#include "struct_defs/struct_02014874.h"
#include "struct_defs/struct_02014890.h"
#include "struct_defs/struct_02014910.h"
#include "struct_defs/struct_02014970.h"

#include "spl.h"

#include "narc.h"
#include "unk_0200D9E8.h"
#include "unk_02014000.h"
#include "heap.h"
#include "unk_02020020.h"

enum {
    EMIT_MAX = 20,
    PARTICLE_MAX = 200,
    FIX_POLYGON_ID = 5,
    MIN_POLYGON_ID = 6,
    MAX_POLYGON_ID = 63,
};

typedef struct UnkStruct_02014014_t {
    UnkSPLStruct1 * unk_00;
    void * unk_04;
    UnkSPLStruct6 * unk_08;
    void * unk_0C;
    void * unk_10;
    void * unk_14;
    UnkFuncPtr_02014014 unk_18;
    UnkFuncPtr_02014014 unk_1C;
    UnkStruct_020203AC * unk_20;
    VecFx32 unk_24;
    u16 unk_30;
    VecFx32 unk_34;
    VecFx32 unk_40;
    VecFx32 unk_4C;
    union {
        NNSGfdFrmTexVramState unk_58_val1;
        NNSGfdTexKey unk_58_val2[16];
    };
    union {
        NNSGfdFrmPlttVramState unk_98_val1;
        NNSGfdPlttKey unk_98_val2[16];
    };
    u8 unk_D8;
    u8 unk_D9;
    u8 unk_DA;
    u8 unk_DB;
} UnkStruct_02014014;

static void * Unk_021BF618[16];

static void * Unk_021BF614;

static UnkStruct_02014014 * Unk_021BF610;

static const VecFx32 Unk_020E5448 = {
    0, 0, 0x4000
};

static const VecFx32 Unk_020E5430 = {
    0, FX32_ONE, 0
};

static const VecFx32 Unk_020E543C = {
    0, 0, 0
};

static void * sub_02014204(u32 param0);
static void * sub_02014230(u32 param0);
static void * sub_0201425C(u32 param0);
static void * sub_02014288(u32 param0);
static void * sub_020142B4(u32 param0);
static void * sub_020142E0(u32 param0);
static void * sub_0201430C(u32 param0);
static void * sub_02014338(u32 param0);
static void * sub_02014364(u32 param0);
static void * sub_02014390(u32 param0);
static void * sub_020143BC(u32 param0);
static void * sub_020143E8(u32 param0);
static void * sub_02014414(u32 param0);
static void * sub_02014440(u32 param0);
static void * sub_0201446C(u32 param0);
static void * sub_02014498(u32 param0);
static inline void * Particle_LocalAlloc(UnkStruct_02014014 * param0, u32 param1);
static void sub_02014560(UnkStruct_02014014 * param0);
static void sub_020145A0(SysTask * param0, void * param1);

void sub_0201411C(UnkStruct_02014014 * param0);
void sub_02014718(UnkStruct_02014014 * param0);
void sub_020144CC(UnkStruct_02014014 * param0, void * param1, int param2, int param3);

static const UnkFuncPtr_0209CD00 Unk_020E5454[] = {
    sub_02014204,
    sub_02014230,
    sub_0201425C,
    sub_02014288,
    sub_020142B4,
    sub_020142E0,
    sub_0201430C,
    sub_02014338,
    sub_02014364,
    sub_02014390,
    sub_020143BC,
    sub_020143E8,
    sub_02014414,
    sub_02014440,
    sub_0201446C,
    sub_02014498,
};

void sub_02014000 (void)
{
    int i;

    for (i = 0; i < 16; i++) {
        Unk_021BF618[i] = NULL;
    }
}

UnkStruct_02014014 * sub_02014014 (UnkFuncPtr_02014014 param0, UnkFuncPtr_02014014 param1, void * param2, int param3, int param4, int param5)
{
    UnkStruct_02014014 * v0;
    int v1;

    for (v1 = 0; v1 < 16; v1++) {
        if (Unk_021BF618[v1] == NULL) {
            break;
        }
    }
    if (v1 >= 16) {
        return NULL;
    }

    v0 = Heap_AllocFromHeap(param5, sizeof(UnkStruct_02014014));
    if (v0 == NULL) {
        GF_ASSERT(FALSE);
    }
    memset(v0, 0, sizeof(UnkStruct_02014014));

    v0->unk_18 = param0;
    v0->unk_1C = param1;

    v0->unk_34 = Unk_020E5448;
    v0->unk_40 = Unk_020E5430;
    v0->unk_4C = Unk_020E543C;

    memset(param2, 0, param3);
    v0->unk_0C = param2;
    v0->unk_10 = param2;
    v0->unk_14 = (void *)((u32)param2 + param3);
    v0->unk_DA = v1;
    Unk_021BF618[v1] = v0;

    if (param4 == 1) {
        v0->unk_20 = sub_020203AC(param5);
        {
            VEC_Set(&v0->unk_24, 0, 0, 0);
            v0->unk_30 = 8192;

            sub_02020784(&Unk_020E543C, &Unk_020E5448,
                         v0->unk_30,
                         0,
                         0,
                         v0->unk_20
                         );
            v0->unk_DB = 0;
            sub_020203D4(v0->unk_20);
        }
    }

    v0->unk_00 = SPL_0209CD00(Unk_020E5454[v1], EMIT_MAX, PARTICLE_MAX,
                          FIX_POLYGON_ID, MIN_POLYGON_ID, MAX_POLYGON_ID);

    sub_02014744(v0, &Unk_020E5430);

    return v0;
}

void sub_0201411C (UnkStruct_02014014 * param0)
{
    int v0;

    sub_02014718(param0);

    if (param0->unk_D8 & (1 << 0)) {
        NNS_GfdSetFrmTexVramState(&param0->unk_58_val1);
    } else if (param0->unk_D8 & (1 << 1)) {
        int v1;
        for (v1 = 0; v1 < 16; v1++) {
            if (param0->unk_58_val2[v1] != NNS_GFD_ALLOC_ERROR_TEXKEY) {
                NNS_GfdFreeLnkTexVram(param0->unk_58_val2[v1]);
                param0->unk_58_val2[v1] = NNS_GFD_ALLOC_ERROR_TEXKEY;
            }
        }
    }

    if (param0->unk_D8 & (1 << 2)) {
        NNS_GfdSetFrmPlttVramState(&param0->unk_98_val1);
    } else if (param0->unk_D8 & (1 << 3)) {
        int v2;
        for (v2 = 0; v2 < 16; v2++) {
            if (param0->unk_98_val2[v2] != NNS_GFD_ALLOC_ERROR_PLTTKEY) {
                NNS_GfdFreeLnkPlttVram(param0->unk_98_val2[v2]);
                param0->unk_98_val2[v2] = NNS_GFD_ALLOC_ERROR_PLTTKEY;
            }
        }
    }
    param0->unk_D8 = 0 | 0;
    param0->unk_08 = NULL;

    if (param0->unk_04 != NULL) {
        Heap_FreeToHeap(param0->unk_04);
        param0->unk_04 = NULL;
    }

    for (v0 = 0; v0 < 16; v0++) {
        if (Unk_021BF618[v0] == param0) {
            Unk_021BF618[v0] = NULL;
            break;
        }
    }

    if (param0->unk_20 != NULL) {
        sub_020203B8(param0->unk_20);
    }

    Heap_FreeToHeap(param0);
}

void sub_020141E4 (void)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (Unk_021BF618[i] != NULL) {
            sub_0201411C(Unk_021BF618[i]);
        }
    }
}

static inline void * Particle_LocalAlloc (UnkStruct_02014014 * param0, u32 param1)
{
    void * v0;
    u32 v1, v2;

    v0 = param0->unk_10;

    v1 = (u32)param0->unk_10 + param1;
    v2 = v1 % 4;
    if (v2 > 0) {
        v1 += 4 - v2;
    }
    param0->unk_10 = (void *)v1;

    if (param0->unk_10 >= param0->unk_14) {
        GF_ASSERT(FALSE);
    }
    return v0;
}

static void * sub_02014204 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[0];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_02014230 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[1];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_0201425C (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[2];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_02014288 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[3];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_020142B4 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[4];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_020142E0 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[5];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_0201430C (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[6];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_02014338 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[7];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_02014364 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[8];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_02014390 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[9];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_020143BC (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[10];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_020143E8 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[11];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_02014414 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[12];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_02014440 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[13];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_0201446C (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[14];
    return Particle_LocalAlloc(v0, param0);
}

static void * sub_02014498 (u32 param0)
{
    UnkStruct_02014014 * v0;

    v0 = Unk_021BF618[15];
    return Particle_LocalAlloc(v0, param0);
}

void * sub_020144C4 (int param0, int param1, int param2)
{
    return NARC_AllocAndReadWholeMemberByIndexPair(param0, param1, param2);
}

void sub_020144CC (UnkStruct_02014014 * param0, void * param1, int param2, int param3)
{
    GF_ASSERT(param0->unk_00 != NULL);
    GF_ASSERT(param0->unk_04 == NULL);

    param0->unk_D8 = param2;

    if (param2 & (1 << 0)) {
        NNS_GfdGetFrmTexVramState(&param0->unk_58_val1);
    } else if (param2 & (1 << 1)) {
        int v0;
        for (v0 = 0; v0 < 16; v0++) {
            param0->unk_58_val2[v0] = NNS_GFD_ALLOC_ERROR_TEXKEY;
        }
    }

    if (param2 & (1 << 2)) {
        NNS_GfdGetFrmPlttVramState(&param0->unk_98_val1);
    } else if (param2 & (1 << 3)) {
        int v1;
        for (v1 = 0; v1 < 16; v1++) {
            param0->unk_98_val2[v1] = NNS_GFD_ALLOC_ERROR_PLTTKEY;
        }
    }

    param0->unk_04 = param1;

    if (param3 == 1) {
        sub_02014560(param0);
    } else {
        sub_0200DA3C(sub_020145A0, param0, 5);
    }
}

static void sub_02014560 (UnkStruct_02014014 * param0)
{
    SPL_0209C988(param0->unk_00, param0->unk_04);

    Unk_021BF610 = param0;
    if (param0->unk_18 == NULL) {
        (void)SPL_0209C7F4(param0->unk_00);
    } else {
        SPL_0209C8BC(param0->unk_00, param0->unk_18);
    }
    if (param0->unk_1C == NULL) {
        (void)SPL_0209C7E0(param0->unk_00);
    } else {
        SPL_0209C808(param0->unk_00, param0->unk_1C);
    }
    Unk_021BF610 = NULL;

    {
        u32 v0;
        v0 = ((const UnkStruct_02014560 *)param0->unk_04)->unk_14;
    }
}

static void sub_020145A0 (SysTask * param0, void * param1)
{
    UnkStruct_02014014 * v0 = param1;

    sub_02014560(v0);
    SysTask_Done(param0);
}

void sub_020145B4 (NNSGfdTexKey param0)
{
    int v0;
    UnkStruct_02014014 * v1;

    GF_ASSERT(param0 != NNS_GFD_ALLOC_ERROR_TEXKEY);
    GF_ASSERT(Unk_021BF610 != NULL);

    v1 = Unk_021BF610;

    for (v0 = 0; v0 < 16; v0++) {
        if (v1->unk_58_val2[v0] == NNS_GFD_ALLOC_ERROR_TEXKEY) {
            v1->unk_58_val2[v0] = param0;
            return;
        }
    }
    GF_ASSERT(FALSE);
}

void sub_020145F4 (NNSGfdPlttKey param0)
{
    int v0;
    UnkStruct_02014014 * v1;

    GF_ASSERT(param0 != NNS_GFD_ALLOC_ERROR_PLTTKEY);
    GF_ASSERT(Unk_021BF610 != NULL);

    v1 = Unk_021BF610;

    for (v0 = 0; v0 < 16; v0++) {
        if (v1->unk_98_val2[v0] == NNS_GFD_ALLOC_ERROR_PLTTKEY) {
            v1->unk_98_val2[v0] = param0;
            return;
        }
    }
    GF_ASSERT(FALSE);
}

void sub_02014638 (UnkStruct_02014014 * param0)
{
    const MtxFx43 * v0;

    if (param0->unk_20 != NULL) {
        sub_02020854(param0->unk_DB, param0->unk_20);
        sub_020203D4(param0->unk_20);
        sub_020203EC();
    }

    NNS_G3dGlbFlush();

    v0 = NNS_G3dGlbGetCameraMtx();
    SPL_0209C5E0(param0->unk_00, v0);

    if (param0->unk_20 != NULL) {
        sub_020203E0();
    }

    NNS_G3dGlbFlush();
}

void sub_02014674 (UnkStruct_02014014 * param0)
{
    SPL_0209C6A8(param0->unk_00);
}

int sub_02014680 (void)
{
    int i, count;

    count = 0;
    for (i = 0; i < 16; i++) {
        if (Unk_021BF618[i] != NULL) {
            count++;
        }
    }
    return count;
}

int sub_0201469C (void)
{
    int i, count;

    count = 0;
    for (i = 0; i < 16; i++) {
        if (Unk_021BF618[i] != NULL) {
            sub_02014638(Unk_021BF618[i]);
            count++;
        }
    }
    return count;
}

int sub_020146C0 (void)
{
    int i, count;

    count = 0;
    for (i = 0; i < 16; i++) {
        if (Unk_021BF618[i] != NULL) {
            sub_02014674(Unk_021BF618[i]);
            count++;
        }
    }
    return count;
}

UnkSPLStruct6 * sub_020146E4 (UnkStruct_02014014 * param0, int param1, const VecFx32 * param2)
{
    UnkSPLStruct6 * v0;

    v0 = SPL_0209C56C(param0->unk_00, param1, param2);
    param0->unk_08 = v0;

    return v0;
}

UnkSPLStruct6 * sub_020146F4 (UnkStruct_02014014 * param0, int param1, UnkFuncPtr_020146F4 param2, void * param3)
{
    UnkSPLStruct6 * v0;

    Unk_021BF614 = param3;
    v0 = SPL_0209C4D8(param0->unk_00, param1, param2);
    Unk_021BF614 = NULL;
    param0->unk_08 = v0;

    return v0;
}

s32 sub_02014710 (UnkStruct_02014014 * param0)
{
    return param0->unk_00->unk_04.unk_04;
}

void sub_02014718 (UnkStruct_02014014 * param0)
{
    SPL_0209C400(param0->unk_00);
}

void sub_02014724 (UnkStruct_02014014 * param0, UnkSPLStruct6 * param1)
{
    SPL_0209C444(param0->unk_00, param1);
}

void * sub_02014730 (UnkStruct_02014014 * param0)
{
    return param0->unk_0C;
}

void sub_02014734 (UnkStruct_02014014 * param0, VecFx32 * param1)
{
    *param1 = param0->unk_40;
}

void sub_02014744 (UnkStruct_02014014 * param0, const VecFx32 * param1)
{
    param0->unk_40 = *param1;
    sub_02020680(param1, param0->unk_20);
}

void * sub_02014764 (void)
{
    return Unk_021BF614;
}

void sub_02014770 (VecFx32 * param0)
{
    *param0 = Unk_020E5430;
}

UnkStruct_020203AC * sub_02014784 (UnkStruct_02014014 * param0)
{
    return param0->unk_20;
}

void sub_02014788 (UnkStruct_02014014 * param0, int param1)
{
    param0->unk_DB = param1;
}

u8 sub_02014790 (UnkStruct_02014014 * param0)
{
    return param0->unk_DB;
}

void sub_02014798 (UnkSPLStruct6 * param0, VecFx16 * param1)
{
    *param1 = param0->unk_C0;
}

void sub_020147B0 (UnkSPLStruct6 * param0, fx32 param1)
{
    param0->unk_90->unk_00->unk_10 = param1;
}

enum {
    SPL_FLD_TYPE_GRAVITY = 0,
    SPL_FLD_TYPE_RANDOM,
    SPL_FLD_TYPE_MAGNET,
    SPL_FLD_TYPE_SPIN,
    SPL_FLD_TYPE_SIMPLE_COLL,
    SPL_FLD_TYPE_CONVERGENCE,
};

static const void * sub_020147B8 (UnkSPLStruct6 * param0, int param1)
{
    int v0;
    int v1;
    UnkStruct_020147B8 * v2;

    v1 = param0->unk_90->unk_1C;

    if (v1 == 0) {
        return NULL;
    }

    for (v0 = 0; v0 < v1; v0++) {
        v2 = &param0->unk_90->unk_18[v0];

        if (v2 == NULL) {
            continue;
        }

        switch (param1) {
        case SPL_FLD_TYPE_GRAVITY:
            if (v2->unk_00 == SPL_020A2204) {
                return v2->unk_04;
            }
            continue;
        case SPL_FLD_TYPE_RANDOM:
            if (v2->unk_00 == SPL_020A213C) {
                return v2->unk_04;
            }
            continue;
        case SPL_FLD_TYPE_MAGNET:
            if (v2->unk_00 == SPL_020A20B8) {
                return v2->unk_04;
            }
            continue;
        case SPL_FLD_TYPE_SPIN:
            if (v2->unk_00 == SPL_020A1FE0) {
                return v2->unk_04;
            }
            continue;
        case SPL_FLD_TYPE_SIMPLE_COLL:
            if (v2->unk_00 == SPL_020A1EC4) {
                return v2->unk_04;
            }
            break;
        case SPL_FLD_TYPE_CONVERGENCE:
            if (v2->unk_00 == SPL_020A1E30) {
                return v2->unk_04;
            }
            continue;
        default:
            return NULL;
        }
    }

    return NULL;
}

void sub_02014874 (UnkSPLStruct6 * param0, VecFx16 * param1)
{
    UnkStruct_02014874 * v0;

    v0 = (UnkStruct_02014874 *)sub_020147B8(param0, SPL_FLD_TYPE_GRAVITY);

    if (v0 == NULL) {
        return;
    }

    v0->unk_00 = *param1;
}

void sub_02014890 (UnkSPLStruct6 * param0, VecFx32 * param1)
{
    UnkStruct_02014890 * v0;

    v0 = (UnkStruct_02014890 *)sub_020147B8(param0, SPL_FLD_TYPE_MAGNET);

    if (v0 == NULL) {
        return;
    }

    v0->unk_00 = *param1;
}

void sub_020148A8 (UnkSPLStruct6 * param0, VecFx32 * param1)
{
    UnkStruct_02014890 * v0;

    v0 = (UnkStruct_02014890 *)sub_020147B8(param0, SPL_FLD_TYPE_MAGNET);

    if (v0 == NULL) {
        VecFx32 v1 = {0, 0, 0};
        *param1 = v1;
        return;
    }

    *param1 = v0->unk_00;
}

void sub_020148DC (UnkSPLStruct6 * param0, fx16 * param1)
{
    UnkStruct_02014890 * v0;

    v0 = (UnkStruct_02014890 *)sub_020147B8(param0, SPL_FLD_TYPE_MAGNET);

    if (v0 == NULL) {
        return;
    }

    v0->unk_0C = *param1;
}

void sub_020148F4 (UnkSPLStruct6 * param0, fx16 * param1)
{
    UnkStruct_02014890 * v0;

    v0 = (UnkStruct_02014890 *)sub_020147B8(param0, SPL_FLD_TYPE_MAGNET);

    if (v0 == NULL) {
        *param1 = 0;
        return;
    }

    *param1 = v0->unk_0C;
}

void sub_02014910 (UnkSPLStruct6 * param0, u16 * param1)
{
    UnkStruct_02014910 * v0;

    v0 = (UnkStruct_02014910 *)sub_020147B8(param0, SPL_FLD_TYPE_SPIN);

    if (v0 == NULL) {
        return;
    }

    v0->unk_00 = *param1;
}

void sub_02014924 (UnkSPLStruct6 * param0, u16 * param1)
{
    UnkStruct_02014910 * v0;

    v0 = (UnkStruct_02014910 *)sub_020147B8(param0, SPL_FLD_TYPE_SPIN);

    if (v0 == NULL) {
        *param1 = 0;
        return;
    }

    *param1 = v0->unk_00;
}

void sub_02014940 (UnkSPLStruct6 * param0, u16 * param1)
{
    UnkStruct_02014910 * v0;

    v0 = (UnkStruct_02014910 *)sub_020147B8(param0, SPL_FLD_TYPE_SPIN);

    if (v0 == NULL) {
        return;
    }

    v0->unk_02 = *param1;
}

void sub_02014954 (UnkSPLStruct6 * param0, u16 * param1)
{
    UnkStruct_02014910 * v0;

    v0 = (UnkStruct_02014910 *)sub_020147B8(param0, SPL_FLD_TYPE_SPIN);

    if (v0 == NULL) {
        *param1 = 0;
        return;
    }

    *param1 = v0->unk_02;
}

void sub_02014970 (UnkSPLStruct6 * param0, VecFx32 * param1)
{
    UnkStruct_02014970 * v0;

    v0 = (UnkStruct_02014970 *)sub_020147B8(param0, SPL_FLD_TYPE_CONVERGENCE);

    if (v0 == NULL) {
        return;
    }

    v0->unk_00 = *param1;
}

void sub_02014988 (UnkSPLStruct6 * param0, VecFx32 * param1)
{
    UnkStruct_02014970 * v0;

    v0 = (UnkStruct_02014970 *)sub_020147B8(param0, SPL_FLD_TYPE_CONVERGENCE);

    if (v0 == NULL) {
        VecFx32 v1 = {0, 0, 0};
        *param1 = v1;
        return;
    }

    *param1 = v0->unk_00;
}

void sub_020149BC (UnkSPLStruct6 * param0, fx16 * param1)
{
    UnkStruct_02014970 * v0;

    v0 = (UnkStruct_02014970 *)sub_020147B8(param0, SPL_FLD_TYPE_CONVERGENCE);

    if (v0 == NULL) {
        return;
    }

    v0->unk_0C = *param1;
}

void sub_020149D4 (UnkSPLStruct6 * param0, fx16 * param1)
{
    UnkStruct_02014970 * v0;

    v0 = (UnkStruct_02014970 *)sub_020147B8(param0, SPL_FLD_TYPE_CONVERGENCE);

    if (v0 == NULL) {
        *param1 = 0;
        return;
    }

    *param1 = v0->unk_0C;
}
