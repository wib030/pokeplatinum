#include <nitro.h>
#include <string.h>

#include "pch/global_pch.h"
#include "assert.h"

#include "consts/abilities.h"
#include "constants/battle.h"
#include "constants/items.h"
#include "constants/species.h"
#include "constants/battle/trainer_ai.h"

#include "struct_decls/struct_party_decl.h"
#include "struct_decls/battle_system.h"
#include "struct_defs/battle_system.h"

#include "battle/common.h"
#include "battle/ai_context.h"
#include "battle/battle_context.h"
#include "battle/battle_controller.h"
#include "battle/trainer_ai.h"
#include "battle/trainer_ai_overflow.h"
#include "battle/battle_lib.h"

#include "flags.h"
#include "pokemon.h"
#include "party.h"
#include "battle/ov16_0223DF00.h"

static const u16 sRiskyMoves[] = {
    BATTLE_EFFECT_HALVE_DEFENSE,
    BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP,
    BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT,
    BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT_FLINCH,
    BATTLE_EFFECT_RECHARGE_AFTER,
    BATTLE_EFFECT_CHARGE_TURN_DEF_UP,
    BATTLE_EFFECT_SKIP_CHARGE_TURN_IN_SUN,
    BATTLE_EFFECT_SPIT_UP,
    BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT,
    BATTLE_EFFECT_LOWER_OWN_ATK_AND_DEF,
    BATTLE_EFFECT_DECREASE_POWER_WITH_LESS_USER_HP,
    BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING,
    BATTLE_EFFECT_RECOIL_HALF,
    0xFFFF
};

static const u16 sAltPowerCalcMoves[] = {
    BATTLE_EFFECT_RANDOM_POWER_BASED_ON_IVS,
    BATTLE_EFFECT_POWER_BASED_ON_LOW_SPEED,
    BATTLE_EFFECT_NATURAL_GIFT,
    BATTLE_EFFECT_JUDGEMENT,
    BATTLE_EFFECT_40_DAMAGE_FLAT,
    BATTLE_EFFECT_LEVEL_DAMAGE_FLAT,
    BATTLE_EFFECT_RANDOM_DAMAGE_1_TO_150_LEVEL,
    BATTLE_EFFECT_POWER_BASED_ON_FRIENDSHIP,
    BATTLE_EFFECT_POWER_BASED_ON_LOW_FRIENDSHIP,
    BATTLE_EFFECT_10_DAMAGE_FLAT,
    BATTLE_EFFECT_INCREASE_POWER_WITH_WEIGHT,
    BATTLE_EFFECT_RANDOM_POWER_MAYBE_HEAL,
    BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_STAT_UP,
    BATTLE_EFFECT_SET_HP_EQUAL_TO_USER,
    BATTLE_EFFECT_FLING,
    BATTLE_EFFECT_HALVE_HP,
    BATTLE_EFFECT_NATURE_POWER,
    0xFFFF
};

static const u16 sNonVolatileStatusMoves[] = {
    BATTLE_EFFECT_STATUS_BADLY_POISON,
    BATTLE_EFFECT_STATUS_POISON,
    BATTLE_EFFECT_STATUS_BURN,
    BATTLE_EFFECT_STATUS_PARALYZE,
    BATTLE_EFFECT_STATUS_SLEEP,
    0xFFFF
};

static const u16 sDamagingNonVolatileStatusMoves[] = {
    BATTLE_EFFECT_STATUS_BADLY_POISON,
    BATTLE_EFFECT_STATUS_POISON,
    BATTLE_EFFECT_STATUS_BURN,
    0xFFFF
};

static const u16 sDefensiveStatusMoves[] = {
    BATTLE_EFFECT_STATUS_BADLY_POISON,
    BATTLE_EFFECT_STATUS_POISON,
    BATTLE_EFFECT_STATUS_BURN,
    BATTLE_EFFECT_STATUS_LEECH_SEED,
    BATTLE_EFFECT_BIND_HIT,
    BATTLE_EFFECT_DISABLE,
    BATTLE_EFFECT_ENCORE,
    BATTLE_EFFECT_DECREASE_LAST_MOVE_PP,
    BATTLE_EFFECT_INFATUATE,
    BATTLE_EFFECT_TORMENT,
    BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE,
    BATTLE_EFFECT_CONFUSE_ALL,
    BATTLE_EFFECT_STATUS_CONFUSE,
    BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION,
    BATTLE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION,
    BATTLE_EFFECT_TAUNT,
    BATTLE_EFFECT_TRANSFER_STATUS,
    0xFFFF
};

