#ifndef POKEPLATINUM_CONSTANTS_ITEMS_H
#define POKEPLATINUM_CONSTANTS_ITEMS_H

#ifndef __ASM_PM_
#include "consts/items.h"

enum {
    BATTLE_POCKET_RECOVER_HP = 0,
    BATTLE_POCKET_RECOVER_STATUS,
    BATTLE_POCKET_POKE_BALLS,
    BATTLE_POCKET_BATTLE_ITEMS,

    BATTLE_POCKET_MAX,
};

enum {
    ITEM_TYPE_FULL_RESTORE = 0,
    ITEM_TYPE_RECOVER_HP,
    ITEM_TYPE_RECOVER_STATUS,
    ITEM_TYPE_STAT_BOOSTER,
    ITEM_TYPE_GUARD_SPEC,

    ITEM_TYPE_MAX
};
#endif // __ASM_PM_

#define POCKET_ITEMS        0
#define POCKET_MEDICINE     1
#define POCKET_BALLS        2
#define POCKET_TMHMS        3
#define POCKET_BERRIES      4
#define POCKET_MAIL         5
#define POCKET_BATTLE_ITEMS 6
#define POCKET_KEY_ITEMS    7
#define POCKET_MAX          8

#define ITEM_RECOVER_CONFUSION  (1 << 0)
#define ITEM_RECOVER_PARALYSIS  (1 << 1)
#define ITEM_RECOVER_FREEZE     (1 << 2)
#define ITEM_RECOVER_BURN       (1 << 3)
#define ITEM_RECOVER_POISON     (1 << 4)
#define ITEM_RECOVER_SLEEP      (1 << 5)
#define ITEM_RECOVER_FULL       (ITEM_RECOVER_SLEEP \
                                | ITEM_RECOVER_POISON \
                                | ITEM_RECOVER_BURN \
                                | ITEM_RECOVER_FREEZE \
                                | ITEM_RECOVER_PARALYSIS)

#define FIRST_MAIL_IDX ITEM_GRASS_MAIL
#define LAST_MAIL_IDX ITEM_BRICK_MAIL
#define NUM_MAILS (LAST_MAIL_IDX - FIRST_MAIL_IDX + 1)

#define FIRST_BERRY_IDX ITEM_CHERI_BERRY
#define LAST_BERRY_IDX ITEM_ROWAP_BERRY
#define NUM_BERRIES (LAST_BERRY_IDX - FIRST_BERRY_IDX + 1)

#define NUM_ITEMS                MAX_ITEMS - 1

#define FLING_EFFECT_NONE                0
#define FLING_EFFECT_PRZ_RESTORE         1
#define FLING_EFFECT_SLP_RESTORE         2
#define FLING_EFFECT_PSN_RESTORE         3
#define FLING_EFFECT_BRN_RESTORE         4
#define FLING_EFFECT_FRZ_RESTORE         5
#define FLING_EFFECT_PP_RESTORE          6
#define FLING_EFFECT_HP_RESTORE          7
#define FLING_EFFECT_CNF_RESTORE         8
#define FLING_EFFECT_ALL_RESTORE         9
#define FLING_EFFECT_HP_PCT_RESTORE     10
#define FLING_EFFECT_HP_RESTORE_SPICY   11
#define FLING_EFFECT_HP_RESTORE_DRY     12
#define FLING_EFFECT_HP_RESTORE_SWEET   13
#define FLING_EFFECT_HP_RESTORE_BITTER  14
#define FLING_EFFECT_HP_RESTORE_SOUR    15
#define FLING_EFFECT_ATK_UP             16
#define FLING_EFFECT_DEF_UP             17
#define FLING_EFFECT_SPEED_UP           18
#define FLING_EFFECT_SPATK_UP           19
#define FLING_EFFECT_SPDEF_UP           20
#define FLING_EFFECT_CRIT_UP            21
#define FLING_EFFECT_RANDOM_UP2         22
#define FLING_EFFECT_TEMP_ACC_UP        23
#define FLING_EFFECT_STATDOWN_RESTORE   24
#define FLING_EFFECT_HEAL_INFATUATION   25
#define FLING_EFFECT_FLINCH             26
#define FLING_EFFECT_PARALYZE           27
#define FLING_EFFECT_POISON             28
#define FLING_EFFECT_BADLY_POISON       29
#define FLING_EFFECT_BURN               30
#define FLING_EFFECT_TRICK_ROOM         31
#define FLING_EFFECT_GRAVITY            32
#define FLING_EFFECT_INFATUATION        33
#define FLING_EFFECT_HAZE               34
#define FLING_EFFECT_RAIN				35
#define FLING_EFFECT_SUN				36
#define FLING_EFFECT_DEFOG				37
#define FLING_EFFECT_WAKE_UP_SLAP		38
#define FLING_EFFECT_NIGHTMARE			39
#define FLING_EFFECT_CONFUSION			40
#define FLING_EFFECT_CHIP				41
#define FLING_EFFECT_LOWER_EVASION		42
#define FLING_EFFECT_LOWER_ACC		    43
#define FLING_EFFECT_INFLICT_CURSE		44
#define FLING_EFFECT_INFLICT_INGRAIN	45
#define FLING_EFFECT_SAND				46
#define FLING_EFFECT_HAIL				47
#define FLING_EFFECT_USER_ATK_UP        48
#define FLING_EFFECT_USER_DEF_UP        49
#define FLING_EFFECT_USER_SPEED_UP      50
#define FLING_EFFECT_USER_SPATK_UP      51
#define FLING_EFFECT_USER_SPDEF_UP      52
#define FLING_EFFECT_PIVOT              53

#define PLUCK_EFFECT_NONE                0
#define PLUCK_EFFECT_PRZ_RESTORE         1
#define PLUCK_EFFECT_SLP_RESTORE         2
#define PLUCK_EFFECT_PSN_RESTORE         3
#define PLUCK_EFFECT_BRN_RESTORE         4
#define PLUCK_EFFECT_FRZ_RESTORE         5
#define PLUCK_EFFECT_PP_RESTORE          6
#define PLUCK_EFFECT_HP_RESTORE          7
#define PLUCK_EFFECT_CNF_RESTORE         8
#define PLUCK_EFFECT_ALL_RESTORE         9
#define PLUCK_EFFECT_HP_PCT_RESTORE     10
#define PLUCK_EFFECT_HP_RESTORE_SPICY   11
#define PLUCK_EFFECT_HP_RESTORE_DRY     12
#define PLUCK_EFFECT_HP_RESTORE_SWEET   13
#define PLUCK_EFFECT_HP_RESTORE_BITTER  14
#define PLUCK_EFFECT_HP_RESTORE_SOUR    15
#define PLUCK_EFFECT_ATK_UP             16
#define PLUCK_EFFECT_DEF_UP             17
#define PLUCK_EFFECT_SPEED_UP           18
#define PLUCK_EFFECT_SPATK_UP           19
#define PLUCK_EFFECT_SPDEF_UP           20
#define PLUCK_EFFECT_CRIT_UP            21
#define PLUCK_EFFECT_RANDOM_UP2         22
#define PLUCK_EFFECT_TEMP_ACC_UP        23

#define ITEM_RETURN_ID           0xFFFF

#endif // POKEPLATINUM_CONSTANTS_ITEMS_H
