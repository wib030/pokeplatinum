#include <nitro.h>
#include <string.h>

#include "functypes/sys_task_func.h"

#include "unk_0201CCF0.h"

typedef struct SysTask {
    UnkStruct_0201CD88 * unk_00;
    SysTask * unk_04;
    SysTask * unk_08;
    u32 unk_0C;
    void * unk_10;
    SysTaskFunc unk_14;
    u32 unk_18;
} SysTask;

typedef struct UnkStruct_0201CD88_t {
    u16 unk_00;
    u16 unk_02;
    SysTask unk_04;
    SysTask ** unk_20;
    SysTask * unk_24;
    BOOL unk_28;
    SysTask * unk_2C;
    SysTask * unk_30;
} UnkStruct_0201CD88;

static void sub_0201CCF0(UnkStruct_0201CD88 * param0, SysTask * param1);
static void sub_0201CD04(UnkStruct_0201CD88 * param0);
static SysTask * sub_0201CD38(UnkStruct_0201CD88 * param0);
static int sub_0201CD54(UnkStruct_0201CD88 * param0, SysTask * param1);
static SysTask * sub_0201CE28(UnkStruct_0201CD88 * param0, SysTaskFunc param1, void * param2, u32 param3);

static void sub_0201CCF0 (UnkStruct_0201CD88 * param0, SysTask * param1)
{
    param1->unk_00 = param0;
    param1->unk_04 = param1->unk_08 = &(param0->unk_04);
    param1->unk_0C = 0;
    param1->unk_10 = NULL;
    param1->unk_14 = NULL;
}

static void sub_0201CD04 (UnkStruct_0201CD88 * param0)
{
    int v0;

    for (v0 = 0; v0 < param0->unk_00; v0++) {
        sub_0201CCF0(param0, &(param0->unk_24[v0]));
        param0->unk_20[v0] = param0->unk_24 + v0;
    }

    param0->unk_02 = 0;
}

static SysTask * sub_0201CD38 (UnkStruct_0201CD88 * param0)
{
    SysTask * v0;

    if (param0->unk_02 == param0->unk_00) {
        return NULL;
    }

    v0 = param0->unk_20[param0->unk_02];
    param0->unk_02++;

    return v0;
}

static int sub_0201CD54 (UnkStruct_0201CD88 * param0, SysTask * param1)
{
    if (param0->unk_02 == 0) {
        return 0;
    }

    (param1)->unk_00 = (param0);
    (param1)->unk_04 = (param1)->unk_08 = &(param0->unk_04);
    (param1)->unk_0C = 0;
    (param1)->unk_10 = NULL;
    (param1)->unk_14 = NULL;

    param0->unk_02--;
    param0->unk_20[param0->unk_02] = param1;

    return 1;
}

u32 sub_0201CD80 (u32 param0)
{
    return sizeof(UnkStruct_0201CD88) + (sizeof(SysTask *) + sizeof(SysTask)) * param0;
}

UnkStruct_0201CD88 * sub_0201CD88 (u32 param0, void * param1)
{
    UnkStruct_0201CD88 * v0;

    GF_ASSERT(param1);

    v0 = param1;

    v0->unk_20 = (SysTask **)((u8 *)(v0) + sizeof(UnkStruct_0201CD88));
    v0->unk_24 = (SysTask *)((u8 *)(v0->unk_20) + sizeof(SysTask *) * param0);
    v0->unk_00 = param0;
    v0->unk_02 = 0;
    v0->unk_28 = 0;

    sub_0201CDB4(v0);
    return v0;
}

void sub_0201CDB4 (UnkStruct_0201CD88 * param0)
{
    sub_0201CD04(param0);

    (&param0->unk_04)->unk_00 = param0;
    (&param0->unk_04)->unk_04 = (&param0->unk_04)->unk_08 = &(param0->unk_04);
    (&param0->unk_04)->unk_0C = 0;
    (&param0->unk_04)->unk_10 = NULL;
    (&param0->unk_04)->unk_14 = NULL;

    param0->unk_2C = param0->unk_04.unk_08;
}

void sub_0201CDD4 (UnkStruct_0201CD88 * param0)
{
    if (param0->unk_28) {
        return;
    }

    param0->unk_2C = param0->unk_04.unk_08;

    while (param0->unk_2C != &(param0->unk_04)) {
        param0->unk_30 = param0->unk_2C->unk_08;

        if (param0->unk_2C->unk_18 == 0) {
            if (param0->unk_2C->unk_14 != NULL) {
                param0->unk_2C->unk_14(param0->unk_2C, param0->unk_2C->unk_10);
            }
        } else {
            param0->unk_2C->unk_18 = 0;
        }

        param0->unk_2C = param0->unk_30;
    }

    param0->unk_2C->unk_14 = NULL;
}

SysTask * sub_0201CE14 (UnkStruct_0201CD88 * param0, SysTaskFunc param1, void * param2, u32 param3)
{
    SysTask * v0;

    param0->unk_28 = 1;
    v0 = sub_0201CE28(param0, param1, param2, param3);
    param0->unk_28 = 0;

    return v0;
}

static SysTask * sub_0201CE28 (UnkStruct_0201CD88 * param0, SysTaskFunc param1, void * param2, u32 param3)
{
    SysTask * v0;
    SysTask * v1;

    v0 = sub_0201CD38(param0);

    if (v0 == NULL) {
        return NULL;
    }

    v0->unk_0C = param3;
    v0->unk_10 = param2;
    v0->unk_14 = param1;

    if (param0->unk_2C->unk_14 != NULL) {
        if (param0->unk_2C->unk_0C <= param3) {
            v0->unk_18 = 1;
        } else {
            v0->unk_18 = 0;
        }
    } else {
        v0->unk_18 = 0;
    }

    for (v1 = param0->unk_04.unk_08; v1 != &(param0->unk_04); v1 = v1->unk_08) {
        if (v1->unk_0C > v0->unk_0C) {
            v0->unk_04 = v1->unk_04;
            v0->unk_08 = v1;
            v1->unk_04->unk_08 = v0;
            v1->unk_04 = v0;

            if (v1 == param0->unk_30) {
                param0->unk_30 = v0;
            }

            return v0;
        }
    }

    if (param0->unk_30 == &(param0->unk_04)) {
        param0->unk_30 = v0;
    }

    v0->unk_04 = param0->unk_04.unk_04;
    v0->unk_08 = &(param0->unk_04);

    param0->unk_04.unk_04->unk_08 = v0;
    param0->unk_04.unk_04 = v0;

    return v0;
}

void sub_0201CEA8 (SysTask * param0)
{
    if (param0->unk_00->unk_30 == param0) {
        param0->unk_00->unk_30 = param0->unk_08;
    }

    param0->unk_04->unk_08 = param0->unk_08;
    param0->unk_08->unk_04 = param0->unk_04;

    sub_0201CD54(param0->unk_00, param0);
}

void sub_0201CECC (SysTask * param0, SysTaskFunc param1)
{
    param0->unk_14 = param1;
}

void * sub_0201CED0 (SysTask * param0)
{
    return param0->unk_10;
}

u32 sub_0201CED4 (SysTask * param0)
{
    return param0->unk_0C;
}
