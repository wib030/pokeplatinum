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

typedef void (*AICommandFunc)(BattleSystem*, BattleContext*);

enum AIEvalStep {
    AI_EVAL_STEP_INIT,
    AI_EVAL_STEP_EVAL,
    AI_EVAL_STEP_END,
};

static void AICmd_IfRandomLessThan(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfRandomGreaterThan(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfRandomEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfRandomNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_AddToMoveScore(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfHPPercentLessThan(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfHPPercentGreaterThan(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfHPPercentEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfHPPercentNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfStatus(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfNotStatus(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfVolatileStatus(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfNotVolatileStatus(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfMoveEffect(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfNotMoveEffect(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfSideCondition(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfNotSideCondition(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLoadedLessThan(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLoadedGreaterThan(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLoadedEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLoadedNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLoadedMask(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLoadedNotMask(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfMoveEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfMoveNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLoadedInTable(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLoadedNotInTable(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfAttackerHasDamagingMoves(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfAttackerHasNoDamagingMoves(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadTurnCount(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadTypeFrom(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadMovePower(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_FlagMoveDamageScore(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadBattlerPreviousMove(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfTempEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfTempNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfSpeedCompareEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfSpeedCompareNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_CountAlivePartyBattlers(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadCurrentMove(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadCurrentMoveEffect(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadBattlerAbility(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_CalcMaxEffectiveness(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfMoveEffectivenessEquals(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfPartyMemberStatus(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfPartyMemberNotStatus(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadCurrentWeather(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfCurrentMoveEffectEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfCurrentMoveEffectNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfStatStageLessThan(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfStatStageGreaterThan(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfStatStageEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfStatStageNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfCurrentMoveKills(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfCurrentMoveDoesNotKill(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfMoveKnown(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfMoveNotKnown(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfMoveEffectKnown(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfMoveEffectNotKnown(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerUnderEffect(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfCurrentMoveMatchesEffect(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_Escape(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_Dummy3E(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_Dummy3F(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadHeldItem(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadHeldItemEffect(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadGender(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadIsFirstTurnInBattle(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadStockpileCount(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadBattleType(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadRecycleItem(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadTypeOfLoadedMove(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadPowerOfLoadedMove(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadEffectOfLoadedMove(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadProtectChain(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_PushAndGoTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_GoTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_PopOrEnd(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfLevel(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfTargetIsTaunted(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfTargetIsNotTaunted(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfTargetIsPartner(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_FlagBattlerIsType(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_CheckBattlerAbility(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfActivatedFlashFire(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfHeldItemEqualTo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfFieldConditionsMask(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadSpikesLayers(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfAnyPartyMemberIsWounded(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfAnyPartyMemberUsedPP(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadFlingPower(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadCurrentMovePP(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfCanUseLastResort(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadCurrentMoveClass(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadDefenderLastUsedMoveClass(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadBattlerSpeedRank(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadBattlerTurnCount(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfPartyMemberDealsMoreDamage(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfHasSuperEffectiveMove(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerDealsMoreDamage(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_SumPositiveStatStages(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_DiffStatStages(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasHigherStat(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasLowerStat(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasEqualStat(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_CheckIfHighestDamageWithPartner(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerFainted(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerNotFainted(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadAbility(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasPhysicalAttack(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasSpecialAttack(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasStatusAttack(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasNoPhysicalAttack(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasNoSpecialAttack(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfBattlerHasNoStatusAttack(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfToxicSpikesClearerAliveInParty(BattleSystem *battleSys, BattleContext  *battleCtx);
static void AICmd_LoadWeight(BattleSystem *battleSys, BattleContext  *battleCtx);
static void AICmd_IfWishActive(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfPartyMemberHasBattleEffect(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfShouldTaunt(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_LoadMoveAccuracy(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfSameAbilities(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfHasBaseAbility(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfDestinyBondFails(BattleSystem *battleSys, BattleContext *battleCtx);
static void AICmd_IfEnemyCanChunkOrKO(BattleSystem* battleSys, BattleContext* battleCtx);
static void AICmd_LoadBattlerCritStage(BattleSystem* battleSys, BattleContext* battleCtx);
static void AICmd_IfCanHazeOrPhaze(BattleSystem* battleSys, BattleContext* battleCtx);

static u8 TrainerAI_MainSingles(BattleSystem *battleSys, BattleContext *battleCtx);
static u8 TrainerAI_MainDoubles(BattleSystem *battleSys, BattleContext *battleCtx);
static void TrainerAI_EvalMoves(BattleSystem *battleSys, BattleContext *battleCtx);
static void TrainerAI_RecordLastMove(BattleSystem *battleSys, BattleContext *battleCtx);
static void TrainerAI_RecordRandomMove(BattleSystem *battleSys, BattleContext *battleCtx);
static void TrainerAI_RevealAllInfo(BattleSystem *battleSys, BattleContext *battleCtx);
static void TrainerAI_RevealBasicInfo(BattleSystem *battleSys, BattleContext *battleCtx);
static void AIScript_PushCursor(BattleSystem *battleSys, BattleContext *battleCtx, int address);
static BOOL AIScript_PopCursor(BattleSystem *battleSys, BattleContext *battleCtx);
static int AIScript_Read(BattleContext *battleCtx);
static int AIScript_ReadOffset(BattleContext *battleCtx, int ofs);
static void AIScript_Iter(BattleContext *battleCtx, int i);
static u8 AIScript_Battler(BattleContext *battleCtx, u8 inBattler);
static s32 TrainerAI_CalcAllDamage(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, u16 *moves, s32 *damageVals, u16 heldItem, u8 *ivs, int ability, BOOL embargo, BOOL varyDamage);
static s32 TrainerAI_CalcDamage(BattleSystem *battleSys, BattleContext *battleCtx, u16 move, u16 heldItem, u8 *ivs, int attacker, int ability, BOOL embargo, u8 variance);
static int TrainerAI_MoveType(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int move);
static int TrainerAI_CalcEndOfTurnHealTick(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static int TrainerAI_CalcEndOfTurnDamageTick(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static void TrainerAI_GetStats(BattleContext *battleCtx, int battler, int *buf1, int *buf2, int stat);
static BOOL AI_CanCurePartyMemberStatus(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_CanImprisonTarget(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender);
static BOOL AI_CanMagicBounceTargetMove(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender);
static BOOL AI_DoNotStatDrop(BattleSystem *battleSys, BattleContext *battleCtx, u16 move, int attacker, int defender);

static BOOL AI_PerishSongKO(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static int AI_CheckInvalidMoves(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int invalidMoves, int opMask);
static BOOL AI_CanUseMove(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int moveSlot, int opMask);
static BOOL AI_AttackerChunksOrKOsDefender(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender);
static BOOL AI_DoNotStatDrop(BattleSystem *battleSys, BattleContext *battleCtx, u16 move, int attacker, int defender);
static BOOL AI_CannotDamageWonderGuard(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_OnlyIneffectiveMoves(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_ShouldSwitchYawn(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_ShouldSwitchToxic(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_HasSuperEffectiveMove(BattleSystem *battleSys, BattleContext *battleCtx, int battler, BOOL alwaysSwitch);
static BOOL AI_HasAbsorbAbilityInParty(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_HasPartyMemberWithSuperEffectiveMove(BattleSystem *battleSys, BattleContext *battleCtx, int battler, u32 checkEffectiveness, u8 rand);
static BOOL AI_TargetHasRelevantContactAbility(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_IsAsleepWithNaturalCure(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_IsHeavilyStatBoosted(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_IsModeratelyBoosted(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_ShouldSwitchWeatherDependent(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL AI_ShouldSwitchWeatherSetter(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL TrainerAI_ShouldSwitch(BattleSystem *battleSys, BattleContext *battleCtx, int battler);
static BOOL TrainerAI_ShouldUseItem(BattleSystem *battleSys, int battler);

static const AICommandFunc sAICommandTable[] = {
    AICmd_IfRandomLessThan,
    AICmd_IfRandomGreaterThan,
    AICmd_IfRandomEqualTo,
    AICmd_IfRandomNotEqualTo,
    AICmd_AddToMoveScore,
    AICmd_IfHPPercentLessThan,
    AICmd_IfHPPercentGreaterThan,
    AICmd_IfHPPercentEqualTo,
    AICmd_IfHPPercentNotEqualTo,
    AICmd_IfStatus,
    AICmd_IfNotStatus,
    AICmd_IfVolatileStatus,
    AICmd_IfNotVolatileStatus,
    AICmd_IfMoveEffect,
    AICmd_IfNotMoveEffect,
    AICmd_IfSideCondition,
    AICmd_IfNotSideCondition,
    AICmd_IfLoadedLessThan,
    AICmd_IfLoadedGreaterThan,
    AICmd_IfLoadedEqualTo,
    AICmd_IfLoadedNotEqualTo,
    AICmd_IfLoadedMask,
    AICmd_IfLoadedNotMask,
    AICmd_IfMoveEqualTo,
    AICmd_IfMoveNotEqualTo,
    AICmd_IfLoadedInTable,
    AICmd_IfLoadedNotInTable,
    AICmd_IfAttackerHasDamagingMoves,
    AICmd_IfAttackerHasNoDamagingMoves,
    AICmd_LoadTurnCount,
    AICmd_LoadTypeFrom,
    AICmd_LoadMovePower,
    AICmd_FlagMoveDamageScore,
    AICmd_LoadBattlerPreviousMove,
    AICmd_IfTempEqualTo,
    AICmd_IfTempNotEqualTo,
    AICmd_IfSpeedCompareEqualTo,
    AICmd_IfSpeedCompareNotEqualTo,
    AICmd_CountAlivePartyBattlers,
    AICmd_LoadCurrentMove,
    AICmd_LoadCurrentMoveEffect,
    AICmd_LoadBattlerAbility,
    AICmd_CalcMaxEffectiveness,
    AICmd_IfMoveEffectivenessEquals,
    AICmd_IfPartyMemberStatus,
    AICmd_IfPartyMemberNotStatus,
    AICmd_LoadCurrentWeather,
    AICmd_IfCurrentMoveEffectEqualTo,
    AICmd_IfCurrentMoveEffectNotEqualTo,
    AICmd_IfStatStageLessThan,
    AICmd_IfStatStageGreaterThan,
    AICmd_IfStatStageEqualTo,
    AICmd_IfStatStageNotEqualTo,
    AICmd_IfCurrentMoveKills,
    AICmd_IfCurrentMoveDoesNotKill,
    AICmd_IfMoveKnown,
    AICmd_IfMoveNotKnown,
    AICmd_IfMoveEffectKnown,
    AICmd_IfMoveEffectNotKnown,
    AICmd_IfBattlerUnderEffect,
    AICmd_IfCurrentMoveMatchesEffect,
    AICmd_Escape,
    AICmd_Dummy3E,
    AICmd_Dummy3F,
    AICmd_LoadHeldItem,
    AICmd_LoadHeldItemEffect,
    AICmd_LoadGender,
    AICmd_LoadIsFirstTurnInBattle,
    AICmd_LoadStockpileCount,
    AICmd_LoadBattleType,
    AICmd_LoadRecycleItem,
    AICmd_LoadTypeOfLoadedMove,
    AICmd_LoadPowerOfLoadedMove,
    AICmd_LoadEffectOfLoadedMove,
    AICmd_LoadProtectChain,
    AICmd_PushAndGoTo,
    AICmd_GoTo,
    AICmd_PopOrEnd,
    AICmd_IfLevel,
    AICmd_IfTargetIsTaunted,
    AICmd_IfTargetIsNotTaunted,
    AICmd_IfTargetIsPartner,
    AICmd_FlagBattlerIsType,
    AICmd_CheckBattlerAbility,
    AICmd_IfActivatedFlashFire,
    AICmd_IfHeldItemEqualTo,
    AICmd_IfFieldConditionsMask,
    AICmd_LoadSpikesLayers,
    AICmd_IfAnyPartyMemberIsWounded,
    AICmd_IfAnyPartyMemberUsedPP,
    AICmd_LoadFlingPower,
    AICmd_LoadCurrentMovePP,
    AICmd_IfCanUseLastResort,
    AICmd_LoadCurrentMoveClass,
    AICmd_LoadDefenderLastUsedMoveClass,
    AICmd_LoadBattlerSpeedRank,
    AICmd_LoadBattlerTurnCount,
    AICmd_IfPartyMemberDealsMoreDamage,
    AICmd_IfHasSuperEffectiveMove,
    AICmd_IfBattlerDealsMoreDamage,
    AICmd_SumPositiveStatStages,
    AICmd_DiffStatStages,
    AICmd_IfBattlerHasHigherStat,
    AICmd_IfBattlerHasLowerStat,
    AICmd_IfBattlerHasEqualStat,
    AICmd_CheckIfHighestDamageWithPartner,
    AICmd_IfBattlerFainted,
    AICmd_IfBattlerNotFainted,
    AICmd_LoadAbility,
    AICmd_IfBattlerHasPhysicalAttack,
    AICmd_IfBattlerHasSpecialAttack,
    AICmd_IfBattlerHasStatusAttack,
    AICmd_IfBattlerHasNoPhysicalAttack,
    AICmd_IfBattlerHasNoSpecialAttack,
    AICmd_IfBattlerHasNoStatusAttack,
    AICmd_IfToxicSpikesClearerAliveInParty,
    AICmd_LoadWeight,
    AICmd_IfWishActive,
	AICmd_IfPartyMemberHasBattleEffect,
    AICmd_IfShouldTaunt,
    AICmd_LoadMoveAccuracy,
    AICmd_IfSameAbilities,
    AICmd_IfHasBaseAbility,
	AICmd_IfDestinyBondFails,
    AICmd_IfEnemyCanChunkOrKO,
    AICmd_LoadBattlerCritStage,
    AICmd_IfCanHazeOrPhaze
};

void TrainerAI_Init(BattleSystem *battleSys, BattleContext *battleCtx, u8 battler, u8 initScore)
{
    // must declare these up here to match
    int i;
    u8 invalidMoves;

    // explicit memset
    u8 *adrs = (u8*)&AI_CONTEXT;
    for (i = 0; i < XtOffset(AIContext*, battlerMoves); i++) {
        adrs[i] = 0;
    }

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (initScore & 1) {
            AI_CONTEXT.moveScore[i] = 100;
        } else {
            AI_CONTEXT.moveScore[i] = 0;
        }

        initScore = initScore >> 1;
    }

    // pick damage rolls for moves and score invalid moves to 0
    invalidMoves = BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_ALL);
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (invalidMoves & FlagIndex(i)) {
            AI_CONTEXT.moveScore[i] = 0;
        }
        else {
            if (AI_CONTEXT.moveScore[i] >= 0) {
                AI_CONTEXT.moveScore[i] += 1;
            }
        }

        AI_CONTEXT.moveDamageRolls[i] = 100 - (BattleSystem_RandNext(battleSys) % 16);
    }

    AI_CONTEXT.scriptStackSize = 0;

    // roaming Pokemon have special AI; otherwise, copy the AI behavior from the trainer data
    if (battleSys->battleType & BATTLE_TYPE_ROAMER) {
        AI_CONTEXT.thinkingMask = AI_FLAG_ROAMING_POKEMON;
    } else {
        AI_CONTEXT.thinkingMask = battleSys->trainers[battler].aiMask;
    }

    // force double-battle strategies, if applicable
    if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
        AI_CONTEXT.thinkingMask |= AI_FLAG_TAG_STRATEGY;
    }
}

u8 TrainerAI_Main(BattleSystem *battleSys, u8 battler)
{
    u8 result;
    BattleContext *battleCtx = battleSys->battleCtx;

    if ((AI_CONTEXT.stateFlags & AI_STATUS_FLAG_CONTINUE) == FALSE) {
        AI_CONTEXT.attacker = battler;
        AI_CONTEXT.defender = BattleSystem_RandomOpponent(battleSys, battleCtx, battler);

        TrainerAI_Init(battleSys, battleCtx, AI_CONTEXT.attacker, AI_INIT_SCORE_ALL_MOVES);
    }

    if ((battleSys->battleType & BATTLE_TYPE_DOUBLES) == FALSE) {
        result = TrainerAI_MainSingles(battleSys, battleCtx);
    } else {
        result = TrainerAI_MainDoubles(battleSys, battleCtx);
    }

    return result;
}

/**
 * @brief Main action-choice routine for single battles.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @return The action that the AI picked for its turn. See enum AIActionChoice.
 */
static u8 TrainerAI_MainSingles(BattleSystem *battleSys, BattleContext *battleCtx)
{
    int i;
    u8 maxScoreMoves[4];
    u8 maxScoreMoveSlots[4];
    u8 numMaxScoreMoves;
    u8 action = AI_ENEMY_ATTACK_1;
    u16 move;

    if (battleCtx->totalTurns < 1) {
        if (AI_CONTEXT.thinkingMask & AI_FLAG_OMNISCIENT) {
            TrainerAI_RevealAllInfo(battleSys, battleCtx);
        }

        if (AI_CONTEXT.thinkingMask & AI_FLAG_EXPERT) {
            TrainerAI_RevealBasicInfo(battleSys, battleCtx);
        }
    }

    TrainerAI_RecordLastMove(battleSys, battleCtx);
	
	if (AI_CONTEXT.thinkingMask & AI_FLAG_PRESCIENT) {
		TrainerAI_RecordRandomMove(battleSys, battleCtx);
	}

    while (AI_CONTEXT.thinkingMask) {
        if (AI_CONTEXT.thinkingMask & AI_FLAG_BASIC) {
            if ((AI_CONTEXT.stateFlags & AI_STATUS_FLAG_CONTINUE) == FALSE) {
                AI_CONTEXT.evalStep = AI_EVAL_STEP_INIT;
            }

            TrainerAI_EvalMoves(battleSys, battleCtx);
        }

        AI_CONTEXT.thinkingMask = AI_CONTEXT.thinkingMask >> 1;
        AI_CONTEXT.thinkingBitShift++;
        AI_CONTEXT.moveSlot = 0;
    }

    if (AI_CONTEXT.stateFlags & AI_STATUS_FLAG_ESCAPE) {
        action = AI_ENEMY_ESCAPE;
    } else if (AI_CONTEXT.stateFlags & AI_STATUS_FLAG_SAFARI) {
        action = AI_ENEMY_SAFARI;
    } else {
        // Get the move with the highest score; break ties randomly
        numMaxScoreMoves = 1;
        maxScoreMoves[0] = AI_CONTEXT.moveScore[0];
        maxScoreMoveSlots[0] = AI_ENEMY_ATTACK_1;

        for (i = 1; i < LEARNED_MOVES_MAX; i++) {
            if (battleCtx->battleMons[AI_CONTEXT.attacker].moves[i]) {    // Attacker has a move in this slot
                // Append to the list of max-score moves if equal score to the current max
                if (maxScoreMoves[0] == AI_CONTEXT.moveScore[i]) {
                    maxScoreMoves[numMaxScoreMoves] = AI_CONTEXT.moveScore[i];
                    maxScoreMoveSlots[numMaxScoreMoves++] = i;
                }

                // Set to be the maximum score if higher score than the current max
                if (maxScoreMoves[0] < AI_CONTEXT.moveScore[i]) {
                    numMaxScoreMoves = 1;
                    maxScoreMoves[0] = AI_CONTEXT.moveScore[i];
                    maxScoreMoveSlots[0] = i;
                }
            }
        }

        action = maxScoreMoveSlots[BattleSystem_RandNext(battleSys) % numMaxScoreMoves];
    }

    AI_CONTEXT.selectedTarget[AI_CONTEXT.attacker] = AI_CONTEXT.defender;
    return action;
}

/**
 * @brief Main action-choice routine for double battles.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @return The action that the AI picked for its turn. See enum AIActionChoice.
 */
static u8 TrainerAI_MainDoubles(BattleSystem *battleSys, BattleContext *battleCtx)
{
    int battler, battlerCount, thinkingMask;
    s16 maxScoreForBattler[MAX_BATTLERS];
    u8 battlerTemp[MAX_BATTLERS];
    s8 actionForBattler[MAX_BATTLERS];
    s16 maxScore;
    u16 move;
    s8 moveSlot;

    for (battler = 0; battler < MAX_BATTLERS; battler++) {
        if (battler == AI_CONTEXT.attacker || battleCtx->battleMons[battler].curHP == 0) {
            actionForBattler[battler] = -1;
            maxScoreForBattler[battler] = -1;
            continue;
        }

        TrainerAI_Init(battleSys, battleCtx, AI_CONTEXT.attacker, 0xf);

        // Record the last moves of enemy battlers
        AI_CONTEXT.defender = battler;
        if ((battler & 1) != (AI_CONTEXT.attacker & 1)) {
            TrainerAI_RecordLastMove(battleSys, battleCtx);
        }
		
		if (AI_CONTEXT.thinkingMask & AI_FLAG_PRESCIENT) {
			TrainerAI_RecordRandomMove(battleSys, battleCtx);
		}

        AI_CONTEXT.thinkingBitShift = 0;
        AI_CONTEXT.moveSlot = 0;
        thinkingMask = AI_CONTEXT.thinkingMask;

        // Evaluate moves according with the current battler as the target
        while (thinkingMask) {
            if (thinkingMask & AI_FLAG_BASIC) {
                if ((AI_CONTEXT.stateFlags & AI_STATUS_FLAG_CONTINUE) == FALSE) {
                    AI_CONTEXT.evalStep = AI_EVAL_STEP_INIT;
                }

                TrainerAI_EvalMoves(battleSys, battleCtx);
            }

            thinkingMask >>= 1;
            AI_CONTEXT.thinkingBitShift++;
            AI_CONTEXT.moveSlot = 0;
        }

        if (AI_CONTEXT.stateFlags & AI_STATUS_FLAG_ESCAPE) {
            actionForBattler[battler] = AI_ENEMY_ESCAPE;
        } else if (AI_CONTEXT.stateFlags & AI_STATUS_FLAG_SAFARI) {
            actionForBattler[battler] = AI_ENEMY_SAFARI;
        } else {
            u8 tmpMaxScores[4];
            u8 tmpMaxScoreMoveSlots[4];
            int numMaxScoreMoves, i;

            // Pick a random move from among the highest-scored moves on this target
            tmpMaxScores[0] = AI_CONTEXT.moveScore[0];
            tmpMaxScoreMoveSlots[0] = 0;
            numMaxScoreMoves = 1;

            for (i = 1; i < LEARNED_MOVES_MAX; i++) {
                if (battleCtx->battleMons[AI_CONTEXT.attacker].moves[i]) {
                    // Same score as max: append to list of possible max-score moves
                    if (tmpMaxScores[0] == AI_CONTEXT.moveScore[i]) {
                        tmpMaxScores[numMaxScoreMoves] = AI_CONTEXT.moveScore[i];
                        tmpMaxScoreMoveSlots[numMaxScoreMoves] = i;
                        numMaxScoreMoves++;
                    }

                    // Higher score than max: set as new max score
                    if (tmpMaxScores[0] < AI_CONTEXT.moveScore[i]) {
                        tmpMaxScores[0] = AI_CONTEXT.moveScore[i];
                        tmpMaxScoreMoveSlots[0] = i;
                        numMaxScoreMoves = 1;
                    }
                }
            }

            actionForBattler[battler] = tmpMaxScoreMoveSlots[BattleSystem_RandNext(battleSys) % numMaxScoreMoves];
            maxScoreForBattler[battler] = tmpMaxScores[0];

            // Score moves on an ally below 100 to -1 (basically, never use them)
            if (battler == (AI_CONTEXT.attacker ^ 2)) {
                if (maxScoreForBattler[battler] < 100) {
                    maxScoreForBattler[battler] = -1;
                }
            }
        }
    }

    // Get the highest overall score among all the possible targets
    maxScore = maxScoreForBattler[0];
    battlerTemp[0] = 0;
    battlerCount = 1;
    for (battler = 1; battler < MAX_BATTLERS; battler++) {
        if (maxScore == maxScoreForBattler[battler]) {
            battlerTemp[battlerCount++] = battler;
        }

        if (maxScore < maxScoreForBattler[battler]) {
            maxScore = maxScoreForBattler[battler];
            battlerTemp[0] = battler;
            battlerCount = 1;
        }
    }

    // Pick a random target from among the maximum-scored targets
    AI_CONTEXT.selectedTarget[AI_CONTEXT.attacker] = battlerTemp[(BattleSystem_RandNext(battleSys) % battlerCount)];
    moveSlot = actionForBattler[AI_CONTEXT.selectedTarget[AI_CONTEXT.attacker]];
    move = battleCtx->battleMons[AI_CONTEXT.attacker].moves[moveSlot];

    // Override targets as needed
    if (AI_CONTEXT.moveTable[move].range == RANGE_USER_OR_ALLY
            && Battler_Side(battleSys, AI_CONTEXT.selectedTarget[AI_CONTEXT.attacker]) == 0) {
        AI_CONTEXT.selectedTarget[AI_CONTEXT.attacker] = AI_CONTEXT.attacker;
    }

    if (move == MOVE_CURSE && Move_IsGhostCurse(battleCtx, move, AI_CONTEXT.attacker) == FALSE) {
        AI_CONTEXT.selectedTarget[AI_CONTEXT.attacker] = AI_CONTEXT.attacker;
    }

    return moveSlot;
}

/**
 * @brief Evaluation loop for scoring each move available to the AI.
 * 
 * This does NOT score the potential choices of using an item or switching
 * a Pokemon for turn.
 * 
 * @param battleSys 
 * @param battleCtx 
 */
static void TrainerAI_EvalMoves(BattleSystem *battleSys, BattleContext *battleCtx)
{
    while (AI_CONTEXT.evalStep != AI_EVAL_STEP_END) {
        switch (AI_CONTEXT.evalStep) {
        case AI_EVAL_STEP_INIT:
            battleCtx->aiScriptCursor = battleCtx->aiScriptTemp[AI_CONTEXT.thinkingBitShift];

            if (battleCtx->battleMons[AI_CONTEXT.attacker].ppCur[AI_CONTEXT.moveSlot] == 0) {
                AI_CONTEXT.move = MOVE_NONE;
            } else {
                AI_CONTEXT.move = battleCtx->battleMons[AI_CONTEXT.attacker].moves[AI_CONTEXT.moveSlot];
            }

            AI_CONTEXT.evalStep++;
            break;

        case AI_EVAL_STEP_EVAL:
            if (AI_CONTEXT.move != MOVE_NONE) {
                sAICommandTable[battleCtx->aiScriptTemp[battleCtx->aiScriptCursor]](battleSys, battleCtx);
            } else {
                AI_CONTEXT.moveScore[AI_CONTEXT.moveSlot] = 0;
                AI_CONTEXT.stateFlags |= AI_STATUS_FLAG_DONE;
            }

            if (AI_CONTEXT.stateFlags & AI_STATUS_FLAG_DONE) {
                // If we haven't gone through all the moves, loop back to INIT state and evaluate the next move
                AI_CONTEXT.moveSlot++;
                if (AI_CONTEXT.moveSlot < LEARNED_MOVES_MAX
                        && (AI_CONTEXT.stateFlags & AI_STATUS_FLAG_BREAK) == FALSE) {
                    AI_CONTEXT.evalStep = AI_EVAL_STEP_INIT;
                } else {
                    AI_CONTEXT.evalStep++;
                }

                AI_CONTEXT.stateFlags &= AI_STATUS_FLAG_DONE_OFF;
            }

            break;

        case AI_EVAL_STEP_END:
            break;
        }
    }
}

static void AICmd_IfRandomLessThan(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if ((BattleSystem_RandNext(battleSys) % 256) < val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfRandomGreaterThan(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if ((BattleSystem_RandNext(battleSys) % 256) > val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfRandomEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if ((BattleSystem_RandNext(battleSys) % 256) == val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfRandomNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if ((BattleSystem_RandNext(battleSys) % 256) != val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_AddToMoveScore(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    AI_CONTEXT.moveScore[AI_CONTEXT.moveSlot] += val;

    if (AI_CONTEXT.moveScore[AI_CONTEXT.moveSlot] < 0) {
        AI_CONTEXT.moveScore[AI_CONTEXT.moveSlot] = 0;
    }
}

static void AICmd_IfHPPercentLessThan(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int targetPercent = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    u32 hpPercent = battleCtx->battleMons[battler].curHP * 100 / battleCtx->battleMons[battler].maxHP;

    if (battleCtx->battleMons[battler].maxHP == 1) {
        if (battleCtx->battleMons[battler].heldItem != ITEM_FOCUS_SASH
        && battleCtx->battleMons[battler].heldItem != ITEM_FOCUS_BAND) {

            AIScript_Iter(battleCtx, jump);
        }
    }
    else {
        if (hpPercent < targetPercent) {
            AIScript_Iter(battleCtx, jump);
        }
    }
}

static void AICmd_IfHPPercentGreaterThan(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int targetPercent = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    u32 hpPercent = battleCtx->battleMons[battler].curHP * 100 / battleCtx->battleMons[battler].maxHP;

    if (battleCtx->battleMons[battler].maxHP == 1) {
        if (battleCtx->battleMons[battler].heldItem == ITEM_FOCUS_SASH
        || battleCtx->battleMons[battler].heldItem == ITEM_FOCUS_BAND) {

            AIScript_Iter(battleCtx, jump);
        }
    }
    else {
        if (hpPercent > targetPercent) {
            AIScript_Iter(battleCtx, jump);
        }
    }
}

static void AICmd_IfHPPercentEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int targetPercent = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    u32 hpPercent = battleCtx->battleMons[battler].curHP * 100 / battleCtx->battleMons[battler].maxHP;

    if (hpPercent == targetPercent) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfHPPercentNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int targetPercent = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    u32 hpPercent = battleCtx->battleMons[battler].curHP * 100 / battleCtx->battleMons[battler].maxHP;

    if (hpPercent != targetPercent) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfStatus(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].status & mask) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfNotStatus(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if ((battleCtx->battleMons[battler].status & mask) == FALSE) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfVolatileStatus(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].statusVolatile & mask) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfNotVolatileStatus(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if ((battleCtx->battleMons[battler].statusVolatile & mask) == FALSE) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfMoveEffect(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].moveEffectsMask & mask) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfNotMoveEffect(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if ((battleCtx->battleMons[battler].moveEffectsMask & mask) == FALSE) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfSideCondition(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    u8 side = Battler_Side(battleSys, battler);

    if (battleCtx->sideConditionsMask[side] & mask) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfNotSideCondition(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    u8 side = Battler_Side(battleSys, battler);

    if ((battleCtx->sideConditionsMask[side] & mask) == FALSE) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfLoadedLessThan(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (AI_CONTEXT.calcTemp < val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfLoadedGreaterThan(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (AI_CONTEXT.calcTemp > val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfLoadedEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (AI_CONTEXT.calcTemp == val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfLoadedNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (AI_CONTEXT.calcTemp != val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfLoadedMask(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (AI_CONTEXT.calcTemp & mask) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfLoadedNotMask(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if ((AI_CONTEXT.calcTemp & mask) == FALSE) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfMoveEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (AI_CONTEXT.move == val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfMoveNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (AI_CONTEXT.move != val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfLoadedInTable(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int ofs = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    int val;

    while ((val = AIScript_ReadOffset(battleCtx, ofs)) != 0xFFFFFFFF) {
        if (AI_CONTEXT.calcTemp == val) {
            AIScript_Iter(battleCtx, jump);
            break;
        }

        ofs++;
    }
}

static void AICmd_IfLoadedNotInTable(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int ofs = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    int val;

    while ((val = AIScript_ReadOffset(battleCtx, ofs)) != 0xFFFFFFFF) {
        if (AI_CONTEXT.calcTemp == val) {
            return;
        }

        ofs++;
    }

    AIScript_Iter(battleCtx, jump);
}

static void AICmd_IfAttackerHasDamagingMoves(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    int jump = AIScript_Read(battleCtx);

    int i;
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (battleCtx->battleMons[AI_CONTEXT.attacker].moves[i] != MOVE_NONE
                && MOVE_DATA(battleCtx->battleMons[AI_CONTEXT.attacker].moves[i]).power) {
            break;
        }
    }

    if (i < LEARNED_MOVES_MAX) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfAttackerHasNoDamagingMoves(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    int jump = AIScript_Read(battleCtx);

    int i;
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (battleCtx->battleMons[AI_CONTEXT.attacker].moves[i] != MOVE_NONE
                && MOVE_DATA(battleCtx->battleMons[AI_CONTEXT.attacker].moves[i]).power) {
            break;
        }
    }

    if (i == LEARNED_MOVES_MAX) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_LoadTurnCount(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = battleCtx->totalTurns;
}

static void AICmd_LoadTypeFrom(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int param = AIScript_Read(battleCtx);
    int partner;

    switch (param) {
    case LOAD_ATTACKER_TYPE_1:
        AI_CONTEXT.calcTemp = BattleMon_Get(battleCtx, AI_CONTEXT.attacker, BATTLEMON_TYPE_1, NULL);
        break;

    case LOAD_DEFENDER_TYPE_1:
        AI_CONTEXT.calcTemp = BattleMon_Get(battleCtx, AI_CONTEXT.defender, BATTLEMON_TYPE_1, NULL);
        break;

    case LOAD_ATTACKER_TYPE_2:
        AI_CONTEXT.calcTemp = BattleMon_Get(battleCtx, AI_CONTEXT.attacker, BATTLEMON_TYPE_2, NULL);
        break;

    case LOAD_DEFENDER_TYPE_2:
        AI_CONTEXT.calcTemp = BattleMon_Get(battleCtx, AI_CONTEXT.defender, BATTLEMON_TYPE_2, NULL);
        break;

    case LOAD_MOVE_TYPE:
        AI_CONTEXT.calcTemp = MOVE_DATA(AI_CONTEXT.move).type;
        break;

    case LOAD_ATTACKER_PARTNER_TYPE_1:
        partner = BattleSystem_Partner(battleSys, AI_CONTEXT.attacker);
        AI_CONTEXT.calcTemp = BattleMon_Get(battleCtx, partner, BATTLEMON_TYPE_1, NULL);
        break;

    case LOAD_DEFENDER_PARTNER_TYPE_1:
        partner = BattleSystem_Partner(battleSys, AI_CONTEXT.defender);
        AI_CONTEXT.calcTemp = BattleMon_Get(battleCtx, partner, BATTLEMON_TYPE_1, NULL);
        break;
    
    case LOAD_ATTACKER_PARTNER_TYPE_2:
        partner = BattleSystem_Partner(battleSys, AI_CONTEXT.attacker);
        AI_CONTEXT.calcTemp = BattleMon_Get(battleCtx, partner, BATTLEMON_TYPE_2, NULL);
        break;

    case LOAD_DEFENDER_PARTNER_TYPE_2:
        partner = BattleSystem_Partner(battleSys, AI_CONTEXT.defender);
        AI_CONTEXT.calcTemp = BattleMon_Get(battleCtx, partner, BATTLEMON_TYPE_1, NULL);
        break;

    default:
        GF_ASSERT(FALSE);
        break;
    }
}

static void AICmd_FlagBattlerIsType(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int type = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (MON_HAS_TYPE(battler, type)) {
        AI_CONTEXT.calcTemp = TRUE;
    } else {
        AI_CONTEXT.calcTemp = FALSE;
    }
}

static void AICmd_LoadMovePower(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = MOVE_DATA(AI_CONTEXT.move).power;
}

static void AICmd_FlagMoveDamageScore(BattleSystem *battleSys, BattleContext *battleCtx)
{
    int i = 0, riskyIdx, altPowerIdx;
    s32 moveDamage[LEARNED_MOVES_MAX];
    BOOL varyDamage;
    u8 ivs[STAT_MAX];

    AIScript_Iter(battleCtx, 1);

    varyDamage = AIScript_Read(battleCtx);

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
            AI_CONTEXT.calcTemp = AI_MOVE_IS_HIGHEST_DAMAGE;
        } else {
            AI_CONTEXT.calcTemp = AI_NOT_HIGHEST_DAMAGE;
        }
    } else {
        AI_CONTEXT.calcTemp = AI_NO_COMPARISON_MADE;
    }
}

static void AICmd_LoadBattlerPreviousMove(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = battleCtx->movePrevByBattler[battler];
}

static void AICmd_IfTempEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (val == AI_CONTEXT.calcTemp) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfTempNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (val != AI_CONTEXT.calcTemp) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfSpeedCompareEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, AI_CONTEXT.attacker, AI_CONTEXT.defender, TRUE) == val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfSpeedCompareNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, AI_CONTEXT.attacker, AI_CONTEXT.defender, TRUE) != val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_CountAlivePartyBattlers(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);

    AI_CONTEXT.calcTemp = 0;

    u8 battler = AIScript_Battler(battleCtx, inBattler);
    Party *party = BattleSystem_Party(battleSys, battler);
    u8 battlerSlot, partnerSlot;

    if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
        battlerSlot = battleCtx->selectedPartySlot[battler];
        partnerSlot = battleCtx->selectedPartySlot[BattleSystem_Partner(battleSys, battler)];
    } else {
        battlerSlot = partnerSlot = battleCtx->selectedPartySlot[battler];
    }

    for (int i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

        if (i != battlerSlot
                && i != partnerSlot
                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG) {
            AI_CONTEXT.calcTemp++;
        }
    }
}

static void AICmd_LoadCurrentMove(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = AI_CONTEXT.move;
}

static void AICmd_LoadCurrentMoveEffect(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = MOVE_DATA(AI_CONTEXT.move).effect;
}

static void AICmd_LoadBattlerAbility(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_ABILITY_SUPPRESSED) {
        AI_CONTEXT.calcTemp = ABILITY_NONE;
    } else if (AI_CONTEXT.attacker != battler && inBattler != AI_BATTLER_ATTACKER_PARTNER) {
        // If we already know an opponent's ability, load that ability
        if (AI_CONTEXT.battlerAbilities[battler]) {
            AI_CONTEXT.calcTemp = AI_CONTEXT.battlerAbilities[battler];
        } else {
            // If the opponent has an ability that traps us, we should already know about it (because it self-announces)
            if (battleCtx->battleMons[battler].ability == ABILITY_SHADOW_TAG
                    || battleCtx->battleMons[battler].ability == ABILITY_MAGNET_PULL
					|| battleCtx->battleMons[battler].ability == ABILITY_THIRSTY
                    || battleCtx->battleMons[battler].ability == ABILITY_ARENA_TRAP) {
                AI_CONTEXT.calcTemp = battleCtx->battleMons[battler].ability;
            } else {
                // Try to guess the opponent's ability (flip a coin)
                int ability1 = PokemonPersonalData_GetSpeciesValue(battleCtx->battleMons[battler].species, MON_DATA_PERSONAL_ABILITY_1);
                int ability2 = PokemonPersonalData_GetSpeciesValue(battleCtx->battleMons[battler].species, MON_DATA_PERSONAL_ABILITY_2);

                if (ability1 && ability2) {
                    if (BattleSystem_RandNext(battleSys) & 1) {
                        AI_CONTEXT.calcTemp = ability1;
                    } else {
                        AI_CONTEXT.calcTemp = ability2;
                    }
                } else if (ability1) {
                    AI_CONTEXT.calcTemp = ability1;
                } else {
                    AI_CONTEXT.calcTemp = ability2;
                }
            }
        }
    } else {
        AI_CONTEXT.calcTemp = battleCtx->battleMons[battler].ability;
    }
}

static void AICmd_CheckBattlerAbility(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int expected = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int tmpAbility;

    if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_ABILITY_SUPPRESSED) {
        tmpAbility = ABILITY_NONE;
    } else if (inBattler == AI_BATTLER_DEFENDER || inBattler == AI_BATTLER_DEFENDER_PARTNER) {
        // If we already know an opponent's ability, load that ability
        if (AI_CONTEXT.battlerAbilities[battler]) {
            tmpAbility = AI_CONTEXT.battlerAbilities[battler];
            AI_CONTEXT.calcTemp = AI_CONTEXT.battlerAbilities[battler];
        } else {
            // If the opponent has an ability that announces, we should already know about it (because it self-announces)
            if (battleCtx->battleMons[battler].ability == ABILITY_SHADOW_TAG
                || battleCtx->battleMons[battler].ability == ABILITY_MAGNET_PULL
				|| battleCtx->battleMons[battler].ability == ABILITY_THIRSTY
                || battleCtx->battleMons[battler].ability == ABILITY_ARENA_TRAP
                || battleCtx->battleMons[battler].ability == ABILITY_INTIMIDATE
                || battleCtx->battleMons[battler].ability == ABILITY_TRACE
                || battleCtx->battleMons[battler].ability == ABILITY_DOWNLOAD
                || battleCtx->battleMons[battler].ability == ABILITY_ANTICIPATION
                || battleCtx->battleMons[battler].ability == ABILITY_FOREWARN
                || battleCtx->battleMons[battler].ability == ABILITY_SLOW_START
                || battleCtx->battleMons[battler].ability == ABILITY_FRISK
                || battleCtx->battleMons[battler].ability == ABILITY_MOLD_BREAKER
                || battleCtx->battleMons[battler].ability == ABILITY_PRESSURE
				|| battleCtx->battleMons[battler].ability == ABILITY_GENETIC_FREAK
                || battleCtx->battleMons[battler].ability == ABILITY_RANDOM_SELECT) {
                tmpAbility = battleCtx->battleMons[battler].ability;
            } else {
                // Try to guess the opponent's ability (flip a coin)
                int ability1 = PokemonPersonalData_GetSpeciesValue(battleCtx->battleMons[battler].species, MON_DATA_PERSONAL_ABILITY_1);
                int ability2 = PokemonPersonalData_GetSpeciesValue(battleCtx->battleMons[battler].species, MON_DATA_PERSONAL_ABILITY_2);

                if (ability1 && ability2) {
                    // If the opponent has two abilities, but neither are the expected one,
                    // coinflip to pick.
                    if (ability1 != expected && ability2 != expected) {
                        if (BattleSystem_RandNext(battleSys) & 1)
                        {
                            tmpAbility = ability1;
                        }
                        else 
                        {
                            tmpAbility = ability2;
                        }
                    // Otherwise, pretend that we don't know about it
                    } else {
                        tmpAbility = ABILITY_NONE;
                    }
                }
            }
        }
    } else {
        tmpAbility = battleCtx->battleMons[battler].ability;
    }

    if (tmpAbility == ABILITY_NONE) {
        AI_CONTEXT.calcTemp = AI_UNKNOWN;
    } else if (tmpAbility == expected) {
        AI_CONTEXT.calcTemp = AI_HAVE;
    } else {
        AI_CONTEXT.calcTemp = AI_NOT_HAVE;
    }
}

static void AICmd_CalcMaxEffectiveness(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    AI_CONTEXT.calcTemp = TYPE_MULTI_IMMUNE;

    for (int i = 0; i < LEARNED_MOVES_MAX; i++) {
        u32 damage = TYPE_MULTI_BASE_DAMAGE;
        u32 effectiveness = 0;
        u16 move = battleCtx->battleMons[AI_CONTEXT.attacker].moves[i];
        int moveType = TrainerAI_MoveType(battleSys, battleCtx, AI_CONTEXT.attacker, move);

        if (move) {
            damage = BattleSystem_ApplyTypeChart(battleSys,
                battleCtx,
                move,
                moveType,
                AI_CONTEXT.attacker,
                AI_CONTEXT.defender,
                damage,
                &effectiveness);

            if (damage == TYPE_MULTI_STAB_DAMAGE * 2) {
                damage = TYPE_MULTI_DOUBLE_DAMAGE;
            } else if (damage == TYPE_MULTI_STAB_DAMAGE * 4) {
                damage = TYPE_MULTI_QUADRUPLE_DAMAGE;
            } else if (damage == TYPE_MULTI_STAB_DAMAGE / 2) {
                damage = TYPE_MULTI_HALF_DAMAGE;
            } else if (damage == TYPE_MULTI_STAB_DAMAGE / 4) {
                damage = TYPE_MULTI_QUARTER_DAMAGE;
            }

            if (effectiveness & MOVE_STATUS_IMMUNE) {
                damage = TYPE_MULTI_IMMUNE;
            }

            if (AI_CONTEXT.calcTemp < damage) {
                AI_CONTEXT.calcTemp = damage;
            }
        }
    }
}

static void AICmd_IfMoveEffectivenessEquals(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int expected = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
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
    } else if (damage == TYPE_MULTI_STAB_DAMAGE * 4) {
        damage = TYPE_MULTI_QUADRUPLE_DAMAGE;
    } else if (damage == TYPE_MULTI_STAB_DAMAGE / 2) {
        damage = TYPE_MULTI_HALF_DAMAGE;
    } else if (damage == TYPE_MULTI_STAB_DAMAGE / 4) {
        damage = TYPE_MULTI_QUARTER_DAMAGE;
    }

    if (effectiveness & MOVE_STATUS_IMMUNE) {
        damage = TYPE_MULTI_IMMUNE;
    }

    if (damage == expected) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfPartyMemberStatus(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    Party *party; // this must be declared first to match
    int inBattler = AIScript_Read(battleCtx);
    u32 statusMask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    u8 slot1, slot2;
    if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
        slot1 = battleCtx->selectedPartySlot[battler];
        slot2 = battleCtx->selectedPartySlot[BattleSystem_Partner(battleSys, battler)];
    } else {
        slot1 = slot2 = battleCtx->selectedPartySlot[battler];
    }

    party = BattleSystem_Party(battleSys, battler);
    for (int i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

        if (i != slot1 && i != slot2
                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & statusMask)) {
            AIScript_Iter(battleCtx, jump);
            return;
        }
    }
}

static void AICmd_IfPartyMemberNotStatus(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    Party *party; // this must be declared first to match
    int inBattler = AIScript_Read(battleCtx);
    u32 statusMask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    u8 slot1, slot2;
    if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
        slot1 = battleCtx->selectedPartySlot[battler];
        slot2 = battleCtx->selectedPartySlot[BattleSystem_Partner(battleSys, battler)];
    } else {
        slot1 = slot2 = battleCtx->selectedPartySlot[battler];
    }

    party = BattleSystem_Party(battleSys, battler);
    for (int i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

        if (i != slot1 && i != slot2
                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & statusMask) == FALSE) {
            AIScript_Iter(battleCtx, jump);
            return;
        }
    }
}

static void AICmd_LoadCurrentWeather(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    AI_CONTEXT.calcTemp = AI_WEATHER_CLEAR;

    if (WEATHER_IS_RAIN) {
        AI_CONTEXT.calcTemp = AI_WEATHER_RAINING;
    }

    if (WEATHER_IS_SAND) {
        AI_CONTEXT.calcTemp = AI_WEATHER_SANDSTORM;
    }

    if (WEATHER_IS_SUN) {
        AI_CONTEXT.calcTemp = AI_WEATHER_SUNNY;
    }

    if (WEATHER_IS_HAIL) {
        AI_CONTEXT.calcTemp = AI_WEATHER_HAILING;
    }

    if (WEATHER_IS_FOG) {
        AI_CONTEXT.calcTemp = AI_WEATHER_DEEP_FOG;
    }
}

static void AICmd_IfCurrentMoveEffectEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int expected = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (MOVE_DATA(AI_CONTEXT.move).effect == expected) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfCurrentMoveEffectNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int expected = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (MOVE_DATA(AI_CONTEXT.move).effect != expected) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfStatStageLessThan(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int stat = AIScript_Read(battleCtx);
    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].statBoosts[stat] < val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfStatStageGreaterThan(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int stat = AIScript_Read(battleCtx);
    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].statBoosts[stat] > val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfStatStageEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int stat = AIScript_Read(battleCtx);
    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].statBoosts[stat] == val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfStatStageNotEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int stat = AIScript_Read(battleCtx);
    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].statBoosts[stat] != val) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfCurrentMoveKills(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    BOOL useDamageRoll = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    int roll;
    if (useDamageRoll == TRUE) {
        roll = AI_CONTEXT.moveDamageRolls[AI_CONTEXT.moveSlot];
    } else {
        roll = 100;
    }

    int riskyIdx;
    for (riskyIdx = 0; sRiskyMoves[riskyIdx] != 0xFFFF; riskyIdx++) {
        if (MOVE_DATA(AI_CONTEXT.move).effect == sRiskyMoves[riskyIdx]) {
            break;
        }
    }

    int altPowerIdx;
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
            AIScript_Iter(battleCtx, jump);
        }
    }
}

static void AICmd_IfCurrentMoveDoesNotKill(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    BOOL useDamageRoll = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    int roll;
    if (useDamageRoll == TRUE) {
        roll = AI_CONTEXT.moveDamageRolls[AI_CONTEXT.moveSlot];
    } else {
        roll = 100;
    }

    int riskyIdx;
    for (riskyIdx = 0; sRiskyMoves[riskyIdx] != 0xFFFF; riskyIdx++) {
        if (MOVE_DATA(AI_CONTEXT.move).effect == sRiskyMoves[riskyIdx]) {
            break;
        }
    }

    int altPowerIdx;
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

        if (battleCtx->battleMons[AI_CONTEXT.defender].curHP > damage) {
            AIScript_Iter(battleCtx, jump);
        }
    }
}

static void AICmd_IfMoveKnown(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int move = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
    case AI_BATTLER_ATTACKER:
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (battleCtx->battleMons[battler].moves[i] == move) {
                break;
            }
        }

        if (i < LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case AI_BATTLER_DEFENDER_PARTNER:
    case AI_BATTLER_ATTACKER_PARTNER:
        if (battleCtx->battleMons[battler].curHP == 0) {
            break;
        }

        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (battleCtx->battleMons[battler].moves[i] == move) {
                break;
            }
        }

        if (i < LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case AI_BATTLER_DEFENDER:
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (AI_CONTEXT.battlerMoves[battler][i] == move) {
                break;
            }
        }

        if (i < LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    default:
        break;
    }
}

static void AICmd_IfMoveNotKnown(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int move = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
    case AI_BATTLER_ATTACKER:
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (battleCtx->battleMons[battler].moves[i] == move) {
                break;
            }
        }

        if (i == LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case AI_BATTLER_ATTACKER_PARTNER:
        if (battleCtx->battleMons[battler].curHP == 0) {
            break;
        }

        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (battleCtx->battleMons[battler].moves[i] == move) {
                break;
            }
        }

        if (i == LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case AI_BATTLER_DEFENDER:
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (AI_CONTEXT.battlerMoves[battler][i] == move) {
                break;
            }
        }

        if (i == LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    default:
        break;
    }
}

static void AICmd_IfMoveEffectKnown(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int effect = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
    case AI_BATTLER_ATTACKER:
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (battleCtx->battleMons[battler].moves[i]
                    && MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect == effect) {
                break;
            }
        }

        if (i < LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case AI_BATTLER_DEFENDER:
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (AI_CONTEXT.battlerMoves[battler][i]
                    && MOVE_DATA(AI_CONTEXT.battlerMoves[battler][i]).effect == effect) {
                break;
            }
        }

        if (i < LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    default:
        break;
    }
}

static void AICmd_IfMoveEffectNotKnown(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int effect = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
    case AI_BATTLER_ATTACKER:
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (battleCtx->battleMons[battler].moves[i]
                    && MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect == effect) {
                break;
            }
        }

        if (i == LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case AI_BATTLER_DEFENDER:
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (AI_CONTEXT.battlerMoves[battler][i]
                    && MOVE_DATA(AI_CONTEXT.battlerMoves[battler][i]).effect == effect) {
                break;
            }
        }

        if (i == LEARNED_MOVES_MAX) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    default:
        break;
    }
}

static void AICmd_IfBattlerUnderEffect(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int check = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    switch (check) {
    case CHECK_DISABLE:
        if (battleCtx->battleMons[battler].moveEffectsData.disabledTurns) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case CHECK_ENCORE:
        if (battleCtx->battleMons[battler].moveEffectsData.encoredTurns) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    default:
        break;
    }
}

static void AICmd_IfCurrentMoveMatchesEffect(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int check = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    switch (check) {
    case CHECK_DISABLE:
        if (battleCtx->battleMons[AI_CONTEXT.attacker].moveEffectsData.disabledMove == AI_CONTEXT.move) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case CHECK_ENCORE:
        if (battleCtx->battleMons[AI_CONTEXT.attacker].moveEffectsData.encoredMove == AI_CONTEXT.move) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    default:
        break;
    }
}

static void AICmd_Escape(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.stateFlags |= (AI_STATUS_FLAG_DONE | AI_STATUS_FLAG_ESCAPE | AI_STATUS_FLAG_BREAK);
}

static void AICmd_Dummy3E(BattleSystem *battleSys, BattleContext *battleCtx)
{
    return;
}

static void AICmd_Dummy3F(BattleSystem *battleSys, BattleContext *battleCtx)
{
    return;
}

static void AICmd_LoadHeldItem(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = battleCtx->battleMons[battler].heldItem;
}

static void AICmd_LoadHeldItemEffect(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (AI_CONTEXT.attacker != battler) {
        AI_CONTEXT.calcTemp = BattleSystem_GetItemData(battleCtx, AI_CONTEXT.battlerHeldItems[battler], ITEM_PARAM_HOLD_EFFECT);
    } else {
        AI_CONTEXT.calcTemp = BattleSystem_GetItemData(battleCtx, battleCtx->battleMons[battler].heldItem, ITEM_PARAM_HOLD_EFFECT);
    }
}

static void AICmd_IfHeldItemEqualTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int expected = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    u16 heldItem;

    if ((battler & 1) == (AI_CONTEXT.attacker & 1)) {
        heldItem = battleCtx->battleMons[battler].heldItem;
    } else {
        heldItem = AI_CONTEXT.battlerHeldItems[battler];
    }

    if (heldItem == expected) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfFieldConditionsMask(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    u32 mask = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    if (battleCtx->fieldConditionsMask & mask) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_LoadSpikesLayers(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u32 sideCondition = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    u8 side = Battler_Side(battleSys, battler);

    switch (sideCondition) {
    case SIDE_CONDITION_SPIKES:
        AI_CONTEXT.calcTemp = battleCtx->sideConditions[side].spikesLayers;
        break;

    case SIDE_CONDITION_TOXIC_SPIKES:
        AI_CONTEXT.calcTemp = battleCtx->sideConditions[side].toxicSpikesLayers;
        break;
    }
}

static void AICmd_IfAnyPartyMemberIsWounded(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    for (int i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
        Pokemon *mon = BattleSystem_PartyPokemon(battleSys, battler, i);

        if (i != battleCtx->selectedPartySlot[battler]
                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL)) {
            AIScript_Iter(battleCtx, jump);
            break;
        }
    }
}

static void AICmd_IfAnyPartyMemberUsedPP(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int moveSlot; // must be declared outside of the loop to match

    for (int partySlot = 0; partySlot < BattleSystem_PartyCount(battleSys, battler); partySlot++) {
        Pokemon *mon = BattleSystem_PartyPokemon(battleSys, battler, partySlot);

        if (partySlot != battleCtx->selectedPartySlot[battler]) {
            for (moveSlot = 0; moveSlot < LEARNED_MOVES_MAX; moveSlot++) {
                if (Pokemon_GetValue(mon, MON_DATA_MOVE1_CUR_PP + moveSlot, NULL) != Pokemon_GetValue(mon, MON_DATA_MOVE1_MAX_PP + moveSlot, NULL)) {
                    AIScript_Iter(battleCtx, jump);
                    break;
                }
            }

            if (moveSlot != LEARNED_MOVES_MAX) {
                break;
            }
        }
    }
}

static void AICmd_LoadFlingPower(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = Battler_ItemFlingPower(battleCtx, battler);
}

static void AICmd_LoadCurrentMovePP(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = battleCtx->battleMons[AI_CONTEXT.attacker].ppCur[AI_CONTEXT.moveSlot];
}

static void AICmd_IfCanUseLastResort(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int numKnownMoves = Battler_CountMoves(battleSys, battleCtx, battler);

    if (battleCtx->battleMons[battler].moveEffectsData.lastResortCount >= (numKnownMoves - 1) && numKnownMoves > 1) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_LoadCurrentMoveClass(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = MOVE_DATA(AI_CONTEXT.move).class;
}

static void AICmd_LoadDefenderLastUsedMoveClass(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = MOVE_DATA(battleCtx->movePrevByBattler[AI_CONTEXT.defender]).class;
}

static void AICmd_LoadBattlerSpeedRank(BattleSystem *battleSys, BattleContext *battleCtx)
{
    // Must declare C89-style to match
    int i, j;
    int speedOrder[4];
    int cmp1, cmp2;
    int maxBattlers;
    int battler;
    int inBattler;

    AIScript_Iter(battleCtx, 1);

    inBattler = AIScript_Read(battleCtx);
    battler = AIScript_Battler(battleCtx, inBattler);
    maxBattlers = BattleSystem_MaxBattlers(battleSys);

    for (i = 0; i < maxBattlers; i++) {
        speedOrder[i] = i;
    }

    for (i = 0; i < maxBattlers - 1; i++) {
        for (j = i + 1; j < maxBattlers; j++) {
            cmp1 = speedOrder[i];
            cmp2 = speedOrder[j];

            if (BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, cmp1, cmp2, TRUE)) {
                speedOrder[i] = cmp2;
                speedOrder[j] = cmp1;
            }
        }
    }

    for (i = 0; i < maxBattlers; i++) {
        if (speedOrder[i] == battler) {
            AI_CONTEXT.calcTemp = i;
            break;
        }
    }
}

static void AICmd_LoadBattlerTurnCount(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = battleCtx->totalTurns - battleCtx->battleMons[battler].moveEffectsData.fakeOutTurnNumber;
}

static void AICmd_IfPartyMemberDealsMoreDamage(BattleSystem *battleSys, BattleContext *battleCtx)
{
    // Declare C89-style to match
    int i, j;
    BOOL varyDamage;
    int jump;
    int battler;
    s32 activeMonDamage;
    s32 partyMonDamage;
    s32 allDamageVals[LEARNED_MOVES_MAX];
    u16 partyMonMoves[LEARNED_MOVES_MAX];
    u8 ivs[STAT_MAX];
    Pokemon *partyMon;

    AIScript_Iter(battleCtx, 1);

    varyDamage = AIScript_Read(battleCtx);
    jump = AIScript_Read(battleCtx);
    battler = AI_CONTEXT.attacker;

    for (i = 0; i < 6; i++) {
        ivs[i] = BattleMon_Get(battleCtx, battler, BATTLEMON_HP_IV + i, NULL);
    }

    activeMonDamage = TrainerAI_CalcAllDamage(battleSys,
        battleCtx,
        AI_CONTEXT.attacker,
        battleCtx->battleMons[battler].moves,
        allDamageVals,
        battleCtx->battleMons[battler].heldItem,
        ivs,
        Battler_Ability(battleCtx, battler),
        battleCtx->battleMons[battler].moveEffectsData.embargoTurns,
        varyDamage);

    for (i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
        if (i != battleCtx->selectedPartySlot[battler]) {
            partyMon = BattleSystem_PartyPokemon(battleSys, battler, i);

            if (Pokemon_GetValue(partyMon, MON_DATA_CURRENT_HP, NULL) != 0
                    && Pokemon_GetValue(partyMon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                    && Pokemon_GetValue(partyMon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG) {
                for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                    partyMonMoves[j] = Pokemon_GetValue(partyMon, MON_DATA_MOVE1 + j, NULL);
                }

                for (j = 0; j < STAT_MAX; j++) {
                    ivs[j] = Pokemon_GetValue(partyMon, MON_DATA_HP_IV + j, NULL);
                }

                partyMonDamage = TrainerAI_CalcAllDamage(battleSys,
                    battleCtx,
                    AI_CONTEXT.attacker,
                    partyMonMoves,
                    allDamageVals,
                    Pokemon_GetValue(partyMon, MON_DATA_HELD_ITEM, NULL),
                    ivs,
                    Pokemon_GetValue(partyMon, MON_DATA_ABILITY, NULL),
                    FALSE,
                    varyDamage);

                if (partyMonDamage > activeMonDamage) {
                    AIScript_Iter(battleCtx, jump);
                    break;
                }
            }
        }
    }
}

static void AICmd_IfHasSuperEffectiveMove(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int jump = AIScript_Read(battleCtx);

    if (AI_HasSuperEffectiveMove(battleSys, battleCtx, AI_CONTEXT.attacker, TRUE) == TRUE) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfBattlerDealsMoreDamage(BattleSystem *battleSys, BattleContext *battleCtx)
{
    int i;
    int inBattler;
    BOOL varyDamage;
    int jump;
    int battler;
    int roll;
    s32 aiDamage;
    s32 battlerDamage;
    s32 damageVals[LEARNED_MOVES_MAX];
    u8 ivs[STAT_MAX];

    AIScript_Iter(battleCtx, 1);

    inBattler = AIScript_Read(battleCtx);
    varyDamage = AIScript_Read(battleCtx);
    jump = AIScript_Read(battleCtx);

    for (i = 0; i < STAT_MAX; i++) {
        ivs[i] = BattleMon_Get(battleCtx, AI_CONTEXT.attacker, BATTLEMON_HP_IV + i, NULL);
    }

    aiDamage = TrainerAI_CalcAllDamage(battleSys,
        battleCtx,
        AI_CONTEXT.attacker,
        battleCtx->battleMons[AI_CONTEXT.attacker].moves,
        damageVals,
        battleCtx->battleMons[AI_CONTEXT.attacker].heldItem,
        ivs,
        Battler_Ability(battleCtx, AI_CONTEXT.attacker),
        battleCtx->battleMons[AI_CONTEXT.attacker].moveEffectsData.embargoTurns,
        varyDamage);
    battler = AIScript_Battler(battleCtx, inBattler);

    if (varyDamage == TRUE) {
        roll = AI_CONTEXT.moveDamageRolls[AI_CONTEXT.moveSlot];
    } else {
        roll = 100;
    }

    battlerDamage = TrainerAI_CalcDamage(battleSys,
        battleCtx,
        battleCtx->movePrevByBattler[battler],
        battleCtx->battleMons[battler].heldItem,
        ivs,
        battler,
        Battler_Ability(battleCtx, battler),
        battleCtx->battleMons[battler].moveEffectsData.embargoTurns,
        roll);

    if (battlerDamage > aiDamage) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_SumPositiveStatStages(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = 0;

    for (int stat = BATTLE_STAT_HP; stat < BATTLE_STAT_MAX; stat++) {
        if (battleCtx->battleMons[battler].statBoosts[stat] > 6) {
            AI_CONTEXT.calcTemp += battleCtx->battleMons[battler].statBoosts[stat] - 6;
        }
    }
}

static void AICmd_DiffStatStages(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int stat = AIScript_Read(battleCtx);
    int battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = battleCtx->battleMons[battler].statBoosts[stat] - battleCtx->battleMons[AI_CONTEXT.attacker].statBoosts[stat];
}

static void AICmd_IfBattlerHasHigherStat(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int stat = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    int battler = AIScript_Battler(battleCtx, inBattler);

    int aiStat, battlerStat;
    TrainerAI_GetStats(battleCtx, battler, &aiStat, &battlerStat, stat);

    if (aiStat < battlerStat) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfBattlerHasLowerStat(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int stat = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    int battler = AIScript_Battler(battleCtx, inBattler);

    int aiStat, battlerStat;
    TrainerAI_GetStats(battleCtx, battler, &aiStat, &battlerStat, stat);

    if (aiStat > battlerStat) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfBattlerHasEqualStat(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int stat = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    int battler = AIScript_Battler(battleCtx, inBattler);

    int aiStat, battlerStat;
    TrainerAI_GetStats(battleCtx, battler, &aiStat, &battlerStat, stat);

    if (aiStat == battlerStat) {
        AIScript_Iter(battleCtx, jump);
    }
}

// Conditional jump in trainer_ai_script if battler has a physical move 

static void AICmd_IfBattlerHasPhysicalAttack(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
        case AI_BATTLER_ATTACKER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_PHYSICAL) {
                    // ignore rapid spin
                    if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect != BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING) {
                         
                        break;
                    }
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_ATTACKER_PARTNER:
            if (battleCtx->battleMons[battler].curHP == 0) {
                break;
            }
            
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_PHYSICAL) {
                    // ignore rapid spin
                    if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect != BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING) {
                         
                        break;
                    }
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_DEFENDER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_PHYSICAL) {
                    // ignore rapid spin
                    if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect != BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING) {
                         
                        break;
                    }
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        default:
            break;
    }
}

// Conditional jump in trainer_ai_script if battler has no physical moves

static void AICmd_IfBattlerHasNoPhysicalAttack(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
        case AI_BATTLER_ATTACKER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_PHYSICAL) {
                    // ignore rapid spin
                    if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect != BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING) {
                         
                        break;
                    }
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_ATTACKER_PARTNER:
            if (battleCtx->battleMons[battler].curHP == 0) {
                break;
            }
            
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                 if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_PHYSICAL) {
                    // ignore rapid spin
                    if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect != BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING) {
                         
                        break;
                    }
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_DEFENDER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_PHYSICAL) {
                    // ignore rapid spin
                    if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect != BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING) {
                         
                        break;
                    }
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        default:
            break;
    }
}

// Conditional jump in trainer_ai_script if battler has a special move 

static void AICmd_IfBattlerHasSpecialAttack(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
        case AI_BATTLER_ATTACKER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_SPECIAL) {
                    break;
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_ATTACKER_PARTNER:
            if (battleCtx->battleMons[battler].curHP == 0) {
                break;
            }
            
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                 if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_SPECIAL) {
                    break;
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_DEFENDER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_SPECIAL) {
                    break;
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        default:
            break;
    }
}

// Conditional jump in trainer_ai_script if battler has no special moves

static void AICmd_IfBattlerHasNoSpecialAttack(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
        case AI_BATTLER_ATTACKER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_SPECIAL) {
                    break;
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_ATTACKER_PARTNER:
            if (battleCtx->battleMons[battler].curHP == 0) {
                break;
            }
            
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                 if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_SPECIAL) {
                    break;
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_DEFENDER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_SPECIAL) {
                    break;
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        default:
            break;
    }
}


// Conditional jump in trainer_ai_script if battler has a status move 

static void AICmd_IfBattlerHasStatusAttack(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
        case AI_BATTLER_ATTACKER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_STATUS) {
                    break;
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_ATTACKER_PARTNER:
            if (battleCtx->battleMons[battler].curHP == 0) {
                break;
            }
            
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                 if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_STATUS) {
                    break;
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_DEFENDER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_STATUS) {
                    break;
                }
            }

            if (i < LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        default:
            break;
    }
}

// Conditional jump in trainer_ai_script if battler has no status moves

static void AICmd_IfBattlerHasNoStatusAttack(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i;

    switch (inBattler) {
        case AI_BATTLER_ATTACKER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_STATUS) {
                    break;
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_ATTACKER_PARTNER:
            if (battleCtx->battleMons[battler].curHP == 0) {
                break;
            }
            
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                 if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_STATUS) {
                    break;
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        case AI_BATTLER_DEFENDER:
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_STATUS) {
                    break;
                }
            }

            if (i >= LEARNED_MOVES_MAX) {
                AIScript_Iter(battleCtx, jump);
            }
            break;

        default:
            break;
    }
}

/**
 * @brief Get the values for a given stat for the AI battler and another given battler.
 * 
 * @param battleCtx 
 * @param battler   The other battler whose stats will be retrieved.
 * @param buf1      Buffer to hold the stat-value for the AI battler.
 * @param buf2      Buffer to hold the stat-value for the given other battler.
 * @param stat      Which stat value to load.
 */
static void TrainerAI_GetStats(BattleContext *battleCtx, int battler, int *buf1, int *buf2, int stat)
{
    switch (stat) {
    case BATTLE_STAT_HP:
        *buf1 = battleCtx->battleMons[AI_CONTEXT.attacker].curHP;
        *buf2 = battleCtx->battleMons[battler].curHP;
        break;

    case BATTLE_STAT_ATTACK:
        *buf1 = battleCtx->battleMons[AI_CONTEXT.attacker].attack;
        *buf2 = battleCtx->battleMons[battler].attack;
        break;

    case BATTLE_STAT_DEFENSE:
        *buf1 = battleCtx->battleMons[AI_CONTEXT.attacker].defense;
        *buf2 = battleCtx->battleMons[battler].defense;
        break;

    case BATTLE_STAT_SP_ATTACK:
        *buf1 = battleCtx->battleMons[AI_CONTEXT.attacker].spAttack;
        *buf2 = battleCtx->battleMons[battler].spAttack;
        break;

    case BATTLE_STAT_SP_DEFENSE:
        *buf1 = battleCtx->battleMons[AI_CONTEXT.attacker].spDefense;
        *buf2 = battleCtx->battleMons[battler].spDefense;
        break;

    case BATTLE_STAT_SPEED:
        *buf1 = battleCtx->battleMons[AI_CONTEXT.attacker].speed;
        *buf2 = battleCtx->battleMons[battler].speed;
        break;

    default:
        GF_ASSERT(FALSE);
        break;
    }
}

static void AICmd_CheckIfHighestDamageWithPartner(BattleSystem *battleSys, BattleContext *battleCtx)
{
    int i = 0, j, k;
    s32 moveDamage;
    s32 damageVals[LEARNED_MOVES_MAX];
    BOOL varyDamage;
    u8 ivs[STAT_MAX];
    int battler;

    AIScript_Iter(battleCtx, 1);
    varyDamage = AIScript_Read(battleCtx);

    for (j = 0; sRiskyMoves[j] != 0xFFFF; j++) {
        if (MOVE_DATA(AI_CONTEXT.move).effect == sRiskyMoves[j]) {
            break;
        }
    }

    for (k = 0; sAltPowerCalcMoves[k] != 0xFFFF; k++) {
        if (MOVE_DATA(AI_CONTEXT.move).effect == sAltPowerCalcMoves[k]) {
            break;
        }
    }

    if (sAltPowerCalcMoves[k] != 0xFFFF
            || (MOVE_DATA(AI_CONTEXT.move).power > 1 && sRiskyMoves[j] == 0xFFFF)) {
        battler = AI_CONTEXT.attacker;

        for (j = 0; j < MAX_BATTLERS_PER_SIDE; j++) {
            for (i = STAT_HP; i < STAT_MAX; i++) {
                ivs[i] = BattleMon_Get(battleCtx, battler, BATTLEMON_HP_IV + i, NULL);
            }

            TrainerAI_CalcAllDamage(battleSys,
                battleCtx,
                battler,
                battleCtx->battleMons[battler].moves,
                damageVals,
                battleCtx->battleMons[battler].heldItem,
                ivs,
                Battler_Ability(battleCtx, battler),
                battleCtx->battleMons[battler].moveEffectsData.embargoTurns,
                varyDamage);

            // Update to the partner for the next iteration
            battler = BattleSystem_Partner(battleSys, AI_CONTEXT.attacker);

            if (j == 0) {
                moveDamage = damageVals[AI_CONTEXT.moveSlot];
            }

            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (damageVals[i] > moveDamage) {
                    break;
                }
            }

            if (i == LEARNED_MOVES_MAX) {
                AI_CONTEXT.calcTemp = AI_MOVE_IS_HIGHEST_DAMAGE;
            } else {
                AI_CONTEXT.calcTemp = AI_NOT_HIGHEST_DAMAGE;
                break;
            }
        }
    } else {
        AI_CONTEXT.calcTemp = AI_NO_COMPARISON_MADE;
    }
}

static void AICmd_IfBattlerFainted(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    GF_ASSERT(inBattler != AI_BATTLER_ATTACKER);
    GF_ASSERT(inBattler != AI_BATTLER_DEFENDER);

    int battler = AIScript_Battler(battleCtx, inBattler);
    if (battleCtx->battlersSwitchingMask & FlagIndex(battler)) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfBattlerNotFainted(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    GF_ASSERT(inBattler != AI_BATTLER_ATTACKER);
    GF_ASSERT(inBattler != AI_BATTLER_DEFENDER);

    int battler = AIScript_Battler(battleCtx, inBattler);
    if ((battleCtx->battlersSwitchingMask & FlagIndex(battler)) == FALSE) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_LoadGender(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = battleCtx->battleMons[battler].gender;
}

static void AICmd_LoadIsFirstTurnInBattle(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].moveEffectsData.fakeOutTurnNumber < battleCtx->totalTurns) {
        AI_CONTEXT.calcTemp = FALSE;
    } else {
        AI_CONTEXT.calcTemp = TRUE;
    }
}

static void AICmd_LoadStockpileCount(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = battleCtx->battleMons[battler].moveEffectsData.stockpileCount;
}

static void AICmd_LoadBattleType(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = battleSys->battleType;
}

static void AICmd_LoadRecycleItem(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = battleCtx->recycleItem[battler];
}

static void AICmd_LoadTypeOfLoadedMove(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = MOVE_DATA(AI_CONTEXT.calcTemp).type;
}

static void AICmd_LoadPowerOfLoadedMove(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = MOVE_DATA(AI_CONTEXT.calcTemp).power;
}

static void AICmd_LoadEffectOfLoadedMove(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = MOVE_DATA(AI_CONTEXT.calcTemp).effect;
}

static void AICmd_LoadProtectChain(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->moveProtect[battler] != MOVE_PROTECT
            && battleCtx->moveProtect[battler] != MOVE_DETECT
            && battleCtx->moveProtect[battler] != MOVE_ENDURE) {
        AI_CONTEXT.calcTemp = 0;
    } else {
        AI_CONTEXT.calcTemp = battleCtx->battleMons[battler].moveEffectsData.protectSuccessTurns;
    }
}

static void AICmd_PushAndGoTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    int jump = AIScript_Read(battleCtx);
    AIScript_PushCursor(battleSys, battleCtx, jump);
}

static void AICmd_GoTo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    int jump = AIScript_Read(battleCtx);
    AIScript_Iter(battleCtx, jump);
}

static void AICmd_PopOrEnd(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    if (AIScript_PopCursor(battleSys, battleCtx) == TRUE) {
        return;
    }

    AI_CONTEXT.stateFlags |= AI_STATUS_FLAG_DONE;
}

static void AICmd_IfLevel(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int op = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    switch (op) {
    case CHECK_HIGHER_THAN_TARGET:
        if (battleCtx->battleMons[AI_CONTEXT.attacker].level > battleCtx->battleMons[AI_CONTEXT.defender].level) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case CHECK_LOWER_THAN_TARGET:
        if (battleCtx->battleMons[AI_CONTEXT.attacker].level < battleCtx->battleMons[AI_CONTEXT.defender].level) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    case CHECK_EQUAL_TO_TARGET:
        if (battleCtx->battleMons[AI_CONTEXT.attacker].level == battleCtx->battleMons[AI_CONTEXT.defender].level) {
            AIScript_Iter(battleCtx, jump);
        }
        break;

    default:
        break;
    }
}

static void AICmd_IfTargetIsTaunted(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    int jump = AIScript_Read(battleCtx);

    if (battleCtx->battleMons[AI_CONTEXT.defender].moveEffectsData.tauntedTurns) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfTargetIsNotTaunted(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    int jump = AIScript_Read(battleCtx);

    if (battleCtx->battleMons[AI_CONTEXT.defender].moveEffectsData.tauntedTurns == 0) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfTargetIsPartner(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int jump = AIScript_Read(battleCtx);

    if ((AI_CONTEXT.attacker & 1) == (AI_CONTEXT.defender & 1)) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfActivatedFlashFire(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].moveEffectsData.flashFire) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_LoadAbility(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    AI_CONTEXT.calcTemp = Battler_Ability(battleCtx, battler);
}

static void AICmd_IfToxicSpikesClearerAliveInParty(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    int i = 0;
    int partyMax;

    u8 battler = AIScript_Battler(battleCtx, inBattler);
    Party *party = BattleSystem_Party(battleSys, battler);
    u8 battlerSlot, partnerSlot;

    battlerSlot = battleCtx->selectedPartySlot[battler];

    if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
        partnerSlot = battleCtx->selectedPartySlot[BattleSystem_Partner(battleSys, battler)];
    }
    else {
        partnerSlot = battlerSlot;
    }

    partyMax = BattleSystem_PartyCount(battleSys, battler);

    for (i = 0; i < partyMax; i++) {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
            && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
            && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
            && (Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL) == TYPE_POISON
                || Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL) == TYPE_POISON)) {
                if (Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL) == TYPE_FLYING
                    || Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL) == TYPE_FLYING
                    || BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_HOLD_EFFECT) == HOLD_EFFECT_LEVITATE_POPPED_IF_HIT
                    || Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_LEVITATE) {
                    if (BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_HOLD_EFFECT) == HOLD_EFFECT_SPEED_DOWN_GROUNDED) {
                        break;
                    }
                }
                else {
                    break;
                }
        }
    }

    if (i < partyMax) {
        AIScript_Iter(battleCtx, jump);
    }
}
/*
static void AICmd_TeamMoveEffectivenessScore(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    // int inBattler = AIScript_Read(battleCtx);
    int val = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u16 move, moveType, moveClass, moveEffect, moveScore;
    u32 moveEffectivenes;
    int attacker, defender;

    moveScore = 40;

    AI_CONTEXT.calcTemp = 0;

    attacker = AI_CONTEXT.atacker;
    defender = AI_CONTEXT.defender;

    Party *party = BattleSystem_Party(battleSys, attacker);
    Party *defenderParty = BattleSystem_Party(battleSys, defender);
    move = AI_CONTEXT.move;

    moveClass = MOVE_DATA(move).class;
    moveEffect = MOVE_DATA(move).effect;
    moveType = TrainerAI_MoveType(battleSys, battleCtx, battler, move);
    
    int i, j, partyMax, defenderPartyMax;

    partyMax = BattleSystem_PartyCount(battleSys, attacker);
    defenderPartyMax = BattleSystem_PartyCount(battleSys, defender);

    for (i = 0; i < partyMax; i++) {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
        && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
        && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG) {
            if (moveClass == CLASS_STATUS) {
                for (j = 0; j < defenderPartyMax; j++) {
                    Pokemon *defenderMon = Party_GetPokemonBySlotIndex(defenderParty, j);
                    if (Pokemon_GetValue(defenderMon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY) {
                        // -14 score for each statused mon in the enemy party
                        if (moveScore >= 14) {
                            moveScore -= 14;
                        }
                        else {
                            if (moveType )
                        }

                    }
                }
                if (battleCtx->battleMons[AI_CONTEXT.defender].status & MON_CONDITION_ANY) {
                    if (moveScore >= )
                }
            }
        }
    }

}
*/

static void AICmd_LoadWeight(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
	
	int monWeight = battleCtx->battleMons[battler].weight;
	
	if (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY)
	{
		monWeight *= 2;
	}

    AI_CONTEXT.calcTemp = monWeight;
}


static void AICmd_IfWishActive(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);


    if ((battleCtx->fieldConditions.wishTurns[battler] > 0)
        && (battleCtx->fieldConditions.wishTurns[battler] <= 2)) {

        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfPartyMemberHasBattleEffect(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    Party *party; // this must be declared first to match
    int inBattler = AIScript_Read(battleCtx);
    u16 expected = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);
    int i, j;
    u16 move;

    u8 slot1, slot2;
    if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
        slot1 = battleCtx->selectedPartySlot[battler];
        slot2 = battleCtx->selectedPartySlot[BattleSystem_Partner(battleSys, battler)];
    } else {
        slot1 = slot2 = battleCtx->selectedPartySlot[battler];
    }

    switch (inBattler) {
        default:
            break;

        case AI_BATTLER_ATTACKER:

            party = BattleSystem_Party(battleSys, battler);
            for (i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
                Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

                if (i != slot1 && i != slot2
                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && ((Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_INCAPACITATED) == FALSE)) {

                    for (j = 0; j < LEARNED_MOVES_MAX; j++) {

                        move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);

                        if (move == expected) {
                    
                            AIScript_Iter(battleCtx, jump);
                            return;
                        }
                    }
                }
            }
            break;

        case AI_BATTLER_DEFENDER_PARTNER:
        case AI_BATTLER_ATTACKER_PARTNER:
            if (battleCtx->battleMons[battler].curHP == 0) {
                break;
            }

            party = BattleSystem_Party(battleSys, battler);
            for (i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
                Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

                if (i != slot1 && i != slot2
                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && ((Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_INCAPACITATED) == FALSE)) {

                    for (j = 0; j < LEARNED_MOVES_MAX; j++) {

                        move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);

                        if (move == expected) {
                    
                            AIScript_Iter(battleCtx, jump);
                            return;
                        }
                    }
                }
            }
            break;
    }
}

static void AICmd_IfShouldTaunt(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (AI_ShouldTauntCheck(battleSys, battleCtx, AI_CONTEXT.attacker, battler)) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_LoadMoveAccuracy(BattleSystem *battleSys, BattleContext *battleCtx)
{
    u8 moveType;

    AIScript_Iter(battleCtx, 1);
    AI_CONTEXT.calcTemp = MOVE_DATA(AI_CONTEXT.move).accuracy;

    moveType = MOVE_DATA(AI_CONTEXT.move).type;

    // Moves with no acc check
    if (AI_CONTEXT.calcTemp == 0) {
        AI_CONTEXT.calcTemp = 100;
    }

    if (NO_CLOUD_NINE) {
        if ((WEATHER_IS_SUN && MOVE_DATA(AI_CONTEXT.move).effect == BATTLE_EFFECT_THUNDER)
            || (WEATHER_IS_SUN && MOVE_DATA(AI_CONTEXT.move).effect == BATTLE_EFFECT_HURRICANE))
        {
            AI_CONTEXT.calcTemp = 50;
        }

        if ((WEATHER_IS_RAIN && MOVE_DATA(AI_CONTEXT.move).effect == BATTLE_EFFECT_THUNDER)
            || (WEATHER_IS_RAIN && MOVE_DATA(AI_CONTEXT.move).effect == BATTLE_EFFECT_HURRICANE))
        {
            AI_CONTEXT.calcTemp = 100;
        }

        if (WEATHER_IS_SAND && moveType == TYPE_ROCK) {
            AI_CONTEXT.calcTemp = AI_CONTEXT.calcTemp * 11 / 10;
        }
    }

    if (MON_HAS_TYPE(AI_CONTEXT.attacker, moveType)) {
        AI_CONTEXT.calcTemp = AI_CONTEXT.calcTemp * 11 / 10;
    }

    if (Battler_HeldItemEffect(battleCtx, AI_CONTEXT.attacker) == HOLD_EFFECT_ACCURACY_UP) {
        AI_CONTEXT.calcTemp = AI_CONTEXT.calcTemp * 11 / 10;
    }

    if (Battler_HeldItemEffect(battleCtx, AI_CONTEXT.attacker) == HOLD_EFFECT_ACCURACY_UP_SLOWER) {
        if (BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, AI_CONTEXT.attacker, AI_CONTEXT.defender, TRUE) == COMPARE_SPEED_SLOWER) {
            AI_CONTEXT.calcTemp = AI_CONTEXT.calcTemp * 6 / 5;
        }
    }

    if (Battler_Ability(battleCtx, AI_CONTEXT.attacker) == ABILITY_COMPOUND_EYES) {
        AI_CONTEXT.calcTemp = AI_CONTEXT.calcTemp * 13 / 10;
    }

    if ((Battler_Ability(battleCtx, AI_CONTEXT.attacker) == ABILITY_NO_GUARD)
	|| (Battler_Ability(battleCtx, AI_CONTEXT.attacker) == ABILITY_SUCTION_CUPS)) {
        AI_CONTEXT.calcTemp = 100;
    }

    if (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_OUR_SIDE, AI_CONTEXT.attacker, ABILITY_ILLUMINATE))
	{
		AI_CONTEXT.calcTemp = AI_CONTEXT.calcTemp * 6 / 5;
	}

    if (battleCtx->battleMons[AI_CONTEXT.attacker].moveEffectsData.micleBerry) {
        AI_CONTEXT.calcTemp = AI_CONTEXT.calcTemp * 6 / 5;
    }

    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY) {
        AI_CONTEXT.calcTemp = AI_CONTEXT.calcTemp * 5 / 3;
    }

    if (AI_CONTEXT.calcTemp > 100) {
        AI_CONTEXT.calcTemp = 100;
    }
}

static void AICmd_IfSameAbilities(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler1 = AIScript_Read(battleCtx);
    int inBattler2 = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);
    u8 battler1 = AIScript_Battler(battleCtx, inBattler1);
    u8 battler2 = AIScript_Battler(battleCtx, inBattler2);

    if (battleCtx->battleMons[battler1].ability ==  battleCtx->battleMons[battler2].ability) {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_IfHasBaseAbility(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    u8 battler = AIScript_Battler(battleCtx, inBattler);

    u8 battlerAbility = Battler_Ability(battleCtx, battler);
    u8 ability1 = PokemonPersonalData_GetSpeciesValue(battleCtx->battleMons[battler].species, MON_DATA_PERSONAL_ABILITY_1);
    u8 ability2 = PokemonPersonalData_GetSpeciesValue(battleCtx->battleMons[battler].species, MON_DATA_PERSONAL_ABILITY_2);

    if (battlerAbility != ABILITY_NONE) {
        if (battlerAbility == ability1 || battlerAbility == ability2) {
            AIScript_Iter(battleCtx, jump);
        }
    }
}

static void AICmd_IfDestinyBondFails(BattleSystem *battleSys, BattleContext *battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    u8 battler = AIScript_Battler(battleCtx, inBattler);

    if (battleCtx->battleMons[battler].moveEffectsData.destinyBondSuccessTurns != 0)
	{
		AIScript_Iter(battleCtx, jump);
	}
}

static void AICmd_IfEnemyCanChunkOrKO(BattleSystem* battleSys, BattleContext* battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    
    int jump = AIScript_Read(battleCtx);

    if (AI_AttackerChunksOrKOsDefender(battleSys, battleCtx, AI_CONTEXT.defender, AI_CONTEXT.attacker))
    {
        AIScript_Iter(battleCtx, jump);
    }
}

static void AICmd_LoadBattlerCritStage(BattleSystem* battleSys, BattleContext* battleCtx)
{
    AIScript_Iter(battleCtx, 1);

    int inBattler = AIScript_Read(battleCtx);
    u8 battler1 = AIScript_Battler(battleCtx, inBattler);
    u8 battler2;
    u16 battler1Species;
    u16 item;
    u32 battler1VolStatus;
    int battler1Side, battler2Side;
    int battler1Ability, itemEffect;
    int critStage;

    critStage = battleCtx->criticalBoosts;

    switch (battler1)
    {
        default:
            battler2 = BattleSystem_RandomOpponent(battleSys, battleCtx, battler1);
            break;

        case AI_BATTLER_ATTACKER_PARTNER:
        case AI_BATTLER_ATTACKER:
            battler2 = AI_CONTEXT.defender;
            break;

        case AI_BATTLER_DEFENDER_PARTNER:
        case AI_BATTLER_DEFENDER:
            battler2 = AI_CONTEXT.attacker;
            break;
    }

    battler1Side = Battler_Side(battleSys, battler1);
    battler1Species = battleCtx->battleMons[battler1].species;
    battler1VolStatus = battleCtx->battleMons[battler1].statusVolatile;
    battler1Ability = battleCtx->battleMons[battler1].ability;

    item = Battler_HeldItem(battleCtx, battler1);
    itemEffect = BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HOLD_EFFECT);

    battler2Side = Battler_Side(battleSys, battler2);

    if ((battleCtx->sideConditionsMask[battler2Side] & SIDE_CONDITION_LUCKY_CHANT)
        || Battler_IgnorableAbility(battleCtx, battler1, battler2, ABILITY_BATTLE_ARMOR))
    {
        critStage = 0;
    }
    else
    {
        if (battler1VolStatus & VOLATILE_CONDITION_FOCUS_ENERGY)
        {
            critStage++;
        }

        if (itemEffect == HOLD_EFFECT_CRITRATE_UP)
        {
            critStage++;
        }

        switch (battler1Species)
        {
        default:
            break;

        case SPECIES_CHANSEY:
            if (itemEffect == HOLD_EFFECT_CHANSEY_CRITRATE_UP)
            {
                critStage += 2;
            }
            break;

        case SPECIES_FARFETCHD:
            if (itemEffect == HOLD_EFFECT_FARFETCHD_CRITRATE_UP)
            {
                critStage += 2;
            }
            break;
        }

        if (battleCtx->sideConditionsMask[battler1Side] & SIDE_CONDITION_LUCKY_CHANT)
        {
            critStage++;
        }

        if (battleCtx->battleMons[battler1].meditateCritBoostFlag)
        {
            critStage++;
        }

        if (battler1Ability == ABILITY_SUPER_LUCK)
        {
            critStage++;
        }
    }

    if (critStage > 3)
    {
        critStage = 3;
    }

    AI_CONTEXT.calcTemp = critStage;
}

static void AICmd_IfCanHazeOrPhaze(BattleSystem* battleSys, BattleContext* battleCtx)
{
    AIScript_Iter(battleCtx, 1);
    int inBattler = AIScript_Read(battleCtx);
    int jump = AIScript_Read(battleCtx);

    int battler1 = AIScript_Battler(battleCtx, inBattler);
    int battler2;
    int i, effect;
    u16 move;
    BOOL canHazeOrPhaze;

    canHazeOrPhaze = FALSE;

    switch (battler1)
    {
    default:
        battler2 = BattleSystem_RandomOpponent(battleSys, battleCtx, battler1);
        break;

    case AI_BATTLER_ATTACKER_PARTNER:
    case AI_BATTLER_ATTACKER:
        battler2 = AI_CONTEXT.defender;
        break;

    case AI_BATTLER_DEFENDER_PARTNER:
    case AI_BATTLER_DEFENDER:
        battler2 = AI_CONTEXT.attacker;
        break;
    }

    for (i = 0; i < LEARNED_MOVES_MAX; i++)
    {
        move = AI_CONTEXT.battlerMoves[battler1][i];

        if (battleCtx->battleMons[battler1].moveEffectsData.encoredMove != MOVE_NONE)
        {
            move = battleCtx->battleMons[battler1].moveEffectsData.encoredMove;
        }

        if (battleCtx->battleMons[battler1].moveEffectsData.choiceLockedMove != MOVE_NONE)
        {
            move = battleCtx->battleMons[battler1].moveEffectsData.choiceLockedMove;
        }

        effect = MOVE_DATA(move).effect;

        if ((BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler1, 0, CHECK_INVALID_ALL) & FlagIndex(i)) == FALSE)
        {

            if (MOVE_DATA(move).class == CLASS_STATUS)
            {
                if (battleCtx->battleMons[battler1].moveEffectsData.tauntedTurns == 0)
                {
                    switch (effect)
                    {
                    default:
                        break;

                    case BATTLE_EFFECT_FORCE_SWITCH:
                        if (move == MOVE_ROAR
                            && AI_CONTEXT.battlerAbilities[battler2] == ABILITY_SOUNDPROOF
                            && Battler_IgnorableAbility(battleCtx, battler1, battler2, ABILITY_SOUNDPROOF))
                        {
                            break;
                        }
                        else
                        {
                            if ((AI_CONTEXT.battlerAbilities[battler2] == ABILITY_SUCTION_CUPS
                                && Battler_IgnorableAbility(battleCtx, battler1, battler2, ABILITY_SUCTION_CUPS)
                                || battleCtx->battleMons[battler2].moveEffectsMask & MOVE_EFFECT_INGRAIN))
                            {
                                break;
                            }
                            else
                            {
                                canHazeOrPhaze = TRUE;
                            }
                        }
                        break;

                    case BATTLE_EFFECT_SWAP_ATK_SP_ATK_STAT_CHANGES:
                    case BATTLE_EFFECT_SWAP_DEF_SP_DEF_STAT_CHANGES:
                    case BATTLE_EFFECT_SWAP_STAT_CHANGES:
                        canHazeOrPhaze = TRUE;
                        break;

                    case BATTLE_EFFECT_RESET_STAT_CHANGES:
                        canHazeOrPhaze = TRUE;
                        break;
                    }
                }

                if (MOVE_DATA(move).range == RANGE_MAGIC_BOUNCE)
                {
                    if (AI_CONTEXT.battlerAbilities[battler2] == ABILITY_MAGIC_BOUNCE)
                    {
                        if (Battler_IgnorableAbility(battleCtx, battler1, battler2, ABILITY_MAGIC_BOUNCE))
                        {
                            canHazeOrPhaze = FALSE;
                        }
                    }
                }
            }
            else
            {
                switch (effect)
                {
                default:
                    break;

                case BATTLE_EFFECT_FORCE_SWITCH_HIT:
                    if (battleCtx->battleMons[battler2].moveEffectsMask & MOVE_EFFECT_INGRAIN)
                    {
                        break;
                    }
                    else
                    {
                        canHazeOrPhaze = TRUE;
                    }
                }
            }
        }

        if (battleCtx->battleMons[battler1].moveEffectsData.encoredMove != MOVE_NONE
            || battleCtx->battleMons[battler1].moveEffectsData.choiceLockedMove != MOVE_NONE)
        {
            break;
        }
    }

    if (canHazeOrPhaze)
    {
        AIScript_Iter(battleCtx, jump);
    }
}

/**
 * @brief Push an address for the AI script onto the cursor stack.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param offset    Distance to jump ahead after pushing the cursor.
 */
static void AIScript_PushCursor(BattleSystem *battleSys, BattleContext *battleCtx, int offset)
{
    AI_CONTEXT.scriptStackPointer[AI_CONTEXT.scriptStackSize++] = battleCtx->aiScriptCursor;
    AIScript_Iter(battleCtx, offset);

    GF_ASSERT(AI_CONTEXT.scriptStackSize <= AI_MAX_STACK_SIZE);
}

/**
 * @brief Pop the top element of the cursor stack into the cursor.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @return TRUE if the cursor stack had an element to be popped; FALSE
 * if it was empty.
 */
static BOOL AIScript_PopCursor(BattleSystem *battleSys, BattleContext *battleCtx)
{
    if (AI_CONTEXT.scriptStackSize) {
        AI_CONTEXT.scriptStackSize--;
        battleCtx->aiScriptCursor = AI_CONTEXT.scriptStackPointer[AI_CONTEXT.scriptStackSize];
        return TRUE;
    }

    return FALSE;
}

/**
 * @brief Record the last move used by an active battler, if it is not
 * already known.
 * 
 * @param battleSys 
 * @param battleCtx 
 */
static void TrainerAI_RecordLastMove(BattleSystem *battleSys, BattleContext *battleCtx)
{
    u8 partySlot;
    u16 move;
    int i, j, partyMax;
    Pokemon *mon;

    move = battleCtx->movePrevByBattler[AI_CONTEXT.defender];

    if (move != MOVE_STRUGGLE
    && move != MOVE_NONE) {
        // Here we want to just learn every instance of a given pivot move
        // on the opponent's team if they use that pivot move because the 
        // active mon data is switched before the AI gets to run this code
        // again.
        if (MOVE_DATA(move).effect == BATTLE_EFFECT_HIT_BEFORE_SWITCH
            || MOVE_DATA(move).effect == BATTLE_EFFECT_FLEE_FROM_WILD_BATTLE) {
            partyMax = BattleSystem_PartyCount(battleSys, AI_CONTEXT.defender);

            for (i = 0; i < partyMax; i++) {
                mon = BattleSystem_PartyPokemon(battleSys, AI_CONTEXT.defender, i);

                for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                
                    if(move == Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL)) {

                        if(AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] != move) {

                            AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                            break;
                        }
                    }
                }
            }
        }
        else {

            partySlot = battleCtx->selectedPartySlot[AI_CONTEXT.defender];

            for (j = 0; j < LEARNED_MOVES_MAX; j++) {

                if (AI_CONTEXT.battlerMoves[AI_CONTEXT.defender][j] == move) {

                    if (AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] == move) {

                        break;
                    }
                    else {

                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] = move;
                        break;
                    }
                }

                if (AI_CONTEXT.battlerMoves[AI_CONTEXT.defender][j] == MOVE_NONE) {

                    if (AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] == MOVE_NONE) {

                        AI_CONTEXT.battlerMoves[AI_CONTEXT.defender][j] = move;
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] = move;
                        break;
                    }

                    AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] = move;
                    AI_CONTEXT.battlerMoves[AI_CONTEXT.defender][j] = move;
                    break;
                }
            }
        }
    }
}

/**
 * @brief Record a random move known by an active battler, if it is not
 * already known.
 * 
 * @param battleSys 
 * @param battleCtx 
 */
static void TrainerAI_RecordRandomMove(BattleSystem *battleSys, BattleContext *battleCtx)
{
    u8 partySlot;
    u16 move;
    int i, j, partyMax, randMove;
    Pokemon *mon;
	
	partySlot = battleCtx->selectedPartySlot[AI_CONTEXT.defender];
	randMove = BattleSystem_RandNext(battleSys) % 4;
	
    move = AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][randMove];

    if (move != MOVE_STRUGGLE
    && move != MOVE_NONE) {
        // Here we want to just learn every instance of a given pivot move
        // on the opponent's team if they use that pivot move because the 
        // active mon data is switched before the AI gets to run this code
        // again.
        if (MOVE_DATA(move).effect == BATTLE_EFFECT_HIT_BEFORE_SWITCH
            || MOVE_DATA(move).effect == BATTLE_EFFECT_FLEE_FROM_WILD_BATTLE) {
            partyMax = BattleSystem_PartyCount(battleSys, AI_CONTEXT.defender);

            for (i = 0; i < partyMax; i++) {
                mon = BattleSystem_PartyPokemon(battleSys, AI_CONTEXT.defender, i);

                for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                
                    if(move == Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL)) {

                        if(AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] != move) {

                            AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                            break;
                        }
                    }
                }
            }
        }
        else {

            partySlot = battleCtx->selectedPartySlot[AI_CONTEXT.defender];

            for (j = 0; j < LEARNED_MOVES_MAX; j++) {

                if (AI_CONTEXT.battlerMoves[AI_CONTEXT.defender][j] == move) {

                    if (AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] == move) {

                        break;
                    }
                    else {

                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] = move;
                        break;
                    }
                }

                if (AI_CONTEXT.battlerMoves[AI_CONTEXT.defender][j] == MOVE_NONE) {

                    if (AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] == MOVE_NONE) {

                        AI_CONTEXT.battlerMoves[AI_CONTEXT.defender][j] = move;
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] = move;
                        break;
                    }

                    AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][partySlot][j] = move;
                    AI_CONTEXT.battlerMoves[AI_CONTEXT.defender][j] = move;
                    break;
                }
            }
        }
    }
}

/**
 * @brief Reveal all moves known by the opponent's party.
 * 
 * @param battleSys 
 * @param battleCtx 
 */
static void TrainerAI_RevealAllInfo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    u8 partySlot, ability;
    u16 move, heldItem;
    int i, j, partyMax;
    Pokemon *mon;

    partyMax = BattleSystem_PartyCount(battleSys, AI_CONTEXT.defender);

    for (i = 0; i < partyMax; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, AI_CONTEXT.defender, i);

        heldItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
        ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);

        AI_CONTEXT.battlerPartyAbilities[AI_CONTEXT.defender][i] = ability;
        AI_CONTEXT.battlerPartyHeldItems[AI_CONTEXT.defender][i] = heldItem;

        for (j = 0; j < LEARNED_MOVES_MAX; j++) {

            move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);

            if (move != MOVE_NONE) {

                AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
            }
            else {
                break;
            }
        }
    }
}


/**
 * @brief Reveal fundamental moves known by the opponent's party, and their
 * abilities.
 * 
 * @param battleSys 
 * @param battleCtx 
 */

static void TrainerAI_RevealBasicInfo(BattleSystem *battleSys, BattleContext *battleCtx)
{
    u8 partySlot, ability, moveType, side;
    u16 move, heldItem;
    int i, j, partyMax;
    int moveClass, effect, movePower;
    Pokemon *mon;

    partyMax = BattleSystem_PartyCount(battleSys, AI_CONTEXT.attacker);

    side = Battler_Side(battleSys, AI_CONTEXT.defender);

    for (i = 0; i < partyMax; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, AI_CONTEXT.defender, i);

        heldItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
        ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);

        AI_CONTEXT.battlerPartyAbilities[AI_CONTEXT.defender][i] = ability;
        AI_CONTEXT.battlerPartyHeldItems[AI_CONTEXT.defender][i] = heldItem;

        for (j = 0; j < LEARNED_MOVES_MAX; j++) {

            move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);

            if (move == MOVE_NONE) {
                break;
            }

            moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);
            moveClass = MOVE_DATA(move).class;
            effect = MOVE_DATA(move).effect;
            movePower = MOVE_DATA(move).power;

            if (movePower == 1) {
                movePower = BattleSystem_CalcMoveDamage(battleSys,
                battleCtx,
                move,
                battleCtx->sideConditionsMask[side],
                battleCtx->fieldConditionsMask,
                movePower,
                moveType,
                AI_CONTEXT.defender,
                AI_CONTEXT.attacker,
                1);
            }
            

            //movePower = 

            if (moveClass != CLASS_STATUS) {

                if (moveType == battleCtx->battleMons[AI_CONTEXT.defender].type1
                || moveType == battleCtx->battleMons[AI_CONTEXT.defender].type2) {

                    AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                }

                if (MOVE_DATA(move).priority > 0) {
                    AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                }

                if (ability == ABILITY_TECHNICIAN
                && move)

                switch (effect) {
                    default:
                        break;

                    // True damage moves
                    case BATTLE_EFFECT_LEVEL_DAMAGE_FLAT:
                    case BATTLE_EFFECT_40_DAMAGE_FLAT:
                    case BATTLE_EFFECT_10_DAMAGE_FLAT:
                    case BATTLE_EFFECT_SET_HP_EQUAL_TO_USER:
                    case BATTLE_EFFECT_HALVE_HP:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Trapping moves
                    case BATTLE_EFFECT_HIT_BEFORE_SWITCH:
                    case BATTLE_EFFECT_BIND_HIT:
                    case BATTLE_EFFECT_WHIRLPOOL:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Hazard-setting hit
                    case BATTLE_EFFECT_SPIKES_MULTI_HIT:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Hazards clearing
                    case BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Fling
                    case BATTLE_EFFECT_FLING:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Knock Off
                    case BATTLE_EFFECT_REMOVE_HELD_ITEM:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Speed Drop
                    case BATTLE_EFFECT_LOWER_SPEED_HIT:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Explosion and Self Destruct
                    case BATTLE_EFFECT_HALVE_DEFENSE:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;
                }

            }
            else {

                if (MapBattleEffectToStatusCondition(battleCtx, effect) != MON_CONDITION_NONE
                    || MapBattleEffectToVolatileStatus(battleCtx, effect) != VOLATILE_CONDITION_NONE
                    || MapBattleEffectToSideCondition(battleCtx, effect) != SIDE_CONDITION_NONE
                    || MapBattleEffectToFieldCondition(battleCtx, effect) != FIELD_CONDITION_NONE) {
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                }

                switch (effect) {
                    default:
                        break;

                    // Healing moves
                    case BATTLE_EFFECT_RESTORE_HALF_HP:
                    case BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN:
                    case BATTLE_EFFECT_SWALLOW:
                    case BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE:
                    case BATTLE_EFFECT_HEAL_IN_3_TURNS:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Baton pass
                    case BATTLE_EFFECT_PASS_STATS_AND_STATUS:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Defog
                    case BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;

                    // Encore
                    case BATTLE_EFFECT_ENCORE:
                        AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][i][j] = move;
                        break;
                }
            }
        }
    }
}

/**
 * @brief Read a word from the AI script at the current cursor position,
 * then increment the cursor.
 * 
 * @param battleCtx 
 * @return Current word for the AI script under the cursor.
 */
static int AIScript_Read(BattleContext *battleCtx)
{
    int word = battleCtx->aiScriptTemp[battleCtx->aiScriptCursor];
    battleCtx->aiScriptCursor++;

    return word;
}

/**
 * @brief Read a word from the AI script at the current cursor position
 * offset by a specified value, then increment the cursor.
 * 
 * @param battleCtx 
 * @return Current word for the AI script under the cursor + an offset.
 */
static int AIScript_ReadOffset(BattleContext *battleCtx, int ofs)
{
    return battleCtx->aiScriptTemp[battleCtx->aiScriptCursor + ofs];
}

/**
 * @brief Increment the cursor for the AI script by a fixed amount.
 * 
 * @param battleCtx 
 * @param i         Amount by which to increment the script cursor.
 */
static void AIScript_Iter(BattleContext *battleCtx, int i)
{
    battleCtx->aiScriptCursor += i;
}

/**
 * @brief Determine the true battler ID for an input battler value as
 * recognized by the AI script.
 * 
 * @param battleCtx 
 * @param inBattler The input battler value.
 * @return True battler ID for the input battler value.
 */
static u8 AIScript_Battler(BattleContext *battleCtx, u8 inBattler)
{
    // the order of this switch statement must be maintained to match
    switch (inBattler) {
    case AI_BATTLER_ATTACKER:
        return AI_CONTEXT.attacker;

    case AI_BATTLER_DEFENDER:
    default:
        return AI_CONTEXT.defender;

    case AI_BATTLER_ATTACKER_PARTNER:
        return AI_CONTEXT.attacker ^ 2;

    case AI_BATTLER_DEFENDER_PARTNER:
        return AI_CONTEXT.defender ^ 2;
    }
}

/**
 * @brief Calculate the damage that will be done by all of an attacker's moves.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param attacker      The attacker's battler ID.
 * @param moves         The attacker's moveset.
 * @param damageVals    Out-param for all damage values as computed by the routine.
 * @param heldItem      The attacker's held item.
 * @param ivs           The attacker's IVs. Used for calculating Hidden Power params.
 * @param ability       The attacker's ability.
 * @param embargoTurns  Number of turns that the attacker is still under Embargo.
 * @param varyDamage    If TRUE, apply random damage variance to each calculation.
 * @return              The highest damage value among all considered moves.
 */
static s32 TrainerAI_CalcAllDamage(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, u16 *moves, s32 *damageVals, u16 heldItem, u8 *ivs, int ability, int embargoTurns, BOOL varyDamage)
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
            if (varyDamage == TRUE) {
                damageRoll = AI_CONTEXT.moveDamageRolls[i];
            } else {
                damageRoll = 100;
            }

            damageVals[i] = TrainerAI_CalcDamage(battleSys, battleCtx, moves[i], heldItem, ivs, attacker, ability, embargoTurns, damageRoll);
        } else {
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

#include "data/battle/weight_to_power.h"

/**
 * @brief Damage calculation routine visible to the AI.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param move          The move being used
 * @param heldItem      The attacker's held item.
 * @param ivs           The attacker's IVs. Used for Hidden Power calculation.  
 * @param attacker      The attacker's ID.
 * @param ability       The attacker's ability.
 * @param embargoTurns  Number of turns that the attacker is still under Embargo.
 * @param variance      Variance factor applied to the damage value. This is presumed
 *                      to be a value in the range [85..100].
 * @return Calculated damage value.
 */
static s32 TrainerAI_CalcDamage(BattleSystem *battleSys, BattleContext *battleCtx, u16 move, u16 heldItem, u8 *ivs, int attacker, int ability, int embargoTurns, u8 variance)
{
    // must declare C89-style to match
    int defendingSide;
    int power;
    int type;
    int typeTmp;
    u32 effectivenessFlags;
    s32 damage;

    defendingSide = Battler_Side(battleSys, AI_CONTEXT.defender);
    damage = 0;
    power = 0;
    type = 0;
    effectivenessFlags = 0;

    switch (move) {
    case MOVE_NATURAL_GIFT:
        if (embargoTurns == 0) {
            power = BattleSystem_GetItemData(battleCtx, heldItem, ITEM_PARAM_NATURAL_GIFT_POWER);

            if (power) {
                type = BattleSystem_GetItemData(battleCtx, heldItem, ITEM_PARAM_NATURAL_GIFT_TYPE);
            } else {
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
        power = 0;
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
        } else {
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
    } else {
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

    if (effectivenessFlags & MOVE_STATUS_IMMUNE) {
        damage = 0;
    } else {
        damage = BattleSystem_Divide(damage * variance, 100);
    }

    return damage;
}

/**
 * @brief Compute the type of a move. Variable-type moves will have their type
 * computed according to the usual routines (i.e., Natural Gift, Judgment,
 * Hidden Power, and Weather Ball). Moves without a variable typing will be
 * returned as TYPE_NORMAL.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The battler using the move.
 * @param move      The move being used.
 * @return The type of the move.
 */
static int TrainerAI_MoveType(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int move)
{
    int result;

    switch (move) {
    case MOVE_NATURAL_GIFT:
        result = Battler_NaturalGiftType(battleCtx, battler);
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
            result = TYPE_NORMAL;
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

/**
 * @brief End-of-turn healing tick routine visible to the AI.
 * 
 * @param battleSys 
 * @param battleCtx
 * @param battler      The battler's ID.
 * @return Calculated healing tick value.
 */
static int TrainerAI_CalcEndOfTurnHealTick(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int defender1, defender2, defender;
    int i;
    u8 heldItemEffect, ability;
    int tick, totalTick;

    totalTick = 0;
    tick = 0;

    heldItemEffect = Battler_HeldItemEffect(battleCtx, battler);
    ability = Battler_Ability(battleCtx, battler);

    // Early exit if under effects of Heal Block
    if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_HEAL_BLOCK)
    {
       return totalTick;
    }

    if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
        defender1 = BATTLER_ENEMY_SLOT_1;
        defender2 = BATTLER_ENEMY_SLOT_2;
    } else {
        defender1 = BATTLER_ENEMY_SLOT_1;
        defender2 = defender1;
    }

    for (i = 0; i < 2; i++)
    {
        if (i = 0)
        {
            defender = defender1;
        }
        else
        {
            if (defender2 == defender1) {
                break;
            }
            defender = defender2;
        }

        if (battleCtx->battleMons[defender].curHP > 0)
        {
            if (battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_LEECH_SEED)
            {
                if (battleCtx->battleMons[defender].ability != ABILITY_MAGIC_GUARD)
                {
                    if (battleCtx->battleMons[defender].curHP < BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 8))
                    {
                        tick = battleCtx->battleMons[defender].curHP;
                    }
                    else
                    {
                        tick = BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 8);
                    }
                }
                if (battleCtx->battleMons[defender].ability == ABILITY_LIQUID_OOZE)
                {
                    tick = 0;
                }

                if (heldItemEffect == HOLD_EFFECT_LEECH_BOOST)
                {
                    tick = tick * 13 / 10;
                }

                totalTick += tick;
                tick = 0;
            }
        }
    }

    switch (heldItemEffect) {
        default:
            break;

        case HOLD_EFFECT_HP_RESTORE_PSN_TYPE:
            if (battleCtx->battleMons[battler].type1 != TYPE_POISON
            && battleCtx->battleMons[battler].type2 != TYPE_POISON)
            {
                break;
            }
            // Otherwise, we are poison, so use leftovers procedure below

        case HOLD_EFFECT_HP_RESTORE_GRADUAL:
            tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);

            // Max HP less than 16
            if (tick < 1) {
                tick = 1;
            }

            totalTick += tick;
            tick = 0;
            break;
    }

    if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_CASTFORM)
        && NO_CLOUD_NINE)
    {
        switch (ability) {
            default:
                break;

            case ABILITY_DRY_SKIN:
                if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING)
                {
                    tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 8);
                    if (tick < 1) {
                        tick = 1;
                    }
                }

                totalTick += tick;
                tick = 0;
                break;

            case ABILITY_RAIN_DISH:
                if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING)
                {
                    tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
                    if (tick < 1) {
                        tick = 1;
                    }
                }

                totalTick += tick;
                tick = 0;
                break;

            case ABILITY_ICE_BODY:
                if (battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING)
                {
                    tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
                    if (tick < 1) {
                        tick = 1;
                    }
                }

                totalTick += tick;
                tick = 0;
                break;

            case ABILITY_PHOTOSYNTHESIS:
                if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY)
                {
                    tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
                    if (tick < 1) {
                        tick = 1;
                    }
                }
                
                totalTick += tick;
                tick = 0;
                break;
        }
    }


    if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY_POISON)
    {
        if (ability == ABILITY_POISON_HEAL)
        {
            tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 8);
            if (tick < 1) {
                tick = 1;
            }
        }

        totalTick += tick;
        tick = 0;
    }

    return totalTick;
}


/**
 * @brief End-of-turn damage tick routine visible to the AI.
 * 
 * @param battleSys 
 * @param battleCtx
 * @param battler      The battler's ID.
 * @return Calculated damage tick value.
 */
static int TrainerAI_CalcEndOfTurnDamageTick(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int defender1, defender2, defender;
    int i;
    u8 heldItemEffect, ability;
    int tick, totalTick;

    totalTick = 0;
    tick = 0;

    heldItemEffect = Battler_HeldItemEffect(battleCtx, battler);
    ability = Battler_Ability(battleCtx, battler);

    // Early exit if battler has Magic Guard ability
    if (ability == ABILITY_MAGIC_GUARD)
    {
       return totalTick;
    }

    if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
        defender1 = BATTLER_ENEMY_SLOT_1;
        defender2 = BATTLER_ENEMY_SLOT_2;
    } else {
        defender1 = BATTLER_ENEMY_SLOT_1;
        defender2 = defender1;
    }

    for (i = 0; i < 2; i++)
    {
        if (i = 0)
        {
            defender = defender1;
        }
        else
        {
            if (defender2 == defender1) {
                break;
            }
            defender = defender2;
        }

        if (battleCtx->battleMons[defender].curHP > 0)
        {
            if (battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_LEECH_SEED)
            {
                if (battleCtx->battleMons[defender].ability != ABILITY_MAGIC_GUARD)
                {
                    if (battleCtx->battleMons[defender].curHP < BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 8))
                    {
                        tick = battleCtx->battleMons[defender].curHP;
                    }
                    else
                    {
                        tick = BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 8);
                    }
                }
                if (battleCtx->battleMons[defender].ability == ABILITY_LIQUID_OOZE)
                {
                    if (battleCtx->battleMons[defender].curHP < BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 8))
                    {
                        tick = battleCtx->battleMons[defender].curHP;
                    }
                    else
                    {
                        tick = BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 8);
                        if (tick < 1) {
                            tick = 1;
                        }
                    }
                }

                if (heldItemEffect == HOLD_EFFECT_LEECH_BOOST)
                {
                    tick = tick * 13 / 10;
                }

                totalTick += tick;
                tick = 0;
            }
        }
    }

    switch (heldItemEffect) {
        default:
            break;

        case HOLD_EFFECT_HP_RESTORE_PSN_TYPE:
            if (battleCtx->battleMons[battler].type1 != TYPE_POISON
            && battleCtx->battleMons[battler].type2 != TYPE_POISON)
            {
                tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
                if (tick < 1) {
                    tick = 1;
                }

                totalTick += tick;
                tick = 0;
            }
            break;

        case HOLD_EFFECT_DMG_USER_CONTACT_XFR:
            if (ability != ABILITY_MAGIC_GUARD)
            {
                tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 8);
                if (tick < 1) {
                    tick = 1;
                }

                totalTick += tick;
                tick = 0;
            }
            break;
    }

    if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_CASTFORM)
        && NO_CLOUD_NINE)
    {
        switch (ability) {
            default:
                break;

            case ABILITY_DRY_SKIN:
            case ABILITY_SOLAR_POWER:
                if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY)
                {
                    tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 8);
                    if (tick < 1) {
                        tick = 1;
                    }

                    totalTick += tick;
                    tick = 0;
                }
                break;
        }

        if (heldItemEffect != HOLD_EFFECT_NO_WEATHER_CHIP_POWDER) {
            if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SANDSTORM)
            {
                if (battleCtx->battleMons[battler].type1 != TYPE_ROCK
                && battleCtx->battleMons[battler].type1 != TYPE_GROUND
                && battleCtx->battleMons[battler].type1 != TYPE_STEEL
                && battleCtx->battleMons[battler].type2 != TYPE_ROCK
                && battleCtx->battleMons[battler].type2 != TYPE_GROUND
                && battleCtx->battleMons[battler].type2 != TYPE_STEEL)
                {
                    tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
                    if (tick < 1) {
                        tick = 1;
                    }
                    
                    totalTick += tick;
                    tick = 0;
                }
            }

            if (battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING
                && ability != ABILITY_ICE_BODY)
            {
                if (battleCtx->battleMons[battler].type1 != TYPE_ICE
                && battleCtx->battleMons[battler].type2 != TYPE_ICE)
                {
                    tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
                    if (tick < 1) {
                        tick = 1;
                    }

                    totalTick += tick;
                    tick = 0;
                }
            }
        }
    }

    if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_LEECH_SEED)
    {
        tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 8);
        if (tick < 1) {
            tick = 1;
        }

        totalTick += tick;
        tick = 0;
    }

    if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CHIP)
    {
        tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 8);
        if (tick < 1) {
            tick = 1;
        }

        totalTick += tick;
        tick = 0;
    }

    if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_BIND)
    {
        tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
        if (tick < 1) {
            tick = 1;
        }

        totalTick += tick;
        tick = 0;
    }

    if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CURSE)
    {
        tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 4);
        if (tick < 1) {
            tick = 1;
        }

        totalTick += tick;
        tick = 0;
    }

    if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_NIGHTMARE)
    {
        tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 4);
        if (tick < 1) {
            tick = 1;
        }

        totalTick += tick;
        tick = 0;
    }

    if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN)
    {
        tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
        if (tick < 1) {
            tick = 1;
        }

        if (ability == ABILITY_HEATPROOF)
        {
            tick /= 2;
        }

        totalTick += tick;
        tick = 0;
    }

    if ((battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP)
    && BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALL_BATTLERS_THEIR_SIDE, battler, ABILITY_BAD_DREAMS) > 0)
    {
        tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 8);
        if (tick < 1) {
            tick = 1;
        }

        totalTick += tick;
        tick = 0;
    }

    if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY_POISON)
    {
        if ((battleCtx->battleMons[battler].status & MON_CONDITION_POISON)
        && ability != ABILITY_POISON_HEAL)
        {
            tick = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 8);
            if (tick < 1) {
                tick = 1;
            }
        }

        if ((battleCtx->battleMons[battler].status & MON_CONDITION_TOXIC)
        && ability != ABILITY_POISON_HEAL)
        {
            tick = ((battleCtx->battleMons[battler].status & MON_CONDITION_TOXIC_COUNTER) >> 8);
            tick *= BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
            if (tick < 1) {
                tick = 1;
            }
        }

        if (ability == ABILITY_POISON_HEAL)
        {
            tick = 0;
        }

        totalTick += tick;
        tick = 0;
    }

    return totalTick;
}

static BOOL AI_CanCurePartyMemberStatus(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int i;
    Party *party;
    BOOL result;

    result = FALSE;

    party = BattleSystem_Party(battleSys, battler);

    for (int i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
        Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
            && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
            && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
            && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY))
        {
            if (Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_GUTS
                && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & (MON_CONDITION_SLEEP | MON_CONDITION_FREEZE | MON_CONDITION_PARALYSIS))) 
            {
                result = TRUE;
            }

            if ((Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_POISON_HEAL
                || Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_MAGIC_GUARD)
                && ((Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY_POISON) == FALSE))
            {
                result = TRUE;
            }

            if (Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_QUICK_FEET
                && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & ~MON_CONDITION_PARALYSIS)) 
            {
                result = TRUE;
            }

            if (Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_MARVEL_SCALE
                && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_INCAPACITATED)) 
            {
                result = TRUE;
            }
        }
    }

    return result;
}

static BOOL AI_CanImprisonTarget(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender)
{
    int i, j, targetPartySlot, side, maxBattlers, attackerMove, defenderMove;
    BOOL result;

    side = Battler_Side(battleSys, attacker);

    // Early exit if an ally is being considered
    if (side == Battler_Side(battleSys, defender)) {
        return FALSE;
    }

    targetPartySlot = battleCtx->selectedPartySlot[defender];

    result = FALSE;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        attackerMove = battleCtx->battleMons[attacker].moves[i];

        for (j = 0; j < LEARNED_MOVES_MAX; j++) {
            defenderMove = battleCtx->battleMons[defender].moves[j];

            if (attackerMove == defenderMove) {

                // We know the move for sure
                if (AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][targetPartySlot][j] == defenderMove) {

                    result = TRUE;
                }
                else {
                    // Cheat and peek the move 50% of the time. "Intuition"
                    if (BattleSystem_RandNext(battleSys) % 2 == 0) {
                        result = TRUE;
                    }
                }

                if (result == TRUE) {
                    break;
                }
            }
        }
    }

    return result;
}


static BOOL AI_CanMagicBounceTargetMove(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender)
{
    int i, j, targetPartySlot, side, maxBattlers, attackerMove, defenderMove, range;
    BOOL result;

    side = Battler_Side(battleSys, attacker);

    // Early exit if an ally is being considered
    if (side == Battler_Side(battleSys, defender)) {
        return FALSE;
    }

    targetPartySlot = battleCtx->selectedPartySlot[defender];

    result = FALSE;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        defenderMove = battleCtx->battleMons[defender].moves[i];
        
        if (defenderMove == MOVE_NONE) {
            break;
        }

        if (MOVE_DATA(defenderMove).class == CLASS_STATUS) {
            range = MOVE_DATA(defenderMove).range;

            // May be other applicable ranges of bounceable moves.
            if (range == RANGE_SINGLE_TARGET
                || range == RANGE_OPPONENT_SIDE
                || range == RANGE_ADJACENT_OPPONENTS) {
                
                if (AI_CONTEXT.battlerPartyMoves[AI_CONTEXT.defender][targetPartySlot][i] == defenderMove) {

                result = TRUE;
                }
                else {
                    // Cheat and peek the move 50% of the time. "Intuition"
                    if (BattleSystem_RandNext(battleSys) % 2 == 0) {
                        result = TRUE;
                    }
                }
            }
        }
    }

    return result;
}

/* @brief Check if any moves are invalid for use by the battler.
 * 
 * This routine is copied from BattleSystem_CheckInvalidMoves in battle_lib
 *
 * @param battleSys
 * @param battleCtx
 * @param battler       The AI's battler.
 * @param invalidMoves  The bitmask of invalid moves.
 * @param opMask        The bitmask of the operation to check.
 * @return  The bitmask of the current move slot to determine validity.
 */
static int AI_CheckInvalidMoves(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int invalidMoves, int opMask)
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

/* @brief Check if a move can be used by an active battler.
 * 
 * This routine is copied from BattleSystem_CanUseMove in battle_lib
 *
 * @param battleSys
 * @param battleCtx
 * @param battler       The AI's battler.
 * @param opMask        The bitmask of the operation to check.
 * @return  TRUE / FALSE if the current move in current slot can be used.
 */
static BOOL AI_CanUseMove(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int moveSlot, int opMask)
{
    BOOL result = TRUE;

    if (AI_CheckInvalidMoves(battleSys, battleCtx, battler, 0, opMask) & FlagIndex(moveSlot)) {
        result = FALSE;
    }

    return result;
}

static BOOL AI_AttackerChunksOrKOsDefender(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender)
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

        if (AI_CanUseMove(battleSys, battleCtx, attacker, k, CHECK_INVALID_ALL_BUT_TORMENT)) {

            moveType = TrainerAI_MoveType(battleSys, battleCtx, attacker, move);
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

                if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                    || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)){
                    if ((moveDamage > battleCtx->battleMons[defender].curHP)
                        || (moveDamage > (battleCtx->battleMons[defender].maxHP / 2))) {
                        result = TRUE;
                        break;
                    }
                }
            }
        }
    }

    return result;
}

static BOOL AI_DoNotStatDrop(BattleSystem *battleSys, BattleContext *battleCtx, u16 move, int attacker, int defender)
{
    BOOL result;

    u8 attackerAbility, defenderAbility;
    int effect;

    result = FALSE;

    attackerAbility = battleCtx->battleMons[attacker].ability;
    defenderAbility = battleCtx->battleMons[defender].ability;
    
    effect = MOVE_DATA(move).effect;

    if (attackerAbility != ABILITY_MOLD_BREAKER) {
        switch (defenderAbility) {
            default:
                break;

            case ABILITY_MAGIC_BOUNCE:
                if (attackerAbility != ABILITY_DEFIANT
                    && attackerAbility != ABILITY_COMPETITIVE) {
                    result = TRUE;
                }
                break;

            case ABILITY_HYPER_CUTTER:
                if (MapBattleEffectToStatDrop(battleCtx, effect) & BATTLE_STAT_FLAG_ATTACK) {
                    result = TRUE;
                }
                break;
                
            case ABILITY_DEFIANT:
            case ABILITY_COMPETITIVE:
            case ABILITY_CLEAR_BODY:
            case ABILITY_WHITE_SMOKE:
                result = TRUE;
                break;
        }
    }

    return result;
}

/**
 * @brief Check if Perish Song is active on a battler and the battler should
 * faint at the end of the turn. If so, treat the next switch as post-KO switch
 * AI.
 * 
 * This routine is bugged; it functionally does nothing. The Perish Song turn
 * count decrements at the end of the turn, so the AI never sees that it WILL
 * die to Perish Song.
 * 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @return TRUE if the AI has a switch to make, FALSE otherwise.
 */
static BOOL AI_PerishSongKO(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    if ((battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_PERISH_SONG)
            && battleCtx->battleMons[battler].moveEffectsData.perishSongTurns == 0) {
        battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
        return TRUE;
    }

    return FALSE;
}

/**
 * @brief Check if an AI's battler cannot damage the opponent's Pokemon due to
 * Wonder Guard. If so, check for any living party member that can deal damage
 * to that Pokemon, and switch to that mon 92% of the time.
 * 
 * This routine does NOT apply to double-battles.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @return TRUE if the AI has a switch to make, FALSE otherwise.
 */
static BOOL AI_CannotDamageWonderGuard(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int i, j;
    int chipDamageIdx, removeAbilityIdx;
    u16 move;
    int moveType, moveClass, moveEffect;
    u32 effectiveness;
    Pokemon *mon;

    if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
        // Sometimes we will want to switch, even in doubles.
        if (BattleSystem_RandNext(battleSys) % 5 == 0) {
            return FALSE;
        }
    }

    if (battleCtx->battleMons[BATTLER_OPP(battler)].ability == ABILITY_WONDER_GUARD) {
        // Check if we have a super-effective move against the opponent
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            move = battleCtx->battleMons[battler].moves[i];
            moveType = TrainerAI_MoveType(battleSys, battleCtx, battler, move);
            moveClass = MOVE_DATA(move).class;
            moveEffect = MOVE_DATA(move).effect;

            if (move) {
                effectiveness = 0;
                BattleSystem_ApplyTypeChart(battleSys, battleCtx, move, moveType, battler, BATTLER_OPP(battler), 0, &effectiveness);

                if (moveClass == CLASS_STATUS
                    || moveEffect == BATTLE_EFFECT_BIND_HIT
                    || moveEffect == BATTLE_EFFECT_WHIRLPOOL) {
                    for (chipDamageIdx = 0; sChipDamageMoves[chipDamageIdx] != 0xFFFF; chipDamageIdx++) {
                        if (moveEffect == sChipDamageMoves[chipDamageIdx] 
                            && (effectiveness & ~MOVE_STATUS_IMMUNE)) {
                            if (moveEffect == BATTLE_EFFECT_CURSE) {
                                if (MON_HAS_TYPE(battler, TYPE_GHOST)) {
                                    return FALSE;
                                }
                            }
                            else {
                                return FALSE;
                            }
                        }
                    }
                    for (removeAbilityIdx = 0; sRemoveAbilityMoves[removeAbilityIdx] != 0xFFFF; removeAbilityIdx++) {
                        if (moveEffect == sRemoveAbilityMoves[removeAbilityIdx]
                            && moveEffect != BATTLE_EFFECT_SWITCH_ABILITIES) {
                                return FALSE;
                        }
                    }
                }

                if (MOVE_DATA(move).power
                    && (effectiveness & MOVE_STATUS_SUPER_EFFECTIVE)) {
                    return FALSE;
                }
            }
        }

        // If we don't, check if any of our party members have a super-effective move
        for (i = 0; i < BattleSystem_PartyCount(battleSys, battler); i++) {
            mon = BattleSystem_PartyPokemon(battleSys, battler, i);

            if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                    && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                    && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                    && i != battleCtx->selectedPartySlot[battler]) {
                for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                    move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);
                    moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);
                    moveClass = MOVE_DATA(move).class;
                    moveEffect = MOVE_DATA(move).effect;

                    if (move) {
                        effectiveness = 0;
                        BattleSystem_CalcEffectiveness(battleCtx,
                            move,
                            moveType,
                            Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                            Battler_Ability(battleCtx, BATTLER_OPP(battler)),
                            Battler_HeldItemEffect(battleCtx, BATTLER_OPP(battler)),
                            BattleMon_Get(battleCtx, BATTLER_OPP(battler), BATTLEMON_TYPE_1, NULL),
                            BattleMon_Get(battleCtx, BATTLER_OPP(battler), BATTLEMON_TYPE_2, NULL),
                            &effectiveness);

                        // If this party member has a super-effective move, switch 11/12 of the time
                        if ((effectiveness & MOVE_STATUS_SUPER_EFFECTIVE) && BattleSystem_RandNext(battleSys) % 12 < 11) {
                            battleCtx->aiSwitchedPartySlot[battler] = i;
                            return TRUE;
                        }
                        // If this party member has chip damage or ability-removing move, switch 1/3 of the time
                        if (moveClass == CLASS_STATUS
                                || moveEffect == BATTLE_EFFECT_BIND_HIT
                                || moveEffect == BATTLE_EFFECT_WHIRLPOOL) {
                            for (chipDamageIdx = 0; sChipDamageMoves[chipDamageIdx] != 0xFFFF; chipDamageIdx++) {
                                if (moveEffect == sChipDamageMoves[chipDamageIdx] && (effectiveness & ~MOVE_STATUS_IMMUNE)) {
                                    if (BattleSystem_RandNext(battleSys) % 3 == 0) {
                                        if (moveEffect == BATTLE_EFFECT_CURSE) {
                                            if (MON_HAS_TYPE(battler, TYPE_GHOST)) {
                                                return TRUE;
                                            }
                                        }
                                        else {

                                        battleCtx->aiSwitchedPartySlot[battler] = i;
                                        return TRUE;
                                        }
                                    }
                                }
                            }
                            for (removeAbilityIdx = 0; sRemoveAbilityMoves[removeAbilityIdx] != 0xFFFF; removeAbilityIdx++) {
                                if (moveEffect == sRemoveAbilityMoves[removeAbilityIdx]
                                    && moveEffect != BATTLE_EFFECT_SWITCH_ABILITIES) {
                                        if (BattleSystem_RandNext(battleSys) % 3 == 0) {

                                            battleCtx->aiSwitchedPartySlot[battler] = i;
                                            return TRUE;
                                        }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return FALSE;
}

/**
 * @brief Check if an AI's battler only has moves which do not deal damage to either
 * of the opponent's Pokemon.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @return TRUE if the AI has a switch to make, FALSE otherwise.
 */
static BOOL AI_OnlyIneffectiveMoves(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int i, j, k;
    u8 defender1, defender2, defender, battlerPartner, side;
    u8 aiSlot1, aiSlot2;
    u16 move, opponentMove;
    int type, range, effect, moveEffect, moveVolatileStatus, moveStatus, partyCount;
    u32 effectiveness, sideCondition, fieldCondition;
    int start, end;
    int numMoves;
    Pokemon *mon;

    // "Player" consts here refer to the AI's perspective.
    if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
        defender1 = BATTLER_PLAYER_SLOT_1;
        defender2 = BATTLER_PLAYER_SLOT_2;
    } else {
        defender1 = BATTLER_PLAYER_SLOT_1;
        defender2 = BATTLER_PLAYER_SLOT_1;
    }


    // Early exit case if taunted into all status moves
    if(battleCtx->battleMons[battler].moveEffectsData.tauntedTurns > 0) {
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            move = battleCtx->battleMons[battler].moves[i];
            
            if (MOVE_DATA(move).class != CLASS_STATUS
                || move == MOVE_NONE) {
                break;
            }
        }

        if (i == LEARNED_MOVES_MAX) {
            battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
            return TRUE;
        }
    }
    

    // Check all of this mon's attacking moves for immunities. If any of our moves can deal damage to
    // either of the opponents' battlers, do not switch.
    numMoves = 0;

    for (j = 0; j < 2; j++) {

        effectiveness = 0;

        if (j == 0) {
            defender = defender1;
            battlerPartner = battler;
        }

        if (j == 1) {

            if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES)) {
                defender = defender2;
                battlerPartner = BattleSystem_Partner(battleSys, battler);
            }
            else {
                battlerPartner = battler;
                break;
            }
        }

        for (i = 0; i < LEARNED_MOVES_MAX; i++) {

        if (battleCtx->battleMons[battler].curHP == 0) {
            break;
        }

        move = battleCtx->battleMons[battler].moves[i];

        if (move == MOVE_NONE) {
            break;
        }

        type = TrainerAI_MoveType(battleSys, battleCtx, battler, move);
        range = MOVE_DATA(move).range;
        effect = MOVE_DATA(move).effect;

            // Check if there is a choice-locked move.
            if (battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove != MOVE_NONE) {

                // Only the choice-locked move need be considered.
                if (move == battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove) {

                    effectiveness = 0;
                    if (battleCtx->battleMons[defender].curHP) {
                        BattleSystem_ApplyTypeChart(battleSys, battleCtx, move, type, battler, defender, 0, &effectiveness);
                    }

                    if (MOVE_DATA(move).power) {

                        // Switch if the move has less than neutral effectiveness when it connects.
                        if ((effectiveness & (MOVE_STATUS_IMMUNE | MOVE_STATUS_RESISTED))
                            && ((effectiveness & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                            battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
                            return TRUE;
                        }
                        else {
                            return FALSE;
                        }
                    }
                    // Status move here
                    else {
                        moveEffect = MapBattleEffectToMoveEffect(battleCtx, effect);
                        if (moveEffect != MOVE_EFFECT_NONE) {
                            if ((battleCtx->battleMons[defender].moveEffectsMask & moveEffect) == FALSE) {
                                return FALSE;
                            }
                        }
                        moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, effect);
                        moveStatus = MapBattleEffectToStatusCondition(battleCtx, effect);
                        // For now, this code just checks if the target has no negative inflictable status
                        // that could be applied. Later on, we'll need to check that the target won't
                        // benefit from poison or burn, as well
                        if (moveVolatileStatus != VOLATILE_CONDITION_NONE) {
                            if ((battleCtx->battleMons[defender].statusVolatile & moveVolatileStatus) == FALSE) {
                                return FALSE;
                            }
                        }

                        if (moveStatus != MON_CONDITION_NONE) {
                            if ((battleCtx->battleMons[defender].status & MON_CONDITION_ANY) == FALSE) {
                                if (Battle_AbilityDetersStatus(battleSys, battleCtx, battleCtx->battleMons[defender].ability, moveStatus))
                                {
                                    return FALSE;
                                }
                            }
                        }

                        battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
                        return TRUE;
                    }
                }
            }
            else { // Otherwise, there is no choice locked move. Calculate switch as usual

                if (move) {

                    if (battleCtx->battleMons[defender].curHP) {
                        BattleSystem_ApplyTypeChart(battleSys, battleCtx, move, type, battler, defender, 0, &effectiveness);
                    }

                    // Generic boosts matter regardless of move power
                    if (AI_IsModeratelyBoosted(battleSys, battleCtx, battler)) {

                        if (battleCtx->battleMons[defender].ability != ABILITY_UNAWARE) {

                            if ((effectiveness & MOVE_STATUS_IMMUNE) == FALSE) {
                                
                                // Each non-immune move gives 90% chance not to switch
                                // when attacker is heavily boosted
                                if ((BattleSystem_RandNext(battleSys) % 10) < 9) {
                                    return FALSE;
                                }
                            }
                        }
                    }

                    if (MOVE_DATA(move).power > 1) {

                        // Attacking stat boosts only matter if we are using a regular damaging move
                        if (AI_IsHeavilyAttackingStatBoosted(battleSys, battleCtx, battler)) {

                            if (battleCtx->battleMons[defender].ability != ABILITY_UNAWARE) {

                                if ((effectiveness & MOVE_STATUS_IMMUNE) == FALSE) {
                                
                                    // Each non-immune move gives 95% chance not to switch
                                    // when attacker is heavily boosted
                                    if ((BattleSystem_RandNext(battleSys) % 20) < 19) {
                                        return FALSE;
                                    }
                                }
                            }
                        }

                        // Pivot moves only care about immunity
                        if((effect == BATTLE_EFFECT_HIT_BEFORE_SWITCH)
                            && ((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)) {

                            return FALSE;
                        }

                        if ((effectiveness & MOVE_STATUS_NOT_VERY_EFFECTIVE) == FALSE) {

                            // Move is immune but not resisted
                            if (effectiveness & MOVE_STATUS_IMMUNE) {

                                // Factor immunity ignoring for ability and items (i.e. Normal or Poison type)
                                if (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY) {

                                    // 1 in 3 chance to consider a switch for each neutral move.
                                    // Explanation of switch chances in comment block below.
                                    if ((BattleSystem_RandNext(battleSys) % 3) < 2) {

                                        return FALSE;
                                    }
                                }
                            }

                            // Move is either basic effectiveness or super effective at this point
                            else {
                                if (effect == BATTLE_EFFECT_FAIL_IF_NOT_USED_ALL_OTHER_MOVES) {
                                    return FALSE;
                                }

                                // Check that no immunity abilities or items will activate
                                if ((effectiveness & MOVE_STATUS_TYPE_RESIST_ABILITY) == FALSE)
                                {
                                    numMoves++;
                                    if ((effectiveness & MOVE_STATUS_TYPE_WEAKNESS_ABILITY)
                                    || (effectiveness & MOVE_STATUS_SUPER_EFFECTIVE)) {
                                        // Always stay in if we have a better-than-neutral hit
                                        return FALSE;
                                    }
                                    // The move should be a neutral hit at this point
                                    else {
                                        // Each neutral move gives a 2/3 chance not to switch,
                                        // or a 1/3 chance to still consider switching.
                                        // This means 33% chance to switch with 1 neutral,
                                        // 11% chance to switch with 2 neutral,
                                        // 3.7% chance to switch with 3 neutral,
                                        // and 1.2% chance to switch with 4 neutral.
                                        if ((BattleSystem_RandNext(battleSys) % 3) < 2) {

                                            return FALSE;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // In this case, we have a fixed-damage or special-damage move.
                    // Unfortunately, the bastards at GameFreak did not distinguish between fixed
                    // damage moves and variable damage moves, so we will have to use a horrible
                    // and slow move list to check!
                    if (MOVE_DATA(move).power == 1) {

                        if ((effectiveness & MOVE_STATUS_IMMUNE) == FALSE) {
                                
                            // Each non-immune move gives 95% chance not to switch
                            // when attacker is heavily boosted
                            if ((BattleSystem_RandNext(battleSys) % 20) < 19) {
                                return FALSE;
                            }

                            if ((effectiveness & MOVE_STATUS_RESISTED) == FALSE) {
                                numMoves++;
                            }
                        }
                        
                        switch (effect) {
                            default:
                                break;

                            // Bide
                            case BATTLE_EFFECT_BIDE:
                                // bide not already clicked on
                                if ((battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_BIDE) == FALSE) {
                                    // Try not to start casting Bide below 60%
                                    if (battleCtx->battleMons[battler].curHP > (battleCtx->battleMons[battler].maxHP * 3 / 5)) {
                                    // If bide can hit, don't switch
                                        if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                                            || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {
                                                    
                                                return FALSE;
                                        }
                                    }
                                }
                                // Bide is clicked on. Don't switch unless they are not attacking or
                                // they will be immune due to switch or type change
                                else {
                                    // First check if bide can hit
                                    if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                                        || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {

                                        // Don't switch if we have stored damage or the opponent is attacking
                                        if ((battleCtx->storedDamage[battler] > 0) || (MOVE_DATA(battleCtx->moveHit[battler]).power > 0)) {
                                            return FALSE;
                                        }
                                    }
                                }
                                break;
                        }
                    }

                    if (MOVE_DATA(move).class == CLASS_STATUS
                        && battleCtx->battleMons[battler].moveEffectsData.tauntedTurns == 0
                        && (AI_AttackerChunksOrKOsDefender(battleSys, battleCtx, battler, defender) == FALSE)) {
                        Party *party;
                        moveEffect = MapBattleEffectToMoveEffect(battleCtx, effect);
                        moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, effect);

                        if (effect == BATTLE_EFFECT_TRANSFER_STATUS) {
                            if ((battleCtx->battleMons[battler].status & MON_CONDITION_FREEZE) == FALSE) {
                                moveStatus = battleCtx->battleMons[battler].status;
                            }
                        }
                        else {
                            moveStatus = MapBattleEffectToStatusCondition(battleCtx, effect);
                        }

                        switch (range) {

                            case RANGE_OPPONENT_SIDE:

                                side = Battler_Side(battleSys, defender);
                                sideCondition = MapBattleEffectToSideCondition(battleCtx, effect);

                                if (sideCondition != SIDE_CONDITION_NONE) {

                                    if ((battleCtx->sideConditionsMask[side] & sideCondition) == FALSE) {
                                    
                                        if (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALL_BATTLERS_THEIR_SIDE, battler, ABILITY_MAGIC_BOUNCE) == 0) {
                                            return FALSE;
                                        }
                                    }
                                }
                                break;

                            case RANGE_USER_SIDE:

                                side = Battler_Side(battleSys, battler);
                                sideCondition = MapBattleEffectToSideCondition(battleCtx, effect);

                                if (sideCondition != SIDE_CONDITION_NONE) {

                                    if ((battleCtx->sideConditionsMask[side] & sideCondition)) {
                                    
                                        return FALSE;
                                    }
                                }

                                if ((effect == BATTLE_EFFECT_CURE_PARTY_STATUS)) {
                                        if (AI_CanCurePartyMemberStatus(battleSys, battleCtx, battler)) {
                                            return FALSE;
                                        }
                                }
                                break;

                            case RANGE_USER:
                                
                                // Imprison has a special case below
                                if (moveEffect != MOVE_EFFECT_NONE
                                    && moveEffect != MOVE_EFFECT_IMPRISON) {
                                    if ((battleCtx->battleMons[battler].moveEffectsMask & moveEffect)) {
                                    
                                        return FALSE;
                                    }
                                }

                                // Try to heal before switch out if we're low hp
                                if (battleCtx->battleMons[battler].curHP < (battleCtx->battleMons[battler].maxHP * 3 / 5)) {

                                    switch (move) {
                                        case MOVE_RECOVER:
                                        case MOVE_SOFTBOILED:
                                        case MOVE_REST:
                                        case MOVE_MILK_DRINK:
                                        case MOVE_MORNING_SUN:
                                        case MOVE_SYNTHESIS:
                                        case MOVE_MOONLIGHT:
                                        case MOVE_HEAL_ORDER:
                                        case MOVE_SLACK_OFF:
                                        case MOVE_ROOST:
                                        case MOVE_WISH:
                                            if ((BattleSystem_RandNext(battleSys) % 4) < 3) {
                                                return FALSE;
                                            }
                                            break;

                                        case MOVE_SWALLOW:
                                            if ((BattleSystem_RandNext(battleSys) % 4) < 3) {
                                                if (battleCtx->battleMons[battler].moveEffectsData.stockpileCount) {
                                                    return FALSE;
                                                }
                                            }
                                            break;

                                        default:
                                            break;
                                    }
                                }

                                switch (effect) {
                                    default:
                                        break;

                                        // Baton pass is always viable
                                    case BATTLE_EFFECT_PASS_STATS_AND_STATUS:
                                        return FALSE;
                                        break;

                                        // grudge will share destiny bond's script for now
                                    case BATTLE_EFFECT_REMOVE_ALL_PP_ON_DEFEAT:
                                        // destiny bond (may change later)
                                    case BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER:
                                        if ((BattleSystem_RandNext(battleSys) % 20) < 19) {
                                            return FALSE;
                                        }
                                        break;

                                        // protect and detect
                                    case BATTLE_EFFECT_PROTECT:
                                        if (battleCtx->battleMons[battler].moveEffectsData.protectSuccessTurns == 0) {
                                            if ((BattleSystem_RandNext(battleSys) % 3) == 0) {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                        // endure
                                    case BATTLE_EFFECT_SURVIVE_WITH_1_HP:
                                        for (k = 0; k < LEARNED_MOVES_MAX; k++) {
                                            if (battleCtx->battleMons[battler].moves[k] == MOVE_ENDEAVOR) {
                                                
                                                // endure considered a viable move if we have endeavor that
                                                // can hit the defender
                                                if (((battleCtx->battleMons[defender].type1 != TYPE_GHOST)
                                                    && (battleCtx->battleMons[defender].type2 != TYPE_GHOST))
                                                    || (Battler_HeldItemEffect(battleCtx, battler) == HOLD_EFFECT_NORMAL_HIT_GHOST)
                                                    || (battleCtx->battleMons[battler].ability == ABILITY_SCRAPPY)) {
                                                        return FALSE;
                                                }
                                            }
                                        }
                                        break;

                                        // Follow me is viable in doubles if partner is alive
                                    case BATTLE_EFFECT_MAKE_GLOBAL_TARGET:
                                        if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
                                            if (battleCtx->battleMons[battlerPartner].curHP > 0
                                                && (battler != battlerPartner)) {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                        // healing wish will need its own code later
                                    case BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON:
                                    case BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON:
                                        if (AI_CanCurePartyMemberStatus(battleSys, battleCtx, battler)) {
                                            return FALSE;
                                        }
                                        party = BattleSystem_Party(battleSys, battler);
                                        for (k = 0; k < BattleSystem_PartyCount(battleSys, battler); k++) {
                                            Pokemon *mon = Party_GetPokemonBySlotIndex(party, k);

                                            if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                                                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                                                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                                                && k != battleCtx->selectedPartySlot[battler]
                                                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) < Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL))
                                            {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                    case BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE:
                                        if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
                                            if (battleCtx->battleMons[battlerPartner].curHP > 0
                                                && (battler != battlerPartner)) {
                                                if ((battleCtx->battleMons[defender].moveEffectsMask & moveEffect) == FALSE) {
                                                    if (AI_CanImprisonTarget(battleSys, battleCtx, battler, defender)) {
                                                        return FALSE;
                                                    }
                                                }
                                            }
                                        }
                                        break;

                                        // Ingrain and defensive boosting moves would be so insanely complicated to code
                                        // for I'm not going to bother doing it right now. There will need to be a new script
                                        // that checks if the current AI mon can basically defensively boost to the point where the
                                        // current opponent cannot KO it. This factor would need to be calculated before the AI has
                                        // accumulated any/many boosts.
                                    case BATTLE_EFFECT_DEF_UP:
                                    case BATTLE_EFFECT_SP_DEF_UP:
                                    case BATTLE_EFFECT_DEF_UP_2:
                                    case BATTLE_EFFECT_SP_DEF_UP_2:
                                    case BATTLE_EFFECT_DEF_UP_DOUBLE_ROLLOUT_POWER:
                                    case BATTLE_EFFECT_SP_DEF_UP_DOUBLE_ELECTRIC_POWER:
                                    case BATTLE_EFFECT_ATK_DEF_UP:
                                    case BATTLE_EFFECT_SP_ATK_SP_DEF_UP:
                                    case BATTLE_EFFECT_DEF_SPD_UP:
                                    case BATTLE_EFFECT_EVA_UP:
                                    case BATTLE_EFFECT_EVA_UP_2:
                                    case BATTLE_EFFECT_EVA_UP_2_MINIMIZE:
                                    case BATTLE_EFFECT_ACC_UP:
                                    case BATTLE_EFFECT_ACC_UP_2:
                                    case BATTLE_EFFECT_GROUND_TRAP_USER_CONTINUOUS_HEAL:
                                    case BATTLE_EFFECT_STOCKPILE:
                                    case BATTLE_EFFECT_SET_SUBSTITUTE:
                                        break;

                                    case BATTLE_EFFECT_APPLY_MAGIC_COAT:
                                        if (AI_CanMagicBounceTargetMove(battleSys, battleCtx, battler, defender)) {
                                            // idk 2/3 chance to magic bounce if we can bounce one of their moves
                                            if ((BattleSystem_RandNext(battleSys) % 3) < 2 ) {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                    case BATTLE_EFFECT_RECYCLE:
                                        if (battleCtx->battleMons[battler].heldItem == ITEM_NONE) {
                                            if (battleCtx->recycleItem[battler] != ITEM_NONE) {
                                                // If not forced out by anything else, stay in to recycle
                                                // our item maybe 25% of the time
                                                if ((BattleSystem_RandNext(battleSys) % 4) == 0) {
                                                    return FALSE;
                                                }
                                            }
                                        }
                                        break;

                                    case BATTLE_EFFECT_HEAL_STATUS:
                                        // If not forced out by anything else, try to stay in to refresh
                                        // status 25% of the time
                                        if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY) {
                                            if ((BattleSystem_RandNext(battleSys) % 4) == 0) {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                    case BATTLE_EFFECT_DO_NOTHING:
                                        // Splash is always a joke move, so we don't need to switch in joke fights
                                        return FALSE;
                                        break;

                                    case BATTLE_EFFECT_FLEE_FROM_WILD_BATTLE:
                                        return FALSE;
                                        break;

                                    case BATTLE_EFFECT_HEAL_IN_3_TURNS:
                                        if (battleCtx->fieldConditions.wishTurns[battler] <= 0) {
                                            if ((BattleSystem_RandNext(battleSys) % 2) == 0) {
                                                return FALSE;
                                            }
                                        }
                                        break;
                                }

                                break;

                            case RANGE_USER_OR_ALLY:
                                switch (effect) {
                                    default:
                                        break;

                                        // Acupressure
                                    case BATTLE_EFFECT_RANDOM_STAT_UP_2:
                                        // Always stay in for acupressure
                                        return FALSE;
                                        break;

                                        // Aqua Ring
                                    case BATTLE_EFFECT_RESTORE_HP_EVERY_TURN:
                                        if (((battleCtx->battleMons[battler].moveEffectsMask & moveEffect) == FALSE)
                                        || (battleCtx->battleMons[battlerPartner].moveEffectsMask & moveEffect) == FALSE) {
                                            return FALSE;
                                        }
                                        break;

                                        // Magnet Rise
                                    case BATTLE_EFFECT_GIVE_GROUND_IMMUNITY:
                                        // Try to stay in to cast it on our partner if we can
                                        if ((battleCtx->battleMons[battlerPartner].moveEffectsMask & moveEffect) == FALSE) {
                                            if (((battleCtx->battleMons[battlerPartner].moveEffectsMask & moveEffect) == FALSE)
                                            && ((battleCtx->battleMons[battlerPartner].type1 == TYPE_ELECTRIC)
                                                || (battleCtx->battleMons[battlerPartner].type1 == TYPE_ROCK)
                                                || (battleCtx->battleMons[battlerPartner].type1 == TYPE_STEEL)
                                                || (battleCtx->battleMons[battlerPartner].type2 == TYPE_ELECTRIC)
                                                || (battleCtx->battleMons[battlerPartner].type2 == TYPE_ROCK)
                                                || (battleCtx->battleMons[battlerPartner].type2 == TYPE_STEEL))) {
                                                    return FALSE;
                                            }
                                        }
                                        break;
                                }
                                break;

                            case RANGE_SINGLE_TARGET_SPECIAL:

                                switch (effect) {

                                    default:
                                        break;

                                        // Mirror Move
                                    case BATTLE_EFFECT_COPY_MOVE:
                                        // Copycat
                                    case BATTLE_EFFECT_USE_LAST_USED_MOVE:
                                        // if copycat move would be neutral or better
                                        if (BattleSystem_TypeMatchupMultiplier(MOVE_DATA(battleCtx->moveHit[battler]).type, battleCtx->battleMons[defender].type1, battleCtx->battleMons[defender].type2, move) >= 40) {
                                            return FALSE;
                                        }
                                        break;

                                        // Assist
                                    case BATTLE_EFFECT_USE_RANDOM_ALLY_MOVE:

                                        partyCount = BattleSystem_PartyCount(battleSys, battler);
                                        party = BattleSystem_Party(battleSys, battler);

                                        // Basically, if we have a living teammate, consider assist viable
                                        for (k = 0; k < partyCount; k++) {
                                            Pokemon *mon = Party_GetPokemonBySlotIndex(party, k);

                                            if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                                                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                                                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                                                && k != battleCtx->selectedPartySlot[battler]
                                                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) < Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL))
                                            {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                        // Metronome
                                    case BATTLE_EFFECT_CALL_RANDOM_MOVE:
                                        // We will consider Metronome always effective
                                        return FALSE;
                                        break;

                                        // Sleep Talk
                                    case BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP:
                                        // This is actually redundant and a waste of time
                                        /*
                                        for (k = 0; k < LEARNED_MOVES_MAX; k++) {
                                            sleepTalkMove = battleCtx->battleMons[battler].moves[k];
                                            effectiveness = 0;
                                            BattleSystem_ApplyTypeChart(battleSys, battleCtx, sleepTalkMove, type, battler, defender, 0, &effectiveness);

                                            // If we have a move that hits neutral, don't switch
                                            if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                                                || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {

                                                    if ((effectiveness & MOVE_STATUS_RESISTED) == FALSE) {
                                                        return FALSE;
                                                    }
                                                }
                                        }
                                        */
                                        break;

                                        // Snatch
                                    case BATTLE_EFFECT_STEAL_STATUS_MOVE:
                                        // Snatch should only flag for not switching if it can be used to
                                        // shut down an opponent for an ally in doubles
                                        if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
                                            if (battleCtx->battleMons[battlerPartner].curHP > 0
                                                && (battler != battlerPartner)) {
                                                for (k = 0; k < LEARNED_MOVES_MAX; k++) {
                                                    opponentMove = battleCtx->battleMons[defender].moves[k];
                                                    if (MOVE_DATA(opponentMove).class == CLASS_SPECIAL) {
                                                        if (MOVE_DATA(opponentMove).flags & MOVE_FLAG_CAN_SNATCH) {
                                                            return FALSE;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        break;
                                }

                                // Me First has its own range?
                            case RANGE_SINGLE_TARGET_ME_FIRST:
                                if (battleCtx->battleMons[battler].speed >= battleCtx->battleMons[defender].speed) {
                                    return FALSE;
                                }
                                break;

                            case RANGE_SINGLE_TARGET: 

                                if (moveEffect != MOVE_EFFECT_NONE) {

                                    if ((Battler_IgnorableAbility(battleCtx, battler, defender, ABILITY_MAGIC_BOUNCE) == FALSE)) {

                                        if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                                           || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {

                                           if ((battleCtx->battleMons[defender].moveEffectsMask & moveEffect) == FALSE) {
                                                return FALSE;
                                           }
                                        }
                                    }
                                }
                                
                                if (moveVolatileStatus != VOLATILE_CONDITION_NONE) {

                                    if ((battleCtx->battleMons[defender].statusVolatile & moveVolatileStatus) == FALSE) {

                                        if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                                            || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {
                                            // For confusion and attraction, check immunity abilities
                                            if (moveVolatileStatus & (VOLATILE_CONDITION_CONFUSION | VOLATILE_CONDITION_ATTRACT)) {
                                            
                                                if (moveVolatileStatus & VOLATILE_CONDITION_CONFUSION) {

                                                    if ((Battler_IgnorableAbility(battleCtx, battler, defender, ABILITY_OWN_TEMPO) == FALSE)
                                                        && (battleCtx->battleMons[defender].ability != ABILITY_TANGLED_FEET)
                                                        && (Battler_IgnorableAbility(battleCtx, battler, defender, ABILITY_MAGIC_BOUNCE) == FALSE)) {
                                                        return FALSE;
                                                    }
                                                }

                                                if (moveVolatileStatus & VOLATILE_CONDITION_ATTRACT) {

                                                    if ((Battler_IgnorableAbility(battleCtx, battler, defender, ABILITY_OBLIVIOUS) == FALSE)
                                                        && (Battler_IgnorableAbility(battleCtx, battler, defender, ABILITY_MAGIC_BOUNCE) == FALSE)) {
                                                        return FALSE;
                                                    }
                                                }
                                            }
                                            else {
                                                if (Battler_IgnorableAbility(battleCtx, battler, defender, ABILITY_MAGIC_BOUNCE) == FALSE) {
                                                    return FALSE;
                                                }
                                            }
                                        }
                                    }
                                }

                                if (moveStatus != MON_CONDITION_NONE) {

                                    if (Battle_AbilityDetersStatus(battleSys, battleCtx, battleCtx->battleMons[defender].ability, moveStatus)
                                        && (Battler_IgnorableAbility(battleCtx, battler, defender, battleCtx->battleMons[defender].ability) == FALSE)) {

                                        if ((battleCtx->battleMons[defender].status & MON_CONDITION_ANY) == FALSE) {
                                            // Only stay in to poison if it's relevant
                                            if (moveStatus & MON_CONDITION_ANY_POISON) {
                                                if ((battleCtx->battleMons[defender].type1 != TYPE_POISON
                                                    && battleCtx->battleMons[defender].type1 != TYPE_STEEL
                                                    && battleCtx->battleMons[defender].type2 != TYPE_POISON
                                                    && battleCtx->battleMons[defender].type2 != TYPE_STEEL)
                                                    || battleCtx->battleMons[battler].ability == ABILITY_CORROSION) {
                                                    
                                                    return FALSE;
                                                }
                                            }

                                            // Only stay in to burn if it's relevant
                                            if (moveStatus & MON_CONDITION_BURN) {
                                                if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                                                    || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {

                                                    if (battleCtx->battleMons[defender].type1 != TYPE_FIRE
                                                        && battleCtx->battleMons[defender].type2 != TYPE_FIRE) {

                                                        if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender))
                                                        {
                                                            return FALSE;
                                                        }
                                                    }
                                                }
                                            }

                                            // Only stay in to paralyze if it's relevant
                                            if (moveStatus & MON_CONDITION_PARALYSIS) {
                                                if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                                                    || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {

                                                    if (battleCtx->battleMons[defender].type1 != TYPE_ELECTRIC
                                                        && battleCtx->battleMons[defender].type2 != TYPE_ELECTRIC) {

                                                        if (AI_ShouldParalyzeCheck(battleSys, battleCtx, defender, battleCtx->battleMons[battler].speed))
                                                        {
                                                            return FALSE;
                                                        }
                                                    }
                                                }
                                            }

                                            if (moveStatus & MON_CONDITION_SLEEP) {
                                                if (((effectiveness & MOVE_STATUS_IMMUNE) == FALSE)
                                                    || (effectiveness & MOVE_STATUS_IGNORE_IMMUNITY)) {
                                                    return FALSE;
                                                }
                                            }
                                        }
                                    }
                                }


                                switch (effect) {
                                    default:
                                        break;

                                    case BATTLE_EFFECT_FAINT_AND_ATK_SP_ATK_DOWN_2:
                                        if (battleCtx->battleMons[defender].ability != ABILITY_DEFIANT
                                            && battleCtx->battleMons[defender].ability != ABILITY_COMPETITIVE) {
                                                return FALSE;
                                            }
                                        break;

                                    case BATTLE_EFFECT_ATK_DEF_DOWN:

                                    case BATTLE_EFFECT_ATK_DOWN:
                                    case BATTLE_EFFECT_ATK_DOWN_2:
                                    
                                    case BATTLE_EFFECT_DEF_DOWN:
                                    case BATTLE_EFFECT_DEF_DOWN_2:

                                    case BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER:

                                    case BATTLE_EFFECT_SP_DEF_DOWN_2:
                                    
                                    case BATTLE_EFFECT_SPEED_DOWN_2:
                                    
                                    case BATTLE_EFFECT_ACC_DOWN:
                                        break;

                                    // Defog
                                    case BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN:

                                        side = Battler_Side(battleSys, battler);

                                        if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_DEFOGGABLE) {
                                            
                                            if (battleCtx->battleMons[defender].ability != ABILITY_DEFIANT
                                                && battleCtx->battleMons[defender].ability != ABILITY_COMPETITIVE) {
                                                    return FALSE;
                                            }
                                        }
                                        break;

                                    // Disable
                                    case BATTLE_EFFECT_DISABLE:

                                        if (battleCtx->battleMons[defender].ability != ABILITY_MAGIC_BOUNCE) {
                                            if (battleCtx->battleMons[defender].moveEffectsData.disabledTurns == 0) {

                                                // 10% chance to swap out
                                                if ((BattleSystem_RandNext(battleSys) % 10) < 9) {
                                                    return FALSE;
                                                }
                                            }
                                        }
                                        break;

                                    // Embargo
                                    case BATTLE_EFFECT_PREVENT_ITEM_USE:
                                        
                                        if (battleCtx->battleMons[defender].ability != ABILITY_MAGIC_BOUNCE) {
                                            if (battleCtx->battleMons[defender].moveEffectsData.embargoTurns == 0) {
                                                if (battleCtx->battleMons[defender].heldItem == ITEM_NONE
                                                    || battleCtx->recycleItem[defender] != ITEM_NONE) {

                                                    // 10% chance to swap out
                                                    if ((BattleSystem_RandNext(battleSys) % 10) < 9) {
                                                    return FALSE;
                                                    }
                                                }
                                            }
                                        }
                                        break;

                                    // Encore
                                    case BATTLE_EFFECT_ENCORE:
                                        if (battleCtx->battleMons[defender].ability != ABILITY_MAGIC_BOUNCE) {
                                            if (battleCtx->battleMons[defender].moveEffectsData.encoredTurns == 0) {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                    // Heart Swap
                                    case BATTLE_EFFECT_SWAP_STAT_CHANGES:
                                        if (AI_IsHeavilyAttackingStatBoosted(battleSys, battleCtx, defender)) {
                                            return FALSE;
                                        }
                                        if (AI_IsModeratelyBoosted(battleSys, battleCtx, defender)) {
                                            if (AI_IsModeratelyBoosted(battleSys, battleCtx, battler) == FALSE) {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                    // Mimic
                                    case BATTLE_EFFECT_COPY_MOVE_FOR_BATTLE:
                                        if (BattleSystem_TypeMatchupMultiplier(MOVE_DATA(battleCtx->moveHit[battler]).type, battleCtx->battleMons[defender].type1, battleCtx->battleMons[defender].type2, move) >= 40) {
                                            return FALSE;
                                        }
                                        break;

                                    // Pain Split
                                    case BATTLE_EFFECT_AVERAGE_HP:
                                        // Stay in if we would stand to gain more than 25% HP back
                                        if (battleCtx->battleMons[battler].curHP < (battleCtx->battleMons[defender].curHP * 2 / 3)) {
                                            return FALSE;
                                        }
                                        break;
                                        

                                    // Power Swap
                                    case BATTLE_EFFECT_SWAP_ATK_SP_ATK_STAT_CHANGES:
                                        if (AI_IsHeavilyAttackingStatBoosted(battleSys, battleCtx, defender)) {
                                            if (AI_IsHeavilyAttackingStatBoosted(battleSys, battleCtx, battler) == FALSE) {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                    // Psych Up
                                    case BATTLE_EFFECT_COPY_STAT_CHANGES:
                                        if (AI_IsHeavilyAttackingStatBoosted(battleSys, battleCtx, defender)) {
                                            if (AI_IsHeavilyAttackingStatBoosted(battleSys, battleCtx, battler) == FALSE) {
                                                return FALSE;
                                            }
                                        }
                                        if (AI_IsModeratelyBoosted(battleSys, battleCtx, defender)) {
                                            if (AI_IsModeratelyBoosted(battleSys, battleCtx, battler) == FALSE) {
                                                return FALSE;
                                            }
                                        }
                                        break;

                                    // Roar and Whirlwind
                                    case BATTLE_EFFECT_FORCE_SWITCH:
                                        if (battleCtx->battleMons[defender].ability != ABILITY_MAGIC_BOUNCE) {
                                            if (move != MOVE_ROAR || battleCtx->battleMons[defender].ability != ABILITY_SOUNDPROOF) {
                                                if (AI_AttackerChunksOrKOsDefender(battleSys, battleCtx, defender, battler) == FALSE) {
                                                    return FALSE;
                                                }
                                                break;
                                            }
                                        }
                                        break;


                                    // Sketch
                                    case BATTLE_EFFECT_LEARN_MOVE_PERMANENT:
                                        // This would probably be buggy as hell. Don't use it.
                                        break;

                                    // Worry Seed
                                    case BATTLE_EFFECT_SET_ABILITY_TO_INSOMNIA:
                                    // Skill Swap
                                    case BATTLE_EFFECT_SWITCH_ABILITIES:
                                        for (k = 0; sRemovableAbilities[k] != 0xFFFF; k++) {
                                            if (battleCtx->battleMons[defender].ability == sRemovableAbilities[k]) {
                                                return FALSE;
                                            }
                                        }
                                        break;
                                        

                                    // Spite
                                    case BATTLE_EFFECT_DECREASE_LAST_MOVE_PP:
                                        // This would be so insanely niche
                                        break;

                                    // Trick and Switcheroo
                                    case BATTLE_EFFECT_SWITCH_HELD_ITEMS:
                                        // They can't (yet), but maybe will add some rudimentary handling later
                                        break;

                                    // To Do:
                                    // Side and Field status moves.
                                }
                                break;

                            case RANGE_ADJACENT_OPPONENTS:
                                switch (effect) {
                                    default:
                                        break;

                                        // Captivate
                                    case BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER:
                                        // special gender consideration for Captivate
                                        if (battleCtx->battleMons[defender].gender != battleCtx->battleMons[battler].gender
                                            && battleCtx->battleMons[defender].gender != GENDER_NONE
                                            && battleCtx->battleMons[battler].gender != GENDER_NONE) {
                                            if (AI_DoNotStatDrop(battleSys, battleCtx, move, battler, defender) == FALSE) {

                                                // 80% chance to stay in for debuffing in doubles
                                                if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
                                                    if ((BattleSystem_RandNext(battleSys) % 5) < 4) {
                                                        return FALSE;
                                                    }
                                                }
                                                else {
                                                // 20% chance to stay in for debuffing in singles
                                                    if ((BattleSystem_RandNext(battleSys) % 5) == 0) {
                                                        return FALSE;
                                                    }
                                                }
                                            }
                                        }
                                        break;

                                        // Growl
                                    case BATTLE_EFFECT_ATK_DOWN:
                                        // special Soundproof consideration for Growl
                                        if (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALL_BATTLERS_THEIR_SIDE, battler, ABILITY_SOUNDPROOF) == 0) {
                                            if (AI_DoNotStatDrop(battleSys, battleCtx, move, battler, defender) == FALSE) {
                                                        
                                                // 80% chance to stay in for debuffing in doubles
                                                if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
                                                    if ((BattleSystem_RandNext(battleSys) % 5) < 4) {
                                                        return FALSE;
                                                    }
                                                }
                                                else {
                                                // 20% chance to stay in for debuffing in singles
                                                    if ((BattleSystem_RandNext(battleSys) % 5) == 0) {
                                                        return FALSE;
                                                    }
                                                }
                                            }
                                        }
                                        break;

                                        // String Shot
                                    case BATTLE_EFFECT_SPEED_DOWN_2:
                                        // special Trick Room consideration for String Shot
                                        if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_TRICK_ROOM) == FALSE) {
                                            if (AI_DoNotStatDrop(battleSys, battleCtx, move, battler, defender) == FALSE) {
                                                        
                                                // 80% chance to stay in for debuffing in doubles
                                                if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
                                                    if ((BattleSystem_RandNext(battleSys) % 5) < 4) {
                                                        return FALSE;
                                                    }
                                                }
                                                else {
                                                // 20% chance to stay in for debuffing in singles
                                                    if ((BattleSystem_RandNext(battleSys) % 5) == 0) {
                                                        return FALSE;
                                                    }
                                                }
                                            }
                                        }
                                        break;

                                        // Leer and Tail Whip
                                    case BATTLE_EFFECT_DEF_DOWN:
                                        // Sweet Scent
                                    case BATTLE_EFFECT_EVA_DOWN:
                                        if (AI_DoNotStatDrop(battleSys, battleCtx, move, battler, defender) == FALSE) {
                                            
                                            // 80% chance to stay in for debuffing in doubles
                                            if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
                                                if ((BattleSystem_RandNext(battleSys) % 5) < 4) {
                                                    return FALSE;
                                                }
                                            }
                                            else {
                                            // 20% chance to stay in for debuffing in singles
                                                if ((BattleSystem_RandNext(battleSys) % 5) == 0) {
                                                    return FALSE;
                                                }
                                            }
                                        }
                                        break;

                                        // Heal Block
                                    case BATTLE_EFFECT_PREVENT_HEALING:
                                        break;
                                }
                                break;

                            case RANGE_ALL_ADJACENT:
                                // Teeter Dance only
                                if ((battleCtx->battleMons[defender].statusVolatile & moveVolatileStatus) == FALSE) {
                                    return FALSE;
                                }
                                break;

                            case RANGE_ALLY:
                                // Helping Hand only
                                if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
                                    if (battleCtx->battleMons[battlerPartner].curHP > 0) {
                                        return FALSE;
                                    }
                                }
                                break;

                            case RANGE_FIELD:

                                fieldCondition = MapBattleEffectToFieldCondition(battleCtx, effect);

                                if (fieldCondition != FIELD_CONDITION_NONE) {
                                    if ((battleCtx->fieldConditionsMask & fieldCondition) == FALSE) {
                                        return FALSE;
                                    }
                                }
                                break;
                        }                        
                    }
                }
            }
        }
    }

    // If we have 1 or more neutral attacking moves, do not switch.
    if (numMoves > 0) {
        return FALSE;
    }

    aiSlot1 = battler;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG) || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        aiSlot2 = aiSlot1;
    } else {
        aiSlot2 = BattleSystem_Partner(battleSys, battler);
    }

    start = 0;
    end = BattleSystem_PartyCount(battleSys, battler);

    // For each of the AI's active party Pokemon on the bench, check if any of them have a
    // damaging move which is super-effective against either of the player's active Pokemon
    // on the battlefield. If any such Pokemon on the bench exists, switch to it 66% of
    // the time.
    for (i = start; i < end; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && i != battleCtx->selectedPartySlot[aiSlot1]
                && i != battleCtx->selectedPartySlot[aiSlot2]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot1]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot2]) {
            for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);
                type = Move_CalcVariableType(battleSys, battleCtx, mon, move);

                if (move && MOVE_DATA(move).power) {
                    effectiveness = 0;
                    if (battleCtx->battleMons[defender1].curHP) {
                        BattleSystem_CalcEffectiveness(battleCtx,
                            move,
                            type,
                            Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                            Battler_Ability(battleCtx, defender1),
                            Battler_HeldItemEffect(battleCtx, defender1),
                            BattleMon_Get(battleCtx, defender1, BATTLEMON_TYPE_1, NULL),
                            BattleMon_Get(battleCtx, defender1, BATTLEMON_TYPE_2, NULL),
                            &effectiveness);
                    }

                    if (effectiveness & MOVE_STATUS_SUPER_EFFECTIVE) {
                        if ((BattleSystem_RandNext(battleSys) % 3) < 2 ) {
                            
                            battleCtx->aiSwitchedPartySlot[battler] = i;
                            return TRUE;
                        }
                    }

                    effectiveness = 0;
                    if (battleCtx->battleMons[defender2].curHP) {
                        BattleSystem_CalcEffectiveness(battleCtx,
                            move,
                            type,
                            Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                            Battler_Ability(battleCtx, defender2),
                            Battler_HeldItemEffect(battleCtx, defender2),
                            BattleMon_Get(battleCtx, defender2, BATTLEMON_TYPE_1, NULL),
                            BattleMon_Get(battleCtx, defender2, BATTLEMON_TYPE_2, NULL),
                            &effectiveness);
                    }

                    if (effectiveness & MOVE_STATUS_SUPER_EFFECTIVE) {
                        if ((BattleSystem_RandNext(battleSys) % 3) < 2) {
                            
                            battleCtx->aiSwitchedPartySlot[battler] = i;
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    // For each of the AI's active party Pokemon on the bench, check if any of them have a
    // damaging move which is normally-effective against either of the player's active
    // Pokemon on the battlefield. If any such Pokemon on the bench exists, switch to it
    // 33% of the time.
    for (i = start; i < end; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && i != battleCtx->selectedPartySlot[aiSlot1]
                && i != battleCtx->selectedPartySlot[aiSlot2]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot1]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot2]) {
            for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);
                type = Move_CalcVariableType(battleSys, battleCtx, mon, move);

                if (move && MOVE_DATA(move).power) {
                    effectiveness = 0;
                    if (battleCtx->battleMons[defender1].curHP) {
                        BattleSystem_CalcEffectiveness(battleCtx,
                            move,
                            type,
                            Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                            Battler_Ability(battleCtx, defender1),
                            Battler_HeldItemEffect(battleCtx, defender1),
                            BattleMon_Get(battleCtx, defender1, BATTLEMON_TYPE_1, NULL),
                            BattleMon_Get(battleCtx, defender1, BATTLEMON_TYPE_2, NULL),
                            &effectiveness);
                    }

                    if (effectiveness == 0 && BattleSystem_RandNext(battleSys) % 3 == 0) {
                        battleCtx->aiSwitchedPartySlot[battler] = i;
                        return TRUE;
                    }

                    effectiveness = 0;
                    if (battleCtx->battleMons[defender2].curHP) {
                        BattleSystem_CalcEffectiveness(battleCtx,
                            move,
                            type,
                            Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                            Battler_Ability(battleCtx, defender2),
                            Battler_HeldItemEffect(battleCtx, defender2),
                            BattleMon_Get(battleCtx, defender2, BATTLEMON_TYPE_1, NULL),
                            BattleMon_Get(battleCtx, defender2, BATTLEMON_TYPE_2, NULL),
                            &effectiveness);
                    }

                    if (effectiveness == 0 && BattleSystem_RandNext(battleSys) % 3 == 0) {
                        battleCtx->aiSwitchedPartySlot[battler] = i;
                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}


/**
 * @brief Check if an AI's battler should switch out due to Yawn status.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @return TRUE if the AI's battler should switch out.
 */
static BOOL AI_ShouldSwitchYawn(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result;
    int defender;
    u8 ability;
    u8 heldItemEffect;

    result = FALSE;

    defender = BattleSystem_RandomOpponent(battleSys, battleCtx, battler);
    ability = battleCtx->battleMons[battler].ability;
    heldItemEffect = Battler_HeldItemEffect(battleCtx, battler);

    if (Battle_AbilityDetersStatus(battleSys, battleCtx, ability, MON_CONDITION_SLEEP)
        && BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_THEIR_SIDE, battler, ABILITY_MOLD_BREAKER) == 0)
    {
        // Early exit if we don't mind being being put to sleep due to ability
        return FALSE;
    }

    if (AI_AttackerChunksOrKOsDefender(battleSys, battleCtx, battler, defender))
    {
        // Early exit if we can just kill
        return FALSE;
    }

    if ((battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_YAWN)
        && (battleCtx->battleMons[battler].status & MON_CONDITION_ANY) == FALSE)
    {
        if (heldItemEffect == HOLD_EFFECT_SLP_RESTORE
        || heldItemEffect == HOLD_EFFECT_STATUS_RESTORE
        || heldItemEffect == HOLD_EFFECT_PSN_USER
        || heldItemEffect == HOLD_EFFECT_BRN_USER)
        {
            result = FALSE;
        }
        else {
            battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
            result = TRUE;
        }
    }

    return result;
}

/**
 * @brief Check if an AI's battler should switch out due to Toxic status.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @return TRUE if the AI's battler should switch out.
 */
static BOOL AI_ShouldSwitchToxic(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result;
    int defender;
    u8 ability;
    u8 heldItemEffect;

    result = FALSE;

    defender = BattleSystem_RandomOpponent(battleSys, battleCtx, battler);
    ability = battleCtx->battleMons[battler].ability;
    heldItemEffect = Battler_HeldItemEffect(battleCtx, battler);

    if (Battle_AbilityDetersStatus(battleSys, battleCtx, ability, MON_CONDITION_ANY_POISON)
        && BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_THEIR_SIDE, battler, ABILITY_MOLD_BREAKER) == 0)
    {
        // Early exit if we don't mind being poisoned due to ability
        return FALSE;
    }

    if (AI_AttackerChunksOrKOsDefender(battleSys, battleCtx, battler, defender))
    {
        // Early exit if we can just kill
        return FALSE;
    }

    if (battleCtx->battleMons[battler].status & MON_CONDITION_TOXIC)
    {
        if (battleCtx->battleMons[battler].status & (MON_CONDITION_TOXIC_COUNTER_2 | MON_CONDITION_TOXIC_COUNTER_3))
        {
            battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
            result = TRUE;
        }
    }

    return result;
}

/**
 * @brief Check if an AI's battler should switch out due to Leech Seed move effect.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @return TRUE if the AI's battler should switch out.
 */
static BOOL AI_ShouldSwitchLeechSeed(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int defender;
    int moveDamage, damageThreshold;
    u8 ability, heldItemEffect;
    u8 side;
    u16 move, moveEffect, moveType, moveClass;
    u32 effectiveness;
    int i, endOfTurnHealingTick, endOfTurnDamageTick, endOfTurnTick, protectMultiplier;
    BOOL hasProtect;

    defender = BattleSystem_RandomOpponent(battleSys, battleCtx, battler);
    ability = battleCtx->battleMons[battler].ability;
    heldItemEffect = Battler_HeldItemEffect(battleCtx, battler);
    side = Battler_Side(battleSys, defender);

    hasProtect = FALSE;

    if (Battle_AbilityDetersMoveEffect(battleSys, battleCtx, ability, MOVE_EFFECT_LEECH_SEED)
        && BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_THEIR_SIDE, battler, ABILITY_MOLD_BREAKER) == 0)
    {
        // Early exit if we don't mind being leech seeded due to ability
        return FALSE;
    }

    if (AI_AttackerChunksOrKOsDefender(battleSys, battleCtx, battler, defender))
    {
        // Early exit if we can just kill
        return FALSE;
    }

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = battleCtx->battleMons[defender].moves[i];

        if (move == MOVE_NONE) {
            break;
        }

        moveEffect = MOVE_DATA(move).effect;

        if (moveEffect == BATTLE_EFFECT_PROTECT) {
            hasProtect = TRUE;
        }
    }

    if (hasProtect) {
        protectMultiplier = 2;
    }
    else {
        protectMultiplier = 1;
    }

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = battleCtx->battleMons[battler].moves[i];
        moveEffect = MOVE_DATA(move).effect;
        moveType = TrainerAI_MoveType(battleSys, battleCtx, battler, move);
        moveClass = MOVE_DATA(move).class;
        effectiveness = 0;

        if (AI_CanUseMove(battleSys, battleCtx, battler, i, CHECK_INVALID_ALL_BUT_TORMENT)) {

            if (moveEffect == BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING
                || moveEffect == BATTLE_EFFECT_PREVENT_HEALING)
            {
                return FALSE;
            }

            if (moveClass != CLASS_STATUS) {
                moveDamage = MOVE_DATA(move).power;

                moveDamage = BattleSystem_CalcMoveDamage(battleSys,
                    battleCtx,
                    move,
                    battleCtx->sideConditionsMask[side],
                    battleCtx->fieldConditionsMask,
                    moveDamage,
                    moveType,
                    battler,
                    defender,
                    1);

                moveDamage = BattleSystem_ApplyTypeChart(battleSys,
                    battleCtx,
                    move,
                    moveType,
                    battler,
                    defender,
                    moveDamage,
                    &effectiveness);

                if ((effectiveness & MOVE_STATUS_IMMUNE)
                && ((effectiveness & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE))
                {
                    moveDamage = 0;
                }

                if (effectiveness & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                    moveDamage /= 2;
                }

                if (effectiveness & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                    moveDamage = moveDamage * 3 / 2;
                }

                endOfTurnHealingTick = TrainerAI_CalcEndOfTurnHealTick(battleSys, battleCtx, defender);
                endOfTurnDamageTick = TrainerAI_CalcEndOfTurnDamageTick(battleSys, battleCtx, defender);
            
                if (endOfTurnHealingTick <= endOfTurnDamageTick) {
                    endOfTurnTick = 0;
                }
                else {
                    endOfTurnTick = endOfTurnHealingTick - endOfTurnDamageTick;
                }

                if (moveDamage > endOfTurnTick * protectMultiplier)
                {
                    return FALSE;
                }
            }
        }
    }

    battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
    return TRUE;
}

/**
 * @brief Check if an AI's battler has a super-effective move against either of the
 * opponent's Pokemon.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @param flag      If TRUE, will always return TRUE if the AI's battler has a super-
 *                  effective move. If FALSE, returns TRUE 90% of the time for either
 *                  target against which the battler has a super-effective move.
 * @return TRUE if the AI's battler has a super-effective move.
 */
static BOOL AI_HasSuperEffectiveMove(BattleSystem *battleSys, BattleContext *battleCtx, int battler, BOOL flag)
{
    int i;
    u32 effectiveness;
    u8 defender;
    u8 oppositeSlot;
    u16 move;
    int type;

    // Look at the slot directly across from us on the opposite side. i.e.,
    // AI slot 1 looks at player slot 1, AI slot 2 looks at player slot 2
    oppositeSlot = BattleSystem_BattlerSlot(battleSys, battler) ^ 1;
    defender = BattleSystem_BattlerOfType(battleSys, oppositeSlot);

    if ((battleCtx->battlersSwitchingMask & FlagIndex(defender)) == FALSE) {
        // Check if the player's battler is weak to any of our moves
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            move = battleCtx->battleMons[battler].moves[i];
            type = TrainerAI_MoveType(battleSys, battleCtx, battler, move);

            if (move) {
                effectiveness = 0;
                BattleSystem_ApplyTypeChart(battleSys, battleCtx, move, type, battler, defender, 0, &effectiveness);

                // If the defending mon is weak to our move, return TRUE 90-100% of the time.
                if (effectiveness & MOVE_STATUS_SUPER_EFFECTIVE) {
                    if (flag) {
                        return TRUE;
                    } else if (BattleSystem_RandNext(battleSys) % 10 != 0) {
                        return TRUE;
                    }
                }
            }
        }
    }

    // Check the defender's partner the same way as above.
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) == FALSE) {
        return FALSE;
    }
    defender = BattleSystem_Partner(battleSys, defender);

    if ((battleCtx->battlersSwitchingMask & FlagIndex(defender)) == FALSE) {
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            move = battleCtx->battleMons[battler].moves[i];
            type = TrainerAI_MoveType(battleSys, battleCtx, battler, move);

            if (move) {
                effectiveness = 0;
                BattleSystem_ApplyTypeChart(battleSys, battleCtx, move, type, battler, defender, 0, &effectiveness);

                // If the defending mon is weak to our move, return TRUE 90-100% of the time.
                if (effectiveness & MOVE_STATUS_SUPER_EFFECTIVE) {
                    if (flag) {
                        return TRUE;
                    } else if (BattleSystem_RandNext(battleSys) % 10 != 0) {
                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}

/**
 * @brief Check if the AI's party has a Pokemon on the bench which has an "absorbing"
 * ability for the move which was last used on it (specifically, Volt Absorb, Water
 * Absorb, and Flash Fire).
 * 
 * This routine will skip its checks roughly 33% of the time if the AI's battler has
 * a super-effective move. It will also skip its checks if the AI's active battler
 * is the one with the absorbing ability.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @return BOOL 
 */
static BOOL AI_HasAbsorbAbilityInParty(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int i, j;
    u8 aiSlot1, aiSlot2;
    u8 moveType;
    u8 ability;
    u8 checkAbility[4];
    // u8 checkAbility, checkAbility2, checkAbility3;
    int start, end, checkAbilityCount;
    Pokemon *mon;

    // If we have a super-effective move against either opponent, do not switch ~33% of the time.
    if (AI_HasSuperEffectiveMove(battleSys, battleCtx, battler, TRUE) && BattleSystem_RandNext(battleSys) % 3 != 0) {
        return FALSE;
    }

    // If we have not been hit by a move by this battler, do not switch.
    if (battleCtx->moveHit[battler] == MOVE_NONE) {
        return FALSE;
    }

    // If the last move that hit us does not deal damage, do not switch.
    if (MOVE_DATA(battleCtx->moveHit[battler]).power == 0) {
        return FALSE;
    }

    moveType = MOVE_DATA(battleCtx->moveHit[battler]).type;
    if (moveType == TYPE_FIRE) {
        checkAbility[0] = ABILITY_FLASH_FIRE;
        checkAbilityCount = 1;
    } else if (moveType == TYPE_WATER) {
        checkAbility[0] = ABILITY_WATER_ABSORB;
        checkAbility[1] = ABILITY_STORM_DRAIN;
        checkAbility[2] = ABILITY_DRY_SKIN;
        checkAbility[3] = ABILITY_THIRSTY;
        checkAbilityCount = 4;
    } else if (moveType == TYPE_ELECTRIC) {
        checkAbility[0] = ABILITY_VOLT_ABSORB;
        checkAbility[1] = ABILITY_LIGHTNING_ROD;
        checkAbility[2] = ABILITY_MOTOR_DRIVE;
        checkAbilityCount = 3;
    } else if (moveType == TYPE_GROUND) {
        checkAbility[0] = ABILITY_LEVITATE;
        checkAbilityCount = 1;
    } else {
        return ABILITY_NONE;
    }

    // If our ability absorbs the type of the last move that hit us, do not switch.
    for (j = 0; j < checkAbilityCount; j++) {
        if (Battler_Ability(battleCtx, battler) == checkAbility[j]) {
            return FALSE;
            break;
        }
    }

    aiSlot1 = battler;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG) || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        aiSlot2 = aiSlot1;
    } else {
        aiSlot2 = BattleSystem_Partner(battleSys, battler);
    }

    start = 0;
    end = BattleSystem_PartyCount(battleSys, battler);

    // Check each Pokemon on the bench for one which has an ability that absorbs
    // the last move that was used.
    for (i = start; i < end; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && i != battleCtx->selectedPartySlot[aiSlot1]
                && i != battleCtx->selectedPartySlot[aiSlot2]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot1]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot2]) {
            ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);

            for (j = 0; j < checkAbilityCount; j++) {
                // Switch to a matching Pokemon 50% of the time.
                if (checkAbility[j] == ability && (BattleSystem_RandNext(battleSys) & 1)) {
                    battleCtx->aiSwitchedPartySlot[battler] = i;
                    return TRUE;
                    break;
                }
            }
        }
    }

    return FALSE;
}

/**
 * @brief Check if the AI has a party member with a super-effective move, constrained
 * to mons with a certain effectiveness matchup against the move that last hit us.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler               The AI's battler.
 * @param checkEffectiveness    The desired effectiveness mask against the last move.
 * @param rand                  Random odds to switch, if conditions are met.
 * @return TRUE if the AI should switch, FALSE if not.
 */
static BOOL AI_HasPartyMemberWithSuperEffectiveMove(BattleSystem *battleSys, BattleContext *battleCtx, int battler, u32 checkEffectiveness, u8 rand)
{
    int i, j;
    u8 aiSlot1, aiSlot2;
    u16 move;
    int moveType;
    u32 effectiveness;
    int start, end;
    Pokemon *mon;

    if (battleCtx->moveHit[battler] == MOVE_NONE || battleCtx->moveHitBattler[battler] == BATTLER_NONE) {
        return FALSE;
    }

    // If the last move that hit us is a status move, do not switch.
    if (MOVE_DATA(battleCtx->moveHit[battler]).power == 0) {
        return FALSE;
    }

    aiSlot1 = battler;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG) || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        aiSlot2 = aiSlot1;
    } else {
        aiSlot2 = BattleSystem_Partner(battleSys, battler);
    }

    start = 0;
    end = BattleSystem_PartyCount(battleSys, battler);

    for (i = start; i < end; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && i != battleCtx->selectedPartySlot[aiSlot1]
                && i != battleCtx->selectedPartySlot[aiSlot2]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot1]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot2]) {
            effectiveness = 0;
            moveType = TrainerAI_MoveType(battleSys, battleCtx, battleCtx->moveHitBattler[battler], battleCtx->moveHit[battler]);

            BattleSystem_CalcEffectiveness(battleCtx,
                battleCtx->moveHit[battler],
                moveType,
                Battler_Ability(battleCtx, battleCtx->moveHitBattler[battler]),
                Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_HOLD_EFFECT),
                Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL),
                Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL),
                &effectiveness);

            if (effectiveness & checkEffectiveness) {
                for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                    move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);

                    if (move == MOVE_NONE) {
                        break;
                    }

                    moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);

                    if (move) {
                        effectiveness = 0;
                        BattleSystem_CalcEffectiveness(battleCtx,
                            move,
                            moveType,
                            Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                            Battler_Ability(battleCtx, battleCtx->moveHitBattler[battler]),
                            Battler_HeldItemEffect(battleCtx, battleCtx->moveHitBattler[battler]),
                            BattleMon_Get(battleCtx, battleCtx->moveHitBattler[battler], BATTLEMON_TYPE_1, NULL),
                            BattleMon_Get(battleCtx, battleCtx->moveHitBattler[battler], BATTLEMON_TYPE_2, NULL),
                            &effectiveness);

                        if ((effectiveness & MOVE_STATUS_SUPER_EFFECTIVE) && BattleSystem_RandNext(battleSys) % rand == 0) {
                            battleCtx->aiSwitchedPartySlot[battler] = i;
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    return FALSE;
}


static BOOL AI_TargetHasRelevantContactAbility(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    u8 ability, defenderAbility, type1, type2, gender, defenderGender;
    u16 move;
    int i, moveClass;
    int hasPhysicalMove;


    ability = battleCtx->battleMons[battler].ability;
    defenderAbility = battleCtx->battleMons[BATTLER_OPP(battler)].ability;
    type1 = battleCtx->battleMons[battler].type1;
    type2 = battleCtx->battleMons[battler].type2;
    gender = battleCtx->battleMons[battler].gender;
    defenderGender = battleCtx->battleMons[BATTLER_OPP(battler)].gender;

    if (Battler_HeldItemEffect(battleCtx, battler) == HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH) {

        return FALSE;
    }

    if (defenderAbility == ABILITY_STATIC
        || defenderAbility == ABILITY_ROUGH_SKIN
        || defenderAbility == ABILITY_EFFECT_SPORE
        || defenderAbility == ABILITY_POISON_POINT
        || defenderAbility == ABILITY_FLAME_BODY
        || defenderAbility == ABILITY_CUTE_CHARM
        || defenderAbility == ABILITY_FRESH_MILK
        || defenderAbility == ABILITY_FREE_SAMPLE
        || defenderAbility == ABILITY_SHAKEDOWN
        || defenderAbility == ABILITY_COTTON_DOWN) {

        if (defenderAbility == ABILITY_STATIC) {
            
            if (type1 == TYPE_ELECTRIC || type2 == TYPE_ELECTRIC) {

                return FALSE;
            }
        }
        else if (defenderAbility == ABILITY_FLAME_BODY) {

            if (type1 == TYPE_FIRE || type2 == TYPE_FIRE) {

                return FALSE;
            }
        }
        else if (defenderAbility == ABILITY_POISON_POINT) {

            if (type1 == TYPE_POISON || type2 == TYPE_POISON
                || type1 == TYPE_STEEL || type2 == TYPE_STEEL
                || ability == ABILITY_POISON_HEAL) {

                    return FALSE;
            }
        }
        else if (defenderAbility == ABILITY_CUTE_CHARM
            || defenderAbility == ABILITY_FRESH_MILK) {

            if (gender == GENDER_NONE
                || defenderGender == GENDER_NONE
                || (gender == defenderGender)) {

                return FALSE;
            }
        }
        else if (defenderAbility == ABILITY_FREE_SAMPLE) {

            if (battleCtx->battleMons[battler].heldItem == ITEM_NONE) {

                return FALSE;
            }
        }
        else if (defenderAbility == ABILITY_COTTON_DOWN) {

            if (battleCtx->fieldConditionsMask & FIELD_CONDITION_TRICK_ROOM) {

                return FALSE;
            }
            if (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SPEED] < 4) {

                return FALSE;
            }
        }
        else if (ability == ABILITY_MAGIC_GUARD) {

            if (defenderAbility == ABILITY_ROUGH_SKIN
                || defenderAbility == ABILITY_POISON_POINT) {

                return FALSE;
            }
            if (defenderAbility == ABILITY_FLAME_BODY) {

                hasPhysicalMove = 0;

                for (i = 0; i < LEARNED_MOVES_MAX; i++) {

                    move = battleCtx->battleMons[battler].moves[i];
                    moveClass = MOVE_DATA(move).class;

                    if (moveClass == CLASS_PHYSICAL) {

                        hasPhysicalMove = 1;
                        break;
                    }
                }

                if (hasPhysicalMove == 0) {

                    return FALSE;
                }
            }
        }
        else {

            return TRUE;
        }
    }
    else {

        return FALSE;
    }

    return FALSE;
}

/**
 * @brief Check if the AI has a party member with a super-effective move, constrained
 * to mons with a certain effectiveness matchup against the move that last hit us.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler               The AI's battler.
 * @param checkEffectiveness    The desired effectiveness mask against the last move.
 * @param rand                  Random odds to switch, if conditions are met.
 * @return TRUE if the AI should switch, FALSE if not.
 */
 /*
static BOOL AI_HasPartyMemberWithHigherSpeed(BattleSystem *battleSys, BattleContext *battleCtx, int battler, u32 checkEffectiveness, u8 rand)
{
    int i, j;
    u8 aiSlot1, aiSlot2;
    u16 move;
    int moveType;
    u32 effectiveness;
    int start, end;
    Pokemon *mon;

    if (battleCtx->moveHit[battler] == MOVE_NONE || battleCtx->moveHitBattler[battler] == BATTLER_NONE) {
        return FALSE;
    }

    // If the last move that hit us is a status move, do not switch.
    if (MOVE_DATA(battleCtx->moveHit[battler]).power == 0) {
        return FALSE;
    }

    aiSlot1 = battler;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG) || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        aiSlot2 = aiSlot1;
    } else {
        aiSlot2 = BattleSystem_Partner(battleSys, battler);
    }

    start = 0;
    end = BattleSystem_PartyCount(battleSys, battler);

    for (i = start; i < end; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && i != battleCtx->selectedPartySlot[aiSlot1]
                && i != battleCtx->selectedPartySlot[aiSlot2]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot1]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot2]) {
            effectiveness = 0;
            moveType = TrainerAI_MoveType(battleSys, battleCtx, battleCtx->moveHitBattler[battler], battleCtx->moveHit[battler]);

            BattleSystem_CalcEffectiveness(battleCtx,
                battleCtx->moveHit[battler],
                moveType,
                Battler_Ability(battleCtx, battleCtx->moveHitBattler[battler]),
                Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_HOLD_EFFECT),
                Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL),
                Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL),
                &effectiveness);

            if (effectiveness & checkEffectiveness) {
                for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                    move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);
                    moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);

                    if (move) {
                        effectiveness = 0;
                        BattleSystem_CalcEffectiveness(battleCtx,
                            move,
                            moveType,
                            Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL),
                            Battler_Ability(battleCtx, battleCtx->moveHitBattler[battler]),
                            Battler_HeldItemEffect(battleCtx, battleCtx->moveHitBattler[battler]),
                            BattleMon_Get(battleCtx, battleCtx->moveHitBattler[battler], BATTLEMON_TYPE_1, NULL),
                            BattleMon_Get(battleCtx, battleCtx->moveHitBattler[battler], BATTLEMON_TYPE_2, NULL),
                            &effectiveness);

                        if ((effectiveness & MOVE_STATUS_SUPER_EFFECTIVE) && BattleSystem_RandNext(battleSys) % rand == 0) {
                            battleCtx->aiSwitchedPartySlot[battler] = i;
                            return TRUE;
                        }
                    }
                }
            }
        }
    }

    return FALSE;
}
*/

/**
 * @brief Check if the AI's battler is asleep and has Natural Cure + an eligible switch.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's battler.
 * @return TRUE if the AI should switch, FALSE otherwise.
 */
static BOOL AI_IsAsleepWithNaturalCure(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    // Don't switch if we aren't asleep, don't have Natural Cure, or are below 40% HP.
    if ((battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) == FALSE
            || Battler_Ability(battleCtx, battler) != ABILITY_NATURAL_CURE
            || battleCtx->battleMons[battler].curHP < (battleCtx->battleMons[battler].maxHP * 2 / 5)) {
        return FALSE;
    }

    // Check for the move that last hit you; i.e., don't switch on your first turn.
    // Switch 50% of the time, and use post-KO switch logic.
    if (battleCtx->moveHit[battler] == MOVE_NONE && (BattleSystem_RandNext(battleSys) & 1)) {
        battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
        return TRUE;
    }

    // If the last move that hit you is a status move, switch 50% of the time, following
    // post-KO switch logic.
    if (MOVE_DATA(battleCtx->moveHit[battler]).power == 0 && (BattleSystem_RandNext(battleSys) & 1)) {
        battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
        return TRUE;
    }

    // If we have a party member with an immunity to the last move that also has a super-effective
    // move, switch 50% of the time.
    if (AI_HasPartyMemberWithSuperEffectiveMove(battleSys, battleCtx, battler, MOVE_STATUS_INEFFECTIVE, 1)) {
        return TRUE;
    }

    // If we have a party member with a resistance to the last move that also has a super-effective
    // move, switch 50% of the time.
    if (AI_HasPartyMemberWithSuperEffectiveMove(battleSys, battleCtx, battler, MOVE_STATUS_NOT_VERY_EFFECTIVE, 1)) {
        return TRUE;
    }

    // Randomly switch 50% of the time, following post-KO switch logic.
    if (BattleSystem_RandNext(battleSys) & 1) {
        battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
        return TRUE;
    }

    return FALSE;
}

/**
 * @brief Check if the AI's current battler is heavily stat-boosted (that is,
 * if the sum of its total positive stat stage changes is greater than or
 * equal to 4.)
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's current battler.
 * @return          TRUE if the AI has a high number of positive stat stages;
 *                  FALSE otherwise.
 */
static BOOL AI_IsHeavilyStatBoosted(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int stat;
    u8 numBoosts = 0;

    for (stat = BATTLE_STAT_HP; stat < BATTLE_STAT_MAX; stat++) {
        if (battleCtx->battleMons[battler].statBoosts[stat] > 6) {
            numBoosts += battleCtx->battleMons[battler].statBoosts[stat] - 6;
        }
    }

    return numBoosts >= 4;
}

/**
 * @brief Check if the AI's current battler is heavily attacking
 * stat-boosted (that is if the sum of its total positive stat stage 
 * changes to Attack and Special Attack is greater than or equal to 4.)
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's current battler.
 * @return          TRUE if the AI has a high number of positive stat stages;
 *                  FALSE otherwise.
 */
static BOOL AI_IsHeavilyAttackingStatBoosted(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int stat, i;
    u8 numAttackingBoosts;
	numAttackingBoosts = 0;
    BOOL hasPhysicalMove, hasSpecialMove;

    hasPhysicalMove = FALSE;
    hasSpecialMove = FALSE;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {

        if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_PHYSICAL) {
            
            if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect != BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING) {
                hasPhysicalMove = TRUE;
            }
        }

        if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_SPECIAL) {
            hasSpecialMove = TRUE;
        }

        if (hasPhysicalMove && hasSpecialMove) {
            break;
        }
    }

    for (stat = BATTLE_STAT_HP; stat < BATTLE_STAT_MAX; stat++) {

        if (stat == BATTLE_STAT_ATTACK) {

            if (hasPhysicalMove) {

                if (battleCtx->battleMons[battler].statBoosts[stat] > 6) {

                    numAttackingBoosts += battleCtx->battleMons[battler].statBoosts[stat] - 6;
                }
            }
        }

        if (stat == BATTLE_STAT_SP_ATTACK) {

            if (hasSpecialMove) {

                if (battleCtx->battleMons[battler].statBoosts[stat] > 6) {
                    numAttackingBoosts += battleCtx->battleMons[battler].statBoosts[stat] - 6;
                }
            }
        }
    }

    return numAttackingBoosts >= 4;
}

