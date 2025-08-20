    .ifndef ASM_BATTLE_SCRIPT_INC
    .set ASM_BATTLE_SCRIPT_INC, 1
#define __ASM_PM_

#include "constants/battle.h"
#include "constants/items.h"
#include "constants/battle/trainer_ai.h"

    .include "consts/abilities.inc"
    .include "consts/battle.inc"
    .include "consts/gender.inc"
    .include "consts/items.inc"
    .include "consts/moves.inc"
    .include "consts/pokemon.inc"
    .include "consts/trainer_ai.inc"
    .include "macros/aicmd.inc"

    .text

    .global gTrainerAITableBasic

gTrainerAITableBasic:

FlagTableBasic:
    LabelDistance Basic_Main,          FlagTable ; AI_FLAG_BASIC
    LabelDistance TerminateBasic,      FlagTable ; All other flags

Basic_Main:
    ; Ignore this flag on partner battlers.
    IfTargetIsPartner Terminate

    ; Score the move according to its damage. If the AI does not know any
    ; moves which are eligible for scoring, skip ahead.
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, Basic_CheckSoundproof

Basic_CheckForImmunity:
    ; Check for any immunity to the current move based on move type and what
    ; we know the battler''s ability to be (if we do at all).
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_NoImmunityAbility
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_VOLT_ABSORB, Basic_CheckElectricAbsorption
    IfLoadedEqualTo ABILITY_MOTOR_DRIVE, Basic_CheckElectricAbsorption
	IfLoadedEqualTo ABILITY_LIGHTNING_ROD, Basic_CheckElectricAbsorption
    IfLoadedEqualTo ABILITY_WATER_ABSORB, Basic_CheckWaterAbsorption
	IfLoadedEqualTo ABILITY_STORM_DRAIN, Basic_CheckWaterAbsorption
    IfLoadedEqualTo ABILITY_FLASH_FIRE, Basic_CheckFireAbsorption
    IfLoadedEqualTo ABILITY_WONDER_GUARD, Basic_CheckWonderGuard
    IfLoadedEqualTo ABILITY_LEVITATE, Basic_CheckGroundAbsorption
    IfLoadedEqualTo ABILITY_DRY_SKIN, Basic_CheckWaterAbsorption2
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedEqualTo ITEM_AIR_BALLOON, Basic_CheckGroundAbsorption
    GoTo Basic_NoImmunityAbility

Basic_CheckElectricAbsorption:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_ELECTRIC, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_CheckWaterAbsorption:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_WATER, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_CheckFireAbsorption:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_FIRE, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_CheckWonderGuard:
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, Basic_NoImmunityAbility
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, Basic_NoImmunityAbility
    GoTo ScoreMinus12

Basic_CheckGroundAbsorption:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_GROUND, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_CheckWaterAbsorption2:
    LoadTypeFrom LOAD_MOVE_TYPE
    IfTempEqualTo TYPE_WATER, ScoreMinus12
    GoTo Basic_NoImmunityAbility

Basic_NoImmunityAbility:
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, Basic_CheckSoundproof

Basic_CheckSoundproof:
    ; Check for immunity to sound-based moves
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_SOUNDPROOF, Basic_ScoreMoveEffect
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_ScoreMoveEffect
    IfMoveEqualTo MOVE_GROWL, ScoreMinus10
    IfMoveEqualTo MOVE_ROAR, ScoreMinus10
    IfMoveEqualTo MOVE_SING, ScoreMinus10
    IfMoveEqualTo MOVE_SUPERSONIC, ScoreMinus10
    IfMoveEqualTo MOVE_SCREECH, ScoreMinus10
    IfMoveEqualTo MOVE_SNORE, ScoreMinus10
    IfMoveEqualTo MOVE_UPROAR, ScoreMinus10
    IfMoveEqualTo MOVE_METAL_SOUND, ScoreMinus10
    IfMoveEqualTo MOVE_GRASS_WHISTLE, ScoreMinus10
    IfMoveEqualTo MOVE_BUG_BUZZ, ScoreMinus10
    IfMoveEqualTo MOVE_CHATTER, ScoreMinus10
    IfMoveEqualTo MOVE_HYPER_VOICE, ScoreMinus10
    IfMoveEqualTo MOVE_PERISH_SONG, ScoreMinus10