static const u16 sChipDamageMoves[] = {
    BATTLE_EFFECT_STATUS_BADLY_POISON,
    BATTLE_EFFECT_STATUS_POISON,
    BATTLE_EFFECT_STATUS_BURN,
    BATTLE_EFFECT_CURSE,
    BATTLE_EFFECT_STATUS_LEECH_SEED,
    BATTLE_EFFECT_STATUS_NIGHTMARE,
    BATTLE_EFFECT_BIND_HIT,
    BATTLE_EFFECT_WHIRLPOOL,
    0xFFFF
};

static const u16 sRemoveAbilityMoves[] = {
    BATTLE_EFFECT_SUPRESS_ABILITY,
    BATTLE_EFFECT_SET_ABILITY_TO_INSOMNIA,
    BATTLE_EFFECT_SWITCH_ABILITIES,
    0xFFFF
};

static const u8 sRemovableAbilities[] = {
    ABILITY_SPEED_BOOST,
    ABILITY_STURDY,
    ABILITY_VOLT_ABSORB,
    ABILITY_WATER_ABSORB,
    ABILITY_COMPOUND_EYES,
    ABILITY_IMMUNITY,
    ABILITY_FLASH_FIRE,
    ABILITY_SHIELD_DUST,
    ABILITY_SHADOW_TAG,
    ABILITY_ROUGH_SKIN,
    ABILITY_WONDER_GUARD,
    ABILITY_LEVITATE,
    ABILITY_EFFECT_SPORE,
    ABILITY_SYNCHRONIZE,
    ABILITY_CLEAR_BODY,
    ABILITY_NATURAL_CURE,
    ABILITY_LIGHTNING_ROD,
    ABILITY_SERENE_GRACE,
    ABILITY_SWIFT_SWIM,
    ABILITY_CHLOROPHYLL,
    ABILITY_ILLUMINATE,
    ABILITY_HUGE_POWER,
    ABILITY_MAGNET_PULL,
    ABILITY_SOUNDPROOF,
    ABILITY_RAIN_DISH,
    ABILITY_PRESSURE,
    ABILITY_THICK_FAT,
    ABILITY_EARLY_BIRD,
    ABILITY_FLAME_BODY,
    ABILITY_HYPER_CUTTER,
    ABILITY_HUSTLE,
    ABILITY_PLUS,
    ABILITY_MINUS,
    ABILITY_FORECAST,
    ABILITY_SHED_SKIN,
    ABILITY_GUTS,
    ABILITY_MARVEL_SCALE,
    ABILITY_ROCK_HEAD,
    ABILITY_ARENA_TRAP,
    ABILITY_WHITE_SMOKE,
    ABILITY_PURE_POWER,
    ABILITY_AIR_LOCK,
    ABILITY_MOTOR_DRIVE,
    ABILITY_RIVALRY,
    ABILITY_SNOW_CLOAK,
    ABILITY_GLUTTONY,
    ABILITY_ANGER_POINT,
    ABILITY_UNBURDEN,
    ABILITY_HEATPROOF,
    ABILITY_SIMPLE,
    ABILITY_DRY_SKIN,
    ABILITY_POISON_HEAL,
    ABILITY_ADAPTABILITY,
    ABILITY_SKILL_LINK,
    ABILITY_HYDRATION,
    ABILITY_SOLAR_POWER,
    ABILITY_MAGIC_GUARD,
    ABILITY_NO_GUARD,
    ABILITY_TECHNICIAN,
    ABILITY_LEAF_GUARD,
    ABILITY_MOLD_BREAKER,
    ABILITY_AFTERMATH,
    ABILITY_UNAWARE,
    ABILITY_TINTED_LENS,
    ABILITY_FILTER,
    ABILITY_SCRAPPY,
    ABILITY_STORM_DRAIN,
    ABILITY_ICE_BODY,
    ABILITY_SOLID_ROCK,
    ABILITY_RECKLESS,
    ABILITY_FLOWER_GIFT,
    ABILITY_BAD_DREAMS,
    ABILITY_SLUSH_RUSH,
    ABILITY_MULTISCALE,
    ABILITY_POISON_TOUCH,
    ABILITY_DEFIANT,
    ABILITY_COMPETITIVE,
    ABILITY_FRESH_MILK,
    ABILITY_HEADACHE,
    ABILITY_RELENTLESS,
    ABILITY_SHEER_FORCE,
    ABILITY_CORROSION,
    ABILITY_STRONG_JAW,
    ABILITY_HOTHEADED,
    ABILITY_MEGA_LAUNCHER,
    ABILITY_TIDAL_FORCE,
    ABILITY_FREE_SAMPLE,
    ABILITY_SHAKEDOWN,
    ABILITY_FLARE_BOOST,
    ABILITY_CHLOROPLAST,
    ABILITY_PHOTOSYNTHESIS,
    ABILITY_SHARPNESS,
    ABILITY_STRANGLE_WEED,
    ABILITY_PEST,
    ABILITY_ROCK_SOLID,
    ABILITY_SUCTION_CUPS,
    ABILITY_THIRSTY,
    ABILITY_AWARE,
    0xFFFF
};

