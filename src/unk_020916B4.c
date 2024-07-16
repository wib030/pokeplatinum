#include <nitro.h>
#include <string.h>

#include "struct_defs/archived_sprite.h"
#include "struct_defs/pokemon_summary_app.h"
#include "struct_defs/struct_02091850.h"
#include "overlay115/struct_ov115_0226527C.h"

#include "unk_0200762C.h"
#include "unk_02015F84.h"
#include "gx_layers.h"
#include "unk_02020020.h"
#include "unk_0202419C.h"
#include "pokemon.h"
#include "pokemon_summary_app.h"
#include "unk_0208EA44.h"
#include "unk_020916B4.h"

typedef struct {
    VecFx16 unk_00;
    VecFx16 unk_06;
    VecFx16 unk_0C;
} UnkStruct_02091B78;

static void sub_02091850(UnkStruct_02091850 * param0);
static void sub_020918CC(VecFx16 * param0, VecFx16 * param1);
static void sub_020918EC(PokemonSummaryApp * param0);
static void sub_02091B78(const UnkStruct_02091B78 * param0, VecFx16 * param1, u8 param2);
static void sub_02091BD4(VecFx16 * param0, VecFx16 * param1, VecFx16 * param2);

static const UnkStruct_02091B78 Unk_020F4FFC[][4] = {
    {
        {
            {0x1412, 0xEC8, 0x0},
            {0x1412, 0x2DF, 0x0},
            {0x0, 0xC, 0x0}
        },
        {
            {0x2098, 0x3C5, 0x0},
            {0x15A2, 0x17F, 0x0},
            {0xB, 0x2, 0x0}
        },
        {
            {0x1BA7, 0xFFFFFFFFFFFFF475, 0x0},
            {0x1504, 0xFFFFFFFFFFFFFF96, 0x0},
            {0x7, 0xFFFFFFFFFFFFFFF5, 0x0}
        },
        {
            {0x1412, 0x12C, 0x0},
            {0x1412, 0x12C, 0x0},
            {0x0, 0x0, 0x0}
        }
    },
    {
        {
            {0x733, 0x3C5, 0x0},
            {0x1225, 0x17F, 0x0},
            {0xFFFFFFFFFFFFFFF5, 0x2, 0x0}
        },
        {
            {0x13B5, 0xEC8, 0x0},
            {0x13B5, 0x2DF, 0x0},
            {0x0, 0xC, 0x0}
        },
        {
            {0x13B5, 0x12C, 0x0},
            {0x13B5, 0x12C, 0x0},
            {0x0, 0x0, 0x0}
        },
        {
            {0xC22, 0xFFFFFFFFFFFFF475, 0x0},
            {0x12C3, 0xFFFFFFFFFFFFFF96, 0x0},
            {0xFFFFFFFFFFFFFFF9, 0xFFFFFFFFFFFFFFF5, 0x0}
        }
    },
    {
        {
            {0x733, 0x3C5, 0x0},
            {0x1225, 0x17F, 0x0},
            {0xFFFFFFFFFFFFFFF5, 0x2, 0x0}
        },
        {
            {0x13B5, 0x12C, 0x0},
            {0x13B5, 0x12C, 0x0},
            {0x0, 0x0, 0x0}
        },
        {
            {0x1BA7, 0xFFFFFFFFFFFFF475, 0x0},
            {0x14B3, 0xFFFFFFFFFFFFFF96, 0x0},
            {0x7, 0xFFFFFFFFFFFFFFF5, 0x0}
        },
        {
            {0xC22, 0xFFFFFFFFFFFFF475, 0x0},
            {0x12C3, 0xFFFFFFFFFFFFFF96, 0x0},
            {0xFFFFFFFFFFFFFFF9, 0xFFFFFFFFFFFFFFF5, 0x0}
        }
    },
    {
        {
            {0x1412, 0x12C, 0x0},
            {0x1412, 0x12C, 0x0},
            {0x0, 0x0, 0x0}
        },
        {
            {0x2098, 0x3C5, 0x0},
            {0x15A2, 0x17F, 0x0},
            {0xB, 0x2, 0x0}
        },
        {
            {0x1BA7, 0xFFFFFFFFFFFFF475, 0x0},
            {0x1504, 0xFFFFFFFFFFFFFF96, 0x0},
            {0x7, 0xFFFFFFFFFFFFFFF5, 0x0}
        },
        {
            {0xC22, 0xFFFFFFFFFFFFF475, 0x0},
            {0x1314, 0xFFFFFFFFFFFFFF96, 0x0},
            {0xFFFFFFFFFFFFFFF9, 0xFFFFFFFFFFFFFFF5, 0x0}
        }
    }
};