Basic_ScoreMoveEffect:
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_SLEEP, Basic_CheckCannotSleep
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_DEFENSE, Basic_CheckCannotExplode
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP, Basic_CheckDreamEater
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_UP, Basic_CheckHighStatStage_Attack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_UP, Basic_CheckHighStatStage_Defense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_UP, Basic_CheckHighStatStage_Speed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_UP, Basic_CheckHighStatStage_SpAttack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_UP, Basic_CheckHighStatStage_SpDefense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ACC_UP, Basic_CheckHighStatStage_Accuracy
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_UP, Basic_CheckHighStatStage_Evasion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DOWN, Basic_CheckLowStatStage_Attack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_DOWN, Basic_CheckLowStatStage_Defense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_DOWN, Basic_CheckLowStatStage_Speed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_DOWN, Basic_CheckLowStatStage_SpAttack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_DOWN, Basic_CheckLowStatStage_SpDefense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ACC_DOWN, Basic_CheckLowStatStage_Accuracy
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_DOWN, Basic_CheckLowStatStage_Evasion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RESET_STAT_CHANGES, Basic_CheckStatStageImbalance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BIDE, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FORCE_SWITCH, Basic_CheckCanForceSwitch
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FORCE_SWITCH_HIT, Basic_CheckCanForceSwitch
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RESTORE_HALF_HP, Basic_CheckCanRecoverHP
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_BADLY_POISON, Basic_CheckCannotPoison
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_LIGHT_SCREEN, Basic_CheckAlreadyUnderLightScreen
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_LIGHT_SCREEN_HIT, Basic_CheckAlreadyUnderLightScreen
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ONE_HIT_KO, Basic_CheckOHKOWouldFail
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_HP, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_40_DAMAGE_FLAT, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_STAT_REDUCTION, Basic_CheckAlreadyUnderMist
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CRIT_UP_2, Basic_CheckAlreadyPumpedUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_CONFUSE, Basic_CheckCannotConfuse
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_UP_2, Basic_CheckHighStatStage_Attack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_UP_2, Basic_CheckHighStatStage_Defense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_UP_2, Basic_CheckHighStatStage_Speed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_UP_2, Basic_CheckHighStatStage_SpAttack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_UP_2, Basic_CheckHighStatStage_SpDefense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ACC_UP_2, Basic_CheckHighStatStage_Accuracy
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_UP_2, Basic_CheckHighStatStage_Evasion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DOWN_2, Basic_CheckLowStatStage_Attack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_DOWN_2, Basic_CheckLowStatStage_Defense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_DOWN_2, Basic_CheckLowStatStage_Speed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_DOWN_2, Basic_CheckLowStatStage_SpAttack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_DOWN_2, Basic_CheckLowStatStage_SpDefense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_DOWN_2, Basic_CheckLowStatStage_Accuracy
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ACC_DOWN_2, Basic_CheckLowStatStage_Evasion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_REFLECT, Basic_CheckAlreadyUnderReflect
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_POISON, Basic_CheckCannotPoison
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_PARALYZE, Basic_CheckCannotParalyze
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_SUBSTITUTE, Basic_CheckCannotSubstitute
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECHARGE_AFTER, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_LEECH_SEED, Basic_CheckCannotLeechSeed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DISABLE, Basic_CheckCannotDisable
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DISABLE_HIT, Basic_CheckCannotDisable
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LEVEL_DAMAGE_FLAT, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RANDOM_DAMAGE_1_TO_150_LEVEL, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_COUNTER, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ENCORE, Basic_CheckCannotEncore
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DAMAGE_WHILE_ASLEEP, Basic_CheckAttackerAsleep
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_NEXT_ATTACK_ALWAYS_HITS, Basic_CheckLockOn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP, Basic_CheckAttackerAsleep
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INCREASE_POWER_WITH_LESS_HP, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_ESCAPE, Basic_CheckMeanLook
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_NIGHTMARE, Basic_CheckNightmare
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_UP_2_MINIMIZE, Basic_CheckHighStatStage_Evasion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CURSE, Basic_CheckCurse
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_SPIKES, Basic_CheckSpikes
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPIKES_MULTI_HIT, Basic_CheckSpikes
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_IGNORE_EVASION_REMOVE_GHOST_IMMUNE, Basic_CheckForesight
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ALL_FAINT_3_TURNS, Basic_CheckPerishSong
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_SANDSTORM, Basic_CheckSandstorm
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION, Basic_CheckCannotConfuse
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INFATUATE, Basic_CheckCannotAttract
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_POWER_BASED_ON_FRIENDSHIP, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RANDOM_POWER_MAYBE_HEAL, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_POWER_BASED_ON_LOW_FRIENDSHIP, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_STATUS, Basic_CheckAlreadyUnderSafeguard
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_GROWTH, Expert_CheckGrowth
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PASS_STATS_AND_STATUS, Basic_CheckBatonPass
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_10_DAMAGE_FLAT, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN, Basic_CheckCanRecoverHP
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MEDITATE, Basic_CheckHighStatStage_Attack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RANDOM_POWER_BASED_ON_IVS, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_RAIN, Basic_CheckRainDance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_SUN, Basic_CheckSunnyDay
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP, Basic_CheckBellyDrum
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_COPY_STAT_CHANGES, Basic_CheckStatStageImbalance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MIRROR_COAT, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CHARGE_TURN_DEF_UP, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_IN_3_TURNS, Basic_CheckFutureSight
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FLEE_FROM_WILD_BATTLE, Expert_UTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_UP_DOUBLE_ROLLOUT_POWER, Basic_CheckHighStatStage_Defense
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_STICKY_WEB, Basic_CheckStickyWeb
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ALWAYS_FLINCH_FIRST_TURN_ONLY, Basic_CheckFirstTurnInBattle
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STOCKPILE, Basic_CheckMaxStockpile
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPIT_UP, Basic_CheckCanSpitUpOrSwallow
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWALLOW, Basic_CheckCanSpitUpOrSwallow
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_HAIL, Basic_CheckHail
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TORMENT, Basic_CheckTorment
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION, Basic_CheckCannotConfuse
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_BURN, Basic_CheckCannotBurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAINT_AND_ATK_SP_ATK_DOWN_2, Basic_CheckMemento
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BOOST_ALLY_POWER_BY_50_PERCENT, Basic_CheckHelpingHand
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWITCH_HELD_ITEMS, Basic_CheckCanRemoveItem
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_GROUND_TRAP_USER_CONTINUOUS_HEAL, Basic_CheckAlreadyIngrained
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LOWER_OWN_ATK_AND_DEF, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECYCLE, Basic_CheckCanRecycle
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_SLEEP_NEXT_TURN, Basic_CheckCannotSleep
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_REMOVE_HELD_ITEM, Basic_CheckCanRemoveItem
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_HP_EQUAL_TO_USER, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE, Basic_CheckCanImprison
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_STATUS, Basic_CheckCanRefreshStatus
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INCREASE_POWER_WITH_WEIGHT, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_ELECTRIC_DAMAGE, Basic_CheckCanMudSport
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DEF_DOWN, Basic_CheckTickle
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_SPD_UP, Basic_CheckCosmicPower
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DEF_UP, Basic_CheckBulkUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_FIRE_DAMAGE, Basic_CheckWaterSport
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_SP_DEF_UP, Basic_CheckCalmMind
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_SPD_UP, Basic_CheckDragonDance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CAMOUFLAGE, Basic_CheckCamouflage
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE, Basic_CheckCanRecoverHP
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_GRAVITY, Basic_CheckGravityActive
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_IGNORE_EVATION_REMOVE_DARK_IMMUNE, Expert_Disable
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_POWER_BASED_ON_LOW_SPEED, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON, Basic_CheckHealingWish
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_NATURAL_GIFT, Basic_CheckNaturalGift
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS, Basic_CheckTailwind
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RANDOM_STAT_UP_2, Basic_CheckAcupressure
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_METAL_BURST, Basic_CheckMetalBurst
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_ITEM_USE, Basic_CheckEmbargo
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FLING, Basic_CheckFling
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TRANSFER_STATUS, Basic_CheckCanPsychoShift
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIGHER_POWER_WHEN_LOW_PP, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_HEALING, Basic_CheckHealBlock
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_HP, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWAP_ATK_DEF, Basic_CheckPowerTrick
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SUPRESS_ABILITY, Basic_CheckGastroAcid
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_CRITS, Basic_CheckLuckyChant
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_USE_LAST_USED_MOVE, Basic_CheckCopycat
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWAP_ATK_SP_ATK_STAT_CHANGES, Basic_CheckPowerSwap
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWAP_DEF_SP_DEF_STAT_CHANGES, Basic_CheckGuardSwap
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_STAT_UP, Basic_CheckNonStandardDamageOrChargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAIL_IF_NOT_USED_ALL_OTHER_MOVES, Basic_CheckLastResort
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_ABILITY_TO_INSOMNIA, Basic_CheckWorrySeed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TOXIC_SPIKES, Basic_CheckToxicSpikes
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWAP_STAT_CHANGES, Basic_CheckStatStageImbalance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RESTORE_HP_EVERY_TURN, Basic_CheckAquaRing
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_GIVE_GROUND_IMMUNITY, Basic_CheckMagnetRise
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING, Basic_CheckRapidSpin
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TRICK_ROOM, Basic_CheckTrickRoom
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER, Basic_CheckCaptivate
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STEALTH_ROCK, Basic_CheckStealthRock
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON, Basic_CheckLunarDance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_IN_3_TURNS, Basic_Wish
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TAUNT, Basic_Taunt
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HOWL, Basic_CheckHighStatStage_Attack
    PopOrEnd 

Basic_CheckCannotSleep:
    ; If the target cannot be put to sleep for any reason, score -12.
	IfEnemySleepClauseActive ScoreMinus12
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus12
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus12
    IfFieldConditionsMask FIELD_CONDITION_UPROAR, ScoreMinus12
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_INSOMNIA, Basic_CheckCannotSleep_CheckMoldBreaker
    IfLoadedEqualTo ABILITY_VITAL_SPIRIT, Basic_CheckCannotSleep_CheckMoldBreaker
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, Basic_CheckCannotSleep_CheckMoldBreaker
    IfLoadedEqualTo ABILITY_EARLY_BIRD, ScoreMinus12
    IfLoadedEqualTo ABILITY_LEAF_GUARD, Basic_CheckCannotSleep_CheckSun
    IfLoadedEqualTo ABILITY_HYDRATION, Basic_CheckCannotSleep_CheckRain
    IfLoadedEqualTo ABILITY_NATURAL_CURE, Basic_CheckCannotSleep_CheckCanSwitch
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus12
    GoTo Basic_CheckCannotSleep_CheckPowderMove

