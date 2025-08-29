#include <nitro.h>
#include <string.h>

#include "rtc.h"
#include "overlay006/ov6_02246B74.h"

// special_dates.c

typedef struct {
    int size;
    const u16 * dayModifier;
} UnkStruct_ov6_0224954C;

#define MODIFIER_0        0
#define MODIFIER_MINUS_5  1
#define MODIFIER_PLUS_5   2
#define MODIFIER_MINUS_10 3
#define MODIFIER_PLUS_10  4
#define MODIFIER_MINUS_20 5
#define MODIFIER_PLUS_20  6

// January
static const u16 Unk_ov6_02249522[5] = {
    ((1 << 8) | (MODIFIER_MINUS_10 & 0xff)),    // New Year's Day
    ((6 << 8) | (MODIFIER_MINUS_10 & 0xff)), // // Three Kings' Day
    ((11 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Coming of age day (?) (on average from 2009 - 2011)
    ((12 << 8) | (MODIFIER_PLUS_10 & 0xff)),    // Junichi Masuda Birthday
    ((29 << 8) | (MODIFIER_PLUS_5 & 0xff))      // Fire Red and Leaf Green release date
};

// February
static const u16 Unk_ov6_02249502[5] = {
    ((2 << 8) | (MODIFIER_MINUS_5 & 0xff)),  // // Groundhog's Day
    ((3 << 8) | (MODIFIER_PLUS_5 & 0xff)),      // Setsubun (modern / average date)
    ((11 << 8) | (MODIFIER_MINUS_5 & 0xff)),    // National Foundation Day
    ((14 << 8) | (MODIFIER_PLUS_10 & 0xff)),    // Valentine's Day
    ((27 << 8) | (MODIFIER_PLUS_10 & 0xff))     // Green release
};

// March
static const u16 Unk_ov6_022494F4[3] = {
    ((3 << 8) | (MODIFIER_PLUS_5 & 0xff)),      // Hinamatsuri
    ((18 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Ruby Sapphire (NA) (off by 1 day due to time zone)
    ((21 << 8) | (MODIFIER_MINUS_10 & 0xff))    // Vernal Equinox
};

// April
static const u16 Unk_ov6_0224951A[7] = {
    ((1 << 8) | (MODIFIER_PLUS_5 & 0xff)),      // Anime anniversary
    ((14 << 8) | (MODIFIER_PLUS_5 & 0xff)),  // // Lyss birthday
    ((22 << 8) | (MODIFIER_PLUS_5 & 0xff)),  // // Holly birthday
    ((25 << 8) | (MODIFIER_PLUS_5 & 0xff)),  // // Diamond and Pearl (NA)
    ((26 << 8) | (MODIFIER_MINUS_5 & 0xff)),    // Game Freak founded
    ((29 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Golden Week
    ((30 << 8) | (MODIFIER_PLUS_5 & 0xff))   // // Maisy birthday
};

// May
static const u16 Unk_ov6_0224950A[4] = {
    ((1 << 8) | (MODIFIER_0 & 0xff)),           // Pokemon Emerald (NA)
    ((3 << 8) | (MODIFIER_PLUS_5 & 0xff)),      // Constitution Memorial Day (Golden Week)
    ((4 << 8) | (MODIFIER_0 & 0xff)),           // Greenery Day (Golden Week)
    ((5 << 8) | (MODIFIER_PLUS_5 & 0xff))       // Children's Day (Golden Week)
};

// June
static const u16 Unk_ov6_022494E0[1] = {
    ((21 << 8) | (MODIFIER_PLUS_5 & 0xff))
};

// July
static const u16 Unk_ov6_022494E8[4] = {
    ((4 << 8) | (MODIFIER_MINUS_20 & 0xff)), // // Independence Day
    ((7 << 8) | (MODIFIER_PLUS_10 & 0xff)),     // Tanabata
    ((18 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Marine Day
    ((24 << 8) | (MODIFIER_PLUS_5 & 0xff))
};

// August
static const u16 Unk_ov6_022494FA[5] = {
    ((7 << 8) | (MODIFIER_PLUS_5 & 0xff)),   // // Elegy birthday
    ((13 << 8) | (MODIFIER_MINUS_5 & 0xff)),    // Obon
    ((14 << 8) | (MODIFIER_MINUS_5 & 0xff)),    // Obon
    ((15 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Obon (main date)
    ((16 << 8) | (MODIFIER_MINUS_5 & 0xff))     // Obon
};

// September
static const u16 Unk_ov6_0224952A[6] = {
    ((7 << 8) | (MODIFIER_PLUS_5 & 0xff)),
    ((12 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Pokemon Yellow (Japan)
    ((15 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Pokemon Emerald (Japan)
    ((20 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Respect for the Aged day
    ((23 << 8) | (MODIFIER_MINUS_10 & 0xff)),   // Nintendo founded
    ((28 << 8) | (MODIFIER_PLUS_5 & 0xff))      // Pokemon Diamond and Pearl (Japan)
};

// October
static const u16 Unk_ov6_022494EE[4] = {
    ((5 << 8) | (MODIFIER_PLUS_5 & 0xff)),
    ((15 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Gold and Silver (NA) or Blue (Japan)
    ((30 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Possibly Halloween
    ((31 << 8) | (MODIFIER_PLUS_20 & 0xff)),  // // Halloween
};

// November
static const u16 Unk_ov6_02249512[4] = {
    ((3 << 8) | (MODIFIER_MINUS_5 & 0xff)),     // Culture Day
    ((12 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Enthronement Day (Akihito)
    ((21 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Ruby Sapphire (Japan)
    ((23 << 8) | (MODIFIER_0 & 0xff))           // Labor Thanksgiving Day
};

// December
static const u16 Unk_ov6_022494E2[3] = {
    ((14 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Crystal (Japan)
    ((23 << 8) | (MODIFIER_PLUS_5 & 0xff)),     // Emperor's birthday (Akihito)
    ((31 << 8) | (MODIFIER_MINUS_5 & 0xff))     // New Year's Eve
};

static const UnkStruct_ov6_0224954C Unk_ov6_0224954C[12] = {
    {0x4, Unk_ov6_02249522},
    {0x4, Unk_ov6_02249502},
    {0x3, Unk_ov6_022494F4},
    {0x4, Unk_ov6_0224951A},
    {0x4, Unk_ov6_0224950A},
    {0x1, Unk_ov6_022494E0},
    {0x3, Unk_ov6_022494E8},
    {0x4, Unk_ov6_022494FA},
    {0x6, Unk_ov6_0224952A},
    {0x3, Unk_ov6_022494EE},
    {0x4, Unk_ov6_02249512},
    {0x3, Unk_ov6_022494E2}
};

static const int Unk_ov6_02249538[] = {
    0,
    -5,
    +5,
    -10,
    +10,
    -20,
    +20
};

int ov6_02246B74 (const int param0, const BOOL param1)
{
    u8 v0;
    int v1;
    RTCDate v2;
    const UnkStruct_ov6_0224954C * v3;

    if (param1) {
        return param0;
    }

    if (param0 == 0) {
        return 0;
    }

    GetCurrentDate(&v2);

    GF_ASSERT(v2.month > 0);
    GF_ASSERT(v2.month <= 12);

    v3 = &(Unk_ov6_0224954C[v2.month - 1]);

    for (v0 = 0; v0 < v3->size; v0++) {
        if (v2.day == ((v3->dayModifier[v0] >> 8) & 0xff)) {
            v1 = param0 + Unk_ov6_02249538[(v3->dayModifier[v0] & 0xff)];

            if (v1 < 0) {
                v1 = 1;
            }

            return v1;
        }
    }

    return param0;
}