void sub_020916B4 (PokemonSummaryApp * param0)
{
    NNS_G3dInit();

    G3X_Init();
    G3X_InitMtxStack();
    G3X_SetShading(GX_SHADING_TOON);
    G3X_AlphaTest(0, 0);
    G3X_AlphaBlend(1);
    G3X_AntiAlias(1);
    G3X_EdgeMarking(0);
    G3X_SetFog(0, 0, 0, 0);
    G3X_SetClearColor(0, 0, 0x7fff, 63, 0);
    G3_SwapBuffers(GX_SORTMODE_AUTO, GX_BUFFERMODE_Z);
    G3_ViewPort(0, 0, 255, 191);

    GXLayers_EngineAToggleLayers(GX_PLANEMASK_BG0, 1);

    G2_SetBG0Priority(2);
}

void sub_02091750 (PokemonSummaryApp * param0)
{
    if (param0->page == 4) {
        sub_020241B4();
        sub_020203EC();

        G3_MtxMode(GX_MTXMODE_PROJECTION);
        G3_Identity();
        G3_MtxMode(GX_MTXMODE_POSITION_VECTOR);
        G3_Identity();

        NNS_G3dGlbFlush();
        sub_02091850(param0->unk_2F0);
        NNS_G3dGlbFlush();
        sub_020918EC(param0);
    }

    NNS_G2dSetupSoftwareSpriteCamera();
    sub_02007768(param0->monSpriteData.spriteManager);

    G3_SwapBuffers(GX_SORTMODE_MANUAL, GX_BUFFERMODE_Z);
}

void sub_020917B0 (PokemonSummaryApp * param0)
{
    sub_020203B8(param0->monSpriteData.unk_00);
    sub_02016114(param0->monSpriteData.animationSys, 0);
    sub_02015FB8(param0->monSpriteData.animationSys);
    sub_02007B6C(param0->monSpriteData.spriteManager);
}

void sub_020917E0 (PokemonSummaryApp * param0)
{
    VecFx32 v0 = {0, 0, 0x10000};
    UnkStruct_ov115_0226527C v1 = {0, 0, 0};
    fx32 v2 = 0x10000;
    u16 v3 = 0x5c1;

    param0->monSpriteData.unk_00 = sub_020203AC(19);

    sub_02020738(&v0, v2, &v1, v3, 1, param0->monSpriteData.unk_00);
    sub_020206BC(0, FX32_CONST(100), param0->monSpriteData.unk_00);
    sub_020206B0(param0->monSpriteData.unk_00);
    sub_020203D4(param0->monSpriteData.unk_00);
}

static void sub_02091850 (UnkStruct_02091850 * param0)
{
    G3_PolygonAttr(GX_LIGHTMASK_NONE, GX_POLYGONMODE_MODULATE, GX_CULL_NONE, 18, 20, 0);
    G3_Begin(GX_BEGIN_QUADS);

    {
        u32 v0;

        for (v0 = 0; v0 < 4; v0++) {
            G3_Color(GX_RGB(8, 31, 15));
            G3_Vtx(param0[v0].unk_00.x, param0[v0].unk_00.y, param0[v0].unk_00.z);
            G3_Color(GX_RGB(8, 31, 15));
            G3_Vtx(param0[v0].unk_06.x, param0[v0].unk_06.y, param0[v0].unk_06.z);
            G3_Color(GX_RGB(8, 31, 15));
            G3_Vtx(param0[v0].unk_12.x, param0[v0].unk_12.y, param0[v0].unk_12.z);
            G3_Color(GX_RGB(8, 31, 15));
            G3_Vtx(param0[v0].unk_0C.x, param0[v0].unk_0C.y, param0[v0].unk_0C.z);
        }
    }

    G3_End();
}

static void sub_020918CC (VecFx16 * param0, VecFx16 * param1)
{
    param0->x += param1->x;
    param0->y += param1->y;
    param0->z += param1->z;
}

