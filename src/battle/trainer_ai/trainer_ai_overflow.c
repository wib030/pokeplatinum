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

#include "data/battle/weight_to_power.h"

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
    ABILITY_MEMORY,
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
    HOLD_EFFECT_WEAKEN_SE_POISON,
    HOLD_EFFECT_WEAKEN_SE_GROUND,
    HOLD_EFFECT_WEAKEN_SE_FLYING,
    HOLD_EFFECT_WEAKEN_SE_PSYCHIC,
    HOLD_EFFECT_WEAKEN_SE_BUG,
    HOLD_EFFECT_WEAKEN_SE_ROCK,
    HOLD_EFFECT_WEAKEN_SE_GHOST,
    HOLD_EFFECT_WEAKEN_SE_DRAGON,
    HOLD_EFFECT_WEAKEN_SE_DARK,
    HOLD_EFFECT_WEAKEN_SE_STEEL,
    HOLD_EFFECT_WEAKEN_NORMAL,
    HOLD_EFFECT_PINCH_ATK_UP,
    HOLD_EFFECT_PINCH_SPEED_UP,
    HOLD_EFFECT_PINCH_SPATK_UP,
    HOLD_EFFECT_PINCH_RANDOM_UP,
    HOLD_EFFECT_PINCH_PRIORITY,
    HOLD_EFFECT_STATDOWN_RESTORE,
    HOLD_EFFECT_SOMETIMES_PRIORITY,
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
    HOLD_EFFECT_NORMAL_HIT_GHOST,
    HOLD_EFFECT_NO_WEATHER_CHIP_POWDER,
    HOLD_EFFECT_WEAK_RAISE_SPA_ATK,
    HOLD_EFFECT_LEVITATE_POPPED_IF_HIT,
    HOLD_EFFECT_SWITCH_ATTACKER_HIT,
    HOLD_EFFECT_RAISE_SPD_NO_STATUS,
    HOLD_EFFECT_THREE_FOUR_FIVE_DICE,
    HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH,
    HOLD_EFFECT_ACCURACY_UP_SLOWER,
    HOLD_EFFECT_BOOST_REPEATED,
    HOLD_EFFECT_FARFETCHD_CRITRATE_UP,
    HOLD_EFFECT_CHANSEY_CRITRATE_UP,
    HOLD_EFFECT_ACC_REDUCE,
    HOLD_EFFECT_POWER_UP_PHYS,
    HOLD_EFFECT_POWER_UP_SPEC,
    HOLD_EFFECT_HP_DRAIN_ON_ATK,
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

static const u8 trickBadHeldItemEffects[] = {
    HOLD_EFFECT_HP_RESTORE_SPICY,
    HOLD_EFFECT_HP_RESTORE_DRY,
    HOLD_EFFECT_HP_RESTORE_SWEET,
    HOLD_EFFECT_HP_RESTORE_BITTER,
    HOLD_EFFECT_HP_RESTORE_SOUR,
    HOLD_EFFECT_EVS_UP_SPEED_DOWN,
    HOLD_EFFECT_CHOICE_ATK,
    HOLD_EFFECT_CHOICE_SPATK,
    HOLD_EFFECT_CHOICE_SPEED,
    HOLD_EFFECT_SPEED_DOWN_GROUNDED,
    HOLD_EFFECT_PRIORITY_DOWN,
    HOLD_EFFECT_DMG_USER_CONTACT_XFR,
    HOLD_EFFECT_LVLUP_ATK_EV_UP,
    HOLD_EFFECT_LVLUP_DEF_EV_UP,
    HOLD_EFFECT_LVLUP_SPATK_EV_UP,
    HOLD_EFFECT_LVLUP_SPDEF_EV_UP,
    HOLD_EFFECT_LVLUP_SPEED_EV_UP,
    HOLD_EFFECT_LVLUP_HP_EV_UP,
    HOLD_EFFECT_PSN_USER,
    HOLD_EFFECT_BRN_USER,
    HOLD_EFFECT_HP_RESTORE_PSN_TYPE,
    0xFFFF
};

static u16 sWebMoves[] = {
	MOVE_STRING_SHOT,
	MOVE_SPIDER_WEB,
};

void ExpertAI_EvalMoreMoves_Singles(BattleSystem* battleSys, BattleContext* battleCtx);
void AI_AddToMoveScore(BattleSystem* battleSys, BattleContext* battleCtx, int val);
int AI_GetRandomNumber(BattleSystem* battleSys);
BOOL AI_CurrentMoveKills(BattleSystem* battleSys, BattleContext* battleCtx, int useDamageRoll);
int AI_FlagMoveDamageScore(BattleSystem* battleSys, BattleContext* battleCtx, int varyDamage);
u8 AI_GetBattlerAbility(BattleSystem* battleSys, BattleContext* battleCtx, int battler);
u32 AI_GetMoveEffectiveness(BattleSystem* battleSys, BattleContext* battleCtx);
u32 AI_GetBattlerHPPercent(BattleSystem* battleSys, BattleContext* battleCtx, u8 battler);
BOOL ExpertAI_AttackerKOsDefender(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, int defender);
s32 ExpertAI_CalcAllDamage(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, u16* moves, s32* damageVals, u16 heldItem, u8* ivs, int ability, int embargoTurns, int varyDamage);
s32 ExpertAI_CalcDamage(BattleSystem* battleSys, BattleContext* battleCtx, u16 move, u16 heldItem, u8* ivs, int attacker, int ability, int embargoTurns, u8 variance);
int ExpertAI_MoveType(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int move);
BOOL ExpertAI_CanUseMove(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int moveSlot, int opMask);
int ExpertAI_CheckInvalidMoves(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int invalidMoves, int opMask);
BOOL ExpertAI_IsBattlerSpecialAttacker(BattleSystem* battleSys, BattleContext* battleCtx, int battler);
BOOL ExpertAI_IsBattlerPhysicalAttacker(BattleSystem* battleSys, BattleContext* battleCtx, int battler);