/**
 * @brief Check if the AI's current battler is moderately
 * stat-boosted (that is if the sum of its total positive stat stage 
 * changes to Attack and Special Attack is greater than or equal to 2.)
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The AI's current battler.
 * @return          TRUE if the AI has a high number of positive stat stages;
 *                  FALSE otherwise.
 */
static BOOL AI_IsModeratelyBoosted(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int stat, i;
    u8 numBoosts;
	numBoosts = 0;
    BOOL hasPhysicalMove, hasSpecialMove;

    hasPhysicalMove = FALSE;
    hasSpecialMove = FALSE;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {

        if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_PHYSICAL) {
            
            if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).effect != BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING) {
                hasPhysicalMove = TRUE;
            }
        }

        if (MOVE_DATA(battleCtx->battleMons[battler].moves[i]).class == CLASS_SPECIAL) {
            hasSpecialMove = TRUE;
        }

        if (hasPhysicalMove && hasSpecialMove) {
            break;
        }
    }

    for (stat = BATTLE_STAT_HP; stat < NUM_BOOSTABLE_STATS; stat++) {

        if (stat == BATTLE_STAT_ATTACK) {

            if (hasPhysicalMove) {

                if (battleCtx->battleMons[battler].statBoosts[stat] > 6) {

                    numBoosts += battleCtx->battleMons[battler].statBoosts[stat] - 6;
                }
            }
        }

        if (stat == BATTLE_STAT_SP_ATTACK) {

            if (hasSpecialMove) {

                if (battleCtx->battleMons[battler].statBoosts[stat] > 6) {
                    numBoosts += battleCtx->battleMons[battler].statBoosts[stat] - 6;
                }
            }
        }

        if (stat == BATTLE_STAT_DEFENSE || stat == BATTLE_STAT_SP_DEFENSE || stat == BATTLE_STAT_EVASION) {
            numBoosts += battleCtx->battleMons[battler].statBoosts[stat] - 6;
        }
    }

    return numBoosts >= 2;
}