static const u8 sPositiveHeldItemEffects[] = {
    HOLD_EFFECT_HP_RESTORE,
    HOLD_EFFECT_SLP_RESTORE,
    HOLD_EFFECT_PP_RESTORE,
    HOLD_EFFECT_STATUS_RESTORE,
    HOLD_EFFECT_HP_PCT_RESTORE,
    HOLD_EFFECT_WEAKEN_SE_FIRE,
    HOLD_EFFECT_WEAKEN_SE_WATER,
    HOLD_EFFECT_WEAKEN_SE_ELECTRIC,
    HOLD_EFFECT_WEAKEN_SE_GRASS,
    HOLD_EFFECT_WEAKEN_SE_ICE,
    HOLD_EFFECT_WEAKEN_SE_FIGHT,
    HOLD_EFFECT_WEAKEN_SE_GROUND,
    HOLD_EFFECT_WEAKEN_SE_FLYING,
    HOLD_EFFECT_WEAKEN_SE_ROCK,
    HOLD_EFFECT_PINCH_ATK_UP,
    HOLD_EFFECT_PINCH_SPEED_UP,
    HOLD_EFFECT_PINCH_SPATK_UP,
    HOLD_EFFECT_PINCH_RANDOM_UP,
    HOLD_EFFECT_PINCH_PRIORITY,
    HOLD_EFFECT_STATDOWN_RESTORE,
    HOLD_EFFECT_SOMETIMES_PRIORITY,
    HOLD_EFFECT_CHOICE_ATK,
    HOLD_EFFECT_SOMETIMES_FLINCH,
    HOLD_EFFECT_LATI_SPECIAL,
    HOLD_EFFECT_CLAMPERL_SPATK,
    HOLD_EFFECT_CLAMPERL_SPDEF,
    HOLD_EFFECT_MAYBE_ENDURE,
    HOLD_EFFECT_HP_RESTORE_GRADUAL,
    HOLD_EFFECT_PIKA_SPATK_UP,
    HOLD_EFFECT_HP_RESTORE_ON_DMG,
    HOLD_EFFECT_POWER_UP_SE,
    HOLD_EFFECT_EXTEND_SCREENS,
    HOLD_EFFECT_HP_DRAIN_ON_ATK,
    HOLD_EFFECT_CHARGE_SKIP,
    HOLD_EFFECT_ENDURE,
    HOLD_EFFECT_ACCURACY_UP_SLOWER,
    HOLD_EFFECT_HP_RESTORE_PSN_TYPE,
    HOLD_EFFECT_EXTEND_HAIL,
    HOLD_EFFECT_EXTEND_SANDSTORM,
    HOLD_EFFECT_EXTEND_SUN,
    HOLD_EFFECT_EXTEND_RAIN,
    HOLD_EFFECT_CHOICE_SPEED,
    HOLD_EFFECT_SWITCH,
    HOLD_EFFECT_CHOICE_SPATK,
    HOLD_EFFECT_NORMAL_HIT_GHOST,
    HOLD_EFFECT_NO_WEATHER_CHIP_POWDER,
    HOLD_EFFECT_WEAK_RAISE_SPA_ATK,
    HOLD_EFFECT_LEVITATE_POPPED_IF_HIT,
    HOLD_EFFECT_SWITCH_ATTACKER_HIT,
    HOLD_EFFECT_EVIOLITE,
    HOLD_EFFECT_LOADED_DICE,
    HOLD_EFFECT_THREE_FOUR_FIVE_DICE,
    0xFFFF
};