static void sub_020918EC (PokemonSummaryApp * param0)
{
    u32 v0;

    if (param0->unk_410 == 4) {
        sub_0208FCD4(param0);
        return;
    }

    if (param0->unk_410 == 3) {
        for (v0 = 0; v0 < 4; v0++) {
            param0->unk_2F0[v0] = param0->unk_3B0[v0];
        }
    } else {
        for (v0 = 0; v0 < 4; v0++) {
            sub_020918CC(&param0->unk_2F0[v0].unk_00, &param0->unk_350[v0].unk_00);
            sub_020918CC(&param0->unk_2F0[v0].unk_06, &param0->unk_350[v0].unk_06);
            sub_020918CC(&param0->unk_2F0[v0].unk_0C, &param0->unk_350[v0].unk_0C);
            sub_020918CC(&param0->unk_2F0[v0].unk_12, &param0->unk_350[v0].unk_12);
        }
    }

    param0->unk_410++;

    if (param0->unk_410 == 4) {
        sub_0208FB54(param0, 1);
    } else {
        sub_0208FB54(param0, 0);
    }
}

void sub_020919E8 (PokemonSummaryApp * param0)
{
    u32 v0;

    if (param0->page != 4) {
        return;
    }

    param0->unk_2F0[0].unk_00 = Unk_020F4FFC[0][3].unk_06;
    param0->unk_2F0[0].unk_06 = Unk_020F4FFC[0][3].unk_06;
    param0->unk_2F0[0].unk_0C = Unk_020F4FFC[0][3].unk_06;
    param0->unk_2F0[0].unk_12 = Unk_020F4FFC[0][3].unk_06;

    param0->unk_2F0[1].unk_00 = Unk_020F4FFC[1][2].unk_06;
    param0->unk_2F0[1].unk_06 = Unk_020F4FFC[1][2].unk_06;
    param0->unk_2F0[1].unk_0C = Unk_020F4FFC[1][2].unk_06;
    param0->unk_2F0[1].unk_12 = Unk_020F4FFC[1][2].unk_06;

    param0->unk_2F0[2].unk_00 = Unk_020F4FFC[2][1].unk_06;
    param0->unk_2F0[2].unk_06 = Unk_020F4FFC[2][1].unk_06;
    param0->unk_2F0[2].unk_0C = Unk_020F4FFC[2][1].unk_06;
    param0->unk_2F0[2].unk_12 = Unk_020F4FFC[2][1].unk_06;

    param0->unk_2F0[3].unk_00 = Unk_020F4FFC[3][0].unk_06;
    param0->unk_2F0[3].unk_06 = Unk_020F4FFC[3][0].unk_06;
    param0->unk_2F0[3].unk_0C = Unk_020F4FFC[3][0].unk_06;
    param0->unk_2F0[3].unk_12 = Unk_020F4FFC[3][0].unk_06;

    sub_02091D50(param0);
}

static void sub_02091B78 (const UnkStruct_02091B78 * param0, VecFx16 * param1, u8 param2)
{
    if (param2 == 0xff) {
        *param1 = param0->unk_00;
    } else if (param2 == 0) {
        *param1 = param0->unk_06;
    } else {
        param1->x = param0->unk_06.x + param0->unk_0C.x * param2;
        param1->y = param0->unk_06.y + param0->unk_0C.y * param2;
        param1->z = param0->unk_06.z + param0->unk_0C.z * param2;
    }
}

static void sub_02091BD4 (VecFx16 * param0, VecFx16 * param1, VecFx16 * param2)
{
    param2->x = FX_F32_TO_FX16(FX_FX16_TO_F32(param1->x - param0->x) / 4);
    param2->y = FX_F32_TO_FX16(FX_FX16_TO_F32(param1->y - param0->y) / 4);
    param2->z = FX_F32_TO_FX16(FX_FX16_TO_F32(param1->z - param0->z) / 4);
}