static BOOL AI_ShouldSwitchWeatherSetter(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int i, pivotMoves, moveEffect, moveType, hpRange, switchTurn;
    u8 ability, heldItemEffect;
    u16 move;
    u32 desiredFieldCondition, effectiveness;
    int moveSetter, defenderContactAbility, hasNonContactPivot;

    ability = battleCtx->battleMons[battler].ability;
    heldItemEffect = Battler_HeldItemEffect(battleCtx, battler);
    moveSetter = 0;
    desiredFieldCondition = 0;

    if (heldItemEffect == HOLD_EFFECT_NONE
    && battleCtx->recycleItem[battler] != ITEM_NONE) {

        heldItemEffect = BattleSystem_GetItemData(battleCtx, battleCtx->recycleItem[battler], ITEM_PARAM_HOLD_EFFECT);
    }

    if (ability == ABILITY_SAND_STREAM) {
        desiredFieldCondition = FIELD_CONDITION_SANDSTORM;
    }
    else if (ability == ABILITY_DRIZZLE) {
        desiredFieldCondition = FIELD_CONDITION_RAINING;
    }
    else if (ability == ABILITY_DROUGHT) {
        desiredFieldCondition = FIELD_CONDITION_SUNNY;
    }
    else if (ability == ABILITY_SNOW_WARNING) {
        desiredFieldCondition = FIELD_CONDITION_HAILING;
    }
    else {
        desiredFieldCondition = 0;
    }

    if (heldItemEffect == HOLD_EFFECT_EXTEND_RAIN ||
        heldItemEffect == HOLD_EFFECT_EXTEND_SUN ||
        heldItemEffect == HOLD_EFFECT_EXTEND_SANDSTORM ||
        heldItemEffect == HOLD_EFFECT_EXTEND_HAIL) {

        for (i = 0; i < LEARNED_MOVES_MAX; i++) {

            move = battleCtx->battleMons[battler].moves[i];
            moveEffect = MOVE_DATA(move).effect;

            if (moveEffect == BATTLE_EFFECT_WEATHER_SANDSTORM
                && heldItemEffect == HOLD_EFFECT_EXTEND_SANDSTORM) {

                    desiredFieldCondition = FIELD_CONDITION_SANDSTORM;
                    moveSetter = 1;
                    break;
            }
            if (moveEffect == BATTLE_EFFECT_WEATHER_RAIN
                && heldItemEffect == HOLD_EFFECT_EXTEND_RAIN) {

                    desiredFieldCondition = FIELD_CONDITION_RAINING;
                    moveSetter = 1;
                    break;
            }
            if (moveEffect == BATTLE_EFFECT_WEATHER_SUN
                && heldItemEffect == HOLD_EFFECT_EXTEND_SUN) {

                    desiredFieldCondition = FIELD_CONDITION_SUNNY;
                    moveSetter = 1;
                    break;
            }
            if (moveEffect == BATTLE_EFFECT_WEATHER_HAIL
                && heldItemEffect == HOLD_EFFECT_EXTEND_HAIL) {

                    desiredFieldCondition = FIELD_CONDITION_HAILING;
                    moveSetter = 1;
                    break;
            }
        }
    }

    if (desiredFieldCondition) {

        switchTurn = battleCtx->battleMons[battler].moveEffectsData.fakeOutTurnNumber;
        switchTurn += BattleSystem_RandNext(battleSys) % 2;
        switchTurn += (BattleSystem_RandNext(battleSys) % 2) * (BattleSystem_RandNext(battleSys) % 2);
        switchTurn += (BattleSystem_RandNext(battleSys) % 2) * (BattleSystem_RandNext(battleSys) % 2) * (BattleSystem_RandNext(battleSys) % 2);

        if (battleCtx->totalTurns >= switchTurn) {

            pivotMoves = 0;
            effectiveness = 0;
            hasNonContactPivot = 0;

            for (i = 0; i < LEARNED_MOVES_MAX; i++) {

                move = battleCtx->battleMons[battler].moves[i];
                moveEffect = MOVE_DATA(move).effect;
                moveType = TrainerAI_MoveType(battleSys, battleCtx, battler, move);

                if (moveEffect == BATTLE_EFFECT_HIT_BEFORE_SWITCH) {

                    BattleSystem_ApplyTypeChart(battleSys, battleCtx, move, moveType, battler, BATTLER_OPP(battler), 0, &effectiveness);

                    // Only count pivot moves that actually hit.
                    // i.e., ignore Volt Switch vs. Ground and Volt Absorb / Lightning Rod / Motor Drive
                    if ((effectiveness & MOVE_STATUS_IMMUNE) == FALSE) {
                    
                        if ((MOVE_DATA(move).flags & MOVE_FLAG_MAKES_CONTACT) == FALSE) {

                            hasNonContactPivot = 1;
                        }
                        pivotMoves++;
                    }
                }

                if (moveEffect == BATTLE_EFFECT_FLEE_FROM_WILD_BATTLE) {
                    pivotMoves++;
                    hasNonContactPivot = 1;
                }
            }

            hpRange = (battleCtx->battleMons[battler].maxHP * 2 / 5) + (((battleCtx->battleMons[battler].maxHP / 10) * (BattleSystem_RandNext(battleSys) % 11)) / 10);

            if (moveSetter) {
                // We set field condition with move and our field condition is up
                if (battleCtx->fieldConditionsMask & desiredFieldCondition) {

                    if (pivotMoves > 0) {

                        if (BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, battler, BATTLER_OPP(battler), TRUE) == COMPARE_SPEED_FASTER) {

                            // Hard switch 2/3 the time if target has negative contact ability
                            if (AI_TargetHasRelevantContactAbility(battleSys, battleCtx, battler)
                                && (hasNonContactPivot == 0)
                                && ((BattleSystem_RandNext(battleSys) % 3) != 0)) {

                                battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
                                return TRUE;
                            }
                            else {

                                return FALSE;
                            }
                        }
                        else {

                            // Hard switch if our weathermon is at or below a random percent from 40 - 50%
                            if (battleCtx->battleMons[battler].curHP <= hpRange) {

                                battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
                                return TRUE;
                            }
                            else {

                                return FALSE;
                            }
                        }
                    }
                    else {

                        battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
                        return TRUE;
                    }
                }
                // At this point, we are a move setter but our effect is not up, so we shouldn't switch
                else {
                    
                    return FALSE;
                }
            }
            // We are an ability setter at this point
            else {
                // If we have a pivot move like U-turn or Volt Switch, we should
                // deprioritize a hard switch when we are faster
                if (pivotMoves > 0) {
            
                    if (BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, battler, BATTLER_OPP(battler), TRUE) == COMPARE_SPEED_FASTER) {

                        // Hard switch 2/3 the time if target has negative contact ability
                        if (AI_TargetHasRelevantContactAbility(battleSys, battleCtx, battler)
                            && (hasNonContactPivot == 0)
                            && ((BattleSystem_RandNext(battleSys) % 3) != 0)) {

                            battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
                            return TRUE;
                        }
                        else {

                            return FALSE;
                        }
                    }
                    else {

                        // Hard switch if our weathermon is at or below 40 - 50%
                        if (battleCtx->battleMons[battler].curHP <= hpRange) {
                    
                            battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
                            return TRUE;
                        }
                        else {

                            return FALSE;
                        }
                    }
                }
                else {

                    battleCtx->aiSwitchedPartySlot[battler] = BattleAI_HotSwitchIn(battleSys, battler);
                    return TRUE;
                }
            }
        }
    }

    return FALSE;
}