Basic_CheckCannotSleep_CheckMoldBreaker:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo ABILITY_MOLD_BREAKER, ScoreMinus12
    GoTo Basic_CheckCannotSleep_CheckPowderMove

Basic_CheckCannotSleep_CheckSun:
    IfFieldConditionsMask FIELD_CONDITION_SUNNY, ScoreMinus12
    GoTo Basic_CheckCannotSleep_CheckPowderMove

Basic_CheckCannotSleep_CheckRain:
    IfFieldConditionsMask FIELD_CONDITION_RAINING, ScoreMinus12
    GoTo Basic_CheckCannotSleep_CheckPowderMove

Basic_CheckCannotSleep_CheckCanSwitch:
    IfTrapped AI_BATTLER_DEFENDER, Basic_CheckCannotSleep_CheckPowderMove
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo 0, ScoreMinus12
    GoTo Basic_CheckCannotSleep_CheckPowderMove

Basic_CheckCannotSleep_CheckPowderMove:
    IfMoveEqualTo MOVE_SLEEP_POWDER, Basic_CheckCannotSleep_PowderMove
    IfMoveEqualTo MOVE_SPORE, Basic_CheckCannotSleep_PowderMove
    GoTo Basic_CheckCannotSleep_End
    
Basic_CheckCannotSleep_PowderMove:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, ScoreMinus12
    GoTo Basic_CheckCannotSleep_End

Basic_CheckCannotSleep_End:
    PopOrEnd

Basic_CheckCannotExplode:
    ; If the target is immune, score -10.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10

    ; If the target has Damp and we do not have Mold Breaker, score -10.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckLastMon
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_DAMP, ScoreMinus10

Basic_CheckLastMon:
    ; If we are on our last Pokemon and the target is not also on their last Pokemon,
    ; score -3.
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo 0, Basic_Explode_Terminate
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo 0, Try50ChanceForScoreMinus3

    ; If the target is also on their last Pokemon, score -1 instead of -10.
    GoTo ScoreMinus1

Basic_Explode_Terminate:
    PopOrEnd 

Basic_CheckNightmare:
    ; If the target is immune to the effect of Nightmare for any reason, score -10.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_NIGHTMARE, ScoreMinus10
    IfNotStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    PopOrEnd 

Basic_CheckDreamEater:
    ; If the target is immune to Dream Eater for any reason, score -10.
    IfNotStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, ScoreMinus10
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    PopOrEnd 

Basic_CheckBellyDrum:
    ; If the attacker is at half HP or less, score -10.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 51, ScoreMinus10

    ; General comments on stat-boosting Status moves below:
    ;   - If the attacker is already at +6, score -10.
    ;   - Special cases for Speed (Trick Room active -> -10) and Accuracy/Evasion (attacker has No Guard -> -10)
Basic_CheckHighStatStage_Attack:
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 12, ScoreMinus12
    PopOrEnd 

Basic_CheckHighStatStage_Defense:
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 12, ScoreMinus12
    PopOrEnd 

Basic_CheckHighStatStage_Speed:
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, ScoreMinus12
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 12, ScoreMinus10
    PopOrEnd 

Basic_CheckHighStatStage_SpAttack:
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 12, ScoreMinus10
    PopOrEnd 

Basic_CheckHighStatStage_SpDefense:
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 12, ScoreMinus10
    PopOrEnd 

Basic_CheckHighStatStage_Accuracy:
    IfAbilityInPlay ABILITY_NO_GUARD, ScoreMinus12
    IfAbilityInPlay ABILITY_SUCTION_CUPS, ScoreMinus12
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 12, ScoreMinus10
    PopOrEnd 

Basic_CheckHighStatStage_Evasion:
    IfAbilityInPlay ABILITY_NO_GUARD, ScoreMinus12
    IfAbilityInPlay ABILITY_SUCTION_CUPS, ScoreMinus12
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 12, ScoreMinus10
    PopOrEnd 

    ; General comments on stat-reducing Status moves below:
    ;   - If the target is already at -6, score -10.
    ;   - If the target has White Smoke or Clear Body, score -10.
    ;   - If reducing Attack -> -10 if the target has Hyper Cutter
    ;   - If reducing Speed -> -10 if Trick Room is currently active
    ;   - If reducing Speed -> -10 if the target has Speed Boost
    ;   - If reducing Accuracy or Evasion -> -10 if either battler has No Guard
    ;   - If reducing Accuracy -> -10 if the target has Keen Eye
Basic_CheckLowStatStage_Attack:
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 0, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_HYPER_CUTTER, ScoreMinus10
    GoTo Basic_CheckClearBodyEffect

Basic_CheckLowStatStage_Defense:
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 0, ScoreMinus10
    GoTo Basic_CheckClearBodyEffect

Basic_CheckLowStatStage_Speed:
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 0, ScoreMinus10
    CheckBattlerAbility AI_BATTLER_DEFENDER, ABILITY_SPEED_BOOST
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    GoTo Basic_CheckClearBodyEffect

Basic_CheckLowStatStage_SpAttack:
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 0, ScoreMinus10
    GoTo Basic_CheckClearBodyEffect

Basic_CheckLowStatStage_SpDefense:
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 0, ScoreMinus10
    GoTo Basic_CheckClearBodyEffect

Basic_CheckLowStatStage_Accuracy:
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_ACCURACY, 0, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_NO_GUARD, ScoreMinus10
	IfLoadedEqualTo ABILITY_SUCTION_CUPS, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_KEEN_EYE, ScoreMinus10
    IfLoadedEqualTo ABILITY_NO_GUARD, ScoreMinus10
	IfLoadedEqualTo ABILITY_SUCTION_CUPS, ScoreMinus10
    GoTo Basic_CheckClearBodyEffect

Basic_CheckLowStatStage_Evasion:
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 0, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_NO_GUARD, ScoreMinus10
	IfLoadedEqualTo ABILITY_SUCTION_CUPS, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_NO_GUARD, ScoreMinus10
	IfLoadedEqualTo ABILITY_SUCTION_CUPS, ScoreMinus10

Basic_CheckClearBodyEffect:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus10
    IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus12
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus12
    PopOrEnd 

Basic_CheckStatStageImbalance:
    ; The name is a little esoteric; an "imbalance" is regarded as the attacker
    ; having any reduced stat stage or the target having any increased stat stage.
    ;
    ; If neither of those are true, score -10.
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ACCURACY, 6, Basic_CheckStatStageImbalance_Terminate
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 6, Basic_CheckStatStageImbalance_Terminate
    GoTo ScoreMinus10

Basic_CheckStatStageImbalance_Terminate:
    PopOrEnd 

Basic_CheckCanForceSwitch:
    ; If the target cannot be forced out for any reason, score -10.
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckCanForceSwitch_Terminate
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SUCTION_CUPS, ScoreMinus10

Basic_CheckCanForceSwitch_Terminate:
    PopOrEnd 

Basic_CheckCanRecoverHP:
    ; If at 100% HP, score -8.
    IfHPPercentNotEqualTo AI_BATTLER_ATTACKER, 100, Basic_CheckCanRecoverHP_Terminate
    AddToMoveScore -8

Basic_CheckCanRecoverHP_Terminate:
    PopOrEnd 