void ExpertAI_EvalMoreMoves_Singles(BattleSystem* battleSys, BattleContext* battleCtx)
{
    u8 abilityTemp;
    u8 attackerSide, defenderSide;
	u8 attackerItemEffect, defenderItemEffect;
    int i;

    if ((AI_CONTEXT.thinkingMask & AI_FLAG_EXPERT) == FALSE)
    {
        return;
    }

    for (i = 0; i < LEARNED_MOVES_MAX; i++)
    {
        if (battleCtx->battleMons[AI_CONTEXT.attacker].moves[i])
        {
            if (ExpertAI_CanUseMove(battleSys, battleCtx, AI_CONTEXT.attacker, i, CHECK_INVALID_ALL))
            {
                AI_CONTEXT.moveSlot = i;
                AI_CONTEXT.move = battleCtx->battleMons[AI_CONTEXT.attacker].moves[AI_CONTEXT.moveSlot];
            }
            else
            {
                AI_CONTEXT.move = MOVE_NONE;
            }

            if (AI_CONTEXT.move != MOVE_NONE)
            {
                switch (MOVE_DATA(AI_CONTEXT.move).effect)
                {
                default:
                    break;

                case BATTLE_EFFECT_RAISE_ALL_STATS_HIT:
                    // Expert_Omniboost_CheckIfKill
                    if (ExpertAI_AttackerKOsDefender(battleSys, battleCtx, AI_CONTEXT.attacker, AI_CONTEXT.defender))
                    {
                        if (AI_CurrentMoveKills(battleSys, battleCtx, USE_MIN_DAMAGE))
                        {
                            if (AI_GetRandomNumber(battleSys) < 205)
                            {
                                AI_AddToMoveScore(battleSys, battleCtx, 6);
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

                    // Extra trick code to not use it repeatedly
                case BATTLE_EFFECT_SWITCH_HELD_ITEMS:
                    BOOL attackerShouldNotTrick;

                    attackerShouldNotTrick = FALSE;

                    attackerItemEffect = Battler_HeldItemEffect(battleCtx, AI_CONTEXT.attacker);
                    defenderItemEffect = Battler_HeldItemEffect(battleCtx, AI_CONTEXT.defender);

                    abilityTemp = AI_GetBattlerAbility(battleSys, battleCtx, AI_CONTEXT.attacker);

                    for (i = 0; sPositiveHeldItemEffects[i] != 0xFFFF; i++)
                    {
                        if (sPositiveHeldItemEffects[i] == attackerItemEffect)
                        {
                            attackerShouldNotTrick = TRUE;
                            break;
                        }
                    }
                    if (attackerShouldNotTrick)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -20);
                        break;
                    }
                    for (i = 0; trickBadHeldItemEffects[i] != 0xFFFF; i++)
                    {
                        if (trickBadHeldItemEffects[i] == defenderItemEffect)
                        {
                            switch (defenderItemEffect)
                            {
                            default:
                                attackerShouldNotTrick = TRUE;
                                break;

                            case HOLD_EFFECT_PSN_USER:
                                if (battleCtx->battleMons[AI_CONTEXT.attacker].status & MON_CONDITION_ANY)
                                {
                                    break;
                                }
                                if (MON_HAS_TYPE(AI_CONTEXT.attacker, TYPE_POISON)
                                    || MON_HAS_TYPE(AI_CONTEXT.attacker, TYPE_STEEL))
                                {
                                    break;
                                }
                                if (Battle_AbilityDetersStatus(battleSys, battleCtx, abilityTemp, MON_CONDITION_ANY_POISON))
                                {
                                    break;
                                }
                                attackerShouldNotTrick = TRUE;
                                break;

                            case HOLD_EFFECT_BRN_USER:
                                if (battleCtx->battleMons[AI_CONTEXT.attacker].status & MON_CONDITION_ANY)
                                {
                                    break;
                                }
                                if (MON_HAS_TYPE(AI_CONTEXT.attacker, TYPE_FIRE))
                                {
                                    break;
                                }
                                if (Battle_AbilityDetersStatus(battleSys, battleCtx, abilityTemp, MON_CONDITION_BURN))
                                {
                                    break;
                                }
                                attackerShouldNotTrick = TRUE;
                                break;

                            case HOLD_EFFECT_HP_RESTORE_PSN_TYPE:
                                if (MON_HAS_TYPE(AI_CONTEXT.attacker, TYPE_POISON))
                                {
                                    break;
                                }
                                attackerShouldNotTrick = TRUE;
                                break;
                            }
                        }
                    }
                    if (attackerShouldNotTrick)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -20);
                        break;
                    }

                    if (AICmd_IfCurrentMoveRevealedOverflow(battleSys, battleCtx))
                    {
                        if (battleCtx->movePrevByBattler[AI_CONTEXT.attacker] == AI_CONTEXT.move)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, -20);
                            break;
                        }
                        if (AI_GetRandomNumber(battleSys) < 192)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, -20);
                            break;
                        }
                    }

                    if (Battler_HeldItem(battleCtx, AI_CONTEXT.attacker) == ITEM_NONE)
                    {
                        if (AI_GetRandomNumber(battleSys) < 170)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 5);
                        }
                    }

                    if (AI_GetRandomNumber(battleSys) < 128)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, 1);
                    }
                    break;

                    // Extra endure code to not use it when you have sturdy at full hp
                case BATTLE_EFFECT_SURVIVE_WITH_1_HP:
                    if (Battler_Ability(battleCtx, AI_CONTEXT.attacker) == ABILITY_STURDY
                        || Battler_HeldItemEffect(battleCtx, AI_CONTEXT.attacker) == HOLD_EFFECT_ENDURE)
                    {
                        if (battleCtx->battleMons[AI_CONTEXT.attacker].curHP == battleCtx->battleMons[AI_CONTEXT.attacker].maxHP)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, -20);
                        }
                    }
                    break;

                    // Extra AI for Sticky Web. Still a work in progress.
                case BATTLE_EFFECT_SET_STICKY_WEB:
                    defenderSide = Battler_Side(battleSys, AI_CONTEXT.defender);
                    if (AI_GetBattlerAbility(battleSys, battleCtx, AI_CONTEXT.defender) != ABILITY_MAGIC_BOUNCE
                        || Battler_Ability(battleCtx, AI_CONTEXT.attacker) == ABILITY_MOLD_BREAKER)
                    {
                        if (battleCtx->sideConditionsMask[defenderSide] & SIDE_CONDITION_STICKY_WEB)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, -20);
                            break;
                        }
                        if (AI_IfMoveEffectKnown(battleSys, battleCtx, AI_CONTEXT.defender, BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING)
                            || AI_IfMoveEffectKnown(battleSys, battleCtx, AI_CONTEXT.defender, BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN))
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, -20);
                            break;
                        }
                        if (AI_IfMoveEffectKnown(battleSys, battleCtx, AI_CONTEXT.defender, BATTLE_EFFECT_APPLY_MAGIC_COAT))
                        {
                            if (AI_GetRandomNumber(battleSys) < 128)
                            {
                                AI_AddToMoveScore(battleSys, battleCtx, -20);
                                break;
                            }
                        }
                        if (AI_GetRandomNumber(battleSys) < 128)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 5);
                            break;
                        }
                    }
                    break;

                    // Extra AI for Trick Room. Still a work in progress.
                case BATTLE_EFFECT_TRICK_ROOM:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_TRICK_ROOM)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -20);
                        break;
                    }
                    if (battleCtx->monSpeedOrder[AI_CONTEXT.attacker] < battleCtx->monSpeedOrder[AI_CONTEXT.defender])
                    {
                        if (AI_GetRandomNumber(battleSys) < 205)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 5);
                            break;
                        }
                    }
                    break;

                    // Extra AI for Light Screen.
                case BATTLE_EFFECT_SET_LIGHT_SCREEN:
                    attackerSide = Battler_Side(battleSys, AI_CONTEXT.attacker);

                    if (battleCtx->sideConditionsMask[attackerSide] & SIDE_CONDITION_LIGHT_SCREEN)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -12);
                        break;
                    }

                    if (ExpertAI_IsBattlerSpecialAttacker(battleSys, battleCtx, AI_CONTEXT.defender))
                    {
                        if (AI_GetRandomNumber(battleSys) < 192)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 3);
                            break;
                        }
                    }

                    if (AI_GetRandomNumber(battleSys) < 170)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -1);
                        break;
                    }


                    break;

                    // Extra AI for Reflect.
                case BATTLE_EFFECT_SET_REFLECT:
                    attackerSide = Battler_Side(battleSys, AI_CONTEXT.attacker);

                    if (battleCtx->sideConditionsMask[attackerSide] & SIDE_CONDITION_REFLECT)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -12);
                        break;
                    }

                    if (ExpertAI_IsBattlerPhysicalAttacker(battleSys, battleCtx, AI_CONTEXT.defender))
                    {
                        if (AI_GetRandomNumber(battleSys) < 192)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 3);
                            break;
                        }
                    }

                    if (AI_GetRandomNumber(battleSys) < 170)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -1);
                        break;
                    }

                    break;

                    // Extra AI for Beat Up.
                case BATTLE_EFFECT_BEAT_UP:
                    attackerItemEffect = Battler_HeldItemEffect(battleCtx, AI_CONTEXT.attacker);

                    if (attackerItemEffect == HOLD_EFFECT_SOMETIMES_FLINCH)
                    {
                        if (AI_GetRandomNumber(battleSys) < 242)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 6);
                        }

                        if (AI_GetRandomNumber(battleSys) < 127)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 4);
                        }
                    }
                    break;

                case BATTLE_EFFECT_GROWTH:
                    if (Battler_Ability(battleCtx, AI_CONTEXT.defender) == ABILITY_UNAWARE
                        || BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS, AI_CONTEXT.attacker, ABILITY_MEMORY))
                    {
                        if (Battler_Ability(battleCtx, AI_CONTEXT.attacker) != ABILITY_MOLD_BREAKER)
                        {
                            break;
                        }
                    }

                    if (NO_CLOUD_NINE)
                    {
                        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY)
                        {
                            if (ExpertAI_StatStageLessThan(battleSys, battleCtx, AI_CONTEXT.attacker, BATTLE_STAT_ATTACK, 8)
                                || ExpertAI_StatStageLessThan(battleSys, battleCtx, AI_CONTEXT.attacker, BATTLE_STAT_SP_ATTACK, 8))
                            {
                                if (AI_GetRandomNumber(battleSys) < 128)
                                {
                                    AI_AddToMoveScore(battleSys, battleCtx, 1);
                                    break;
                                }
                            }

                            if (ExpertAI_StatStageLessThan(battleSys, battleCtx, AI_CONTEXT.attacker, BATTLE_STAT_ATTACK, 9)
                                || ExpertAI_StatStageLessThan(battleSys, battleCtx, AI_CONTEXT.attacker, BATTLE_STAT_SP_ATTACK, 9))
                            {
                                if (AI_GetRandomNumber(battleSys) < 85)
                                {
                                    AI_AddToMoveScore(battleSys, battleCtx, 1);
                                    break;
                                }
                            }
                        }
                    }

                    break;

                case BATTLE_EFFECT_SWITCH_HIT_NO_ANIM:
                    if (AI_GetMoveEffectiveness(battleSys, battleCtx) == TYPE_MULTI_IMMUNE)
                    {
                        break;
                    }
                case BATTLE_EFFECT_SWITCH_HIT:
                    if (ExpertAI_IsStatDropped(battleSys, battleCtx, AI_CONTEXT.attacker))
                    {
                        if (AI_GetRandomNumber(battleSys) < 250)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 2);
                        }
                    }
                    break;

                case BATTLE_EFFECT_DEF_DOWN_2:
                    if (ExpertAI_StatStageLessThan(battleSys, battleCtx, AI_CONTEXT.defender, BATTLE_STAT_DEFENSE, BATTLE_STAT_BOOST_NEUTRAL))
                    {
                        if (AI_GetRandomNumber(battleSys) < 192)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, -2);
                        }
                    }
                    else
                    {
                        abilityTemp = AI_GetBattlerAbility(battleSys, battleCtx, AI_CONTEXT.defender);

                        if (Battle_AbilityDetersStatDrop(battleSys, battleCtx, abilityTemp, BATTLE_STAT_FLAG_DEFENSE) == FALSE)
                        {
                            if (AI_GetRandomNumber(battleSys) < 170)
                            {
                                AI_AddToMoveScore(battleSys, battleCtx, 3);
                            }
                        }
                    }
                    break;

                case BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER:
                    if (battleCtx->battleMons[AI_CONTEXT.attacker].moveEffectsData.destinyBondSuccessTurns > 0)
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -10);
                        break;
                    }
                    if (ExpertAI_AttackerKOsDefender(battleSys, battleCtx, AI_CONTEXT.defender, AI_CONTEXT.attacker))
                    {
                        if (AI_GetRandomNumber(battleSys) < 100)
                        {
                            break;
                        }

                        AI_AddToMoveScore(battleSys, battleCtx, 1);

                        if (AI_GetBattlerHPPercent(battleSys, battleCtx, AI_CONTEXT.attacker) > 50)
                        {
                            break;
                        }

                        if (AI_GetRandomNumber(battleSys) < 12)
                        {
                            break;
                        }

                        AI_AddToMoveScore(battleSys, battleCtx, 3);
                    }
                    else
                    {
                        AI_AddToMoveScore(battleSys, battleCtx, -1);

                        if (AI_GetBattlerHPPercent(battleSys, battleCtx, AI_CONTEXT.attacker) > 70)
                        {
                            break;
                        }

                        if (AI_GetRandomNumber(battleSys) < 32)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 1);
                        }

                        if (AI_GetBattlerHPPercent(battleSys, battleCtx, AI_CONTEXT.attacker) > 53)
                        {
                            break;
                        }

                        if (AI_GetRandomNumber(battleSys) < 64)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 1);
                        }

                        if (AI_GetBattlerHPPercent(battleSys, battleCtx, AI_CONTEXT.attacker) > 30)
                        {
                            break;
                        }

                        if (AI_GetRandomNumber(battleSys) < 128)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, 2);
                        }

                    }
                    break;
                }
            }
        }
    }

    return;
}