static BOOL AI_ShouldSwitchWeatherDependent(BattleSystem *battleSys, BattleContext *battleCtx, int battler) {
    
    int i, j, partyCount, moveEffect, desiredMoveEffect;
    int moveSetter;
    u8 ability, desiredWeatherAbility, heldItemEffect, desiredWeatherItemEffect;
    u16 move;
    u32 abilityFieldCondition, fieldConditions;
    Pokemon *mon;

    partyCount = BattleSystem_PartyCount(battleSys, battler);
    ability = battleCtx->battleMons[battler].ability;
    desiredWeatherAbility = ABILITY_NONE;
    abilityFieldCondition = 0;   
    heldItemEffect = Battler_HeldItemEffect(battleCtx, battler);
    moveSetter = 0;
    desiredWeatherItemEffect = 0;
    desiredMoveEffect = 0;

    if (heldItemEffect == HOLD_EFFECT_NONE
        && battleCtx->recycleItem[battler] != ITEM_NONE) {
        heldItemEffect = BattleSystem_GetItemData(battleCtx, battleCtx->recycleItem[battler], ITEM_PARAM_HOLD_EFFECT);
    }

    if (ability == ABILITY_SAND_FORCE
        || ability == ABILITY_SAND_VEIL) {

        abilityFieldCondition = FIELD_CONDITION_SANDSTORM;
        desiredWeatherAbility = ABILITY_SAND_STREAM;
        desiredWeatherItemEffect = HOLD_EFFECT_EXTEND_SANDSTORM;
        desiredMoveEffect = BATTLE_EFFECT_WEATHER_SANDSTORM;
    }
    else if (ability == ABILITY_SWIFT_SWIM
        || ability == ABILITY_RAIN_DISH
        || ability == ABILITY_DRY_SKIN
        || ability == ABILITY_HYDRATION) {

        abilityFieldCondition = FIELD_CONDITION_RAINING;
        desiredWeatherAbility = ABILITY_DRIZZLE;
        desiredWeatherItemEffect = HOLD_EFFECT_EXTEND_RAIN;
        desiredMoveEffect = BATTLE_EFFECT_WEATHER_RAIN;
    }
    else if (ability == ABILITY_CHLOROPHYLL
		|| ability == ABILITY_CHLOROPLAST
        || ability == ABILITY_PHOTOSYNTHESIS
        || ability == ABILITY_SOLAR_POWER
        || ability == ABILITY_LEAF_GUARD
        || ability == ABILITY_FLOWER_GIFT) {

        abilityFieldCondition = FIELD_CONDITION_SUNNY;
        desiredWeatherAbility = ABILITY_DROUGHT;
        desiredWeatherItemEffect = HOLD_EFFECT_EXTEND_SUN;
        desiredMoveEffect = BATTLE_EFFECT_WEATHER_SUN;
    }
    else if (ability == ABILITY_ICE_BODY
        || ability == ABILITY_SNOW_CLOAK
        || ability == ABILITY_SLUSH_RUSH) {

        abilityFieldCondition = FIELD_CONDITION_HAILING;
        desiredWeatherAbility = ABILITY_SNOW_WARNING;
        desiredWeatherItemEffect = HOLD_EFFECT_EXTEND_HAIL;
        desiredMoveEffect = BATTLE_EFFECT_WEATHER_HAIL;
    }
    else if (ability == ABILITY_FORECAST) {
        
        abilityFieldCondition = FIELD_CONDITION_CASTFORM;
    }
    else {
        abilityFieldCondition = 0;
        desiredWeatherItemEffect = 0;
        desiredMoveEffect = 0;
    }

    if (heldItemEffect == HOLD_EFFECT_EXTEND_RAIN ||
    heldItemEffect == HOLD_EFFECT_EXTEND_SUN ||
    heldItemEffect == HOLD_EFFECT_EXTEND_SANDSTORM ||
    heldItemEffect == HOLD_EFFECT_EXTEND_HAIL) {

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {

        move = battleCtx->battleMons[battler].moves[i];
        moveEffect = MOVE_DATA(move).effect;

        if (moveEffect == BATTLE_EFFECT_WEATHER_SANDSTORM
            && heldItemEffect == HOLD_EFFECT_EXTEND_SANDSTORM) {

                moveSetter = 1;
                break;
        }
        if (moveEffect == BATTLE_EFFECT_WEATHER_RAIN
            && heldItemEffect == HOLD_EFFECT_EXTEND_RAIN) {

                moveSetter = 1;
                break;
        }
        if (moveEffect == BATTLE_EFFECT_WEATHER_SUN
            && heldItemEffect == HOLD_EFFECT_EXTEND_SUN) {

                moveSetter = 1;
                break;
        }
        if (moveEffect == BATTLE_EFFECT_WEATHER_HAIL
            && heldItemEffect == HOLD_EFFECT_EXTEND_HAIL) {

                moveSetter = 1;
                break;
        }
    }
}

    if (desiredWeatherAbility) {

        // Don't switch if the field condition we need is active
        if (battleCtx->fieldConditionsMask & abilityFieldCondition) {

            return FALSE;
        }
        // The field condition we need is not active
        else {

            if (moveSetter) {

                return FALSE;
            }
            
            for (i = 0; i < partyCount; i++) {

                mon = BattleSystem_PartyPokemon(battleSys, battler, i);
                heldItemEffect = BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_HOLD_EFFECT);

                // Only consider alive teammates
                if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                    && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                    && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                    && i != battleCtx->selectedPartySlot[battler]) {

                    // If our weather setter is alive, we should consider switching
                    if (Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == desiredWeatherAbility) {

                        // These abilities don't care that much, so they only switch sometimes
                        if (ability == ABILITY_SAND_FORCE
                            || ability == ABILITY_SAND_VEIL
                            || ability == ABILITY_SNOW_CLOAK) {

                            if ((battleCtx->battleMons[battler].curHP > (battleCtx->battleMons[battler].maxHP / 2))
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK] < 7)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_ATTACK] < 7)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_DEFENSE] < 7)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_DEFENSE] < 7)) 
                            {
                                if ((BattleSystem_RandNext(battleSys) % 6) == 0) {
                                    
                                    battleCtx->aiSwitchedPartySlot[battler] = i;
                                    return TRUE;
                                }
                            }
                        }
                        else {
                            // Don't switch if heavily boosted
                            if ((battleCtx->battleMons[battler].curHP > (battleCtx->battleMons[battler].maxHP / 4))
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK] < 8)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_ATTACK] < 8)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_DEFENSE] < 8)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_DEFENSE] < 8)) {
                            
                                battleCtx->aiSwitchedPartySlot[battler] = i;
                                return TRUE;
                            }
                        }
                    }

                    if (heldItemEffect == desiredWeatherItemEffect) {

                        for (j = 0; j < LEARNED_MOVES_MAX; j++ ) {

                            move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);
                            moveEffect = MOVE_DATA(move).effect;

                            if (moveEffect == desiredMoveEffect) {

                                if (ability == ABILITY_SAND_FORCE
                                    || ability == ABILITY_SAND_VEIL
                                    || ability == ABILITY_SNOW_CLOAK) {

                                    if ((battleCtx->battleMons[battler].curHP > (battleCtx->battleMons[battler].maxHP / 2))
                                        && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK] < 7)
                                        && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_ATTACK] < 7)
                                        && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_DEFENSE] < 7)
                                        && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_DEFENSE] < 7)) 
                                    {
                                        if ((BattleSystem_RandNext(battleSys) % 6) == 0) {
                                            
                                            battleCtx->aiSwitchedPartySlot[battler] = i;
                                            return TRUE;
                                        }
                                    }
                                }
                                else {
                                    // Don't switch if heavily boosted
                                    if ((battleCtx->battleMons[battler].curHP > (battleCtx->battleMons[battler].maxHP / 4)
                                        && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK] < 8)
                                        && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_ATTACK] < 8)
                                        && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_DEFENSE] < 8)
                                        && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_DEFENSE] < 8))) {
                            
                                        battleCtx->aiSwitchedPartySlot[battler] = i;
                                        return TRUE;
                                    }
                                }
                            }
                        }
                    }

                    // Castform will need to switch more because he sucks
                    if (ability == ABILITY_FORECAST) {

                        if (Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_DROUGHT
                            || Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_DRIZZLE
                            || Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_SNOW_WARNING
							|| Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_SAND_STREAM) {

                            if ((battleCtx->battleMons[battler].curHP > (battleCtx->battleMons[battler].maxHP / 2)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK] < 8)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_ATTACK] < 8)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_DEFENSE] < 8)
                                && (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_DEFENSE] < 8))) {
                            
                                battleCtx->aiSwitchedPartySlot[battler] = i;
                                return TRUE;
                            }
                        }
                    }
                }
            }
        }
    }

    // At this point, our weather setter doesn't exist or is KO'd. No need to switch.
    return FALSE;
}

