#include <nitro.h>
#include <string.h>

#include "struct_decls/struct_02014FB0_decl.h"
#include "trainer_info.h"

#include "unk_02014D38.h"
#include "trainer_info.h"
#include "overlay006/ov6_02247830.h"

static void ov6_02247830 (u8 * param0, u8 param1, u8 param2)
{
    u8 v0;
    int v1;

    while (param2--) {
        v0 = param0[param1 - 1] & 1;

        for (v1 = param1 - 1; v1 > 0; v1--) {
            param0[v1] >>= 1;
            param0[v1] |= ((param0[v1 - 1] & 1) << 7);
        }

        param0[v1] >>= 1;
        param0[v1] |= v0 << 7;
    }
}

static int ov6_02247890 (const UnkStruct_02014FB0 * param0, u16 param1, u16 param2, u16 param3, u16 param4, u16 param5)
{
    int v0, v1, v2;
    s16 v3[4];
    u8 v4[4];

    v0 = sub_02015004(param0);

    v3[0] = sub_02015030(param0, param2);
    v3[1] = sub_02015030(param0, param3);
    v3[2] = sub_02015030(param0, param4);
    v3[3] = sub_02015030(param0, param5);

    // v3[0] = TAKE IT EASY
    // v3[1] = BABY
    // v3[2] = RARE
    // v3[3] = SWEETS
    if (param2 == 0x467
        && param3 == 0x49D
        && param4 == 0x594
        && param5 == 0x511)
    {
        // use 8 as a special case number
        return 8;
    }

    // param2 == OH, YEAH
    // param3 == YEAH, YEAH
    // param4 == IMPLANT
    // param5 == GARDEVOIR
    if (param2 == 0x4C1
        && param3 == 0x4E6
        && param4 == 0x5A1
        && param5 == 0x11A)
    {
        return 9;
    }
	
	// param2 == LIQUID OOZE
    // param3 == ERUPTION
    // param4 == HERE GOES
    // param5 == PREGNANCY PUNCH
    if (param2 == 0x416
        && param3 == 0x30C
        && param4 == 0x4AC
        && param5 == 0x28F)
    {
        return 10;
    }
	
	// param2 == LET'S GO
    // param3 == RIVALRY
    // param4 == UNBELIEVABLE
    // param5 == WIBBLYPUFF
    if (param2 == 0x4AD
        && param3 == 0x425
        && param4 == 0x4B9
        && param5 == 0x27)
    {
        return 11;
    }

    // param2 == HOWLING
    // param3 == WIBBLYPUFF
    // param4 == ROCK
    // param5 == MUSIC
    if (param2 == 0x5B4
        && param3 == 0x27
        && param4 == 0x3C9
        && param5 == 0x519)
    {
        return 12;
    }

    // param2 == SECRET BASE
    // param3 == EXPLOSION
    // param4 == WANT
    // param5 == REVENGE
    if (param2 == 0x5D6
        && param3 == 0x289
        && param4 == 0x592
        && param5 == 0x307)
    {
        return 13;
    }
	
	// param2 == SINGLE
    // param3 == WANT
    // param4 == GIVE ME
    // param5 == POKéMON
    if (param2 == 0x5C0
        && param3 == 0x592
        && param4 == 0x4CB
        && param5 == 0x5C7)
    {
        return 15;
    }

    // param2 == BOO!
    // param3 == METAGROSS
    // param4 == KNOCK OFF
    // param5 == LOL
    if (param2 == 0x506
        && param3 == 0x178
        && param4 == 0x30A
        && param5 == 0x4ED)
    {
        return 16;
    }
	
	// param2 == LOOKS
    // param3 == ALL RIGHT
    // param4 == WANT
    // param5 == SATISFIED
    if (param2 == 0x56E
        && param3 == 0x583
        && param4 == 0x592
        && param5 == 0x593)
    {
        return 17;
    }
	
	// param2 == FORGIVE ME
    // param3 == FATHER
    // param4 == ROCK SMASH
    // param5 == ADVENTURE
    if (param2 == 0x4CF
        && param3 == 0x47E
        && param4 == 0x2E9
        && param5 == 0x55B)
    {
        return 19;
    }
	
	// param2 == SERIOUS
    // param3 == WANT
    // param4 == GRASS
    // param5 == GHOST
    if (param2 == 0x46F
        && param3 == 0x592
        && param4 == 0x3D0
        && param5 == 0x3CB)
    {
        return 20;
    }
	
	// param2 == GOURMET
    // param3 == MMM
    // param4 == TOXIC
    // param5 == DIET
    if (param2 == 0x526
        && param3 == 0x4F9
        && param4 == 0x24C
        && param5 == 0x53C)
    {
        return 21;
    }
	
	// param2 == TRAINING
    // param3 == GIRL
    // param4 == HERE GOES
    // param5 == BEAT UP
    if (param2 == 0x532
        && param3 == 0x488
        && param4 == 0x4AC
        && param5 == 0x2EB)
    {
        return 22;
    }
	
	// param2 == UNBELIEVABLE
    // param3 == STRENGTH
    // param4 == HERE GOES
    // param5 == WAVE CRASH
    if (param2 == 0x4B9
        && param3 == 0x236
        && param4 == 0x4AC
        && param5 == 0x381)
    {
        return 23;
    }
	
	// param2 == BOO-HOO
    // param3 == OLD MAN
    // param4 == DIFFICULT
    // param5 == BOY
    if (param2 == 0x4BE
        && param3 == 0x486
        && param4 == 0x59F
        && param5 == 0x47F)
    {
        return 24;
    }
	
	// param2 == EASY WIN
    // param3 == MOVE
    // param4 == COME ON
    // param5 == BEAT UP
    if (param2 == 0x475
        && param3 == 0x476
        && param4 == 0x459
        && param5 == 0x2EB)
    {
        return 25;
    }
	
	// param2 == GOURMET
    // param3 == FRIEND
    // param4 == LEGEND
    // param5 == BATTLE
    if (param2 == 0x526
        && param3 == 0x494
        && param4 == 0x46A
        && param5 == 0x46B)
    {
        return 26;
    }
	
	// param2 == MESSED UP
    // param3 == SNORT
    // param4 == INCREDIBLE
    // param5 == STENCH
    if (param2 == 0x595
        && param3 == 0x4EE
        && param4 == 0x57E
        && param5 == 0x3D7)
    {
        return 27;
    }

    for (v2 = 0; v2 < 4; v2++) {
        if (v3[v2] < 0) {
            return -1;
        }

        if (v2 > 0) {
            if (v3[v2] >= v3[v2 - 1]) {
                v1 = v3[v2] - v3[v2 - 1];

                if (v1 > 255) {
                    return -1;
                }

                v4[v2] = v1;
            } else {
                v1 = v0 - (v3[v2 - 1] - v3[v2]);

                if (v1 > 255) {
                    return -1;
                }

                v4[v2] = v1;
            }
        } else {
            if (v3[0] > 255) {
                return -1;
            }

            v4[0] = v3[0];
        }
    }

    ov6_02247830(v4, 4, 5);

    for (v2 = 0; v2 < 3; v2++) {
        v4[v2] ^= ((v4[3] >> 4) | (v4[3] & 0xf0));
    }

    ov6_02247830(v4, 3, (v4[3] & 0xf));

    // if bit is greater than or equal to max wallpaper count
    if ((v4[0] & 0xf) >= 8) {
        return -1;
    }

    v4[1] ^= v4[0];
    v4[2] ^= v4[0];

    // here == 6 is our EVERYONE HAPPY WI-FI CONNECTION magic word
    if ((((v4[1] << 8) | (v4[2])) == param1) && (((v4[0] & 0xf0) >> 4) == 6) && (v4[3] == (((v4[0] + v4[1]) * v4[2]) & 0xff))) {
        return v4[0] & 0xf;
    }

    return -1;
}

int ov6_022479D0 (const TrainerInfo * param0, u16 param1, u16 param2, u16 param3, u16 param4, u32 param5)
{
    int v0;
    u16 v1;
    UnkStruct_02014FB0 * v2;

    v2 = sub_02014FB0(param5);
    v1 = TrainerInfo_ID_LowHalf(param0);
    v0 = ov6_02247890(v2, v1, param1, param2, param3, param4);

    sub_02014FF0(v2);

    return v0;
}