void ExpertAI_EvalMoreMoves_Doubles(BattleSystem* battleSys, BattleContext* battleCtx)
{
    u8 abilityTemp;
    u8 attackerSide, defenderSide;
    u8 attackerItemEffect, defenderItemEffect;
    int i;

    if ((AI_CONTEXT.thinkingMask & AI_FLAG_EXPERT) == FALSE)
    {
        return;
    }

    for (i = 0; i < LEARNED_MOVES_MAX; i++)
    {
        if (battleCtx->battleMons[AI_CONTEXT.attacker].moves[i])
        {
            if (ExpertAI_CanUseMove(battleSys, battleCtx, AI_CONTEXT.attacker, i, CHECK_INVALID_ALL))
            {
                AI_CONTEXT.moveSlot = i;
                AI_CONTEXT.move = battleCtx->battleMons[AI_CONTEXT.attacker].moves[AI_CONTEXT.moveSlot];
            }
            else
            {
                AI_CONTEXT.move = MOVE_NONE;
            }

            if (AI_CONTEXT.move != MOVE_NONE)
            {
                switch (MOVE_DATA(AI_CONTEXT.move).effect)
                {
                default:
                    break;


                case BATTLE_EFFECT_DEF_DOWN_2:
                    if (ExpertAI_StatStageLessThan(battleSys, battleCtx, AI_CONTEXT.defender, BATTLE_STAT_DEFENSE, BATTLE_STAT_BOOST_NEUTRAL))
                    {
                        if (AI_GetRandomNumber(battleSys) < 192)
                        {
                            AI_AddToMoveScore(battleSys, battleCtx, -2);
                        }
                    }
                    else
                    {
                        abilityTemp = AI_GetBattlerAbility(battleSys, battleCtx, AI_CONTEXT.defender);

                        if (Battle_AbilityDetersStatDrop(battleSys, battleCtx, abilityTemp, BATTLE_STAT_FLAG_DEFENSE) == FALSE)
                        {
                            if (AI_GetRandomNumber(battleSys) < 170)
                            {
                                AI_AddToMoveScore(battleSys, battleCtx, 3);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
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

        u32 damage = ExpertAI_CalcDamage(battleSys,
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

        ExpertAI_CalcAllDamage(battleSys,
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
        ExpertAI_MoveType(battleSys, battleCtx, AI_CONTEXT.attacker, AI_CONTEXT.move),
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

BOOL ExpertAI_AttackerKOsDefender(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, int defender)
{
    BOOL result;
    int k;
    int moveType, moveDamage, movePower;
    u8 side;
    u16 move;
    u32 effectiveness;

    result = FALSE;

    side = Battler_Side(battleSys, defender);

    for (k = 0; k < LEARNED_MOVES_MAX; k++) {
        effectiveness = 0;
        move = battleCtx->battleMons[attacker].moves[k];

        if (move == MOVE_NONE) {
            break;
        }

        if (ExpertAI_CanUseMove(battleSys, battleCtx, attacker, k, CHECK_INVALID_ALL_BUT_TORMENT)) {

            moveType = ExpertAI_MoveType(battleSys, battleCtx, attacker, move);
            movePower = MOVE_DATA(move).power;

            if (movePower > 0) {
                moveDamage = BattleSystem_CalcMoveDamage(battleSys,
                    battleCtx,
                    move,
                    battleCtx->sideConditionsMask[side],
                    battleCtx->fieldConditionsMask,
                    movePower,
                    moveType,
                    attacker,
                    defender,
                    1);

                moveDamage = BattleSystem_ApplyTypeChart(battleSys,
                    battleCtx,
                    move,
                    moveType,
                    attacker,
                    defender,
                    moveDamage,
                    &effectiveness);

                moveDamage *= DAMAGE_VARIANCE_MIN_ROLL;
                moveDamage /= 100;

                if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                    || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {
                    if (moveDamage >= battleCtx->battleMons[defender].curHP) {
                        result = TRUE;
                        break;
                    }
                }
            }
        }
    }

    return result;
}

BOOL AI_AttackerChunksOrKOsDefender(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, int defender)
{
    BOOL result;
    int k;
    int moveType, moveDamage, movePower;
    u8 side;
    u16 move;
    u32 effectiveness;

    result = FALSE;

    side = Battler_Side(battleSys, defender);

    for (k = 0; k < LEARNED_MOVES_MAX; k++) {
        effectiveness = 0;
        move = battleCtx->battleMons[attacker].moves[k];

        if (move == MOVE_NONE) {
            break;
        }

        if (ExpertAI_CanUseMove(battleSys, battleCtx, attacker, k, CHECK_INVALID_ALL_BUT_TORMENT)) {

            moveType = ExpertAI_MoveType(battleSys, battleCtx, attacker, move);
            movePower = MOVE_DATA(move).power;

            if (movePower > 0) {
                moveDamage = BattleSystem_CalcMoveDamage(battleSys,
                    battleCtx,
                    move,
                    battleCtx->sideConditionsMask[side],
                    battleCtx->fieldConditionsMask,
                    movePower,
                    moveType,
                    attacker,
                    defender,
                    1);

                moveDamage = BattleSystem_ApplyTypeChart(battleSys,
                    battleCtx,
                    move,
                    moveType,
                    attacker,
                    defender,
                    moveDamage,
                    &effectiveness);

                moveDamage *= DAMAGE_VARIANCE_MIN_ROLL;
                moveDamage /= 100;

                if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                    || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {
                    if (moveDamage >= battleCtx->battleMons[defender].curHP / 2) {
                        result = TRUE;
                        break;
                    }
                }
            }
        }
    }

    return result;
}

s32 ExpertAI_CalcAllDamage(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, u16* moves, s32* damageVals, u16 heldItem, u8* ivs, int ability, int embargoTurns, int varyDamage)
{
    int i, riskyScanIdx, altPowerScanIdx;
    s32 maxDamage;
    u8 damageRoll;

    maxDamage = 0;

    // Step 1: Compute the true damage of a given move.
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        riskyScanIdx = 0;
        while (sRiskyMoves[riskyScanIdx] != 0xFFFF) {
            if (MOVE_DATA(moves[i]).effect == sRiskyMoves[riskyScanIdx]) {
                break;
            }

            riskyScanIdx++;
        }

        altPowerScanIdx = 0;
        while (sAltPowerCalcMoves[altPowerScanIdx] != 0xFFFF) {
            if (MOVE_DATA(moves[i]).effect == sAltPowerCalcMoves[altPowerScanIdx]) {
                break;
            }

            altPowerScanIdx++;
        }

        if (sAltPowerCalcMoves[altPowerScanIdx] != 0xFFFF
            || (moves[i] != MOVE_NONE && sRiskyMoves[riskyScanIdx] == 0xFFFF && MOVE_DATA(moves[i]).power > 1)) {
            if (varyDamage == ROLL_FOR_DAMAGE) {
                damageRoll = AI_CONTEXT.moveDamageRolls[i];
            }
            else if (varyDamage == USE_MIN_DAMAGE) {
                damageRoll = DAMAGE_VARIANCE_MIN_ROLL;
            }
            else {
                damageRoll = DAMAGE_VARIANCE_MAX_ROLL;
            }

            damageVals[i] = ExpertAI_CalcDamage(battleSys, battleCtx, moves[i], heldItem, ivs, attacker, ability, embargoTurns, damageRoll);
        }
        else {
            damageVals[i] = 0;
        }
    }

    // Step 2: Determine the maximum-damage of all moves.
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (maxDamage < damageVals[i]) {
            maxDamage = damageVals[i];
        }
    }

    return maxDamage;
}

s32 ExpertAI_CalcDamage(BattleSystem* battleSys, BattleContext* battleCtx, u16 move, u16 heldItem, u8* ivs, int attacker, int ability, int embargoTurns, u8 variance)
{
    // must declare C89-style to match
    int defendingSide;
    int power;
    int type;
    int typeTmp;
    int multiHitChance;
    int multiHitHits;
    u32 effectivenessFlags;
    s32 damage;

    defendingSide = Battler_Side(battleSys, AI_CONTEXT.defender);
    damage = 0;
    power = 0;
    type = 0;
    effectivenessFlags = 0;
    multiHitHits = 1;

    switch (move) {
    case MOVE_NATURAL_GIFT:
        if (embargoTurns == 0) {
            power = BattleSystem_GetItemData(battleCtx, heldItem, ITEM_PARAM_NATURAL_GIFT_POWER);

            if (power) {
                type = BattleSystem_GetItemData(battleCtx, heldItem, ITEM_PARAM_NATURAL_GIFT_TYPE);
            }
            else {
                type = TYPE_NORMAL;
            }
        }
        break;

    case MOVE_JUDGMENT:
        if (embargoTurns == 0) {
            power = 0;

            switch (BattleSystem_GetItemData(battleCtx, heldItem, ITEM_PARAM_HOLD_EFFECT)) {
            case HOLD_EFFECT_ARCEUS_FIGHTING:
                type = TYPE_FIGHTING;
                break;

            case HOLD_EFFECT_ARCEUS_FLYING:
                type = TYPE_FLYING;
                break;

            case HOLD_EFFECT_ARCEUS_POISON:
                type = TYPE_POISON;
                break;

            case HOLD_EFFECT_ARCEUS_GROUND:
                type = TYPE_GROUND;
                break;

            case HOLD_EFFECT_ARCEUS_ROCK:
                type = TYPE_ROCK;
                break;

            case HOLD_EFFECT_ARCEUS_BUG:
                type = TYPE_BUG;
                break;

            case HOLD_EFFECT_ARCEUS_GHOST:
                type = TYPE_GHOST;
                break;

            case HOLD_EFFECT_ARCEUS_STEEL:
                type = TYPE_STEEL;
                break;

            case HOLD_EFFECT_ARCEUS_FIRE:
                type = TYPE_FIRE;
                break;

            case HOLD_EFFECT_ARCEUS_WATER:
                type = TYPE_WATER;
                break;

            case HOLD_EFFECT_ARCEUS_GRASS:
                type = TYPE_GRASS;
                break;

            case HOLD_EFFECT_ARCEUS_ELECTRIC:
                type = TYPE_ELECTRIC;
                break;

            case HOLD_EFFECT_ARCEUS_PSYCHIC:
                type = TYPE_PSYCHIC;
                break;

            case HOLD_EFFECT_ARCEUS_ICE:
                type = TYPE_ICE;
                break;

            case HOLD_EFFECT_ARCEUS_DRAGON:
                type = TYPE_DRAGON;
                break;

            case HOLD_EFFECT_ARCEUS_DARK:
                type = TYPE_DARK;
                break;

            default:
                type = TYPE_NORMAL;
                break;
            }
        }
        break;

    case MOVE_FLING:
        if (embargoTurns == 0) {
            power = Battler_ItemFlingPower(battleCtx, attacker);
            type = Battler_FlingType(battleCtx, attacker);
        }
        break;


    case MOVE_HIDDEN_POWER:
        /*
            power = ((ivs[STAT_HP] & 2) >> 1)
                | ((ivs[STAT_ATTACK] & 2) >> 0)
                | ((ivs[STAT_DEFENSE] & 2) << 1)
                | ((ivs[STAT_SPEED] & 2) << 2)
                | ((ivs[STAT_SPECIAL_ATTACK] & 2) << 3)
                | ((ivs[STAT_SPECIAL_DEFENSE] & 2) << 4);
        */
        type = ((ivs[STAT_HP] & 1) >> 0)
            | ((ivs[STAT_ATTACK] & 1) << 1)
            | ((ivs[STAT_DEFENSE] & 1) << 2)
            | ((ivs[STAT_SPEED] & 1) << 3)
            | ((ivs[STAT_SPECIAL_ATTACK] & 1) << 4)
            | ((ivs[STAT_SPECIAL_DEFENSE] & 1) << 5);

        // power = power * 40 / 63 + 30;
        type = (type * 15 / 63) + 1;

        if (type >= TYPE_MYSTERY) {
            type++;
        }
        break;

    case MOVE_GYRO_BALL:
        power = 1 + 25 * battleCtx->monSpeedValues[AI_CONTEXT.defender] / battleCtx->monSpeedValues[attacker];

        if (power > 150) {
            power = 150;
        }

        type = TYPE_NORMAL; // default to the base move type
        break;

    case MOVE_DRAGON_RAGE:
        damage = 40;
        break;

    case MOVE_SEISMIC_TOSS:
    case MOVE_NIGHT_SHADE:
        damage = battleCtx->battleMons[attacker].level;
        break;

    case MOVE_RETURN:
        power = battleCtx->battleMons[attacker].friendship * 10 / 25;
        type = TYPE_NORMAL;
        break;

    case MOVE_FRUSTRATION:
        power = (255 - battleCtx->battleMons[attacker].friendship) * 10 / 25;
        type = TYPE_NORMAL;
        break;

        /*
        case MOVE_MAGNITUDE:
            // Simulate a Magnitude roll
            power = BattleSystem_RandNext(battleSys) % 100;

            if (power < 5) {
                power = 10;
            } else if (power < 15) {
                power = 30;
            } else if (power < 35) {
                power = 50;
            } else if (power < 65) {
                power = 70;
            } else if (power < 85) {
                power = 90;
            } else if (power < 95) {
                power = 110;
            } else {
                power = 150;
            }

            type = TYPE_NORMAL;
            break;
        */

    case MOVE_SONIC_BOOM:
        damage = 20;
        break;

    case MOVE_LOW_KICK:
    case MOVE_GRASS_KNOT: {
        int i;

        int monWeight = battleCtx->battleMons[AI_CONTEXT.defender].weight;

        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY)
        {
            monWeight *= 2;
        }

        for (i = 0; sWeightToPower[i][0] != 0xFFFF; i++) {
            if (sWeightToPower[i][0] >= monWeight) {
                break;
            }
        }

        if (sWeightToPower[i][0] != 0xFFFF) {
            power = sWeightToPower[i][1];
        }
        else {
            power = 150;
        }

        break;
    }

    default:
        // Move has no special calculation logic; default to the basic calc
        power = 0;
        type = TYPE_NORMAL;
        break;
    }

    if (damage == 0) {
        damage = BattleSystem_CalcMoveDamage(battleSys,
            battleCtx,
            move,
            battleCtx->sideConditionsMask[defendingSide],
            battleCtx->fieldConditionsMask,
            power,
            type,
            attacker,
            AI_CONTEXT.defender,
            1);
    }
    else {
        battleCtx->battleStatusMask |= SYSCTL_IGNORE_TYPE_CHECKS;
    }

    damage = BattleSystem_ApplyTypeChart(battleSys,
        battleCtx,
        move,
        type,
        attacker,
        AI_CONTEXT.defender,
        damage,
        &effectivenessFlags);
    battleCtx->battleStatusMask &= ~SYSCTL_IGNORE_TYPE_CHECKS;

    if (BattleAI_IsMultiHitMove(battleSys, battleCtx, attacker, MOVE_DATA(move).effect))
    {
        multiHitHits = BattleSystem_GetMultiHitExpectedMoveHits(battleSys, battleCtx, attacker, AI_CONTEXT.defender, move);
    }

    if (effectivenessFlags & MOVE_STATUS_IMMUNE) {
        damage = 0;
    }
    else {
        damage = BattleSystem_Divide(damage * variance, 100);
    }

    if (multiHitHits > 1
        && damage)
    {
        if (Battle_MapResistBerryEffectToType(Battler_HeldItemEffect(battleCtx, AI_CONTEXT.defender)) == type)
        {
            if (effectivenessFlags & MOVE_STATUS_SUPER_EFFECTIVE)
            {
                damage += damage * 2 * (multiHitHits - 1);
            }
            else
            {
                damage *= multiHitHits;
            }
        }
        else
        {
            damage *= multiHitHits;
        }
    }

    return damage;
}

int ExpertAI_MoveType(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int move)
{
    int result;

    switch (move) {
    case MOVE_NATURAL_GIFT:
        result = Battler_NaturalGiftType(battleCtx, battler);
        break;

    case MOVE_FLING:
        result = Battler_FlingType(battleCtx, battler);
        break;

    case MOVE_JUDGMENT:
        switch (Battler_HeldItemEffect(battleCtx, battler)) {
        case HOLD_EFFECT_ARCEUS_FIGHTING:
            result = TYPE_FIGHTING;
            break;

        case HOLD_EFFECT_ARCEUS_FLYING:
            result = TYPE_FLYING;
            break;

        case HOLD_EFFECT_ARCEUS_POISON:
            result = TYPE_POISON;
            break;

        case HOLD_EFFECT_ARCEUS_GROUND:
            result = TYPE_GROUND;
            break;

        case HOLD_EFFECT_ARCEUS_ROCK:
            result = TYPE_ROCK;
            break;

        case HOLD_EFFECT_ARCEUS_BUG:
            result = TYPE_BUG;
            break;

        case HOLD_EFFECT_ARCEUS_GHOST:
            result = TYPE_GHOST;
            break;

        case HOLD_EFFECT_ARCEUS_STEEL:
            result = TYPE_STEEL;
            break;

        case HOLD_EFFECT_ARCEUS_FIRE:
            result = TYPE_FIRE;
            break;

        case HOLD_EFFECT_ARCEUS_WATER:
            result = TYPE_WATER;
            break;

        case HOLD_EFFECT_ARCEUS_GRASS:
            result = TYPE_GRASS;
            break;

        case HOLD_EFFECT_ARCEUS_ELECTRIC:
            result = TYPE_ELECTRIC;
            break;

        case HOLD_EFFECT_ARCEUS_PSYCHIC:
            result = TYPE_PSYCHIC;
            break;

        case HOLD_EFFECT_ARCEUS_ICE:
            result = TYPE_ICE;
            break;

        case HOLD_EFFECT_ARCEUS_DRAGON:
            result = TYPE_DRAGON;
            break;

        case HOLD_EFFECT_ARCEUS_DARK:
            result = TYPE_DARK;
            break;

        default:
            result = MOVE_DATA(move).type;
            break;
        }
        break;

    case MOVE_HIDDEN_POWER:
        result = ((battleCtx->battleMons[battler].hpIV & 1) >> 0)
            | ((battleCtx->battleMons[battler].attackIV & 1) << 1)
            | ((battleCtx->battleMons[battler].defenseIV & 1) << 2)
            | ((battleCtx->battleMons[battler].speedIV & 1) << 3)
            | ((battleCtx->battleMons[battler].spAttackIV & 1) << 4)
            | ((battleCtx->battleMons[battler].spDefenseIV & 1) << 5);
        result = (result * 15 / 63) + 1;

        if (result >= TYPE_MYSTERY) {
            result++;
        }
        break;

    case MOVE_WEATHER_BALL:
        if (NO_CLOUD_NINE && (battleCtx->fieldConditionsMask & FIELD_CONDITION_WEATHER)) {
            if (WEATHER_IS_RAIN) {
                result = TYPE_WATER;
            }

            if (WEATHER_IS_SAND) {
                result = TYPE_ROCK;
            }

            if (WEATHER_IS_SUN) {
                result = TYPE_FIRE;
            }

            if (WEATHER_IS_HAIL) {
                result = TYPE_ICE;
            }
        }
        break;

    default:
        result = TYPE_NORMAL;
        break;
    }

    return result;
}

BOOL ExpertAI_CanUseMove(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int moveSlot, int opMask)
{
    BOOL result = TRUE;

    if (ExpertAI_CheckInvalidMoves(battleSys, battleCtx, battler, 0, opMask) & FlagIndex(moveSlot)) {
        result = FALSE;
    }

    return result;
}

int ExpertAI_CheckInvalidMoves(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int invalidMoves, int opMask)
{
    int itemEffect = Battler_HeldItemEffect(battleCtx, battler);

    for (int i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (battleCtx->battleMons[battler].moves[i] == MOVE_NONE
                && (opMask & CHECK_INVALID_NO_MOVE)) {
            invalidMoves |= FlagIndex(i);
        }

        if (battleCtx->battleMons[battler].ppCur[i] == 0
                && (opMask & CHECK_INVALID_NO_PP)) {
            invalidMoves |= FlagIndex(i);
        }

        if (battleCtx->battleMons[battler].moves[i] == battleCtx->battleMons[battler].moveEffectsData.disabledMove
                && (opMask & CHECK_INVALID_DISABLED)) {
            invalidMoves |= FlagIndex(i);
        }

        if (battleCtx->battleMons[battler].moves[i] == battleCtx->movePrevByBattler[battler]
                && (opMask & CHECK_INVALID_TORMENTED)
                && (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_TORMENT)) {
            invalidMoves |= FlagIndex(i);
        }

        if (battleCtx->battleMons[battler].moveEffectsData.tauntedTurns
                && (opMask & CHECK_INVALID_TAUNTED)
                && MOVE_DATA(battleCtx->battleMons[battler].moves[i]).power == 0) {
            invalidMoves |= FlagIndex(i);
        }
		
		if (itemEffect == HOLD_EFFECT_RAISE_SPD_NO_STATUS
		&& (opMask & CHECK_INVALID_ASS_VEST)
        && MOVE_DATA(battleCtx->battleMons[battler].moves[i]).power == 0) {
            invalidMoves |= FlagIndex(i);
        }

        if (Move_Imprisoned(battleSys, battleCtx, battler, battleCtx->battleMons[battler].moves[i])
                && (opMask & CHECK_INVALID_IMPRISONED)) {
            invalidMoves |= FlagIndex(i);
        }

        if (Move_FailsInHighGravity(battleSys, battleCtx, battler, battleCtx->battleMons[battler].moves[i])
                && (opMask & CHECK_INVALID_GRAVITY)) {
            invalidMoves |= FlagIndex(i);
        }

        if (Move_HealBlocked(battleSys, battleCtx, battler, battleCtx->battleMons[battler].moves[i])
                && (opMask & CHECK_INVALID_HEAL_BLOCK)) {
            invalidMoves |= FlagIndex(i);
        }

        if (battleCtx->battleMons[battler].moveEffectsData.encoredMove
           && battleCtx->battleMons[battler].moveEffectsData.encoredMove != battleCtx->battleMons[battler].moves[i]
           && (opMask & CHECK_INVALID_ENCORE)) {
            invalidMoves |= FlagIndex(i);
        }

        if ((itemEffect == HOLD_EFFECT_CHOICE_ATK || itemEffect == HOLD_EFFECT_CHOICE_SPEED || itemEffect == HOLD_EFFECT_CHOICE_SPATK)
                && (opMask & CHECK_INVALID_CHOICE_ITEM)) {
            if (battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove
                    && battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove != battleCtx->battleMons[battler].moves[i]) {
                invalidMoves |= FlagIndex(i);
            }
        }
    }

    return invalidMoves;
}

BOOL AICmd_IfCurrentMoveRevealedOverflow(BattleSystem* battleSys, BattleContext* battleCtx)
{
    if (MOVE_DATA(AI_CONTEXT.move).pp > battleCtx->battleMons[AI_CONTEXT.attacker].ppCur[AI_CONTEXT.moveSlot])
    {
        return TRUE;
    }

    return FALSE;
}

BOOL AI_IfMoveEffectKnown(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int moveEffect)
{
    int i;
    BOOL result;

    result = FALSE;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (AI_CONTEXT.battlerMoves[battler][i]
            && MOVE_DATA(AI_CONTEXT.battlerMoves[battler][i]).effect == moveEffect) {
            break;
        }
    }

    if (i < LEARNED_MOVES_MAX) {
        result = TRUE;
    }

    return result;
}

BOOL BattleAI_IsMoveBlockedByWebMaster(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, int defender, u16 move)
{
    int i;
    BOOL result;

    result = FALSE;

    // Early exit if defender's ability is not Web Master
    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_WEB_MASTER) == FALSE)
    {
        return result;
    }

    for (i = 0; i < NELEMS(sWebMoves); i++)
    {
        if (sWebMoves[i] == move)
        {
            result = TRUE;
            break;
        }
    }

    return result;
}

BOOL ExpertAI_IsBattlerSpecialAttacker(BattleSystem* battleSys, BattleContext* battleCtx, int battler)
{
    BOOL result;

    result = FALSE;

    if (Battle_BattleMonIsSpecialAttacker(battleSys, battleCtx, battler))
    {
        result = TRUE;
    }

    return result;
}

BOOL ExpertAI_IsBattlerPhysicalAttacker(BattleSystem* battleSys, BattleContext* battleCtx, int battler)
{
    BOOL result;

    result = FALSE;

    if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, battler))
    {
        result = TRUE;
    }

    return result;
}

BOOL ExpertAI_StatStageLessThan(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int stat, int val)
{
    int statBoost;
    BOOL result;

    result = FALSE;

    statBoost = battleCtx->battleMons[battler].statBoosts[stat];

    if (Battler_Ability(battleCtx, battler) == ABILITY_SIMPLE)
    {
        if (statBoost <= BATTLE_STAT_BOOST_NEUTRAL)
        {
            if ((BATTLE_STAT_BOOST_NEUTRAL - statBoost) * 2 < BATTLE_STAT_BOOST_NUM_STAGES)
            {
                statBoost = BATTLE_STAT_BOOST_NEUTRAL - ((BATTLE_STAT_BOOST_NEUTRAL - statBoost) * 2);
            }
            else
            {
                statBoost = BATTLE_STAT_BOOST_MIN;
            }
        }
        else
        {
            if ((statBoost - BATTLE_STAT_BOOST_NEUTRAL) * 2 < BATTLE_STAT_BOOST_NUM_STAGES)
            {
                statBoost = BATTLE_STAT_BOOST_NEUTRAL + ((statBoost - BATTLE_STAT_BOOST_NEUTRAL) * 2);
            }
            else
            {
                statBoost = BATTLE_STAT_BOOST_MAX;
            }
        }
    }

    if (statBoost < val) {
        result = TRUE;
    }

    return result;
}

BOOL ExpertAI_IsStatDropped(BattleSystem* battleSys, BattleContext* battleCtx, int battler)
{
    BOOL result;
    BOOL hasHaze;
    u8 ability;
    u16 move;
    int i, j;
    int statStage, battleStatFlag, moveEffect;

    result = FALSE;

    hasHaze = FALSE;

    ability = Battler_Ability(battleCtx, battler);

    battleStatFlag = BATTLE_STAT_FLAG_NONE;

    if (ability == ABILITY_UNAWARE
        || BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS, battler, ABILITY_MEMORY))
    {
        return result;
    }

    for (j = 0; j < LEARNED_MOVES_MAX; j++)
    {
        if (ExpertAI_CanUseMove(battleSys, battleCtx, battler, j, CHECK_INVALID_ALL_BUT_TORMENT))
        {
            move = battleCtx->battleMons[battler].moves[j];
            moveEffect = MOVE_DATA(move).effect;

            switch (moveEffect)
            {
            default:
                break;

            case BATTLE_EFFECT_RESET_STAT_CHANGES:
            case BATTLE_EFFECT_COPY_STAT_CHANGES:
            case BATTLE_EFFECT_SWAP_ATK_SP_ATK_STAT_CHANGES:
            case BATTLE_EFFECT_SWAP_STAT_CHANGES:
                hasHaze = TRUE;
                break;
            }
        }
    }

    for (i = BATTLE_STAT_ATTACK; i < BATTLE_STAT_MAX; i++)
    {
        if (i != BATTLE_STAT_SPEED)
        {
            statStage = battleCtx->battleMons[battler].statBoosts[i];

            if (statStage < BATTLE_STAT_BOOST_NEUTRAL)
            {
                if (i == BATTLE_STAT_ATTACK)
                {
                    if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, battler))
                    {
                        if (hasHaze == FALSE)
                        {
                            result = TRUE;
                            break;
                        }
                    }
                }

                if (i == BATTLE_STAT_SP_ATTACK)
                {
                    if (Battle_BattleMonIsSpecialAttacker(battleSys, battleCtx, battler))
                    {
                        if (hasHaze == FALSE)
                        {
                            result = TRUE;
                            break;
                        }
                    }
                }

                if (i == BATTLE_STAT_DEFENSE)
                {
                    if ((statStage < BATTLE_STAT_BOOST_NEUTRAL - 1)
                        && hasHaze == FALSE)
                    {
                        result = TRUE;
                        break;
                    }
                }

                if (i == BATTLE_STAT_SP_DEFENSE)
                {
                    if ((statStage < BATTLE_STAT_BOOST_NEUTRAL - 1)
                        && hasHaze == FALSE)
                    {
                        result = TRUE;
                        break;
                    }
                }
            }
        }
    }

    return result;
}