Basic_CheckCannotPoison:
    ; If the target is immune to the usual effects of Poison for any reason, score -10.
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, Basic_CheckCannotPoison_CheckCorrosion
    IfLoadedEqualTo TYPE_POISON, ScoreMinus10
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_STEEL, Basic_CheckCannotPoison_CheckCorrosion
    IfLoadedEqualTo TYPE_POISON, ScoreMinus10
    GoTo Basic_CheckCannotPoison_CheckDefenderAbility

Basic_CheckCannotPoison_CheckDefenderAbility:
    ; Check for immunity by ability
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus10
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus10
    IfLoadedEqualTo ABILITY_SHED_SKIN, Try95ChanceForScoreMinus12
    IfLoadedNotEqualTo ABILITY_LEAF_GUARD, Basic_CheckCannotPoison_Hydration
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SUNNY, ScoreMinus10
    GoTo Basic_CheckCannotPoison_StatusOrSafeguard

Basic_CheckCannotPoison_CheckCorrosion:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo ABILITY_CORROSION, ScoreMinus12
    GoTo Basic_CheckCannotPoison_CheckDefenderAbility

Basic_CheckCannotPoison_Hydration:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_HYDRATION, Basic_CheckCannotPoison_StatusOrSafeguard
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_RAINING, ScoreMinus10

Basic_CheckCannotPoison_StatusOrSafeguard:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus10
    GoTo Basic_CheckCannotPoison_CheckPowder

Basic_CheckCannotPoison_CheckPowder:
    IfMoveEqualTo MOVE_POISON_POWDER, Basic_CheckCannotPoison_Powder
    GoTo Basic_CheckCannotPoison_End

Basic_CheckCannotPoison_Powder:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, ScoreMinus12
    GoTo Basic_CheckCannotPoison_End

Basic_CheckCannotPoison_End:
    PopOrEnd 

Basic_CheckAlreadyUnderLightScreen:
    ; If already under the effect of Light Screen, score -8.
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_LIGHT_SCREEN, ScoreMinus8
    PopOrEnd 

Basic_CheckOHKOWouldFail:
    ; If the OHKO move would always fail for any reason, score -10.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckOHKOWouldFail_Levels
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_STURDY, ScoreMinus10
	IfLoadedEqualTo ABILITY_ROCK_SOLID, ScoreMinus10

Basic_CheckOHKOWouldFail_Levels:
    IfLevel CHECK_LOWER_THAN_TARGET, ScoreMinus10
    PopOrEnd 

Basic_CheckMagnitude:
    ; If the target''s ability is Levitate and the attacker''s ability is not Mold Breaker, score -10.
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckNonStandardDamageOrChargeTurn
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LEVITATE, ScoreMinus10

Basic_CheckNonStandardDamageOrChargeTurn:
    ; If the target is immune to this move by its typing or due to the target''s ability being
    ; Wonder Guard, score -10.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_WONDER_GUARD, Basic_CheckNonStandardDamageOrChargeTurn_Terminate
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckNonStandardDamageOrChargeTurn_Terminate
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, Basic_CheckNonStandardDamageOrChargeTurn_Terminate
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, Basic_CheckNonStandardDamageOrChargeTurn_Terminate
    GoTo ScoreMinus10

Basic_CheckNonStandardDamageOrChargeTurn_Terminate:
    PopOrEnd 

Basic_CheckAlreadyUnderMist:
    ; If already under the effect of Mist, score -8.
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_MIST, ScoreMinus8
    PopOrEnd 

Basic_CheckAlreadyPumpedUp:
    ; If already under the effect of Focus Energy, score -10.
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_FOCUS_ENERGY, ScoreMinus10
    PopOrEnd 

Basic_CheckCannotConfuse:
    ; If the target is already confused, score -5.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CONFUSION, ScoreMinus5

    ; If the target otherwise cannot be confused, score -10.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_OWN_TEMPO, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus10
    PopOrEnd 

Basic_CheckAlreadyUnderReflect:
    ; If already under the effect of Reflect, score -8.
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_REFLECT, ScoreMinus8
    PopOrEnd 

Basic_CheckCannotParalyze:
    ; If the target cannot be paralyzed for any reason, score -10.
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_ELECTRIC, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_ELECTRIC, ScoreMinus12
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LIMBER, ScoreMinus10
    IfLoadedEqualTo ABILITY_QUICK_FEET, ScoreMinus10
    IfMoveEqualTo MOVE_THUNDER_WAVE, Basic_CheckCannotParalyze_ThunderWave
    IfMoveEqualTo MOVE_STUN_SPORE, Basic_CheckCannotParalyze_PowderMove
    GoTo Basic_CheckCannotParalyze_ImmuneToStatus

Basic_CheckCannotParalyze_ThunderWave:
    LoadBattlerAbility AI_BATTLER_ATTACKER    
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckCannotParalyze_ImmuneToStatus
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MOTOR_DRIVE, ScoreMinus10
    IfLoadedEqualTo ABILITY_VOLT_ABSORB, ScoreMinus10
	IfLoadedEqualTo ABILITY_LIGHTNING_ROD, ScoreMinus10
    GoTo Basic_CheckCannotParalyze_ImmuneToStatus

Basic_CheckCannotParalyze_PowderMove:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, ScoreMinus12
    GoTo Basic_CheckCannotParalyze_ImmuneToStatus

Basic_CheckCannotParalyze_ImmuneToStatus:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus10
    PopOrEnd 

Basic_CheckCannotSubstitute:
    ; If the attacker''s Substitute would fail, score -8/-10.
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, Basic_CheckCannotSubstitute_CheckSpeed
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 26, ScoreMinus12
    GoTo Basic_CheckCannotSubstitute_End

Basic_CheckCannotSubstitute_CheckSpeed:
    IfSpeedCompareNotEqualTo COMPARE_SPEED_SLOWER, ScoreMinus12
    IfRandomLessThan 85, ScoreMinus2

Basic_CheckCannotSubstitute_End:
    PopOrEnd

Basic_CheckCannotLeechSeed:
    ; If the target is already Seeded or immune to the effects of Leech Seed, score -10.
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LEECH_SEED, ScoreMinus20
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus10
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    IfLoadedEqualTo ABILITY_LIQUID_OOZE, ScoreMinus10
    PopOrEnd 

Basic_CheckCannotDisable:
    ; If the target is already Disabled, score -8.
    IfBattlerUnderEffect AI_BATTLER_DEFENDER, CHECK_DISABLE, ScoreMinus8
    PopOrEnd 

Basic_CheckCannotEncore:
    ; If the target is already Encored, score -8.
    IfBattlerUnderEffect AI_BATTLER_DEFENDER, CHECK_ENCORE, ScoreMinus8
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    IfLoadedEqualTo MOVE_NONE, ScoreMinus12
    PopOrEnd 

Basic_CheckAttackerAsleep:
    ; If the attacker is not currently asleep, score -8.
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_SLEEP, ScoreMinus8
    PopOrEnd 

Basic_CheckLockOn:
    ; If the target is already Locked On, or either battler has No Guard, score -10.
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LOCK_ON, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_NO_GUARD, ScoreMinus10
	IfLoadedEqualTo ABILITY_SUCTION_CUPS, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_NO_GUARD, ScoreMinus10
	IfLoadedEqualTo ABILITY_SUCTION_CUPS, ScoreMinus10
    PopOrEnd 

Basic_CheckMeanLook:
    ; If the target is already under the effect of Mean Look, score -10.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_MEAN_LOOK, ScoreMinus10
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_BIND, Try50ChanceForScoreMinus1
    PopOrEnd 