static const u8 sNegativeHeldItemEffects[] = {
    HOLD_EFFECT_PSN_USER,
    HOLD_EFFECT_BRN_USER,
    HOLD_EFFECT_PRIORITY_DOWN,
    HOLD_EFFECT_SPEED_DOWN_GROUNDED,
    HOLD_EFFECT_HP_RESTORE_PSN_TYPE,
    HOLD_EFFECT_DMG_USER_CONTACT_XFR,
    0xFFFF
};

void TrainerAI_EvalMoreMoves_ExpertSingles(BattleSystem* battleSys, BattleContext* battleCtx)
{
    u8 abilityTemp;
    u8 attackerSide, defenderSide;

    if ((AI_CONTEXT.thinkingMask & AI_FLAG_EXPERT) == FALSE)
    {
        return;
    }

    switch (MOVE_DATA(AI_CONTEXT.move).effect)
    {
    default:
        break;

    case BATTLE_EFFECT_RAISE_ALL_STATS_HIT:
        // Expert_Omniboost_CheckIfKill
        if (AI_AttackerKOsDefender(battleSys, battleCtx, AI_CONTEXT.attacker, AI_CONTEXT.defender))
        {
            if (AI_CurrentMoveKills(battleSys, battleCtx, USE_MIN_DAMAGE))
            {
                if (AI_GetRandomNumber(battleSys) < 243)
                {
                    AI_AddToMoveScore(battleSys, battleCtx, 3);
                }
            }
            else
            {
                if (AI_FlagMoveDamageScore(battleSys, battleCtx, USE_MIN_DAMAGE) == AI_NOT_HIGHEST_DAMAGE)
                {
                    AI_AddToMoveScore(battleSys, battleCtx, -12);
                    break;
                }
                else
                {
                    if (AI_GetRandomNumber(battleSys) < 243)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, 1);
                    }
                }
            }
        }

        abilityTemp = AI_GetBattlerAbility(battleSys, battleCtx, AI_CONTEXT.attacker);
        attackerSide = Battler_Side(battleSys, AI_CONTEXT.attacker);

        // Expert_Omniboost_ChanceBoostCheck
        if (abilityTemp == ABILITY_SERENE_GRACE
            || (battleCtx->sideConditionsMask[attackerSide] & SIDE_CONDITION_LUCKY_CHANT))
        {
            if (AI_GetRandomNumber(battleSys) < 192)
            {
                AI_AddToMoveScore(battleSys, battleCtx, 1);

                if (AI_GetRandomNumber(battleSys) < 64)
                {
                    AI_AddToMoveScore(battleSys, battleCtx, 1);
                }
            }
        }

        // Expert_Omniboost_Main
        if (AI_GetMoveEffectiveness(battleSys, battleCtx) == TYPE_MULTI_IMMUNE)
        {
            AI_AddToMoveScore(battleSys, battleCtx, -12);
            break;
        }

        if (AI_GetBattlerHPPercent(battleSys, battleCtx, AI_CONTEXT.attacker) < 50)
        {
            if (AI_GetRandomNumber(battleSys) < 128)
            {
                AI_AddToMoveScore(battleSys, battleCtx, -1);
                break;
            }
        }

        if (AI_GetRandomNumber(battleSys) < 128)
        {
            AI_AddToMoveScore(battleSys, battleCtx, 1);
        }

        // Expert_Omniboost_End
        break;
    }

    return;
}

void AI_AddToMoveScore(BattleSystem* battleSys, BattleContext* battleCtx, int val)
{
    AI_CONTEXT.moveScore[AI_CONTEXT.moveSlot] += val;

    if (AI_CONTEXT.moveScore[AI_CONTEXT.moveSlot] < 0) {
        AI_CONTEXT.moveScore[AI_CONTEXT.moveSlot] = 0;
    }

    return;
}

int AI_GetRandomNumber(BattleSystem* battleSys)
{
    return (BattleSystem_RandNext(battleSys) % 256);
}