/**
 * @brief Check if the AI should switch for turn.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   TRUE if the battler
 * @return BOOL 
 */
static BOOL TrainerAI_ShouldSwitch(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int i, alivePartyMons;
    u8 aiSlot1, aiSlot2;
    int start, end;
    Pokemon *mon;

    // Don't try to make illegal switches
    // This definition is naive: the AI does not consider itself immune to Magnet Pull from an ally,
    // Shadow Tag if it also has Shadow Tag, Arena Trap if it is a Flying-type, or always able to switch
    // if it is holding a Shed Shell.
    if (((battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_TRAPPED)
            || (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_INGRAIN)
            || (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALL_BATTLERS_THEIR_SIDE, battler, ABILITY_SHADOW_TAG)
                && battleCtx->battleMons[battler].ability != ABILITY_SHADOW_TAG)
            || (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALL_BATTLERS_THEIR_SIDE, battler, ABILITY_ARENA_TRAP)
                && !MON_HAS_TYPE(battler, TYPE_FLYING)
                && battleCtx->battleMons[battler].ability != ABILITY_LEVITATE
                && Battler_HeldItemEffect(battleCtx, battler) != HOLD_EFFECT_LEVITATE_POPPED_IF_HIT)
            || (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALL_BATTLERS_EXCEPT_ME, battler, ABILITY_MAGNET_PULL)
                && MON_HAS_TYPE(battler, TYPE_STEEL))
			|| (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALL_BATTLERS_EXCEPT_ME, battler, ABILITY_THIRSTY)
                && MON_HAS_TYPE(battler, TYPE_WATER)))
            && battleCtx->battleMons[battler].heldItem != ITEM_SHED_SHELL) {
            //&& battleCtx->battleMons[battler].ability != ABILITY_NEUTRALIZING_GAS) {
        return FALSE;
    }

    alivePartyMons = 0;
    aiSlot1 = battler;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG) || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        aiSlot2 = aiSlot1;
    } else {
        aiSlot2 = BattleSystem_Partner(battleSys, battler);
    }

    // Check for living party members (obviously, do not try to switch if there are none).
    start = 0;
    end = BattleSystem_PartyCount(battleSys, battler);
    for (i = start; i < end; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                && i != battleCtx->selectedPartySlot[aiSlot1]
                && i != battleCtx->selectedPartySlot[aiSlot2]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot1]
                && i != battleCtx->aiSwitchedPartySlot[aiSlot2]) {
            alivePartyMons++;
        }
    }

    if (alivePartyMons) {
        if (AI_PerishSongKO(battleSys, battleCtx, battler)) {
            return TRUE;
        }

        if (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_THEIR_SIDE, battler, ABILITY_WONDER_GUARD) > 0) {

            if (AI_CannotDamageWonderGuard(battleSys, battleCtx, battler)) {
                return TRUE;
            }
        }

        if (AI_ShouldSwitchYawn(battleSys, battleCtx, battler)) {
            return TRUE;
        }

        if (AI_ShouldSwitchToxic(battleSys, battleCtx, battler)) {
            return TRUE;
        }

        if (BattleAI_ValidateSwitch(battleSys, battler) == FALSE) {
            return FALSE;
        }

        // Anything below this function is not run for Joke fights
        if (battleSys->trainers[battler].aiMask & AI_FLAG_JOKE) {
            return FALSE;
        }

        if (AI_HasAbsorbAbilityInParty(battleSys, battleCtx, battler)) {
            return TRUE;
        }

        if (AI_IsAsleepWithNaturalCure(battleSys, battleCtx, battler)) {
            return TRUE;
        }

        if (AI_OnlyIneffectiveMoves(battleSys, battleCtx, battler)) {
            return TRUE;
        }

        if (AI_ShouldSwitchWeatherSetter(battleSys, battleCtx, battler)) {
            return TRUE;
        }

        if (AI_ShouldSwitchWeatherDependent(battleSys, battleCtx, battler)) {
            return TRUE;
        }

        // Do not switch if we have a super-effective move.
        // Note that this has a 10% chance of returning FALSE for each of our
        // moves that are super-effective against either opponent.
        if (AI_HasSuperEffectiveMove(battleSys, battleCtx, battler, FALSE)) {
            return FALSE;
        }

        // Never switch if the active battler has 4+ positive stat stages to an attacking stat.
        if (AI_IsHeavilyAttackingStatBoosted(battleSys, battleCtx, battler)) {
            return FALSE;
        }

        // 33% of the time, switch to a party member with an immunity to the last move that hit
        // this battler which also has a super-effective move against an opposing Pokemon.
        if (AI_HasPartyMemberWithSuperEffectiveMove(battleSys, battleCtx, battler, 0x8, 2)) {
            return TRUE;
        }

        // 25% of the time, switch to a party member with an immunity to the last move that hit
        // this battler which also has a super-effective move against an opposing Pokemon.
        if (AI_HasPartyMemberWithSuperEffectiveMove(battleSys, battleCtx, battler, 0x4, 3)) {
            return TRUE;
        }
    }

    return FALSE;
}