Basic_CheckCurse:
    ; Branch for a Ghost-type using Curse
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, Basic_CheckCurse_GhostType
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, Basic_CheckCurse_GhostType

    ; If user is already max stat boosts, score -10.
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 12, Try90ChanceForScoreMinus12
    PopOrEnd 

Basic_CheckCurse_GhostType:
    ; If the target is immune to the effect, score -10.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CURSE, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    PopOrEnd 

Basic_CheckSpikes:
    ; If the target already has 3 layers of Spikes or is on their last Pokemon, score -10.
    LoadSpikesLayers AI_BATTLER_DEFENDER, SIDE_CONDITION_SPIKES
    IfLoadedEqualTo 3, ScoreMinus10
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, ScoreMinus10
    PopOrEnd 

Basic_CheckForesight:
    ; If the target is already under the effect, score -10.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_FORESIGHT, ScoreMinus10
    PopOrEnd 

Basic_CheckPerishSong:
    ; If the target is already under the effect, score -10.
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_PERISH_SONG, ScoreMinus10
    PopOrEnd 

Basic_CheckSandstorm:
    ; If the current weather is Sand, score -8.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SANDSTORM, ScoreMinus8
    PopOrEnd 

Basic_CheckCannotAttract:
    ; If the target cannot be Infatuated for any reason, score -12.
    ;
    ; If the target can be infatuated, 50% chance for score +1 and 25% chance for score +2.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, ScoreMinus12
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_OBLIVIOUS, ScoreMinus12
    LoadGender AI_BATTLER_ATTACKER
    IfLoadedEqualTo GENDER_MALE, Basic_CheckCannotAttract_BothMale
    IfLoadedEqualTo GENDER_FEMALE, Basic_CheckCannotAttract_BothFemale
    IfLoadedEqualTo GENDER_NONE, ScoreMinus12
    GoTo ScoreMinus12

Basic_CheckCannotAttract_BothMale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_FEMALE, Basic_CheckCannotAttract_CanAttract
    GoTo ScoreMinus12

Basic_CheckCannotAttract_BothFemale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_MALE, Basic_CheckCannotAttract_CanAttract
    GoTo ScoreMinus12

Basic_CheckCannotAttract_CanAttract:
    IfRandomLessThan 128, Basic_CheckCannotAttract_Terminate
    AddToMoveScore 1
    IfRandomLessThan 128, Basic_CheckCannotAttract_Terminate
    AddToMoveScore 1
    GoTo Basic_CheckCannotAttract_Terminate

Basic_CheckCannotAttract_Terminate:
    PopOrEnd 

Basic_CheckAlreadyUnderSafeguard:
    ; If already under the effect of Safeguard, score -8.
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_SAFEGUARD, ScoreMinus8
    PopOrEnd 

Basic_CheckMemento:
    ; If the target''s ability blocks the stat drop and the attacker does not have Mold Breaker,
    ; score -10.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckMemento_CheckStatStages
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus10
    IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10

Basic_CheckMemento_CheckStatStages:
    ; If the target''s Attack is already at -6, score -10.
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 0, ScoreMinus10

    ; If the target''s SpAttack is already at -6, score -8.
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 0, ScoreMinus8

    ; If the attacker is on their last Pokemon, score -10.
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus10
    PopOrEnd 

Basic_CheckBatonPass:
    ; If the attacker is on its last Pokemon, score -10.
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus10
    PopOrEnd 

Basic_CheckRainDance:
    ; If the attacker''s ability is Swift Swim or Hydration, skip the defender-Hydration check below.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SWIFT_SWIM, Basic_CheckCurrentWeatherIsRain
    IfLoadedEqualTo ABILITY_HYDRATION, Basic_CheckCurrentWeatherIsRain

    ; If the target''s ability is Hydration and they are currently statused, score -8.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_HYDRATION, Basic_CheckCurrentWeatherIsRain
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus8

Basic_CheckCurrentWeatherIsRain:
    ; If the weather is currently Rain, score -8.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_RAINING, ScoreMinus8
    PopOrEnd 

Basic_CheckSunnyDay:
    ; If the attacker''s ability is any of Flower Gift, Leaf Guard, or Solar Power, skip the defender-
    ; Hydration check below.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_FLOWER_GIFT, Basic_CheckCurrentWeatherIsSun
    IfLoadedEqualTo ABILITY_LEAF_GUARD, Basic_CheckCurrentWeatherIsSun
    IfLoadedEqualTo ABILITY_SOLAR_POWER, Basic_CheckCurrentWeatherIsSun

    ; If the target''s ability is Hydration and they are currently statused, score -10.
    ; Why does this consider Hydration? This is clearly a bug, but what was the intention?
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_HYDRATION, Basic_CheckCurrentWeatherIsSun
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus10

Basic_CheckCurrentWeatherIsSun:
    ; If the weather is currently Sun, score -8.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SUNNY, ScoreMinus8
    PopOrEnd 

Basic_CheckFutureSight:
    ; If either the attacker or the target are currently under the effect of Future Sight, score -12.
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_FUTURE_SIGHT, ScoreMinus12
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_FUTURE_SIGHT, ScoreMinus12
    PopOrEnd 

Basic_CheckFirstTurnInBattle:
    ; If it is not the attacker''s first turn in battle, score -10.
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo FALSE, ScoreMinus10
    PopOrEnd 

Basic_CheckMaxStockpile:
    ; If the Stockpile count is already at 3, score -10.
    LoadStockpileCount AI_BATTLER_ATTACKER
    IfLoadedEqualTo 3, ScoreMinus10
    PopOrEnd 

Basic_CheckCanSpitUpOrSwallow:
    ; If the target is immune to the move by its typing or the Stockpile count is 0, score -10.
    ; Note that this means that Swallow will never be used against a Ghost-type Pokemon, even though
    ; it would still have an effect.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    LoadStockpileCount AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus10

    ; Treat Swallow like a standard recovery move.
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWALLOW, Basic_CheckCanRecoverHP
    PopOrEnd 

Basic_CheckHail:
    ; If the current weather is Hail, score -8.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_HAILING, ScoreMinus8

    ; If any opposing battler''s ability is Ice Body, score -8.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_ICE_BODY, Basic_CheckHail_Terminate
    AddToMoveScore -8

    ; If an attacker''s ability is also Ice Body, score +8 (undo the previous modifier).
    ; This feels like a bug of misintention; the intention here seems to be for an attacker with
    ; Ice Body to have an incentive to use Hail, but that is not realized. Instead, such an
    ; attacker can only have a disincentive undone.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo ABILITY_ICE_BODY, Basic_CheckHail_Terminate
    AddToMoveScore 8

Basic_CheckHail_Terminate:
    PopOrEnd 

Basic_CheckTorment:
    ; If the target is already under the effect, score -10.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_TORMENT, ScoreMinus10
    PopOrEnd 

Basic_CheckCannotBurn:
    ; If the target cannot be burned for any reason, score -10.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_WATER_VEIL, ScoreMinus10
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus10
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus10
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus10
    PopOrEnd 

Basic_CheckHelpingHand:
    ; If the battle type is not Doubles, score -10.
    LoadBattleType 
    IfLoadedNotMask BATTLE_TYPE_DOUBLES, ScoreMinus10
    PopOrEnd 

Basic_CheckCanRemoveItem:
    ; If the defender''s ability is Sticky Hold or they do not have a held item, score -10.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_STICKY_HOLD, ScoreMinus10
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedEqualTo ITEM_NONE, ScoreMinus10
    PopOrEnd 