BOOL AI_CurrentMoveKills(BattleSystem* battleSys, BattleContext* battleCtx, int useDamageRoll)
{
    int roll;
    int riskyIdx;
    int altPowerIdx;
    BOOL moveKills;

    moveKills = FALSE;

    if (useDamageRoll == ROLL_FOR_DAMAGE) {
        roll = AI_CONTEXT.moveDamageRolls[AI_CONTEXT.moveSlot];
    }
    else if (useDamageRoll == USE_MIN_DAMAGE) {
        roll = DAMAGE_VARIANCE_MIN_ROLL;
    }
    else {
        roll = DAMAGE_VARIANCE_MAX_ROLL;
    }

    for (riskyIdx = 0; sRiskyMoves[riskyIdx] != 0xFFFF; riskyIdx++) {
        if (MOVE_DATA(AI_CONTEXT.move).effect == sRiskyMoves[riskyIdx]) {
            break;
        }
    }

    for (altPowerIdx = 0; sAltPowerCalcMoves[altPowerIdx] != 0xFFFF; altPowerIdx++) {
        if (MOVE_DATA(AI_CONTEXT.move).effect == sAltPowerCalcMoves[altPowerIdx]) {
            break;
        }
    }

    if (sAltPowerCalcMoves[altPowerIdx] != 0xFFFF
        || (MOVE_DATA(AI_CONTEXT.move).power > 1 && sRiskyMoves[riskyIdx] == 0xFFFF)) {
        u8 ivs[STAT_MAX];
        for (int stat = STAT_HP; stat < STAT_MAX; stat++) {
            ivs[stat] = BattleMon_Get(battleCtx, AI_CONTEXT.attacker, BATTLEMON_HP_IV + stat, NULL);
        }

        u32 damage = TrainerAI_CalcDamage(battleSys,
            battleCtx,
            AI_CONTEXT.move,
            battleCtx->battleMons[AI_CONTEXT.attacker].heldItem,
            ivs,
            AI_CONTEXT.attacker,
            Battler_Ability(battleCtx, AI_CONTEXT.attacker),
            battleCtx->battleMons[AI_CONTEXT.attacker].moveEffectsData.embargoTurns,
            roll);

        if (battleCtx->battleMons[AI_CONTEXT.defender].curHP <= damage) {
            moveKills = TRUE;
        }
    }

    return moveKills;
}

int AI_FlagMoveDamageScore(BattleSystem* battleSys, BattleContext* battleCtx, int varyDamage)
{
    int i = 0, riskyIdx, altPowerIdx;
    s32 moveDamage[LEARNED_MOVES_MAX];
    u8 ivs[STAT_MAX];

    for (riskyIdx = 0; sRiskyMoves[riskyIdx] != 0xFFFF; riskyIdx++) {
        if (MOVE_DATA(AI_CONTEXT.move).effect == sRiskyMoves[riskyIdx]) {
            break;
        }
    }

    for (altPowerIdx = 0; sAltPowerCalcMoves[altPowerIdx] != 0xFFFF; altPowerIdx++) {
        if (MOVE_DATA(AI_CONTEXT.move).effect == sAltPowerCalcMoves[altPowerIdx]) {
            break;
        }
    }

    if (sAltPowerCalcMoves[altPowerIdx] != 0xFFFF
        || (MOVE_DATA(AI_CONTEXT.move).power > 1 && sRiskyMoves[riskyIdx] == 0xFFFF)) {
        for (i = 0; i < STAT_MAX; i++) {
            ivs[i] = BattleMon_Get(battleCtx, AI_CONTEXT.attacker, BATTLEMON_HP_IV + i, NULL);
        }

        TrainerAI_CalcAllDamage(battleSys,
            battleCtx,
            AI_CONTEXT.attacker,
            battleCtx->battleMons[AI_CONTEXT.attacker].moves,
            moveDamage,
            battleCtx->battleMons[AI_CONTEXT.attacker].heldItem,
            ivs,
            Battler_Ability(battleCtx, AI_CONTEXT.attacker),
            battleCtx->battleMons[AI_CONTEXT.attacker].moveEffectsData.embargoTurns,
            varyDamage);

        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (moveDamage[i] > moveDamage[AI_CONTEXT.moveSlot]) {
                break;
            }
        }

        if (i == LEARNED_MOVES_MAX) {
            return AI_MOVE_IS_HIGHEST_DAMAGE;
        }
        else {
            return AI_NOT_HIGHEST_DAMAGE;
        }
    }

    return AI_NO_COMPARISON_MADE;
}

