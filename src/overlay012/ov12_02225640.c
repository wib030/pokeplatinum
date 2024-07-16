#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_020203AC_decl.h"

#include "overlay012/struct_ov12_02225640.h"
#include "overlay115/struct_ov115_0226527C.h"

#include "spl.h"

#include "unk_02014000.h"
#include "unk_02020020.h"
#include "overlay012/ov12_0221FC20.h"
#include "overlay012/ov12_02225640.h"
#include "overlay012/ov12_02235254.h"

static void ov12_02225640(UnkStruct_ov12_02225640 * param0, int param1[]);
static BOOL ov12_0222564C(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);
static BOOL ov12_02225670(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);
static BOOL ov12_022256AC(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);
static BOOL ov12_022256E8(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);
static BOOL ov12_02225724(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);
static BOOL ov12_02225784(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);
static BOOL ov12_022257C0(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);
static BOOL ov12_022257FC(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);
static BOOL ov12_02225824(UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1);

static BOOL(*const Unk_ov12_02239E10[])(UnkSPLStruct6 *, UnkStruct_ov12_02225640 *) = {
    ov12_0222564C,
    ov12_02225670,
    ov12_022256AC,
    ov12_022256E8,
    ov12_02225724,
    ov12_02225784,
    ov12_022257C0,
    ov12_022257FC,
    ov12_02225824
};

static void ov12_02225640 (UnkStruct_ov12_02225640 * param0, int param1[])
{
    ov12_0222325C(param0->unk_00, param1, 4);
}

static BOOL ov12_0222564C (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    UnkStruct_020203AC * v0;
    UnkStruct_ov115_0226527C v1 = {0, 0, 0};

    v0 = sub_02014784(param1->unk_04);
    sub_020209D4(&v1, v0);

    return 1;
}

static BOOL ov12_02225670 (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    UnkStruct_020203AC * v0;
    UnkStruct_ov115_0226527C v1 = {(0x1000 * 2), (0x1000 * 2), 0};

    v0 = sub_02014784(param1->unk_04);

    sub_02014788(param1->unk_04, 1);
    sub_020209D4(&v1, v0);

    return 1;
}

static BOOL ov12_022256AC (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    int v0[4];
    UnkStruct_ov115_0226527C v1 = {0, 0, 0, 0};
    UnkStruct_020203AC * v2;

    ov12_02225640(param1, v0);

    v1.unk_00 = v0[1];
    v1.unk_02 = v0[2];
    v1.unk_04 = v0[3];

    v2 = sub_02014784(param1->unk_04);

    sub_020209D4(&v1, v2);
    return 1;
}

static BOOL ov12_022256E8 (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    UnkStruct_020203AC * v0;
    UnkStruct_ov115_0226527C v1 = {-(0x1000 * 3), (0x1000 * 2), 0x1000};

    v0 = sub_02014784(param1->unk_04);

    sub_02014788(param1->unk_04, 1);
    sub_020209D4(&v1, v0);

    return 1;
}

static BOOL ov12_02225724 (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    UnkStruct_020203AC * v0;
    UnkStruct_ov115_0226527C v1 = {49664, 5952, 4096};
    UnkStruct_ov115_0226527C v2 = {9248, 3744, 0};

    v0 = sub_02014784(param1->unk_04);
    sub_02014788(param1->unk_04, 1);

    if (ov12_0221FDD4(param1->unk_00) == 1) {
        sub_020209D4(&v1, v0);
    } else {
        sub_020209D4(&v2, v0);
    }

    return 1;
}

static BOOL ov12_02225784 (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    UnkStruct_020203AC * v0;
    UnkStruct_ov115_0226527C v1 = {49664, 5952, 4096};

    v0 = sub_02014784(param1->unk_04);

    sub_02014788(param1->unk_04, 1);
    sub_020209D4(&v1, v0);

    return 1;
}

static BOOL ov12_022257C0 (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    UnkStruct_020203AC * v0;
    UnkStruct_ov115_0226527C v1 = {0, 0, -6000};

    v0 = sub_02014784(param1->unk_04);

    sub_02014788(param1->unk_04, 1);
    sub_020209D4(&v1, v0);

    return 1;
}

static BOOL ov12_022257FC (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    VecFx32 v0;
    UnkStruct_020203AC * v1;

    v1 = sub_02014784(param1->unk_04);

    ov12_022356E8(param1->unk_00, param1->unk_24, &v0);
    sub_02020ACC(&v0, v1);

    return 1;
}

static BOOL ov12_02225824 (UnkSPLStruct6 * param0, UnkStruct_ov12_02225640 * param1)
{
    VecFx32 v0;
    UnkStruct_020203AC * v1;

    v1 = sub_02014784(param1->unk_04);

    ov12_022356E8(param1->unk_00, param1->unk_28, &v0);
    sub_02020ACC(&v0, v1);

    return 1;
}

void ov12_0222584C (int param0, UnkSPLStruct6 * param1, UnkStruct_ov12_02225640 * param2)
{
    BOOL v0;

    v0 = Unk_ov12_02239E10[param0](param1, param2);

    if (v0 == 0) {
        (void)0;
    }
}