Basic_CheckAlreadyIngrained:
    ; If the attacker is already under the effect, score -10.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_INGRAIN, ScoreMinus10
    PopOrEnd 

Basic_CheckCanRecycle:
    ; If there is no item to be recycled, score -10.
    LoadRecycleItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_NONE, ScoreMinus10
    PopOrEnd 

Basic_CheckCanImprison:
    ; If either the attacker or a target are under the effect of Imprison, score -10.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_IMPRISON, ScoreMinus10
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_IMPRISONED, ScoreMinus10
    PopOrEnd 

Basic_CheckCanRefreshStatus:
    ; If the attacker is not Burned, Poisoned, or Paralyzed, score -10.
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_FACADE_BOOST, ScoreMinus10
    PopOrEnd 

Basic_CheckCanMudSport:
    ; If the attacker is already under the respective effect, score -10.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_MUD_SPORT, ScoreMinus10
    PopOrEnd 

Basic_CheckTickle:
    ; If the target''s ability is Clear Body or White Smoke and the attacker''s ability is not
    ; Mold Breaker, score -10.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckTickle_CheckStatStages
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus10
    IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10

Basic_CheckTickle_CheckStatStages:
    ; If the target''s Attack is at -6, score -10.
    ; If the target''s Defense is at -6, score -8.
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 0, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 0, ScoreMinus8
    PopOrEnd 

Basic_CheckCosmicPower:
    ; If the attacker''s Defense is already at +6, score -10.
    ; If the attacker''s SpDefense is already at +6, score -8.
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 12, ScoreMinus8
    PopOrEnd 

Basic_CheckBulkUp:
    ; If the attacker''s Attack is already at +6, score -10.
    ; If the attacker''s Defense is already at +6, 90% chance for score -12.
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 12, Try90ChanceForScoreMinus12
    PopOrEnd 

Basic_CheckWaterSport:
    ; If the attacker is already under the respective effect, score -10.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_WATER_SPORT, ScoreMinus10
    PopOrEnd 

Basic_CheckCalmMind:
    ; If the attacker''s SpAttack is already at +6, score -10.
    ; If the attacker''s SpDefense is already at +6, 90% chance for score -12.
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 12, Try90ChanceForScoreMinus12
    PopOrEnd 

Basic_CheckDragonDance:
    ; If Trick Room is in effect, score -10.
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, ScoreMinus10
    ; If the attacker''s Attack is already at +6, score -10.
    ; If the attacker''s Speed is already at +6, 90% chance for score -12.
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 12, Try90ChanceForScoreMinus12
    PopOrEnd 

Basic_CheckCamouflage:
    ; If the attacker is already under the respective effect, score -10.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_CAMOUFLAGE, ScoreMinus10
    PopOrEnd 

Basic_CheckGravityActive:
    ; If Gravity is already active, score -10.
    IfFieldConditionsMask FIELD_CONDITION_GRAVITY, ScoreMinus10
    PopOrEnd 

Basic_CheckMiracleEye:
    ; If the target is already under the respective effect, score -10.
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_MIRACLE_EYE, ScoreMinus10
    PopOrEnd 

Basic_CheckHealingWish:
    ; Start at -20
    AddToMoveScore -20

    ; If the attacker is on their last Pokemon, score additional -10.
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus10

    ; If none of the attacker''s party members are statused or at less than 100% HP,
    ; score additional -10.
    IfPartyMemberStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Basic_CheckHealingWish_Terminate
    IfAnyPartyMemberIsWounded AI_BATTLER_ATTACKER, Basic_CheckHealingWish_Terminate
    GoTo ScoreMinus10

Basic_CheckHealingWish_Terminate:
    PopOrEnd 

Basic_CheckNaturalGift:
    ; If the attacker does not have an eligible berry or the target is immune to that berry''s
    ; Natural Gift type, score -10.
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedNotInTable Basic_NaturalGiftBerries, ScoreMinus10
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    PopOrEnd 

Basic_NaturalGiftBerries:
    TableEntry ITEM_CHERI_BERRY
    TableEntry ITEM_CHESTO_BERRY
    TableEntry ITEM_PECHA_BERRY
    TableEntry ITEM_RAWST_BERRY
    TableEntry ITEM_ASPEAR_BERRY
    TableEntry ITEM_LEPPA_BERRY
    TableEntry ITEM_ORAN_BERRY
    TableEntry ITEM_PERSIM_BERRY
    TableEntry ITEM_LUM_BERRY
    TableEntry ITEM_SITRUS_BERRY
    TableEntry ITEM_FIGY_BERRY
    TableEntry ITEM_WIKI_BERRY
    TableEntry ITEM_MAGO_BERRY
    TableEntry ITEM_AGUAV_BERRY
    TableEntry ITEM_IAPAPA_BERRY
    TableEntry ITEM_RAZZ_BERRY
    TableEntry ITEM_BLUK_BERRY
    TableEntry ITEM_NANAB_BERRY
    TableEntry ITEM_WEPEAR_BERRY
    TableEntry ITEM_PINAP_BERRY
    TableEntry ITEM_POMEG_BERRY
    TableEntry ITEM_KELPSY_BERRY
    TableEntry ITEM_QUALOT_BERRY
    TableEntry ITEM_HONDEW_BERRY
    TableEntry ITEM_GREPA_BERRY
    TableEntry ITEM_TAMATO_BERRY
    TableEntry ITEM_CORNN_BERRY
    TableEntry ITEM_MAGOST_BERRY
    TableEntry ITEM_RABUTA_BERRY
    TableEntry ITEM_NOMEL_BERRY
    TableEntry ITEM_SPELON_BERRY
    TableEntry ITEM_PAMTRE_BERRY
    TableEntry ITEM_WATMEL_BERRY
    TableEntry ITEM_DURIN_BERRY
    TableEntry ITEM_BELUE_BERRY
    TableEntry ITEM_OCCA_BERRY
    TableEntry ITEM_PASSHO_BERRY
    TableEntry ITEM_WACAN_BERRY
    TableEntry ITEM_RINDO_BERRY
    TableEntry ITEM_YACHE_BERRY
    TableEntry ITEM_CHOPLE_BERRY
    TableEntry ITEM_KEBIA_BERRY
    TableEntry ITEM_SHUCA_BERRY
    TableEntry ITEM_COBA_BERRY
    TableEntry ITEM_PAYAPA_BERRY
    TableEntry ITEM_TANGA_BERRY
    TableEntry ITEM_CHARTI_BERRY
    TableEntry ITEM_KASIB_BERRY
    TableEntry ITEM_HABAN_BERRY
    TableEntry ITEM_COLBUR_BERRY
    TableEntry ITEM_BABIRI_BERRY
    TableEntry ITEM_CHILAN_BERRY
    TableEntry ITEM_LIECHI_BERRY
    TableEntry ITEM_GANLON_BERRY
    TableEntry ITEM_SALAC_BERRY
    TableEntry ITEM_PETAYA_BERRY
    TableEntry ITEM_APICOT_BERRY
    TableEntry ITEM_LANSAT_BERRY
    TableEntry ITEM_STARF_BERRY
    TableEntry ITEM_ENIGMA_BERRY
    TableEntry ITEM_MICLE_BERRY
    TableEntry ITEM_CUSTAP_BERRY
    TableEntry ITEM_JABOCA_BERRY
    TableEntry ITEM_ROWAP_BERRY
    TableEntry TABLE_END