int TrainerAI_PickCommand(BattleSystem *battleSys, int battler)
{
    // must declare C89-style to match
    int i;
    u8 battler1, battler2;
    u32 battleType;
    int end;
    Pokemon *mon;
    BattleContext *battleCtx;

    battleCtx = battleSys->battleCtx;
    battleType = BattleSystem_BattleType(battleSys);

    if ((battleType & BATTLE_TYPE_TRAINER) || Battler_Side(battleSys, battler) == BATTLE_SIDE_PLAYER) {
        if (TrainerAI_ShouldSwitch(battleSys, battleCtx, battler)) {
            // If this is a switch which should use the post-KO switch logic, then do so.
            // If there is no valid battler, pick the first one in party order.
            if (battleCtx->aiSwitchedPartySlot[battler] == 6) {
                if ((i = BattleAI_HotSwitchIn(battleSys, battler)) == 6) {
                    battler1 = battler;
                    if ((battleType & BATTLE_TYPE_TAG) || (battleType & BATTLE_TYPE_2vs2)) {
                        battler2 = battler1;
                    } else {
                        battler2 = BattleSystem_Partner(battleSys, battler);
                    }

                    end = BattleSystem_PartyCount(battleSys, battler);
                    for (i = 0; i < end; i++) {
                        mon = BattleSystem_PartyPokemon(battleSys, battler, i);

                        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                                && i != battleCtx->selectedPartySlot[battler1]
                                && i != battleCtx->selectedPartySlot[battler2]
                                && i != battleCtx->aiSwitchedPartySlot[battler1]
                                && i != battleCtx->aiSwitchedPartySlot[battler2]) {
                            break;
                        }
                    }
                }

                battleCtx->aiSwitchedPartySlot[battler] = i;
            }

            return PLAYER_INPUT_PARTY;
        }

        // Check if the AI determines that it should use an item
        if (TrainerAI_ShouldUseItem(battleSys, battler)) {
            return PLAYER_INPUT_ITEM;
        }
    }

    return PLAYER_INPUT_FIGHT;
}