void sub_02091D50 (PokemonSummaryApp * param0)
{
    u32 v0;

    sub_02091B78(&Unk_020F4FFC[0][0], &param0->unk_3B0[0].unk_00, param0->monData.cool);
    sub_02091B78(&Unk_020F4FFC[0][1], &param0->unk_3B0[0].unk_06, param0->monData.beauty);
    sub_02091B78(&Unk_020F4FFC[0][2], &param0->unk_3B0[0].unk_12, param0->monData.cute);
    sub_02091B78(&Unk_020F4FFC[0][3], &param0->unk_3B0[0].unk_0C, 0);

    sub_02091B78(&Unk_020F4FFC[1][0], &param0->unk_3B0[1].unk_00, param0->monData.tough);
    sub_02091B78(&Unk_020F4FFC[1][1], &param0->unk_3B0[1].unk_06, param0->monData.cool);
    sub_02091B78(&Unk_020F4FFC[1][2], &param0->unk_3B0[1].unk_12, 0);
    sub_02091B78(&Unk_020F4FFC[1][3], &param0->unk_3B0[1].unk_0C, param0->monData.smart);

    sub_02091B78(&Unk_020F4FFC[2][0], &param0->unk_3B0[2].unk_00, param0->monData.tough);
    sub_02091B78(&Unk_020F4FFC[2][1], &param0->unk_3B0[2].unk_06, 0);
    sub_02091B78(&Unk_020F4FFC[2][2], &param0->unk_3B0[2].unk_12, param0->monData.cute);
    sub_02091B78(&Unk_020F4FFC[2][3], &param0->unk_3B0[2].unk_0C, param0->monData.smart);

    sub_02091B78(&Unk_020F4FFC[3][0], &param0->unk_3B0[3].unk_00, 0);
    sub_02091B78(&Unk_020F4FFC[3][1], &param0->unk_3B0[3].unk_06, param0->monData.beauty);
    sub_02091B78(&Unk_020F4FFC[3][2], &param0->unk_3B0[3].unk_12, param0->monData.cute);
    sub_02091B78(&Unk_020F4FFC[3][3], &param0->unk_3B0[3].unk_0C, param0->monData.smart);

    for (v0 = 0; v0 < 4; v0++) {
        sub_02091BD4(&param0->unk_2F0[v0].unk_00, &param0->unk_3B0[v0].unk_00, &param0->unk_350[v0].unk_00);
        sub_02091BD4(&param0->unk_2F0[v0].unk_06, &param0->unk_3B0[v0].unk_06, &param0->unk_350[v0].unk_06);
        sub_02091BD4(&param0->unk_2F0[v0].unk_0C, &param0->unk_3B0[v0].unk_0C, &param0->unk_350[v0].unk_0C);
        sub_02091BD4(&param0->unk_2F0[v0].unk_12, &param0->unk_3B0[v0].unk_12, &param0->unk_350[v0].unk_12);
    }

    param0->unk_410 = 0;
}

void sub_02091F8C (PokemonSummaryApp * param0)
{
    ArchivedSprite v0;
    void * v1;

    param0->monSpriteData.spriteManager = sub_0200762C(19);

    v1 = PokemonSummary_MonData(param0);

    if (param0->data->dataType == 2) {
        BoxPokemon_BuildArchivedSprite(&v0, v1, 2, 0);
    } else {
        Pokemon_BuildArchivedSprite(&v0, v1, 2);
    }

    PokeSprite_LoadAnimationFrames(param0->narcPlPokeData, param0->monSpriteData.frames, param0->monData.species, 1);

    param0->monSpriteData.flip = PokemonPersonalData_GetFormValue(param0->monData.species, param0->monData.form, 28) ^ 1;
    param0->monSpriteData.sprite = sub_02007C34(param0->monSpriteData.spriteManager, &v0, 52, 104, 0, 0, param0->monSpriteData.frames, NULL);

    sub_02007DEC(param0->monSpriteData.sprite, 35, param0->monSpriteData.flip);
}

void sub_02092028 (PokemonSummaryApp * param0)
{
    if (param0->monData.isEgg != 0) {
        PokeSprite_LoadAnimation(param0->narcPlPokeData, param0->monSpriteData.animationSys, param0->monSpriteData.sprite, 0, 2, param0->monSpriteData.flip, 0);
    } else {
        sub_02007B98(param0->monSpriteData.sprite, 1);
        PokeSprite_LoadAnimation(param0->narcPlPokeData, param0->monSpriteData.animationSys, param0->monSpriteData.sprite, param0->monData.species, 2, param0->monSpriteData.flip, 0);
    }
}

void sub_02092098 (PokemonSummaryApp * param0)
{
    sub_02016114(param0->monSpriteData.animationSys, 0);
    sub_02007B6C(param0->monSpriteData.spriteManager);
    sub_02091F8C(param0);
    sub_02092028(param0);
}