Basic_CheckTailwind:
    ; If Trick Room is currently active or Tailwind is already active for the attacker''s side
    ; of the field, score -10.
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, ScoreMinus12
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_TAILWIND, ScoreMinus12
    PopOrEnd 

Basic_CheckAcupressure:
    ; If any of the attacker''s stat stages are already at +6, score -10.
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 12, ScoreMinus10
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 12, ScoreMinus10
    PopOrEnd

Basic_CheckMetalBurst:
    ; If the target is immune to Metal Burst due to its typing (?), score -10.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10

    ; If the target is holding a Lagging Tail, score -10.
	LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedInTable Basic_LaggingTailItemEffect, ScoreMinus10

    ; If the attacker is holding a Lagging Tail, terminate.
	LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedInTable Basic_LaggingTailItemEffect, Basic_CheckMetalBurst_Terminate

    ; If the attacker is faster than the target, score -10.
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, ScoreMinus10

Basic_CheckMetalBurst_Terminate:
    PopOrEnd
	
Basic_LaggingTailItemEffect:
    TableEntry HOLD_EFFECT_PRIORITY_DOWN
    TableEntry TABLE_END

Basic_CheckEmbargo:
    ; If the target is already under the respective effect, score -10.
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_EMBARGO, ScoreMinus10

    ; If a recyclable item for the target''s side exists, terminate.
    LoadRecycleItem AI_BATTLER_DEFENDER
    IfLoadedEqualTo ITEM_NONE, Basic_CheckEmbargo_Terminate

    ; If the battle is taking place in the Frontier, score -10.
    LoadBattleType 
    IfLoadedMask BATTLE_TYPE_FRONTIER, ScoreMinus10

Basic_CheckEmbargo_Terminate:
    PopOrEnd 

Basic_CheckFling:
    ; If the target is immune to the move due to its typing (?), score -12.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus12

    ; If Fling would have 0 base power, score -20.
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_NONE, ScoreMinus20
    LoadFlingPower AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus20

    ; Branch according to possible status effects.
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedInTable Basic_FlingItems_Poison, Basic_FlingPoison
    IfLoadedInTable Basic_FlingItems_Burn, Basic_FlingBurn
    IfLoadedInTable Basic_FlingItems_Paralyze, Basic_FlingParalyze
    PopOrEnd 

Basic_FlingPoison:
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, Basic_FlingPoison_AttackerChecks
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, Basic_FlingPoison_AttackerChecks
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_HEAL, Basic_FlingPoison_AttackerChecks
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_POISON, Basic_FlingPoison_AttackerChecks
    IfLoadedEqualTo TYPE_STEEL, Basic_FlingPoison_AttackerChecks
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_POISON, Basic_FlingPoison_AttackerChecks
    IfLoadedEqualTo TYPE_STEEL, Basic_FlingPoison_AttackerChecks
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_IMMUNITY, Basic_FlingPoison_AttackerChecks
    IfLoadedEqualTo ABILITY_POISON_HEAL, Basic_FlingPoison_AttackerChecks
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Basic_FlingPoison_AttackerChecks
    PopOrEnd 

Basic_FlingPoison_AttackerChecks:
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_SAFEGUARD, ScoreMinus5
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, ScoreMinus5
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_POISON, ScoreMinus5
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus5
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_POISON, ScoreMinus5
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus5
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus5
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus5
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus5
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus5
    AddToMoveScore 3
    PopOrEnd 

Basic_FlingBurn:
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, Basic_FlingBurn_AttackerChecks
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, Basic_FlingBurn_AttackerChecks
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, Basic_FlingBurn_AttackerChecks
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, Basic_FlingBurn_AttackerChecks
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Basic_FlingBurn_AttackerChecks
    IfLoadedEqualTo ABILITY_WATER_VEIL, Basic_FlingBurn_AttackerChecks
    PopOrEnd 

Basic_FlingBurn_AttackerChecks:
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_SAFEGUARD, ScoreMinus5
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, ScoreMinus5
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus5
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus5
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus5
    IfLoadedEqualTo ABILITY_WATER_VEIL, ScoreMinus5
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus5
    AddToMoveScore 3
    PopOrEnd 

Basic_FlingParalyze:
    ; If the target cannot be Paralyzed, score -5.
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus5
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus5
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LIMBER, ScoreMinus5
    PopOrEnd 

Basic_FlingItems_Poison:
    TableEntry HOLD_EFFECT_PSN_USER
    TableEntry HOLD_EFFECT_STRENGTHEN_POISON
    TableEntry TABLE_END

Basic_FlingItems_Burn:
    TableEntry HOLD_EFFECT_BRN_USER
    TableEntry TABLE_END

Basic_FlingItems_Paralyze:
    TableEntry HOLD_EFFECT_PIKA_SPATK_UP
    TableEntry TABLE_END

Basic_CheckCanPsychoShift:
    ; If the attacker does not have a status condition or the target already has a status
    ; condition, score -10.
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, ScoreMinus10
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus10

    ; If the target is protected by Safeguard, score -10.
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus10

    ; Branch according to the attacker''s status condition.
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, Basic_PsychoShift_Poison
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_BURN, Basic_PsychoShift_Burn
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_PARALYSIS, Basic_PsychoShift_Paralysis
    GoTo Basic_PsychoShift_Terminate

Basic_PsychoShift_Poison:
    ; If the attacker has Poison Heal, score -10.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus10

    ; If the target is immune to the effects of poison for any reason, score -10.
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_POISON, ScoreMinus10
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus10
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_POISON, ScoreMinus10
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus10
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus10
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    GoTo Basic_PsychoShift_Terminate

Basic_PsychoShift_Burn:
    ; If the target is immune to the effects of burn for any reason, score -10.
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus10
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    IfLoadedEqualTo ABILITY_WATER_VEIL, ScoreMinus10
    GoTo Basic_PsychoShift_Terminate

Basic_PsychoShift_Paralysis:
    ; If the target''s ability is Limber, score -10.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LIMBER, ScoreMinus10

Basic_PsychoShift_Terminate:
    PopOrEnd 

Basic_CheckHealBlock:
    ; If the target is already under the effect, score -10.
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_HEAL_BLOCK, ScoreMinus10
    PopOrEnd 

Basic_CheckPowerTrick:
    ; If the attacker is already under the effect, score -10.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_POWER_TRICK, ScoreMinus10
    PopOrEnd 

Basic_CheckGastroAcid:
    ; If the target is already under the effect, score -10.
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_ABILITY_SUPPRESSED, ScoreMinus10

    ; If the target has any of the following abilities, score -10.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MULTITYPE, ScoreMinus10
    IfLoadedEqualTo ABILITY_TRUANT, ScoreMinus10
    IfLoadedEqualTo ABILITY_SLOW_START, ScoreMinus10
    IfLoadedEqualTo ABILITY_STENCH, ScoreMinus10
    IfLoadedEqualTo ABILITY_RUN_AWAY, ScoreMinus10
    IfLoadedEqualTo ABILITY_PICKUP, ScoreMinus10
    IfLoadedEqualTo ABILITY_HONEY_GATHER, ScoreMinus10
    PopOrEnd 

Basic_CheckLuckyChant:
    ; If the attacker is already under the effect, score -10.
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_LUCKY_CHANT, ScoreMinus10
    PopOrEnd 

Basic_CheckCopycat:
    ; If it''s the first turn of the battle and the attacker is faster than its target, score -10.
    LoadTurnCount 
    IfLoadedNotEqualTo 0, Basic_CheckCopycat_Terminate
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, ScoreMinus10

Basic_CheckCopycat_Terminate:
    PopOrEnd 