u8 AI_GetBattlerAbility(BattleSystem* battleSys, BattleContext* battleCtx, int battler)
{

    if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_ABILITY_SUPPRESSED
        || BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS, 0, ABILITY_NEUTRALIZING_GAS)) {
        return ABILITY_NONE;
    }
    else if (AI_CONTEXT.attacker != battler && battler != AI_BATTLER_ATTACKER_PARTNER) {
        // If we already know an opponent's ability, load that ability
        if (AI_CONTEXT.battlerAbilities[battler]) {
            return Battler_Ability(battleCtx, battler);
        }
        else {
            // If the opponent has an ability that traps us, we should already know about it (because it self-announces)
            if (Battler_Ability(battleCtx, battler) == ABILITY_SHADOW_TAG
                || Battler_Ability(battleCtx, battler) == ABILITY_MAGNET_PULL
                || Battler_Ability(battleCtx, battler) == ABILITY_THIRSTY
                || Battler_Ability(battleCtx, battler) == ABILITY_ARENA_TRAP) {
                return battleCtx->battleMons[battler].ability;
            }
            else {
                // Try to guess the opponent's ability (flip a coin)
                int ability1 = PokemonPersonalData_GetSpeciesValue(battleCtx->battleMons[battler].species, MON_DATA_PERSONAL_ABILITY_1);
                int ability2 = PokemonPersonalData_GetSpeciesValue(battleCtx->battleMons[battler].species, MON_DATA_PERSONAL_ABILITY_2);

                if (Battler_Ability(battleCtx, battler) != ABILITY_NONE)
                {
                    if (ability1 && ability2) {
                        if (BattleSystem_RandNext(battleSys) & 1) {
                            return ability1;
                        }
                        else {
                            return ability2;
                        }
                    }
                    else if (ability1) {
                        return ability1;
                    }
                    else {
                        return ability2;
                    }
                }
                else
                {
                    return Battler_Ability(battleCtx, battler);
                }
            }
        }
    }
    else {
        return Battler_Ability(battleCtx, battler);
    }

    if (AI_CONTEXT.thinkingMask & AI_FLAG_PRESCIENT)
    {
        if ((BattleSystem_RandNext(battleSys) % 3) > 0)
        {
            return Battler_Ability(battleCtx, battler);
        }
    }

    return ABILITY_NONE;
}

u32 AI_GetMoveEffectiveness(BattleSystem* battleSys, BattleContext* battleCtx)
{
    u32 damage = TYPE_MULTI_BASE_DAMAGE;
    u32 effectiveness = 0;

    damage = BattleSystem_ApplyTypeChart(battleSys,
        battleCtx,
        AI_CONTEXT.move,
        TrainerAI_MoveType(battleSys, battleCtx, AI_CONTEXT.attacker, AI_CONTEXT.move),
        AI_CONTEXT.attacker,
        AI_CONTEXT.defender,
        damage,
        &effectiveness);

    if (damage == TYPE_MULTI_STAB_DAMAGE * 2) {
        damage = TYPE_MULTI_DOUBLE_DAMAGE;
    }
    else if (damage == TYPE_MULTI_STAB_DAMAGE * 4) {
        damage = TYPE_MULTI_QUADRUPLE_DAMAGE;
    }
    else if (damage == TYPE_MULTI_STAB_DAMAGE / 2) {
        damage = TYPE_MULTI_HALF_DAMAGE;
    }
    else if (damage == TYPE_MULTI_STAB_DAMAGE / 4) {
        damage = TYPE_MULTI_QUARTER_DAMAGE;
    }

    if (effectiveness & MOVE_STATUS_IMMUNE) {

        if ((effectiveness & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)
        {
            damage = TYPE_MULTI_IMMUNE;
        }
    }

    return damage;
}

u32 AI_GetBattlerHPPercent(BattleSystem* battleSys, BattleContext* battleCtx, u8 battler)
{
    u32 hpPercent;

    if (battleCtx->battleMons[battler].maxHP == 0)
    {
        return 0;
    }

    hpPercent = battleCtx->battleMons[battler].curHP * 100 / battleCtx->battleMons[battler].maxHP;

    if (battleCtx->battleMons[battler].maxHP == 1) {
        if (battleCtx->battleMons[battler].heldItem != ITEM_FOCUS_SASH
            && battleCtx->battleMons[battler].heldItem != ITEM_FOCUS_BAND) {

            hpPercent = 1;
        }
        else
        {
            hpPercent = 100;
        }
    }

    return hpPercent;
}