BOOL ExpertAI_MoveEffectKnownByBattler(BattleSystem* battleSys, BattleContext* battleCtx, int battler, u16 moveEffect)
{
    BOOL result;
    int i;

    result = FALSE;

    for (i = 0; i < LEARNED_MOVES_MAX; i++)
    {
        if (AI_CONTEXT.battlerMoves[battler][i] == MOVE_NONE)
        {
            break;
        }
        
        if (moveEffect == MOVE_DATA(AI_CONTEXT.battlerMoves[battler][i]).effect)
        {
            result = TRUE;
            break;
        }
    }

    return result;
}

BOOL ExpertAI_AttackerCanStatusDefender(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, int defender)
{
    BOOL result;
    int i;
    u8 defenderAbility, defenderSide;
    u16 move, moveEffect;
    u32 moveStatusCondition, moveVolatileStatus;
    u32 effectiveness;

    result = FALSE;

    // early exit for magic bounce
    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_MAGIC_BOUNCE)
        && Battler_Ability(battleCtx, attacker) != ABILITY_MAGIC_BOUNCE)
    {
        return result;
    }

    defenderAbility = Battler_Ability(battleCtx, defender);
    defenderSide = Battler_Side(battleSys, defender);
    moveStatusCondition = MON_CONDITION_NONE;
    moveVolatileStatus = VOLATILE_CONDITION_NONE;
    effectiveness = 0;

    if (battleCtx->sideConditionsMask[defenderSide] & SIDE_CONDITION_SAFEGUARD)
    {
        return result;
    }

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {

        if ((BattleSystem_CheckInvalidMoves(battleSys, battleCtx, attacker, 0, CHECK_INVALID_ALL) & FlagIndex(i)) == FALSE)
        {
            move = battleCtx->aiContext.battlerMoves[attacker][i];

            if (move == MOVE_NONE) {
                break;
            }

            if (MOVE_DATA(move).class == CLASS_STATUS
                || MOVE_DATA(move).effectChance > 20)
            {
                moveEffect = MOVE_DATA(move).effect;
                moveStatusCondition = MapBattleEffectToStatusCondition(battleCtx, moveEffect);
                moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, moveEffect);

                BattleSystem_ApplyTypeChart(battleSys,
                    battleCtx,
                    move,
                    ExpertAI_MoveType(battleSys, battleCtx, attacker, move),
                    attacker,
                    defender,
                    TYPE_MULTI_BASE_DAMAGE,
                    &effectiveness);

                if ((effectiveness & MOVE_STATUS_IMMUNE) == FALSE
                    || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY))
                {
                    if (moveStatusCondition & MON_CONDITION_ANY)
                    {
                        if (BattleAI_BattlerTypeDetersStatusMove(battleSys, battleCtx, defender, move, moveStatusCondition) == FALSE)
                        {
                            if (Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, moveStatusCondition) == FALSE)
                            {
                                result = TRUE;
                                break;
                            }
                        }
                    }

                    if (moveVolatileStatus & VOLATILE_CONDITION_INFLICTABLE_NEGATIVE)
                    {
                        if (Battle_AbilityDetersVolatileStatus(battleSys, battleCtx, defenderAbility, moveVolatileStatus) == FALSE)
                        {
                            result = TRUE;
                            break;
                        }
                    }
                }
            }
        }
    }

    return result;
}