/**
 * @brief Determine if the AI should use an item on its active battler.
 * 
 * Several buffers will be filled, if an item should be used:
 * 1. The item type (e.g., Full Restore, Potion, etc.)
 * 2. Any parameters for the item, e.g. what status condition it heals
 * 3. What item number is used
 * 
 * The trainer's pocket of items will also be updated appropriately.
 * 
 * @param battleSys 
 * @param battler   The AI's battler.
 * @return          TRUE if an item should be used, FALSE if not.
 */
static BOOL TrainerAI_ShouldUseItem(BattleSystem *battleSys, int battler)
{
    int i;
    u8 aliveMons, deadMons;
    u16 item;
    u8 hpRestore;
    BOOL result;
    Party *party;
    Pokemon *mon;
    BattleContext *battleCtx;

    aliveMons = 0;
    deadMons = 0;

    battleCtx = battleSys->battleCtx;
    AI_CONTEXT.usedItemCondition[battler >> 1] = 0;
    result = FALSE;

    // Don't let the AI partners ever use items in battle against trainers.
    if ((battleSys->battleType & BATTLE_TYPE_TRAINER_WITH_AI_PARTNER) == BATTLE_TYPE_TRAINER_WITH_AI_PARTNER
            && BattleSystem_BattlerSlot(battleSys, battler) == BATTLER_TYPE_PLAYER_SIDE_SLOT_2) {
        return result;
    }

    // Don't try to use items if it's illegal to do so.
    if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_EMBARGO) {
        return result;
    }

    party = BattleSystem_Party(battleSys, battler);
    for (i = 0; i < Party_GetCurrentCount(party); i++) {
        mon = Party_GetPokemonBySlotIndex(party, i);

        if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                && Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG) {
            aliveMons++;
        }
        else {
            deadMons++;
        }
    }

    for (i = 0; i < MAX_TRAINER_ITEMS; i++) {
        if (i == 0 || aliveMons <= AI_CONTEXT.trainerItemCounts[battler >> 1] - i + 1) {
            item = AI_CONTEXT.trainerItems[battler >> 1][i];

            if (item == ITEM_NONE) {
                continue;
            }

            if (item == ITEM_FULL_RESTORE) {
                if (battleCtx->battleMons[battler].curHP < (battleCtx->battleMons[battler].maxHP / 3)
                        && battleCtx->battleMons[battler].curHP) {
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_FULL_RESTORE;
                    result = TRUE;
                }
            } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HP_RESTORE)) {
                hpRestore = BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HP_RESTORED);

                // Use an HP restore item if the battler is at less than 1/3 HP or if the full HP restore
                // value of the item would be used.
                if (hpRestore) {
                    if (battleCtx->battleMons[battler].curHP
                            && (battleCtx->battleMons[battler].curHP < (battleCtx->battleMons[battler].maxHP / 3)
                                || (battleCtx->battleMons[battler].maxHP - battleCtx->battleMons[battler].curHP) > hpRestore)) {
                        AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_RECOVER_HP;
                        result = TRUE;
                    }
                }
            } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HEAL_SLEEP)) {
                if (battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] |= FlagIndex(5);
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_RECOVER_STATUS;
                    result = TRUE;
                }
            } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HEAL_POISON)) {
                if ((battleCtx->battleMons[battler].status & MON_CONDITION_POISON)
                        || (battleCtx->battleMons[battler].status & MON_CONDITION_TOXIC)) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] |= FlagIndex(4);
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_RECOVER_STATUS;
                    result = TRUE;
                }
            } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HEAL_BURN)) {
                if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] |= FlagIndex(3);
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_RECOVER_STATUS;
                    result = TRUE;
                }
            } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HEAL_FREEZE)) {
                if (battleCtx->battleMons[battler].status & MON_CONDITION_FREEZE) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] |= FlagIndex(2);
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_RECOVER_STATUS;
                    result = TRUE;
                }
            } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HEAL_PARALYSIS)) {
                if (battleCtx->battleMons[battler].status & MON_CONDITION_PARALYSIS) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] |= FlagIndex(1);
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_RECOVER_STATUS;
                    result = TRUE;
                }
            } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HEAL_CONFUSION)) {
                if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] |= FlagIndex(0);
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_RECOVER_STATUS;
                    result = TRUE;
                }
            // Don't try to use any of these until after the first turn that a mon is in play.
            } else if ((battleCtx->battleMons[battler].moveEffectsData.fakeOutTurnNumber - battleCtx->totalTurns) >= 0) {
                if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_ATK_STAGES)) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] = BATTLE_STAT_ATTACK;
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_STAT_BOOSTER;
                    result = TRUE;
                } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_DEF_STAGES)) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] = BATTLE_STAT_DEFENSE;
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_STAT_BOOSTER;
                    result = TRUE;
                } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_SPATK_STAGES)) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] = BATTLE_STAT_SP_ATTACK;
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_STAT_BOOSTER;
                    result = TRUE;
                } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_SPDEF_STAGES)) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] = BATTLE_STAT_SP_DEFENSE;
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_STAT_BOOSTER;
                    result = TRUE;
                } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_SPEED_STAGES)) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] = BATTLE_STAT_SPEED;
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_STAT_BOOSTER;
                    result = TRUE;
                } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_ACC_STAGES)) {
                    AI_CONTEXT.usedItemCondition[battler >> 1] = BATTLE_STAT_ACCURACY;
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_STAT_BOOSTER;
                    result = TRUE;
                } else if (BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_GUARD_SPEC)
                        && (battleCtx->sideConditionsMask[1] & SIDE_CONDITION_MIST) == FALSE) {
                    AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_GUARD_SPEC;
                    result = TRUE;
                }
            }
            else {
                // Unrecognized item type
                AI_CONTEXT.usedItemType[battler >> 1] = ITEM_TYPE_MAX;
            }

            if (result == TRUE) {
                AI_CONTEXT.usedItem[battler >> 1] = item;
                AI_CONTEXT.trainerItems[battler >> 1][i] = 0;
            }
        }
    }

    return result;
}