Basic_CheckPowerSwap:
    ; If Power Swap would result in a net-negative change to stat stages for both Attack
    ; and SpAttack, score -10.
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK
    IfLoadedLessThan 1, Basic_CheckGuardSwap_SpAttack
    GoTo Basic_CheckPowerSwap_Terminate

Basic_CheckGuardSwap_SpAttack:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK
    IfLoadedLessThan 1, ScoreMinus10

Basic_CheckPowerSwap_Terminate:
    PopOrEnd 

Basic_CheckGuardSwap:
    ; If Guard Swap would result in a net-negative change to stat stages for both Defense
    ; and SpDefense, score -10.
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE
    IfLoadedLessThan 1, Basic_CheckGuardSwap_SpDefense
    GoTo Basic_CheckGuardSwap_Terminate

Basic_CheckGuardSwap_SpDefense:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE
    IfLoadedLessThan 1, ScoreMinus10

Basic_CheckGuardSwap_Terminate:
    PopOrEnd 

Basic_CheckLastResort:
    ; If the attacker has yet to use all of its other moves, score -10.
    IfCanUseLastResort AI_BATTLER_ATTACKER, Basic_CheckLastResort_Terminate
    AddToMoveScore -10

Basic_CheckLastResort_Terminate:
    PopOrEnd 

Basic_CheckWorrySeed:
    ; If the target has any of the following abilities, score -10.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_TRUANT, ScoreMinus10
    IfLoadedEqualTo ABILITY_INSOMNIA, ScoreMinus10
    IfLoadedEqualTo ABILITY_VITAL_SPIRIT, ScoreMinus10
    IfLoadedEqualTo ABILITY_MULTITYPE, ScoreMinus10

    ; If the target is asleep and does not know either Sleep Talk or Snore, score -10.
    IfNotStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Basic_CheckWorrySeed_Terminate
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_SLEEP_TALK, Basic_CheckWorrySeed_Terminate
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_SNORE, Basic_CheckWorrySeed_Terminate
    AddToMoveScore -10

Basic_CheckWorrySeed_Terminate:
    PopOrEnd 

Basic_CheckToxicSpikes:
    ; If the target''s side of the field already has 2 layers of Toxic Spikes, score -10.
    LoadSpikesLayers AI_BATTLER_DEFENDER, SIDE_CONDITION_TOXIC_SPIKES
    IfLoadedEqualTo 2, ScoreMinus10

    ; If the target is the last battler, score -10.
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, ScoreMinus10
    PopOrEnd 
    PopOrEnd 

Basic_CheckAquaRing:
    ; If the attacker is already under the effect, score -10.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_AQUA_RING, ScoreMinus10
    PopOrEnd 

Basic_CheckMagnetRise:
    ; If the attacker is already under the effect, score -10.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_MAGNET_RISE, ScoreMinus10

    ; If the attacker''s ability is Levitate, score -10.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_LEVITATE, ScoreMinus10

    ; If either of the attacker''s types are Flying, score -10.
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_FLYING, ScoreMinus10
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_FLYING, ScoreMinus10
    PopOrEnd

Basic_CheckRapidSpin:
    ; If the user''s speed is not at +6 or their side of the field has Hazards, or if the user
    ; is under the effect of a binding move or Leech Seed, ignore

    AddToMoveScore -3
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED, ScorePlus5
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_BIND, ScorePlus5
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_SPIKES, ScorePlus5
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_STEALTH_ROCK, ScorePlus5
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_TOXIC_SPIKES, ScorePlus5
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Basic_RapidSpin_TryScorePlus4
    IfSpeedCompareEqualTo COMPARE_SPEED_TIE, ScorePlus5
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 12, ScoreMinus6
    GoTo Basic_RapidSpin_End

Basic_RapidSpin_TryScorePlus4:
    IfRandomLessThan 160, Basic_RapidSpin_End
    AddToMoveScore 4

Basic_RapidSpin_End:
    PopOrEnd

Basic_CheckTrickRoom:
    ; If the attacker is faster than the target, score -10.
    ; Treat speed ties as being faster than the target.
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, ScoreMinus10
    IfSpeedCompareEqualTo COMPARE_SPEED_TIE, ScoreMinus10
    PopOrEnd 

Basic_CheckCaptivate:
    ; If the target''s ability is any of Oblivious, Clear Body, or White Smoke and the attacker''s
    ; ability is not Mold Breaker, score -10.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_CheckCaptivate_CheckGender
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_OBLIVIOUS, ScoreMinus10
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus10
    IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus10

Basic_CheckCaptivate_CheckGender:
    ; If the target and the attacker share gender or the target has no gender, score -10.
    LoadGender AI_BATTLER_ATTACKER
    IfLoadedEqualTo GENDER_MALE, Basic_CheckCaptivate_CheckMale
    IfLoadedEqualTo GENDER_FEMALE, Basic_CheckCaptivate_CheckFemale
    GoTo ScoreMinus10

Basic_CheckCaptivate_CheckMale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_FEMALE, Basic_CheckCaptivate_CheckStatStage
    GoTo ScoreMinus10

Basic_CheckCaptivate_CheckFemale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_MALE, Basic_CheckCaptivate_CheckStatStage
    GoTo ScoreMinus10

Basic_CheckCaptivate_CheckStatStage:
    ; If the target is already at -6, score -10.
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 1, ScoreMinus10
    PopOrEnd 

Basic_CheckStealthRock:
    ; If the target''s side of the field is already under the effect of Stealth Rock, score -10.
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STEALTH_ROCK, ScoreMinus10

    ; If the target is on their last Pokemon, score -10.
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, ScoreMinus10
    PopOrEnd 
	
Basic_CheckStickyWeb:
    ; If the target''s side of the field is already under the effect of Sticky Web, score -10.
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STICKY_WEB, ScoreMinus10

    ; If the target is on their last Pokemon, score -10.
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, ScoreMinus10
    PopOrEnd

Basic_CheckLunarDance:
    ; Start at -20
    AddToMoveScore -20

    ; If the attacker is on their last Pokemon, score additional -10.
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus10

    ; If none of the attacker''s party members are statused, at less than 100% HP, or at
    ; less than full PP on all of their moves, score -10.
    IfAnyPartyMemberIsWounded AI_BATTLER_ATTACKER, Basic_CheckLunarDance_Terminate
    IfPartyMemberStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Basic_CheckLunarDance_Terminate
    IfAnyPartyMemberUsedPP AI_BATTLER_ATTACKER, Basic_CheckLunarDance_Terminate
    GoTo ScoreMinus10

Basic_CheckLunarDance_Terminate:
    PopOrEnd 

Basic_Wish:
    IfWishActive AI_BATTLER_ATTACKER, ScoreMinus20
    IfRandomLessThan 128, ScorePlus1
    GoTo Basic_Wish_End

Basic_Wish_End:
    PopOrEnd

Basic_Taunt:
    IfTargetIsNotTaunted Basic_Taunt_CheckAbility
    GoTo ScoreMinus10

Basic_Taunt_CheckAbility:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Basic_Taunt_CheckFirstTurn
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus10
    GoTo Basic_Taunt_CheckFirstTurn

Basic_Taunt_CheckFirstTurn:
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, ScorePlus3
    IfRandomLessThan 128, Basic_Taunt_End
    AddToMoveScore 1
    GoTo Basic_Taunt_End

Basic_Taunt_End:
    PopOrEnd

TerminateBasic:
    PopOrEnd 

    .endif