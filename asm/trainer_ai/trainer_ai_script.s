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

    .global gTrainerAITable

gTrainerAITable:

FlagTable:
    LabelDistance Terminate,           FlagTable ; AI_FLAG_BASIC
    LabelDistance EvalAttack_Main,     FlagTable ; AI_FLAG_EVAL_ATTACK
    LabelDistance Expert_Main,         FlagTable ; AI_FLAG_EXPERT
    LabelDistance SetupFirstTurn_Main, FlagTable ; AI_FLAG_SETUP_FIRST_TURN
    LabelDistance Risky_Main,          FlagTable ; AI_FLAG_RISKY
    LabelDistance DamagePriority_Main, FlagTable ; AI_FLAG_DAMAGE_PRIORITY
    LabelDistance BatonPass_Main,      FlagTable ; AI_FLAG_BATON_PASS
    LabelDistance TagStrategy_Main,    FlagTable ; AI_FLAG_TAG_STRATEGY
    LabelDistance CheckHP_Main,        FlagTable ; AI_FLAG_CHECK_HP
    LabelDistance Weather_Main,        FlagTable ; AI_FLAG_WEATHER
    LabelDistance Harrassment_Main,    FlagTable ; AI_FLAG_HARRASSMENT
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_11
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_12
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_13
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_14
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_15
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_16
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_17
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_18
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_19
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_20
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_21
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_22
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_23
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_24
    LabelDistance Terminate,           FlagTable ; AI_FLAG_UNUSED_25
    LabelDistance Terminate,           FlagTable ; AI_FLAG_JOKE
    LabelDistance Terminate,           FlagTable ; AI_FLAG_PRESCIENT
    LabelDistance Terminate,           FlagTable ; AI_FLAG_OMNISCIENT
    LabelDistance RoamingPokemon_Main, FlagTable ; AI_FLAG_ROAMING_POKEMON
    LabelDistance Safari_Main,         FlagTable ; AI_FLAG_SAFARI
    LabelDistance CatchTutorial_Main,  FlagTable ; AI_FLAG_CATCH_TUTORIAL

Expert_Main:
    ; This flag will never target its partner.
    IfTargetIsPartner Terminate

    ; Evaluate moves which match a known effect according to this jump table.
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_SLEEP, Expert_StatusSleep
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECOVER_HALF_DAMAGE_DEALT, Expert_DrainMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_DEFENSE, Expert_Explosion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP, Expert_DreamEater
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_COPY_MOVE, Expert_MirrorMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_UP, Expert_StatusAttackUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_UP, Expert_StatusDefenseUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_UP, Expert_StatusSpeedUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_UP, Expert_StatusSpAttackUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_UP, Expert_StatusSpDefenseUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_SP_DEF_UP, Expert_StatusSpAttackUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ACC_UP, Expert_StatusAccuracyUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_UP, Expert_StatusEvasionUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BYPASS_ACCURACY, Expert_BypassAccuracyMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DOWN, Expert_StatusAttackDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_DOWN, Expert_StatusDefenseDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_DOWN, Expert_StatusSpeedDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_DOWN, Expert_StatusSpAttackDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_DOWN, Expert_StatusSpDefenseDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ACC_DOWN, Expert_StatusAccuracyDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_DOWN, Expert_StatusEvasionDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RESET_STAT_CHANGES, Expert_Haze
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BIDE, Expert_Bide
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FORCE_SWITCH, Expert_Phaze
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FORCE_SWITCH_HIT, Expert_Phaze
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CONVERSION, Expert_Conversion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RESTORE_HALF_HP, Expert_Recovery
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_BADLY_POISON, Expert_Toxic
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_LIGHT_SCREEN, Expert_LightScreen
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_LIGHT_SCREEN_HIT, Expert_LightScreen
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_REST, Expert_Rest
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ONE_HIT_KO, Expert_OHKOMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT, Expert_ChargeTurnNoInvuln
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_HP, Expert_SuperFang
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BIND_HIT, Expert_BindingMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIGH_CRITICAL, Expert_HighCritical
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECOIL_QUARTER, Expert_RecoilMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_CONFUSE, Expert_StatusConfuse
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_UP_2, Expert_StatusAttackUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_UP_2, Expert_StatusDefenseUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_UP_2, Expert_StatusSpeedUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_UP_2, Expert_StatusSpAttackUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_UP_2, Expert_StatusSpDefenseUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ACC_UP_2, Expert_StatusAccuracyUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_UP_2, Expert_StatusEvasionUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DOWN_2, Expert_StatusAttackDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_DOWN_2, Expert_StatusDefenseDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_DOWN_2, Expert_StatusSpeedDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_DOWN_2, Expert_StatusSpAttackDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_DEF_DOWN_2, Expert_StatusSpDefenseDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_DOWN_2, Expert_StatusAccuracyDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ACC_DOWN_2, Expert_StatusEvasionDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_REFLECT, Expert_Reflect
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_POISON, Expert_StatusPoison
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_PARALYZE, Expert_StatusParalyze
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION, Expert_Swagger
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LOWER_SPEED_HIT, Expert_SpeedDownOnHit
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT_FLINCH, Expert_ChargeTurnNoInvuln
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PRIORITY_NEG_1_BYPASS_ACCURACY, Expert_VitalThrow
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_SUBSTITUTE, Expert_Substitute
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECHARGE_AFTER, Expert_RechargeTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_LEECH_SEED, Expert_LeechSeed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DISABLE, Expert_Disable
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DISABLE_HIT, Expert_Disable
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_COUNTER, Expert_Counter
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ENCORE, Expert_Encore
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_AVERAGE_HP, Expert_PainSplit
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_NIGHTMARE, Expert_Nightmare
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_NEXT_ATTACK_ALWAYS_HITS, Expert_LockOn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP, Expert_SleepTalk
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER, Expert_DestinyBond
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INCREASE_POWER_WITH_LESS_HP, Expert_Reversal
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CURE_PARTY_STATUS, Expert_HealBell
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STEAL_HELD_ITEM, Expert_Thief
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_ESCAPE, Expert_BindingMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EVA_UP_2_MINIMIZE, Expert_StatusEvasionUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CURSE, Expert_Curse
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PROTECT, Expert_Protect
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_SPIKES, Expert_Spikes
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPIKES_MULTI_HIT, Expert_Spikes
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_IGNORE_EVASION_REMOVE_GHOST_IMMUNE, Expert_Foresight
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SURVIVE_WITH_1_HP, Expert_Endure
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PASS_STATS_AND_STATUS, Expert_BatonPass
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_BEFORE_SWITCH, Expert_Pursuit
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN, Expert_Synthesis
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MEDITATE, Expert_StatusAttackUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_RAIN, Expert_RainDance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_SUN, Expert_SunnyDay
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP, Expert_BellyDrum
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_COPY_STAT_CHANGES, Expert_PsychUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MIRROR_COAT, Expert_MirrorCoat
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_CHARGE_TURN_DEF_UP, Expert_ChargeTurnNoInvuln
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SKIP_CHARGE_TURN_IN_SUN, Expert_SolarBeam
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FLY, Expert_ChargeTurnWithInvuln
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_STICKY_WEB, Expert_StickyWeb
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ALWAYS_FLINCH_FIRST_TURN_ONLY, Expert_FakeOut
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPIT_UP, Expert_SpitUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWALLOW, Expert_Recovery
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_HAIL, Expert_Hail
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION, Expert_Flatter
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAINT_AND_ATK_SP_ATK_DOWN_2, Expert_Explosion
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_WHEN_STATUSED, Expert_Facade
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT, Expert_FocusPunch
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_AND_CURE_PARALYSIS, Expert_SmellingSalts
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWITCH_HELD_ITEMS, Expert_Trick
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_COPY_ABILITY, Expert_ChangeUserAbility
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_GROUND_TRAP_USER_CONTINUOUS_HEAL, Expert_Ingrain
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LOWER_OWN_ATK_AND_DEF, Expert_Superpower
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_APPLY_MAGIC_COAT, Expert_MagicCoat
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECYCLE, Expert_Recycle
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_IF_HIT, Expert_Revenge
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_REMOVE_SCREENS, Expert_BrickBreak
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_REMOVE_HELD_ITEM, Expert_KnockOff
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_HP_EQUAL_TO_USER, Expert_Endeavor
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DECREASE_POWER_WITH_LESS_USER_HP, Expert_WaterSpout
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWITCH_ABILITIES, Expert_SkillSwap
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE, Expert_Imprison
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_STATUS, Expert_Refresh
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STEAL_STATUS_MOVE, Expert_Snatch
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECOIL_THIRD, Expert_RecoilMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIGH_CRITICAL_BURN_HIT, Expert_HighCritical
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_ELECTRIC_DAMAGE, Expert_MudSport
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_USER_SP_ATK_DOWN_2, Expert_Overheat
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DEF_DOWN, Expert_StatusDefenseDown
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_SPD_UP, Expert_StatusSpDefenseUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_DEF_UP, Expert_StatusDefenseUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIGH_CRITICAL_POISON_HIT, Expert_HighCritical
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_FIRE_DAMAGE, Expert_WaterSport
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_SP_DEF_UP, Expert_StatusSpDefenseUp
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ATK_SPD_UP, Expert_DragonDance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE, Expert_Recovery
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_GRAVITY, Expert_Gravity
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_IGNORE_EVATION_REMOVE_DARK_IMMUNE, Expert_Disable
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_HEAL_SLEEP, Expert_WakeUpSlap
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SPEED_DOWN_HIT, Expert_HammerArm
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_POWER_BASED_ON_LOW_SPEED, Expert_GyroBall
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON, Expert_HealingWish
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_WHEN_BELOW_HALF, Expert_Brine
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_REMOVE_PROTECT, Expert_Feint
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_EAT_BERRY, Expert_Pluck
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS, Expert_Tailwind
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RANDOM_STAT_UP_2, Expert_Acupressure
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_METAL_BURST, Expert_MetalBurst
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWITCH_HIT, Expert_UTurn
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWITCH_HIT_NO_ANIM, Expert_UTurn
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DEF_SPD_DOWN_HIT, Expert_CloseCombat
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_IF_MOVING_SECOND, Expert_Payback
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_IF_TARGET_HIT, Expert_Assurance
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_ITEM_USE, Expert_Embargo
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FLING, Expert_Fling
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TRANSFER_STATUS, Expert_PsychoShift
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIGHER_POWER_WHEN_LOW_PP, Expert_TrumpCard
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_HEALING, Expert_HealBlock
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_HP, Expert_WringOut
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWAP_ATK_DEF, Expert_PowerTrick
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SUPRESS_ABILITY, Expert_GastroAcid
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PREVENT_CRITS, Expert_LuckyChant
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_USE_MOVE_FIRST, Expert_MeFirst
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_USE_LAST_USED_MOVE, Expert_Copycat
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWAP_ATK_SP_ATK_STAT_CHANGES, Expert_PowerSwap
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWAP_DEF_SP_DEF_STAT_CHANGES, Expert_GuardSwap
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_STAT_UP, Expert_Punishment
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAIL_IF_NOT_USED_ALL_OTHER_MOVES, Expert_LastResort
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SET_ABILITY_TO_INSOMNIA, Expert_WorrySeed
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING, Expert_SuckerPunch
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TOXIC_SPIKES, Expert_ToxicSpikes
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SWAP_STAT_CHANGES, Expert_HeartSwap
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RESTORE_HP_EVERY_TURN, Expert_AquaRing
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_GIVE_GROUND_IMMUNITY, Expert_MagnetRise
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECOIL_BURN_HIT, Expert_RecoilMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DIVE, Expert_ChargeTurnWithInvuln
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DIG, Expert_ChargeTurnWithInvuln
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN, Expert_Defog
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TRICK_ROOM, Expert_TrickRoom
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BLIZZARD, Expert_Blizzard
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECOIL_PARALYZE_HIT, Expert_RecoilMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BOUNCE, Expert_ChargeTurnWithInvuln
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER, Expert_Captivate
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STEALTH_ROCK, Expert_StealthRock
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RECOIL_HALF, Expert_RecoilMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON, Expert_HealingWish
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_SHADOW_FORCE, Expert_ShadowForce
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INFATUATE_HIT, Expert_LovelyPunch
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_POWER_EACH_TURN, Expert_FuryCutter
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RAISE_SP_ATK_HIT, Expert_ChargeBeam
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_INCREASE_POWER_WITH_WEIGHT, Expert_WeightMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HEAL_IN_3_TURNS, Expert_Wish
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TAUNT, Expert_Taunt
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HOWL, Expert_StatusAttackUp
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_BULLDOZE, Expert_SpeedDownOnHit
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STOCKPILE, Expert_Stockpile
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_TORMENT, Expert_Torment
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RAISE_DEF_HIT, Expert_StatusDefenseUp
	IfCurrentMoveEffectEqualTo BATTLE_EFFECT_MULTI_HIT_TEN, Expert_ChumRush

    ; All other moves have no additional logic.
    PopOrEnd 

Expert_StatusSleep:
    ; If the attacker knows a move which requires the target to be asleep (Dream Eater or Nightmare
    ; effects), 50% chance of score +1.
	IfEnemySleepClauseActive ScoreMinus12
    IfMoveBlockedBySoundproof ScoreMinus12
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SYNCHRONIZE, Expert_StatusSleep_TryScoreMinus3
    IfLoadedEqualTo ABILITY_GUTS, Expert_StatusSleep_TryScoreMinus3
    IfLoadedEqualTo ABILITY_MARVEL_SCALE, Expert_StatusSleep_TryScoreMinus3
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_SLP_RESTORE, Expert_StatusSleep_TryScoreMinus3
    IfLoadedEqualTo HOLD_EFFECT_STATUS_RESTORE, Expert_StatusSleep_TryScoreMinus3
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP, Expert_StatusSleep_TryScoreMinus3
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_UPROAR, Expert_StatusSleep_TryScoreMinus3
    IfMoveEffectKnown AI_BATTLER_DEFENDER_PARTNER, BATTLE_EFFECT_UPROAR, Expert_StatusSleep_TryScoreMinus3
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_DOUBLE_POWER_HEAL_SLEEP, Expert_StatusSleep_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP, Expert_StatusSleep_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_STATUS_NIGHTMARE, Expert_StatusSleep_TryScorePlus1
    GoTo Expert_StatusSleep_End

Expert_StatusSleep_TryScoreMinus3:
    IfRandomLessThan 8, Expert_StatusSleep_End
    AddToMoveScore -1
    IfRandomLessThan 32, Expert_StatusSleep_End
    AddToMoveScore -1
    IfRandomLessThan 128, Expert_StatusSleep_End
    AddToMoveScore -1
    GoTo Expert_StatusSleep_End

Expert_StatusSleep_TryScorePlus1:
    IfRandomLessThan 128, Expert_StatusSleep_End
    AddToMoveScore 1

Expert_StatusSleep_End:
    PopOrEnd 

Expert_DrainMove:
    ; If current move kills, score +3.
    ;
    ; If HP is below 50%, score +3. If HP is below 75%, score +1.
    ;
    ; If move 4x resisted, score -5. If move is 2x resist, score -3.
    ;
    ; If move is super effective, score +2.
    ;
    ; If move is neutral or STAB, 47.5% chance for score +1.
    IfCurrentMoveKills ROLL_FOR_DAMAGE, Expert_DrainMove_TryScorePlus3
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_DrainMove_TryScorePlus3
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 75, Expert_DrainMove_TryScorePlus1
    GoTo Expert_DrainMove_CheckEffectiveness

Expert_DrainMove_CheckEffectiveness:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LIQUID_OOZE, Expert_DrainMove_TryScoreMinus5
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus12
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_DrainMove_TryScoreMinus5
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_DrainMove_TryScoreMinus3
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, ScorePlus2
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, ScorePlus2
    IfRandomLessThan 96, Expert_DrainMove_End
    AddToMoveScore 1
    GoTo Expert_DrainMove_End

Expert_DrainMove_TryScorePlus3:
    AddToMoveScore 1
    IfRandomLessThan 32, Expert_DrainMove_End
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_DrainMove_End
    AddToMoveScore 1
    GoTo Expert_DrainMove_CheckEffectiveness

Expert_DrainMove_TryScorePlus1:
    IfRandomLessThan 128, Expert_DrainMove_End
    AddToMoveScore 1
    GoTo Expert_DrainMove_CheckEffectiveness

Expert_DrainMove_TryScoreMinus5:
    IfRandomLessThan 16, Expert_DrainMove_End
    AddToMoveScore -5
    GoTo Expert_DrainMove_End

Expert_DrainMove_TryScoreMinus3:
    IfRandomLessThan 64, Expert_DrainMove_End
    AddToMoveScore -3
    GoTo Expert_DrainMove_End

Expert_DrainMove_End:
    PopOrEnd 

Expert_Explosion:
    ; If defender is behind substitute, score -12.
    ; If defender has an invulnerable turn move, 95% chance for score -12.
    ;
    ; If defender has +2 or higher evasion stage, check if it matters. If so, 95% chance for score -12.
    ; If attacker has -2 or lower accuracy stage, check if it matters. If so, 95% chance for score -12.
    ;
    ; If the defender knows protect, attempt to juke it randomly based on protect chain.
    ;
    ; 10% chance for flat score -1.
    ;
    ; If Explosion is not highest damage, score -12.
    ; If Explosion is highest damage and it kills, score +1.
    ; If Explosion does not kill, 50% chance for score -1.
    ;
    ; If Explosion kills, check the target HP. If the target HP is lower than 20%, score -2.
    ; If the target HP is between 20% and 33%, 95% chance for score -3. Else, score +1.
    ;
    ; If attacker is faster than target, 33% chance for score +1.
    ; If attacker is slower than target, if the target knows Substitute, 95% chance for score -12.
    ; Otherwise, 25% chance for score -1.
    ;
    ; If attacker is less than 20% HP, 87.5% chance for score +1.
    ; If attacker is betweem 20% and 66% HP, 66% chance for score +1.
    ; Otherwise, 75% chance for score -1.
    ;
    ; If it is the attacker''s first turn out, 66% chance for score -5 after all other factors.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_SUBSTITUTE, ScoreMinus12
    IfBattlerHasInvulnerableMove AI_BATTLER_DEFENDER, Expert_Explosion_TryScoreMinus12
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 7, Expert_Explosion_CheckAttackerAcc
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 5, Expert_Explosion_CheckAttackerAcc
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_PROTECT, Expert_Explosion_ProtectJuke
    IfRandomLessThan 230, Expert_Explosion_CheckDamage
    AddToMoveScore -1
    GoTo Expert_Explosion_CheckDamage

Expert_Explosion_TryScoreMinus12:
    IfRandomLessThan 1, Expert_Explosion_End
    AddToMoveScore -1
    IfRandomLessThan 12, Expert_Explosion_End
    AddToMoveScore -11
    GoTo Expert_Explosion_End

Expert_Explosion_CheckAttackerAcc:
    IfAbilityInPlay ABILITY_NO_GUARD, Expert_Explosion_CheckDamage
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_FORESIGHT, Expert_Explosion_CheckDamage
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LOCK_ON, Expert_Explosion_CheckDamage
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 5, Expert_Explosion_TryScoreMinus12
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_UNAWARE, Expert_Explosion_CheckDamage
    GoTo Expert_Explosion_TryScoreMinus12

Expert_Explosion_ProtectJuke:
    LoadProtectChain AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, Expert_Explosion_ProtectJuke_0
    IfLoadedGreaterThan 0, Expert_Explosion_ProtectJuke_1
    GoTo Expert_Explosion_CheckDamage

Expert_Explosion_ProtectJuke_0:
    IfRandomLessThan 128, ScoreMinus12
    GoTo Expert_Explosion_CheckDamage
   
Expert_Explosion_ProtectJuke_1:
    IfRandomLessThan 85, ScoreMinus12
    GoTo Expert_Explosion_CheckDamage

Expert_Explosion_CheckDamage:
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NOT_HIGHEST_DAMAGE, ScoreMinus12
    AddToMoveScore 1
    IfCurrentMoveKills ROLL_FOR_DAMAGE, Expert_Explosion_CheckTargetHP
    AddToMoveScore -1
    IfRandomLessThan 128, Expert_Explosion_CheckTargetHP
    AddToMoveScore -1
    GoTo Expert_Explosion_CheckTargetHP

Expert_Explosion_CheckTargetHP:
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 20, ScoreMinus2
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 33, Expert_Explosion_TryScoreMinus3
    AddToMoveScore 1
    GoTo Expert_Explosion_CheckSpeed

Expert_Explosion_TryScoreMinus3:
    IfRandomLessThan 1, Expert_Explosion_End
    AddToMoveScore -1
    IfRandomLessThan 16, Expert_Explosion_End
    AddToMoveScore -2
    GoTo Expert_Explosion_End

Expert_Explosion_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_Explosion_SpeedIncentive
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SET_SUBSTITUTE, Expert_Explosion_TryScoreMinus12
    IfRandomLessThan 192, Expert_Explosion_CheckUserHP
    AddToMoveScore -1
    GoTo Expert_Explosion_CheckUserHP

Expert_Explosion_SpeedIncentive:
    IfRandomLessThan 170, Expert_Explosion_CheckUserHP
    AddToMoveScore 1
    GoTo Expert_Explosion_CheckUserHP

Expert_Explosion_CheckUserHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 20, Expert_Explosion_LowHPIncentive
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 66, Expert_Explosion_MediumHPIncentive
    IfRandomLessThan 64, Expert_Explosion_CheckTurnCount
    AddToMoveScore -1
    GoTo Expert_Explosion_CheckTurnCount

Expert_Explosion_LowHPIncentive:
    IfRandomLessThan 32, Expert_Explosion_CheckTurnCount
    AddToMoveScore 1
    GoTo Expert_Explosion_CheckTurnCount
    
Expert_Explosion_MediumHPIncentive:
    IfRandomLessThan 85, Expert_Explosion_CheckTurnCount
    AddToMoveScore 1
    GoTo Expert_Explosion_CheckTurnCount
    
Expert_Explosion_CheckTurnCount:
    LoadTurnCount
    IfLoadedEqualTo 0, Expert_Explosion_FirstTurn
    GoTo Expert_Explosion_End

Expert_Explosion_FirstTurn:
    IfRandomLessThan 85, Expert_Explosion_End
    AddToMoveScore -5
    GoTo Expert_Explosion_End

Expert_Explosion_End:
    PopOrEnd 

Expert_DreamEater:
    ; Deincentivize use when not effective.
    ;
    ; Cheat and peek the sleep turns. If they wake up this turn, check for
    ; outspeed. If we outspeed, 66% chance for +3 score. If we''re slower, score -12.
    ; If they don''t wake up this turn, 66% chance for score +3.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus12
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, ScoreMinus3
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, ScoreMinus1
    LoadSleepTurns AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, Expert_DreamEater_TryScorePlus3
    IfLoadedEqualTo 1, Expert_DreamEater_CheckSpeed
    IfLoadedEqualTo 0, ScoreMinus12
    GoTo Expert_DreamEater_End

Expert_DreamEater_TryScorePlus3:
    IfRandomLessThan 85, Expert_DreamEater_End
    AddToMoveScore 3
    GoTo Expert_DreamEater_End

Expert_DreamEater_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_DreamEater_TryScorePlus3
    AddToMoveScore -12
    GoTo Expert_DreamEater_End

Expert_DreamEater_ScoreMinus1:
    AddToMoveScore -1

Expert_DreamEater_End:
    PopOrEnd 

Expert_MirrorMove:
    ; If the attacker is faster than its target and the last-used move by that target is in the below
    ; list, 50% chance of score +2.
    ;
    ; Otherwise, if the last-used move by the target is *not* in the table, 68.75% chance of score -1.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_MirrorMove_TryScoreMinus1
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    IfLoadedNotInTable Expert_MirrorMove_MoveTable, Expert_MirrorMove_TryScoreMinus1
    IfRandomLessThan 128, Expert_MirrorMove_End
    AddToMoveScore 2
    GoTo Expert_MirrorMove_End

Expert_MirrorMove_TryScoreMinus1:
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_MirrorMove_MoveTable, Expert_MirrorMove_End
    IfRandomLessThan 80, Expert_MirrorMove_End
    AddToMoveScore -1

Expert_MirrorMove_End:
    PopOrEnd 

Expert_MirrorMove_MoveTable:
    TableEntry MOVE_SLEEP_POWDER
    TableEntry MOVE_LOVELY_KISS
    TableEntry MOVE_SPORE
    TableEntry MOVE_HYPNOSIS
    TableEntry MOVE_SING
    TableEntry MOVE_GRASS_WHISTLE
    TableEntry MOVE_SHADOW_PUNCH
    TableEntry MOVE_SAND_ATTACK
    TableEntry MOVE_SMOKE_SCREEN
    TableEntry MOVE_TOXIC
    TableEntry MOVE_GUILLOTINE
    TableEntry MOVE_HORN_DRILL
    TableEntry MOVE_FISSURE
    TableEntry MOVE_SHEER_COLD
    TableEntry MOVE_CROSS_CHOP
    TableEntry MOVE_AEROBLAST
    TableEntry MOVE_CONFUSE_RAY
    TableEntry MOVE_SWEET_KISS
    TableEntry MOVE_SCREECH
    TableEntry MOVE_COTTON_SPORE
    TableEntry MOVE_SCARY_FACE
    TableEntry MOVE_FAKE_TEARS
    TableEntry MOVE_METAL_SOUND
    TableEntry MOVE_THUNDER_WAVE
    TableEntry MOVE_GLARE
    TableEntry MOVE_POISON_POWDER
    TableEntry MOVE_SHADOW_BALL
    TableEntry MOVE_DYNAMIC_PUNCH
    TableEntry MOVE_HYPER_BEAM
    TableEntry MOVE_EXTREME_SPEED
    TableEntry MOVE_THIEF
    TableEntry MOVE_COVET
    TableEntry MOVE_ATTRACT
    TableEntry MOVE_SWAGGER
    TableEntry MOVE_TORMENT
    TableEntry MOVE_FLATTER
    TableEntry MOVE_TRICK
    TableEntry MOVE_SUPERPOWER
    TableEntry MOVE_SKILL_SWAP
    TableEntry MOVE_PSYCHO_SHIFT
    TableEntry MOVE_POWER_SWAP
    TableEntry MOVE_GUARD_SWAP
    TableEntry MOVE_SUCKER_PUNCH
    TableEntry MOVE_HEART_SWAP
    TableEntry MOVE_SWITCHEROO
    TableEntry MOVE_CAPTIVATE
    TableEntry MOVE_DARK_VOID
    TableEntry MOVE_SHARPEN
    TableEntry TABLE_END

Expert_StatusAttackUp:
    ; If the defender deters boosting, 95% chance for score -12.
    ;
    ; If the attacker is statused, check if it is burned. If so, try to boost back
    ; to neutral at +2. If +2 or higher, 87.5% chance for score -5.
    ;
    ; If the attacker is poisoned or paralyzed, 75% chance for score -3.
    ;
    ; If the attacker is unboosted, score +1.
    ;
    ; If the attacker is boosted +3 or more, 75% chance for score -1.
    ; If the attacker is boosted +4 or more, 97% chance for additional score -1.
    ;
    ; If the attacker is at 100% HP, 66% chance of additional score +1.
    ;
    ; If defender can chunk or KO attacker, 99% chance for score -12.
    ;
    ; If the attacker''s HP is > 70%, no further score changes.
    ; If the attacker''s HP is < 40%, score -2.
    ; Otherwise, additional 87.5% chance for score -2.
    IfBattlerDetersBoosting AI_BATTLER_DEFENDER, Try95ChanceForScoreMinus12
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_StatusAttackUp_CheckBurn
    IfHasStatusThreat AI_BATTLER_DEFENDER, Try95ChanceForScoreMinus12
    AddToMoveScore 1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 7, Expert_StatusAttackUp_CheckUserAtMaxHP
    AddToMoveScore -1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 9, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfRandomLessThan 64, Expert_StatusAttackUp_CheckUserAtMaxHP
    AddToMoveScore -1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 10, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfRandomLessThan 16, Expert_StatusAttackUp_CheckUserAtMaxHP
    AddToMoveScore -1
    GoTo Expert_StatusAttackUp_CheckUserAtMaxHP

Expert_StatusAttackUp_CheckBurn:
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_BURN, Expert_StatusAttackUp_CheckParalysis
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_GUTS, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfLoadedEqualTo ABILITY_FLARE_BOOST, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PASS_STATS_AND_STATUS, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 8, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfRandomLessThan 32, Expert_StatusAttackUp_CheckUserAtMaxHP
    AddToMoveScore -5
    GoTo Expert_StatusAttackUp_CheckUserAtMaxHP

Expert_StatusAttackUp_CheckParalysis:
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_PARALYSIS, Expert_StatusAttackUp_CheckPoison
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_GUTS, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfLoadedEqualTo ABILITY_QUICK_FEET, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfRandomLessThan 64, Expert_StatusAttackUp_CheckUserAtMaxHP
    AddToMoveScore -3
    GoTo Expert_StatusAttackUp_CheckUserAtMaxHP

Expert_StatusAttackUp_CheckPoison:
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, Expert_StatusAttackUp_CheckSleep
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_GUTS, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfLoadedEqualTo ABILITY_POISON_HEAL, Expert_StatusAttackUp_CheckUserAtMaxHP
    IfRandomLessThan 64, Expert_StatusAttackUp_CheckUserAtMaxHP
    AddToMoveScore -3
    GoTo Expert_StatusAttackUp_CheckUserAtMaxHP

Expert_StatusAttackUp_CheckSleep:
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_SLEEP, Expert_StatusAttackUp_CheckUserAtMaxHP
    LoadSleepTurns AI_BATTLER_ATTACKER
    IfLoadedGreaterThan 1, ScoreMinus12
    GoTo Expert_StatusAttackUp_CheckUserAtMaxHP

Expert_StatusAttackUp_CheckUserAtMaxHP:
    ; Unfinished. Need to add a function to check for sash breaking
    IfHPPercentNotEqualTo AI_BATTLER_ATTACKER, 100, Expert_StatusAttackUp_CheckUserHPRange
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MULTISCALE, Expert_StatusAttackUp_CheckSashBreaking
    IfLoadedEqualTo ABILITY_STURDY, Expert_StatusAttackUp_CheckSashBreaking
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo HOLD_EFFECT_ENDURE, Expert_StatusAttackUp_CheckSashBreaking
    IfRandomLessThan 85, Expert_StatusAttackUp_CheckUserHPRange
    AddToMoveScore 1
    GoTo Expert_StatusAttackUp_CheckUserHPRange

Expert_StatusAttackUp_CheckSashBreaking:
    IfCanBreakSashOrSturdy AI_BATTLER_DEFENDER, Expert_StatusAttackUp_CheckUserHPRange
    GoTo Try95ChanceForScorePlus3

Expert_StatusAttackUp_CheckUserHPRange:
    IfEnemyCanChunkOrKO Try99ChanceForScoreMinus12
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_StatusAttackUp_End
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_StatusAttackUp_ScoreMinus2
    IfRandomLessThan 32, Expert_StatusAttackUp_End

Expert_StatusAttackUp_ScoreMinus2:
    AddToMoveScore -2

Expert_StatusAttackUp_End:
    PopOrEnd 

Expert_StatusDefenseUp:
    ; If the attacker is at +3 stat stage or higher, ~60.9% chance of additional score -1.
    ;
    ; If the attacker is at 100% HP, 50% chance of additional score +2.
    ;
    ; If the attacker''s HP is >= 70%, ~78.1% chance to suppress all further score modifiers.
    ;
    ; If the attacker''s HP is < 40%, additional score -2.
    ;
    ; Otherwise:
    ; - If the target''s last-used move was a Status move, ~76.6% chance of score -2.
    ; - If the target''s last-used move was a Special move, 58.6% chance of score -2.
    ; - Otherwise, score -2.
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 9, Expert_StatusDefenseUp_CheckUserAtMaxHP
    IfRandomLessThan 100, Expert_StatusDefenseUp_CheckUserHighHP
    AddToMoveScore -1
    GoTo Expert_StatusDefenseUp_CheckUserHighHP

Expert_StatusDefenseUp_CheckUserAtMaxHP:
    IfHPPercentNotEqualTo AI_BATTLER_ATTACKER, 100, Expert_StatusDefenseUp_CheckUserHighHP
    IfRandomLessThan 128, Expert_StatusDefenseUp_CheckUserHighHP
    AddToMoveScore 2

Expert_StatusDefenseUp_CheckUserHighHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 70, Expert_StatusDefenseUp_CheckUserMediumHP
    IfRandomLessThan 200, Expert_StatusDefenseUp_End

Expert_StatusDefenseUp_CheckUserMediumHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_StatusDefenseUp_ScoreMinus2
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadPowerOfLoadedMove 
    IfLoadedEqualTo 0, Expert_StatusDefenseUp_UserAtLowHP
    LoadDefenderLastUsedMoveClass 
    IfLoadedEqualTo CLASS_SPECIAL, Expert_StatusDefenseUp_ScoreMinus2
    IfRandomLessThan 60, Expert_StatusDefenseUp_End

Expert_StatusDefenseUp_UserAtLowHP:
    IfRandomLessThan 60, Expert_StatusDefenseUp_End

Expert_StatusDefenseUp_ScoreMinus2:
    AddToMoveScore -2

Expert_StatusDefenseUp_End:
    PopOrEnd 

Expert_StatusDefenseUp_PreSplitPhysicalTypes:
    TableEntry TYPE_NORMAL
    TableEntry TYPE_FIGHTING
    TableEntry TYPE_POISON
    TableEntry TYPE_GROUND
    TableEntry TYPE_FLYING
    TableEntry TYPE_ROCK
    TableEntry TYPE_BUG
    TableEntry TYPE_GHOST
    TableEntry TYPE_STEEL
    TableEntry TABLE_END

Expert_StatusSpeedUp:
    ; If the AI is faster than its target, score -3.
    ;
    ; If the AI is slower than its target, 72.7% chance of score +3.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_StatusSpeedUp_TryScorePlus3
    AddToMoveScore -3
    GoTo Expert_StatusSpeedUp_End

Expert_StatusSpeedUp_TryScorePlus3:
    IfRandomLessThan 70, Expert_StatusSpeedUp_End
    AddToMoveScore 3

Expert_StatusSpeedUp_End:
    PopOrEnd 

Expert_StatusSpAttackUp:
    ; If the attacker is at +3 stat stage or higher, ~60.9% chance of additional score -1.
    ;
    ; If the attacker is at 100% HP, 50% chance of additional score +2.
    ;
    ; If the attacker''s HP is > 70%, no further score changes.
    ;
    ; If the attacker''s HP is < 40%, additional score -2.
    ;
    ; Otherwise, ~84.4% chance of additional score -2.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_UNAWARE, ScoreMinus20
    LoadBattlerAbility AI_BATTLER_ATTACKER
	IfLoadedEqualTo ABILITY_AWARE, ScoreMinus20
    IfLoadedNotEqualTo ABILITY_SIMPLE, Expert_StatusSpAttackUp_CheckMoves
    AddToMoveScore 1
    GoTo Expert_StatusSpAttackUp_CheckMoves

Expert_StatusSpAttackUp_CheckMoves:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_RESET_STAT_CHANGES, ScoreMinus20
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_FORCE_SWITCH, Expert_StatusSpAttackUp_CheckSoundProof
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TAUNT, Expert_StatusSpAttackUp_TryScoreMinus10
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ENCORE, Expert_StatusSpAttackUp_TryScoreMinus10
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PASS_STATS_AND_STATUS, Expert_StatusSpAttackUp_TryScorePlus2
    GoTo Expert_StatusSpAttackUp_CheckSpeed

Expert_StatusSpAttackUp_TryScorePlus2:
    IfRandomLessThan 32, Expert_StatusSpAttackUp_CheckSpeed
    AddToMoveScore 2
    GoTo Expert_StatusSpAttackUp_CheckSpeed

Expert_StatusSpAttackUp_CheckSoundProof:
    IfRandomLessThan 16, Expert_StatusSpAttackUp_CheckSpeed
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SOUNDPROOF, Expert_StatusSpAttackUp_CheckSpeed
    IfRandomLessThan 80, Expert_StatusSpAttackUp_CheckSpeed
    GoTo ScoreMinus12

Expert_StatusSpAttackUp_TryScoreMinus10:
    IfRandomLessThan 64, Expert_StatusSpAttackUp_CheckSpeed
    GoTo ScoreMinus10

Expert_StatusSpAttackUp_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_StatusSpAttackUp_Faster
    IfRandomLessThan 50, Expert_StatusSpAttackUp_Slower
    AddToMoveScore -2
    GoTo Expert_StatusSpAttackUp_Slower

Expert_StatusSpAttackUp_Faster:
    IfRandomLessThan 50, Expert_StatusSpAttackUp_Slower
    AddToMoveScore 2
    GoTo Expert_StatusSpAttackUp_Slower

Expert_StatusSpAttackUp_Slower:
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 9, Expert_StatusSpAttackUp_CheckUserAtMaxHP
    IfRandomLessThan 100, Expert_StatusSpAttackUp_CheckUserHPRange
    AddToMoveScore -1
    GoTo Expert_StatusSpAttackUp_CheckUserHPRange

Expert_StatusSpAttackUp_CheckUserAtMaxHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 88, Expert_StatusSpAttackUp_CheckUserHPRange
    IfRandomLessThan 128, Expert_StatusSpAttackUp_CheckUserHPRange
    AddToMoveScore 2

Expert_StatusSpAttackUp_CheckUserHPRange:
    IfEnemyCanChunkOrKO Expert_StatusSpAttackUp_ScoreMinus10
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_StatusSpAttackUp_End
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_StatusSpAttackUp_ScoreMinus2
    IfRandomLessThan 70, Expert_StatusSpAttackUp_End

Expert_StatusSpAttackUp_ScoreMinus10:
    AddToMoveScore -10
    GoTo Expert_StatusSpAttackUp_End

Expert_StatusSpAttackUp_ScoreMinus2:
    AddToMoveScore -2

Expert_StatusSpAttackUp_End:
    PopOrEnd 

Expert_StatusSpDefenseUp:
    ; If the attacker is at +3 stat stage or higher, ~60.9% chance of additional score -1.
    ;
    ; If the attacker is at 100% HP, 50% chance of additional score +2.
    ;
    ; If the attacker''s HP is >= 70%, ~78.1% chance to suppress all further score modifiers.
    ;
    ; If the attacker''s HP is < 40%, additional score -2.
    ;
    ; Otherwise:
    ; - If the target''s last-used move was a Status move, ~76.6% chance of score -2.
    ; - If the target''s last-used move was a Physical move, 58.6% chance of score -2.
    ; - Otherwise, score -2.
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 9, Expert_StatusSpDefenseUp_CheckUserAtMaxHP
    IfRandomLessThan 100, Expert_StatusSpDefenseUp_CheckUserHighHP
    AddToMoveScore -1
    GoTo Expert_StatusSpDefenseUp_CheckUserHighHP

Expert_StatusSpDefenseUp_CheckUserAtMaxHP:
    IfHPPercentNotEqualTo AI_BATTLER_ATTACKER, 100, Expert_StatusSpDefenseUp_CheckUserHighHP
    IfRandomLessThan 128, Expert_StatusSpDefenseUp_CheckUserHighHP
    AddToMoveScore 2

Expert_StatusSpDefenseUp_CheckUserHighHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 70, Expert_StatusSpDefenseUp_CheckUserMediumHP
    IfRandomLessThan 200, Expert_StatusSpDefenseUp_End

Expert_StatusSpDefenseUp_CheckUserMediumHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_StatusSpDefenseUp_TryScoreMinus2
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadPowerOfLoadedMove 
    IfLoadedEqualTo 0, Expert_StatusSpDefenseUp_UserAtLowHP
    LoadDefenderLastUsedMoveClass 
    IfLoadedEqualTo CLASS_PHYSICAL, Expert_StatusSpDefenseUp_TryScoreMinus2
    IfRandomLessThan 60, Expert_StatusSpDefenseUp_End

Expert_StatusSpDefenseUp_UserAtLowHP:
    IfRandomLessThan 60, Expert_StatusSpDefenseUp_End

Expert_StatusSpDefenseUp_TryScoreMinus2:
    AddToMoveScore -2

Expert_StatusSpDefenseUp_End:
    PopOrEnd 

Expert_StatusSpDefenseUp_PreSplitPhysicalTypes:
    TableEntry TYPE_NORMAL
    TableEntry TYPE_FIGHTING
    TableEntry TYPE_POISON
    TableEntry TYPE_GROUND
    TableEntry TYPE_FLYING
    TableEntry TYPE_ROCK
    TableEntry TYPE_BUG
    TableEntry TYPE_GHOST
    TableEntry TYPE_STEEL
    TableEntry TABLE_END

Expert_StatusAccuracyUp:
    ; If the attacker is at +3 stat stage or higher, ~80.5% chance of additional score -2.
    ;
    ; If the attacker''s HP is at < 70%, score -2.
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 9, Expert_StatusAccuracyUp_TryScoreMinus2
    IfRandomLessThan 50, Expert_StatusAccuracyUp_TryScoreMinus2
    AddToMoveScore -2

Expert_StatusAccuracyUp_TryScoreMinus2:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_StatusAccuracyUp_End
    AddToMoveScore -2

Expert_StatusAccuracyUp_End:
    PopOrEnd 

Expert_StatusEvasionUp:
    ; If the attacker''s HP is >= 90%, ~60.9% chance of additional score +3.
    ;
    ; If the attacker is at +3 stat stage or higher, 50% chance of additional score -1.
    ;
    ; If the target is Badly Poisoned:
    ; - If the attacker''s HP is > 50%, ~80.5% chance of additional score +3.
    ; - If the attacker''s HP is <= 50%, ~55.3% chance of additional score +3.
    ;
    ; If the target is affected by Leech Seed, ~72.7% chance of additional score +3.
    ;
    ; If the attacker is affected by Ingrain or Aqua Ring, 50% chance of additional score +2.
    ;
    ; If the target is affected by Curse, ~72.7% chance of additional score +3.
    ;
    ; If the attacker''s HP is > 70%, suppress all further modifiers. Otherwise:
    ; - If the attacker is at exactly +0 stat stage, no further score modifiers.
    ; - If either the attacker''s HP or the target''s HP are < 40%, score -2.
    ; - Otherwise, ~72.7% chance of score -2.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 90, Expert_StatusEvasionUp_CheckUserStatStage
    IfRandomLessThan 100, Expert_StatusEvasionUp_CheckUserStatStage
    AddToMoveScore 3

Expert_StatusEvasionUp_CheckUserStatStage:
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 9, Expert_StatusEvasionUp_CheckEnemyBadlyPoisoned
    IfRandomLessThan 128, Expert_StatusEvasionUp_CheckEnemyBadlyPoisoned
    AddToMoveScore -1

Expert_StatusEvasionUp_CheckEnemyBadlyPoisoned:
    IfNotStatus AI_BATTLER_DEFENDER, MON_CONDITION_TOXIC, Expert_StatusEvasionUp_CheckEnemySeeded
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 50, Expert_StatusEvasionUp_TryScorePlus3
    IfRandomLessThan 80, Expert_StatusEvasionUp_CheckEnemySeeded

Expert_StatusEvasionUp_TryScorePlus3:
    IfRandomLessThan 50, Expert_StatusEvasionUp_CheckEnemySeeded
    AddToMoveScore 3

Expert_StatusEvasionUp_CheckEnemySeeded:
    IfNotMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LEECH_SEED, Expert_StatusEvasionUp_CheckUserIngrained
    IfRandomLessThan 70, Expert_StatusEvasionUp_CheckUserIngrained
    AddToMoveScore 3

Expert_StatusEvasionUp_CheckUserIngrained:
    IfNotMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_INGRAIN, Expert_StatusEvasionUp_CheckUserHasAquaRing
    IfRandomLessThan 128, Expert_StatusEvasionUp_CheckEnemyCursed
    AddToMoveScore 2
    GoTo Expert_StatusEvasionUp_CheckEnemyCursed

Expert_StatusEvasionUp_CheckUserHasAquaRing:
    IfNotMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_AQUA_RING, Expert_StatusEvasionUp_CheckEnemyCursed
    IfRandomLessThan 128, Expert_StatusEvasionUp_CheckEnemyCursed
    AddToMoveScore 2
    GoTo Expert_StatusEvasionUp_CheckEnemyCursed

Expert_StatusEvasionUp_CheckEnemyCursed:
    IfNotVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CURSE, Expert_StatusEvasionUp_CheckHPRanges
    IfRandomLessThan 70, Expert_StatusEvasionUp_CheckHPRanges
    AddToMoveScore 3

Expert_StatusEvasionUp_CheckHPRanges:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_StatusEvasionUp_End
    IfStatStageEqualTo AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 6, Expert_StatusEvasionUp_End
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_StatusEvasionUp_ScoreMinus2
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 40, Expert_StatusEvasionUp_ScoreMinus2
    IfRandomLessThan 70, Expert_StatusEvasionUp_End

Expert_StatusEvasionUp_ScoreMinus2:
    AddToMoveScore -2

Expert_StatusEvasionUp_End:
    PopOrEnd 

Expert_BypassAccuracyMove:
    ; If the target is at +5 Evasion or higher, or the attacker is at -5 Accuracy or lower, 60.9%
    ; chance of score +2, 39.1% chance of score +1.
    ;
    ; If the target is at +3 Evasion or higher, or the attacker is at -3 Accuracy or lower, score +1.
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 10, Expert_BypassAccuracyMove_ScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 2, Expert_BypassAccuracyMove_ScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 8, Expert_BypassAccuracyMove_TryScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 4, Expert_BypassAccuracyMove_TryScorePlus1
    GoTo Expert_BypassAccuracyMove_End

Expert_BypassAccuracyMove_ScorePlus1:
    AddToMoveScore 1

Expert_BypassAccuracyMove_TryScorePlus1:
    IfRandomLessThan 100, Expert_BypassAccuracyMove_End
    AddToMoveScore 1

Expert_BypassAccuracyMove_End:
    PopOrEnd 

Expert_StatusAttackDown:
	; If the defender has Clear Body or White Smoke, score -3
	; If the defender has Defiant or Competitive, score -10
	;
    ; If the target is at any stat stage other than +0, additional score -1. Also, further modify
    ; the score according to all of the following which apply:
    ; - If the attacker''s HP is at 90% or lower, additional score -1.
    ; - If the target is at -3 stat stage or lower, 80.5% chance of additional score -2.
    ;
    ; If the target''s HP is at 70% or lower, additional score -2.
    ;
    ; If the move last used by the target was not a Special move, 50% chance of score -2.
	LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus3
	IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus3
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
	
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 6, Expert_StatusAttackDown_CheckTargetHP
    AddToMoveScore -1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 90, Expert_StatusAttackDown_CheckTargetStatStage
    AddToMoveScore -1

Expert_StatusAttackDown_CheckTargetStatStage:
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 3, Expert_StatusAttackDown_CheckTargetHP
    IfRandomLessThan 50, Expert_StatusAttackDown_CheckTargetHP
    AddToMoveScore -2

Expert_StatusAttackDown_CheckTargetHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, Expert_StatusAttackDown_CheckLastUsedMove
    AddToMoveScore -2

Expert_StatusAttackDown_CheckLastUsedMove:
    LoadDefenderLastUsedMoveClass 
    IfLoadedNotEqualTo CLASS_SPECIAL, Expert_StatusAttackDown_End
    IfRandomLessThan 128, Expert_StatusAttackDown_End
    AddToMoveScore -2

Expert_StatusAttackDown_End:
    PopOrEnd 

Expert_StatusAttackDown_PreSplitPhysicalTypes:
    TableEntry TYPE_NORMAL
    TableEntry TYPE_FIGHTING
    TableEntry TYPE_GROUND
    TableEntry TYPE_ROCK
    TableEntry TYPE_BUG
    TableEntry TYPE_STEEL
    TableEntry TABLE_END

Expert_StatusDefenseDown:
	; If the defender has Clear Body or White Smoke, score -3
	; If the defender has Defiant or Competitive, score -10
	;
    ; If the attacker''s HP is < 70%, 80.5% chance of additional score -2.
    ;
    ; If the target''s stat stage is otherwise at -3 or lower, 80.5% chance of additional score -2.
    ;
    ; If the target''s HP is < 70%, score -2.
	LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus3
	IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus3
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
	
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 70, Expert_StatusDefenseDown_TryScoreMinus2
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 3, Expert_StatusDefenseDown_CheckTargetHP

Expert_StatusDefenseDown_TryScoreMinus2:
    IfRandomLessThan 50, Expert_StatusDefenseDown_CheckTargetHP
    AddToMoveScore -2

Expert_StatusDefenseDown_CheckTargetHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, Expert_StatusDefenseDown_End
    AddToMoveScore -2

Expert_StatusDefenseDown_End:
    PopOrEnd 

Expert_SpeedDownOnHit:
	; If the defender has Clear Body or White Smoke, score -3
	; If the defender has Defiant or Competitive, score -10
	;
    ; If the target is immune to or would quad-resist the move, 95%-100% chance for score -12.
    ;
    ; Treat non-highest-damage moves as Speed-reducing status moves.
    ;
    ; If trick room is up, 50% chance for score minus 1.
	LoadBattlerIgnorableAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus3
	IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus3
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus12
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Try95ChanceForScoreMinus12
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NOT_HIGHEST_DAMAGE, Expert_StatusSpeedDown_CheckSpeed
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, Try50ChanceForScoreMinus1
    IfRandomLessThan 140, Try95ChanceForScorePlus1
    PopOrEnd 

Expert_SpeedDownOnHit_End:
    PopOrEnd 

Expert_StatusSpeedDown:
	; If the defender has Clear Body or White Smoke, score -3
	; If the defender has Defiant or Competitive, score -10
	;
    ; If the attacker is slower than its target, 60% chance for score +1,
    ; 30% chance for score +2.
    ;
    ; If the attacker is faster than its target, score -3.
	LoadBattlerIgnorableAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus3
	IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus3
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
    GoTo Expert_StatusSpeedDown_CheckSpeed

Expert_StatusSpeedDown_CheckSpeed:
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, ScoreMinus12
    IfSpeedCompareNotEqualTo COMPARE_SPEED_FASTER, Expert_StatusSpeedDown_TryScorePlus2
    AddToMoveScore -3
    GoTo Expert_StatusSpeedDown_End

Expert_StatusSpeedDown_TryScorePlus2:
    IfRandomLessThan 103, Expert_StatusSpeedDown_End
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_StatusSpeedDown_End
    AddToMoveScore 1

Expert_StatusSpeedDown_End:
    PopOrEnd 

Expert_StatusSpAttackDown:
	; If the defender has Clear Body or White Smoke, score -3
	; If the defender has Defiant or Competitive, score -10
	;
    ; If the target is at any stat stage other than +0, additional score -1. Also, further modify
    ; the score according to all of the following which apply:
    ; - If the attacker''s HP is at 90% or lower, additional score -1.
    ; - If the target is at -3 stat stage or lower, 80.5% chance of additional score -2.
    ;
    ; If the target''s HP is at 70% or lower, additional score -2.
    ;
    ; If the move last used by the target was not a Physical move, 50% chance of score -2.
	LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus3
	IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus3
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
	
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 6, Expert_StatusSpAttackDown_CheckTargetHP
    AddToMoveScore -1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 90, Expert_StatusSpAttackDown_CheckTargetStatStage
    AddToMoveScore -1

Expert_StatusSpAttackDown_CheckTargetStatStage:
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 3, Expert_StatusSpAttackDown_CheckTargetHP
    IfRandomLessThan 50, Expert_StatusSpAttackDown_CheckTargetHP
    AddToMoveScore -2

Expert_StatusSpAttackDown_CheckTargetHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, Expert_StatusSpAttackDown_CheckLastUsedMove
    AddToMoveScore -2

Expert_StatusSpAttackDown_CheckLastUsedMove:
    LoadDefenderLastUsedMoveClass 
    IfLoadedNotEqualTo CLASS_PHYSICAL, Expert_StatusSpAttackDown_End
    IfRandomLessThan 128, Expert_StatusSpAttackDown_End
    AddToMoveScore -2

Expert_StatusSpAttackDown_End:
    PopOrEnd 

Expert_StatusSpAttackDown_PreSplitSpecialTypes:
    TableEntry TYPE_FIRE
    TableEntry TYPE_WATER
    TableEntry TYPE_GRASS
    TableEntry TYPE_ELECTRIC
    TableEntry TYPE_PSYCHIC
    TableEntry TYPE_ICE
    TableEntry TYPE_DRAGON
    TableEntry TYPE_DARK
    TableEntry TABLE_END

Expert_StatusSpDefenseDown:
	; If the defender has Clear Body or White Smoke, score -10
	; If the defender has Defiant, Competitive, or Magic Bounce score -12
	;
    ; If the attacker''s HP is <= 60%, 87.5% chance of additional score -2.
    ;
    ; If the target''s stat stage is otherwise at -3 or lower, 75% chance of additional score -3.
    ;
    ; If we have no special attacks and target is already at -2 or lower, score -12
	LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus10
	IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus10
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus12
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus12
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus12
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 6, Expert_StatusSpDefenseDown_DebuffNeeded
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 4, Expert_StatusSpDefenseDown_CheckTargetHP
    IfBattlerHasNoSpecialAttack AI_BATTLER_ATTACKER, ScoreMinus12
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 0, Expert_StatusSpDefenseDown_TryScoreMinus3
    GoTo ScoreMinus12

Expert_StatusSpDefenseDown_TryScoreMinus3:
    IfRandomLessThan 64, Expert_StatusSpDefenseDown_CheckTargetHP
    AddToMoveScore -3
    GoTo Expert_StatusSpDefenseDown_CheckTargetHP

Expert_StatusSpDefenseDown_DebuffNeeded:
    IfRandomLessThan 64, Expert_StatusSpDefenseDown_End
    AddToMoveScore 2
    GoTo Expert_StatusSpDefenseDown_End

Expert_StatusSpDefenseDown_CheckTargetHP:
    AddToMoveScore 1
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 60, Expert_StatusSpDefenseDown_End
    IfRandomLessThan 32, Expert_StatusSpDefenseDown_End
    AddToMoveScore -3
    GoTo Expert_StatusSpDefenseDown_End

Expert_StatusSpDefenseDown_End:
    PopOrEnd 

Expert_StatusAccuracyDown:
	; If the defender has Clear Body or White Smoke, score -3
	; If the defender has Defiant or Competitive, score -10
	;
    ; If the target''s HP is <= 70% and the attacker''s HP is NOT >= 70%, 60.9% chance of additional
    ; score -1.
    ;
    ; If the attacker is at -2 accuracy or lower, 68.75% chance of additional score -2.
    ;
    ; If the target is Badly Poisoned, ~72.7% chance of additional score +2.
    ;
    ; If the target is affected by Leech Seed, ~72.7% chance of additional score +2.
    ;
    ; If the attacker is affected by Ingrain or Aqua Ring, 50% chance of additional score +1.
    ;
    ; If the target is affected by Curse, ~72.7% chance of additional score +2.
    ;
    ; If the attacker''s HP is > 70%, suppress all further modifiers. Otherwise:
    ; - If the attacker is at exactly +0 stat stage, no further score modifiers.
    ; - If either the attacker''s HP or the target''s HP are < 40%, score -2.
    ; - Otherwise, ~72.7% chance of score -2.
	LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus3
	IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus3
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
	
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 70, Expert_StatusAccuracyDown_TryScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, Expert_StatusAccuracyDown_CheckUserAccuracy

Expert_StatusAccuracyDown_TryScoreMinus1:
    IfRandomLessThan 100, Expert_StatusAccuracyDown_CheckUserAccuracy
    AddToMoveScore -1

Expert_StatusAccuracyDown_CheckUserAccuracy:
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 4, Expert_StatusAccuracyDown_CheckTargetBadlyPoisoned
    IfRandomLessThan 80, Expert_StatusAccuracyDown_CheckTargetBadlyPoisoned
    AddToMoveScore -2

Expert_StatusAccuracyDown_CheckTargetBadlyPoisoned:
    IfNotStatus AI_BATTLER_DEFENDER, MON_CONDITION_TOXIC, Expert_StatusAccuracyDown_CheckTargetSeeded
    IfRandomLessThan 70, Expert_StatusAccuracyDown_CheckTargetSeeded
    AddToMoveScore 2

Expert_StatusAccuracyDown_CheckTargetSeeded:
    IfNotMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LEECH_SEED, Expert_StatusAccuracyDown_CheckUserIngrained
    IfRandomLessThan 70, Expert_StatusAccuracyDown_CheckUserIngrained
    AddToMoveScore 2

Expert_StatusAccuracyDown_CheckUserIngrained:
    IfNotMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_INGRAIN, Expert_StatusAccuracyDown_CheckUserHasAquaRing
    IfRandomLessThan 128, Expert_StatusAccuracyDown_CheckTargetCursed
    AddToMoveScore 1
    GoTo Expert_StatusAccuracyDown_CheckTargetCursed

Expert_StatusAccuracyDown_CheckUserHasAquaRing:
    IfNotMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_AQUA_RING, Expert_StatusAccuracyDown_CheckTargetCursed
    IfRandomLessThan 128, Expert_StatusAccuracyDown_CheckTargetCursed
    AddToMoveScore 1

Expert_StatusAccuracyDown_CheckTargetCursed:
    IfNotVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CURSE, Expert_StatusAccuracyDown_CheckHPRanges
    IfRandomLessThan 70, Expert_StatusAccuracyDown_CheckHPRanges
    AddToMoveScore 2

Expert_StatusAccuracyDown_CheckHPRanges:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_StatusAccuracyDown_End
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_ACCURACY, 6, Expert_StatusAccuracyDown_End
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_StatusAccuracyDown_ScoreMinus2
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 40, Expert_StatusAccuracyDown_ScoreMinus2
    IfRandomLessThan 70, Expert_StatusAccuracyDown_End

Expert_StatusAccuracyDown_ScoreMinus2:
    AddToMoveScore -2

Expert_StatusAccuracyDown_End:
    PopOrEnd 

Expert_StatusEvasionDown:
	; If the defender has Clear Body or White Smoke, score -3
	; If the defender has Defiant or Competitive, score -10
	;
    ; If the attacker''s HP is < 70%, 80.5% chance of additional score -2.
    ;
    ; Otherwise, if the target''s stat stage is -3 or lower, 80.5% chance of additional score -2.
    ;
    ; If the target''s HP is <= 70%, score -2.
	LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_CLEAR_BODY, ScoreMinus3
	IfLoadedEqualTo ABILITY_WHITE_SMOKE, ScoreMinus3
	IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus10
	IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus10
	
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 70, Expert_StatusEvasionDown_TryScoreMinus2
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 3, Expert_StatusEvasionDown_CheckTargetHP

Expert_StatusEvasionDown_TryScoreMinus2:
    IfRandomLessThan 50, Expert_StatusEvasionDown_CheckTargetHP
    AddToMoveScore -2

Expert_StatusEvasionDown_CheckTargetHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, Expert_StatusEvasionDown_End
    AddToMoveScore -2

Expert_StatusEvasionDown_End:
    PopOrEnd 

Expert_Haze:
    ; If any of the attacker''s stat stages are at +2 or higher, or any of the target''s stat stages
    ; are at -2 or lower, 80.4% chance of additional score -3.
    ;
    ; If any of the attacker''s stat stages are at -2 or lower, or any of the target''s stat stages
    ; are at +2 or higher, 95% chance of additional score +3.
    ;
    ; If any of the attacker''s stat stages are at -1 or lower, or any of the target''s stat stages
    ; are at +1 or higher, 50% chance of additional score +1.
    ;
    ; Otherwise, score -1.

    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_PASS_STATS_AND_STATUS, Expert_Haze_AntiBatonPass

    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 7, Expert_Haze_TryScoreMinus3
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 7, Expert_Haze_TryScoreMinus3
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 7, Expert_Haze_TryScoreMinus3
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 7, Expert_Haze_TryScoreMinus3
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 7, Expert_Haze_TryScoreMinus3
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 7, Expert_Haze_TryScoreMinus3
    
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 5, Expert_Haze_TryScoreMinus3
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 5, Expert_Haze_TryScoreMinus3
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 5, Expert_Haze_TryScoreMinus3
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 5, Expert_Haze_TryScoreMinus3
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 5, Expert_Haze_TryScoreMinus3
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_ACCURACY, 5, Expert_Haze_TryScoreMinus3
    GoTo Expert_Haze_CheckToEncourage

Expert_Haze_CheckToEncourage:
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 7, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 7, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 7, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 7, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 7, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 7, Expert_Haze_TryScorePlus3

    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 5, Expert_Haze_TryScorePlus3
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 5, Expert_Haze_TryScorePlus3
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 5, Expert_Haze_TryScorePlus3
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 5, Expert_Haze_TryScorePlus3
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 5, Expert_Haze_TryScorePlus3
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 5, Expert_Haze_TryScorePlus3
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 5, Expert_Haze_TryScorePlus3

    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 6, Expert_Haze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 6, Expert_Haze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 6, Expert_Haze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 6, Expert_Haze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 6, Expert_Haze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 6, Expert_Haze_TryScorePlus1

    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 6, Expert_Haze_TryScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 6, Expert_Haze_TryScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 6, Expert_Haze_TryScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 6, Expert_Haze_TryScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 6, Expert_Haze_TryScorePlus1

    IfRandomLessThan 50, Expert_Haze_End
    AddToMoveScore -1
    GoTo Expert_Haze_End

Expert_Haze_AntiBatonPass:
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadEffectOfLoadedMove
    IfLoadedEqualTo BATTLE_EFFECT_SET_SUBSTITUTE, Expert_Haze_TryScoreMinus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 6, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 6, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 6, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 6, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 6, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 6, Expert_Haze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ACCURACY, 6, Expert_Haze_TryScorePlus3
    IfRandomLessThan 50, Expert_Haze_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_Haze_End

Expert_Haze_TryScoreMinus3:
    IfRandomLessThan 50, Expert_Haze_CheckToEncourage
    AddToMoveScore -3
    GoTo Expert_Haze_CheckToEncourage

Expert_Haze_TryScorePlus3:
    IfRandomLessThan 12, Expert_Haze_End
    AddToMoveScore 3

Expert_Haze_TryScorePlus1:
    IfRandomLessThan 128, Expert_Haze_End
    AddToMoveScore 1


Expert_Haze_End:
    PopOrEnd 

Expert_Bide:
    ; If the attacker''s HP is <= 90%, score -2.
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 90, Expert_Bide_End
    AddToMoveScore -2

Expert_Bide_End:
    PopOrEnd 

Expert_Phaze:
    ; Don''t use phaze if enemy is on their last mon.
    ; 
    ; If target is immune to phaze via Ingrain or Suction Cups, there is a 1/3 chance to peek
    ; its item. If the item is Shed Shell, then +2 and proceed as usual. Otherwise, -3.
    ;
    ; If the target''s side of the field has Spikes, Stealth Rock, or Toxic Spikes set, 50% chance of
    ; score +1.
    ;
    ; If the target has a stat stage of +2 or higher in any of the following stats, 50% chance of
    ; score +2:
    ; - Attack
    ; - Defense
    ; - SpAttack
    ; - SpDefense
    ; - Evasion
    ;
    ; Otherwise, score -3.
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, ScoreMinus10
    IfMoveEffect AI_BATTLER_DEFENDER, BATTLE_EFFECT_GROUND_TRAP_USER_CONTINUOUS_HEAL, Expert_Phaze_TryCheckItem
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SUCTION_CUPS, Expert_Phaze_TryCheckItem
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_PASS_STATS_AND_STATUS, Expert_Phaze_AntiBatonPass
    GoTo Expert_Phaze_CheckToDiscourage

Expert_Phaze_CheckToDiscourage:
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_PERISH_SONG, ScoreMinus2
    IfHasSuperEffectiveMove Expert_Phaze_TryScoreMinus1
    GoTo Expert_Phaze_CheckToEncourage
    
Expert_Phaze_CheckToEncourage:
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 7, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 7, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 7, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 7, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 7, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 7, Expert_Phaze_TryScorePlus3

    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 6, Expert_Phaze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 6, Expert_Phaze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 6, Expert_Phaze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 6, Expert_Phaze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 6, Expert_Phaze_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 6, Expert_Phaze_TryScorePlus1

    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STEALTH_ROCK, Expert_Phaze_TryScorePlus1
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SPIKES, Expert_Phaze_TryScorePlus1
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_TOXIC_SPIKES, Expert_Phaze_TryScorePlus1

    IfRandomLessThan 50, Expert_Phaze_End
    AddToMoveScore -1
    GoTo Expert_Phaze_End

Expert_Phaze_AntiBatonPass:
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadEffectOfLoadedMove
    IfLoadedEqualTo BATTLE_EFFECT_SET_SUBSTITUTE, ScorePlus5
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_SUBSTITUTE, ScorePlus3
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_AQUA_RING, ScorePlus3
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_FOCUS_ENERGY, ScorePlus3
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_ABILITY_SUPPRESSED, ScorePlus2
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_MAGNET_RISE, ScorePlus2
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 6, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 6, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 6, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SPEED, 6, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 6, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 6, Expert_Phaze_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ACCURACY, 6, Expert_Phaze_TryScorePlus3
    IfRandomLessThan 50, Expert_Phaze_CheckToDiscourage
    AddToMoveScore -1
    GoTo Expert_Phaze_End

Expert_Phaze_TryScorePlus3:
    IfRandomLessThan 12, Expert_Phaze_End
    AddToMoveScore 3
    GoTo Expert_Phaze_End

Expert_Phaze_TryScorePlus1:
    IfRandomLessThan 128, Expert_Phaze_End
    AddToMoveScore 1
    GoTo Expert_Phaze_End

Expert_Phaze_TryScoreMinus1:
    IfRandomLessThan 128, Expert_Phaze_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_Phaze_End

Expert_Phaze_TryCheckItem:
    IfRandomLessThan 85, Expert_Phaze_CheckItem
    AddToMoveScore -3
    GoTo Expert_Phaze_End

Expert_Phaze_CheckItem:
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ITEM_SHED_SHELL, ScoreMinus3
    AddToMoveScore 2
    GoTo Expert_Phaze_CheckToEncourage

Expert_Phaze_End:
    PopOrEnd 

Expert_Conversion:
    ; If the attacker''s HP is <= 90%, additional score -2.
    ;
    ; If it is NOT the first global turn of the battle, ~78.1% chance of score -2.
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 90, Expert_Conversion_CheckTurnCount
    AddToMoveScore -2

Expert_Conversion_CheckTurnCount:
    LoadTurnCount 
    IfLoadedEqualTo 0, Expert_Conversion_End
    IfRandomLessThan 200, ScoreMinus2

Expert_Conversion_End:
    PopOrEnd 

Expert_Synthesis:
    ; Treat Synthesis-type effects like any other recovery move, but additional score -2 if the
    ; weather is Hail, Rain, or Sand.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_HAILING, Expert_Synthesis_ScoreMinus2
    IfLoadedEqualTo AI_WEATHER_RAINING, Expert_Synthesis_ScoreMinus2
    IfLoadedEqualTo AI_WEATHER_SANDSTORM, Expert_Synthesis_ScoreMinus2
    GoTo Expert_Recovery

Expert_Synthesis_ScoreMinus2:
    AddToMoveScore -2

Expert_Recovery:
    ; If the attacker is at full HP, score -3 and terminate.
    ;
    ; If the attacker is faster than its opponent, score -8 and terminate.
    ;
    ; If the attacker is at 70% HP or more, ~88.3% chance of score -3 and terminate.
    ;
    ; If the opponent does not know Snatch, ~92.2% chance of score +2.
    ; If the opponent does know Snatch, ~56.2% chance of score +2.
    IfHPPercentEqualTo AI_BATTLER_ATTACKER, 100, Expert_Recovery_ScoreMinus3AndEnd
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Recovery_CheckHP
    AddToMoveScore -8
    GoTo Expert_Recovery_End

Expert_Recovery_Unused:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_Recovery_CheckForSnatch
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 80, Expert_Recovery_ScoreMinus3AndEnd
    IfRandomLessThan 70, Expert_Recovery_CheckForSnatch

Expert_Recovery_ScoreMinus3AndEnd:
    AddToMoveScore -3
    GoTo Expert_Recovery_End

Expert_Recovery_CheckHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 70, Expert_Recovery_CheckForSnatch
    IfRandomLessThan 30, Expert_Recovery_CheckForSnatch
    AddToMoveScore -3
    GoTo Expert_Recovery_End

Expert_Recovery_CheckForSnatch:
    IfMoveEffectNotKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STEAL_STATUS_MOVE, Expert_Recovery_TryScorePlus2
    IfRandomLessThan 100, Expert_Recovery_End

Expert_Recovery_TryScorePlus2:
    IfRandomLessThan 20, Expert_Recovery_End
    AddToMoveScore 2

Expert_Recovery_End:
    PopOrEnd 

Expert_Toxic:
    ; If the attacker has at least one damaging move, apply all of the following which apply:
    ; - If the attacker''s HP <= 50%, 80.5% chance of additional score -3.
    ; - If the defender''s HP <= 50%, 80.5% chance of additional score -3.
    IfAttackerHasNoDamagingMoves Expert_Toxic_CheckStatStages
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 50, Expert_Toxic_CheckTargetHP
    IfRandomLessThan 50, Expert_Toxic_CheckTargetHP
    AddToMoveScore -3

Expert_Toxic_CheckTargetHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 50, Expert_Toxic_CheckStatStages
    IfRandomLessThan 50, Expert_Toxic_CheckStatStages
    AddToMoveScore -3

Expert_Toxic_CheckStatStages:
	IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 7, Expert_Toxic_TryScorePlus2
	IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 7, Expert_Toxic_TryScorePlus2
    GoTo Expert_Toxic_CheckDamage

Expert_Toxic_TryScorePlus2:
    IfRandomLessThan 60, Expert_Toxic_CheckDamage
    AddToMoveScore 2
	
Expert_Toxic_CheckDamage:
	IfCanChunkOrKOEnemy Expert_Toxic_TryScoreMinus3
	GoTo Expert_Toxic_End
	
Expert_Toxic_TryScoreMinus3:
	IfRandomLessThan 60, Expert_Toxic_End
    AddToMoveScore -3

Expert_Toxic_End:
    PopOrEnd
	
Expert_LeechSeed:
    ; If the attacker has at least one damaging move, apply all of the following which apply:
    ; - If the attacker''s HP <= 50%, 80.5% chance of additional score -3.
    ; - If the defender''s HP <= 50%, 80.5% chance of additional score -3.
    IfAttackerHasNoDamagingMoves Expert_LeechSeed_CheckMoveEffectsKnown
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 50, Expert_LeechSeed_CheckTargetHP
    IfRandomLessThan 50, Expert_LeechSeed_CheckTargetHP
    AddToMoveScore -3

Expert_LeechSeed_CheckTargetHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 32, Expert_LeechSeed_CheckMoveEffectsKnown
    IfRandomLessThan 50, Expert_LeechSeed_CheckMoveEffectsKnown
    AddToMoveScore -3

Expert_LeechSeed_CheckMoveEffectsKnown:
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LEECH_SEED, ScoreMinus20
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING, Expert_LeechSeed_ScoreMinus2
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PROTECT, Expert_LeechSeed_SynergizingMove
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_SET_SUBSTITUTE, Expert_LeechSeed_SynergizingMove
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, Expert_LeechSeed_CheckPoison
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_BURN, Expert_LeechSeed_CheckChip
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_CURSE, Expert_LeechSeed_CheckChip
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_CHIP, Expert_LeechSeed_CheckChip
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_BIND, Expert_LeechSeed_CheckChip
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED, Expert_LeechSeed_CheckChip
    GoTo Expert_LeechSeed_End

Expert_LeechSeed_CheckPoison:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_HEAL, Expert_LeechSeed_End
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_LeechSeed_End
    IfRandomLessThan 64, Expert_LeechSeed_End
    AddToMoveScore 1
    IfRandomLessThan 64, Expert_LeechSeed_End
    AddToMoveScore 1
    GoTo Expert_LeechSeed_End

Expert_LeechSeed_CheckChip:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_LeechSeed_End
    IfRandomLessThan 64, Expert_LeechSeed_End
    AddToMoveScore 1
    IfRandomLessThan 64, Expert_LeechSeed_End
    AddToMoveScore 1
    GoTo Expert_LeechSeed_End

Expert_LeechSeed_SynergizingMove:
    IfRandomLessThan 12, Expert_LeechSeed_End
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_LeechSeed_End
    AddToMoveScore 1
    IfRandomLessThan 230, Expert_LeechSeed_End
    AddToMoveScore 1
    GoTo Expert_LeechSeed_End

Expert_LeechSeed_ScoreMinus2:
    AddToMoveScore -2

Expert_LeechSeed_End:
    PopOrEnd 

Expert_LightScreen:
    ; If the attacker''s HP is < 50%, score -2.
    ;
    ; If the attacker''s HP is >= 90%, 50% of additional score +1.
    ;
    ; If the opponent''s last-used move was a Special move, 75% chance of score +1.
    LoadBattlerCritStage AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, ScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN, Expert_LightScreen_CheckDetersDefog
    IfBattlerIsSpecialAttacker AI_BATTLER_DEFENDER, Expert_LightScreen_TryScorePlus2
    GoTo Expert_LightScreen_Main

Expert_LightScreen_TryScorePlus2:
    IfRandomLessThan 24, Expert_LightScreen_Main
    AddToMoveScore 1
    IfRandomLessThan 24, Expert_LightScreen_Main
    AddToMoveScore 1
    GoTo Expert_LightScreen_Main

Expert_LightScreen_CheckDetersDefog:
    AddToMoveScore -1
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Expert_LightScreen_Main
    AddToMoveScore 1
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_DEFIANT, ScorePlus3
    IfLoadedEqualTo ABILITY_COMPETITIVE, ScorePlus3
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScorePlus3
    GoTo Expert_LightScreen_Main
    
Expert_LightScreen_Main:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, ScorePlus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 88, ScorePlus1
    GoTo Expert_LightScreen_End

Expert_LightScreen_End:
    PopOrEnd

Expert_Rest:
    ; If the attacker is faster than its target:
    ; - If the attacker is at full HP, 60.9% chance of score -8 and terminate.
    ; - If the attacker''s HP is > 50%, score -3 and terminate.
    ; - If the attacker''s HP is >= 40%, 72.7% chance of score -3 and terminate.
    ;
    ; If the attacker is slower than its target:
    ; - If the attacker''s HP > 70%, score -3 and terminate.
    ; - If the attacker''s HP >= 60%, 80.5% chance of score -3 and terminate.
    ;
    ; If the opponent does not know Snatch, 96.1% chance of score +3.
    ;
    ; If the opponent knows Snatch, 77.3% chance of score +3.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_MIRACLE_EYE, ScoreMinus12
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_HEAL_BLOCK, ScoreMinus12
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STATUS_NIGHTMARE, ScoreMinus12
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_BAD_DREAMS, ScoreMinus12
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, ScoreMinus10
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_EARLY_BIRD, Expert_Rest_EarlyBirdSleepTalk
    LoadRecycleItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_CHESTO_BERRY, ScoreMinus12
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_CHESTO_BERRY, Expert_Rest_Chesto
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_SLEEP_TALK, Expert_Rest_EarlyBirdSleepTalk
    GoTo Expert_Rest_CheckSpeed

Expert_Rest_EarlyBirdSleepTalk:
    IfRandomLessThan 128, Expert_Rest_CheckSpeed
    AddToMoveScore 1
    GoTo Expert_Rest_CheckSpeed

Expert_Rest_Chesto:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 60, ScoreMinus10
    IfRandomLessThan 8, Expert_Rest_CheckSpeed
    AddToMoveScore 2
    GoTo Expert_Rest_CheckSpeed

Expert_Rest_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Rest_SlowerCheckHP
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_Rest_TryScorePlus3
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_Rest_TryScorePlus1
    GoTo Expert_Rest_CheckForSnatch

Expert_Rest_FasterScoreMinus3:
    AddToMoveScore -3
    GoTo Expert_Rest_End

Expert_Rest_SlowerCheckHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_Rest_TryScorePlus3
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 60, Expert_Rest_TryScorePlus1
    GoTo Expert_Rest_CheckForSnatch

Expert_Rest_TryScorePlus1:
    IfRandomLessThan 32, Expert_Rest_CheckForSnatch
    AddToMoveScore 1
    GoTo Expert_Rest_CheckForSnatch

Expert_Rest_TryScorePlus3:
    LoadSleepTurns AI_BATTLER_ATTACKER
    IfLoadedEqualTo 1, Try95ChanceForScorePlus5
    IfRandomLessThan 12, Expert_Rest_CheckForSnatch
    AddToMoveScore 3
    GoTo Expert_Rest_CheckForSnatch

Expert_Rest_CheckForSnatch:
    IfRandomLessThan 32, Expert_Rest_End
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STEAL_STATUS_MOVE, ScoreMinus3
    GoTo Expert_Rest_End

Expert_Rest_End:
    PopOrEnd 

Expert_OHKOMove:
    ; 25% chance of score +1.
    IfRandomLessThan 192, Expert_OHKOMove_End
    AddToMoveScore 1

Expert_OHKOMove_End:
    PopOrEnd 

Expert_SuperFang:
    ; If the target is at 25% HP or less, score -1.
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 25, Expert_SuperFang_End
    AddToMoveScore -1

Expert_SuperFang_End:
    PopOrEnd 

Expert_BindingMove:
    ; If the target is under any of the following conditions or effects, 50% chance of score +1:
    ; - Toxic
    ; - Curse
    ; - Perish Song
    ; - Attract
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_TOXIC, Expert_BindingMove_TryScorePlus1
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CURSE, Expert_BindingMove_TryScorePlus1
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_PERISH_SONG, Expert_BindingMove_TryScorePlus1
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, Expert_BindingMove_TryScorePlus1
    GoTo Expert_BindingMove_End

Expert_BindingMove_TryScorePlus1:
    IfRandomLessThan 128, Expert_BindingMove_End
    AddToMoveScore 1

Expert_BindingMove_End:
    PopOrEnd 

Expert_HighCritical:
    ; If the move is super-effective against the target, 50% chance of score +1.
    ;
    ; If the move would deal normal damage against the target, 25% chance of score +1.
    LoadBattlerCritStage AI_BATTLER_ATTACKER
    IfLoadedGreaterThan 2, Expert_HighCritical_End
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_HighCritical_End
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_HighCritical_End
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_HighCritical_End
    LoadMoveClass
    IfLoadedEqualTo CLASS_PHYSICAL, Expert_HighCritical_CheckDefense
    ; Special move here
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_LIGHT_SCREEN, Expert_HighCritical_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 7, Expert_HighCritical_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 6, Expert_HighCritical_TryScorePlus1
    IfRandomLessThan 128, Expert_HighCritical_End

Expert_HighCritical_CheckDefense:
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_REFLECT, Expert_HighCritical_TryScorePlus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_UNAWARE, Expert_HighCritical_End
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 7, Expert_HighCritical_TryScorePlus3
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 6, Expert_HighCritical_TryScorePlus1
    GoTo Expert_HighCritical_End

Expert_HighCritical_TryScorePlus3:
    IfRandomLessThan 16, Expert_HighCritical_End
    AddToMoveScore 1
    IfRandomLessThan 16, Expert_HighCritical_End
    AddToMoveScore 1
    IfRandomLessThan 16, Expert_HighCritical_End
    AddToMoveScore 1
    GoTo Expert_HighCritical_End

Expert_HighCritical_TryScorePlus1:
    IfRandomLessThan 128, Expert_HighCritical_End
    AddToMoveScore 1

Expert_HighCritical_End:
    PopOrEnd 

Expert_Swagger:
    ; If the attacker knows Psych Up:
    ; - If the target''s attack stat is -3 or higher, score -5 and terminate.
    ; - If it is the first turn of the battle, score +5.
    ; - Otherwise, score +3.
    ;
    ; Otherwise, act identically to Flatter.
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_PSYCH_UP, Expert_Swagger_PsychUp

Expert_Flatter:
    ; 50% chance of additional score +1.
    ;
    ; Otherwise, act identically to any other Confusion-inducing Status move.
    IfRandomLessThan 128, Expert_StatusConfuse
    AddToMoveScore 1

Expert_StatusConfuse:
    ; If the target''s HP is <= 70%, 50% chance of additional score -1.
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, Expert_StatusConfuse_End
    IfRandomLessThan 128, Expert_StatusConfuse_CheckHP
    AddToMoveScore -1

Expert_StatusConfuse_CheckHP:
    ; If the target''s HP is <= 50%, additional score -1.
    ;
    ; If the target''s HP is also <= 30%, additional score -1.
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 50, Expert_StatusConfuse_End
    AddToMoveScore -1
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 30, Expert_StatusConfuse_End
    AddToMoveScore -1

Expert_StatusConfuse_End:
    PopOrEnd 

Expert_Swagger_PsychUp:
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 3, Expert_Swagger_ScoreMinus5
    AddToMoveScore 3
    LoadTurnCount 
    IfLoadedNotEqualTo 0, Expert_Swagger_End
    AddToMoveScore 2
    GoTo Expert_Swagger_End

Expert_Swagger_ScoreMinus5:
    AddToMoveScore -5

Expert_Swagger_End:
    PopOrEnd 

Expert_Reflect:
    ; If the attacker''s HP is < 50%, score -2.
    ;
    ; If the attacker''s HP is >= 90%, 50% of additional score +1.
    ;
    ; If the opponent''s last-used move was a Physical move, 75% chance of score +1.
    LoadBattlerCritStage AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, ScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN, Expert_Reflect_CheckDetersDefog
    IfBattlerIsPhysicalAttacker AI_BATTLER_DEFENDER, Expert_Reflect_TryScorePlus2
    GoTo Expert_Reflect_Main

Expert_Reflect_TryScorePlus2:
    IfRandomLessThan 24, Expert_Reflect_Main
    AddToMoveScore 1
    IfRandomLessThan 24, Expert_Reflect_Main
    AddToMoveScore 1
    GoTo Expert_Reflect_Main

Expert_Reflect_CheckDetersDefog:
    AddToMoveScore -1
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Expert_Reflect_Main
    AddToMoveScore 1
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_DEFIANT, ScorePlus3
    IfLoadedEqualTo ABILITY_COMPETITIVE, ScorePlus3
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScorePlus3
    GoTo Expert_Reflect_Main
    
Expert_Reflect_Main:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_REMOVE_SCREENS, Expert_Reflect_CheckGhost
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, ScorePlus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 88, ScorePlus1
    GoTo Expert_Reflect_End

Expert_Reflect_CheckGhost:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, Expert_Reflect_CheckGhostItemAndAbility
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, Expert_Reflect_CheckGhostItemAndAbility
    IfRandomLessThan 12, Expert_Reflect_End
    AddToMoveScore 2
    GoTo Expert_Reflect_End

Expert_Reflect_CheckGhostItemAndAbility:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NORMAL_HIT_GHOST, ScoreMinus12
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SCRAPPY, ScoreMinus12
    GoTo Expert_Reflect_End

Expert_Reflect_End:
    PopOrEnd

Expert_StatusPoison:
    ; If the attacker''s HP is < 50% or the defender''s HP is <= 50%, score -1.
    IfMoveEqualTo MOVE_POISON_POWDER, Expert_StatusPoison_PowderMove
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_StatusPoison_ScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 50, Expert_StatusPoison_End

Expert_StatusPoison_PowderMove:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, ScoreMinus12
    GoTo Expert_StatusPoison_End

Expert_StatusPoison_ScoreMinus1:
    AddToMoveScore -1

Expert_StatusPoison_End:
    PopOrEnd 

Expert_StatusParalyze:
    ; If the attacker is slower than its target, 92.2% chance of score +3.
    ;
    ; If the attacker''s HP is <= 70%, score -1.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_StatusParalyze_TryScorePlus3
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_StatusParalyze_End
    IfMoveEqualTo MOVE_STUN_SPORE, Expert_StatusParalyze_PowderMove
    AddToMoveScore -1
    GoTo Expert_StatusParalyze_End

Expert_StatusParalyze_PowderMove:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, ScoreMinus12
    GoTo Expert_StatusParalyze_End

Expert_StatusParalyze_TryScorePlus3:
    IfRandomLessThan 20, Expert_StatusParalyze_End
    AddToMoveScore 3

Expert_StatusParalyze_End:
    PopOrEnd 

Expert_VitalThrow:
    ; If the attacker is slower than its target, no change.
    ;
    ; If the attacker''s HP > 60%, no change.
    ;
    ; If the attacker''s HP < 40%, 80.5% chance of score -1.
    ;
    ; Otherwise, 23.9% chance of score -1.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_VitalThrow_End
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 60, Expert_VitalThrow_End
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_VitalThrow_TryScoreMinus1
    IfRandomLessThan 180, Expert_VitalThrow_End

Expert_VitalThrow_TryScoreMinus1:
    IfRandomLessThan 50, Expert_VitalThrow_End
    AddToMoveScore -1

Expert_VitalThrow_End:
    PopOrEnd 

Expert_Substitute:
    ; If the attacker knows specifically Focus Punch, 62.5% chance of additional score +1.
    ;
    ; If the attacker''s HP <= 90%, roll for a 60.9% chance of additional score -1 a number of times
    ; corresponding to the attacker''s HP:
    ; - > 70%: roll once
    ; - > 50%: roll twice
    ; - <= 50%: roll thrice
    ; These rolls are cumulative; e.g., an attacker at 53% HP can receive additional score -2.
    ;
    ; If the attacker is faster than its opponent, consider the move that the opponent last used:
    ; - If it was a Status move that induces a non-volatile status condition and the opponent is
    ; currently Asleep, Poisoned, Paralyzed, Burned, or Frozen, 60.9% chance of score +1.
    ; - If it was a Status move that induces Confusion and the opponent is currently Confused, 60.9%
    ; chance of score +1.
    ; - If it was Leech Seed and the opponent is currently Seeded, 60.9% chance of score +1.
    ;
    ; Otherwise, no further score modifications.
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PASS_STATS_AND_STATUS, Expert_Substitute_BatonPass
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_STATUS_LEECH_SEED, Expert_Substitute_LeechSeed
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_STATUS_BADLY_POISON, Expert_Substitute_Toxic
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT, Expert_Substitute_FocusPunch
    IfSpeedCompareNotEqualTo COMPARE_SPEED_FASTER, Expert_Substitute_Slower
    IfRandomLessThan 64, Expert_Substitute_Main
    AddToMoveScore 1
    GoTo Expert_Substitute_Main

Expert_Substitute_BatonPass:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_Substitute_BatonPass_TryScorePlus2
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_Substitute_HighPriorityPinchBerries, Expert_Substitute_BatonPass_TryScorePlus2
    IfLoadedInTable Expert_Substitute_LowPriorityPinchBerries, Expert_Substitute_BatonPass_TryScorePlus1
    GoTo Expert_Substitute_Main

Expert_Substitute_BatonPass_TryScorePlus2:
    IfRandomLessThan 32, Expert_Substitute_Main
    AddToMoveScore 1
    IfRandomLessThan 32, Expert_Substitute_Main
    AddToMoveScore 1
    GoTo Expert_Substitute_Main
    
Expert_Substitute_BatonPass_TryScorePlus1:
    IfRandomLessThan 128, Expert_Substitute_Main
    AddToMoveScore 1
    GoTo Expert_Substitute_Main

Expert_Substitute_LeechSeed:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LIQUID_OOZE, Expert_Substitute_Main
    IfNotMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED_RECIPIENT, Expert_Substitute_LeechSeed_TryScoreMinus3
    AddToMoveScore 1
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PROTECT, Expert_Substitute_LoadProtectChain
    IfRandomLessThan 40, Expert_Substitute_Main
    AddToMoveScore 1
    GoTo Expert_Substitute_Main

Expert_Substitute_LoadProtectChain:
    LoadProtectChain AI_BATTLER_ATTACKER
    IfLoadedGreaterThan 0, Expert_Substitute_LeechSeed_TryScorePlus3
    IfRandomLessThan 64, Expert_Substitute_Main
    AddToMoveScore -1
    GoTo Expert_Substitute_Main

Expert_Substitute_LeechSeed_TryScorePlus3:
    IfRandomLessThan 28, Expert_Substitute_Main
    AddToMoveScore 3
    GoTo Expert_Substitute_Main

Expert_Substitute_LeechSeed_TryScoreMinus3:
    IfRandomLessThan 28, Expert_Substitute_Main
    AddToMoveScore -3
    GoTo Expert_Substitute_Main

Expert_Substitute_Toxic:
    IfBattlerDetersStatus AI_BATTLER_DEFENDER, MON_CONDITION_TOXIC, Expert_Substitute_Main
    IfNotStatus AI_BATTLER_DEFENDER, MON_CONDITION_TOXIC, Expert_Substitute_Toxic_TryScoreMinus1
    IfRandomLessThan 128, Expert_Substitute_Main
    AddToMoveScore 1
    GoTo Expert_Substitute_Main
    
Expert_Substitute_Toxic_TryScoreMinus1:
    IfRandomLessThan 64, Expert_Substitute_Main
    AddToMoveScore -1
    GoTo Expert_Substitute_Main

Expert_Substitute_FocusPunch:
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, Expert_Substitute_FocusPunch_TryScoreMinus2
    IfRandomLessThan 85, Expert_Substitute_Main
    AddToMoveScore 1
    IfRandomLessThan 85, Expert_Substitute_Main
    AddToMoveScore 1
    GoTo Expert_Substitute_Main

Expert_Substitute_FocusPunch_TryScoreMinus2:
    IfRandomLessThan 85, Expert_Substitute_Main
    AddToMoveScore -1
    IfRandomLessThan 85, Expert_Substitute_Main
    AddToMoveScore -1
    GoTo Expert_Substitute_Main

Expert_Substitute_Slower:
    IfEnemyCanChunkOrKO Expert_Substitute_Slower_TryScoreMinus3
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PROTECT, Expert_Substitute_LoadProtectChain
    IfRandomLessThan 192, Expert_Substitute_Main
    AddToMoveScore 1
    GoTo Expert_Substitute_Main

Expert_Substitute_Slower_TryScoreMinus3:
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, Expert_Substitute_Main
    IfRandomLessThan 6, Expert_Substitute_Main
    AddToMoveScore -1
    IfRandomLessThan 32, Expert_Substitute_Main
    AddToMoveScore -1
    IfRandomLessThan 85, Expert_Substitute_Main
    AddToMoveScore -1
    GoTo Expert_Substitute_Main

Expert_Substitute_Main:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_FORCE_SWITCH, Expert_Substitute_PhazeCheck
    IfHasSubstituteIncentive AI_BATTLER_ATTACKER, Expert_Substitute_HasIncentive
    IfRandomLessThan 12, Expert_Substitute_Main2
    AddToMoveScore -1
    IfRandomLessThan 128, Expert_Substitute_Main2
    AddToMoveScore -1
    IfRandomLessThan 128, Expert_Substitute_Main2
    AddToMoveScore -1
    GoTo Expert_Substitute_Main2

Expert_Substitute_PhazeCheck:
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_WHIRLWIND, Expert_Substitute_Phaze_TryScoreMinus3
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SOUNDPROOF, Expert_Substitute_Main2

Expert_Substitute_Phaze_TryScoreMinus3:
    IfRandomLessThan 12, Expert_Substitute_Main2
    AddToMoveScore -2
    IfRandomLessThan 128, Expert_Substitute_Main2
    AddToMoveScore -1
    GoTo Expert_Substitute_Main2

Expert_Substitute_HasIncentive:
    IfRandomLessThan 64, Expert_Substitute_Main2
    AddToMoveScore 3
    GoTo Expert_Substitute_Main2

Expert_Substitute_Main2:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ENCORE, Expert_Substitute_CheckSpeed
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TAUNT, Expert_Substitute_CheckSpeed
    IfRandomLessThan 128, Expert_Substitute_CheckUserHP
    AddToMoveScore 1
    GoTo Expert_Substitute_CheckUserHP

Expert_Substitute_CheckSpeed:
    IfSpeedCompareNotEqualTo COMPARE_SPEED_FASTER, ScoreMinus12
    IfRandomLessThan 136, ScoreMinus12
    GoTo Expert_Substitute_CheckUserHP

Expert_Substitute_CheckUserHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 74, Expert_Substitute_HighHP
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 51, Expert_Substitute_MediumHP
    IfRandomLessThan 170, Expert_Substitute_End
    AddToMoveScore 1
    GoTo Expert_Substitute_End

Expert_Substitute_HighHP:
    IfRandomLessThan 85, Expert_Substitute_End
    AddToMoveScore 1
    IfRandomLessThan 85, Expert_Substitute_End
    AddToMoveScore 1
    GoTo Expert_Substitute_End

Expert_Substitute_MediumHP:
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_Substitute_End
    AddToMoveScore -2
    GoTo Expert_Substitute_End

Expert_Substitute_End:
    PopOrEnd 

Expert_Substitute_HighPriorityPinchBerries:
    TableEntry HOLD_EFFECT_PINCH_ATK_UP
    TableEntry HOLD_EFFECT_PINCH_SPEED_UP
    TableEntry HOLD_EFFECT_PINCH_SPATK_UP
    TableEntry HOLD_EFFECT_PINCH_RANDOM_UP
    TableEntry HOLD_EFFECT_PINCH_PRIORITY
    TableEntry TABLE_END

Expert_Substitute_LowPriorityPinchBerries:
    TableEntry HOLD_EFFECT_PINCH_DEF_UP
    TableEntry HOLD_EFFECT_PINCH_SPDEF_UP
    TableEntry HOLD_EFFECT_PINCH_CRITRATE_UP
    TableEntry HOLD_EFFECT_PINCH_ACC_UP
    TableEntry TABLE_END

Expert_RechargeTurn:
    ; If the opponent would resist or is immune to the move, score -1.
    ;
    ; If the attacker''s ability is Truant, 68.75% chance of score +1.
    ;
    ; If the attacker is slower than the opponent and the attacker''s HP is >= 60%, score -1.
    ;
    ; If the attacker is faster than the opponent and the attacker''s HP is > 40%, score -1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_RechargeTurn_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_RechargeTurn_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_RechargeTurn_ScoreMinus1
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_TRUANT, Expert_RechargeTurn_TryScorePlus1
	IfLoadedEqualTo ABILITY_RELENTLESS, Expert_RechargeTurn_CheckRelentless
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_RechargeTurn_CheckUserHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 40, Expert_RechargeTurn_ScoreMinus1
    GoTo Expert_RechargeTurn_End

Expert_RechargeTurn_TryScorePlus1:
    IfRandomLessThan 80, Expert_RechargeTurn_End
    AddToMoveScore 1
    GoTo Expert_RechargeTurn_End

Expert_RechargeTurn_CheckUserHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 60, Expert_RechargeTurn_End

Expert_RechargeTurn_ScoreMinus1:
    AddToMoveScore -1
    GoTo Expert_RechargeTurn_End

Expert_RechargeTurn_CheckRelentless:
    AddToMoveScore 1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_RechargeTurn_End
	AddToMoveScore 2
    GoTo Expert_RechargeTurn_End

Expert_RechargeTurn_End:
    PopOrEnd

Expert_Disable:
    ; If the attacker is slower than the opponent, try to preemptively use Disable.
    ;
    ; If the opponent''s last move is in the tables, +3 or +1 depending on the table.
    ;
    ; If the move is not in the tables and is a status move, 95% chance for score -1.
    ;
    ; 25% Chance for flat score +1 into a damaging move.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Disable_Slower
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    IfLoadedEqualTo MOVE_NONE, ScoreMinus12
    LoadEffectOfLoadedMove
    IfLoadedInTable Expert_Disable_HighPriorityDisable, Try75ChanceForScorePlus3
    IfLoadedInTable Expert_Disable_HighPriorityDisable, Try66ChanceForScorePlus1
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadPowerOfLoadedMove
    IfLoadedGreaterThan 100, Try66ChanceForScorePlus1
    IfLoadedEqualTo 0, Expert_Disable_TryScoreMinus1
    IfRandomLessThan 192, Expert_Disable_End
    AddToMoveScore 1
    GoTo Expert_Disable_End

Expert_Disable_Slower:
    AddToMoveScore 1
    IfBattlerIsPhysicalAttacker AI_BATTLER_DEFENDER, Try66ChanceForScorePlus1
    IfBattlerIsSpecialAttacker AI_BATTLER_DEFENDER, Try66ChanceForScorePlus1
    AddToMoveScore -1
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadEffectOfLoadedMove
    IfLoadedInTable Expert_Disable_HighPriorityDisable, Try75ChanceForScorePlus3
    IfLoadedInTable Expert_Disable_HighPriorityDisable, Try66ChanceForScorePlus1
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadPowerOfLoadedMove
    IfLoadedGreaterThan 100, Try66ChanceForScorePlus1
    IfLoadedEqualTo 0, Expert_Disable_TryScoreMinus1
    IfRandomLessThan 192, Expert_Disable_End
    AddToMoveScore 1
    GoTo Expert_Disable_End

Expert_Disable_TryScoreMinus1:
    IfRandomLessThan 12, Expert_Disable_End
    AddToMoveScore -1

Expert_Disable_End:
    PopOrEnd

Expert_Disable_HighPriorityDisable:
    TableEntry BATTLE_EFFECT_FORCE_SWITCH
    TableEntry BATTLE_EFFECT_ACC_DOWN_2
    TableEntry BATTLE_EFFECT_ACC_DOWN
    TableEntry BATTLE_EFFECT_STATUS_SLEEP
    TableEntry BATTLE_EFFECT_COUNTER
    TableEntry BATTLE_EFFECT_MIRROR_COAT
    TableEntry BATTLE_EFFECT_EVA_UP_2
    TableEntry BATTLE_EFFECT_EVA_UP
    TableEntry BATTLE_EFFECT_RECOVER_HALF_DAMAGE_DEALT
    TableEntry BATTLE_EFFECT_CONTINUE_AND_CONFUSE_SELF
    TableEntry BATTLE_EFFECT_ATK_SPD_UP
    TableEntry BATTLE_EFFECT_RESTORE_HALF_HP
    TableEntry BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN
    TableEntry BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE
    TableEntry BATTLE_EFFECT_DEF_SPD_UP
    TableEntry BATTLE_EFFECT_SP_ATK_SP_DEF_UP
    TableEntry BATTLE_EFFECT_CALL_RANDOM_MOVE
    TableEntry BATTLE_EFFECT_MULTI_HIT_TEN
    TableEntry BATTLE_EFFECT_MULTI_HIT
    TableEntry BATTLE_EFFECT_BEAT_UP
    TableEntry BATTLE_EFFECT_HIT_THREE_TIMES
    TableEntry BATTLE_EFFECT_RAISE_DEF_HIT
    TableEntry BATTLE_EFFECT_PREGNANCY_PUNCH
    TableEntry BATTLE_EFFECT_EGG_BOMB
    TableEntry BATTLE_EFFECT_SET_SUBSTITUTE
    TableEntry BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER
    TableEntry BATTLE_EFFECT_DECREASE_LAST_MOVE_PP
    TableEntry BATTLE_EFFECT_SPEED_DOWN_HIT
    TableEntry BATTLE_EFFECT_INFATUATE_HIT
    TableEntry BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP
    TableEntry BATTLE_EFFECT_THAW_AND_BURN_HIT
    TableEntry BATTLE_EFFECT_HIT_BEFORE_SWITCH
    TableEntry BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING
    TableEntry BATTLE_EFFECT_UPROAR
    TableEntry BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION
    TableEntry BATTLE_EFFECT_MAKE_GLOBAL_TARGET
    TableEntry BATTLE_EFFECT_USE_RANDOM_ALLY_MOVE
    TableEntry BATTLE_EFFECT_STEAL_STATUS_MOVE
    TableEntry BATTLE_EFFECT_DOUBLE_POWER_EACH_TURN_LOCK_INTO
    TableEntry BATTLE_EFFECT_DOUBLE_POWER_EACH_TURN
    TableEntry BATTLE_EFFECT_RAISE_ATTACK_HIT
    TableEntry BATTLE_EFFECT_CURE_PARTY_STATUS
    TableEntry BATTLE_EFFECT_HEAL_STATUS
    TableEntry BATTLE_EFFECT_HOWL
    TableEntry BATTLE_EFFECT_BULLDOZE
    TableEntry BATTLE_EFFECT_FORCE_SWITCH_HIT
    TableEntry BATTLE_EFFECT_RAISE_SP_ATK_HIT
    TableEntry BATTLE_EFFECT_RANDOM_STAT_UP_2
    TableEntry BATTLE_EFFECT_METAL_BURST
    TableEntry BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING
    TableEntry BATTLE_EFFECT_PROTECT
    TableEntry BATTLE_EFFECT_RECHARGE_AFTER
    TableEntry BATTLE_EFFECT_LOWER_SP_DEF_2_HIT
    TableEntry BATTLE_EFFECT_POISON_MULTI_HIT
    TableEntry TABLE_END

Expert_Disable_LowPriorityDisable:
    TableEntry BATTLE_EFFECT_LEVEL_DAMAGE_FLAT
    TableEntry BATTLE_EFFECT_ATK_UP
    TableEntry BATTLE_EFFECT_ATK_UP_2
    TableEntry BATTLE_EFFECT_DEF_UP
    TableEntry BATTLE_EFFECT_DEF_UP_2
    TableEntry BATTLE_EFFECT_SP_DEF_UP
    TableEntry BATTLE_EFFECT_SP_DEF_UP_2
    TableEntry BATTLE_EFFECT_SP_ATK_UP
    TableEntry BATTLE_EFFECT_SP_ATK_UP_2
    TableEntry BATTLE_EFFECT_ATK_DEF_UP
    TableEntry BATTLE_EFFECT_GROWTH
    TableEntry BATTLE_EFFECT_AVERAGE_HP
    TableEntry BATTLE_EFFECT_HEAL_IN_3_TURNS
    TableEntry BATTLE_EFFECT_SP_DEF_UP_DOUBLE_ELECTRIC_POWER
    TableEntry BATTLE_EFFECT_DEF_UP_DOUBLE_ROLLOUT_POWER
    TableEntry BATTLE_EFFECT_SPIKES_MULTI_HIT
    TableEntry BATTLE_EFFECT_HIT_TWICE
    TableEntry BATTLE_EFFECT_PRIORITY_1
    TableEntry BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN
    TableEntry BATTLE_EFFECT_RANDOM_POWER_BASED_ON_IVS
    TableEntry BATTLE_EFFECT_RAISE_ALL_STATS_HIT
    TableEntry BATTLE_EFFECT_BOOST_ALLY_POWER_BY_50_PERCENT
    TableEntry BATTLE_EFFECT_REMOVE_SCREENS
    TableEntry BATTLE_EFFECT_CURSE
    TableEntry BATTLE_EFFECT_INCREASE_POWER_WITH_WEIGHT
    TableEntry BATTLE_EFFECT_DEF_DOWN
    TableEntry BATTLE_EFFECT_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_ATK_DOWN
    TableEntry BATTLE_EFFECT_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2
    TableEntry TABLE_END


Expert_Counter:
    ; If behind substitute, score -10.
    ;
    ; If defender has no physical attacks whatsoever, score -30.
    ;
    ; If defender knows Disable, Torment, or Encore, score -2 to -4.
    ;
    ; Score +1 if the defender is a physical attacker.
    ;
    ; 75% chance to predict defender wake up turn and get score +1.
    ; Otherwise, score -3 if the defender is asleep.
    ;
    ; If defender has a status or special move, check if we have
    ; used Counter yet. If not, 66% chance for score +1.
    ;
    ; If defender has status move, check if defender is taunted.
    ; If defender is taunted, score +1. Otherwise, 25% chance for
    ; score -1.
    ;
    ; If enemy chunks or KOs us, check if we have more than 56%
    ; HP remaining. If not, 87.5% chance for score -2.
    IfBattlerHasNoPhysicalAttack AI_BATTLER_DEFENDER, ScoreMinus30
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, ScoreMinus10
    IfBattlerIsPhysicalAttacker AI_BATTLER_DEFENDER, Expert_Counter_Main
    IfRandomLessThan 8, Expert_Counter_Main
    GoTo ScoreMinus3

Expert_Counter_Main:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DISABLE, Expert_Counter_TryScoreMinus4
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TORMENT, Expert_Counter_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ENCORE, Expert_Counter_TryScoreMinus4
    AddToMoveScore 1
    IfEnemyCanChunkOrKO Expert_Counter_CheckHP
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Expert_Counter_CheckSleepTurns
    IfBattlerHasStatusMove AI_BATTLER_DEFENDER, Expert_Counter_HasStatus
    IfBattlerIsPhysicalAttacker AI_BATTLER_DEFENDER, Expert_Counter_CheckPP
    IfRandomLessThan 64, Expert_Counter_End
    AddToMoveScore 1
    GoTo Expert_Counter_End

Expert_Counter_CheckHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 56, Expert_Counter_TryScoreMinus4
    IfRandomLessThan 32, Expert_Counter_End
    AddToMoveScore -2
    GoTo Expert_Counter_End

Expert_Counter_HasStatus:
    AddToMoveScore 1
    IfTargetIsTaunted Expert_Counter_CheckPP
    AddToMoveScore -1
    IfRandomLessThan 192, Expert_Counter_CheckPP
    AddToMoveScore -1
    GoTo Expert_Counter_CheckPP

Expert_Counter_CheckSleepTurns:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP, Expert_Counter_SleepTalk
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DAMAGE_WHILE_ASLEEP, ScorePlus1
    LoadSleepTurns AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, ScoreMinus3
    IfRandomLessThan 64, ScoreMinus3
    AddToMoveScore 1
    IfRandomLessThan 64, ScoreMinus1
    AddToMoveScore 1
    GoTo Expert_Counter_End

Expert_Counter_CheckPP:
    IfCurrentMoveRevealed Expert_Counter_TryScoreMinus2
    IfRandomLessThan 85, Expert_Counter_End
    AddToMoveScore 1
    GoTo Expert_Counter_End

Expert_Counter_SleepTalk:
    IfTargetIsTaunted ScoreMinus30
    IfRandomLessThan 25, Expert_Counter_End
    AddToMoveScore 1
    GoTo Expert_Counter_End

Expert_Counter_TryScoreMinus4:
    IfRandomLessThan 25, Expert_Counter_End
    AddToMoveScore -4
    GoTo Expert_Counter_End

Expert_Counter_TryScoreMinus2:
    IfRandomLessThan 128, Expert_Counter_End
    AddToMoveScore -2
    GoTo Expert_Counter_End

Expert_Counter_End:
    PopOrEnd

Expert_Encore:
    ; If first turn, try to encore at +1 to +3 score if slower.
    ;
    ; If target is disabled, try to encore at +3 score.
    ;
    ; Otherwise, follow logic function to encore.
    IfBattlerUnderEffect AI_BATTLER_DEFENDER, CHECK_DISABLE, Expert_Encore_TryScorePlus3
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_Encore_FirstTurn
    IfShouldEncore AI_BATTLER_DEFENDER, ScorePlus3
    AddToMoveScore -3
    GoTo Expert_Encore_End

Expert_Encore_TryScorePlus3:
    IfRandomLessThan 12, Expert_Encore_End
    AddToMoveScore 3
    GoTo Expert_Encore_End

Expert_Encore_FirstTurn:
    IfSpeedCompareNotEqualTo COMPARE_SPEED_SLOWER, ScoreMinus12
    AddToMoveScore 1
    IfRandomLessThan 32, Expert_Encore_End
    AddToMoveScore 1
    IfRandomLessThan 32, Expert_Encore_End
    AddToMoveScore 1
    GoTo Expert_Encore_End

Expert_Encore_End:
    PopOrEnd 

Expert_PainSplit:
    ; If the opponent''s HP < 80%, score -1.
    ;
    ; If the attacker is slower than its opponent:
    ; - If the attacker''s HP > 60%, score -1.
    ; - Otherwise, score +1.
    ;
    ; If the attacker''s HP > 40%, score -1.
    ;
    ; Otherwise, score -1.
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_MOVE_IS_HIGHEST_DAMAGE, Try95ChanceForScorePlus3
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 50, Expert_PainSplit_ScoreMinus1
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_PainSplit_CheckUserHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 40, Expert_PainSplit_ScoreMinus1
    AddToMoveScore 1
    GoTo Expert_PainSplit_End

Expert_PainSplit_CheckUserHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 60, Expert_PainSplit_ScoreMinus1
    AddToMoveScore 1
    GoTo Expert_PainSplit_End

Expert_PainSplit_ScoreMinus1:
    AddToMoveScore -1

Expert_PainSplit_End:
    PopOrEnd 

Expert_Nightmare:
    ; Score +2.
    AddToMoveScore 2
    PopOrEnd 

Expert_LockOn:
    ; 50% chance of score +2.
    IfRandomLessThan 128, Expert_LockOn_End
    AddToMoveScore 2

Expert_LockOn_End:
    PopOrEnd 

Expert_SleepTalk:
    ; If the attacker is asleep, score +10.
    ;
    ; Otherwise, score -5.
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_SLEEP, ScorePlus10
    AddToMoveScore -5
    PopOrEnd 

Expert_DestinyBond:
    ; Start at score -1. If the attacker is slower than its opponent, terminate.
    ;
    ; If the attacker''s HP > 70%, terminate. Otherwise, 50% chance of additional score +1.
    ;
    ; If the attacker''s HP > 50%, terminate. Otherwise, 50% chance of additional score +1.
    ;
    ; If the attacker''s HP > 30%, terminate. Otherwise, 60.9% chance of additional score +2.
	IfDestinyBondFails AI_BATTLER_ATTACKER, Expert_DestinyBond_Minus10
    IfEnemyCanChunkOrKO Expert_DestinyBond_Yolo
    AddToMoveScore -1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_DestinyBond_End
    IfRandomLessThan 128, Expert_DestinyBond_CheckUserMediumHP
    AddToMoveScore 1

Expert_DestinyBond_CheckUserMediumHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 55, Expert_DestinyBond_End
    IfRandomLessThan 128, Expert_DestinyBond_CheckUserLowHP
    AddToMoveScore 1

Expert_DestinyBond_CheckUserLowHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 30, Expert_DestinyBond_End
    IfRandomLessThan 100, Expert_DestinyBond_End
    AddToMoveScore 2

Expert_DestinyBond_Minus10:
	AddToMoveScore -10
    PopOrEnd 

Expert_DestinyBond_Yolo:
    IfRandomLessThan 100, Expert_DestinyBond_End
    AddToMoveScore 1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 50, Expert_DestinyBond_End
    IfRandomLessThan 12, Expert_DestinyBond_End
    AddToMoveScore 3
    GoTo Expert_DestinyBond_End
	
Expert_DestinyBond_End:
    PopOrEnd

Expert_Reversal:
    ; If the attacker is slower than its opponent:
    ; - If the attacker''s HP > 60%, score -1.
    ; - If the attacker''s HP > 40%, score +0.
    ; - Otherwise, 60.9% chance of score +1.
    ;
    ; If the attacker is faster than its opponent:
    ; - If the attacker''s HP > 33%, score -1.
    ; - If the attacker''s HP > 20%, score +0.
    ; - If the attacker''s HP >= 8%, 60.9% chance of score +1.
    ; - If the attacker''s HP < 8%, 60.9% chance of score +2, 39.1% chance of score +1.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Reversal_SlowerCheckHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 33, Expert_Reversal_ScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 20, Expert_Reversal_End
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 8, Expert_Reversal_ScorePlus1
    GoTo Expert_Reversal_TryScorePlus1

Expert_Reversal_SlowerCheckHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 60, Expert_Reversal_ScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 40, Expert_Reversal_End
    GoTo Expert_Reversal_TryScorePlus1

Expert_Reversal_ScorePlus1:
    AddToMoveScore 1

Expert_Reversal_TryScorePlus1:
    IfRandomLessThan 100, Expert_Reversal_End
    AddToMoveScore 1
    GoTo Expert_Reversal_End

Expert_Reversal_ScoreMinus1:
    AddToMoveScore -1

Expert_Reversal_End:
    PopOrEnd 

Expert_HealBell:
    ; If neither the attacker nor any of its party members have a non-volatile status condition,
    ; score -5.
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TAUNT, Expert_HealBell_CheckSpeed
    GoTo Expert_HealBell_Main

Expert_HealBell_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_HealBell_SlowIntoTaunt
    GoTo Expert_HealBell_Main

Expert_HealBell_SlowIntoTaunt:
    IfRandomLessThan 32, Expert_HealBell_Main
    AddToMoveScore -1
    GoTo Expert_HealBell_Main

Expert_HealBell_Main:
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_HealBell_CheckStatusThreat
    IfPartyMemberStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_HealBell_End
    AddToMoveScore -10
    GoTo Expert_HealBell_End

Expert_HealBell_CheckStatusThreat:
    IfHasStatusThreat AI_BATTLER_DEFENDER, Expert_HealBell_VsStatuser
    IfRandomLessThan 12, Expert_HealBell_End
    AddToMoveScore 3
    GoTo Expert_HealBell_End

Expert_HealBell_VsStatuser:
    AddToMoveScore 1
    IfRandomLessThan 85, Expert_HealBell_End
    AddToMoveScore -2
    GoTo Expert_HealBell_End

Expert_HealBell_PartyStatusBonus:
    IfRandomLessThan 2, Expert_HealBell_End
    AddToMoveScore 3
    GoTo Expert_HealBell_End

Expert_HealBell_End:
    PopOrEnd 

Expert_Thief:
    ; If the opponent''s held item does NOT have one of the encouraged effects, score -2.
    ;
    ; Otherwise, 80.5% chance of score +1.
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedNotInTable Expert_Thief_EncouragedItemEffects, Expert_Thief_ScoreMinus2
    IfRandomLessThan 50, Expert_Thief_End
    AddToMoveScore 1
    GoTo Expert_Thief_End

Expert_Thief_ScoreMinus2:
    AddToMoveScore -2

Expert_Thief_End:
    PopOrEnd 

Expert_Thief_EncouragedItemEffects:
    TableEntry HOLD_EFFECT_SLP_RESTORE
    TableEntry HOLD_EFFECT_STATUS_RESTORE
    TableEntry HOLD_EFFECT_HP_RESTORE
    TableEntry HOLD_EFFECT_ACC_REDUCE
    TableEntry HOLD_EFFECT_HP_RESTORE_GRADUAL
    TableEntry HOLD_EFFECT_PIKA_SPATK_UP
    TableEntry HOLD_EFFECT_CUBONE_ATK_UP
    TableEntry HOLD_EFFECT_WEAKEN_SE_FIRE
    TableEntry HOLD_EFFECT_WEAKEN_SE_WATER
    TableEntry HOLD_EFFECT_WEAKEN_SE_ELECTRIC
    TableEntry HOLD_EFFECT_WEAKEN_SE_GRASS
    TableEntry HOLD_EFFECT_WEAKEN_SE_ICE
    TableEntry HOLD_EFFECT_WEAKEN_SE_FIGHT
    TableEntry HOLD_EFFECT_WEAKEN_SE_POISON
    TableEntry HOLD_EFFECT_WEAKEN_SE_GROUND
    TableEntry HOLD_EFFECT_WEAKEN_SE_FLYING
    TableEntry HOLD_EFFECT_WEAKEN_SE_PSYCHIC
    TableEntry HOLD_EFFECT_WEAKEN_SE_BUG
    TableEntry HOLD_EFFECT_WEAKEN_SE_ROCK
    TableEntry HOLD_EFFECT_WEAKEN_SE_GHOST
    TableEntry HOLD_EFFECT_WEAKEN_SE_DRAGON
    TableEntry HOLD_EFFECT_WEAKEN_SE_DARK
    TableEntry HOLD_EFFECT_WEAKEN_SE_STEEL
    TableEntry HOLD_EFFECT_WEAKEN_NORMAL
    TableEntry HOLD_EFFECT_HP_RESTORE_PSN_TYPE
    TableEntry TABLE_END

Expert_Curse:
    ; If the attacker has a Ghost typing:
    ; - If the attacker''s HP > 80%, score +0.
    ; - Otherwise, score -1.
    ;
    ; If the attacker''s Defense stat stage is at +3 or higher, score +0 and terminate.
    ;
    ; If the attacker knows the move Gyro Ball or Trick Room, 87.5% chance of additional score +1.
    ;
    ; 50% chance from here-on of additional score +1.
    ;
    ; If the attacker''s Defense stat stage is at +1 or lower, 50% chance of additional score +1.
    ;
    ; If the attacker''s Defense stat stage is at +0 or lower, 50% chance of additional score +1.
    ; (This is cumulative with the previous check.)
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, Expert_Curse_GhostCheckHP
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, Expert_Curse_GhostCheckHP
    IfBattlerDetersBoosting AI_BATTLER_DEFENDER, ScoreMinus3
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_Curse_ChanceForScoreMinus2
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 75, Expert_Curse_ChanceForScoreMinus1
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 8, Expert_Curse_ChanceForScoreMinus2
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 7, Expert_Curse_ChanceForScoreMinus1
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_GYRO_BALL, Expert_Curse_HighChanceScorePlus1
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_TRICK_ROOM, Expert_Curse_HighChanceScorePlus1
    GoTo Expert_Curse_FlipCoinScorePlus1

Expert_Curse_ChanceForScoreMinus1:
    IfRandomLessThan 85, Expert_Curse_End
    AddToMoveScore -1
    GoTo Expert_Curse_End

Expert_Curse_ChanceForScoreMinus2:
    IfRandomLessThan 26, Expert_Curse_End
    AddToMoveScore -2
    GoTo Expert_Curse_End

Expert_Curse_HighChanceScorePlus1:
    IfRandomLessThan 32, Expert_Curse_CheckDefenseStage
    AddToMoveScore 1

Expert_Curse_FlipCoinScorePlus1:
    IfRandomLessThan 128, Expert_Curse_CheckDefenseStage
    AddToMoveScore 1

Expert_Curse_CheckDefenseStage:
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 7, Expert_Curse_End
    IfRandomLessThan 128, Expert_Curse_CheckDefenseStageAnyBoosts
    AddToMoveScore 1

Expert_Curse_CheckDefenseStageAnyBoosts:
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 6, Expert_Curse_End
    IfRandomLessThan 128, Expert_Curse_End
    AddToMoveScore 1
    GoTo Expert_Curse_End

Expert_Curse_GhostCheckHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_Curse_GhostLowHealthBonus
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 80, Expert_Curse_End
    AddToMoveScore -1
    GoTo Expert_Curse_End

Expert_Curse_GhostLowHealthBonus:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 12, ScorePlus10
    IfRandomLessThan 128, Expert_Curse_End
    AddToMoveScore 1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 33, Expert_Curse_End
    IfRandomLessThan 32, Expert_Curse_End
    AddToMoveScore 1
    GoTo Expert_Curse_End


Expert_Curse_End:
    PopOrEnd 

Expert_Protect:
    ; Attempts to rack up chip damage and passive healing.
    ;
    ; Special procedure to initialize status-item abilities like Guts and Poison Heal
    ; as well as Speed Boost.
    ;
    ; Will try especially to block an incoming charge-turn move.
    ; 
    ; Ignores protect when opponent is Asleep or Frozen.
    ;
    ; Will attempt to stall out non-beneficial field effects, and ignore protect
    ; while beneficial field effects are up.
    ;
    ; Special procedure to give high chance of protect if wish has been used.
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FEINT, Expert_Protect_TryScoreMinus2
    LoadProtectChain AI_BATTLER_ATTACKER
    IfLoadedGreaterThan 0, Expert_Protect_StreakBreaker
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_Protect_CheckItemStatusAbility
    IfFieldConditionsMask FIELD_CONDITION_SANDSTORM, Expert_Protect_CheckSandstorm
    IfFieldConditionsMask FIELD_CONDITION_HAILING, Expert_Protect_CheckHail
    IfFieldConditionsMask FIELD_CONDITION_RAINING, Expert_Protect_CheckRain
    IfFieldConditionsMask FIELD_CONDITION_SUNNY, Expert_Protect_CheckSun
    IfFieldConditionsMask FIELD_CONDITION_TRICK_ROOM, Expert_Protect_CheckTrickRoom
    IfFieldConditionsMask FIELD_CONDITION_GRAVITY, Expert_Protect_CheckGravity
    IfWishActive AI_BATTLER_ATTACKER, Expert_Protect_WishTryScorePlus3
    IfRandomLessThan 32, ScoreMinus2
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_StreakBreaker:
    IfRandomLessThan 8, Expert_Protect_End
    AddToMoveScore -30
    GoTo Expert_Protect_End

Expert_Protect_CheckItemStatusAbility:
    IfRandomLessThan 16, Expert_Protect_CheckStatusConditions
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SPEED_BOOST, ScorePlus2
    AddToMoveScore -1
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_Protect_CheckItem
    AddToMoveScore 4
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_Protect_ItemStatusAbilities, Expert_Protect_CheckStatusConditions
    AddToMoveScore -2
    IfRandomLessThan 64, Expert_Protect_CheckStatusConditions
    AddToMoveScore -2
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_ItemStatusAbilities:
    TableEntry ABILITY_GUTS
    TableEntry ABILITY_MARVEL_SCALE
    TableEntry ABILITY_FLARE_BOOST
    TableEntry ABILITY_POISON_HEAL
    TableEntry TABLE_END

Expert_Protect_CheckSandstorm:
    AddToMoveScore 2
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedInTable Expert_Protect_SandstormTypes, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedInTable Expert_Protect_SandstormTypes, Expert_Protect_CheckStatusConditions
    AddToMoveScore -4
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedInTable Expert_Protect_SandstormTypes, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedInTable Expert_Protect_SandstormTypes, Expert_Protect_CheckStatusConditions
    AddToMoveScore 4
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Protect_CheckStatusConditions
    AddToMoveScore -4
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Protect_CheckStatusConditions
    AddToMoveScore 2
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_SandstormTypes:
    TableEntry TYPE_ROCK
    TableEntry TYPE_GROUND
    TableEntry TYPE_STEEL
    TableEntry TABLE_END

Expert_Protect_CheckHail:
    AddToMoveScore 2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_Protect_HailAbilities, Expert_Protect_CheckStatusConditions
    AddToMoveScore -4
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Protect_HailAbilities, Expert_Protect_CheckStatusConditions
    AddToMoveScore 4
    IfLoadedEqualTo ABILITY_SLUSH_RUSH, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_ICE, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_ICE, Expert_Protect_CheckStatusConditions
    AddToMoveScore -4
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_ICE, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_ICE, Expert_Protect_CheckStatusConditions
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_BLIZZARD, Expert_Protect_CheckStatusConditions
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_SHEER_COLD, Expert_Protect_CheckStatusConditions
    AddToMoveScore 2
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_HailAbilities:
    TableEntry ABILITY_SNOW_CLOAK
    TableEntry ABILITY_ICE_BODY
    TableEntry TABLE_END

Expert_Protect_CheckRain:
    AddToMoveScore 2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_Protect_RainAbilities, Expert_Protect_CheckStatusConditions
    AddToMoveScore -4
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Protect_RainAbilities, Expert_Protect_CheckStatusConditions
    AddToMoveScore 4
    IfLoadedEqualTo ABILITY_SWIFT_SWIM, Expert_Protect_CheckStatusConditions
    IfLoadedEqualTo ABILITY_HYDRATION, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_WATER, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_WATER, Expert_Protect_CheckStatusConditions
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_RAZOR_WIND, Expert_Protect_CheckStatusConditions
    AddToMoveScore -2
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_RainAbilities:
    TableEntry ABILITY_RAIN_DISH
    TableEntry ABILITY_DRY_SKIN
    TableEntry TABLE_END

Expert_Protect_TryScoreMinus2:
    IfRandomLessThan 128, Expert_Protect_CheckStatusConditions
    AddToMoveScore -2
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_CheckSun:
    AddToMoveScore 2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_PHOTOSYNTHESIS, Expert_Protect_CheckStatusConditions
    AddToMoveScore -4
    IfLoadedEqualTo ABILITY_SOLAR_POWER, Expert_Protect_CheckStatusConditions
    IfLoadedEqualTo ABILITY_DRY_SKIN, Expert_Protect_CheckStatusConditions
    AddToMoveScore 4
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Protect_SunAbilities, Expert_Protect_CheckStatusConditions
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_SOLAR_BEAM, Expert_Protect_CheckStatusConditions
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_WEATHER_BALL, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, Expert_Protect_CheckStatusConditions
    AddToMoveScore -2
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_SunAbilities:
    TableEntry ABILITY_CHLOROPHYLL
    TableEntry ABILITY_SOLAR_POWER
    TableEntry ABILITY_LEAF_GUARD
    TableEntry ABILITY_FLOWER_GIFT
    TableEntry ABILITY_CHLOROPLAST
    TableEntry TABLE_END

Expert_Protect_CheckTrickRoom:
    AddToMoveScore 2
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_Protect_CheckStatusConditions
    AddToMoveScore -2
    IfSpeedCompareEqualTo COMPARE_SPEED_TIE, Expert_Protect_CheckStatusConditions
    IfRandomLessThan 64, Expert_Protect_CheckStatusConditions
    AddToMoveScore -2
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_CheckGravity:
    AddToMoveScore 2
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GROUND, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GROUND, Expert_Protect_CheckStatusConditions
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_TIDAL_FORCE, Expert_Protect_CheckStatusConditions
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_LEVITATE, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_FLYING, Expert_Protect_CheckStatusConditions
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_FLYING, Expert_Protect_CheckStatusConditions
    AddToMoveScore -2
    IfRandomLessThan 224, Expert_Protect_CheckStatusConditions
    AddToMoveScore 1
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_WishTryScorePlus3:
    IfRandomLessThan 32, Expert_Protect_CheckStatusConditions
    AddToMoveScore 3
    GoTo Expert_Protect_CheckStatusConditions

Expert_Protect_CheckStatusConditions:
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, Expert_Protect_CheckPoisonHeal
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_BURN, Expert_Protect_CheckMagicGuard
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED, Expert_Protect_CheckMagicGuard
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_CURSE, Expert_Protect_CheckMagicGuard
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_BIND, Expert_Protect_CheckMagicGuard
    AddToMoveScore 3
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_UNDERGROUND, Expert_Protect_CheckItem
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_UNDERWATER, Expert_Protect_CheckItem
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_AIRBORNE, Expert_Protect_CheckItem
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_CHARGE, Expert_Protect_CheckItem
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_MOVE_LOCKED, Expert_Protect_CheckItem
    AddToMoveScore -3
    IfRandomLessThan 8, Expert_Protect_CheckItem
    AddToMoveScore 1
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY_POISON, Expert_Protect_CheckEnemyPoisonImmune
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_BURN, Expert_Protect_CheckEnemyMagicGuard
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED_RECIPIENT, Expert_Protect_CheckItem
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_AQUA_RING, Expert_Protect_CheckItem
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_INGRAIN, Expert_Protect_CheckItem
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_PERISH_SONG, Expert_Protect_CheckItem
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CURSE, Expert_Protect_CheckEnemyMagicGuard
    AddToMoveScore -16
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_SHADOW_FORCE, ScoreMinus12
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_YAWN, Expert_Protect_CheckItem
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Expert_Protect_CheckItem
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Expert_Protect_CheckItem
    AddToMoveScore 14
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_RESTORE_HALF_HP, Expert_Protect_CheckItem
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP, Expert_Protect_CheckItem
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE, Expert_Protect_CheckItem
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN, Expert_Protect_CheckItem
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HEAL_IN_3_TURNS, Expert_Protect_CheckItem
    AddToMoveScore 1
    IfRandomLessThan 64, Expert_Protect_CheckItem
    AddToMoveScore -1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON, Expert_Protect_CheckItem
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON, Expert_Protect_CheckItem
    AddToMoveScore 1
    GoTo Expert_Protect_CheckItem

Expert_Protect_CheckPoisonHeal:
    AddToMoveScore 2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_HEAL, Expert_Protect_CheckItem
    AddToMoveScore -5
    GoTo Expert_Protect_CheckItem

Expert_Protect_CheckMagicGuard:
    AddToMoveScore 1
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Protect_CheckItem
    AddToMoveScore -6
    GoTo Expert_Protect_CheckItem

Expert_Protect_CheckEnemyPoisonImmune:
    AddToMoveScore -1
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_POISON_HEAL, Expert_Protect_CheckItem
    IfRandomLessThan 32, Expert_Protect_CheckItem
    AddToMoveScore 1
    GoTo Expert_Protect_CheckItem

Expert_Protect_CheckEnemyMagicGuard:
    AddToMoveScore -1
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Protect_CheckItem
    IfRandomLessThan 32, Expert_Protect_CheckItem
    AddToMoveScore 1
    GoTo Expert_Protect_CheckItem

Expert_Protect_CheckItem:
    IfRandomLessThan 192, Expert_Protect_End
    AddToMoveScore 1
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_LEFTOVERS, Expert_Protect_End
    IfLoadedEqualTo ITEM_BLACK_SLUDGE, Expert_Protect_CheckBlackSludge
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedEqualTo ITEM_STICKY_BARB, Expert_Protect_End
    AddToMoveScore -2
    IfLoadedEqualTo ITEM_LEFTOVERS, Expert_Protect_End
    IfLoadedEqualTo ITEM_BLACK_SLUDGE, Expert_Protect_End
    AddToMoveScore 1
    GoTo Expert_Protect_End
    
Expert_Protect_CheckBlackSludge:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_POISON, Expert_Protect_End
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_POISON, Expert_Protect_End
    AddToMoveScore -3
    GoTo Expert_Protect_End

Expert_Protect_End:
    PopOrEnd 

Expert_Foresight:
    ; If the defender has a Ghost typing, 98.5% chance of score +2.
    ;
    ; If the target''s Evasion stat stage is at +2 or higher, 68.75% chance of score +2.
    ;
    ; Otherwise, score -2.
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, Expert_Foresight_FirstRoll
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, Expert_Foresight_FirstRoll
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GHOSTLY, Expert_Foresight_FirstRoll
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 7, Expert_Foresight_SecondRoll
    AddToMoveScore -2
    GoTo Expert_Foresight_End

Expert_Foresight_FirstRoll:
    IfRandomLessThan 80, Expert_Foresight_End

Expert_Foresight_SecondRoll:
    IfRandomLessThan 12, Expert_Foresight_End
    AddToMoveScore 2

Expert_Foresight_End:
    PopOrEnd 

Expert_Endure:
    ; If the attacker''s HP < 12%, score -1.
    ;
    ; If the attacker''s HP < 35%, 72.7% chance of score +1.
    LoadProtectChain AI_BATTLER_ATTACKER
    IfLoadedGreaterThan 0, ScoreMinus12
    IfEnemyCanKO Expert_Endure_TryScorePlus4
    IfRandomLessThan 64, Expert_Endure_Main
    AddToMoveScore -1
    IfRandomLessThan 128, Expert_Endure_Main
    AddToMoveScore -1
    GoTo Expert_Endure_Main

Expert_Endure_TryScorePlus4:
    IfRandomLessThan 12, Expert_Endure_Main
    AddToMoveScore 1
    IfRandomLessThan 64, Expert_Endure_Main
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_Endure_Main
    AddToMoveScore 1
    IfRandomLessThan 192, Expert_Endure_Main
    AddToMoveScore 1
    GoTo Expert_Endure_Main

Expert_Endure_Main:
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, Expert_Endure_CheckPoisonHeal
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_BURN, Expert_Endure_CheckMagicGuard
    IfFieldConditionsMask FIELD_CONDITION_HAILING, Expert_Endure_CheckHail
    IfFieldConditionsMask FIELD_CONDITION_SANDSTORM, Expert_Endure_CheckSand
    GoTo Expert_Endure_CheckHP

Expert_Endure_CheckPoisonHeal:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_HEAL, Expert_Endure_CheckHP
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Endure_CheckHP
    AddToMoveScore -12
    GoTo Expert_Endure_End

Expert_Endure_CheckMagicGuard:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Endure_CheckHP
    AddToMoveScore -12
    GoTo Expert_Endure_End

Expert_Endure_CheckHail:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_ICE, Expert_Endure_CheckHP
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_ICE, Expert_Endure_CheckHP
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Endure_CheckHP
	IfLoadedEqualTo ABILITY_SNOW_WARNING, Expert_Endure_CheckHP
	IfLoadedEqualTo ABILITY_SLUSH_RUSH, Expert_Endure_CheckHP
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, Expert_Endure_CheckHP
    AddToMoveScore -12
    GoTo Expert_Endure_End

Expert_Endure_CheckSand:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_ROCK, Expert_Endure_CheckHP
    IfLoadedEqualTo TYPE_GROUND, Expert_Endure_CheckHP
    IfLoadedEqualTo TYPE_STEEL, Expert_Endure_CheckHP
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_ROCK, Expert_Endure_CheckHP
    IfLoadedEqualTo TYPE_GROUND, Expert_Endure_CheckHP
    IfLoadedEqualTo TYPE_STEEL, Expert_Endure_CheckHP
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Endure_CheckHP
	IfLoadedEqualTo ABILITY_SAND_STREAM, Expert_Endure_CheckHP
	IfLoadedEqualTo ABILITY_SAND_FORCE, Expert_Endure_CheckHP
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, Expert_Endure_CheckHP
    AddToMoveScore -12
    GoTo Expert_Endure_End

Expert_Endure_CheckHP:
    LoadBattlerIgnorableAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_STURDY, Expert_Endure_SturdyHPCheck
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 25, Expert_Endure_ScoreMinus10
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_Endure_TryScorePlus1
    GoTo Expert_Endure_End

Expert_Endure_SturdyHPCheck:
    IfHPPercentEqualTo AI_BATTLER_ATTACKER, 100, ScoreMinus12
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 25, Expert_Endure_ScoreMinus10
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_Endure_TryScorePlus1
    GoTo Expert_Endure_End

Expert_Endure_ScoreMinus10:
    AddToMoveScore -10
    GoTo Expert_Endure_End

Expert_Endure_TryScorePlus1:
    IfRandomLessThan 70, Expert_Endure_End
    AddToMoveScore 1

Expert_Endure_End:
    PopOrEnd 

Expert_BatonPass:
    ; If any of the attacker''s stat stages are at +3 or higher, 75% chance of score +2 if either
    ; of the following is true:
    ; - The attacker is slower than its target and has HP <= 95%
    ; - The attacker is faster than its target and has HP <= 75%
    ; If neither are true, score +0.
    ;
    ; If any of the attacker''s stat stages are at +2, score -2 if either of the following is true:
    ; - The attacker is slower than its target and has HP <= 95%
    ; - The attacker is faster than its target and has HP <= 75%
    ; If neither are true, score +0.
    ;
    ; Otherwise, score -2.
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 8, Expert_BatonPass_HighStatStage_CheckSpeedAndHP
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 8, Expert_BatonPass_HighStatStage_CheckSpeedAndHP
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 8, Expert_BatonPass_HighStatStage_CheckSpeedAndHP
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 8, Expert_BatonPass_HighStatStage_CheckSpeedAndHP
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 8, Expert_BatonPass_HighStatStage_CheckSpeedAndHP
    GoTo Expert_BatonPass_CheckMediumStatStage

Expert_BatonPass_HighStatStage_CheckSpeedAndHP:
    AddToMoveScore 2
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_BatonPass_HighStatStage_SlowerCheckHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 75, Expert_BatonPass_End
    GoTo Expert_BatonPass_HighStatStage_TryScorePlus2

Expert_BatonPass_HighStatStage_SlowerCheckHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 95, Expert_BatonPass_End
    GoTo Expert_BatonPass_HighStatStage_TryScorePlus2

Expert_BatonPass_HighStatStage_TryScorePlus2:
    IfRandomLessThan 64, Expert_BatonPass_End
    AddToMoveScore 2
    GoTo Expert_BatonPass_End

Expert_BatonPass_CheckMediumStatStage:
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 7, Expert_BatonPass_MediumStatStage_CheckSpeedAndHP
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 7, Expert_BatonPass_MediumStatStage_CheckSpeedAndHP
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 7, Expert_BatonPass_MediumStatStage_CheckSpeedAndHP
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 7, Expert_BatonPass_MediumStatStage_CheckSpeedAndHP
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 7, Expert_BatonPass_MediumStatStage_CheckSpeedAndHP
    IfRandomLessThan 192, Expert_BatonPass_CheckMoveEffects
    AddToMoveScore -1
    GoTo Expert_BatonPass_CheckMoveEffects

Expert_BatonPass_MediumStatStage_CheckSpeedAndHP:
    AddToMoveScore 1
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_BatonPass_MediumStatStage_SlowerCheckHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 75, Expert_BatonPass_ScoreMinus2
    GoTo Expert_BatonPass_End

Expert_BatonPass_MediumStatStage_SlowerCheckHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 95, Expert_BatonPass_CheckMoveEffects
    GoTo Expert_BatonPass_ScoreMinus2

Expert_BatonPass_ScoreMinus2:
    AddToMoveScore -2
    GoTo Expert_BatonPass_CheckMoveEffects

Expert_BatonPass_CheckMoveEffects:
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_BATON_PASS_GOOD, Expert_BatonPass_TryScorePlus1
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, Expert_BatonPass_TryScorePlus2
    GoTo Expert_BatonPass_End

Expert_BatonPass_TryScorePlus1:
    IfRandomLessThan 32, Expert_BatonPass_End
    AddToMoveScore 1
    GoTo Expert_BatonPass_End

Expert_BatonPass_TryScorePlus2:
    IfRandomLessThan 170, Expert_BatonPass_End
    AddToMoveScore 2
    GoTo Expert_BatonPass_End

Expert_BatonPass_End:
    PopOrEnd 

Expert_Pursuit:
    ; If Pursuit kills, score +10.
    ;
    ; If defender has Rattled, score -3.
    ;
    ; If the defender is trapped, -1 score.
    ;
    ;
    ; If it is the attacker''s first turn in battle, 50% chance of additional score +1.
    ;
    ; If the defender can deter contact moves, 50% chance for score -2,
    ; 50% chance for score -4.
    ;
    ; If the defender has a relevant pivoting move, 75% chance for score +1.
    ;
    ; If the attacker has an ability that makes Pursuit better, score +1.
    ; Otherwise, 12.5% chance for score -1.
    IfCurrentMoveKills ROLL_FOR_DAMAGE, ScorePlus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_RATTLED, ScoreMinus3
    IfTrapped AI_BATTLER_DEFENDER, Expert_Pursuit_TrappedPenalty
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_Pursuit_TryScorePlus1
    IfBattlerDetersContactMove AI_BATTLER_DEFENDER, Expert_Pursuit_DeterContact
    GoTo Expert_Pursuit_CheckPivot

Expert_Pursuit_TrappedPenalty:
    IfRandomLessThan 12, Expert_Pursuit_CheckAttackerAbility
    AddToMoveScore -1
    GoTo Expert_Pursuit_CheckAttackerAbility
    
Expert_Pursuit_DeterContact:
    AddToMoveScore -2
    IfRandomLessThan 128, Expert_Pursuit_CheckPivot
    AddToMoveScore -2
    GoTo Expert_Pursuit_CheckPivot

Expert_Pursuit_TryScorePlus1:
    IfRandomLessThan 128, Expert_Pursuit_CheckPivot
    AddToMoveScore 1

Expert_Pursuit_CheckPivot:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SWITCH_HIT_NO_ANIM, Expert_Pursuit_PivotMove
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SWITCH_HIT, Expert_Pursuit_PivotMove
    GoTo Expert_Pursuit_CheckAttackerAbility

Expert_Pursuit_PivotMove:
    IfRandomLessThan 64, Expert_Pursuit_CheckAttackerAbility
    AddToMoveScore 1
    GoTo Expert_Pursuit_CheckAttackerAbility

Expert_Pursuit_CheckAttackerAbility:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_Pursuit_AttackerAbilities, Expert_Pursuit_AbilityBonus
    IfRandomLessThan 224, Expert_Pursuit_End
    AddToMoveScore -1
    GoTo Expert_Pursuit_End

Expert_Pursuit_AttackerAbilities:
    TableEntry ABILITY_TECHNICIAN
    TableEntry ABILITY_POISON_TOUCH
    TableEntry ABILITY_SHAKEDOWN
    TableEntry ABILITY_FREE_SAMPLE
    TableEntry TABLE_END

Expert_Pursuit_AbilityBonus:
    IfRandomLessThan 128, Expert_Pursuit_End
    AddToMoveScore 1
    GoTo Expert_Pursuit_End

Expert_Pursuit_End:
    PopOrEnd 

Expert_RainDance:
    ; If the attacker is slower than its opponent and has the ability Swift Swim, score +1 and
    ; terminate.
    ;
    ; If the attacker''s HP < 40%, score -1.
    ;
    ; If the current weather is Hail, Sun, or Sandstorm, score +1.
    ;
    ; If the attacker has the ability Rain Dish or is statused and has the ability Hydration,
    ; score +1.
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_RainDance_OtherChecks
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SWIFT_SWIM, Expert_RainDance_TryScorePlus10

Expert_RainDance_OtherChecks:
    LoadCurrentWeather 
    IfLoadedNotEqualTo AI_WEATHER_RAINING, Expert_RainDance_TryScorePlus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_RAIN_DISH, Expert_RainDance_TryScorePlus10
    IfLoadedNotEqualTo ABILITY_HYDRATION, Expert_RainDance_End
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_RainDance_TryScorePlus10
    GoTo Expert_RainDance_End

Expert_RainDance_TryScorePlus10:
    AddToMoveScore 3
    IfRandomLessThan 64, Expert_RainDance_End
    AddToMoveScore 7
    GoTo Expert_RainDance_End

Expert_RainDance_End:
    PopOrEnd 

Expert_SunnyDay:
    ; If the attacker''s HP < 40%, score -1.
    ;
    ; If the current weather is Hail, Rain, or Sandstorm, score +1.
    ;
    ; If the attacker has the ability Flower Gift or is statused and has the ability Leaf Guard,
    ; score +1.
    LoadCurrentWeather 
    IfLoadedNotEqualTo AI_WEATHER_SUNNY, Expert_SunnyDay_TryScorePlus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_FLOWER_GIFT, Expert_SunnyDay_TryScorePlus10
    IfLoadedNotEqualTo ABILITY_LEAF_GUARD, Expert_SunnyDay_End
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_SunnyDay_TryScorePlus10
    GoTo Expert_SunnyDay_End

Expert_SunnyDay_TryScorePlus10:
    AddToMoveScore 3
    IfRandomLessThan 64, Expert_SunnyDay_End
    AddToMoveScore 7
    GoTo Expert_SunnyDay_End

Expert_SunnyDay_End:
    PopOrEnd 

Expert_Sandstorm:
    ;
    ; If the current weather is Hail, Rain, or Sun, score +1.
    ;
    ; If the attacker has the ability Flower Gift or is statused and has the ability Leaf Guard,
    ; score +1.
    LoadCurrentWeather 
    IfLoadedNotEqualTo AI_WEATHER_SANDSTORM, Expert_Sandstorm_TryScorePlus10
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SAND_FORCE, Expert_Sandstorm_TryScorePlus10
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_Sandstorm_TryScorePlus10
    GoTo Expert_Sandstorm_End

Expert_Sandstorm_TryScorePlus10:
    AddToMoveScore 3
    IfRandomLessThan 64, Expert_Sandstorm_End
    AddToMoveScore 7
    GoTo Expert_Sandstorm_End

Expert_Sandstorm_End:
    PopOrEnd 

Expert_BellyDrum:
    ; If the attacker is at less than 74% HP, score -2.
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 10, Expert_BellyDrum_End
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_UNAWARE, ScoreMinus10
    LoadHeldItem AI_BATTLER_ATTACKER
	IfLoadedEqualTo ABILITY_AWARE, ScoreMinus10
    IfLoadedNotInTable Expert_BellyDrum_DesirableItems, Expert_BellyDrum_Item_TryScoreMinus1
    GoTo Expert_BellyDrum_StatusCheck

Expert_BellyDrum_Item_TryScoreMinus1:
    IfRandomLessThan 85, Expert_BellyDrum_StatusCheck
    AddToMoveScore -1
    GoTo Expert_BellyDrum_StatusCheck

Expert_BellyDrum_StatusCheck:
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_CONFUSION, Expert_BellyDrum_StatusTryScoreMinus1
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_ATTRACT, Expert_BellyDrum_StatusTryScoreMinus1
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_YAWN, Expert_BellyDrum_StatusTryScoreMinus1
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_BellyDrum_FieldCheck
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_CURSE, Expert_BellyDrum_StatusTryScoreMinus3
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED, Expert_BellyDrum_StatusTryScoreMinus1
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_HEAL, Expert_BellyDrum_FieldCheck
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, Expert_BellyDrum_StatusTryScoreMinus1
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Expert_BellyDrum_StatusTryScorePlus1
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_YAWN, Expert_BellyDrum_StatusTryScorePlus1
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_PARALYSIS, Expert_BellyDrum_StatusTryScorePlus1
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_AQUA_RING, Expert_BellyDrum_StatusTryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 7, Expert_BellyDrum_StatusTryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 7, Expert_BellyDrum_StatusTryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 7, Expert_BellyDrum_StatusTryScorePlus1
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_BellyDrum_FieldCheck
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LEECH_SEED, Expert_BellyDrum_StatusTryScorePlus1
    GoTo Expert_BellyDrum_FieldCheck

Expert_BellyDrum_StatusTryScoreMinus1:
    IfRandomLessThan 170, Expert_BellyDrum_FieldCheck
    AddToMoveScore -1
    GoTo Expert_BellyDrum_FieldCheck

Expert_BellyDrum_StatusTryScorePlus1:
    IfRandomLessThan 64, Expert_BellyDrum_FieldCheck
    AddToMoveScore 1
    GoTo Expert_BellyDrum_FieldCheck

Expert_BellyDrum_StatusTryScoreMinus3:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 76, Expert_BellyDrum_ScoreMinus12
    IfRandomLessThan 205, Expert_BellyDrum_FieldCheck
    AddToMoveScore -3
    GoTo Expert_BellyDrum_FieldCheck

Expert_BellyDrum_FieldCheck:
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_FUTURE_SIGHT, Expert_BellyDrum_TryScoreMinus1
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_REFLECT, Expert_BellyDrum_TryScorePlus1
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_LIGHT_SCREEN, Expert_BellyDrum_TryScorePlus1
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, Expert_BellyDrum_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_SET_SUBSTITUTE, Expert_BellyDrum_TryScoreMinus1
    GoTo Expert_BellyDrum_HPCheck

Expert_BellyDrum_TryScoreMinus1:
    IfRandomLessThan 170, Expert_BellyDrum_HPCheck
    AddToMoveScore -1
    GoTo Expert_BellyDrum_HPCheck
	
Expert_BellyDrum_TryScorePlus1:
    IfRandomLessThan 170, Expert_BellyDrum_HPCheck
    AddToMoveScore +1
    GoTo Expert_BellyDrum_HPCheck

Expert_BellyDrum_HPCheck:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 51, Expert_BellyDrum_ScoreMinus12HPCheck
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 76, Expert_BellyDrum_Item_ScorePlus1
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedEqualTo ITEM_SITRUS_BERRY, ScorePlus1
    IfRandomLessThan 128, ScorePlus1
    GoTo Expert_BellyDrum_End

Expert_BellyDrum_Item_ScorePlus1:
    LoadHeldItem AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_BellyDrum_DesirableItems, ScorePlus1
    GoTo Expert_BellyDrum_End

Expert_BellyDrum_ScoreMinus12:
    AddToMoveScore -12
	GoTo Expert_BellyDrum_FieldCheck
	
Expert_BellyDrum_ScoreMinus12HPCheck:
    AddToMoveScore -12
	GoTo Expert_BellyDrum_End

Expert_BellyDrum_DesirableItems:
    TableEntry ITEM_AGUAV_BERRY
    TableEntry ITEM_FIGY_BERRY
    TableEntry ITEM_WIKI_BERRY
    TableEntry ITEM_MAGO_BERRY
    TableEntry ITEM_IAPAPA_BERRY
    TableEntry ITEM_SITRUS_BERRY
    TableEntry ITEM_STARF_BERRY
    TableEntry ITEM_LIECHI_BERRY
    TableEntry ITEM_GANLON_BERRY
    TableEntry ITEM_SALAC_BERRY
    TableEntry ITEM_PETAYA_BERRY
    TableEntry ITEM_APICOT_BERRY
    TableEntry ITEM_LANSAT_BERRY
    TableEntry ITEM_CUSTAP_BERRY
    TableEntry ITEM_MICLE_BERRY
    TableEntry ITEM_ORAN_BERRY
    TableEntry ITEM_BERRY_JUICE
    TableEntry TABLE_END

Expert_BellyDrum_End:
    PopOrEnd 

Expert_PsychUp:
    ; If opponent has 3 > 2 > 1 or more stat stages to evasion or an 
    ; attacking stat than us, 90% > 75% > 33% chance for +2 > +1 > +1 score
    ; If opponent has 3 > 2 > 1 or more stat stages to a defense stat
    ; than us, 66% > 50% > 25% chance for +2 > +1 > +1 score
    ; for each stat, if there is no stat boost to gain, 1/6 chance for -1 score
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION
    IfLoadedGreaterThan 2, Expert_PsychUp_Evasion_HighDiff
    IfLoadedGreaterThan 1, Expert_PsychUp_Evasion_MediumDiff
    IfLoadedGreaterThan 0, Expert_PsychUp_Evasion_LowDiff
    IfRandomLessThan 213, Expert_PsychUp_CheckAttack
    AddToMoveScore -1
    GoTo Expert_PsychUp_CheckAttack

Expert_PsychUp_Evasion_HighDiff:
    IfRandomLessThan 25, Expert_PsychUp_CheckAttack
    AddToMoveScore 2
    GoTo Expert_PsychUp_CheckAttack

Expert_PsychUp_Evasion_MediumDiff:
    IfRandomLessThan 64, Expert_PsychUp_CheckAttack
    AddToMoveScore 1
    GoTo Expert_PsychUp_CheckAttack

Expert_PsychUp_Evasion_LowDiff:
    IfRandomLessThan 170, Expert_PsychUp_CheckAttack
    AddToMoveScore 1
    GoTo Expert_PsychUp_CheckAttack

Expert_PsychUp_CheckAttack:
    IfBattlerHasNoPhysicalAttack AI_BATTLER_ATTACKER, Expert_PsychUp_CheckSpecialAttack
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK
    IfLoadedGreaterThan 2, Expert_PsychUp_Attack_HighDiff
    IfLoadedGreaterThan 1, Expert_PsychUp_Attack_MediumDiff
    IfLoadedGreaterThan 0, Expert_PsychUp_Attack_LowDiff
    IfRandomLessThan 213, Expert_PsychUp_CheckSpecialAttack
    AddToMoveScore -1
    GoTo Expert_PsychUp_CheckSpecialAttack

Expert_PsychUp_Attack_HighDiff:
    IfRandomLessThan 25, Expert_PsychUp_CheckSpecialAttack
    AddToMoveScore 2
    GoTo Expert_PsychUp_CheckSpecialAttack

Expert_PsychUp_Attack_MediumDiff:
    IfRandomLessThan 64, Expert_PsychUp_CheckSpecialAttack
    AddToMoveScore 1
    GoTo Expert_PsychUp_CheckSpecialAttack

Expert_PsychUp_Attack_LowDiff:
    IfRandomLessThan 170, Expert_PsychUp_CheckSpecialAttack
    AddToMoveScore 1
    GoTo Expert_PsychUp_CheckSpecialAttack

Expert_PsychUp_CheckSpecialAttack:
    IfBattlerHasNoSpecialAttack AI_BATTLER_ATTACKER, Expert_PsychUp_CheckDefense
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK
    IfLoadedGreaterThan 2, Expert_PsychUp_Special_AttackHighDiff
    IfLoadedGreaterThan 1, Expert_PsychUp_Special_AttackMediumDiff
    IfLoadedGreaterThan 0, Expert_PsychUp_Special_AttackLowDiff
    IfRandomLessThan 213, Expert_PsychUp_CheckDefense
    AddToMoveScore -1
    GoTo Expert_PsychUp_CheckDefense

Expert_PsychUp_Special_AttackHighDiff:
    IfRandomLessThan 25, Expert_PsychUp_CheckDefense
    AddToMoveScore 2
    GoTo Expert_PsychUp_CheckDefense

Expert_PsychUp_Special_AttackMediumDiff:
    IfRandomLessThan 64, Expert_PsychUp_CheckDefense
    AddToMoveScore 1
    GoTo Expert_PsychUp_CheckDefense

Expert_PsychUp_Special_AttackLowDiff:
    IfRandomLessThan 170, Expert_PsychUp_CheckDefense
    AddToMoveScore 1
    GoTo Expert_PsychUp_CheckDefense

Expert_PsychUp_CheckDefense:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE
    IfLoadedGreaterThan 2, Expert_PsychUp_DefenseHighDiff
    IfLoadedGreaterThan 1, Expert_PsychUp_DefenseMediumDiff
    IfLoadedGreaterThan 0, Expert_PsychUp_DefenseLowDiff
    IfRandomLessThan 213, Expert_PsychUp_CheckSpecialDefense
    AddToMoveScore -1
    GoTo Expert_PsychUp_CheckSpecialDefense

Expert_PsychUp_DefenseHighDiff:
    IfRandomLessThan 85, Expert_PsychUp_CheckSpecialDefense
    AddToMoveScore 2
    GoTo Expert_PsychUp_CheckSpecialDefense

Expert_PsychUp_DefenseMediumDiff:
    IfRandomLessThan 128, Expert_PsychUp_CheckSpecialDefense
    AddToMoveScore 1
    GoTo Expert_PsychUp_CheckSpecialDefense

Expert_PsychUp_DefenseLowDiff:
    IfRandomLessThan 192, Expert_PsychUp_CheckSpecialDefense
    AddToMoveScore 1
    GoTo Expert_PsychUp_CheckSpecialDefense

Expert_PsychUp_CheckSpecialDefense:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE
    IfLoadedGreaterThan 2, Expert_PsychUp_SpecialDefenseHighDiff
    IfLoadedGreaterThan 1, Expert_PsychUp_SpecialDefenseMediumDiff
    IfLoadedGreaterThan 0, Expert_PsychUp_SpecialDefenseLowDiff
    IfRandomLessThan 213, Expert_PsychUp_End
    AddToMoveScore -1
    GoTo Expert_PsychUp_End

Expert_PsychUp_SpecialDefenseHighDiff:
    IfRandomLessThan 85, Expert_PsychUp_End
    AddToMoveScore 2
    GoTo Expert_PsychUp_End

Expert_PsychUp_SpecialDefenseMediumDiff:
    IfRandomLessThan 128, Expert_PsychUp_End
    AddToMoveScore 1
    GoTo Expert_PsychUp_End

Expert_PsychUp_SpecialDefenseLowDiff:
    IfRandomLessThan 192, Expert_PsychUp_End
    AddToMoveScore 1
    GoTo Expert_PsychUp_End

Expert_PsychUp_End:
    PopOrEnd 

Expert_MirrorCoat:
    ; If behind substitute, score -10.
    ;
    ; If defender has no special attacks whatsoever, score -30.
    ;
    ; If defender knows Disable, Torment, or Encore, score -2 to -4.
    ;
    ; Score +1 if the defender is a special attacker.
    ;
    ; 75% chance to predict defender wake up turn and get score +1.
    ; Otherwise, score -3 if the defender is asleep.
    ;
    ; If defender has a status or physical move, check if we have
    ; used Mirror Coat yet. If not, 66% chance for score +1.
    ;
    ; If defender has status move, check if defender is taunted.
    ; If defender is taunted, score +1. Otherwise, 25% chance for
    ; score -1.
    ;
    ; If enemy chunks or KOs us, check if we have more than 56%
    ; HP remaining. If not, 87.5% chance for score -2.
    IfBattlerHasNoSpecialAttack AI_BATTLER_DEFENDER, ScoreMinus30
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, ScoreMinus10
    IfBattlerIsSpecialAttacker AI_BATTLER_DEFENDER, Expert_MirrorCoat_Main
    IfRandomLessThan 8, Expert_MirrorCoat_Main
    GoTo ScoreMinus3

Expert_MirrorCoat_Main:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DISABLE, Expert_MirrorCoat_TryScoreMinus4
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TORMENT, Expert_MirrorCoat_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ENCORE, Expert_MirrorCoat_TryScoreMinus4
    AddToMoveScore 1
    IfEnemyCanChunkOrKO Expert_MirrorCoat_CheckHP
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Expert_MirrorCoat_CheckSleepTurns
    IfBattlerHasStatusMove AI_BATTLER_DEFENDER, Expert_MirrorCoat_HasStatus
    IfBattlerIsPhysicalAttacker AI_BATTLER_DEFENDER, Expert_MirrorCoat_CheckPP
    IfRandomLessThan 64, Expert_MirrorCoat_End
    AddToMoveScore 1
    GoTo Expert_MirrorCoat_End

Expert_MirrorCoat_CheckHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 56, Expert_MirrorCoat_TryScoreMinus4
    IfRandomLessThan 32, Expert_MirrorCoat_End
    AddToMoveScore -2
    GoTo Expert_MirrorCoat_End

Expert_MirrorCoat_HasStatus:
    AddToMoveScore 1
    IfTargetIsTaunted Expert_MirrorCoat_CheckPP
    AddToMoveScore -1
    IfRandomLessThan 192, Expert_MirrorCoat_CheckPP
    AddToMoveScore -1
    GoTo Expert_MirrorCoat_CheckPP

Expert_MirrorCoat_CheckSleepTurns:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP, Expert_MirrorCoat_SleepTalk
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DAMAGE_WHILE_ASLEEP, ScorePlus1
    LoadSleepTurns AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, ScoreMinus3
    IfRandomLessThan 64, ScoreMinus3
    AddToMoveScore 1
    IfRandomLessThan 64, ScoreMinus1
    AddToMoveScore 1
    GoTo Expert_MirrorCoat_End

Expert_MirrorCoat_CheckPP:
    IfCurrentMoveRevealed Expert_MirrorCoat_TryScoreMinus2
    IfRandomLessThan 85, Expert_MirrorCoat_End
    AddToMoveScore 1
    GoTo Expert_MirrorCoat_End

Expert_MirrorCoat_SleepTalk:
    IfTargetIsTaunted ScoreMinus30
    IfRandomLessThan 25, Expert_MirrorCoat_End
    AddToMoveScore 1
    GoTo Expert_MirrorCoat_End

Expert_MirrorCoat_TryScoreMinus4:
    IfRandomLessThan 25, Expert_MirrorCoat_End
    AddToMoveScore -4
    GoTo Expert_MirrorCoat_End

Expert_MirrorCoat_TryScoreMinus2:
    IfRandomLessThan 128, Expert_MirrorCoat_End
    AddToMoveScore -2
    GoTo Expert_MirrorCoat_End

Expert_MirrorCoat_End:
    PopOrEnd

Expert_ChargeTurnNoInvuln:
    ; If the opponent resists or is immune to the move, score -2 and terminate.
    ;
    ; If the move would skip its charge turn in Sun and the current weather is Sun, score +2.
    ;
    ; If the attacker is holding a Power Herb, score +2.
    ;
    ; If the opponent knows the move Protect, score -2.
    ;
    ; If the attacker''s HP <= 38%, score -1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_ChargeTurnNoInvuln_ScoreMinus2
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_ChargeTurnNoInvuln_ScoreMinus2
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_ChargeTurnNoInvuln_ScoreMinus2
    GoTo Expert_ChargeTurnNoInvuln_CheckForPowerHerb

Expert_ChargeTurnNoInvuln_CheckForPowerHerb:
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_POWER_HERB, Expert_ChargeTurnNoInvuln_ScorePlus2
    GoTo Expert_ChargeTurnNoInvuln_CheckForProtectAndHP

Expert_ChargeTurnNoInvuln_ScorePlus2:
    AddToMoveScore 2
    GoTo Expert_ChargeTurnNoInvuln_End

Expert_ChargeTurnNoInvuln_CheckForProtectAndHP:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_PROTECT, Expert_ChargeTurnNoInvuln_ScoreMinus2
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 38, Expert_ChargeTurnNoInvuln_End
    AddToMoveScore -1
    GoTo Expert_ChargeTurnNoInvuln_End

Expert_ChargeTurnNoInvuln_ScoreMinus2:
    AddToMoveScore -2

Expert_ChargeTurnNoInvuln_End:
    PopOrEnd 

Expert_SolarBeam:
    ; If move is resisted or immune, score -1 to -10.
    ; 
    ; 25% chance for score +1 if move is neutral or better.
    ;
    ; If using solar beam to boost is feasible, score +1.
    ; Otherwise, score -3.
    ;
    ; If using solar beam as a coverage move for cheesing, score -3
    ; if the move is not super effective.
    ;
    ; If solar beam goes without charging, 50% chance for score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_SolarBeam_ScoreMinus3
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_SolarBeam_ScoreMinus2
    IfRandomLessThan 192, Expert_SolarBeam_CheckBoost
    AddToMoveScore 1
    GoTo Expert_SolarBeam_CheckBoost

Expert_SolarBeam_ScoreMinus3:
    AddToMoveScore -1
    IfRandomLessThan 12, Expert_SolarBeam_CheckBoost
    AddToMoveScore -1
    IfRandomLessThan 64, Expert_SolarBeam_CheckBoost
    AddToMoveScore -1
    GoTo Expert_SolarBeam_CheckBoost

Expert_SolarBeam_ScoreMinus2:
    AddToMoveScore -1
    IfRandomLessThan 12, Expert_SolarBeam_CheckBoost
    AddToMoveScore -1
    GoTo Expert_SolarBeam_CheckBoost

Expert_SolarBeam_CheckBoost:
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo HOLD_EFFECT_CHARGE_SKIP, Expert_SolarBeam_YoloCoverMove
    LoadCurrentWeather
    IfLoadedEqualTo AI_WEATHER_SUNNY, Expert_SolarBeam_NoBoost
    IfCanHazeOrPhaze AI_BATTLER_DEFENDER, Expert_SolarBeam_DeincentivizeBoost
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 7, Expert_SolarBeam_DeincentivizeBoost
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_UNAWARE, Expert_SolarBeam_DeincentivizeBoost
    IfRandomLessThan 12, Expert_SolarBeam_End
    AddToMoveScore 1
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SIMPLE, ScorePlus2
    GoTo Expert_SolarBeam_End

Expert_SolarBeam_YoloCoverMove:
    IfMoveEffectivenessEquals TYPE_MULTI_BASE_DAMAGE, ScoreMinus3
    IfMoveEffectivenessEquals TYPE_MULTI_STAB_DAMAGE, ScoreMinus3
    AddToMoveScore 1
    GoTo Expert_SolarBeam_End

Expert_SolarBeam_NoBoost:
    IfRandomLessThan 128, Expert_SolarBeam_End
    AddToMoveScore 1
    GoTo Expert_SolarBeam_End

Expert_SolarBeam_DeincentivizeBoost:
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NOT_HIGHEST_DAMAGE, ScoreMinus10
    IfRandomLessThan 12, Expert_SolarBeam_End
    AddToMoveScore -3
    GoTo Expert_SolarBeam_End

Expert_SolarBeam_End:
    PopOrEnd

Expert_ChargeTurnWithInvuln:
    ; If the attacker is holding a Power Herb, score +2.
    ;
    ; If the opponent knows a Protect move, score -1.
    ;
    ; If the opponent is immune to or would resist the move, score +1. (Bug?)
    ;
    ; If the opponent is under any of the following conditions, score +1:
    ; - Toxic
    ; - Curse
    ; - Leech Seed
    ;
    ; If the current weather is Sand or Hail and the attacker''s type makes them immune to the
    ; corresponding damage effect, 68.75% chance of score +1.
    ;
    ; If the attacker is faster than its opponent and the opponent''s last-used move is not an
    ; always-hit effect (e.g. Aerial Ace), 68.75% chance of score +1.
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_POWER_HERB, Expert_ChargeTurnNoInvuln_ScorePlus2
    IfMoveEffectNotKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_PROTECT, Expert_ShadowForce
    AddToMoveScore -1
    GoTo Expert_ChargeTurnWithInvuln_End

Expert_ShadowForce:
    ; Shadow Force is handled identically to ChargeTurnWithInvuln, but only gets score +1 for Power Herb
    ; and does not consider if the opponent knows a Protect move (which it would bypass).
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_ChargeTurnWithInvuln_ScorePlus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_ChargeTurnWithInvuln_ScorePlus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_ChargeTurnWithInvuln_ScorePlus1
    IfHeldItemEqualTo AI_BATTLER_ATTACKER, ITEM_POWER_HERB, Expert_ChargeTurnWithInvuln_ScorePlus1AndEnd
    GoTo Expert_ChargeTurnWithInvuln_CheckConditions

Expert_ChargeTurnWithInvuln_ScorePlus1AndEnd:
    AddToMoveScore 1
    GoTo Expert_ChargeTurnWithInvuln_End

Expert_ChargeTurnWithInvuln_CheckConditions:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY_POISON, Expert_ChargeTurnWithInvuln_TryScoreMinus1
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CURSE, Expert_ChargeTurnWithInvuln_TryScoreMinus1
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_LEECH_SEED, Expert_ChargeTurnWithInvuln_TryScoreMinus1
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SANDSTORM, Expert_ChargeTurnWithInvuln_CheckSandImmuneType
    IfLoadedEqualTo AI_WEATHER_HAILING, Expert_ChargeTurnWithInvuln_CheckHailImmuneType
    GoTo Expert_ChargeTurnWithInvuln_CompareSpeed

Expert_ChargeTurnWithInvuln_CheckSandImmuneType:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedInTable Expert_ChargeTurnWithInvuln_SandImmuneTypes, Expert_ChargeTurnWithInvuln_TryScorePlus1
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedInTable Expert_ChargeTurnWithInvuln_SandImmuneTypes, Expert_ChargeTurnWithInvuln_TryScorePlus1
    GoTo Expert_ChargeTurnWithInvuln_CompareSpeed

Expert_ChargeTurnWithInvuln_CheckHailImmuneType:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_ICE_BODY, Expert_ChargeTurnWithInvuln_CompareSpeed
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_ChargeTurnWithInvuln_CompareSpeed
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_ICE, Expert_ChargeTurnWithInvuln_TryScorePlus1
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_ICE, Expert_ChargeTurnWithInvuln_TryScorePlus1
    GoTo Expert_ChargeTurnWithInvuln_CompareSpeed

Expert_ChargeTurnWithInvuln_CompareSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_ChargeTurnWithInvuln_End
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadEffectOfLoadedMove 
    IfLoadedNotEqualTo BATTLE_EFFECT_NEXT_ATTACK_ALWAYS_HITS, Expert_ChargeTurnWithInvuln_TryScorePlus1
    GoTo Expert_ChargeTurnWithInvuln_End

Expert_ChargeTurnWithInvuln_TryScoreMinus1:
    IfRandomLessThan 12, Expert_ChargeTurnWithInvuln_End
    AddToMoveScore -1
    GoTo Expert_ChargeTurnWithInvuln_End

Expert_ChargeTurnWithInvuln_TryScorePlus1:
    IfRandomLessThan 80, Expert_ChargeTurnWithInvuln_End
    AddToMoveScore 1

Expert_ChargeTurnWithInvuln_End:
    PopOrEnd 

Expert_ChargeTurnWithInvuln_ScorePlus1:
    AddToMoveScore 1
    PopOrEnd 

Expert_ChargeTurnWithInvuln_SandImmuneTypes:
    TableEntry TYPE_GROUND
    TableEntry TYPE_ROCK
    TableEntry TYPE_STEEL
    TableEntry TABLE_END

Expert_FakeOut:
    ; Score +5.
    ; 
    ; Deprioritize if not first turn out
    ;
    ; Deprioritize if negative contact ability
    ; Unless attacker is holding protective pads
    ; or unless Fake Out can kill anyway

    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo FALSE, ScoreMinus10
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedInTable FakeOut_AbilityPunish, Expert_FakeOut_WillItKill
    AddToMoveScore 5
    GoTo Expert_FakeOut_End

Expert_FakeOut_WillItKill:
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH, ScorePlus5
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_FakeOut_End
    IfCurrentMoveKills ROLL_FOR_DAMAGE, Expert_FakeOut_TryScorePlus3
    GoTo ScoreMinus1

Expert_FakeOut_TryScorePlus3:
    IfRandomLessThan 128, ScorePlus3
    GoTo Expert_FakeOut_End
    
FakeOut_AbilityPunish:
    TableEntry ABILITY_EFFECT_SPORE
    TableEntry ABILITY_ROUGH_SKIN
    TableEntry ABILITY_STATIC
    TableEntry ABILITY_POISON_POINT
    TableEntry ABILITY_FLAME_BODY
    TableEntry ABILITY_FRESH_MILK
    TableEntry ABILITY_INNER_FOCUS
    TableEntry ABILITY_STEADFAST
	TableEntry TABLE_END

Expert_FakeOut_End:
    PopOrEnd 

Expert_SpitUp:
    ; If the attacker''s Stockpile count is 2 or higher, 68.75% chance of score +2.
    LoadStockpileCount AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus10
    IfLoadedLessThan 2, Expert_SpitUp_End
    IfRandomLessThan 80, Expert_SpitUp_End
    AddToMoveScore 2

Expert_SpitUp_End:
    PopOrEnd 

Expert_Hail:
    ;
    ; If the current weather is Sun, Rain, or Sand, additional score +1. If the attacker also knows
    ; the move Blizzard, additional score +2.
    ;
    ; If the attacker has the ability Ice Body, additional score +2.
    LoadCurrentWeather 
    IfLoadedNotEqualTo AI_WEATHER_HAILING, Expert_Hail_TryScorePlus5
    GoTo Expert_Hail_End

Expert_Hail_TryScorePlus5:
    AddToMoveScore 2
    IfRandomLessThan 64, Expert_Hail_CheckBlizzard
    AddToMoveScore 3
    GoTo Expert_Hail_CheckBlizzard

Expert_Hail_CheckBlizzard:
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_BLIZZARD, ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_SHEER_COLD, ScorePlus3
    GoTo Expert_Hail_CheckIceBody

Expert_Hail_CheckIceBody:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo ABILITY_ICE_BODY, Expert_Hail_End
    AddToMoveScore 2
    GoTo Expert_Hail_End

Expert_Hail_End:
    PopOrEnd 

Expert_Facade:
    ; If the attacker has a status condition which would boost Facade, score +2.
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_FACADE_BOOST, Expert_Facade_End
    AddToMoveScore 2

Expert_Facade_End:
    PopOrEnd 

Expert_FocusPunch:
    ; If the opponent is immune to or would resist the move, score -1.
    ;
    ; If the attacker is behind a Substitute, score +5.
    ;
    ; If the opponent is asleep, score +1.
    ;
    ; If the opponent is confused or infatuated, 60.9% chance of score +1.
    ;
    ; If it is not the attacker''s first turn in battle, 21.875% chance of score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_FocusPunch_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_FocusPunch_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_FocusPunch_ScoreMinus1
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, ScorePlus3
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Expert_FocusPunch_ScorePlus2
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, Expert_FocusPunch_TryScorePlus1
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CONFUSION, Expert_FocusPunch_TryScorePlus1
	
	IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 8, Expert_FocusPunch_ScorePlus2
	IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 8, Expert_FocusPunch_ScorePlus2
	
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo FALSE, Expert_FocusPunch_End
    IfRandomLessThan 200, Expert_FocusPunch_End
    AddToMoveScore 1
    GoTo Expert_FocusPunch_End

Expert_FocusPunch_ScoreMinus1:
    AddToMoveScore -1
    GoTo Expert_FocusPunch_End

Expert_FocusPunch_TryScorePlus1:
    IfRandomLessThan 100, Expert_FocusPunch_End

Expert_FocusPunch_ScorePlus1:
    AddToMoveScore 1
	
Expert_FocusPunch_ScorePlus2:
    AddToMoveScore 2

Expert_FocusPunch_End:
    PopOrEnd 

Expert_SmellingSalts:
    ; If the opponent is paralyzed, score +1.
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_PARALYSIS, Expert_SmellingSalts_ScorePlus1
    GoTo Expert_SmellingSalts_End

Expert_SmellingSalts_ScorePlus1:
    AddToMoveScore 1

Expert_SmellingSalts_End:
    PopOrEnd 

Expert_Trick:
    ; If the attacker is holding a Disruptive item:
    ; - If the opponent is holding a bad item to trade with, score -3.
    ; - Otherwise, score +5.
    ;
    ; If the attacker is holding an item that poisons its bearer:
    ; - If the opponent is holding a bad item to trade with, score -3.
    ; - If the opponent does not meet any of the following criteria, score +5:
    ;   - Has a non-volatile status condition.
    ;   - Is protected by Safeguard.
    ;   - Has a Steel or Poison typing.
    ;   - Has the ability Immunity, Magic Guard, or Poison Heal.
    ; - If the attacker meets any of the following criteria, score -3:
    ;   - Has a non-volatile status condition.
    ;   - Is protected by Safeguard.
    ;   - Has a Steel or Poison typing.
    ;   - Has the ability Immunity, Magic Guard, Poison Heal, or Klutz.
    ; - Otherwise, score +5.
    ;
    ; If the attacker is holding an item that burns its bearer:
    ; - If the opponent is holding a bad item to trade with, score -3.
    ; - If the opponent does not meet any of the following criteria, score +5:
    ;   - Has a non-volatile status condition.
    ;   - Is protected by Safeguard.
    ;   - Has a Fire typing.
    ;   - Has the ability Water Veil or Magic Guard.
    ; - If the attacker meets any of the following criteria, score -3:
    ;   - Has a non-volatile status condition.
    ;   - Is protected by Safeguard.
    ;   - Has a Fire typing.
    ;   - Has the ability Water Veil, Magic Guard, or Klutz.
    ; - Otherwise, score +5.
    ;
    ; If the attacker is holding Black Sludge:
    ; - If the opponent is holding a bad item to trade with, score -3.
    ; - If the opponent does not meet any of the following criteria, score +5:
    ;   - Has a Poison typing.
    ;   - Has the ability Magic Guard.
    ; - If the attacker meets any of the following criteria, score -3:
    ;   - Has a Poison typing.
    ;   - Has the ability Magic Guard or Klutz.
    ; - Otherwise, score +5.
    ;
    ; If the attacker is holding a Flavor Berry:
    ; - If the opponent is holding a bad item to trade with or a flavor berry, score -3.
    ; - Otherwise, 80.5% chance of score +2.
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_Trick_DisruptiveItems, Expert_Trick_CheckOpponentItem
    IfLoadedInTable Expert_Trick_PoisoningItems, Expert_Trick_CheckOpponentForPoison
    IfLoadedInTable Expert_Trick_BurningItems, Expert_Trick_CheckOpponentForBurn
    IfLoadedInTable Expert_Trick_BlackSludge, Expert_Trick_CheckOpponentForSludge
    IfLoadedInTable Expert_Trick_FlavorBerries, Expert_Trick_CheckOpponentForFlavorBerry

Expert_Trick_ScoreMinus3:
    AddToMoveScore -3
    GoTo Expert_Trick_End

Expert_Trick_CheckOpponentItem:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Trick_BadOpponentItems, Expert_Trick_ScoreMinus3
    AddToMoveScore 5
    GoTo Expert_Trick_End

Expert_Trick_CheckOpponentForPoison:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Trick_BadOpponentItems, Expert_Trick_ScoreMinus3
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, Expert_Trick_CheckAttackerForPoison
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, Expert_Trick_CheckAttackerForPoison
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, Expert_Trick_CheckAttackerForPoison
    IfLoadedEqualTo TYPE_POISON, Expert_Trick_CheckAttackerForPoison
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_STEEL, Expert_Trick_CheckAttackerForPoison
    IfLoadedEqualTo TYPE_POISON, Expert_Trick_CheckAttackerForPoison
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_IMMUNITY, Expert_Trick_CheckAttackerForPoison
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Trick_CheckAttackerForPoison
    IfLoadedEqualTo ABILITY_POISON_HEAL, Expert_Trick_CheckAttackerForPoison
    AddToMoveScore 5
    GoTo Expert_Trick_End

Expert_Trick_CheckAttackerForPoison:
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_Trick_ScoreMinus3
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_SAFEGUARD, Expert_Trick_ScoreMinus3
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, Expert_Trick_ScoreMinus3
    IfLoadedEqualTo TYPE_POISON, Expert_Trick_ScoreMinus3
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_STEEL, Expert_Trick_ScoreMinus3
    IfLoadedEqualTo TYPE_POISON, Expert_Trick_ScoreMinus3
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_IMMUNITY, Expert_Trick_ScoreMinus3
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Trick_ScoreMinus3
    IfLoadedEqualTo ABILITY_POISON_HEAL, Expert_Trick_ScoreMinus3
    AddToMoveScore 5
    GoTo Expert_Trick_End

Expert_Trick_CheckOpponentForBurn:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Trick_BadOpponentItems, Expert_Trick_ScoreMinus3
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_WATER_VEIL, Expert_Trick_CheckAttackerForBurn
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Trick_CheckAttackerForBurn
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, Expert_Trick_CheckAttackerForBurn
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, Expert_Trick_CheckAttackerForBurn
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, Expert_Trick_CheckAttackerForBurn
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, Expert_Trick_CheckAttackerForBurn
    AddToMoveScore 5
    GoTo Expert_Trick_End

Expert_Trick_CheckAttackerForBurn:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_WATER_VEIL, Expert_Trick_ScoreMinus3
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Trick_ScoreMinus3
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, Expert_Trick_ScoreMinus3
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_SAFEGUARD, Expert_Trick_ScoreMinus3
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, Expert_Trick_ScoreMinus3
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, Expert_Trick_ScoreMinus3
    AddToMoveScore 5
    GoTo Expert_Trick_End

Expert_Trick_CheckOpponentForSludge:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Trick_BadOpponentItems, Expert_Trick_ScoreMinus3
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_POISON, Expert_Trick_CheckAttackerForSludge
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_POISON, Expert_Trick_CheckAttackerForSludge
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Trick_CheckAttackerForPoison
    AddToMoveScore 5
    GoTo Expert_Trick_End

Expert_Trick_CheckAttackerForSludge:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_POISON, Expert_Trick_ScoreMinus3
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_POISON, Expert_Trick_ScoreMinus3
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_Trick_ScoreMinus3
    AddToMoveScore 5
    GoTo Expert_Trick_End

Expert_Trick_CheckOpponentForFlavorBerry:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Trick_BadOpponentItemsAndFlavorBerries, Expert_Trick_ScoreMinus3
    IfRandomLessThan 50, Expert_Trick_End
    AddToMoveScore 2

Expert_Trick_End:
    PopOrEnd 

Expert_Trick_FlavorBerries:
    TableEntry HOLD_EFFECT_HP_RESTORE_SPICY
    TableEntry HOLD_EFFECT_HP_RESTORE_DRY
    TableEntry HOLD_EFFECT_HP_RESTORE_SWEET
    TableEntry HOLD_EFFECT_HP_RESTORE_BITTER
    TableEntry HOLD_EFFECT_HP_RESTORE_SOUR
    TableEntry TABLE_END

Expert_Trick_DisruptiveItems:
    TableEntry HOLD_EFFECT_CHOICE_ATK
    TableEntry HOLD_EFFECT_CHOICE_SPATK
    TableEntry HOLD_EFFECT_CHOICE_SPEED
    TableEntry HOLD_EFFECT_SPEED_DOWN_GROUNDED
    TableEntry HOLD_EFFECT_PRIORITY_DOWN
    TableEntry HOLD_EFFECT_DMG_USER_CONTACT_XFR
    TableEntry HOLD_EFFECT_LVLUP_ATK_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_DEF_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPATK_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_DEF_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPDEF_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPEED_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_HP_EV_UP
	TableEntry HOLD_EFFECT_EVS_UP_SPEED_DOWN
    TableEntry TABLE_END

Expert_Trick_PoisoningItems:
    TableEntry HOLD_EFFECT_PSN_USER
    TableEntry TABLE_END

Expert_Trick_BurningItems:
    TableEntry HOLD_EFFECT_BRN_USER
    TableEntry TABLE_END

Expert_Trick_BlackSludge:
    TableEntry HOLD_EFFECT_HP_RESTORE_PSN_TYPE
    TableEntry TABLE_END

Expert_Trick_BadOpponentItemsAndFlavorBerries:
    TableEntry HOLD_EFFECT_HP_RESTORE_SPICY
    TableEntry HOLD_EFFECT_HP_RESTORE_DRY
    TableEntry HOLD_EFFECT_HP_RESTORE_SWEET
    TableEntry HOLD_EFFECT_HP_RESTORE_BITTER
    TableEntry HOLD_EFFECT_HP_RESTORE_SOUR
    TableEntry HOLD_EFFECT_EVS_UP_SPEED_DOWN
    TableEntry HOLD_EFFECT_CHOICE_ATK
    TableEntry HOLD_EFFECT_CHOICE_SPATK
    TableEntry HOLD_EFFECT_CHOICE_SPEED
    TableEntry HOLD_EFFECT_SPEED_DOWN_GROUNDED
    TableEntry HOLD_EFFECT_PRIORITY_DOWN
    TableEntry HOLD_EFFECT_DMG_USER_CONTACT_XFR
    TableEntry HOLD_EFFECT_LVLUP_ATK_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_DEF_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPATK_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPDEF_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPEED_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_HP_EV_UP
    TableEntry HOLD_EFFECT_PSN_USER
    TableEntry HOLD_EFFECT_BRN_USER
    TableEntry HOLD_EFFECT_HP_RESTORE_PSN_TYPE
    TableEntry TABLE_END

Expert_Trick_BadOpponentItems:
    TableEntry HOLD_EFFECT_EVS_UP_SPEED_DOWN
    TableEntry HOLD_EFFECT_CHOICE_ATK
    TableEntry HOLD_EFFECT_CHOICE_SPATK
    TableEntry HOLD_EFFECT_CHOICE_SPEED
    TableEntry HOLD_EFFECT_SPEED_DOWN_GROUNDED
    TableEntry HOLD_EFFECT_PRIORITY_DOWN
    TableEntry HOLD_EFFECT_DMG_USER_CONTACT_XFR
    TableEntry HOLD_EFFECT_LVLUP_ATK_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_DEF_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPATK_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPDEF_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_SPEED_EV_UP
    TableEntry HOLD_EFFECT_LVLUP_HP_EV_UP
    TableEntry HOLD_EFFECT_PSN_USER
    TableEntry HOLD_EFFECT_BRN_USER
    TableEntry HOLD_EFFECT_HP_RESTORE_PSN_TYPE
    TableEntry TABLE_END

Expert_ChangeUserAbility:
    ; If the attacker has a desirable ability, score -1.
    ;
    ; If the opponent has a desirable ability, 80.5% chance of score +2.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_ChangeUserAbility_DesirableAbilities, Expert_ChangeUserAbility_ScoreMinus1
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_ChangeUserAbility_DesirableAbilities, Expert_ChangeUserAbility_TryScorePlus2

Expert_ChangeUserAbility_ScoreMinus1:
    AddToMoveScore -1
    GoTo Expert_ChangeUserAbility_End

Expert_ChangeUserAbility_TryScorePlus2:
    IfRandomLessThan 50, Expert_ChangeUserAbility_End
    AddToMoveScore 2

Expert_ChangeUserAbility_End:
    PopOrEnd 

Expert_ChangeUserAbility_DesirableAbilities:
    TableEntry ABILITY_SPEED_BOOST
    TableEntry ABILITY_SAND_VEIL
    TableEntry ABILITY_STATIC
    TableEntry ABILITY_FLASH_FIRE
    TableEntry ABILITY_WONDER_GUARD
    TableEntry ABILITY_EFFECT_SPORE
    TableEntry ABILITY_SWIFT_SWIM
    TableEntry ABILITY_HUGE_POWER
    TableEntry ABILITY_RAIN_DISH
    TableEntry ABILITY_SHED_SKIN
    TableEntry ABILITY_MARVEL_SCALE
    TableEntry ABILITY_PURE_POWER
    TableEntry ABILITY_CHLOROPHYLL
    TableEntry ABILITY_SHIELD_DUST
    TableEntry ABILITY_ADAPTABILITY
    TableEntry ABILITY_MAGIC_GUARD
    TableEntry ABILITY_MOLD_BREAKER
    TableEntry ABILITY_UNAWARE
    TableEntry ABILITY_TINTED_LENS
    TableEntry ABILITY_FILTER
    TableEntry ABILITY_RELENTLESS
    TableEntry ABILITY_RECKLESS
    TableEntry TABLE_END

Expert_SkillSwap:
    ; Don''t cast if we have same ability as defender.
    ; Don''t cast twice in a row.
    ; Try to cast into Encore users only if we are faster
    ;
    ; Plus 1 or 2 points for undesirable abilities on attacker
    ; Plus 4 or 5 points for abilities we always want to get rid of on attacker
    ; Plus 10 points if attacker has Truant, Slow Start, or Normalize
    ;
    ; Plus 1 or 2 points for desirable abilities on defender
    ; Plus 4 or 5 points for abilities we always want to take away from defender
    ; 95% chance to cancel above bonus if attacker''s ability is in the same tier as defender
    ;
    ; Highly discourage swapping with a defender that has a bad or negative ability
    IfSameAbilities AI_BATTLER_ATTACKER, AI_BATTLER_DEFENDER, ScoreMinus20
    LoadBattlerPreviousMove AI_BATTLER_ATTACKER
    IfLoadedEqualTo MOVE_SKILL_SWAP, ScoreMinus20
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ENCORE, Expert_SkillSwap_CheckSpeed
    GoTo Expert_SkillSwap_CheckDefenderHasBaseAbility

Expert_SkillSwap_CheckSpeed:
    IfSpeedCompareNotEqualTo COMPARE_SPEED_SLOWER, Expert_SkillSwap_CheckDefenderHasBaseAbility
    IfRandomLessThan 192, Expert_SkillSwap_CheckDefenderHasBaseAbility
    GoTo ScoreMinus12

Expert_SkillSwap_CheckDefenderHasBaseAbility:
    IfHasBaseAbility AI_BATTLER_DEFENDER, Expert_SkillSwap_DefenderBaseAbilityScoreBoost
    AddToMoveScore -1
    IfRandomLessThan 230, Expert_SkillSwap_CheckAttackerHasBaseAbility
    AddToMoveScore -1
    GoTo Expert_SkillSwap_CheckAttackerHasBaseAbility

Expert_SkillSwap_DefenderBaseAbilityScoreBoost:
    AddToMoveScore 1
    GoTo Expert_SkillSwap_CheckAttackerHasBaseAbility

Expert_SkillSwap_CheckAttackerHasBaseAbility:
    IfHasBaseAbility AI_BATTLER_ATTACKER, Expert_SkillSwap_AttackerBaseAbilityScoreBoost
    IfRandomLessThan 128, Expert_SkillSwap_CheckAttackerAbility
    AddToMoveScore -1
    GoTo Expert_SkillSwap_CheckAttackerAbility

Expert_SkillSwap_AttackerBaseAbilityScoreBoost:
    AddToMoveScore 2
    GoTo Expert_SkillSwap_CheckAttackerAbility

Expert_SkillSwap_CheckAttackerAbility:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_SkillSwap_ForbiddenDefenderAbilities, ScorePlus10
    IfLoadedInTable Expert_SkillSwap_AlwaysGiveAbilities, Expert_SkillSwap_AttackerAbilityBigScoreBoost
    IfLoadedInTable Expert_SkillSwap_UndesirableAbilities, Expert_SkillSwap_AttackerAbilityScoreBoost
    IfLoadedInTable Expert_SkillSwap_AlwaysTakeAbilities, Expert_SkillSwap_CheckMajorScoreCancel
    IfLoadedInTable Expert_SkillSwap_DesirableAbilities, Expert_SkillSwap_CheckMinorScoreCancel
    GoTo Expert_SkillSwap_CheckDefenderAbility

Expert_SkillSwap_AttackerAbilityScoreBoost:
    AddToMoveScore 1
    IfRandomLessThan 230, Expert_SkillSwap_CheckDefenderAbility
    AddToMoveScore 1
    GoTo Expert_SkillSwap_CheckDefenderAbility

Expert_SkillSwap_AttackerAbilityBigScoreBoost:
    AddToMoveScore 4
    IfRandomLessThan 230, Expert_SkillSwap_CheckDefenderAbility
    AddToMoveScore 1
    GoTo Expert_SkillSwap_CheckDefenderAbility

Expert_SkillSwap_CheckMinorScoreCancel:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotInTable Expert_SkillSwap_DesirableAbilities, Expert_SkillSwap_CheckDefenderAbility
    IfRandomLessThan 243, ScoreMinus20
    GoTo Expert_SkillSwap_End
    
Expert_SkillSwap_CheckMajorScoreCancel:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotInTable Expert_SkillSwap_AlwaysTakeAbilities, Expert_SkillSwap_CheckDefenderAbility
    IfRandomLessThan 243, ScoreMinus20
    GoTo Expert_SkillSwap_End

Expert_SkillSwap_CheckDefenderAbility:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_SkillSwap_ForbiddenDefenderAbilities, ScoreMinus30
    IfLoadedInTable Expert_SkillSwap_AlwaysGiveAbilities, ScoreMinus20
    IfLoadedInTable Expert_SkillSwap_UndesirableAbilities, ScoreMinus10
    IfLoadedInTable Expert_SkillSwap_AlwaysTakeAbilities, Expert_SkillSwap_DefenderAbilityBigScoreBoost
    IfLoadedInTable Expert_SkillSwap_DesirableAbilities, Expert_SkillSwap_DefenderAbilityScoreBoost
    GoTo Expert_SkillSwap_End

Expert_SkillSwap_DefenderAbilityScoreBoost:
    AddToMoveScore 1
    IfRandomLessThan 230, Expert_SkillSwap_End
    AddToMoveScore 1
    GoTo Expert_SkillSwap_End

Expert_SkillSwap_DefenderAbilityBigScoreBoost:
    AddToMoveScore 4
    IfRandomLessThan 230, Expert_SkillSwap_End
    AddToMoveScore 1
    GoTo Expert_SkillSwap_End
    
Expert_SkillSwap_End:
    PopOrEnd

Expert_SkillSwap_UndesirableAbilities:
    TableEntry ABILITY_BATTLE_ARMOR
    TableEntry ABILITY_DAMP
    TableEntry ABILITY_OBLIVIOUS
    TableEntry ABILITY_OWN_TEMPO
    TableEntry ABILITY_INNER_FOCUS
    TableEntry ABILITY_MAGMA_ARMOR
    TableEntry ABILITY_WATER_VEIL
    TableEntry ABILITY_RUN_AWAY
    TableEntry ABILITY_KEEN_EYE
    TableEntry ABILITY_PICKUP
    TableEntry ABILITY_TRUANT
    TableEntry ABILITY_CUTE_CHARM
    TableEntry ABILITY_STICKY_HOLD
    TableEntry ABILITY_OVERGROW
    TableEntry ABILITY_BLAZE
    TableEntry ABILITY_TORRENT
    TableEntry ABILITY_SWARM
    TableEntry ABILITY_SHELL_ARMOR
    TableEntry ABILITY_TANGLED_FEET
    TableEntry ABILITY_NORMALIZE
    TableEntry ABILITY_SNIPER
    TableEntry ABILITY_STALL
    TableEntry ABILITY_KLUTZ
    TableEntry ABILITY_ANTICIPATION
    TableEntry ABILITY_FOREWARN
    TableEntry ABILITY_SLOW_START
    TableEntry ABILITY_HONEY_GATHER
    TableEntry ABILITY_FRISK
    TableEntry ABILITY_IMPOSTER
    TableEntry TABLE_END

Expert_SkillSwap_AlwaysGiveAbilities:
    TableEntry ABILITY_DAMP
    TableEntry ABILITY_OBLIVIOUS
    TableEntry ABILITY_OWN_TEMPO
    TableEntry ABILITY_INNER_FOCUS
    TableEntry ABILITY_MAGMA_ARMOR
    TableEntry ABILITY_RUN_AWAY
    TableEntry ABILITY_KEEN_EYE
    TableEntry ABILITY_PICKUP
    TableEntry ABILITY_TRUANT
    TableEntry ABILITY_STICKY_HOLD
    TableEntry ABILITY_STALL
    TableEntry ABILITY_KLUTZ
    TableEntry ABILITY_ANTICIPATION
    TableEntry ABILITY_FOREWARN
    TableEntry ABILITY_SLOW_START
    TableEntry ABILITY_HONEY_GATHER
    TableEntry ABILITY_FRISK
	TableEntry ABILITY_AWARE
    TableEntry TABLE_END
    
Expert_SkillSwap_DesirableAbilities:
    TableEntry ABILITY_SPEED_BOOST
    TableEntry ABILITY_STURDY
    TableEntry ABILITY_LIMBER
    TableEntry ABILITY_STATIC
    TableEntry ABILITY_VOLT_ABSORB
    TableEntry ABILITY_WATER_ABSORB
    TableEntry ABILITY_COMPOUND_EYES
    TableEntry ABILITY_COLOR_CHANGE
    TableEntry ABILITY_IMMUNITY
    TableEntry ABILITY_FLASH_FIRE
    TableEntry ABILITY_SHIELD_DUST
    TableEntry ABILITY_INTIMIDATE
    TableEntry ABILITY_SHADOW_TAG
    TableEntry ABILITY_ROUGH_SKIN
    TableEntry ABILITY_WONDER_GUARD
    TableEntry ABILITY_LEVITATE
    TableEntry ABILITY_EFFECT_SPORE
    TableEntry ABILITY_SYNCHRONIZE
    TableEntry ABILITY_CLEAR_BODY
    TableEntry ABILITY_NATURAL_CURE
    TableEntry ABILITY_LIGHTNING_ROD
    TableEntry ABILITY_SERENE_GRACE
    TableEntry ABILITY_SWIFT_SWIM
    TableEntry ABILITY_CHLOROPHYLL
    TableEntry ABILITY_ILLUMINATE
    TableEntry ABILITY_HUGE_POWER
    TableEntry ABILITY_POISON_POINT
    TableEntry ABILITY_MAGNET_PULL
    TableEntry ABILITY_SOUNDPROOF
    TableEntry ABILITY_RAIN_DISH
    TableEntry ABILITY_PRESSURE
    TableEntry ABILITY_THICK_FAT
    TableEntry ABILITY_EARLY_BIRD
    TableEntry ABILITY_FLAME_BODY
    TableEntry ABILITY_HUSTLE
    TableEntry ABILITY_SHED_SKIN
    TableEntry ABILITY_GUTS
    TableEntry ABILITY_MARVEL_SCALE
    TableEntry ABILITY_LIQUID_OOZE
    TableEntry ABILITY_ROCK_HEAD
    TableEntry ABILITY_ARENA_TRAP
    TableEntry ABILITY_PURE_POWER
    TableEntry ABILITY_MOTOR_DRIVE
    TableEntry ABILITY_RIVALRY
    TableEntry ABILITY_STEADFAST
    TableEntry ABILITY_ANGER_POINT
    TableEntry ABILITY_UNBURDEN
    TableEntry ABILITY_HEATPROOF
    TableEntry ABILITY_SIMPLE
    TableEntry ABILITY_DRY_SKIN
    TableEntry ABILITY_DOWNLOAD
    TableEntry ABILITY_IRON_FIST
    TableEntry ABILITY_POISON_HEAL
    TableEntry ABILITY_ADAPTABILITY
    TableEntry ABILITY_SKILL_LINK
    TableEntry ABILITY_HYDRATION
    TableEntry ABILITY_SOLAR_POWER
    TableEntry ABILITY_QUICK_FEET
    TableEntry ABILITY_MAGIC_GUARD
    TableEntry ABILITY_TECHNICIAN
    TableEntry ABILITY_AFTERMATH
    TableEntry ABILITY_UNAWARE
    TableEntry ABILITY_TINTED_LENS
    TableEntry ABILITY_FILTER
    TableEntry ABILITY_SCRAPPY
    TableEntry ABILITY_STORM_DRAIN
    TableEntry ABILITY_ICE_BODY
    TableEntry ABILITY_SOLID_ROCK
    TableEntry ABILITY_RECKLESS
    TableEntry ABILITY_BAD_DREAMS
    TableEntry ABILITY_SLUSH_RUSH
    TableEntry ABILITY_MULTISCALE
    TableEntry ABILITY_POISON_TOUCH
    TableEntry ABILITY_DEFIANT
    TableEntry ABILITY_COMPETITIVE
    TableEntry ABILITY_FRESH_MILK
    TableEntry ABILITY_HEADACHE
    TableEntry ABILITY_RELENTLESS
    TableEntry ABILITY_SHEER_FORCE
    TableEntry ABILITY_CORROSION
    TableEntry ABILITY_STRONG_JAW
    TableEntry ABILITY_SAND_FORCE
    TableEntry ABILITY_MAGIC_BOUNCE
    TableEntry ABILITY_HOTHEADED
    TableEntry ABILITY_MEGA_LAUNCHER
    TableEntry ABILITY_TIDAL_FORCE
    TableEntry ABILITY_FREE_SAMPLE
    TableEntry ABILITY_FLARE_BOOST
    TableEntry ABILITY_CHLOROPLAST
    TableEntry ABILITY_COTTON_DOWN
    TableEntry ABILITY_PHOTOSYNTHESIS
    TableEntry ABILITY_SHARPNESS
    TableEntry ABILITY_STRANGLE_WEED
    TableEntry ABILITY_PEST
    TableEntry ABILITY_UNOWN_ENERGY
    TableEntry ABILITY_ROCK_SOLID
    TableEntry ABILITY_COWARD
	TableEntry ABILITY_THIRSTY
    TableEntry TABLE_END

Expert_SkillSwap_AlwaysTakeAbilities:
	TableEntry ABILITY_SPEED_BOOST
	TableEntry ABILITY_LIMBER
	TableEntry ABILITY_VOLT_ABSORB
	TableEntry ABILITY_WATER_ABSORB
	TableEntry ABILITY_COMPOUND_EYES
	TableEntry ABILITY_COLOR_CHANGE
	TableEntry ABILITY_IMMUNITY
	TableEntry ABILITY_FLASH_FIRE
	TableEntry ABILITY_SHADOW_TAG
	TableEntry ABILITY_ROUGH_SKIN
	TableEntry ABILITY_WONDER_GUARD
	TableEntry ABILITY_LEVITATE
	TableEntry ABILITY_LIGHTNING_ROD
	TableEntry ABILITY_SERENE_GRACE
	TableEntry ABILITY_SWIFT_SWIM
	TableEntry ABILITY_CHLOROPHYLL
	TableEntry ABILITY_ILLUMINATE
	TableEntry ABILITY_HUGE_POWER
	TableEntry ABILITY_MAGNET_PULL
	TableEntry ABILITY_RAIN_DISH
	TableEntry ABILITY_PRESSURE
	TableEntry ABILITY_THICK_FAT
	TableEntry ABILITY_FLAME_BODY
	TableEntry ABILITY_HUSTLE
	TableEntry ABILITY_GUTS
	TableEntry ABILITY_MARVEL_SCALE
	TableEntry ABILITY_ROCK_HEAD
	TableEntry ABILITY_ARENA_TRAP
	TableEntry ABILITY_PURE_POWER
	TableEntry ABILITY_MOTOR_DRIVE
	TableEntry ABILITY_STEADFAST
	TableEntry ABILITY_ANGER_POINT
	TableEntry ABILITY_UNBURDEN
	TableEntry ABILITY_HEATPROOF
	TableEntry ABILITY_SIMPLE
	TableEntry ABILITY_DRY_SKIN
	TableEntry ABILITY_IRON_FIST
	TableEntry ABILITY_POISON_HEAL
	TableEntry ABILITY_ADAPTABILITY
	TableEntry ABILITY_SKILL_LINK
	TableEntry ABILITY_HYDRATION
	TableEntry ABILITY_SOLAR_POWER
	TableEntry ABILITY_QUICK_FEET
	TableEntry ABILITY_MAGIC_GUARD
	TableEntry ABILITY_TECHNICIAN
	TableEntry ABILITY_AFTERMATH
	TableEntry ABILITY_UNAWARE
	TableEntry ABILITY_TINTED_LENS
	TableEntry ABILITY_FILTER
	TableEntry ABILITY_STORM_DRAIN
	TableEntry ABILITY_ICE_BODY
	TableEntry ABILITY_SOLID_ROCK
	TableEntry ABILITY_SLUSH_RUSH
	TableEntry ABILITY_MULTISCALE
	TableEntry ABILITY_POISON_TOUCH
	TableEntry ABILITY_DEFIANT
	TableEntry ABILITY_COMPETITIVE
	TableEntry ABILITY_HEADACHE
	TableEntry ABILITY_RELENTLESS
	TableEntry ABILITY_SHEER_FORCE
	TableEntry ABILITY_CORROSION
	TableEntry ABILITY_STRONG_JAW
	TableEntry ABILITY_SAND_FORCE
	TableEntry ABILITY_MAGIC_BOUNCE
	TableEntry ABILITY_HOTHEADED
	TableEntry ABILITY_MEGA_LAUNCHER
	TableEntry ABILITY_TIDAL_FORCE
	TableEntry ABILITY_FLARE_BOOST
	TableEntry ABILITY_CHLOROPLAST
	TableEntry ABILITY_PHOTOSYNTHESIS
	TableEntry ABILITY_SHARPNESS
	TableEntry ABILITY_STRANGLE_WEED
	TableEntry ABILITY_PEST
	TableEntry ABILITY_UNOWN_ENERGY
	TableEntry ABILITY_ROCK_SOLID
	TableEntry ABILITY_THIRSTY
	TableEntry TABLE_END
    
Expert_SkillSwap_ForbiddenDefenderAbilities:
    TableEntry ABILITY_SLOW_START
    TableEntry ABILITY_TRUANT
    TableEntry ABILITY_NORMALIZE
	TableEntry ABILITY_AWARE
    TableEntry TABLE_END

Expert_Ingrain:
    ; No score change.
    PopOrEnd 

Expert_Superpower:
    ; If the opponent would resist or is immune to the move, score -1.
    ;
    ; If the attacker''s Attack stat stage is at -1 or lower, score -1.
    ;
    ; If the attacker is slower than its opponent and has HP >= 60%, score -1.
    ;
    ; If the attacker is faster than its opponent and has HP > 40%, score -1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_Superpower_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_Superpower_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_Superpower_ScoreMinus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 6, Expert_Superpower_ScoreMinus1
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Superpower_CheckUserHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 40, Expert_Superpower_ScoreMinus1
    GoTo Expert_Superpower_End

Expert_Superpower_CheckUserHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 60, Expert_Superpower_End

Expert_Superpower_ScoreMinus1:
    AddToMoveScore -1

Expert_Superpower_End:
    PopOrEnd 

Expert_MagicCoat:
    ; If the opponent does not have a magic bounceable move, score -12.
    ;
    ; If the opponent just switched in, 66% chance for score +1,
    ; 33% chance for additional score +1, and 6.25% chance for additional score +1.
    ; Otherwise, 60% chance for score -1.
    ;
    ; If the attacker can kill on a different move, 94% chance for score -12.
    ; Otherwise, 33% chance for general score -1.
    ;
    ; If attacker is below 62% HP, 66% chance for score -1.
    ; 
    ; If it is the user''s first turn out, 50% chance for score +1 and 20%
    ; chance for score +2. Otherwise, 40% chance for score -1 and 16% chance
    ; for score -2.
    ;
    ; If the defender knows a hazards move that hasn''t been applied to the
    ; attacker''s side of the field, override previous directive and apply
    ; 75% chance for score +1, 37.5% chance for score +2.
    LoadBattlerIgnorableAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus12
    IfBattlerHasBounceableMove AI_BATTLER_DEFENDER, Expert_MagicCoat_Main
    GoTo ScoreMinus12

Expert_MagicCoat_Main:
	LoadIsFirstTurnInBattle AI_BATTLER_DEFENDER
	IfLoadedEqualTo TRUE, Expert_MagicCoat_FirstTurnBonus
    IfRandomLessThan 16, Expert_MagicCoat_CheckHP
    IfCanKOEnemy ScoreMinus12
    IfRandomLessThan 170, Expert_MagicCoat_CheckHP
    AddToMoveScore -1
	GoTo Expert_MagicCoat_CheckHP

Expert_MagicCoat_FirstTurnBonus:
    IfRandomLessThan 85, Expert_MagicCoat_CheckHP
    AddToMoveScore 1
	IfRandomLessThan 170, Expert_MagicCoat_CheckHP
    AddToMoveScore 1
	IfRandomLessThan 240, Expert_MagicCoat_CheckHP
    AddToMoveScore 1
	GoTo Expert_MagicCoat_CheckHP

Expert_MagicCoat_CheckHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 61, Expert_MagicCoat_CheckUserFirstTurn
    IfRandomLessThan 85, Expert_MagicCoat_CheckUserFirstTurn
    AddToMoveScore -1
	GoTo Expert_MagicCoat_CheckUserFirstTurn

Expert_MagicCoat_CheckUserFirstTurn:
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo FALSE, Expert_MagicCoat_CheckHazards
    IfRandomLessThan 128, Expert_MagicCoat_End
    AddToMoveScore 1
    IfRandomLessThan 153, Expert_MagicCoat_End
    AddToMoveScore 1
    GoTo Expert_MagicCoat_End

Expert_MagicCoat_CheckHazards:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STEALTH_ROCK, Expert_MagicCoat_CheckRocks
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SET_SPIKES, Expert_MagicCoat_CheckSpikes
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TOXIC_SPIKES, Expert_MagicCoat_CheckToxicSpikes
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SET_STICKY_WEB, Expert_MagicCoat_CheckStickyWeb
    GoTo Expert_MagicCoat_TryScoreMinus2

Expert_MagicCoat_CheckRocks:
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_STEALTH_ROCK, Expert_MagicCoat_TryScoreMinus2
    GoTo Expert_MagicCoat_HazardsBonus

Expert_MagicCoat_CheckSpikes:
    LoadSpikesLayers AI_BATTLER_ATTACKER, SIDE_CONDITION_SPIKES
    IfLoadedEqualTo 0, Expert_MagicCoat_HazardsBonus
    IfLoadedEqualTo 3, Expert_MagicCoat_TryScoreMinus2
    IfRandomLessThan 128, Expert_MagicCoat_End
    GoTo Expert_MagicCoat_TryScoreMinus2

Expert_MagicCoat_CheckToxicSpikes:
    LoadSpikesLayers AI_BATTLER_ATTACKER, SIDE_CONDITION_TOXIC_SPIKES
    IfLoadedEqualTo 0, Expert_MagicCoat_HazardsBonus
    IfLoadedEqualTo 2, Expert_MagicCoat_TryScoreMinus2
    IfRandomLessThan 128, Expert_MagicCoat_End
    GoTo Expert_MagicCoat_TryScoreMinus2

Expert_MagicCoat_CheckStickyWeb:
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_STICKY_WEB, Expert_MagicCoat_TryScoreMinus2
    GoTo Expert_MagicCoat_HazardsBonus

Expert_MagicCoat_HazardsBonus:
    IfRandomLessThan 64, Expert_MagicCoat_End
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_MagicCoat_End
    AddToMoveScore 1
    GoTo Expert_MagicCoat_End

Expert_MagicCoat_TryScoreMinus2:
    IfRandomLessThan 153, Expert_MagicCoat_End
    AddToMoveScore -1
    IfRandomLessThan 153, Expert_MagicCoat_End
    AddToMoveScore -1

Expert_MagicCoat_End:
    PopOrEnd 

Expert_Recycle:
    ; If the attacker''s Recyclable item is *not* any of the following, score -2:
    ; - Chesto Berry
    ; - Lum Berry
    ; - Starf Berry
    ;
    ; Otherwise, 80.5% chance of score +1.
    LoadRecycleItem AI_BATTLER_ATTACKER
    IfLoadedNotInTable Expert_Recycle_DesirableItems, Expert_Recycle_ScoreMinus2
    IfRandomLessThan 50, Expert_Recycle_End
    AddToMoveScore 1
    GoTo Expert_Recycle_End

Expert_Recycle_ScoreMinus2:
    AddToMoveScore -2

Expert_Recycle_End:
    PopOrEnd 

Expert_Recycle_DesirableItems:
    TableEntry ITEM_CHESTO_BERRY
    TableEntry ITEM_LUM_BERRY
    TableEntry ITEM_STARF_BERRY
    TableEntry ITEM_SITRUS_BERRY
    TableEntry ITEM_CUSTAP_BERRY
    TableEntry ITEM_AIR_BALLOON
    TableEntry ITEM_LEPPA_BERRY
    TableEntry ITEM_MICLE_BERRY
    TableEntry ITEM_AGUAV_BERRY
    TableEntry ITEM_FIGY_BERRY
    TableEntry ITEM_WIKI_BERRY
    TableEntry ITEM_MAGO_BERRY
    TableEntry ITEM_IAPAPA_BERRY
    TableEntry ITEM_LIECHI_BERRY
    TableEntry ITEM_GANLON_BERRY
    TableEntry ITEM_SALAC_BERRY
    TableEntry ITEM_PETAYA_BERRY
    TableEntry ITEM_APICOT_BERRY
    TableEntry ITEM_LANSAT_BERRY
    TableEntry ITEM_JABOCA_BERRY
    TableEntry ITEM_ROWAP_BERRY
    TableEntry ITEM_WEAKNESS_POLICY
    TableEntry TABLE_END

Expert_Revenge:
    ; If the opponent is asleep, infatuated, or confused, score -2.
    ;
    ; Otherwise, 70.3% chance of score -2, 29.7% chance of score +2.
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Expert_Revenge_ScoreMinus2
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, Expert_Revenge_ScoreMinus2
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CONFUSION, Expert_Revenge_ScoreMinus2
    IfRandomLessThan 180, Expert_Revenge_ScoreMinus2
    AddToMoveScore 2
    GoTo Expert_Revenge_End

Expert_Revenge_ScoreMinus2:
    AddToMoveScore -2

Expert_Revenge_End:
    PopOrEnd 

Expert_BrickBreak:
    ; If the opponent''s side of the field is under the effect of Reflect or Light Screen, score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_REFLECT, Expert_BrickBreak_TryScorePlus2
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_LIGHT_SCREEN, Expert_BrickBreak_TryScorePlus2
    GoTo Expert_BrickBreak_End

Expert_BrickBreak_TryScorePlus2:
    AddToMoveScore 1
    IfRandomLessThan 64, Expert_BrickBreak_End
    AddToMoveScore 1

Expert_BrickBreak_End:
    PopOrEnd 

Expert_KnockOff:
    ; Try not to Knock Off into Colbur Berry or Weakness Policy.
    ;
    ; Try not to knock off into Unburden or Rattled mons.
    ;
    ; If the enemy has an item, 75% chance for score +1.
    ;
    ; If the enemy has no item, 50% chance for score -1.
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_WEAKEN_SE_DARK, ScoreMinus1
    IfLoadedEqualTo HOLD_EFFECT_WEAK_RAISE_SPA_ATK, Expert_KnockOff_CheckEffectiveness
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_RATTLED, ScoreMinus2
    IfLoadedEqualTo ABILITY_UNBURDEN, ScoreMinus10
    LoadHeldItem AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ITEM_NONE, Expert_KnockOff_TryScorePlus1
    IfRandomLessThan 128, Expert_KnockOff_End
    AddToMoveScore -1
    GoTo Expert_KnockOff_End

Expert_KnockOff_CheckEffectiveness:
    IfCurrentMoveKills ROLL_FOR_DAMAGE, Expert_KnockOff_End
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, ScoreMinus3
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, ScoreMinus3
    AddToMoveScore 3
    GoTo Expert_KnockOff_End

Expert_KnockOff_TryScorePlus2:
    IfRandomLessThan 25, Expert_KnockOff_End
    AddToMoveScore 2
    GoTo Expert_KnockOff_End

Expert_KnockOff_TryScorePlus1:
    IfRandomLessThan 64, Expert_KnockOff_End
    AddToMoveScore 1
    GoTo Expert_KnockOff_End

Expert_KnockOff_End:
    PopOrEnd 

Expert_Endeavor:
    ; If the opponent''s HP < 70%, score -1 and terminate.
    ;
    ; If the attacker is slower than its opponent:
    ; - If the attacker''s HP > 50%, score -1.
    ; - Otherwise, score +1.
    ;
    ; If the attacker is faster than its opponent:
    ; - If the attacker''s HP > 40%, score -1.
    ; - Otherwise, score +1.
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_MOVE_IS_HIGHEST_DAMAGE, Try95ChanceForScorePlus3
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 50, Expert_Endeavor_ScoreMinus1
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Endeavor_SlowerCheckHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 40, Expert_Endeavor_ScoreMinus1
    AddToMoveScore 1
    GoTo Expert_Endeavor_End

Expert_Endeavor_SlowerCheckHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 50, Expert_Endeavor_ScoreMinus1
    AddToMoveScore 1
    GoTo Expert_Endeavor_End

Expert_Endeavor_ScoreMinus1:
    AddToMoveScore -1

Expert_Endeavor_End:
    PopOrEnd 

Expert_WaterSpout:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the attacker is slower than its opponent and the opponent''s HP <= 70%, score -1.
    ;
    ; If the attacker is faster than its opponent and the opponent''s HP < 53%, score -1.
    ;
    ; If our partner knows Follow Me, score +2.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_WaterSpout_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_WaterSpout_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_WaterSpout_ScoreMinus1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 53, Expert_WaterSpout_ScoreMinus1
    IfMoveEffectKnown AI_BATTLER_ATTACKER_PARTNER, BATTLE_EFFECT_MAKE_GLOBAL_TARGET, ScorePlus2
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_WaterSpout_SlowerCheckHP
    GoTo Expert_WaterSpout_End

Expert_WaterSpout_SlowerCheckHP:
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, Expert_WaterSpout_End
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_WaterSpout_End

Expert_WaterSpout_ScoreMinus1:
    AddToMoveScore -1

Expert_WaterSpout_End:
    PopOrEnd 

Expert_Imprison:
    ; If it is not the attacker''s first turn in battle, 60.9% chance of score +2.
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedGreaterThan FALSE, Expert_Imprison_End
    IfRandomLessThan 100, Expert_Imprison_End
    AddToMoveScore 2

Expert_Imprison_End:
    PopOrEnd 

Expert_Refresh:
    ; If the opponent''s HP < 50%, score -1.
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_FACADE_BOOST, Expert_Refresh_TryScorePlus2
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_Refresh_ScoreMinus1
    GoTo Expert_Refresh_End

Expert_Refresh_TryScorePlus2:
    IfRandomLessThan 12, Expert_Refresh_End
    AddToMoveScore 1
    IfRandomLessThan 12, Expert_Refresh_End
    AddToMoveScore 1
    GoTo Expert_Refresh_End

Expert_Refresh_ScoreMinus1:
    AddToMoveScore -1

Expert_Refresh_End:
    PopOrEnd 

Expert_Snatch:
    ; If it is the attacker''s first turn in battle, 41.4% chance of score +2.
    ;
    ; 11.7% chance of score +0 and terminate.
    ;
    ; If the attacker is slower than its opponent:
    ; - If the opponent''s HP > 25%, 88.3% chance of score -2.
    ; - If the opponent knows a flat-Recovery move or Defense Curl, 41.4% chance of score +2.
    ; - Otherwise, 10.2% chance of score +1.
    ;
    ; If the attacker is faster than its opponent:
    ; - If the attacker is not at full HP, 88.3% chance of score -2.
    ; - If the opponent''s HP < 70%, 88.3% chance of score -2.
    ; - Otherwise, 67.6% chance of score -2.
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_Snatch_TryScorePlus2
    IfRandomLessThan 30, Expert_Snatch_End
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Snatch_UserIsSlower
    IfHPPercentNotEqualTo AI_BATTLER_ATTACKER, 100, Expert_Snatch_TryScoreMinus2
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 70, Expert_Snatch_TryScoreMinus2
    IfRandomLessThan 60, Expert_Snatch_End
    GoTo Expert_Snatch_TryScoreMinus2

Expert_Snatch_UserIsSlower:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 25, Expert_Snatch_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_RESTORE_HALF_HP, Expert_Snatch_TryScorePlus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DEF_UP_DOUBLE_ROLLOUT_POWER, Expert_Snatch_TryScorePlus2
    GoTo Expert_Snatch_TryScorePlus1

Expert_Snatch_TryScorePlus2:
    IfRandomLessThan 150, Expert_Snatch_End
    AddToMoveScore 2
    GoTo Expert_Snatch_End

Expert_Snatch_TryScorePlus1:
    IfRandomLessThan 230, Expert_Snatch_TryScoreMinus2
    AddToMoveScore 1
    GoTo Expert_Snatch_End

Expert_Snatch_TryScoreMinus2:
    IfRandomLessThan 30, Expert_Snatch_End
    AddToMoveScore -2

Expert_Snatch_End:
    PopOrEnd 

Expert_MudSport:
    ; If the attacker''s HP < 50%, score -1.
    ;
    ; If the opponent has an Electric typing, score +1.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_MudSport_ScoreMinus1
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_ELECTRIC, Expert_MudSport_ScorePlus1
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_ELECTRIC, Expert_MudSport_ScorePlus1
    GoTo Expert_MudSport_ScoreMinus1

Expert_MudSport_ScorePlus1:
    AddToMoveScore 1
    GoTo Expert_MudSport_End

Expert_MudSport_ScoreMinus1:
    AddToMoveScore -1

Expert_MudSport_End:
    PopOrEnd 

Expert_Overheat:
    ; If the defender has unaware, score +1.
    ;
    ; If the attacker has a high hit stage, +1 or +2, depending on stage.
    ;
    ; If the move is resisted or ineffective, score -3 to -10.
    ;
    ; If another move deals more damage, score -10.
    ;
    ; If the attacker is very low HP, score +2. Else, score +1.
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_UNAWARE, Expert_Overheat_CheckCritStage
    AddToMoveScore 1
    GoTo Expert_Overheat_CheckCritStage

Expert_Overheat_CheckCritStage:
    LoadBattlerCritStage AI_BATTLER_ATTACKER
    IfLoadedGreaterThan 2, Expert_Overheat_TryScorePlus2
    IfLoadedGreaterThan 0, Expert_Overheat_TryScorePlus1
    GoTo Expert_Overheat_CheckEffectiveness

Expert_Overheat_TryScorePlus2:
    IfRandomLessThan 12, Expert_Overheat_CheckEffectiveness
    AddToMoveScore 2
    GoTo Expert_Overheat_CheckEffectiveness

Expert_Overheat_TryScorePlus1:
    IfRandomLessThan 12, Expert_Overheat_CheckEffectiveness
    AddToMoveScore 1
    GoTo Expert_Overheat_CheckEffectiveness

Expert_Overheat_CheckEffectiveness:
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, ScoreMinus5
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, ScoreMinus3
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NOT_HIGHEST_DAMAGE, ScoreMinus10
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 5, ScorePlus2
    IfRandomLessThan 85, Expert_Overheat_End
    AddToMoveScore 1
    GoTo Expert_Overheat_End

Expert_Overheat_End:
    PopOrEnd 

Expert_WaterSport:
    ; If the attacker''s HP < 50%, score -1.
    ;
    ; If the opponent has a Fire typing, score +1.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_WaterSport_ScoreMinus1
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, Expert_WaterSport_ScorePlus1
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, Expert_WaterSport_ScorePlus1
    GoTo Expert_WaterSport_ScoreMinus1

Expert_WaterSport_ScorePlus1:
    AddToMoveScore 1
    GoTo Expert_WaterSport_End

Expert_WaterSport_ScoreMinus1:
    AddToMoveScore -1

Expert_WaterSport_End:
    PopOrEnd 

Expert_DragonDance:
    ; If the attacker is slower than its opponent, 50% chance of score +1.
    ;
    ; If the attacker''s HP <= 50%, 72.7% chance of score -1.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_DragonDance_UserSlower
    IfCanHazeOrPhaze AI_BATTLER_DEFENDER, Expert_DragonDance_TryScoreMinus2
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_DESTINY_BOND, ScorePlus3
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ENCORE, Expert_DragonDance_CheckStatBoosts
    IfEnemyCanChunkOrKO Expert_DragonDance_CheckSubstitute
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 9, Expert_DragonDance_TryScoreMinus2
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 8, Expert_DragonDance_TryScoreMinus2
    IfRandomLessThan 51, Expert_DragonDance_CheckSubstitute
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DEF_UP_2, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STEAL_STATUS_MOVE, Expert_DragonDance_TryScoreMinus1
    AddToMoveScore 1
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_BIDE, ScorePlus3
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_COUNTER, Expert_DragonDance_TryScorePlus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_METAL_BURST, Expert_DragonDance_TryScorePlus2
    GoTo Expert_DragonDance_CheckSubstitute

Expert_DragonDance_UserSlower:
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TAUNT, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ATK_SPD_UP, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TRICK_ROOM, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ENCORE, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_FAINT_AND_ATK_SP_ATK_DOWN_2, Expert_DragonDance_TryScoreMinus2
    IfCanHazeOrPhaze AI_BATTLER_DEFENDER, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SPEED_UP_2, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DEF_UP_2, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STEAL_STATUS_MOVE, Expert_DragonDance_TryScoreMinus1
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, Expert_DragonDance_CheckSubstitute
    IfHasStatusThreat AI_BATTLER_DEFENDER, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STATUS_CONFUSE, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_CONFUSE_ALL, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SPEED_DOWN_HIT, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SPEED_DOWN, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SPEED_DOWN_2, Expert_DragonDance_TryScoreMinus1
    IfRandomLessThan 128, Expert_DragonDance_End
    AddToMoveScore 1
    GoTo Expert_DragonDance_End

Expert_DragonDance_CheckSubstitute:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SKILL_LINK, Expert_DragonDance_TryScoreMinus2
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, ScorePlus3
    IfHasStatusThreat AI_BATTLER_DEFENDER, Expert_DragonDance_TryScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STATUS_CONFUSE, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_CONFUSE_ALL, Expert_DragonDance_TryScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_FAINT_AND_ATK_SP_ATK_DOWN_2, Expert_DragonDance_TryScoreMinus2
    GoTo Expert_DragonDance_End

Expert_DragonDance_TryScoreMinus1:
    IfRandomLessThan 84, Expert_DragonDance_End
    AddToMoveScore -1
    GoTo Expert_DragonDance_End


Expert_DragonDance_TryScoreMinus2:
    IfRandomLessThan 25, Expert_DragonDance_End
    AddToMoveScore -2
    GoTo Expert_DragonDance_End

Expert_DragonDance_CheckStatBoosts:
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SPEED, 6, Expert_DragonDance_TryScoreMinus2
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 6, Expert_DragonDance_TryScoreMinus2
    IfRandomLessThan 84, Expert_DragonDance_End
    AddToMoveScore 1
    GoTo Expert_DragonDance_End

Expert_DragonDance_TryScorePlus2:
    IfRandomLessThan 64, Expert_DragonDance_End
    AddToMoveScore 2
    GoTo Expert_DragonDance_End

Expert_DragonDance_End:
    PopOrEnd 

Expert_Gravity:
    ; If the opponent has Levitate, is under the effect of Magnet Rise, or has a Flying typing, 75%
    ; chance of score +1.
    ;
    ; If the attacker''s HP >= 60%, 37.5% chance of score +1.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LEVITATE, Expert_Gravity_TryScorePlus1
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_MAGNET_RISE, Expert_Gravity_TryScorePlus1
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FLYING, Expert_Gravity_TryScorePlus1
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FLYING, Expert_Gravity_TryScorePlus1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 60, Expert_Gravity_End
    IfRandomLessThan 128, Expert_Gravity_TryScorePlus1
    GoTo Expert_Gravity_End

Expert_Gravity_TryScorePlus1:
    IfRandomLessThan 64, Expert_Gravity_End
    AddToMoveScore 1

Expert_Gravity_End:
    PopOrEnd 

Expert_MiracleEye:
    ; If the opponent has a Dark typing, 47.3% chance of score +2.
    ;
    ; If the opponent''s Evasion stat stage is at +3 or higher, 68.75% chance of score +2.
    ;
    ; Otherwise, score -2.
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_DARK, Expert_MiracleEye_ExtraRandomGate
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_DARK, Expert_MiracleEye_ExtraRandomGate
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 8, Expert_MiracleEye_ScorePlus2
    AddToMoveScore -2
    PopOrEnd 

Expert_MiracleEye_ExtraRandomGate:
    IfRandomLessThan 80, Expert_MiracleEye_End

Expert_MiracleEye_ScorePlus2:
    IfRandomLessThan 80, Expert_MiracleEye_End
    AddToMoveScore 2

Expert_MiracleEye_End:
    PopOrEnd 

Expert_WakeUpSlap:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the opponent is asleep, score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_WakeUpSlap_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_WakeUpSlap_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_WakeUpSlap_ScoreMinus1
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Expert_WakeUpSlap_ScorePlus1
    GoTo Expert_WakeUpSlap_End

Expert_WakeUpSlap_ScoreMinus1:
    AddToMoveScore -1
    GoTo Expert_WakeUpSlap_End

Expert_WakeUpSlap_ScorePlus1:
    AddToMoveScore 1

Expert_WakeUpSlap_End:
    PopOrEnd 

Expert_HammerArm:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the attacker is slower than its opponent, score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_HammerArm_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_HammerArm_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_HammerArm_ScoreMinus1
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_HammerArm_ScorePlus1
    GoTo Expert_HammerArm_End

Expert_HammerArm_ScoreMinus1:
    AddToMoveScore -1
    PopOrEnd 

Expert_HammerArm_ScorePlus1:
    AddToMoveScore 1

Expert_HammerArm_End:
    PopOrEnd 

Expert_GyroBall:
    ; No score changes.
    PopOrEnd 

Expert_Brine:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the opponent''s HP <= 50%, 50% chance of score +1, 50% chance of score +2.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_Brine_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_Brine_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_Brine_ScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 50, Expert_Brine_End
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_Brine_End
    AddToMoveScore 1
    GoTo Expert_Brine_End

Expert_Brine_ScoreMinus1:
    AddToMoveScore -1

Expert_Brine_End:
    PopOrEnd 

Expert_Feint:
    ; If the opponent does not know Protect, 75% chance of score +0.
    ;
    ; If the attacker is under any of the following conditions, 50% chance of additional score +1:
    ; - Toxic
    ; - Curse
    ; - Perish Song
    ; - Attract
    ; - Leech Seed
    ; - Yawn
    ;
    ; Otherwise, if the opponent is not at maximum HP and is holding Leftovers or Black Sludge, 50%
    ; chance of additional score +1.
    ;
    ; If the opponent''s Protect chain is 0, 50% chance of score +1.
    ;
    ; If the opponent''s Protect chain is 1, 25% chance of score +1.
    ;
    ; Otherwise, score -2.
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_PROTECT, Expert_Feint_CheckConditions
    IfRandomLessThan 64, Expert_Feint_CheckConditions
    GoTo Expert_Feint_End

Expert_Feint_CheckConditions:
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_TOXIC, Expert_Feint_TryScorePlus1
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_CURSE, Expert_Feint_TryScorePlus1
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_PERISH_SONG, Expert_Feint_TryScorePlus1
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_ATTRACT, Expert_Feint_TryScorePlus1
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED, Expert_Feint_TryScorePlus1
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_YAWN, Expert_Feint_TryScorePlus1
    IfHPPercentEqualTo AI_BATTLER_DEFENDER, 100, Expert_Feint_CheckProtectChain
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedNotInTable Expert_Feint_GradualRecoveryItems, Expert_Feint_CheckProtectChain

Expert_Feint_TryScorePlus1:
    IfRandomLessThan 128, Expert_Feint_CheckProtectChain
    AddToMoveScore 1

Expert_Feint_CheckProtectChain:
    LoadProtectChain AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, Expert_Feint_NoProtectChain
    IfLoadedEqualTo 1, Expert_Feint_ProtectChain1
    IfLoadedGreaterThan 1, Expert_Feint_ProtectChain2OrMore

Expert_Feint_NoProtectChain:
    IfRandomLessThan 128, Expert_Feint_End
    AddToMoveScore 1
    GoTo Expert_Feint_End

Expert_Feint_ProtectChain1:
    IfRandomLessThan 192, Expert_Feint_End
    AddToMoveScore 1
    GoTo Expert_Feint_End

Expert_Feint_ProtectChain2OrMore:
    AddToMoveScore -2

Expert_Feint_End:
    PopOrEnd 

Expert_Feint_GradualRecoveryItems:
    TableEntry HOLD_EFFECT_HP_RESTORE_GRADUAL
    TableEntry HOLD_EFFECT_HP_RESTORE_PSN_TYPE
    TableEntry TABLE_END

Expert_Pluck:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If it is the attacker''s first turn in battle, 75% chance of additional score +1.
    ;
    ; 50% chance of score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_Pluck_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_Pluck_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_Pluck_ScoreMinus1
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo FALSE, Expert_Pluck_TryScorePlus1
    IfRandomLessThan 64, Expert_Pluck_TryScorePlus1
    AddToMoveScore 1

Expert_Pluck_TryScorePlus1:
    IfRandomLessThan 128, Expert_Pluck_End
    AddToMoveScore 1
    GoTo Expert_Pluck_End

Expert_Pluck_ScoreMinus1:
    AddToMoveScore -1

Expert_Pluck_End:
    PopOrEnd 

Expert_Tailwind:
    ; If attacker can instead score a KO, 95% chance for score -12
    ;
    ; If the attacker is faster than its opponent, 25% chance for score -1.
    ;
    ; Otherwise, score +1.
    IfCanKOEnemy Expert_Tailwind_TryEarlyEnd
    AddToMoveScore 1
    GoTo Expert_Tailwind_SpeedCompare

Expert_Tailwind_TryEarlyEnd:
    IfRandomLessThan 243, ScoreMinus12
    GoTo Expert_Tailwind_SpeedCompare

Expert_Tailwind_SpeedCompare:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_Tailwind_TryScoreMinus1
    AddToMoveScore 1
    GoTo Expert_Tailwind_End

Expert_Tailwind_TryScoreMinus1:
    IfRandomLessThan 192, Expert_Tailwind_End
    AddToMoveScore -1
    GoTo Expert_Tailwind_End

Expert_Tailwind_End:
    PopOrEnd 

Expert_Acupressure:
    ; If the attacker''s HP <= 50%, score -1.
    ;
    ; If the attacker''s HP > 90%, 75% chance of score +1.
    ;
    ; Otherwise, 37.5% chance of score +1.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 51, Expert_Acupressure_ScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 75, Expert_Acupressure_TryScorePlus1
    IfRandomLessThan 128, Expert_Acupressure_End

Expert_Acupressure_TryScorePlus1:
    IfRandomLessThan 64, Expert_Acupressure_End
    AddToMoveScore 1
    GoTo Expert_Acupressure_End

Expert_Acupressure_ScoreMinus1:
    AddToMoveScore -1

Expert_Acupressure_End:
    PopOrEnd 

Expert_MetalBurst:
    ; If the opponent is asleep, infatuated, or confused or they know any of the following move
    ; effects, score -1 and terminate:
    ; - Avalanche / Revenge
    ; - Focus Punch
    ; - Vital Throw
    ;
    ; If the attacker''s HP <= 30%, 96% chance of additional score -1.
    ;
    ; If the attacker''s HP <= 50%, 60.9% chance of additional score -1.
    ;
    ; If the attacker''s HP > 50%, 25% chance of additional score +1.
    ;
    ; If the opponent''s last-used move was not a Status move and they are not Taunted, 60.9% chance
    ; of additional score +1.
    ;
    ; If the opponent is not Taunted, 60.9% chance of score +1.
    IfHasStatusThreat AI_BATTLER_DEFENDER, Try90ChanceForScoreMinus12
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_STURDY, Expert_MetalBurst_CheckSashBreak
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo HOLD_EFFECT_ENDURE, Expert_MetalBurst_CheckSashBreak
    IfRandomLessThan 170, Expert_MetalBurst_CheckDefenderCondition
    AddToMoveScore 1
    GoTo Expert_MetalBurst_CheckDefenderCondition

Expert_MetalBurst_CheckSashBreak:
    AddToMoveScore -1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 100, Expert_MetalBurst_CheckDefenderCondition
    IfCanBreakSashOrSturdy AI_BATTLER_DEFENDER, Try90ChanceForScoreMinus12
    AddToMoveScore 2
    IfRandomLessThan 128, Expert_MetalBurst_CheckDefenderCondition
    AddToMoveScore 1
    GoTo Expert_MetalBurst_CheckDefenderCondition

Expert_MetalBurst_CheckDefenderCondition:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, Expert_MetalBurst_DefenderSleep
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_FREEZE, Expert_MetalBurst_DefenderFreeze
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, Expert_MetalBurst_ScoreMinus1
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CONFUSION, Expert_MetalBurst_ScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_DOUBLE_POWER_IF_HIT, Expert_MetalBurst_ScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT, Expert_MetalBurst_ScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_PRIORITY_NEG_1_BYPASS_ACCURACY, Expert_MetalBurst_ScoreMinus1
    GoTo Expert_MetalBurst_CheckAttackerHP

Expert_MetalBurst_DefenderSleep:
    LoadSleepTurns AI_BATTLER_DEFENDER
    IfLoadedLessThan 2, Expert_MetalBurst_CheckAttackerHP
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP, Expert_MetalBurst_CheckAttackerHP
    GoTo ScoreMinus12

Expert_MetalBurst_DefenderFreeze:
    IfRandomLessThan 32, Expert_MetalBurst_CheckAttackerHP
    GoTo ScoreMinus12

Expert_MetalBurst_CheckLastUsedMove:
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    LoadPowerOfLoadedMove 
    IfLoadedEqualTo 0, Expert_MetalBurst_TryScorePlus1
    IfTargetIsNotTaunted Expert_MetalBurst_TryScorePlus1
    IfRandomLessThan 100, Expert_MetalBurst_TryScorePlus1
    AddToMoveScore 1

Expert_MetalBurst_TryScorePlus1:
    IfTargetIsNotTaunted Expert_MetalBurst_End
    IfRandomLessThan 100, Expert_MetalBurst_End
    AddToMoveScore 1
    GoTo Expert_MetalBurst_End

Expert_MetalBurst_VolatileStatusPenalty:
    IfRandomLessThan 64, Expert_MetalBurst_CheckAttackerHP
    AddToMoveScore -1
    GoTo Expert_MetalBurst_CheckAttackerHP

Expert_MetalBurst_CheckAttackerHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, Expert_MetalBurst_AttackerHPHigh
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_MetalBurst_AttackerHPLow
    IfRandomLessThan 128, Expert_MetalBurst_End
    AddToMoveScore 1
    GoTo Expert_MetalBurst_End

Expert_MetalBurst_AttackerHPHigh:
    IfRandomLessThan 64, Expert_MetalBurst_End
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_MetalBurst_End
    AddToMoveScore 1
    GoTo Expert_MetalBurst_End

Expert_MetalBurst_AttackerHPLow:
    AddToMoveScore -1
    IfRandomLessThan 16, Expert_MetalBurst_End
    AddToMoveScore -1
    IfRandomLessThan 16, Expert_MetalBurst_End
    GoTo Expert_MetalBurst_End

Expert_MetalBurst_ScoreMinus1:
    AddToMoveScore -1

Expert_MetalBurst_End:
    IfEnemyCanKO Try90ChanceForScoreMinus12
    PopOrEnd 

Expert_UTurn:
    ; If the opponent is immune to the move, score -10 and terminate.
    ;
    ; If the attacker is the last living party member, terminate.
    ;
    ; If the attacker has another super-effective move on its opponent, 75% chance of additional score -2.
    ;
    ; If no party member deals more damage than the attacker, 75% chance of score -2 and terminate.
    ;
    ; If the opponent''s HP > 70%, 75% chance of additional score +1.
    ;
    ; If the opponent''s HP > 40%, 50% chance of additional score +1. (Cumulative with the prior check)
    ;
    ; If the attacker is faster than its opponent, 50% chance of score +1.
    ; If the attacker is slower and has plenty of health, score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, Expert_UTurn_End
	IfBattlerStatDropped AI_BATTLER_ATTACKER, Expert_UTurn_TryScorePlus1
    IfHasSuperEffectiveMove Expert_UTurn_TryScoreMinus2
    GoTo Expert_UTurn_CheckPartyDamage

Expert_UTurn_TryScorePlus1:
    IfRandomLessThan 51, Expert_UTurn_CheckPartyDamage
    AddToMoveScore 1
    GoTo Expert_UTurn_CheckPartyDamage

Expert_UTurn_TryScoreMinus2:
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, Expert_UTurn_CheckPartyDamage
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, Expert_UTurn_CheckPartyDamage
    IfRandomLessThan 64, Expert_UTurn_CheckPartyDamage
    AddToMoveScore -2
    GoTo Expert_UTurn_End

Expert_UTurn_CheckPartyDamage:
    IfPartyMemberDealsMoreDamage USE_MAX_DAMAGE, Expert_UTurn_CheckTargetHP
    IfPartyMemberHasBattleEffect AI_BATTLER_ATTACKER, MOVE_ZAP_CANNON, Expert_UTurn_CheckTargetHP
    IfPartyMemberHasBattleEffect AI_BATTLER_ATTACKER, MOVE_U_TURN, Expert_UTurn_CheckTargetHP
    IfPartyMemberHasBattleEffect AI_BATTLER_ATTACKER, MOVE_TELEPORT, Expert_UTurn_CheckTargetHP
    IfRandomLessThan 64, Expert_UTurn_CheckTargetHP
    AddToMoveScore -2
    GoTo Expert_UTurn_End

Expert_UTurn_CheckTargetHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, Expert_UTurn_75PercentScorePlus1
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 40, Expert_UTurn_50PercentScorePlus1
    GoTo Expert_UTurn_CheckSpeed

Expert_UTurn_75PercentScorePlus1:
    IfRandomLessThan 64, Expert_UTurn_50PercentScorePlus1
    AddToMoveScore 1
    GoTo Expert_UTurn_50PercentScorePlus1

Expert_UTurn_50PercentScorePlus1:
    IfRandomLessThan 128, Expert_UTurn_CheckSpeed
    AddToMoveScore 1
    GoTo Expert_UTurn_CheckSpeed

Expert_UTurn_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_UTurn_FastScorePlus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 45, Expert_UTurn_ScorePlus1
    IfRandomLessThan 128, Expert_UTurn_End
    AddToMoveScore -1
    GoTo Expert_UTurn_End

Expert_UTurn_FastScorePlus1:
    IfRandomLessThan 128, Expert_UTurn_End
    AddToMoveScore 1
    GoTo Expert_UTurn_End

Expert_UTurn_ScorePlus1:
    AddToMoveScore 1
    GoTo Expert_UTurn_End

Expert_UTurn_End:
    PopOrEnd 

Expert_CloseCombat:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the attacker is slower than its opponent and its HP <= 80%, score -1.
    ;
    ; If the attacker is faster than its opponent and its HP <= 60%, score -1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_CloseCombat_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_CloseCombat_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_CloseCombat_ScoreMinus1
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_CloseCombat_SlowerCheckHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 60, Expert_CloseCombat_End
    GoTo Expert_CloseCombat_ScoreMinus1

Expert_CloseCombat_SlowerCheckHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 80, Expert_CloseCombat_End

Expert_CloseCombat_ScoreMinus1:
    AddToMoveScore -1

Expert_CloseCombat_End:
    PopOrEnd 

Expert_Payback:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the attacker is slower than its opponent and the attacker''s HP >= 30%, 75% chance of
    ; score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_Payback_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_Payback_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_Payback_ScoreMinus1
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_Payback_End
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 30, Expert_Payback_End
    IfRandomLessThan 64, Expert_Payback_End
    AddToMoveScore 1
    PopOrEnd 

Expert_Payback_ScoreMinus1:
    AddToMoveScore -1

Expert_Payback_End:
    PopOrEnd 

Expert_Assurance:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the attacker is slower than its opponent:
    ; - If the attacker''s ability is Rough Skin, 50% chance of score +1.
    ; - If the attacker is holding a Jaboca Berry or Rowap Berry, 50% chance of score +1.
    ; - Otherwise, 25% chance of score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_Assurance_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_Assurance_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_Assurance_ScoreMinus1
    IfSpeedCompareEqualTo COMPARE_SPEED_FASTER, Expert_Assurance_End
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_ROUGH_SKIN, Expert_Assurance_TryScorePlus1
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_Assurance_RecoilBerries, Expert_Assurance_TryScorePlus1
    IfRandomLessThan 128, Expert_Assurance_TryScorePlus1
    GoTo Expert_Assurance_End

Expert_Assurance_ScoreMinus1:
    AddToMoveScore -1
    GoTo Expert_Assurance_End

Expert_Assurance_TryScorePlus1:
    IfRandomLessThan 128, Expert_Assurance_End
    AddToMoveScore 1

Expert_Assurance_End:
    PopOrEnd 

Expert_Assurance_RecoilBerries:
    TableEntry ITEM_JABOCA_BERRY
    TableEntry ITEM_ROWAP_BERRY
    TableEntry TABLE_END

Expert_Embargo:
    ; 50% chance of score +1.
    IfRandomLessThan 128, Expert_Embargo_End
    AddToMoveScore 1

Expert_Embargo_End:
    PopOrEnd 

Expert_Fling:
    ; If fling power is 0 or the defender is immune, score - 12.
    ;
    ; Score +1 to +4 if it is first turn and attacker is holding an item
    ; that we want to throw right away.
    ;
    ; If there is a special fling effect, score the move according to the
    ; called move effect.
    ;
    ; If fling is resisted 4x, 90% chance for score -1.
    ; If fling is resisted, 50% chance for score -3.
    ;
    ; If fling is not resisted and is highest damage, 95% chance for score
    ; +1.
    LoadFlingPower AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, ScoreMinus12
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus12
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_Fling_CheckInstaFling
    LoadFlingEffect AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo FLING_EFFECT_NONE, Expert_Fling_TryFlingItemBoost
    GoTo Expert_Fling_Main

Expert_Fling_CheckInstaFling:
    LoadFlingEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo FLING_EFFECT_CHIP, Try95ChanceForScorePlus5
    IfLoadedInTable Expert_Fling_FieldEffects, Expert_Fling_InstaFlingBoost
	LoadHeldItem AI_BATTLER_ATTACKER
	IfLoadedInTable Expert_Fling_InstantFlingByItem, Expert_Fling_InstaFlingBoost
    GoTo Expert_Fling_CheckFlingEffect

Expert_Fling_InstaFlingBoost:
    AddToMoveScore 1
    IfRandomLessThan 12, Expert_Fling_Main
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_Fling_Main
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_Fling_Main
    AddToMoveScore 1
    GoTo Expert_Fling_CheckFlingEffect

Expert_Fling_CheckFlingEffect:
    LoadFlingEffect AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo FLING_EFFECT_NONE, Expert_Fling_TryFlingItemBoost
    GoTo Expert_Fling_Main

Expert_Fling_TryFlingItemBoost:
    LoadFlingEffect AI_BATTLER_ATTACKER
    IfLoadedInTable Expert_Fling_FieldEffects, Expert_Fling_CheckFieldEffect
    IfLoadedInTable Expert_Fling_StatusEffects, Expert_Fling_CheckStatus
    IfLoadedInTable Expert_Fling_StatBoostEffects, Expert_Fling_CheckStatBoost
    IfLoadedEqualTo FLING_EFFECT_PIVOT, Expert_Phaze
    IfLoadedEqualTo FLING_EFFECT_HAZE, Expert_Haze
    IfLoadedEqualTo FLING_EFFECT_DEFOG, Expert_Defog
    IfLoadedEqualTo FLING_EFFECT_INFATUATION, Expert_CheckCannotAttract
    IfLoadedEqualTo FLING_EFFECT_LOWER_ACC, Expert_StatusAccuracyDown
    IfLoadedEqualTo FLING_EFFECT_LOWER_EVASION, Expert_StatusEvasionDown
    IfLoadedEqualTo FLING_EFFECT_CONFUSION, Expert_CheckCannotConfuse
    IfLoadedEqualTo FLING_EFFECT_NIGHTMARE, Expert_CheckNightmare
    IfLoadedEqualTo FLING_EFFECT_WAKE_UP_SLAP, Expert_WakeUpSlap
    IfLoadedEqualTo FLING_EFFECT_INFLICT_CURSE, Try50ChanceForScorePlus1
    IfLoadedEqualTo FLING_EFFECT_INFLICT_INGRAIN, Expert_BindingMove
    GoTo Expert_Fling_Main

Expert_Fling_CheckFieldEffect:
    AddToMoveScore 1
    IfLoadedEqualTo FLING_EFFECT_RAIN, Expert_CheckCurrentWeatherIsRain
    IfLoadedEqualTo FLING_EFFECT_SUN, Expert_CheckCurrentWeatherIsSun
    IfLoadedEqualTo FLING_EFFECT_HAIL, Expert_CheckHail
    IfLoadedEqualTo FLING_EFFECT_SAND, Expert_CheckSandstorm
    IfLoadedEqualTo FLING_EFFECT_TRICK_ROOM, Expert_TrickRoom
    IfLoadedEqualTo FLING_EFFECT_GRAVITY, Expert_Gravity
    GoTo Expert_Fling_Main

Expert_Fling_CheckStatus:
    AddToMoveScore 1
    IfLoadedEqualTo FLING_EFFECT_PARALYZE, Expert_CheckCannotParalyze
    IfLoadedEqualTo FLING_EFFECT_BURN, Expert_CheckCannotBurn
    IfLoadedEqualTo FLING_EFFECT_POISON, Expert_CheckCannotPoison
    IfLoadedEqualTo FLING_EFFECT_BADLY_POISON, Expert_CheckCannotPoison
    GoTo Expert_Fling_Main

Expert_Fling_CheckStatBoost:
	IfLoadedEqualTo FLING_EFFECT_USER_ATK_UP, Expert_StatusAttackUp
    IfLoadedEqualTo FLING_EFFECT_USER_DEF_UP, Expert_StatusDefenseUp
    IfLoadedEqualTo FLING_EFFECT_USER_SPEED_UP, Expert_StatusSpeedUp
    IfLoadedEqualTo FLING_EFFECT_USER_SPATK_UP, Expert_StatusSpAttackUp
	IfLoadedEqualTo FLING_EFFECT_USER_SPDEF_UP, Expert_StatusSpDefenseUp
	GoTo Expert_Fling_Main

Expert_Fling_Main:
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Try90ChanceForScoreMinus12
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Try50ChanceForScoreMinus3
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, Try50ChanceForScorePlus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, Try75ChanceForScorePlus3
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NOT_HIGHEST_DAMAGE, Try50ChanceForScoreMinus3
    IfRandomLessThan 16, Expert_Fling_End
    AddToMoveScore 1
    GoTo Expert_Fling_End

Expert_Fling_End:
    PopOrEnd

Expert_Fling_FieldEffects:
    TableEntry FLING_EFFECT_TRICK_ROOM
    TableEntry FLING_EFFECT_GRAVITY
    TableEntry FLING_EFFECT_RAIN
    TableEntry FLING_EFFECT_SUN
    TableEntry FLING_EFFECT_SAND
    TableEntry FLING_EFFECT_HAIL
    TableEntry TABLE_END

Expert_Fling_StatusEffects:
    TableEntry FLING_EFFECT_PARALYZE
    TableEntry FLING_EFFECT_POISON
    TableEntry FLING_EFFECT_BADLY_POISON
    TableEntry FLING_EFFECT_BURN
    TableEntry TABLE_END

Expert_Fling_StatBoostEffects:
    TableEntry FLING_EFFECT_USER_ATK_UP
    TableEntry FLING_EFFECT_USER_DEF_UP
    TableEntry FLING_EFFECT_USER_SPEED_UP
    TableEntry FLING_EFFECT_USER_SPATK_UP
    TableEntry FLING_EFFECT_USER_SPDEF_UP
    TableEntry TABLE_END

Expert_Fling_InstantFlingByItem:
    TableEntry ITEM_BLACKGLASSES
	TableEntry ITEM_MOOMOO_MILK
    TableEntry ITEM_SILK_SCARF
    TableEntry TABLE_END


Expert_CheckCannotParalyze:
    ; If the target cannot be paralyzed for any reason, score -10.
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_ELECTRIC, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_ELECTRIC, ScoreMinus12
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LIMBER, ScoreMinus10
    IfLoadedEqualTo ABILITY_QUICK_FEET, ScoreMinus10
    IfMoveEqualTo MOVE_THUNDER_WAVE, Expert_CheckCannotParalyze_ThunderWave
    IfMoveEqualTo MOVE_STUN_SPORE, Expert_CheckCannotParalyze_PowderMove
    GoTo Expert_CheckCannotParalyze_ImmuneToStatus

Expert_CheckCannotParalyze_ThunderWave:
    LoadBattlerAbility AI_BATTLER_ATTACKER    
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Expert_CheckCannotParalyze_ImmuneToStatus
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MOTOR_DRIVE, ScoreMinus10
    IfLoadedEqualTo ABILITY_VOLT_ABSORB, ScoreMinus10
	IfLoadedEqualTo ABILITY_LIGHTNING_ROD, ScoreMinus10
    GoTo Expert_CheckCannotParalyze_ImmuneToStatus

Expert_CheckCannotParalyze_PowderMove:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, ScoreMinus12
    GoTo Expert_CheckCannotParalyze_ImmuneToStatus

Expert_CheckCannotParalyze_ImmuneToStatus:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus10
    PopOrEnd 
	
Expert_CheckCannotBurn:
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

Expert_CheckCannotPoison:
    ; If the target is immune to the usual effects of Poison for any reason, score -10.
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, Expert_CheckCannotPoison_CheckCorrosion
    IfLoadedEqualTo TYPE_POISON, ScoreMinus10
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_STEEL, Expert_CheckCannotPoison_CheckCorrosion
    IfLoadedEqualTo TYPE_POISON, ScoreMinus10
    GoTo Expert_CheckCannotPoison_CheckDefenderAbility

Expert_CheckCannotPoison_CheckDefenderAbility:
    ; Check for immunity by ability
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus10
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus10
    IfLoadedEqualTo ABILITY_SHED_SKIN, Try95ChanceForScoreMinus12
    IfLoadedNotEqualTo ABILITY_LEAF_GUARD, Expert_CheckCannotPoison_Hydration
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SUNNY, ScoreMinus10
    GoTo Expert_CheckCannotPoison_StatusOrSafeguard

Expert_CheckCannotPoison_CheckCorrosion:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo ABILITY_CORROSION, ScoreMinus12
    GoTo Expert_CheckCannotPoison_CheckDefenderAbility

Expert_CheckCannotPoison_Hydration:
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_HYDRATION, Expert_CheckCannotPoison_StatusOrSafeguard
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_RAINING, ScoreMinus10

Expert_CheckCannotPoison_StatusOrSafeguard:
    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus10
    GoTo Expert_CheckCannotPoison_CheckPowder

Expert_CheckCannotPoison_CheckPowder:
    IfMoveEqualTo MOVE_POISON_POWDER, Expert_CheckCannotPoison_Powder
    PopOrEnd

Expert_CheckCannotPoison_Powder:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GRASS, ScoreMinus12
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_WEATHER_CHIP_POWDER, ScoreMinus12
    PopOrEnd

Expert_CheckCurrentWeatherIsRain:
    ; If the weather is currently Rain, score -8.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_RAINING, ScoreMinus8
    PopOrEnd 

Expert_CheckCurrentWeatherIsSun:
    ; If the weather is currently Sun, score -8.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SUNNY, ScoreMinus8
    PopOrEnd 

Expert_CheckHail:
    ; If the current weather is Hail, score -8.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_HAILING, ScoreMinus8

    ; If any opposing battler''s ability is Ice Body, score -8.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_ICE_BODY, Expert_CheckHail_End
    AddToMoveScore -8

    ; If an attacker''s ability is also Ice Body, score +8 (undo the previous modifier).
    ; This feels like a bug of misintention; the intention here seems to be for an attacker with
    ; Ice Body to have an incentive to use Hail, but that is not realized. Instead, such an
    ; attacker can only have a disincentive undone.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo ABILITY_ICE_BODY, Expert_CheckHail_End
    AddToMoveScore 8
    
Expert_CheckHail_End:
    PopOrEnd

Expert_CheckSandstorm:
    ; If the current weather is Sand, score -8.
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SANDSTORM, ScoreMinus8
    PopOrEnd 

Expert_CheckCannotAttract:
    ; If the target cannot be Infatuated for any reason, score -12.
    ;
    ; If the target can be infatuated, 50% chance for score +1 and 25% chance for score +2.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, ScoreMinus12
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_OBLIVIOUS, ScoreMinus12
    LoadGender AI_BATTLER_ATTACKER
    IfLoadedEqualTo GENDER_MALE, Expert_CheckCannotAttract_BothMale
    IfLoadedEqualTo GENDER_FEMALE, Expert_CheckCannotAttract_BothFemale
    IfLoadedEqualTo GENDER_NONE, ScoreMinus12
    GoTo ScoreMinus12

Expert_CheckCannotAttract_BothMale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_FEMALE, Expert_CheckCannotAttract_CanAttract
    GoTo ScoreMinus12

Expert_CheckCannotAttract_BothFemale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_MALE, Expert_CheckCannotAttract_CanAttract
    GoTo ScoreMinus12

Expert_CheckCannotAttract_CanAttract:
    IfRandomLessThan 128, Expert_CheckCannotAttract_Terminate
    AddToMoveScore 1
    IfRandomLessThan 128, Expert_CheckCannotAttract_Terminate
    AddToMoveScore 1
    GoTo Expert_CheckCannotAttract_Terminate

Expert_CheckCannotAttract_Terminate:
    PopOrEnd 

Expert_CheckCannotConfuse:
    ; If the target is already confused, score -5.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_CONFUSION, ScoreMinus5

    ; If the target otherwise cannot be confused, score -10.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_OWN_TEMPO, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SAFEGUARD, ScoreMinus10
    PopOrEnd 

Expert_CheckNightmare:
    ; If the target is immune to the effect of Nightmare for any reason, score -10.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_NIGHTMARE, ScoreMinus10
    IfNotStatus AI_BATTLER_DEFENDER, MON_CONDITION_SLEEP, ScoreMinus10
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus10
    PopOrEnd 

Expert_PsychoShift:
    ; If the attacker does not have any status condition, score -12.
	
	IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY_POISON, Expert_StatusPoison
	IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_BURN, Expert_CheckCannotBurn
	IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_PARALYSIS, Expert_StatusParalyze
	
    AddToMoveScore -12
	GoTo Expert_PsychoShift_End

Expert_PsychoShift_End:
    PopOrEnd 

Expert_TrumpCard:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the move has 1 PP remaining, score +3.
    ;
    ; If the move has 2 PP remaining, 60.9% chance of score +2, 39.1% chance of score +1.
    ;
    ; If the move has 3 PP remaining, 60.9% chance of score +1.
    ;
    ; If the opponent''s ability is Pressure, 88.3% chance of additional score +1.
    ;
    ; If the opponent''s Evasion stat stage is +5 or higher or the attacker''s Accuracy stat stage is
    ; -5 or lower, 60.9% chance of score +2, 39.1% chance of score +1.
    ;
    ; If the opponent''s Evasion stat stage is +3 or higher or the attacker''s Accuracy stat stage is
    ; -3 or lower, 60.9% chance of score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_TrumpCard_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_TrumpCard_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_TrumpCard_ScoreMinus1
    LoadCurrentMovePP 
    IfLoadedEqualTo 1, Expert_TrumpCard_ScorePlus3
    IfLoadedEqualTo 2, Expert_TrumpCard_ScorePlus1Maybe2
    IfLoadedEqualTo 3, Expert_TrumpCard_ScorePlus1
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedNotEqualTo ABILITY_PRESSURE, Expert_TrumpCard_CheckStats
    IfRandomLessThan 30, Expert_TrumpCard_CheckStats
    AddToMoveScore 1

Expert_TrumpCard_CheckStats:
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 10, Expert_TrumpCard_ScorePlus1Maybe2
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 2, Expert_TrumpCard_ScorePlus1Maybe2
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 8, Expert_TrumpCard_ScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ACCURACY, 4, Expert_TrumpCard_ScorePlus1
    GoTo Expert_TrumpCard_End

Expert_TrumpCard_ScorePlus1Maybe2:
    AddToMoveScore 1

Expert_TrumpCard_ScorePlus1:
    IfRandomLessThan 100, Expert_TrumpCard_End
    AddToMoveScore 1
    GoTo Expert_TrumpCard_End

Expert_TrumpCard_ScorePlus3:
    AddToMoveScore 3
    GoTo Expert_TrumpCard_End

Expert_TrumpCard_ScoreMinus1:
    AddToMoveScore -1

Expert_TrumpCard_End:
    PopOrEnd 

Expert_HealBlock:
    ; If the opponent knows a move with any of the following effects, 90.2% chance of score +1:
    ; - Dream Eater
    ; - Restore half HP
    ; - Roost
    ; - Sun-boosted recovery
    ; - Rest
    ; - Swallow
    ; - Draining moves
    ; - Ingrain
    ; - Aqua Ring
    ; - Leech Seed
    ; - Lunar Dance, Healing Wish
    ;
    ; If the attacker is under the effect of Leech Seed or the opponent is under the effect of Ingrain
    ; or Aqua Ring, 90.2% chance of score +1.
    ;
    ; Otherwise, 56.4% chance of score +1.
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_RESTORE_HALF_HP, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_REST, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_SWALLOW, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_RECOVER_HALF_DAMAGE_DEALT, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_GROUND_TRAP_USER_CONTINUOUS_HEAL, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_RESTORE_HP_EVERY_TURN, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_STATUS_LEECH_SEED, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON, Expert_HealBlock_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON, Expert_HealBlock_TryScorePlus1
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_LEECH_SEED, Expert_HealBlock_TryScorePlus1
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_AQUA_RING, Expert_HealBlock_TryScorePlus1
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_INGRAIN, Expert_HealBlock_TryScorePlus1
    IfRandomLessThan 96, Expert_HealBlock_TryScorePlus1
    GoTo Expert_HealBlock_End

Expert_HealBlock_TryScorePlus1:
    IfRandomLessThan 25, Expert_HealBlock_End
    AddToMoveScore 1

Expert_HealBlock_End:
    PopOrEnd 

Expert_WringOut:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the opponent''s HP < 50%, score -1.
    ;
    ; If the opponent is at full HP:
    ; - Start with 90.2% chance of score +1.
    ; - If the attacker is faster than its opponent, additional score +2.
    ; - If the attacker is slower than its opponent, additional score +1.
    ;
    ; If the opponent''s HP > 85%, 90.2% chance of score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_WringOut_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_WringOut_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_WringOut_ScoreMinus1
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 50, Expert_WringOut_ScoreMinus1
    IfHPPercentEqualTo AI_BATTLER_DEFENDER, 100, Expert_WringOut_CheckSpeed
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 85, Expert_WringOut_TryScorePlus1
    GoTo Expert_WringOut_End

Expert_WringOut_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_WringOut_ScorePlus1
    AddToMoveScore 1

Expert_WringOut_ScorePlus1:
    AddToMoveScore 1

Expert_WringOut_TryScorePlus1:
    IfRandomLessThan 25, Expert_WringOut_End
    AddToMoveScore 1
    GoTo Expert_WringOut_End

Expert_WringOut_ScoreMinus1:
    AddToMoveScore -1

Expert_WringOut_End:
    PopOrEnd 

Expert_PowerTrick:
    ; If the attacker''s HP > 90%, 62.5% chance of score +1.
    ;
    ; If the attacker''s HP > 60%, 50% chance of score +1.
    ;
    ; If the attacker''s HP > 30%, 35.9% chance of score +1.
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 90, Expert_PowerTrick_LikelyScorePlus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 60, Expert_PowerTrick_CoinFlipScorePlus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 30, Expert_PowerTrick_UnlikelyScorePlus1
    GoTo ScoreMinus2

Expert_PowerTrick_LikelyScorePlus1:
    IfRandomLessThan 96, Expert_PowerTrick_End
    AddToMoveScore 1
    GoTo Expert_PowerTrick_End

Expert_PowerTrick_CoinFlipScorePlus1:
    IfRandomLessThan 128, Expert_PowerTrick_End
    AddToMoveScore 1
    GoTo Expert_PowerTrick_End

Expert_PowerTrick_UnlikelyScorePlus1:
    IfRandomLessThan 164, Expert_PowerTrick_End
    AddToMoveScore 1
    GoTo Expert_PowerTrick_End

Expert_PowerTrick_End:
    PopOrEnd 

Expert_GastroAcid:
    ; Check if target has a strongly beneficial ability
    ;
    ; If not, score -12
    ;
    ; If yes, check if we outspeed. If we outspeed, score +3
    ;
    ; If we don''t outspeed, check if we''re at high or low HP
    ;
    ; If we are at high or low HP, score +2
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_GastroAcid_AbilityChecklist, Expert_GastroAcid_SpeedCheck
    IfLoadedEqualTo ABILITY_NONE, ScoreMinus12
    IfRandomLessThan 5, Expert_GastroAcid_SpeedCheck
    AddToMoveScore -3
    GoTo Expert_GastroAcid_End

Expert_GastroAcid_SpeedCheck:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_GastroAcid_HPCheck
    IfRandomLessThan 16, Expert_GastroAcid_End
    AddToMoveScore +3
    GoTo Expert_GastroAcid_End

Expert_GastroAcid_HPCheck:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 80, Expert_GastroAcid_TryScorePlus2
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 30, Expert_GastroAcid_TryScorePlus2
    IfRandomLessThan 32, Expert_GastroAcid_TryScorePlus2
    GoTo Expert_GastroAcid_End

Expert_GastroAcid_TryScorePlus2:
    IfRandomLessThan 128, Expert_GastroAcid_End
    AddToMoveScore +2
    GoTo Expert_GastroAcid_End
    
Expert_GastroAcid_AbilityChecklist:
    TableEntry ABILITY_HUGE_POWER
    TableEntry ABILITY_GUTS
    TableEntry ABILITY_MAGIC_GUARD
    TableEntry ABILITY_MARVEL_SCALE
    TableEntry ABILITY_SPEED_BOOST
    TableEntry ABILITY_FLASH_FIRE
    TableEntry ABILITY_SHADOW_TAG
    TableEntry ABILITY_ROUGH_SKIN
    TableEntry ABILITY_WONDER_GUARD
    TableEntry ABILITY_LEVITATE
    TableEntry ABILITY_SWIFT_SWIM
    TableEntry ABILITY_MAGNET_PULL
    TableEntry ABILITY_PRESSURE
    TableEntry ABILITY_THICK_FAT
    TableEntry ABILITY_FLAME_BODY
    TableEntry ABILITY_HUSTLE
    TableEntry ABILITY_ARENA_TRAP
    TableEntry ABILITY_PURE_POWER
    TableEntry ABILITY_UNBURDEN
    TableEntry ABILITY_HEATPROOF
    TableEntry ABILITY_SIMPLE
    TableEntry ABILITY_POISON_HEAL
    TableEntry ABILITY_ADAPTABILITY
    TableEntry ABILITY_SKILL_LINK
    TableEntry ABILITY_SOLAR_POWER
    TableEntry ABILITY_NORMALIZE
    TableEntry ABILITY_NO_GUARD
    TableEntry ABILITY_TECHNICIAN
    TableEntry ABILITY_UNAWARE
    TableEntry ABILITY_TINTED_LENS
    TableEntry ABILITY_ICE_BODY
    TableEntry ABILITY_FLOWER_GIFT
    TableEntry ABILITY_BAD_DREAMS
    TableEntry ABILITY_SLUSH_RUSH
    TableEntry ABILITY_MULTISCALE
    TableEntry ABILITY_DEFIANT
    TableEntry ABILITY_COMPETITIVE
    TableEntry ABILITY_FRESH_MILK
    TableEntry ABILITY_RELENTLESS
    TableEntry ABILITY_SHEER_FORCE
    TableEntry ABILITY_CORROSION
    TableEntry ABILITY_STRONG_JAW
    TableEntry ABILITY_IRON_FIST
    TableEntry ABILITY_HOTHEADED
    TableEntry ABILITY_ROCK_HEAD
    TableEntry ABILITY_MEGA_LAUNCHER
    TableEntry ABILITY_FREE_SAMPLE
    TableEntry ABILITY_SHAKEDOWN
    TableEntry ABILITY_FLARE_BOOST
    TableEntry ABILITY_SHARPNESS
    TableEntry ABILITY_STRANGLE_WEED
	TableEntry ABILITY_STEADFAST
	TableEntry ABILITY_ROCK_SOLID
	TableEntry ABILITY_SUCTION_CUPS
	TableEntry ABILITY_THIRSTY
    TableEntry TABLE_END

Expert_GastroAcid_End:
    PopOrEnd 

Expert_LuckyChant:
    ; If the attacker''s HP < 70%, score -1.
    ;
    ; If the opponent knows a move with a high critical-hit ratio, score +1.
    ;
    ; Otherwise, 25% chance of score +1.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 70, Expert_LuckyChant_ScoreMinus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HIGH_CRITICAL, Expert_LuckyChant_ScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HIGH_CRITICAL_BURN_HIT, Expert_LuckyChant_ScorePlus1
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_HIGH_CRITICAL_POISON_HIT, Expert_LuckyChant_ScorePlus1
    IfRandomLessThan 64, Expert_LuckyChant_ScorePlus1
    GoTo Expert_LuckyChant_End

Expert_LuckyChant_ScorePlus1:
    AddToMoveScore 1
    GoTo Expert_LuckyChant_End

Expert_LuckyChant_ScoreMinus1:
    AddToMoveScore -1

Expert_LuckyChant_End:
    PopOrEnd 

Expert_MeFirst:
    ; If the attacker is slower than its opponent, score -2.
    ;
    ; If the attacker deals more damage than its opponent, 87.5% chance of additional score +1.
    ;
    ; If the opponent''s last-used move was a Damaging move, 50% chance of additional score +1.
    ;
    ; 75% chance of score +1.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_MeFirst_ScoreMinus2
    IfBattlerDealsMoreDamage AI_BATTLER_DEFENDER, USE_MAX_DAMAGE, Expert_MeFirst_TryScorePlus1
    GoTo Expert_MeFirst_CheckLastUsedMove

Expert_MeFirst_TryScorePlus1:
    IfRandomLessThan 32, Expert_MeFirst_CheckLastUsedMove
    AddToMoveScore 1

Expert_MeFirst_CheckLastUsedMove:
    LoadDefenderLastUsedMoveClass 
    IfLoadedEqualTo CLASS_STATUS, Expert_MeFirst_TryScorePlus1AndEnd
    IfRandomLessThan 128, Expert_MeFirst_End
    AddToMoveScore 1

Expert_MeFirst_TryScorePlus1AndEnd:
    IfRandomLessThan 64, Expert_MeFirst_End
    AddToMoveScore 1
    GoTo Expert_MeFirst_End

Expert_MeFirst_ScoreMinus2:
    AddToMoveScore -2

Expert_MeFirst_End:
    PopOrEnd 

Expert_Copycat:
    ; If the attacker is slower than its opponent, deals less damage than its opponent, and the
    ; opponent''s last-used move is not an encouraged move, 68.75% chance of score -1.
    ;
    ; If the attacker is faster than its opponent:
    ; - If the attacker deals more damage than its opponent, 87.5% chance of score +2.
    ; - If the opponent''s last-used move is an encouraged move, 50% chance of score +2.
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Copycat_CheckMoveEncouraged
    IfBattlerDealsMoreDamage AI_BATTLER_DEFENDER, USE_MAX_DAMAGE, Expert_Copycat_TryScorePlus2
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    IfLoadedNotInTable Expert_Copycat_EncouragedMoves, Expert_Copycat_CheckMoveEncouraged
    IfRandomLessThan 128, Expert_Copycat_End
    AddToMoveScore 2
    GoTo Expert_Copycat_End

Expert_Copycat_TryScorePlus2:
    IfRandomLessThan 32, Expert_Copycat_End
    AddToMoveScore 2
    GoTo Expert_Copycat_End

Expert_Copycat_CheckMoveEncouraged:
    IfBattlerDealsMoreDamage AI_BATTLER_DEFENDER, USE_MAX_DAMAGE, Expert_Copycat_End
    LoadBattlerPreviousMove AI_BATTLER_DEFENDER
    IfLoadedInTable Expert_Copycat_EncouragedMoves, Expert_Copycat_End
    IfRandomLessThan 80, Expert_Copycat_End
    AddToMoveScore -1

Expert_Copycat_End:
    PopOrEnd 

Expert_Copycat_EncouragedMoves:
    TableEntry MOVE_SLEEP_POWDER
    TableEntry MOVE_LOVELY_KISS
    TableEntry MOVE_SPORE
    TableEntry MOVE_HYPNOSIS
    TableEntry MOVE_SING
    TableEntry MOVE_GRASS_WHISTLE
    TableEntry MOVE_SHADOW_PUNCH
    TableEntry MOVE_SAND_ATTACK
    TableEntry MOVE_SMOKE_SCREEN
    TableEntry MOVE_TOXIC
    TableEntry MOVE_GUILLOTINE
    TableEntry MOVE_HORN_DRILL
    TableEntry MOVE_FISSURE
    TableEntry MOVE_SHEER_COLD
    TableEntry MOVE_CROSS_CHOP
    TableEntry MOVE_AEROBLAST
    TableEntry MOVE_CONFUSE_RAY
    TableEntry MOVE_SWEET_KISS
    TableEntry MOVE_SCREECH
    TableEntry MOVE_COTTON_SPORE
    TableEntry MOVE_SCARY_FACE
    TableEntry MOVE_FAKE_TEARS
    TableEntry MOVE_METAL_SOUND
    TableEntry MOVE_THUNDER_WAVE
    TableEntry MOVE_GLARE
    TableEntry MOVE_POISON_POWDER
    TableEntry MOVE_SHADOW_BALL
    TableEntry MOVE_DYNAMIC_PUNCH
    TableEntry MOVE_HYPER_BEAM
    TableEntry MOVE_EXTREME_SPEED
    TableEntry MOVE_THIEF
    TableEntry MOVE_COVET
    TableEntry MOVE_ATTRACT
    TableEntry MOVE_SWAGGER
    TableEntry MOVE_TORMENT
    TableEntry MOVE_FLATTER
    TableEntry MOVE_TRICK
    TableEntry MOVE_SUPERPOWER
    TableEntry MOVE_SKILL_SWAP
    TableEntry MOVE_PSYCHO_SHIFT
    TableEntry MOVE_POWER_SWAP
    TableEntry MOVE_GUARD_SWAP
    TableEntry MOVE_SUCKER_PUNCH
    TableEntry MOVE_HEART_SWAP
    TableEntry MOVE_SWITCHEROO
    TableEntry MOVE_CAPTIVATE
    TableEntry MOVE_DARK_VOID
    TableEntry TABLE_END

Expert_PowerSwap:
    ; Find the difference in stat stages between the attacker and its opponent for the Attack stat.
    ;
    ; If the difference is > 3:
    ; - If the difference in SpAttack stages > 3:
    ;   - 50% chance of score +5.
    ;   - 25% chance of score +4.
    ;   - 12.5% chance of score +3.
    ;   - 6.25% chance of score +2.
    ;   - 3.125% chance of score +1.
    ;   - 3.125% chance of score +0.
    ; - If the difference in SpAttack stages > 1:
    ;   - 50% chance of score +4.
    ;   - 25% chance of score +3.
    ;   - 12.5% chance of score +2.
    ;   - 6.25% chance of score +1.
    ;   - 6.25% chance of score +0.
    ; - If the difference in SpAttack stages = 0:
    ;   - 50% chance of score +3.
    ;   - 25% chance of score +2.
    ;   - 12.5% chance of score +1.
    ;   - 12.5% chance of score +0.
    ; - Otherwise, no score change.
    ;
    ; If the difference is > 1:
    ; - If the difference in SpAttack stages > 3:
    ;   - 50% chance of score +4.
    ;   - 25% chance of score +3.
    ;   - 12.5% chance of score +2.
    ;   - 6.25% chance of score +1.
    ;   - 6.25% chance of score +0.
    ; - If the difference in SpAttack stages > 1:
    ;   - 50% chance of score +3.
    ;   - 25% chance of score +2.
    ;   - 12.5% chance of score +1.
    ;   - 12.5% chance of score +0.
    ; - If the difference in SpAttack stages = 0:
    ;   - 50% chance of score +2.
    ;   - 25% chance of score +1.
    ;   - 25% chance of score +0.
    ; - Otherwise, no score change.
    ;
    ; If the difference is > 0:
    ; - If the difference in SpAttack stages > 3:
    ;   - 50% chance of score +3.
    ;   - 25% chance of score +2.
    ;   - 12.5% chance of score +1.
    ;   - 12.5% chance of score +0.
    ; - If the difference in SpAttack stages > 1:
    ;   - 50% chance of score +2.
    ;   - 25% chance of score +1.
    ;   - 25% chance of score +0.
    ; - If the difference in SpAttack stages = 0:
    ;   - 50% chance of score +1.
    ;   - 50% chance of score +0.
    ; - Otherwise, no score change.
    ;
    ; If the difference = 0:
    ; - If the difference in SpAttack stages > 3:
    ;   - 50% chance of score +3.
    ;   - 25% chance of score +2.
    ;   - 12.5% chance of score +1.
    ;   - 12.5% chance of score +0.
    ; - If the difference in SpAttack stages > 1:
    ;   - 50% chance of score +2.
    ;   - 25% chance of score +1.
    ;   - 25% chance of score +0.
    ; - If the difference in SpAttack stages > 0:
    ;   - 50% chance of score +1.
    ;   - 50% chance of score +0.
    ; - Otherwise, no score change.
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK
    IfLoadedGreaterThan 3, Expert_PowerSwap_CheckSpAttack_HighDiff
    IfLoadedGreaterThan 1, Expert_PowerSwap_CheckSpAttack_MediumDiff
    IfLoadedGreaterThan 0, Expert_PowerSwap_CheckSpAttack_LowDiff
    IfLoadedEqualTo 0, Expert_PowerSwap_CheckSpAttack_NoDiff
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_CheckSpAttack_HighDiff:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK
    IfLoadedGreaterThan 3, Expert_PowerSwap_TryScorePlus5
    IfLoadedGreaterThan 1, Expert_PowerSwap_TryScorePlus4
    IfLoadedEqualTo 0, Expert_PowerSwap_TryScorePlus3
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_CheckSpAttack_MediumDiff:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK
    IfLoadedGreaterThan 3, Expert_PowerSwap_TryScorePlus4
    IfLoadedGreaterThan 1, Expert_PowerSwap_TryScorePlus3
    IfLoadedEqualTo 0, Expert_PowerSwap_TryScorePlus2
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_CheckSpAttack_LowDiff:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK
    IfLoadedGreaterThan 3, Expert_PowerSwap_TryScorePlus3
    IfLoadedGreaterThan 1, Expert_PowerSwap_TryScorePlus2
    IfLoadedEqualTo 0, Expert_PowerSwap_TryScorePlus1
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_CheckSpAttack_NoDiff:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK
    IfLoadedGreaterThan 3, Expert_PowerSwap_TryScorePlus3
    IfLoadedGreaterThan 1, Expert_PowerSwap_TryScorePlus2
    IfLoadedGreaterThan 0, Expert_PowerSwap_TryScorePlus1
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_TryScorePlus5:
    IfRandomLessThan 128, Expert_PowerSwap_TryScorePlus4
    AddToMoveScore 5
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_TryScorePlus4:
    IfRandomLessThan 128, Expert_PowerSwap_TryScorePlus3
    AddToMoveScore 4
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_TryScorePlus3:
    IfRandomLessThan 128, Expert_PowerSwap_TryScorePlus2
    AddToMoveScore 3
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_TryScorePlus2:
    IfRandomLessThan 128, Expert_PowerSwap_TryScorePlus1
    AddToMoveScore 2
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_TryScorePlus1:
    IfRandomLessThan 128, Expert_PowerSwap_CheckSpAttack_End
    AddToMoveScore 1
    GoTo Expert_PowerSwap_CheckSpAttack_End

Expert_PowerSwap_CheckSpAttack_End:
    PopOrEnd 

Expert_GuardSwap:
    ; Find the difference in stat stages between the attacker and its opponent for the Defense stat.
    ;
    ; If the difference is > 3:
    ; - If the difference in SpDefense stages > 3:
    ;   - 50% chance of score +5.
    ;   - 25% chance of score +4.
    ;   - 12.5% chance of score +3.
    ;   - 6.25% chance of score +2.
    ;   - 3.125% chance of score +1.
    ;   - 3.125% chance of score +0.
    ; - If the difference in SpDefense stages > 1:
    ;   - 50% chance of score +4.
    ;   - 25% chance of score +3.
    ;   - 12.5% chance of score +2.
    ;   - 6.25% chance of score +1.
    ;   - 6.25% chance of score +0.
    ; - If the difference in SpDefense stages = 0:
    ;   - 50% chance of score +3.
    ;   - 25% chance of score +2.
    ;   - 12.5% chance of score +1.
    ;   - 12.5% chance of score +0.
    ; - Otherwise, no score change.
    ;
    ; If the difference is > 1:
    ; - If the difference in SpDefense stages > 3:
    ;   - 50% chance of score +4.
    ;   - 25% chance of score +3.
    ;   - 12.5% chance of score +2.
    ;   - 6.25% chance of score +1.
    ;   - 6.25% chance of score +0.
    ; - If the difference in SpDefense stages > 1:
    ;   - 50% chance of score +3.
    ;   - 25% chance of score +2.
    ;   - 12.5% chance of score +1.
    ;   - 12.5% chance of score +0.
    ; - If the difference in SpDefense stages = 0:
    ;   - 50% chance of score +2.
    ;   - 25% chance of score +1.
    ;   - 25% chance of score +0.
    ; - Otherwise, no score change.
    ;
    ; If the difference is > 0:
    ; - If the difference in SpDefense stages > 3:
    ;   - 50% chance of score +3.
    ;   - 25% chance of score +2.
    ;   - 12.5% chance of score +1.
    ;   - 12.5% chance of score +0.
    ; - If the difference in SpDefense stages > 1:
    ;   - 50% chance of score +2.
    ;   - 25% chance of score +1.
    ;   - 25% chance of score +0.
    ; - If the difference in SpDefense stages = 0:
    ;   - 50% chance of score +1.
    ;   - 50% chance of score +0.
    ; - Otherwise, no score change.
    ;
    ; If the difference = 0:
    ; - If the difference in SpDefense stages > 3:
    ;   - 50% chance of score +3.
    ;   - 25% chance of score +2.
    ;   - 12.5% chance of score +1.
    ;   - 12.5% chance of score +0.
    ; - If the difference in SpDefense stages > 1:
    ;   - 50% chance of score +2.
    ;   - 25% chance of score +1.
    ;   - 25% chance of score +0.
    ; - If the difference in SpDefense stages > 0:
    ;   - 50% chance of score +1.
    ;   - 50% chance of score +0.
    ; - Otherwise, no score change.
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE
    IfLoadedGreaterThan 3, Expert_GuardSwap_CheckSpDefense_HighDiff
    IfLoadedGreaterThan 1, Expert_GuardSwap_CheckSpDefense_MediumDiff
    IfLoadedGreaterThan 0, Expert_GuardSwap_CheckSpDefense_LowDiff
    IfLoadedEqualTo 0, Expert_GuardSwap_CheckSpDefense_NoDiff
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_CheckSpDefense_HighDiff:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE
    IfLoadedGreaterThan 3, Expert_GuardSwap_TryScorePlus5
    IfLoadedGreaterThan 1, Expert_GuardSwap_TryScorePlus4
    IfLoadedEqualTo 0, Expert_GuardSwap_TryScorePlus3
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_CheckSpDefense_MediumDiff:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE
    IfLoadedGreaterThan 3, Expert_GuardSwap_TryScorePlus4
    IfLoadedGreaterThan 1, Expert_GuardSwap_TryScorePlus3
    IfLoadedEqualTo 0, Expert_GuardSwap_TryScorePlus2
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_CheckSpDefense_LowDiff:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE
    IfLoadedGreaterThan 3, Expert_GuardSwap_TryScorePlus3
    IfLoadedGreaterThan 1, Expert_GuardSwap_TryScorePlus2
    IfLoadedEqualTo 0, Expert_GuardSwap_TryScorePlus1
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_CheckSpDefense_NoDiff:
    DiffStatStages AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE
    IfLoadedGreaterThan 3, Expert_GuardSwap_TryScorePlus3
    IfLoadedGreaterThan 1, Expert_GuardSwap_TryScorePlus2
    IfLoadedGreaterThan 0, Expert_GuardSwap_TryScorePlus1
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_TryScorePlus5:
    IfRandomLessThan 128, Expert_GuardSwap_TryScorePlus4
    AddToMoveScore 5
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_TryScorePlus4:
    IfRandomLessThan 128, Expert_GuardSwap_TryScorePlus3
    AddToMoveScore 4
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_TryScorePlus3:
    IfRandomLessThan 128, Expert_GuardSwap_TryScorePlus2
    AddToMoveScore 3
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_TryScorePlus2:
    IfRandomLessThan 128, Expert_GuardSwap_TryScorePlus1
    AddToMoveScore 2
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_TryScorePlus1:
    IfRandomLessThan 128, Expert_GuardSwap_End
    AddToMoveScore 1
    GoTo Expert_GuardSwap_End

Expert_GuardSwap_End:
    PopOrEnd 

Expert_Punishment:
    ; If the opponent resists or is immune to the move, score +0.
    ;
    ; Sum the total positive stat stages for the opponent:
    ; - If > 6:
    ;     - 50% chance of score +4
    ;     - 25% chance of score +3
    ;     - 12.5% chance of score +2
    ;     - 6.25% chance of score +1
    ;     - 6.25% chance of score +0
    ; - If = 6:
    ;     - 50% chance of score +3
    ;     - 25% chance of score +2
    ;     - 12.5% chance of score +1
    ;     - 12.5% chance of score +0
    ; - If = 5:
    ;     - 50% chance of score +2
    ;     - 25% chance of score +1
    ;     - 25% chance of score +0
    ; - If > 2:
    ;     - 50% chance of score +1
    ;     - 50% chance of score +0
    ; - Otherwise, score +0.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_Punishment_End
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_Punishment_End
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_Punishment_End
    SumPositiveStatStages AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 6, Expert_Punishment_TryScorePlus4
    IfLoadedGreaterThan 5, Expert_Punishment_TryScorePlus3
    IfLoadedGreaterThan 4, Expert_Punishment_TryScorePlus2
    IfLoadedGreaterThan 3, Expert_Punishment_TryScorePlus1
    IfLoadedGreaterThan 2, Expert_Punishment_TryScorePlus1
    GoTo Expert_Punishment_End

Expert_Punishment_TryScorePlus4:
    IfRandomLessThan 128, Expert_Punishment_TryScorePlus3
    AddToMoveScore 4

Expert_Punishment_TryScorePlus3:
    IfRandomLessThan 128, Expert_Punishment_TryScorePlus2
    AddToMoveScore 3

Expert_Punishment_TryScorePlus2:
    IfRandomLessThan 128, Expert_Punishment_TryScorePlus1
    AddToMoveScore 2

Expert_Punishment_TryScorePlus1:
    IfRandomLessThan 128, Expert_Punishment_End
    AddToMoveScore 1

Expert_Punishment_End:
    PopOrEnd 

Expert_LastResort:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; If the attacker can use Last Resort, score +1. Otherwise, score +0.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_LastResort_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_LastResort_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_LastResort_ScoreMinus1
    IfCanUseLastResort AI_BATTLER_ATTACKER, Expert_LastResort_ScorePlus1
    GoTo Expert_LastResort_End

Expert_LastResort_ScoreMinus1:
    AddToMoveScore -1
    GoTo Expert_LastResort_End

Expert_LastResort_ScorePlus1:
    AddToMoveScore 1

Expert_LastResort_End:
    PopOrEnd 

Expert_WorrySeed:
    ; If the opponent knows the move Rest, additional score +1.
    ;
    ; If the attacker''s HP >= 50%, 50% chance of additional score +1.
    ;
    ; 75% chance of score +1.
    IfMoveNotKnown AI_BATTLER_DEFENDER, MOVE_REST, Expert_WorrySeed_CheckUserHP
    AddToMoveScore 1

Expert_WorrySeed_CheckUserHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_WorrySeed_TryScorePlus1
    IfRandomLessThan 128, Expert_WorrySeed_TryScorePlus1
    AddToMoveScore 1

Expert_WorrySeed_TryScorePlus1:
    IfRandomLessThan 64, Expert_WorrySeed_End
    AddToMoveScore 1
    GoTo Expert_WorrySeed_End

Expert_WorrySeed_End:
    PopOrEnd 

Expert_SuckerPunch:
    ; If the opponent resists or is immune to the move, score -1.
    ;
    ; 75% chance of score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_SuckerPunch_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_SuckerPunch_ScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_SuckerPunch_ScoreMinus1
    IfRandomLessThan 64, Expert_SuckerPunch_End
    AddToMoveScore 1
    GoTo Expert_SuckerPunch_End

Expert_SuckerPunch_ScoreMinus1:
    AddToMoveScore -1

Expert_SuckerPunch_End:
    PopOrEnd 

Expert_HeartSwap:
    ; If the opponent does not have any of the following stats at +2 stage or greater and is not
    ; under the effect of Focus Energy, score -2 and terminate:
    ; - Attack
    ; - Defense
    ; - SpAttack
    ; - SpDefense
    ; - Evasion
    ;
    ; If the attacker has any of the following stats at +0 stage or lower or is not under the
    ; effect of Focus Energy, score +1:
    ; - Attack
    ; - Defense
    ; - SpAttack
    ; - SpDefense
    ;
    ; If the attacker''s Evasion stat is at +0 stage or lower, score +2.
    ;
    ; Otherwise, 80.5% chance of score -2.
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 7, Expert_HeartSwap_CheckUserStages
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_DEFENSE, 7, Expert_HeartSwap_CheckUserStages
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 7, Expert_HeartSwap_CheckUserStages
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_DEFENSE, 7, Expert_HeartSwap_CheckUserStages
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 7, Expert_HeartSwap_CheckUserStages
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_FOCUS_ENERGY, Expert_HeartSwap_CheckUserStages
    GoTo Expert_HeartSwap_ScoreMinus2

Expert_HeartSwap_CheckUserStages:
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 7, Expert_HeartSwap_ScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_DEFENSE, 7, Expert_HeartSwap_ScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 7, Expert_HeartSwap_ScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_DEFENSE, 7, Expert_HeartSwap_ScorePlus1
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_EVASION, 7, Expert_HeartSwap_ScorePlus2
    IfNotVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_FOCUS_ENERGY, Expert_HeartSwap_ScorePlus1
    IfRandomLessThan 50, Expert_HeartSwap_End
    GoTo Expert_HeartSwap_ScoreMinus2

Expert_HeartSwap_ScorePlus2:
    AddToMoveScore 1

Expert_HeartSwap_ScorePlus1:
    AddToMoveScore 1
    PopOrEnd 

Expert_HeartSwap_ScoreMinus2:
    AddToMoveScore -2

Expert_HeartSwap_End:
    PopOrEnd 

Expert_AquaRing:
    ; If the attacker''s HP >= 30%, 50% chance of score +1.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 30, Expert_AquaRing_End
    IfRandomLessThan 128, Expert_AquaRing_End
    AddToMoveScore 1

Expert_AquaRing_End:
    PopOrEnd 

Expert_MagnetRise:
    ; If the attacker''s HP < 50%, ignore all further score changes.
    ;
    ; If the opponent knows one of the following moves, additional score +1:
    ; - Earthquake
    ; - Earth Power
    ; - Fissure
    ;
    ; If the opponent has a Ground typing, score +1. Otherwise, 50% chance of score +1.
	
	; If the attacker is already under the effect, score -12.
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_MAGNET_RISE, ScoreMinus12

    ; If the attacker''s ability is Levitate, score -12.
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_LEVITATE, ScoreMinus12

    ; If either of the attacker''s types are Flying, score -12.
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_FLYING, ScoreMinus12
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_FLYING, ScoreMinus12
	
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_MagnetRise_End
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_EARTHQUAKE, Expert_MagnetRise_InitialScorePlus1
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_EARTH_POWER, Expert_MagnetRise_InitialScorePlus1
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FISSURE, Expert_MagnetRise_InitialScorePlus1
    GoTo Expert_MagnetRise_CheckOpponentTyping

Expert_MagnetRise_InitialScorePlus1:
    AddToMoveScore 1

Expert_MagnetRise_CheckOpponentTyping:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GROUND, Expert_MagnetRise_ScorePlus1
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GROUND, Expert_MagnetRise_ScorePlus1
    IfRandomLessThan 128, Expert_MagnetRise_End

Expert_MagnetRise_ScorePlus1:
    AddToMoveScore 1
	LoadBattlerAbility AI_BATTLER_ATTACKER
	IfLoadedEqualTo ABILITY_MAGNET_PULL, Expert_MagnetRise_CheckSpeed
	GoTo Expert_MagnetRise_End
	
Expert_MagnetRise_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_MagnetRise_ScorePlus3
	AddToMoveScore 1
	GoTo Expert_MagnetRise_End
    
Expert_MagnetRise_ScorePlus3:
    AddToMoveScore 3

Expert_MagnetRise_End:
    PopOrEnd 

Expert_Defog:
	; If the defender has Defiant or Competitive, score -10
	;
    ; If the opponent''s side of the field is under the effect of Light Screen or Reflect:
    ; - If the attacker''s HP < 30% and there are no remaining party members:
    ;   - 80.5% chance of additional score -2.
    ;   - If the opponent''s HP > 70%, score -2.
    ; - Start at score +1.
    ; - If the opponent has at least one remaining party member and their side of the field is
    ; under the effect of Spikes, Stealth Rock, or Toxic Spikes, 50% chance of score -1.
    ; - Proceed to the final if-block below.
    ;
    ; If the opponent''s side of the field is under the effect of Spikes, Stealth Rock, or Toxic
    ; Spikes, additional score -2.
    ;
    ; If all of the following conditions are met, score -2:
    ; - The attacker''s HP >= 70%
    ; - The opponent''s Evasion stat is at -2 stage or greater
    ; - The opponent''s HP <= 70%
    ; Otherwise:
    ; - 80.5% chance of additional score -2.
    ; - If the opponent''s HP <= 70% score -2.
	LoadBattlerAbility AI_BATTLER_DEFENDER
	IfLoadedEqualTo ABILITY_COMPETITIVE, Expert_Defog_CheckCompetitive
	IfLoadedEqualTo ABILITY_DEFIANT, Expert_Defog_CheckDefiant
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_LIGHT_SCREEN, Expert_Defog_ScreenScrubbing
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_REFLECT, Expert_Defog_ScreenScrubbing
    GoTo Expert_Defog_CheckHazardsScore

 Expert_Defog_CheckHazardsScore:
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, Expert_Defog_HazardsScoreMinus12
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SPIKES, Expert_Defog_HazardsScoreSpikesTryMinus2
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STEALTH_ROCK, Expert_Defog_HazardsScoreStealthRockTryMinus2
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_TOXIC_SPIKES, Expert_Defog_HazardsScoreToxicSpikesTryMinus2
	IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STICKY_WEB, Expert_Defog_HazardsScoreStickyWebTryMinus2
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_STEALTH_ROCK, Expert_Defog_HazardsScorePlus2
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_SPIKES, Expert_Defog_HazardsScorePlus2
    IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_TOXIC_SPIKES, Expert_Defog_HazardsScorePlus2
	IfSideCondition AI_BATTLER_ATTACKER, SIDE_CONDITION_STICKY_WEB, Expert_Defog_HazardsScorePlus2
    GoTo Expert_Defog_CheckUserHPAndOpponentEvasion
    

Expert_Defog_HazardsScoreSpikesTryMinus2:
    IfMoveEffectNotKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_SET_SPIKES, Expert_Defog_CheckEnemyTeamAliveAndTryMinus2

Expert_Defog_HazardsScoreStealthRockTryMinus2:
    IfMoveEffectNotKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_STEALTH_ROCK, Expert_Defog_CheckEnemyTeamAliveAndTryMinus2

Expert_Defog_HazardsScoreToxicSpikesTryMinus2:
    IfMoveEffectNotKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_TOXIC_SPIKES, Expert_Defog_CheckEnemyTeamAliveAndTryMinus2
	
Expert_Defog_HazardsScoreStickyWebTryMinus2:
    IfMoveEffectNotKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_SET_STICKY_WEB, Expert_Defog_CheckEnemyTeamAliveAndTryMinus2

Expert_Defog_CheckEnemyTeamAliveAndTryMinus2:
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 0, Expert_Defog_TryHazardsScoreMinus2

Expert_Defog_TryHazardsScoreMinus2:
    IfRandomLessThan 13, Expert_Defog_HazardsScoreMinus2

Expert_Defog_CheckDefiant:
    AddToMoveScore -12
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 2, Expert_Defog_HazardsScorePlus12
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 12, Expert_Defog_HazardsScorePlus12

Expert_Defog_CheckCompetitive:
    AddToMoveScore -12
    IfStatStageLessThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 2, Expert_Defog_HazardsScorePlus12
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 12, Expert_Defog_HazardsScorePlus12

Expert_Defog_ScreenScrubbing:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 30, Expert_Defog_ScreenScrubbingCheckHazards
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, Expert_Defog_TryScoreMinus2

Expert_Defog_ScreenScrubbingCheckHazards:
    AddToMoveScore 1
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, Expert_Defog_End
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_SPIKES, Expert_Defog_TryScoreMinus1
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STEALTH_ROCK, Expert_Defog_TryScoreMinus1
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_TOXIC_SPIKES, Expert_Defog_TryScoreMinus1
    GoTo Expert_Defog_CheckUserHPAndOpponentEvasion

Expert_Defog_TryScoreMinus1:
    IfRandomLessThan 128, Expert_Defog_CheckUserHPAndOpponentEvasion
    AddToMoveScore -1
    GoTo Expert_Defog_CheckUserHPAndOpponentEvasion

Expert_Defog_CheckUserHPAndOpponentEvasion:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_Defog_TryScoreMinus2
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_EVASION, 3, Expert_Defog_CheckOpponentHP
    GoTo Expert_Defog_End

Expert_Defog_TryScoreMinus2:
    IfRandomLessThan 50, Expert_Defog_CheckOpponentHP
    AddToMoveScore -2

Expert_Defog_CheckOpponentHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 30, Expert_Defog_End
    IfRandomLessThan 128, ScoreMinus2
    GoTo Expert_Defog_End

Expert_Defog_HazardsScoreMinus2:
    AddToMoveScore -2

Expert_Defog_HazardsScorePlus2:
    AddToMoveScore 2

Expert_Defog_HazardsScoreMinus12:
    AddToMoveScore -12

Expert_Defog_HazardsScorePlus12:
    AddToMoveScore 12

Expert_Defog_End:
    PopOrEnd 

Expert_TrickRoom:
    ; If the battle is a Double Battle, ignore all further score modifiers.
    ;
    ; If the attacker''s HP <= 30% and there are no remaining party members, score +0.
    ;
    ; If the attacker is faster than its opponent, score -1.
    ;
    ; If the attacker is slower than its opponent, 75% chance of score +3.
    LoadBattleType 
    IfLoadedMask BATTLE_TYPE_DOUBLES, Expert_TrickRoom_End
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 30, Expert_TrickRoom_CheckSpeed
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, Expert_TrickRoom_End

Expert_TrickRoom_CheckSpeed:
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_TrickRoom_TryScorePlus3
    AddToMoveScore -1
    GoTo Expert_TrickRoom_End

Expert_TrickRoom_TryScorePlus3:
    IfRandomLessThan 64, Expert_TrickRoom_End
    AddToMoveScore 3

Expert_TrickRoom_End:
    PopOrEnd 

Expert_Blizzard:
    ; If the opponent resists or is immune to the move, 80.5% chance of score -3.
    ;
    ; If the current weather is Hail, score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_Blizzard_TryScoreMinus3
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_Blizzard_TryScoreMinus3
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_Blizzard_TryScoreMinus3
    LoadCurrentWeather 
    IfLoadedNotEqualTo AI_WEATHER_HAILING, Expert_Blizzard_End
    AddToMoveScore 1
    GoTo Expert_Blizzard_End

Expert_Blizzard_TryScoreMinus3:
    IfRandomLessThan 50, Expert_Blizzard_End
    AddToMoveScore -3

Expert_Blizzard_End:
    PopOrEnd 

Expert_Captivate:
    ; If the opponent''s SpAttack stat stage is at any value other than +0:
    ; - Start at score -1.
    ; - If the attacker''s HP <= 90%, additional score -1.
    ; - If the opponent''s SpAttack stat stage is at -3 or lower, 80.5% chance of additional score -2.
    ;
    ; If the opponent''s HP <= 70%, additional score -2.
    ;
    ; If the opponent''s last-used move was a Physical move, 75% chance of score -1.
    IfStatStageEqualTo AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 6, Expert_Captivate_CheckOpponentHP
    AddToMoveScore -1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 90, Expert_Captivate_CheckLowStatStage
    AddToMoveScore -1

Expert_Captivate_CheckLowStatStage:
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_SP_ATTACK, 3, Expert_Captivate_CheckOpponentHP
    IfRandomLessThan 50, Expert_Captivate_CheckOpponentHP
    AddToMoveScore -2

Expert_Captivate_CheckOpponentHP:
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, Expert_Captivate_CheckOpponentLastMove
    AddToMoveScore -2

Expert_Captivate_CheckOpponentLastMove:
    LoadDefenderLastUsedMoveClass 
    IfLoadedNotEqualTo CLASS_PHYSICAL, Expert_Captivate_End
    IfRandomLessThan 64, Expert_Captivate_End
    AddToMoveScore -1

Expert_Captivate_End:
    PopOrEnd 

Expert_StealthRock:
    ; Don''t use if defender is on their last mon.
    ;
    ; Check if defender knows Rapid Spin, Defog, or Taunt.
    ;
    ; If Rapid Spin, check if attacker has a move that punishes on contact.
    ; If attacker does have move that punishes on contact, try to click rocks into a spinner.
    ;
    ; Similar for Defog, but with Defiant / Competitive
    ;
    ; If defender knows Taunt, try rocks 25% of the time, otherwise score -1.
    ;
    ; Try rocks has 50% chance to default to +1 score if Stealth Rocks is not already up.
    ;
    ; 90% chance to add +4 score during the first 3 turns of a battle.
    ;
    ; If the attacker knows either of the moves Roar, Whirlwind or Dragon Tail (Swift), 33% chance of additional score +1.
    ;
    ; If the attacker has just switched in, 33% chance of additional score +1.

    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedLessThan 2, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STEALTH_ROCK, ScoreMinus10
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus10
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_RAPID_SPIN, Expert_StealthRock_CheckRapidSpin
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_DEFOG, Expert_StealthRock_CheckDefog
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_TAUNT, Expert_StealthRock_TryScoreMinus1
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, Expert_StealthRock_CheckToEncourage
    IfRandomLessThan 64, Expert_StealthRock_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_StealthRock_CheckToEncourage

Expert_StealthRock_CheckToEncourage:
    LoadTurnCount
    IfLoadedLessThan 3, Expert_StealthRock_TryScorePlus4
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_ROAR, Expert_StealthRock_TryScorePlus1
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_WHIRLWIND, Expert_StealthRock_TryScorePlus1
	IfMoveKnown AI_BATTLER_ATTACKER, MOVE_SWIFT, Expert_StealthRock_TryScorePlus1
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_StealthRock_TryScorePlus1
    IfRandomLessThan 128, Expert_StealthRock_End
    AddToMoveScore 1
    GoTo Expert_StealthRock_End

Expert_StealthRock_CheckRapidSpin:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, Expert_StealthRock_CheckItem
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, Expert_StealthRock_CheckItem
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_ABILITY_SUPPRESSED, ScoreMinus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedInTable StealthRock_AbilityPunish_RapidSpin, Expert_StealthRock_CheckAbilityImmunity
    IfRandomLessThan 25, Expert_StealthRock_CheckToEncourage
    AddToMoveScore -8
    GoTo Expert_StealthRock_End

Expert_StealthRock_CheckItem:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NORMAL_HIT_GHOST, ScoreMinus3
    GoTo Expert_StealthRock_CheckToEncourage

Expert_StealthRock_CheckAbilityImmunity:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus3
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH, ScoreMinus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_POINT, Expert_StealthRock_CheckPoisonPoint
    IfLoadedEqualTo ABILITY_FLAME_BODY, Expert_StealthRock_CheckFlameBody
    IfLoadedEqualTo ABILITY_STATIC, Expert_StealthRock_CheckStatic
    IfLoadedEqualTo ABILITY_FRESH_MILK, Expert_StealthRock_CheckFreshMilk
    GoTo Expert_StealthRock_CheckToEncourage

Expert_StealthRock_CheckPoisonPoint:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus3
    IfLoadedEqualTo TYPE_POISON, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_StealthRock_CheckToEncourage

Expert_StealthRock_CheckFlameBody:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_StealthRock_CheckToEncourage

Expert_StealthRock_CheckStatic:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_ELECTRIC, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfLoadedEqualTo ABILITY_QUICK_FEET, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_StealthRock_CheckToEncourage

Expert_StealthRock_CheckFreshMilk:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus3
    IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus3
    IfLoadedEqualTo ABILITY_OBLIVIOUS, ScoreMinus3
    LoadGender AI_BATTLER_ATTACKER
    IfLoadedEqualTo GENDER_MALE, Expert_StealthRock_CheckFreshMilk_BothMale
    IfLoadedEqualTo GENDER_FEMALE, Expert_StealthRock_CheckFreshMilk_BothFemale
    GoTo Expert_StealthRock_CheckToEncourage

Expert_StealthRock_CheckFreshMilk_BothMale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_MALE, ScoreMinus3
    GoTo Expert_StealthRock_CheckToEncourage

Expert_StealthRock_CheckFreshMilk_BothFemale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_FEMALE, ScoreMinus3
    GoTo Expert_StealthRock_CheckToEncourage

StealthRock_AbilityPunish_RapidSpin:
    TableEntry ABILITY_EFFECT_SPORE
    TableEntry ABILITY_ROUGH_SKIN
    TableEntry ABILITY_STATIC
    TableEntry ABILITY_POISON_POINT
    TableEntry ABILITY_FLAME_BODY
    TableEntry ABILITY_FRESH_MILK
	TableEntry ABILITY_STEADFAST
    TableEntry TABLE_END

Expert_StealthRock_CheckDefog:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedInTable StealthRock_AbilityPunish_Defog, Expert_StealthRock_CheckToEncourage
    AddToMoveScore -8
    GoTo Expert_StealthRock_End

StealthRock_AbilityPunish_Defog:
    TableEntry ABILITY_DEFIANT
    TableEntry ABILITY_COMPETITIVE
    TableEntry TABLE_END

Expert_StealthRock_TryScorePlus1:
    IfRandomLessThan 85, Expert_StealthRock_End
    AddToMoveScore 1
    GoTo Expert_StealthRock_End

Expert_StealthRock_TryScorePlus4:
    IfRandomLessThan 25, Expert_StealthRock_End
    AddToMoveScore 4
    GoTo Expert_StealthRock_End

Expert_StealthRock_TryScoreMinus1:
    IfRandomLessThan 64, Expert_StealthRock_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_StealthRock_End

Expert_StealthRock_End: 
    PopOrEnd

Expert_Spikes:
    ; Don''t use if defender is on their last mon.
    ;
    ; Check if defender knows Rapid Spin, Defog, or Taunt.
    ;
    ; If Rapid Spin, check if attacker has a move that punishes on contact.
    ; If attacker does have move that punishes on contact, try to click spikes into a spinner.
    ;
    ; Similar for Defog, but with Defiant / Competitive
    ;
    ; If defender knows Taunt, try spikes 25% of the time, otherwise score -1.
    ;
    ; Try spikes has 50% chance to default to +1 score if Stealth Rocks is not already up.
    ;
    ; 90% chance to add +score during the first 3 turns of a battle.
    ; The +score here depends on number of spikes currently up, with first and last layers
    ; valued highest.
    ;
    ; If the attacker knows either of the moves Roar, Whirlwind or Dragon Tail (Swift), 33% chance of additional score +1.
    ;
    ; If the attacker has just switched in, 33% chance of additional score +1.

    LoadSpikesLayers AI_BATTLER_DEFENDER, SIDE_CONDITION_SPIKES
    IfLoadedEqualTo 3, ScoreMinus10
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedLessThan 2, ScoreMinus10
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus10
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_RAPID_SPIN, Expert_Spikes_CheckRapidSpin
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_DEFOG, Expert_Spikes_CheckDefog
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_TAUNT, Expert_Spikes_TryScoreMinus1
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, Expert_Spikes_CheckToEncourage
    IfRandomLessThan 64, Expert_Spikes_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_Spikes_CheckToEncourage

Expert_Spikes_CheckRapidSpin:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, Expert_Spikes_CheckItem
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, Expert_Spikes_CheckItem
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_ABILITY_SUPPRESSED, ScoreMinus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Spikes_AbilityPunish_RapidSpin, Expert_Spikes_CheckAbilityImmunity
    IfRandomLessThan 25, Expert_Spikes_CheckToEncourage
    AddToMoveScore -8
    GoTo Expert_Spikes_End

Expert_Spikes_CheckItem:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NORMAL_HIT_GHOST, ScoreMinus3
    GoTo Expert_Spikes_CheckToEncourage

Expert_Spikes_CheckAbilityImmunity:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus3
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH, ScoreMinus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_POINT, Expert_Spikes_CheckPoisonPoint
    IfLoadedEqualTo ABILITY_FLAME_BODY, Expert_Spikes_CheckFlameBody
    IfLoadedEqualTo ABILITY_STATIC, Expert_Spikes_CheckStatic
    IfLoadedEqualTo ABILITY_FRESH_MILK, Expert_Spikes_CheckFreshMilk
    GoTo Expert_Spikes_CheckToEncourage

Expert_Spikes_CheckPoisonPoint:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus3
    IfLoadedEqualTo TYPE_POISON, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_Spikes_CheckToEncourage

Expert_Spikes_CheckFlameBody:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_Spikes_CheckToEncourage

Expert_Spikes_CheckStatic:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_ELECTRIC, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfLoadedEqualTo ABILITY_QUICK_FEET, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_Spikes_CheckToEncourage

Expert_Spikes_CheckFreshMilk:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus3
    IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus3
    IfLoadedEqualTo ABILITY_OBLIVIOUS, ScoreMinus3
    LoadGender AI_BATTLER_ATTACKER
    IfLoadedEqualTo GENDER_MALE, Expert_Spikes_CheckFreshMilk_BothMale
    IfLoadedEqualTo GENDER_FEMALE, Expert_Spikes_CheckFreshMilk_BothFemale
    GoTo Expert_Spikes_CheckToEncourage

Expert_Spikes_CheckFreshMilk_BothMale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_MALE, ScoreMinus3
    GoTo Expert_Spikes_CheckToEncourage

Expert_Spikes_CheckFreshMilk_BothFemale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_FEMALE, ScoreMinus3
    GoTo Expert_Spikes_CheckToEncourage

Spikes_AbilityPunish_RapidSpin:
    TableEntry ABILITY_EFFECT_SPORE
    TableEntry ABILITY_ROUGH_SKIN
    TableEntry ABILITY_STATIC
    TableEntry ABILITY_POISON_POINT
    TableEntry ABILITY_FLAME_BODY
    TableEntry ABILITY_FRESH_MILK
	TableEntry ABILITY_STEADFAST
    TableEntry TABLE_END

Expert_Spikes_CheckDefog:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Spikes_AbilityPunish_Defog, Expert_Spikes_CheckToEncourage
    AddToMoveScore -8
    GoTo Expert_Spikes_End

Spikes_AbilityPunish_Defog:
    TableEntry ABILITY_COMPETITIVE
    TableEntry ABILITY_DEFIANT
    TableEntry TABLE_END

Expert_Spikes_CheckToEncourage:
    LoadTurnCount
    IfLoadedLessThan 3, Expert_Spikes_SpikesScore
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_ROAR, Expert_Spikes_TryScorePlus1
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_WHIRLWIND, Expert_Spikes_TryScorePlus1
	IfMoveKnown AI_BATTLER_ATTACKER, MOVE_SWIFT, Expert_Spikes_TryScorePlus1
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_Spikes_TryScorePlus1
    IfRandomLessThan 128, Expert_Spikes_End
    AddToMoveScore 1
    GoTo Expert_Spikes_SpikesScore

Expert_Spikes_TryScorePlus1:
    IfRandomLessThan 85, Expert_Spikes_End
    AddToMoveScore 1
    GoTo Expert_Spikes_End

Expert_Spikes_SpikesScore:
    IfRandomLessThan 25, Expert_Spikes_End
    LoadSpikesLayers AI_BATTLER_DEFENDER, SIDE_CONDITION_SPIKES
    IfLoadedEqualTo 0, ScorePlus3
    IfLoadedEqualTo 1, ScorePlus1
    IfLoadedEqualTo 2, ScorePlus2
    AddToMoveScore -3
    GoTo Expert_Spikes_End

Expert_Spikes_TryScoreMinus1:
    IfRandomLessThan 64, Expert_Spikes_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_Spikes_End
    
Expert_Spikes_End: 
    PopOrEnd

Expert_ToxicSpikes:
    ; Don''t use if defender is on their last mon.
    ;
    ; Check if defender knows Rapid Spin, Defog, or Taunt.
    ;
    ; If Rapid Spin, check if attacker has a move that punishes on contact.
    ; If attacker does have move that punishes on contact, try to click toxic spikes into a spinner.
    ;
    ; Similar for Defog, but with Defiant / Competitive
    ;
    ; If defender knows Taunt, try toxic spikes 25% of the time, otherwise score -1.
    ;
    ; Try toxic spikes has 50% chance to default to +1 score if Stealth Rocks is not already up.
    ;
    ; 90% chance to add +score during the first 3 turns of a battle.
    ; The +score here depends on number of toxic spikes currently up, with first layer valued most.
    ;
    ; If the attacker knows either of the moves Roar, Whirlwind or Dragon Tail (Swift), 33% chance of additional score +1.
    ;
    ; If the attacker has just switched in, 33% chance of additional score +1.

    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedLessThan 2, ScoreMinus10
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus10
    LoadSpikesLayers AI_BATTLER_DEFENDER, SIDE_CONDITION_TOXIC_SPIKES
    IfLoadedEqualTo 2, ScoreMinus10
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_RAPID_SPIN, Expert_ToxicSpikes_CheckRapidSpin
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_DEFOG, Expert_ToxicSpikes_CheckDefog
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_TAUNT, Expert_ToxicSpikes_TryScoreMinus1
    IfToxicSpikesClearerAliveInParty AI_BATTLER_DEFENDER, Expert_ToxicSpikes_TryScoreMinus1
    IfRandomLessThan 64, Expert_ToxicSpikes_CheckToEncourage
    AddToMoveScore 2
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, Expert_StealthRock_CheckToEncourage
    IfRandomLessThan 64, Expert_StealthRock_CheckToEncourage
    AddToMoveScore -2
    GoTo Expert_ToxicSpikes_CheckToEncourage


Expert_ToxicSpikes_CheckRapidSpin:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, Expert_ToxicSpikes_CheckItem
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, Expert_ToxicSpikes_CheckItem
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_ABILITY_SUPPRESSED, ScoreMinus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedInTable Spikes_AbilityPunish_RapidSpin, Expert_ToxicSpikes_CheckAbilityImmunity
    IfRandomLessThan 25, Expert_ToxicSpikes_CheckToEncourage
    AddToMoveScore -8
    GoTo Expert_ToxicSpikes_End

Expert_ToxicSpikes_CheckItem:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NORMAL_HIT_GHOST, ScoreMinus3
    GoTo Expert_ToxicSpikes_CheckToEncourage

Expert_ToxicSpikes_CheckAbilityImmunity:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus3
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH, ScoreMinus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_POINT, Expert_ToxicSpikes_CheckPoisonPoint
    IfLoadedEqualTo ABILITY_FLAME_BODY, Expert_ToxicSpikes_CheckFlameBody
    IfLoadedEqualTo ABILITY_STATIC, Expert_ToxicSpikes_CheckStatic
    IfLoadedEqualTo ABILITY_FRESH_MILK, Expert_ToxicSpikes_CheckFreshMilk
    GoTo Expert_ToxicSpikes_CheckToEncourage

Expert_ToxicSpikes_CheckPoisonPoint:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus3
    IfLoadedEqualTo TYPE_POISON, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_ToxicSpikes_CheckToEncourage

Expert_ToxicSpikes_CheckFlameBody:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_ToxicSpikes_CheckToEncourage

Expert_ToxicSpikes_CheckStatic:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_ELECTRIC, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfLoadedEqualTo ABILITY_QUICK_FEET, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_ToxicSpikes_CheckToEncourage

Expert_ToxicSpikes_CheckFreshMilk:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus3
    IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus3
    IfLoadedEqualTo ABILITY_OBLIVIOUS, ScoreMinus3
    LoadGender AI_BATTLER_ATTACKER
    IfLoadedEqualTo GENDER_MALE, Expert_ToxicSpikes_CheckFreshMilk_BothMale
    IfLoadedEqualTo GENDER_FEMALE, Expert_ToxicSpikes_CheckFreshMilk_BothFemale
    GoTo Expert_ToxicSpikes_CheckToEncourage

Expert_ToxicSpikes_CheckFreshMilk_BothMale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_MALE, ScoreMinus3
    GoTo Expert_ToxicSpikes_CheckToEncourage

Expert_ToxicSpikes_CheckFreshMilk_BothFemale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_FEMALE, ScoreMinus3
    GoTo Expert_ToxicSpikes_CheckToEncourage

ToxicSpikes_AbilityPunish_RapidSpin:
    TableEntry ABILITY_EFFECT_SPORE
    TableEntry ABILITY_ROUGH_SKIN
    TableEntry ABILITY_STATIC
    TableEntry ABILITY_POISON_POINT
    TableEntry ABILITY_FLAME_BODY
    TableEntry ABILITY_FRESH_MILK
    TableEntry TABLE_END

Expert_ToxicSpikes_CheckDefog:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedInTable ToxicSpikes_AbilityPunish_Defog, Expert_ToxicSpikes_CheckToEncourage
    AddToMoveScore -8
    GoTo Expert_ToxicSpikes_End

ToxicSpikes_AbilityPunish_Defog:
    TableEntry ABILITY_COMPETITIVE
    TableEntry ABILITY_DEFIANT
    TableEntry TABLE_END

Expert_ToxicSpikes_CheckToEncourage:
    LoadTurnCount
    IfLoadedLessThan 3, Expert_ToxicSpikes_ToxicSpikesScore
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_ROAR, Expert_ToxicSpikes_TryScorePlus1
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_WHIRLWIND, Expert_ToxicSpikes_TryScorePlus1
	IfMoveKnown AI_BATTLER_ATTACKER, MOVE_SWIFT, Expert_ToxicSpikes_TryScorePlus1
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_ToxicSpikes_TryScorePlus1
    IfRandomLessThan 128, Expert_ToxicSpikes_End
    AddToMoveScore 1
    GoTo Expert_ToxicSpikes_ToxicSpikesScore

Expert_ToxicSpikes_TryScorePlus1:
    IfRandomLessThan 85, Expert_ToxicSpikes_ToxicSpikesScore
    AddToMoveScore 1
    GoTo Expert_ToxicSpikes_ToxicSpikesScore

Expert_ToxicSpikes_ToxicSpikesScore:
    IfRandomLessThan 25, Expert_ToxicSpikes_End
    LoadSpikesLayers AI_BATTLER_DEFENDER, SIDE_CONDITION_TOXIC_SPIKES
    IfLoadedEqualTo 0, ScorePlus3
    IfLoadedEqualTo 1, ScorePlus1
    AddToMoveScore -3
    GoTo Expert_ToxicSpikes_End

Expert_ToxicSpikes_TryScoreMinus1:
    IfRandomLessThan 64, Expert_ToxicSpikes_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_ToxicSpikes_End
    
Expert_ToxicSpikes_End:
    PopOrEnd
	
Expert_StickyWeb:
    ; Don''t use if defender is on their last mon.
    ;
    ; Check if defender knows Rapid Spin, Defog, or Taunt.
    ;
    ; If Rapid Spin, check if attacker has a move that punishes on contact.
    ; If attacker does have move that punishes on contact, try to click rocks into a spinner.
    ;
    ; Similar for Defog, but with Defiant / Competitive
    ;
    ; If defender knows Taunt, try rocks 25% of the time, otherwise score -1.
    ;
    ; Try rocks has 50% chance to default to +1 score if Stealth Rocks is not already up.
    ;
    ; 90% chance to add +4 score during the first 3 turns of a battle.
    ;
    ; If the attacker knows either of the moves Roar, Whirlwind or Dragon Tail (Swift), 33% chance of additional score +1.
    ;
    ; If the attacker has just switched in, 33% chance of additional score +1.

    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedEqualTo 0, ScoreMinus10
    IfSideCondition AI_BATTLER_DEFENDER, SIDE_CONDITION_STICKY_WEB, ScoreMinus10
	LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus10
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_RAPID_SPIN, Expert_StickyWeb_CheckRapidSpin
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_DEFOG, Expert_StickyWeb_CheckDefog
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_TAUNT, Expert_StickyWeb_TryScoreMinus1
    CountAlivePartyBattlers AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 1, Expert_StickyWeb_CheckToEncourage
    IfRandomLessThan 64, Expert_StickyWeb_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_StickyWeb_CheckToEncourage

Expert_StickyWeb_CheckToEncourage:
    LoadTurnCount
    IfLoadedLessThan 3, Expert_StickyWeb_TryScorePlus4
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_ROAR, Expert_StickyWeb_TryScorePlus1
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_WHIRLWIND, Expert_StickyWeb_TryScorePlus1
	IfMoveKnown AI_BATTLER_ATTACKER, MOVE_SWIFT, Expert_StickyWeb_TryScorePlus1
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_StickyWeb_TryScorePlus1
    IfRandomLessThan 128, Expert_StickyWeb_End
    AddToMoveScore 1
    GoTo Expert_StickyWeb_End

Expert_StickyWeb_CheckRapidSpin:
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, Expert_StickyWeb_CheckItem
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, Expert_StickyWeb_CheckItem
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_ABILITY_SUPPRESSED, ScoreMinus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedInTable StickyWeb_AbilityPunish_RapidSpin, Expert_StickyWeb_CheckAbilityImmunity
    IfRandomLessThan 25, Expert_StickyWeb_CheckToEncourage
    AddToMoveScore -8
    GoTo Expert_StealthRock_End

Expert_StickyWeb_CheckItem:
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NORMAL_HIT_GHOST, ScoreMinus3
    GoTo Expert_StickyWeb_CheckToEncourage

Expert_StickyWeb_CheckAbilityImmunity:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, ScoreMinus3
    LoadHeldItemEffect AI_BATTLER_DEFENDER
    IfLoadedEqualTo HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH, ScoreMinus3
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_POISON_POINT, Expert_StickyWeb_CheckPoisonPoint
    IfLoadedEqualTo ABILITY_FLAME_BODY, Expert_StickyWeb_CheckFlameBody
    IfLoadedEqualTo ABILITY_STATIC, Expert_StickyWeb_CheckStatic
    IfLoadedEqualTo ABILITY_FRESH_MILK, Expert_StickyWeb_CheckFreshMilk
    GoTo Expert_StickyWeb_CheckToEncourage

Expert_StickyWeb_CheckPoisonPoint:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, ScoreMinus3
    IfLoadedEqualTo TYPE_POISON, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_POISON_HEAL, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfLoadedEqualTo ABILITY_IMMUNITY, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_StickyWeb_CheckToEncourage

Expert_StickyWeb_CheckFlameBody:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_StickyWeb_CheckToEncourage

Expert_StickyWeb_CheckStatic:
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_ELECTRIC, ScoreMinus3
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_GUTS, ScoreMinus3
    IfLoadedEqualTo ABILITY_SHED_SKIN, ScoreMinus3
    IfLoadedEqualTo ABILITY_QUICK_FEET, ScoreMinus3
    IfMoveKnown AI_BATTLER_DEFENDER, MOVE_FACADE, ScoreMinus3
    GoTo Expert_StickyWeb_CheckToEncourage

Expert_StickyWeb_CheckFreshMilk:
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_DEFIANT, ScoreMinus3
    IfLoadedEqualTo ABILITY_COMPETITIVE, ScoreMinus3
    IfLoadedEqualTo ABILITY_OBLIVIOUS, ScoreMinus3
    LoadGender AI_BATTLER_ATTACKER
    IfLoadedEqualTo GENDER_MALE, Expert_StickyWeb_CheckFreshMilk_BothMale
    IfLoadedEqualTo GENDER_FEMALE, Expert_StickyWeb_CheckFreshMilk_BothFemale
    GoTo Expert_StickyWeb_CheckToEncourage

Expert_StickyWeb_CheckFreshMilk_BothMale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_MALE, ScoreMinus3
    GoTo Expert_StickyWeb_CheckToEncourage

Expert_StickyWeb_CheckFreshMilk_BothFemale:
    LoadGender AI_BATTLER_DEFENDER
    IfLoadedEqualTo GENDER_FEMALE, ScoreMinus3
    GoTo Expert_StickyWeb_CheckToEncourage

StickyWeb_AbilityPunish_RapidSpin:
    TableEntry ABILITY_EFFECT_SPORE
    TableEntry ABILITY_ROUGH_SKIN
    TableEntry ABILITY_STATIC
    TableEntry ABILITY_POISON_POINT
    TableEntry ABILITY_FLAME_BODY
    TableEntry ABILITY_FRESH_MILK
	TableEntry ABILITY_STEADFAST
    TableEntry TABLE_END

Expert_StickyWeb_CheckDefog:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedInTable StickyWeb_AbilityPunish_Defog, Expert_StickyWeb_CheckToEncourage
    AddToMoveScore -8
    GoTo Expert_StickyWeb_End

StickyWeb_AbilityPunish_Defog:
    TableEntry ABILITY_DEFIANT
    TableEntry ABILITY_COMPETITIVE
    TableEntry TABLE_END

Expert_StickyWeb_TryScorePlus1:
    IfRandomLessThan 85, Expert_StickyWeb_End
    AddToMoveScore 1
    GoTo Expert_StickyWeb_End

Expert_StickyWeb_TryScorePlus4:
    IfRandomLessThan 25, Expert_StickyWeb_End
    AddToMoveScore 4
    GoTo Expert_StickyWeb_End

Expert_StickyWeb_TryScoreMinus1:
    IfRandomLessThan 64, Expert_StickyWeb_CheckToEncourage
    AddToMoveScore -1
    GoTo Expert_StickyWeb_End

Expert_StickyWeb_End: 
    PopOrEnd

Expert_RecoilMove:
    ; If the opponent resists or is immune to the move, ignore all further score modifiers.
    ;
    ; If the attacker has either of the abilities Rock Head or Magic Guard, score +1.
    IfMoveEffectivenessEquals TYPE_MULTI_IMMUNE, Expert_RecoilMove_End
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, Expert_RecoilMove_End
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, Expert_RecoilMove_End
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_ROCK_HEAD, Expert_RecoilMove_ScorePlus1
	IfLoadedEqualTo ABILITY_ROCK_SOLID, Expert_RecoilMove_ScorePlus1
	IfLoadedEqualTo ABILITY_HOTHEADED, Expert_RecoilMove_ScorePlus1
    IfLoadedEqualTo ABILITY_MAGIC_GUARD, Expert_RecoilMove_ScorePlus1
    GoTo Expert_RecoilMove_End

Expert_RecoilMove_ScorePlus1:
    AddToMoveScore 1

Expert_RecoilMove_End:
    PopOrEnd 

Expert_HealingWish:
    ; If the attacker''s HP >= 80% and the attacker is faster than its opponent, 25% of score -5.
    ;
    ; If the attacker''s HP > 50%, 80.5% chance of score -1.
    ;
    ; 75% chance to ignore this section of modifiers:
    ; - Start at score +1.
    ; - If the attacker does not have a super-effective move against its opponent, 25% chance of
    ; additional score +1.
    ; - If a party member deals more damage than the attacker, 50% chance of additional score +1.
    ;
    ; If the attacker''s HP <= 30%, 50% chance of score +1.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 80, Expert_HealingWish_HappyPath
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_HealingWish_HappyPath
    IfRandomLessThan 192, Expert_HealingWish_End
    GoTo ScoreMinus5

Expert_HealingWish_HappyPath:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 50, Expert_HealingWish_TryScoreMinus1
    IfRandomLessThan 192, Expert_HealingWish_CheckUserAtLowHP
    AddToMoveScore 1
    IfHasSuperEffectiveMove Expert_HealingWish_CheckPartyMemberDamage
    IfRandomLessThan 192, Expert_HealingWish_CheckPartyMemberDamage
    AddToMoveScore 1

Expert_HealingWish_CheckPartyMemberDamage:
    IfPartyMemberDealsMoreDamage USE_MAX_DAMAGE, Expert_HealingWish_TryScorePlus1
    GoTo Expert_HealingWish_CheckUserAtLowHP

Expert_HealingWish_TryScorePlus1:
    IfRandomLessThan 128, Expert_HealingWish_CheckUserAtLowHP
    AddToMoveScore 1

Expert_HealingWish_CheckUserAtLowHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 30, Expert_HealingWish_End
    IfRandomLessThan 128, Expert_HealingWish_End
    AddToMoveScore 1
    GoTo Expert_HealingWish_End

Expert_HealingWish_TryScoreMinus1:
    IfRandomLessThan 50, Expert_HealingWish_End
    AddToMoveScore -1

Expert_HealingWish_End:
    PopOrEnd 
	
Expert_CheckGrowth:
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_HAILING, Expert_CheckGrowth_ScoreMinus2
    IfLoadedEqualTo AI_WEATHER_RAINING, Expert_CheckGrowth_ScoreMinus2
    IfLoadedEqualTo AI_WEATHER_SANDSTORM, Expert_CheckGrowth_ScoreMinus2
    GoTo Expert_StatusSpAttackUp

Expert_CheckGrowth_ScoreMinus2:
    AddToMoveScore -2
	
Expert_LovelyPunch:
    ; If the target cannot be Infatuated for any reason, score -2.
    IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_ATTRACT, ScoreMinus2
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_OBLIVIOUS, ScoreMinus2
    LoadGender AI_BATTLER_ATTACKER
    IfLoadedEqualTo GENDER_MALE, Expert_CheckCannotAttract_BothMale
    IfLoadedEqualTo GENDER_FEMALE, Expert_CheckCannotAttract_BothFemale
    GoTo ScoreMinus2

Expert_FuryCutter:
    ; Cancel chain if highly resisted, otherwise try to maintain chain
    ; Extra chance to start a chain on 4x damage
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, ScoreMinus1
    LoadBattlerPreviousMove AI_BATTLER_ATTACKER
    LoadEffectOfLoadedMove
    IfLoadedEqualTo BATTLE_EFFECT_DOUBLE_POWER_EACH_TURN, Expert_FuryCutterChanceForStreak
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, Expert_FuryCutterChanceForPlus1
    GoTo Expert_FuryCutterEnd

Expert_FuryCutterChanceForStreak:
    IfRandomLessThan 25, Expert_FuryCutterEnd
    AddToMoveScore 12
    GoTo Expert_FuryCutterEnd

Expert_FuryCutterChanceForPlus1:
    IfRandomLessThan 64, Expert_FuryCutterEnd
    AddToMoveScore 1
    GoTo Expert_FuryCutterEnd

Expert_FuryCutterEnd:
    PopOrEnd


Expert_ChargeBeam:
    ;
    ; If current Sp. Atk stage is less than +1, 75% chance for score +2
    ; If current Sp. Atk stage is less than +2, 50% chance for score +1
    ; 87.5% chance for score -1 if HP is less than 2/3
    ; Be a little greedier for Charge Beam if we have Simple ability
    ;
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SIMPLE, Expert_ChargeBeam_Simple
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 7, Expert_ChargeBeam_TryScorePlus2
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 8, Expert_ChargeBeam_TryScorePlus1
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 11, ScoreMinus1
    GoTo Expert_ChargeBeam_CheckHP

Expert_ChargeBeam_Simple:
    IfStatStageLessThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 10, Expert_ChargeBeam_TryScorePlus2
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 10, ScoreMinus1
    GoTo Expert_ChargeBeam_CheckHP
    

Expert_ChargeBeam_TryScorePlus2:
    IfRandomLessThan 64, Expert_ChargeBeam_CheckHP
    AddToMoveScore 2
    GoTo Expert_ChargeBeam_CheckHP

Expert_ChargeBeam_TryScorePlus1:
    IfRandomLessThan 128, Expert_ChargeBeam_CheckHP

Expert_ChargeBeam_CheckHP:
    IfRandomLessThan 32, Expert_ChargeBeam_End
    AddToMoveScore 1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 67, ScoreMinus1
    GoTo Expert_ChargeBeam_End

Expert_ChargeBeam_End:
    PopOrEnd

Expert_WeightMove:
    ; If target''s weight is greater than 200 pounds, 75% chance for +1 score
    ; and 56.25% chance for +2 score.
    ; If target''s weight is less than 50 pounds, 93.75% chance for -1 score.
    LoadWeight AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 2000, Expert_WeightMove_TryScorePlus2
    IfLoadedLessThan 500, Expert_WeightMove_TryScoreMinus1
    GoTo Expert_WeightMove_End

Expert_WeightMove_TryScorePlus2:
    IfRandomLessThan 64, Expert_WeightMove_End
    AddToMoveScore 1
    IfRandomLessThan 64, Expert_WeightMove_End
    AddToMoveScore 1
    GoTo Expert_WeightMove_End

Expert_WeightMove_TryScoreMinus1:
    IfRandomLessThan 16, Expert_WeightMove_End
    AddToMoveScore -1
    GoTo Expert_WeightMove_End

Expert_WeightMove_End:
    PopOrEnd

Expert_Wish:
    ; If wish is already active, score minus 12 and end
    ;
    ; If we''re slower, try to use wish slightly earlier
    ; If we have substitute up, try to use wish
    ;
    ; If we have a move that synergizes with wish, such as
    ; a pivot move or protect, 80% chance for score +1
    ;
    ; If our HP is low and we''re slower, 80% chance for
    ; score -1
    IfWishActive AI_BATTLER_ATTACKER, ScoreMinus20
    IfRandomLessThan 16, Expert_Wish_CheckMoves
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, ScorePlus3
    IfSpeedCompareEqualTo COMPARE_SPEED_SLOWER, Expert_Wish_Speedslower
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 80, Expert_Wish_CheckMoves
    AddToMoveScore 1
    GoTo Expert_Wish_CheckMoves

Expert_Wish_Speedslower:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 20, Expert_Wish_TryScoreMinus1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 80, ScorePlus1
    IfRandomLessThan 154, Expert_Wish_CheckMoves
    AddToMoveScore 1
    GoTo Expert_Wish_CheckMoves

Expert_Wish_CheckMoves:
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_HIT_BEFORE_SWITCH, Expert_Wish_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PROTECT, Expert_Wish_TryScorePlus1
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_FLEE_FROM_WILD_BATTLE, Expert_Wish_TryScorePlus1
    GoTo Expert_Wish_End

Expert_Wish_TryScoreMinus1:
    IfRandomLessThan 200, Expert_Wish_End
    AddToMoveScore -1
    GoTo Expert_Wish_End

Expert_Wish_TryScorePlus1:
    IfRandomLessThan 50, Expert_Wish_End
    AddToMoveScore 1
    GoTo Expert_Wish_End

Expert_Wish_End:
    PopOrEnd
    
Expert_Taunt:
    IfTargetIsNotTaunted Expert_Taunt_CheckAbility
    GoTo ScoreMinus10

Expert_Taunt_CheckAbility:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Expert_Taunt_CheckFirstTurn
	IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, Expert_Taunt_CheckFirstTurn
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus10
    GoTo Expert_Taunt_CheckFirstTurn

Expert_Taunt_CheckFirstTurn:
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_Taunt_TryScorePlus1
    GoTo Expert_Taunt_ValidateTaunt

Expert_Taunt_TryScorePlus1:
    IfRandomLessThan 32, Expert_Taunt_ValidateTaunt
    AddToMoveScore 1
    GoTo Expert_Taunt_ValidateTaunt

Expert_Taunt_ValidateTaunt:
    IfShouldTaunt AI_BATTLER_DEFENDER, ScorePlus3
    AddToMoveScore -4
    GoTo Expert_Taunt_End

Expert_Taunt_End:
    PopOrEnd

Expert_Stockpile:
    ;   If Spit Up is known and effective, 80% chance for Score +1
    ;   If Swallow is known and effective, 80% chance for Score +1, or +2 if less than 2 stockpiled
    ;   Don''t use this move if they can phaze, taunt, encore, or psych up
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_SPIT_UP, Expert_Stockpile_SpitUpKnown
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_SWALLOW, Expert_Stockpile_SwallowKnown
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_UNAWARE, ScoreMinus12
	LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_AWARE, ScoreMinus12
    LoadBattlerCritStage AI_BATTLER_DEFENDER
    IfLoadedGreaterThan 0, ScoreMinus12
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, Expert_Stockpile_TryScoreMinus3
    LoadStockpileCount AI_BATTLER_ATTACKER
    IfLoadedGreaterThan 1, Expert_Stockpile_CheckMoves
    AddToMoveScore 1
    GoTo Expert_Stockpile_CheckMoves

Expert_Stockpile_SpitUpKnown:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SCRAPPY, Expert_Stockpile_CheckMoves
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo HOLD_EFFECT_NORMAL_HIT_GHOST, Expert_Stockpile_CheckMoves
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GHOST, ScoreMinus12
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GHOST, ScoreMinus12
    IfRandomLessThan 51, Expert_Stockpile_CheckMoves
    AddToMoveScore 1
    GoTo Expert_Stockpile_CheckMoves

Expert_Stockpile_SwallowKnown:
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_HEAL_BLOCK, ScoreMinus12
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_HEAL_INVERSION, ScoreMinus20
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 40, Expert_Stockpile_TryScoreMinus3
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 88, Expert_Stockpile_TryScorePlus1
    IfRandomLessThan 51, Expert_Stockpile_CheckMoves
    AddToMoveScore 1
    LoadStockpileCount AI_BATTLER_ATTACKER
    IfLoadedGreaterThan 1, Expert_Stockpile_CheckMoves
    AddToMoveScore 1
    GoTo Expert_Stockpile_CheckMoves

Expert_Stockpile_TryScoreMinus3:
    IfRandomLessThan 13, Expert_Stockpile_CheckMoves
    AddToMoveScore -3
    GoTo Expert_Stockpile_CheckMoves

Expert_Stockpile_TryScorePlus1:
    IfRandomLessThan 26, Expert_Stockpile_CheckMoves
    AddToMoveScore 1
    GoTo Expert_Stockpile_CheckMoves

Expert_Stockpile_CheckMoves:
    IfCanHazeOrPhaze AI_BATTLER_DEFENDER, ScoreMinus5
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_TAUNT, ScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_ENCORE, ScoreMinus2
    IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_COPY_STAT_CHANGES, ScoreMinus2
    IfRandomLessThan 205, Expert_Stockpile_CheckHP
    AddToMoveScore 1
    GoTo Expert_Stockpile_CheckHP

Expert_Stockpile_CheckHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 88, ScorePlus1
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, ScoreMinus2
    GoTo Expert_Stockpile_End

Expert_Stockpile_End:
    LoadStockpileCount AI_BATTLER_ATTACKER
    IfLoadedEqualTo 3, ScoreMinus20
    PopOrEnd
	
Expert_Torment:
    IfNotVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_TORMENT, Expert_Torment_CheckAbility
    GoTo ScoreMinus10

Expert_Torment_CheckAbility:
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, Expert_Torment_CheckFirstTurn
	IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, Expert_Torment_CheckFirstTurn
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_MAGIC_BOUNCE, ScoreMinus10
    GoTo Expert_Torment_CheckFirstTurn

Expert_Torment_CheckFirstTurn:
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo TRUE, Expert_Torment_TryScorePlus1
    GoTo Expert_Torment_ValidateTorment

Expert_Torment_TryScorePlus1:
    IfRandomLessThan 32, Expert_Torment_ValidateTorment
    AddToMoveScore 1
    GoTo Expert_Torment_ValidateTorment

Expert_Torment_ValidateTorment:
	IfVolatileStatus AI_BATTLER_DEFENDER, VOLATILE_CONDITION_SUBSTITUTE, ScoreMinus10
	IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_SUBSTITUTE, Expert_Torment_TryScorePlus3
	IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PROTECT, ScorePlus2
	IfEnemyCanChunkOrKO Expert_Torment_TryScoreMinus1
	IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING, Expert_Torment_TryScorePlus2
	IfMoveEffectKnown AI_BATTLER_DEFENDER, BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN, Expert_Torment_TryScorePlus2
	GoTo Expert_Torment_End
	
Expert_Torment_TryScoreMinus1:
    IfRandomLessThan 12, Expert_Torment_End
    AddToMoveScore -1
    GoTo Expert_Torment_End
	
Expert_Torment_TryScorePlus3:
	IfHPPercentLessThan AI_BATTLER_DEFENDER, 40, Expert_Torment_End
	AddToMoveScore 1
    IfRandomLessThan 85, Expert_Torment_End
    AddToMoveScore 2
    GoTo Expert_Torment_End
	
Expert_Torment_TryScorePlus2:
    IfRandomLessThan 128, Expert_Torment_End
    AddToMoveScore 2
    GoTo Expert_Torment_End

Expert_Torment_End:
    PopOrEnd
	
Expert_ChumRush:
    IfAbilityInPlay ABILITY_STORM_DRAIN, ScoreMinus12
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SKILL_LINK, Expert_ChumRush_TryScorePlus1
    IfLoadedEqualTo ABILITY_STRONG_JAW, Expert_ChumRush_TryScorePlus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, Expert_ChumRush_SuperEffective
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, Expert_ChumRush_SuperEffective
    LoadHeldItemEffect AI_BATTLER_ATTACKER
    IfLoadedEqualTo HOLD_EFFECT_THREE_FOUR_FIVE_DICE, Expert_ChumRush_TryScorePlus1
    GoTo Expert_ChumRush_CheckDefenderHP

Expert_ChumRush_TryScorePlus1:
    IfRandomLessThan 128, Expert_ChumRush_CheckDefenderHP
    AddToMoveScore 1
    GoTo Expert_ChumRush_CheckDefenderHP

Expert_ChumRush_SuperEffective:
    IfRandomLessThan 64, Expert_ChumRush_CheckDefenderHP
    AddToMoveScore 1
    IfRandomLessThan 64, Expert_ChumRush_CheckDefenderHP
    AddToMoveScore 1
    GoTo Expert_ChumRush_CheckDefenderHP

Expert_ChumRush_CheckDefenderHP:
	IfHPPercentLessThan AI_BATTLER_DEFENDER, 30, Expert_ChumRush_TryScorePlus3
	IfHPPercentLessThan AI_BATTLER_DEFENDER, 60, Expert_ChumRush_TryScorePlus2
	IfHPPercentLessThan AI_BATTLER_DEFENDER, 80, Expert_ChumRush_CheckDefenderDetersContact
    IfRandomLessThan 170, Expert_ChumRush_CheckDefenderDetersContact
	AddToMoveScore -1
	GoTo Expert_ChumRush_CheckDefenderDetersContact
	
Expert_ChumRush_TryScorePlus2:
	AddToMoveScore 1
    IfRandomLessThan 85, Expert_ChumRush_CheckDefenderDetersContact
    AddToMoveScore 1
    GoTo Expert_ChumRush_CheckDefenderDetersContact
	
Expert_ChumRush_TryScorePlus3:
    IfRandomLessThan 8, Expert_ChumRush_CheckDefenderDetersContact
	AddToMoveScore 1
    IfRandomLessThan 32, Expert_ChumRush_CheckDefenderDetersContact
    AddToMoveScore 1
    IfRandomLessThan 64, Expert_ChumRush_CheckDefenderDetersContact
    AddToMoveScore 1
    GoTo Expert_ChumRush_CheckDefenderDetersContact

Expert_ChumRush_CheckDefenderDetersContact:
    IfBattlerDetersContactMove AI_BATTLER_DEFENDER, Try75ChanceForScoreMinus3
    IfRandomLessThan 230, Expert_ChumRush_End
    AddToMoveScore 1
    GoTo Expert_ChumRush_End

Expert_ChumRush_End:
    PopOrEnd

ScoreMinus1:
    AddToMoveScore -1
    PopOrEnd 

ScoreMinus2:
    AddToMoveScore -2
    PopOrEnd 

ScoreMinus3:
    AddToMoveScore -3
    PopOrEnd 

ScoreMinus5:
    AddToMoveScore -5
    PopOrEnd 

ScoreMinus6: ; unused
    AddToMoveScore -6
    PopOrEnd 

ScoreMinus8:
    AddToMoveScore -8
    PopOrEnd 

ScoreMinus10:
    AddToMoveScore -10
    PopOrEnd 

ScoreMinus12:
    AddToMoveScore -12
    PopOrEnd
    
ScoreMinus20:
    AddToMoveScore -20
    PopOrEnd

ScoreMinus30:
    AddToMoveScore -30
    PopOrEnd 

ScorePlus1:
    AddToMoveScore 1
    PopOrEnd 

ScorePlus2:
    AddToMoveScore 2
    PopOrEnd 

ScorePlus3:
    AddToMoveScore 3
    PopOrEnd 

ScorePlus5:
    AddToMoveScore 5
    PopOrEnd 

ScorePlus10:
    AddToMoveScore 10
    PopOrEnd 

Try50ChanceForScorePlus1:
    IfRandomLessThan 128, ScorePlus1
    PopOrEnd

Try66ChanceForScorePlus1:
    IfRandomLessThan 170, ScorePlus1
    PopOrEnd

Try90ChanceForScorePlus1:
    IfRandomLessThan 230, ScorePlus1
    PopOrEnd

Try95ChanceForScorePlus1:
    IfRandomLessThan 244, ScorePlus1
    PopOrEnd

Try75ChanceForScorePlus3:
    IfRandomLessThan 192, ScorePlus3
    PopOrEnd

Try95ChanceForScorePlus3:
    IfRandomLessThan 244, ScorePlus3
    PopOrEnd

Try95ChanceForScorePlus5:
    IfRandomLessThan 244, ScorePlus5
    PopOrEnd

Try50ChanceForScoreMinus1:
    IfRandomLessThan 128, ScoreMinus1
    PopOrEnd

Try95ChanceForScoreMinus1:
    IfRandomLessThan 244, ScoreMinus1
    PopOrEnd

Try99ChanceForScoreMinus1:
    IfRandomLessThan 254, ScoreMinus1
    PopOrEnd

Try50ChanceForScoreMinus3:
    IfRandomLessThan 128, ScoreMinus3
    PopOrEnd

Try75ChanceForScoreMinus3:
    IfRandomLessThan 192, ScoreMinus3
    PopOrEnd

Try90ChanceForScoreMinus12:
    IfRandomLessThan 230, ScoreMinus12
    PopOrEnd

Try95ChanceForScoreMinus12:
    IfRandomLessThan 244, ScoreMinus12
    PopOrEnd

Try99ChanceForScoreMinus12:
    IfRandomLessThan 254, ScoreMinus12
    PopOrEnd

EvalAttack_Main:
    ; Never target the partner.
    IfTargetIsPartner Terminate
 
    IfCurrentMoveKills ROLL_FOR_DAMAGE, EvalAttack_CheckForKill

    ; If this move does not out-damage all other moves, score -1.
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NOT_HIGHEST_DAMAGE, Try95ChanceForScoreMinus1

    ; Explosion, Focus Punch, and Sucker Punch are judged as Risky by this routine.
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_DEFENSE, EvalAttack_RiskyAttack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT, EvalAttack_RiskyAttack
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING, EvalAttack_RiskyAttack

    ; Check for quad-effectiveness.
    GoTo EvalAttack_CheckQuadEffective

EvalAttack_RiskyAttack:
    ; 50% chance of score -2.
    IfRandomLessThan 128, EvalAttack_CheckQuadEffective
    AddToMoveScore -2

EvalAttack_CheckQuadEffective:
    ; If quad-effective, 31.25% chance of score +2.
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, EvalAttack_TryScorePlus2
    PopOrEnd 

EvalAttack_TryScorePlus2:
    IfRandomLessThan 80, EvalAttack_Terminate
    AddToMoveScore 2
    PopOrEnd 

EvalAttack_CheckForKill:
    ; Do not evaluate kills with Explosion or Self-Destruct for this routine.
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_DEFENSE, EvalAttack_Terminate

    ; Inaccurate moves will receive 2 points less score
    LoadMoveAccuracy
    IfLoadedLessThan 100, EvalAttack_ScoreMinus2_Inaccurate

    ; Randomly increase the score of a move that kills.
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT, EvalAttack_TryScorePlus4
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING, EvalAttack_TryScorePlus4
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HIT_IN_3_TURNS, EvalAttack_TryScorePlus4

    ; Priority kill is score +2. This is because priorty moves are low-power, and this routine prioritizes
    ; raw damage output.
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PRIORITY_1, EvalAttack_ScorePlus2
    GoTo EvalAttack_ScorePlus4

EvalAttack_TryScorePlus4:
    ; ~33.6% of the time, score +4.
    IfRandomLessThan 170, EvalAttack_Terminate
    GoTo EvalAttack_ScorePlus4

EvalAttack_ScorePlus2:
    AddToMoveScore 2

EvalAttack_ScorePlus4:
    AddToMoveScore 4
    GoTo EvalAttack_Terminate

EvalAttack_ScoreMinus2_Inaccurate:
    IfRandomLessThan 32, EvalAttack_Terminate
    AddToMoveScore -2
    GoTo EvalAttack_Terminate

EvalAttack_Terminate:
    PopOrEnd 

SetupFirstTurn_Main:
    IfTargetIsPartner Terminate

    ; If this is not the first turn, terminate.
    LoadTurnCount 
    IfLoadedNotEqualTo 0, SetupFirstTurn_Terminate

    ; If the current move''s effect is not known tobe a setup move, break.
    LoadCurrentMoveEffect 
    IfLoadedNotInTable SetupFirstTurn_SetupEffects, SetupFirstTurn_Terminate

    ; 68.75% of the time, score +2.
    IfRandomLessThan 80, SetupFirstTurn_Terminate
    AddToMoveScore 2

SetupFirstTurn_Terminate:
    PopOrEnd 

SetupFirstTurn_SetupEffects:
    TableEntry BATTLE_EFFECT_ATK_UP
    TableEntry BATTLE_EFFECT_DEF_UP
    TableEntry BATTLE_EFFECT_SPEED_UP
    TableEntry BATTLE_EFFECT_SP_ATK_UP
    TableEntry BATTLE_EFFECT_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ACC_UP
    TableEntry BATTLE_EFFECT_EVA_UP
    TableEntry BATTLE_EFFECT_ATK_DOWN
    TableEntry BATTLE_EFFECT_DEF_DOWN
    TableEntry BATTLE_EFFECT_SPEED_DOWN
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN
    TableEntry BATTLE_EFFECT_ACC_DOWN
    TableEntry BATTLE_EFFECT_EVA_DOWN
    TableEntry BATTLE_EFFECT_CONVERSION
    TableEntry BATTLE_EFFECT_SET_LIGHT_SCREEN
    TableEntry BATTLE_EFFECT_SP_DEF_UP_2
    TableEntry BATTLE_EFFECT_CRIT_UP_2
    TableEntry BATTLE_EFFECT_STATUS_CONFUSE
    TableEntry BATTLE_EFFECT_ATK_UP_2
    TableEntry BATTLE_EFFECT_DEF_UP_2
    TableEntry BATTLE_EFFECT_SPEED_UP_2
    TableEntry BATTLE_EFFECT_SP_ATK_UP_2
    TableEntry BATTLE_EFFECT_SP_DEF_UP_2
    TableEntry BATTLE_EFFECT_ACC_UP_2
    TableEntry BATTLE_EFFECT_EVA_UP_2
    TableEntry BATTLE_EFFECT_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_SPEED_DOWN_2
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_EVA_DOWN_2
    TableEntry BATTLE_EFFECT_ACC_DOWN_2
    TableEntry BATTLE_EFFECT_SET_REFLECT
    TableEntry BATTLE_EFFECT_STATUS_POISON
    TableEntry BATTLE_EFFECT_STATUS_PARALYZE
    TableEntry BATTLE_EFFECT_SET_SUBSTITUTE
    TableEntry BATTLE_EFFECT_STATUS_LEECH_SEED
    TableEntry BATTLE_EFFECT_EVA_UP_2_MINIMIZE
    TableEntry BATTLE_EFFECT_CURSE
    TableEntry BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION
    TableEntry BATTLE_EFFECT_CAMOUFLAGE
    TableEntry BATTLE_EFFECT_STATUS_SLEEP_NEXT_TURN
    TableEntry BATTLE_EFFECT_DEF_UP_DOUBLE_ROLLOUT_POWER
    TableEntry BATTLE_EFFECT_TORMENT
    TableEntry BATTLE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION
    TableEntry BATTLE_EFFECT_STATUS_BURN
    TableEntry BATTLE_EFFECT_GROUND_TRAP_USER_CONTINUOUS_HEAL
    TableEntry BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE
    TableEntry BATTLE_EFFECT_CONFUSE_ALL
    TableEntry BATTLE_EFFECT_ATK_DEF_DOWN
    TableEntry BATTLE_EFFECT_DEF_SPD_UP
    TableEntry BATTLE_EFFECT_ATK_DEF_UP
    TableEntry BATTLE_EFFECT_SP_ATK_SP_DEF_UP
    TableEntry BATTLE_EFFECT_CAMOUFLAGE
    TableEntry BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS
    TableEntry BATTLE_EFFECT_RANDOM_STAT_UP_2
    TableEntry BATTLE_EFFECT_PREVENT_CRITS
    TableEntry BATTLE_EFFECT_GIVE_GROUND_IMMUNITY
    TableEntry BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN
    TableEntry BATTLE_EFFECT_WHIRLPOOL
    TableEntry BATTLE_EFFECT_STEALTH_ROCK
    TableEntry BATTLE_EFFECT_SET_SPIKES
    TableEntry BATTLE_EFFECT_TOXIC_SPIKES
    TableEntry TABLE_END

DamagePriority_Main:
    ; Do not target your partner.
    IfTargetIsPartner Terminate

    ; If the current move is not variable power or is Risky, break.
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedNotEqualTo AI_NO_COMPARISON_MADE, DamagePriority_Terminate

    ; ~61% of the time, score +2.
    IfRandomLessThan 100, DamagePriority_Terminate
    AddToMoveScore 2

DamagePriority_Terminate:
    PopOrEnd 

Risky_Main:
    ; Do not target your partner.
    IfTargetIsPartner Terminate

    ; If the current move effect is judged to not be Risky, break;
    LoadCurrentMoveEffect 
    IfLoadedNotInTable Risky_RiskyEffects, Risky_Terminate

    ; 50% of the time, score +2.
    IfRandomLessThan 128, Risky_Terminate
    AddToMoveScore 2

Risky_Terminate:
    PopOrEnd 

Risky_RiskyEffects:
    TableEntry BATTLE_EFFECT_STATUS_SLEEP
    TableEntry BATTLE_EFFECT_HALVE_DEFENSE
    TableEntry BATTLE_EFFECT_COPY_MOVE
    TableEntry BATTLE_EFFECT_ONE_HIT_KO
    TableEntry BATTLE_EFFECT_HIGH_CRITICAL
    TableEntry BATTLE_EFFECT_STATUS_CONFUSE
    TableEntry BATTLE_EFFECT_CALL_RANDOM_MOVE
    TableEntry BATTLE_EFFECT_RANDOM_DAMAGE_1_TO_150_LEVEL
    TableEntry BATTLE_EFFECT_COUNTER
    TableEntry BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER
    TableEntry BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION
    TableEntry BATTLE_EFFECT_INFATUATE
    TableEntry BATTLE_EFFECT_RANDOM_POWER_MAYBE_HEAL
    TableEntry BATTLE_EFFECT_RAISE_ALL_STATS_HIT
    TableEntry BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP
    TableEntry BATTLE_EFFECT_MIRROR_COAT
    TableEntry BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT
    TableEntry BATTLE_EFFECT_DOUBLE_POWER_IF_HIT
    TableEntry BATTLE_EFFECT_CONFUSE_ALL
    TableEntry BATTLE_EFFECT_POWER_BASED_ON_LOW_SPEED
    TableEntry BATTLE_EFFECT_RANDOM_STAT_UP_2
    TableEntry BATTLE_EFFECT_METAL_BURST
    TableEntry BATTLE_EFFECT_DOUBLE_POWER_IF_MOVING_SECOND
    TableEntry BATTLE_EFFECT_USE_MOVE_FIRST
    TableEntry BATTLE_EFFECT_HIT_FIRST_IF_TARGET_ATTACKING
    TableEntry TABLE_END

BatonPass_Main:
    IfTargetIsPartner Terminate

    ; If there are no other party members alive, break.
    CountAlivePartyBattlers AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, BatonPass_Terminate

    ; If the move deals damage, ignore it for this flag.
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedNotEqualTo AI_NO_COMPARISON_MADE, BatonPass_Terminate

    ; If the attacker does not know Baton Pass, 31.25% chance of no score changes.
    IfMoveEffectKnown AI_BATTLER_ATTACKER, BATTLE_EFFECT_PASS_STATS_AND_STATUS, BatonPass_EvalMove
    IfRandomLessThan 80, Risky_Terminate

BatonPass_EvalMove:
    ; Handle these +2 boosting moves separately.
    IfMoveEqualTo MOVE_SWORDS_DANCE, BatonPass_SetupAtHighHP
    IfMoveEqualTo MOVE_DRAGON_DANCE, BatonPass_SetupAtHighHP
    IfMoveEqualTo MOVE_CALM_MIND, BatonPass_SetupAtHighHP
    IfMoveEqualTo MOVE_NASTY_PLOT, BatonPass_SetupAtHighHP

    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PROTECT, BatonPass_EvalProtect

    IfMoveEqualTo MOVE_BATON_PASS, BatonPass_EvalBatonPass

    ; ~92% of the time, score +3.
    IfRandomLessThan 20, Risky_Terminate
    AddToMoveScore 3

BatonPass_SetupAtHighHP:
    ; On turn 1 of the entire battle, score +5.
    LoadTurnCount 
    IfLoadedEqualTo 0, ScorePlus5

    ; If the attacker is at < 60% HP, score -10.
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 60, ScoreMinus10

    ; Otherwise, score +1.
    GoTo ScorePlus1

BatonPass_EvalProtect:
    ; If the current move''s effect is Protect and the last move that we used
    ; is either Detect or Protect, score -10.
    LoadBattlerPreviousMove AI_BATTLER_ATTACKER
    IfLoadedInTable BatonPass_ProtectDetect, ScoreMinus10

    ; Else, score +2.
    AddToMoveScore 2
    PopOrEnd 

BatonPass_ProtectDetect:
    TableEntry MOVE_PROTECT
    TableEntry MOVE_DETECT
    TableEntry TABLE_END

BatonPass_EvalBatonPass:
    ; On turn 1 of the entire battle, score -10.
    LoadTurnCount 
    IfLoadedEqualTo 0, ScoreMinus10

    ; Score +1 for each positive stat stage for Attack or Special Attack
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 8, ScorePlus3
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 7, ScorePlus2
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_ATTACK, 6, ScorePlus1
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 8, ScorePlus3
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 7, ScorePlus2
    IfStatStageGreaterThan AI_BATTLER_ATTACKER, BATTLE_STAT_SP_ATTACK, 6, ScorePlus1
    PopOrEnd 

BatonPass_Terminate:
    PopOrEnd 

TagStrategy_Main:
    IfTargetIsPartner TagStrategy_Partner

    ; If the move does not deal damage, skip ahead
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, TagStrategy_CheckSpecialScoring

    ; Flat-damage move effects have a special handler; this includes OHKO moves
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ONE_HIT_KO, TagStrategy_ScoreMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_40_DAMAGE_FLAT, TagStrategy_ScoreMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LEVEL_DAMAGE_FLAT, TagStrategy_ScoreMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RANDOM_DAMAGE_1_TO_150_LEVEL, TagStrategy_ScoreMove
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_10_DAMAGE_FLAT, TagStrategy_ScoreMove

    ; If the move is not-very-effective, try to reduce its score
    IfMoveEffectivenessEquals TYPE_MULTI_HALF_DAMAGE, TagStrategy_TryScoreMinus1
    IfMoveEffectivenessEquals TYPE_MULTI_QUARTER_DAMAGE, TagStrategy_TryScoreMinus2

    ; All other moves
    GoTo TagStrategy_ScoreMove

TagStrategy_TryScoreMinus1:
    ; If the maximum roll would kill, do not reduce the score
    IfCurrentMoveKills USE_MAX_DAMAGE, TagStrategy_ScoreMove

    ; If the target is on their last Pokemon, do not reduce the score
    IfHPPercentEqualTo AI_BATTLER_DEFENDER_PARTNER, 0, TagStrategy_ScoreMove

    ; 75% of the time, reduce score by 1
    IfRandomLessThan 64, TagStrategy_ScoreMove
    AddToMoveScore -1
    GoTo TagStrategy_ScoreMove

TagStrategy_TryScoreMinus2:
    ; If the maximum roll would kill, do not reduce the score
    IfCurrentMoveKills USE_MAX_DAMAGE, TagStrategy_ScoreMove

    ; If the target is on their last Pokemon, do not reduce the score
    IfHPPercentEqualTo AI_BATTLER_DEFENDER_PARTNER, 0, TagStrategy_ScoreMove

    ; 75% of the time, reduce score by 2
    IfRandomLessThan 64, TagStrategy_ScoreMove
    AddToMoveScore -2
    GoTo TagStrategy_ScoreMove

TagStrategy_ScoreMove:
    ; If this is not a highest-damage move for the attacking side, handle the move "normally"
    CheckIfHighestDamageWithPartner USE_MAX_DAMAGE
    IfLoadedNotEqualTo AI_MOVE_IS_HIGHEST_DAMAGE, TagStrategy_CheckBeforeScoring

    ; Handle Explosion and Self-Destruct like "normal" moves
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_HALVE_DEFENSE, TagStrategy_CheckSpecialScoring

    ; Sometimes prioritize using priority +1 moves
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PRIORITY_1, TagStrategy_TryScorePlus1

    ; 50% of the time, increase score by 1
    IfRandomLessThan 128, TagStrategy_CheckBeforeScoring
    AddToMoveScore 1

    ; Proceed to "normal" handling
    GoTo TagStrategy_CheckSpecialScoring

TagStrategy_TryScorePlus1:
    ; ~80.5% of the time, increase score by 1
    IfRandomLessThan 50, TagStrategy_CheckBeforeScoring
    AddToMoveScore 1
    GoTo TagStrategy_CheckSpecialScoring

TagStrategy_CheckBeforeScoring:
    ; Flat-damage move effects have a special handler; this includes OHKO moves
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ONE_HIT_KO, TagStrategy_CheckSpecialScoring
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_40_DAMAGE_FLAT, TagStrategy_CheckSpecialScoring
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LEVEL_DAMAGE_FLAT, TagStrategy_CheckSpecialScoring
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RANDOM_DAMAGE_1_TO_150_LEVEL, TagStrategy_CheckSpecialScoring
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_10_DAMAGE_FLAT, TagStrategy_CheckSpecialScoring

    ; If the move is super-effective, try to increase its score
    IfMoveEffectivenessEquals TYPE_MULTI_DOUBLE_DAMAGE, TagStrategy_TryPrioritizingDoubleEffective
    IfMoveEffectivenessEquals TYPE_MULTI_QUADRUPLE_DAMAGE, TagStrategy_TryPrioritizingQuadEffective

    GoTo TagStrategy_CheckSpecialScoring

TagStrategy_TryPrioritizingDoubleEffective:
    ; ~61% of the time, score +1
    IfRandomLessThan 100, TagStrategy_CheckSpecialScoring
    AddToMoveScore 1
    GoTo TagStrategy_CheckSpecialScoring

TagStrategy_TryPrioritizingQuadEffective:
    ; 75% of the time, score +1
    IfRandomLessThan 64, TagStrategy_CheckSpecialScoring
    AddToMoveScore 1
    GoTo TagStrategy_CheckSpecialScoring

TagStrategy_CheckSpecialScoring:
    ; Handle each of these moves with their own routine
    IfMoveEqualTo MOVE_SKILL_SWAP, TagStrategy_SkillSwap
    LoadTypeFrom LOAD_MOVE_TYPE
    IfMoveEqualTo MOVE_EARTHQUAKE, TagStrategy_Earthquake
    IfMoveEqualTo MOVE_MAGNITUDE, TagStrategy_Earthquake
    IfMoveEqualTo MOVE_FUTURE_SIGHT, TagStrategy_FutureSight
    IfMoveEqualTo MOVE_DOOM_DESIRE, TagStrategy_FutureSight
    IfMoveEqualTo MOVE_RAIN_DANCE, TagStrategy_RainDance
    IfMoveEqualTo MOVE_SUNNY_DAY, TagStrategy_SunnyDay
    IfMoveEqualTo MOVE_HAIL, TagStrategy_Hail
    IfMoveEqualTo MOVE_SANDSTORM, TagStrategy_Sandstorm
    IfMoveEqualTo MOVE_GRAVITY, TagStrategy_Gravity
    IfMoveEqualTo MOVE_TRICK_ROOM, TagStrategy_TrickRoom
    IfMoveEqualTo MOVE_FOLLOW_ME, TagStrategy_FollowMe
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_PROTECT, TagStrategy_Protect
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS, TagStrategy_Tailwind
    LoadTypeFrom LOAD_MOVE_TYPE
    IfLoadedEqualTo TYPE_ELECTRIC, TagStrategy_CheckElectricMove
    IfLoadedEqualTo TYPE_FIRE, TagStrategy_CheckFireMove
    IfLoadedEqualTo TYPE_WATER, TagStrategy_CheckWaterMove
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_HELPING_HAND, TagStrategy_PartnerKnowsHelpingHand
    PopOrEnd 

TagStrategy_RainDance:
    ; If the move is Rain Dance, apply modifiers for each of the attacker and partner which meet the
    ; following conditions:
    ;  - The battler has Hydration and is currently statused -> score +2
    ;  - The battler has Dry Skin -> score +2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_HYDRATION, TagStrategy_RainDance_SelfHasHydration
    IfLoadedEqualTo ABILITY_DRY_SKIN, TagStrategy_RainDance_SelfScorePlus2
    GoTo TagStrategy_RainDance_CheckPartner

TagStrategy_RainDance_SelfHasHydration:
    IfNotStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, TagStrategy_RainDance_CheckPartner

TagStrategy_RainDance_SelfScorePlus2:
    AddToMoveScore 2
    GoTo TagStrategy_RainDance_CheckPartner

TagStrategy_RainDance_CheckPartner:
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_HYDRATION
    IfLoadedEqualTo AI_HAVE, TagStrategy_RainDance_PartnerHasHydration
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_DRY_SKIN
    IfLoadedEqualTo AI_HAVE, TagStrategy_RainDance_PartnerScorePlus2
    GoTo TagStrategy_RainDance_End

TagStrategy_RainDance_PartnerHasHydration:
    IfNotStatus AI_BATTLER_ATTACKER_PARTNER, MON_CONDITION_ANY, TagStrategy_RainDance_End

TagStrategy_RainDance_PartnerScorePlus2:
    AddToMoveScore 2
    GoTo TagStrategy_RainDance_End

TagStrategy_RainDance_End:
    PopOrEnd 

TagStrategy_SunnyDay:
    ; If the move is Sunny Day, apply modifiers for each of the attacker and partner which meet the
    ; following conditions:
    ;  - The battler has Leaf Guard, is not currently statused, and is at 30% HP or higher -> score +2
    ;  - The battler has Flower Gift -> score +2
    ;  - The battler has Dry Skin -> score -2
    ;  - The battler has Solar Power and is at 50% HP or higher -> score +1
    ;  - The battler has Solar Power, is at less than 50% HP -> 50% chance of score -2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_LEAF_GUARD, TagStrategy_SunnyDay_SelfHasLeafGuard
    IfLoadedEqualTo ABILITY_FLOWER_GIFT, TagStrategy_SunnyDay_SelfScorePlus2
    IfLoadedEqualTo ABILITY_DRY_SKIN, TagStrategy_SunnyDay_SelfScoreMinus2
    IfLoadedEqualTo ABILITY_SOLAR_POWER, TagStrategy_SunnyDay_SelfHasSolarPower
    GoTo TagStrategy_SunnyDay_CheckPartner

TagStrategy_SunnyDay_SelfHasLeafGuard:
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, TagStrategy_SunnyDay_CheckPartner
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 30, TagStrategy_SunnyDay_CheckPartner

TagStrategy_SunnyDay_SelfScorePlus2:
    AddToMoveScore 2
    GoTo TagStrategy_SunnyDay_CheckPartner

TagStrategy_SunnyDay_SelfScoreMinus2:
    AddToMoveScore -2
    GoTo TagStrategy_SunnyDay_CheckPartner

TagStrategy_SunnyDay_SelfHasSolarPower:
    IfHPPercentLessThan AI_BATTLER_ATTACKER, 50, TagStrategy_SunnyDay_SelfTryScoreMinus2
    AddToMoveScore 1

TagStrategy_SunnyDay_SelfTryScoreMinus2:
    IfRandomLessThan 128, TagStrategy_SunnyDay_CheckPartner
    AddToMoveScore -2

TagStrategy_SunnyDay_CheckPartner:
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_LEAF_GUARD
    IfLoadedEqualTo AI_HAVE, TagStrategy_SunnyDay_PartnerHasLeafGuard
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_FLOWER_GIFT
    IfLoadedEqualTo AI_HAVE, TagStrategy_SunnyDay_PartnerScorePlus2
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_DRY_SKIN
    IfLoadedEqualTo AI_HAVE, TagStrategy_SunnyDay_PartnerScoreMinus2
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_SOLAR_POWER
    IfLoadedEqualTo AI_HAVE, TagStrategy_SunnyDay_PartnerHasSolarPower
    GoTo TagStrategy_SunnyDay_End

TagStrategy_SunnyDay_PartnerHasLeafGuard:
    IfStatus AI_BATTLER_ATTACKER_PARTNER, MON_CONDITION_ANY, TagStrategy_SunnyDay_End
    IfHPPercentLessThan AI_BATTLER_ATTACKER_PARTNER, 30, TagStrategy_SunnyDay_End

TagStrategy_SunnyDay_PartnerScorePlus2:
    AddToMoveScore 2
    GoTo TagStrategy_SunnyDay_End

TagStrategy_SunnyDay_PartnerScoreMinus2:
    AddToMoveScore -2
    GoTo TagStrategy_SunnyDay_End

TagStrategy_SunnyDay_PartnerHasSolarPower:
    IfHPPercentLessThan AI_BATTLER_ATTACKER_PARTNER, 50, TagStrategy_SunnyDay_PartnerTryScoreMinus2
    AddToMoveScore 1

TagStrategy_SunnyDay_PartnerTryScoreMinus2:
    IfRandomLessThan 128, TagStrategy_SunnyDay_End
    AddToMoveScore -2

TagStrategy_SunnyDay_End:
    PopOrEnd 

TagStrategy_Hail:
    ; If the move is Hail, apply modifiers for each of the attacker and partner which meet the
    ; following conditions:
    ;  - The battler has Ice Body -> score +2
    ;  - The battler has Snow Cloak -> score +2
    ;  - The battler knows Blizzard -> score +2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_ICE_BODY, TagStrategy_Hail_SelfScorePlus2
    IfLoadedEqualTo ABILITY_SNOW_CLOAK, TagStrategy_Hail_SelfScorePlus2
    IfMoveKnown AI_BATTLER_ATTACKER, MOVE_BLIZZARD, TagStrategy_Hail_SelfScorePlus2
    GoTo TagStrategy_Hail_CheckPartner

TagStrategy_Hail_SelfScorePlus2:
    AddToMoveScore 2

TagStrategy_Hail_CheckPartner:
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_ICE_BODY
    IfLoadedEqualTo AI_HAVE, TagStrategy_Hail_PartnerScorePlus2
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_SNOW_CLOAK
    IfLoadedEqualTo AI_HAVE, TagStrategy_Hail_PartnerScorePlus2
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_BLIZZARD, TagStrategy_Hail_PartnerScorePlus2
    GoTo TagStrategy_Hail_End

TagStrategy_Hail_PartnerScorePlus2:
    AddToMoveScore 2

TagStrategy_Hail_End:
    PopOrEnd 

TagStrategy_Sandstorm:
    ; If the move is Sandstorm, apply modifiers for each of the attacker and partner which meet the
    ; following conditions:
    ;  - The battler has Sand Veil -> score +2
    ;  - The battler has a Rock typing -> score +2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_SAND_VEIL, TagStrategy_Sandstorm_SelfScorePlus2
    LoadTypeFrom LOAD_ATTACKER_TYPE_1
    IfLoadedEqualTo TYPE_ROCK, TagStrategy_Sandstorm_SelfScorePlus2
    LoadTypeFrom LOAD_ATTACKER_TYPE_2
    IfLoadedEqualTo TYPE_ROCK, TagStrategy_Sandstorm_SelfScorePlus2
    GoTo TagStrategy_Sandstorm_CheckPartner

TagStrategy_Sandstorm_SelfScorePlus2:
    AddToMoveScore 2
    GoTo TagStrategy_Sandstorm_CheckPartner

TagStrategy_Sandstorm_CheckPartner:
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_SAND_VEIL
    IfLoadedEqualTo AI_HAVE, TagStrategy_Sandstorm_PartnerScorePlus2
    LoadTypeFrom LOAD_ATTACKER_PARTNER_TYPE_1
    IfLoadedEqualTo TYPE_ROCK, TagStrategy_Sandstorm_PartnerScorePlus2
    LoadTypeFrom LOAD_ATTACKER_PARTNER_TYPE_2
    IfLoadedEqualTo TYPE_ROCK, TagStrategy_Sandstorm_PartnerScorePlus2
    GoTo TagStrategy_Sandstorm_End

TagStrategy_Sandstorm_PartnerScorePlus2:
    AddToMoveScore 2

TagStrategy_Sandstorm_End:
    PopOrEnd 

TagStrategy_Gravity:
    ; If Gravity is currently active, score -30
    IfFieldConditionsMask FIELD_CONDITION_GRAVITY, TagStrategy_PartnerScoreMinus30

    ; Apply the following score modifiers:
    ;  - For each allied battler which has Levitate, a Flying typing, or is under the effect of
    ;    Magnet Rise -> score -5
    ;  - For each enemy battler which has Levitate, a Flying typing, or is under the effect of
    ;    Magnet Rise -> 75% chance of score +3
    CheckBattlerAbility AI_BATTLER_ATTACKER, ABILITY_LEVITATE
    IfLoadedEqualTo AI_HAVE, TagStrategy_Gravity_SelfScoreMinus5
    FlagBattlerIsType AI_BATTLER_ATTACKER, TYPE_FLYING
    IfLoadedEqualTo AI_HAVE, TagStrategy_Gravity_SelfScoreMinus5
    IfMoveEffect AI_BATTLER_ATTACKER, MOVE_EFFECT_MAGNET_RISE, TagStrategy_Gravity_SelfScoreMinus5
    GoTo TagStrategy_Gravity_CheckPartner

TagStrategy_Gravity_SelfScoreMinus5:
    AddToMoveScore -5
    GoTo TagStrategy_Gravity_CheckPartner

TagStrategy_Gravity_CheckPartner:
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_LEVITATE
    IfLoadedEqualTo AI_HAVE, TagStrategy_Gravity_PartnerScoreMinus5
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_FLYING
    IfLoadedEqualTo AI_HAVE, TagStrategy_Gravity_PartnerScoreMinus5
    IfMoveEffect AI_BATTLER_ATTACKER_PARTNER, MOVE_EFFECT_MAGNET_RISE, TagStrategy_Gravity_PartnerScoreMinus5
    GoTo TagStrategy_Gravity_CheckTarget

TagStrategy_Gravity_PartnerScoreMinus5:
    AddToMoveScore -5
    GoTo TagStrategy_Gravity_CheckTarget

TagStrategy_Gravity_CheckTarget:
    CheckBattlerAbility AI_BATTLER_DEFENDER, ABILITY_LEVITATE
    IfLoadedEqualTo AI_HAVE, TagStrategy_Gravity_TargetTryScorePlus3
    FlagBattlerIsType AI_BATTLER_DEFENDER, TYPE_FLYING
    IfLoadedEqualTo AI_HAVE, TagStrategy_Gravity_TargetTryScorePlus3
    IfMoveEffect AI_BATTLER_DEFENDER, MOVE_EFFECT_MAGNET_RISE, TagStrategy_Gravity_TargetTryScorePlus3
    GoTo TagStrategy_Gravity_CheckTargetPartner

TagStrategy_Gravity_TargetTryScorePlus3:
    IfRandomLessThan 64, TagStrategy_Gravity_CheckTargetPartner
    AddToMoveScore 3
    GoTo TagStrategy_Gravity_CheckTargetPartner

TagStrategy_Gravity_CheckTargetPartner:
    CheckBattlerAbility AI_BATTLER_DEFENDER_PARTNER, ABILITY_LEVITATE
    IfLoadedEqualTo AI_HAVE, TagStrategy_Gravity_TargetPartnerTryScorePlus3
    FlagBattlerIsType AI_BATTLER_DEFENDER_PARTNER, TYPE_FLYING
    IfLoadedEqualTo AI_HAVE, TagStrategy_Gravity_TargetPartnerTryScorePlus3
    IfMoveEffect AI_BATTLER_DEFENDER_PARTNER, MOVE_EFFECT_MAGNET_RISE, TagStrategy_Gravity_TargetPartnerTryScorePlus3
    GoTo TagStrategy_Gravity_End

TagStrategy_Gravity_TargetPartnerTryScorePlus3:
    IfRandomLessThan 64, TagStrategy_Gravity_End
    AddToMoveScore 3
    GoTo TagStrategy_Gravity_End

TagStrategy_Gravity_End:
    PopOrEnd 

TagStrategy_TrickRoom:
    ; If the battle has been reduced to either side having only one active Pokemon, score -30
    IfHPPercentEqualTo AI_BATTLER_ATTACKER_PARTNER, 0, ScoreMinus30
    IfHPPercentEqualTo AI_BATTLER_DEFENDER_PARTNER, 0, ScoreMinus30
    IfHPPercentEqualTo AI_BATTLER_DEFENDER, 0, ScoreMinus30

    ; Branch according to the attacker''s Speed-ordering in battle
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER
    IfLoadedEqualTo 0, TagStrategy_TrickRoom_SelfMovesFirst
    IfLoadedEqualTo 1, TagStrategy_TrickRoom_SelfMovesSecond
    IfLoadedEqualTo 2, TagStrategy_TrickRoom_SelfMovesThird
    IfLoadedEqualTo 3, TagStrategy_TrickRoom_SelfMovesLast
    GoTo TagStrategy_TrickRoom_End

TagStrategy_TrickRoom_SelfMovesFirst:
    ; If our partner moves second, score -30
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedEqualTo 1, ScoreMinus30
    IfLoadedEqualTo 0, ScoreMinus30
    GoTo TagStrategy_TrickRoom_ScoreMinus5

TagStrategy_TrickRoom_SelfMovesSecond:
    ; If our partner moves before us, score -30
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedEqualTo 0, ScoreMinus30
    GoTo TagStrategy_TrickRoom_ScoreMinus5

TagStrategy_TrickRoom_SelfMovesThird:
    ; If our partner does not move last in turn-order, score -5
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedNotEqualTo 3, TagStrategy_TrickRoom_ScoreMinus5

    ; 75% chance of score +5, 25% chance of score -5
    IfRandomLessThan 64, TagStrategy_TrickRoom_ScoreMinus5
    AddToMoveScore 5
    GoTo TagStrategy_TrickRoom_End

TagStrategy_TrickRoom_SelfMovesLast:
    ; If our partner does not move third in turn-order, score -5
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedNotEqualTo 2, TagStrategy_TrickRoom_ScoreMinus5

    ; 75% chance of score +5, 25% chance of score -5
    IfRandomLessThan 64, TagStrategy_TrickRoom_ScoreMinus5
    AddToMoveScore 5
    GoTo TagStrategy_TrickRoom_End

TagStrategy_TrickRoom_ScoreMinus5:
    AddToMoveScore -5

TagStrategy_TrickRoom_End:
    PopOrEnd 

TagStrategy_FollowMe:
    ; If the move is Follow Me, apply a score modifier according to the following conditional tree:
    ;  - If the attacker''s HP > 90%, and:
    ;    - If the partner''s HP > 90%, 75% chance of score -1
    ;    - If the partner''s HP is between 50% and 90%, 75% chance of score +1
    ;    - If the partner''s HP is between 30% and 50%, 75% chance of score +2
    ;    - If the partner''s HP is < 30%, 75% chance of score +3
    ;  - If the attacker''s HP is between 50% and 90%, and:
    ;    - If the partner''s HP > 90%, 75% chance of score -2
    ;    - If the partner''s HP is between 50% and 90%, 75% chance of score -1
    ;    - If the partner''s HP is between 30% and 50%, 75% chance of score +1
    ;    - If the partner''s HP is < 30%, 75% chance of score +2
    ;  - If the attacker''s HP is between 30% and 50%, and:
    ;    - If the partner''s HP > 90%, 75% chance of score -2
    ;    - If the partner''s HP is between 50% and 90%, 75% chance of score -2
    ;    - If the partner''s HP is between 30% and 50%, 75% chance of score +1
    ;    - If the partner''s HP is < 30%, 75% chance of score +2
    ;  - If the attacker''s HP < 30%, 75% chance of score -5
	IfHPPercentEqualTo AI_BATTLER_ATTACKER_PARTNER, 0, ScoreMinus30
	LoadProtectChain AI_BATTLER_ATTACKER_PARTNER
    IfLoadedEqualTo 0, TagStrategy_FollowMe_CheckHP
    IfRandomLessThan 12, TagStrategy_FollowMe_CheckHP
    AddToMoveScore 1
    IfRandomLessThan 32, TagStrategy_FollowMe_CheckHP
    AddToMoveScore 1
    IfRandomLessThan 52, TagStrategy_FollowMe_CheckHP
    AddToMoveScore 1
	GoTo TagStrategy_FollowMe_CheckHP
	
TagStrategy_FollowMe_CheckHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 90, TagStrategy_FollowMe_SelfHighHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 50, TagStrategy_FollowMe_SelfMediumHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 30, TagStrategy_FollowMe_SelfLowHP
    IfRandomLessThan 64, TagStrategy_FollowMe_End
    GoTo ScoreMinus5

TagStrategy_FollowMe_SelfHighHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 90, TagStrategy_FollowMe_TryScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 50, TagStrategy_FollowMe_TryScorePlus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 30, TagStrategy_FollowMe_TryScorePlus2
    GoTo TagStrategy_FollowMe_TryScorePlus3

TagStrategy_FollowMe_SelfMediumHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 90, TagStrategy_FollowMe_TryScoreMinus2
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 50, TagStrategy_FollowMe_TryScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 30, TagStrategy_FollowMe_TryScorePlus1
    GoTo TagStrategy_FollowMe_TryScorePlus2

TagStrategy_FollowMe_SelfLowHP:
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 90, TagStrategy_FollowMe_TryScoreMinus2
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 50, TagStrategy_FollowMe_TryScoreMinus2
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 30, TagStrategy_FollowMe_TryScorePlus1
    GoTo TagStrategy_FollowMe_TryScorePlus2

TagStrategy_FollowMe_TryScoreMinus1:
    IfRandomLessThan 64, TagStrategy_FollowMe_End
    AddToMoveScore -1
    GoTo TagStrategy_FollowMe_End

TagStrategy_FollowMe_TryScoreMinus2:
    IfRandomLessThan 64, TagStrategy_FollowMe_End
    AddToMoveScore -2
    GoTo TagStrategy_FollowMe_End

TagStrategy_FollowMe_TryScorePlus1:
    IfRandomLessThan 64, TagStrategy_FollowMe_End
    AddToMoveScore 1
    GoTo TagStrategy_FollowMe_End

TagStrategy_FollowMe_TryScorePlus2:
    IfRandomLessThan 64, TagStrategy_FollowMe_End
    AddToMoveScore 2
    GoTo TagStrategy_FollowMe_End

TagStrategy_FollowMe_TryScorePlus3:
    IfRandomLessThan 64, TagStrategy_FollowMe_End
    AddToMoveScore 3
    GoTo TagStrategy_FollowMe_End

TagStrategy_FollowMe_End:
    PopOrEnd 

TagStrategy_Protect:
    ; Bonus chance to protect turn 1 in doubles
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo FALSE, TagStrategy_Protect_End
    IfRandomLessThan 32, TagStrategy_Protect_End
    AddToMoveScore 1
    IfRandomLessThan 32, TagStrategy_Protect_End
    AddToMoveScore 1
    GoTo TagStrategy_Protect_End

TagStrategy_Protect_End:
    PopOrEnd

TagStrategy_Tailwind:
    ; If speed boosting is not deterred, 87.5% chance for score +3.
    IfBattlerDetersBoosting AI_BATTLER_DEFENDER, ScoreMinus3
    IfBattlerDetersBoosting AI_BATTLER_DEFENDER_PARTNER, ScoreMinus3
    IfRandomLessThan 32, TagStrategy_Tailwind_End
    AddToMoveScore 3
    GoTo TagStrategy_Tailwind_End

TagStrategy_Tailwind_End:
    PopOrEnd

TagStrategy_PartnerKnowsHelpingHand:
    ; If our partner knows Helping Hand, then damaging moves (aside from flat-damage moves)
    ; get score +1
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_ONE_HIT_KO, TagStrategy_PartnerHelpingHand_End
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_40_DAMAGE_FLAT, TagStrategy_PartnerHelpingHand_End
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_LEVEL_DAMAGE_FLAT, TagStrategy_PartnerHelpingHand_End
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_RANDOM_DAMAGE_1_TO_150_LEVEL, TagStrategy_PartnerHelpingHand_End
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_10_DAMAGE_FLAT, TagStrategy_PartnerHelpingHand_End
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedNotEqualTo AI_NO_COMPARISON_MADE, ScorePlus1

TagStrategy_PartnerHelpingHand_End:
    PopOrEnd 

TagStrategy_Unused_1:
    IfStatus AI_BATTLER_ATTACKER, MON_CONDITION_ANY, TagStrategy_Unused_2
    PopOrEnd 

TagStrategy_Unused_2:
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, ScoreMinus5
    AddToMoveScore 1
    IfLoadedEqualTo AI_MOVE_IS_HIGHEST_DAMAGE, ScorePlus2
    PopOrEnd 

TagStrategy_Earthquake:
    ; If the move is Earthquake and our partner:
    ;  - Is immune to Earthquake (has Levitate, a Flying typing, or Magnet Rise), score +2
    ;  - Is weak to Earthquake (has Fire, Electric, Poison, or Rock typing), score -10
    ;  - Otherwise, score -3
    ;
    ; Note that this does not check for if the partner is alive; this means that a solo
    ; battler will score Earthquake and Magnitude an additional -3
    IfMoveEffect AI_BATTLER_ATTACKER_PARTNER, MOVE_EFFECT_MAGNET_RISE, ScorePlus2
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_LEVITATE
    IfLoadedEqualTo AI_HAVE, ScorePlus2
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_FLYING
    IfLoadedEqualTo AI_HAVE, ScorePlus2
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_FIRE
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_ELECTRIC
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_POISON
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_ROCK
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    GoTo ScoreMinus3

TagStrategy_FutureSight:
    ; If the move is Future Sight or Doom Desire:
    ;  - If we have no partner, apply no additional modifiers
    ;  - If our partner knows Future Sight or Doom Desire and:
    ;    - They would move before us, score -3
    ;    - They speed-tie us, 50% chance of score -3
    IfHPPercentEqualTo AI_BATTLER_ATTACKER_PARTNER, 0, TagStrategy_FutureSight_End
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_FUTURE_SIGHT, TagStrategy_FutureSight_CheckSelfSpeed
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_DOOM_DESIRE, TagStrategy_FutureSight_CheckSelfSpeed
    GoTo TagStrategy_FutureSight_End

TagStrategy_FutureSight_CheckSelfSpeed:
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER
    IfLoadedEqualTo 3, ScoreMinus3
    IfLoadedEqualTo 2, TagStrategy_FutureSight_SelfMovesThird
    IfLoadedEqualTo 1, TagStrategy_FutureSight_SelfMovesSecond
    IfLoadedEqualTo 0, TagStrategy_FutureSight_SelfMovesFirst
    GoTo TagStrategy_FutureSight_End

TagStrategy_FutureSight_SelfMovesThird:
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedEqualTo 0, ScoreMinus3
    IfLoadedEqualTo 1, ScoreMinus3
    IfRandomLessThan 128, TagStrategy_FutureSight_End
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedEqualTo 2, ScoreMinus3
    GoTo TagStrategy_FutureSight_End

TagStrategy_FutureSight_SelfMovesSecond:
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedEqualTo 0, ScoreMinus3
    IfRandomLessThan 128, TagStrategy_FutureSight_End
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedEqualTo 1, ScoreMinus3
    GoTo TagStrategy_FutureSight_End

TagStrategy_FutureSight_SelfMovesFirst:
    IfRandomLessThan 128, TagStrategy_FutureSight_End
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedEqualTo 0, ScoreMinus3
    GoTo TagStrategy_FutureSight_End

TagStrategy_FutureSight_End:
    PopOrEnd 

TagStrategy_SkillSwap:
    ; If the move is Skill Swap and:
    ;  - The attacker has Truant, Slow Start, Stall, or Klutz, score +5
    ;  - The target has Shadow Tag, Pure Power, Huge Power, Mold Breaker, Solid Rock, Filter, or
    ;    Flower Gift, score +2
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_TRUANT, ScorePlus5
    IfLoadedEqualTo ABILITY_SLOW_START, ScorePlus5
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SHADOW_TAG, ScorePlus2
    IfLoadedEqualTo ABILITY_PURE_POWER, ScorePlus2
    IfLoadedEqualTo ABILITY_HUGE_POWER, ScorePlus2
    IfLoadedEqualTo ABILITY_MOLD_BREAKER, ScorePlus2
    IfLoadedEqualTo ABILITY_RELENTLESS, ScorePlus2
    IfLoadedEqualTo ABILITY_FILTER, ScorePlus2
    IfLoadedEqualTo ABILITY_FLOWER_GIFT, ScorePlus2
    PopOrEnd 

TagStrategy_CheckElectricMove:
    ; If the move is Discharge, handle it similarly to Earthquake. Otherwise, apply all of the
    ; following which are met:
    ;  - The target''s partner would redirect the move with Lightning Rod, score -1; additional
    ;    score -8 if the target''s partner is also a Ground type
    ;  - The attacker''s partner has Lightning Rod, score -10
    IfMoveEqualTo MOVE_DISCHARGE, TagStrategy_SpreadElectricMove
    CheckBattlerAbility AI_BATTLER_DEFENDER_PARTNER, ABILITY_LIGHTNING_ROD
    IfLoadedEqualTo AI_HAVE, TagStrategy_TargetProtectedByLightningRod
    GoTo TagStrategy_PartnerHasLightningRod

TagStrategy_TargetProtectedByLightningRod:
    AddToMoveScore -1
    FlagBattlerIsType AI_BATTLER_DEFENDER_PARTNER, TYPE_GROUND
    IfLoadedEqualTo AI_NOT_HAVE, TagStrategy_PartnerHasLightningRod
    AddToMoveScore -8

TagStrategy_PartnerHasLightningRod:
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_LIGHTNING_ROD
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    IfMoveEqualTo MOVE_DISCHARGE, TagStrategy_SpreadElectricMove
    GoTo TagStrategy_CheckElectric_End

TagStrategy_SpreadElectricMove:
    ; If our partner has Volt Absorb or Motor Drive, score +3
    ;
    ; If our partner otherwise has a Water or Flying typing, score -10
    ;
    ; If our partner otherwise has a Ground typing, score +3
    ;
    ; Else, score -3
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_MOTOR_DRIVE
    IfLoadedEqualTo AI_HAVE, ScorePlus3
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_VOLT_ABSORB
    IfLoadedEqualTo AI_HAVE, ScorePlus3
	CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_LIGHTNING_ROD
    IfLoadedEqualTo AI_HAVE, ScorePlus3
	FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_GROUND
    IfLoadedEqualTo AI_HAVE, ScorePlus3
	
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_WATER
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_FLYING
    IfLoadedEqualTo AI_HAVE, ScoreMinus10

    AddToMoveScore -3

TagStrategy_CheckElectric_End:
    PopOrEnd 

TagStrategy_CheckWaterMove:
    ; If the move is Surf, handle it similarly to Earthquake. Otherwise, apply all of the
    ; following which are met:
    ;  - The target''s partner would redirect the move with Storm Drain, score -1
    ;  - The attacker''s partner has Storm Drain, score -10
    IfMoveEqualTo MOVE_SURF, TagStrategy_SpreadWaterMove
    CheckBattlerAbility AI_BATTLER_DEFENDER_PARTNER, ABILITY_STORM_DRAIN
    IfLoadedEqualTo AI_NOT_HAVE, TagStrategy_CheckPartnerStormDrain
    AddToMoveScore -1

TagStrategy_CheckPartnerStormDrain:
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_STORM_DRAIN
    IfLoadedEqualTo AI_HAVE, ScoreMinus10

    ; This line should never result in a branch
    IfMoveEqualTo MOVE_SURF, TagStrategy_SpreadWaterMove
    GoTo TagStrategy_CheckWater_End

TagStrategy_SpreadWaterMove:
    ; If our partner has Dry Skin or Water Absorb, score +3
    ;
    ; If our partner otherwise has a Ground, Rock or Fire typing, score -10
    ;
    ; Else, score -3
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_DRY_SKIN
    IfLoadedEqualTo AI_HAVE, ScorePlus3
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_WATER_ABSORB
    IfLoadedEqualTo AI_HAVE, ScorePlus3
	CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_STORM_DRAIN
    IfLoadedEqualTo AI_HAVE, ScorePlus3

    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_GROUND
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_FIRE
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
	FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_ROCK
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    AddToMoveScore -3

TagStrategy_CheckWater_End:
    PopOrEnd 

TagStrategy_CheckFireMove:
    ; If the AI''s Flash Fire has been activated, score additional +1 on top of all further modifiers
    ;
    ; If the move is Lava Plume, then:
    ;  - If our partner has Dry Skin or Flash Fire, score +3
    ;  - If our partner has a Grass, Steel, Ice, or Bug typing, score -10
    ;  - Otherwise, score -3
    IfActivatedFlashFire AI_BATTLER_ATTACKER, TagStrategy_FlashFireScorePlus1
    GoTo TagStrategy_CheckLavaPlume

TagStrategy_FlashFireScorePlus1:
    AddToMoveScore 1

TagStrategy_CheckLavaPlume:
    IfMoveEqualTo MOVE_LAVA_PLUME, TagStrategy_SpreadFireMove
    GoTo TagStrategy_CheckFire_End

TagStrategy_SpreadFireMove:
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_DRY_SKIN
    IfLoadedEqualTo AI_HAVE, ScoreMinus3
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_FLASH_FIRE
    IfLoadedEqualTo AI_HAVE, ScorePlus3
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_GRASS
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_STEEL
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_ICE
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    FlagBattlerIsType AI_BATTLER_ATTACKER_PARTNER, TYPE_BUG
    IfLoadedEqualTo AI_HAVE, ScoreMinus10
    AddToMoveScore -3

TagStrategy_CheckFire_End:
    PopOrEnd 

TagStrategy_Partner:
    IfBattlerFainted AI_BATTLER_ATTACKER_PARTNER, TagStrategy_PartnerScoreMinus30
    FlagMoveDamageScore USE_MAX_DAMAGE
    IfLoadedEqualTo AI_NO_COMPARISON_MADE, TagStrategy_PartnerStatusMove
    LoadTypeFrom LOAD_MOVE_TYPE
    IfLoadedEqualTo TYPE_FIRE, TagStrategy_CheckPartnerFireAbsorption
    IfLoadedEqualTo TYPE_ELECTRIC, TagStrategy_CheckPartnerElectricAbsorption
    IfLoadedEqualTo TYPE_WATER, TagStrategy_CheckPartnerWaterAbsorption
    IfMoveEqualTo MOVE_FLING, TagStrategy_PartnerTrick

TagStrategy_ScoreMinus30:
    GoTo ScoreMinus30

TagStrategy_CheckPartnerFireAbsorption:
    ; If our partner has Flash Fire and has not yet activated Flash Fire, score +3
    ;
    ; Otherwise, score -30
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_FLASH_FIRE
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerFlashFireActive
    GoTo TagStrategy_ScoreMinus30

TagStrategy_CheckPartnerFlashFireActive:
    IfActivatedFlashFire AI_BATTLER_ATTACKER_PARTNER, TagStrategy_ScoreMinus30
    GoTo ScorePlus3

TagStrategy_CheckPartnerElectricAbsorption:
    ; If our partner has Motor Drive:
    ;  - 62.5% chance of no score change
    ;  - If our partner is at +6 speed, score -30
    ;  - Else, score +3
    ;
    ; If our partner has Volt Absorb:
    ;  - If our partner is at 100% HP, score -10
    ;  - If our partner''s HP >90%, no score change
    ;  - If our partner''s HP >75%, 25% chance of score +3, 75% chance of no change
    ;  - If our partner''s HP >50%, 50% chance of score +3, 50% chance of no change
    ;  - Else, 75% chance of score +3, 25% chance of no change
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_MOTOR_DRIVE
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerMotorDrive
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_VOLT_ABSORB
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerVoltAbsorb
	CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_LIGHTNING_ROD
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerLightningRodAbsorb
    GoTo TagStrategy_ScoreMinus30

TagStrategy_CheckPartnerMotorDrive:
    IfRandomLessThan 160, TagStrategy_CheckElectricAbsorption_End
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_SPEED, 12, TagStrategy_ScoreMinus30
    GoTo ScorePlus3

TagStrategy_CheckPartnerVoltAbsorb:
    IfHPPercentEqualTo AI_BATTLER_ATTACKER_PARTNER, 100, ScoreMinus10
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 90, TagStrategy_CheckElectricAbsorption_End
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 75, TagStrategy_PartnerVoltAbsorb_75PercentHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 50, TagStrategy_PartnerVoltAbsorb_50PercentHP
    GoTo TagStrategy_PartnerVoltAbsorb_LessThan50PercentHP

TagStrategy_PartnerVoltAbsorb_75PercentHP:
    IfRandomLessThan 64, ScorePlus3
    GoTo TagStrategy_CheckElectricAbsorption_End

TagStrategy_PartnerVoltAbsorb_50PercentHP:
    IfRandomLessThan 128, ScorePlus3
    GoTo TagStrategy_CheckElectricAbsorption_End

TagStrategy_PartnerVoltAbsorb_LessThan50PercentHP:
    IfRandomLessThan 192, ScorePlus3
    GoTo TagStrategy_CheckElectricAbsorption_End

TagStrategy_CheckElectricAbsorption_End:
    PopOrEnd
	
TagStrategy_CheckPartnerLightningRodAbsorb:
    IfRandomLessThan 160, TagStrategy_CheckElectricAbsorption_End
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_SP_ATTACK, 12, TagStrategy_ScoreMinus30
    GoTo ScorePlus3

TagStrategy_CheckPartnerWaterAbsorption:
    ; If our partner has Water Absorb or Dry Skin:
    ;  - If our partner is at 100% HP, score -10
    ;  - If our partner''s HP >90%, no score change
    ;  - If our partner''s HP >75%, 25% chance of score +3, 75% chance of no change
    ;  - If our partner''s HP >50%, 50% chance of score +3, 50% chance of no change
    ;  - Else, 75% chance of score +3, 25% chance of no change
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_WATER_ABSORB
    IfLoadedEqualTo AI_HAVE, TagStrategy_PartnerWaterAbsorb
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_DRY_SKIN
    IfLoadedEqualTo AI_HAVE, TagStrategy_PartnerWaterAbsorb
	CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_STORM_DRAIN
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerStormDrainAbsorb
    GoTo TagStrategy_ScoreMinus30

TagStrategy_PartnerWaterAbsorb:
    IfHPPercentEqualTo AI_BATTLER_ATTACKER_PARTNER, 100, ScoreMinus10
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 90, TagStrategy_CheckWaterAbsorption_End
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 75, TagStrategy_PartnerWaterAbsorb_75PercentHP
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 50, TagStrategy_PartnerWaterAbsorb_50PercentHP
    GoTo TagStrategy_PartnerWaterAbsorb_LessThan50PercentHP

TagStrategy_PartnerWaterAbsorb_75PercentHP:
    IfRandomLessThan 64, ScorePlus3
    GoTo TagStrategy_CheckWaterAbsorption_End

TagStrategy_PartnerWaterAbsorb_50PercentHP:
    IfRandomLessThan 128, ScorePlus3
    GoTo TagStrategy_CheckWaterAbsorption_End

TagStrategy_PartnerWaterAbsorb_LessThan50PercentHP:
    IfRandomLessThan 192, ScorePlus3
    GoTo TagStrategy_CheckWaterAbsorption_End

TagStrategy_CheckWaterAbsorption_End:
    PopOrEnd
	
TagStrategy_CheckPartnerStormDrainAbsorb:
    IfRandomLessThan 160, TagStrategy_CheckWaterAbsorption_End
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_SP_ATTACK, 12, TagStrategy_ScoreMinus30
    GoTo ScorePlus3

TagStrategy_PartnerStatusMove:
	IfHPPercentEqualTo AI_BATTLER_ATTACKER_PARTNER, 0, ScoreMinus30
    IfMoveEqualTo MOVE_SKILL_SWAP, TagStrategy_PartnerSkillSwap
    IfMoveEqualTo MOVE_WILL_O_WISP, TagStrategy_PartnerWillOWisp
    IfMoveEqualTo MOVE_THUNDER_WAVE, TagStrategy_PartnerThunderWave
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_BADLY_POISON, TagStrategy_PartnerPoisonStatus
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_STATUS_POISON, TagStrategy_PartnerPoisonStatus
    IfMoveEqualTo MOVE_HELPING_HAND, TagStrategy_PartnerUsingHelpingHand
    IfMoveEqualTo MOVE_SWAGGER, TagStrategy_PartnerSwagger
    IfMoveEqualTo MOVE_TRICK, TagStrategy_PartnerTrick
    IfMoveEqualTo MOVE_SWITCHEROO, TagStrategy_PartnerTrick
    IfMoveEqualTo MOVE_GASTRO_ACID, TagStrategy_PartnerGastroAcid
    IfMoveEqualTo MOVE_ACUPRESSURE, TagStrategy_PartnerAcupressure
    GoTo TagStrategy_PartnerScoreMinus30

TagStrategy_PartnerSkillSwap:
    ; If our partner has Truant or Slow Start, score +10.
    ;
    ; If we can give Levitate to an Electric-type partner, score +1; additional +1 if our partner
    ; is mono-Electric.
    ;
    ; If we can give an Accuracy-increasing ability and our partner has an inaccurate move, score +3.
    ;
    ; Otherwise, score -30.
    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_TRUANT, ScorePlus10
    IfLoadedEqualTo ABILITY_SLOW_START, ScorePlus10

    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedNotEqualTo ABILITY_LEVITATE, TagStrategy_PartnerSkillSwap_GiveAccuracyIncrease

    LoadBattlerAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_LEVITATE, TagStrategy_PartnerScoreMinus30

    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedNotEqualTo TYPE_ELECTRIC, TagStrategy_PartnerSkillSwap_GiveAccuracyIncrease
    AddToMoveScore 1

    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedNotEqualTo TYPE_ELECTRIC, TagStrategy_PartnerSkillSwap_GiveAccuracyIncrease
    AddToMoveScore 1

    PopOrEnd 

TagStrategy_PartnerSkillSwap_GiveAccuracyIncrease:
    LoadBattlerAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_COMPOUND_EYES, TagStrategy_PartnerSkillSwap_PartnerHasInaccurateMove
    IfLoadedEqualTo ABILITY_NO_GUARD, TagStrategy_PartnerSkillSwap_PartnerHasInaccurateMove
	IfLoadedEqualTo ABILITY_SUCTION_CUPS, TagStrategy_PartnerSkillSwap_PartnerHasInaccurateMove
    GoTo TagStrategy_PartnerScoreMinus30

TagStrategy_PartnerSkillSwap_PartnerHasInaccurateMove:
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_FIRE_BLAST, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_THUNDER, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_CROSS_CHOP, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_HYDRO_PUMP, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_DYNAMIC_PUNCH, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_BLIZZARD, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_ZAP_CANNON, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_MEGAHORN, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_FOCUS_BLAST, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_GUNK_SHOT, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_MAGMA_STORM, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_POWER_WHIP, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_SEED_FLARE, TagStrategy_PartnerSkillSwap_ScorePlus3
    IfMoveKnown AI_BATTLER_ATTACKER_PARTNER, MOVE_HEAD_SMASH, TagStrategy_PartnerSkillSwap_ScorePlus3
    GoTo TagStrategy_PartnerScoreMinus30

TagStrategy_PartnerSkillSwap_ScorePlus3:
    GoTo ScorePlus3

TagStrategy_PartnerWillOWisp:
    ; If our partner has Flash Fire, handle it identically to the earlier Fire Absorption routine
    ;
    ; If our partner meets all of the following conditions, score +5:
    ;  - Has the Guts ability
    ;  - Is not currently statused
    ;  - Does not have a Fire typing
    ;  - Is not holding a Flame Orb or Toxic Orb
    ;  - Is at 81% HP or greater
    ;
    ; Otherwise, score -30
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_FLASH_FIRE
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerFireAbsorption

    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_GUTS
    IfLoadedNotEqualTo AI_HAVE, TagStrategy_PartnerScoreMinus30

    IfStatus AI_BATTLER_ATTACKER_PARTNER, MON_CONDITION_ANY, TagStrategy_PartnerScoreMinus30

    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_FIRE, TagStrategy_PartnerScoreMinus30
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_FIRE, TagStrategy_PartnerScoreMinus30

    IfHeldItemEqualTo AI_BATTLER_ATTACKER_PARTNER, ITEM_FLAME_ORB, TagStrategy_PartnerScoreMinus30
    IfHeldItemEqualTo AI_BATTLER_ATTACKER_PARTNER, ITEM_TOXIC_ORB, TagStrategy_PartnerScoreMinus30

    IfHPPercentLessThan AI_BATTLER_ATTACKER_PARTNER, 81, TagStrategy_PartnerScoreMinus30

    GoTo ScorePlus5

TagStrategy_PartnerThunderWave:
    ; If our partner has a Ground typing or has an ability other than Motor Drive or Volt Absorb, score -30
    ;
    ; Otherwise, handle the move identically to other Electric moves
    LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_GROUND, TagStrategy_PartnerScoreMinus30
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_GROUND, TagStrategy_PartnerScoreMinus30

    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_MOTOR_DRIVE
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerElectricAbsorption

    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_VOLT_ABSORB
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerElectricAbsorption
	
	CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_LIGHTNING_ROD
    IfLoadedEqualTo AI_HAVE, TagStrategy_CheckPartnerElectricAbsorption

    GoTo TagStrategy_PartnerScoreMinus30

TagStrategy_PartnerPoisonStatus:
    ; If our partner meets all of the following conditions, score +5:
    ;  - Has the Poison Heal ability
    ;  - Is not currently statused
    ;  - Is not holding a Toxic Orb
    ;  - Is at 91% HP or less
	;  - Is not Steel or Poison type
    ;
    ; Otherwise, score -30
    ;
    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_POISON_HEAL
    IfLoadedNotEqualTo AI_HAVE, TagStrategy_PartnerScoreMinus30

    IfStatus AI_BATTLER_DEFENDER, MON_CONDITION_ANY, TagStrategy_PartnerScoreMinus30

    IfHeldItemEqualTo AI_BATTLER_ATTACKER_PARTNER, ITEM_TOXIC_ORB, TagStrategy_PartnerScoreMinus30

    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 91, TagStrategy_PartnerScoreMinus30
	
	LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_STEEL, TagStrategy_PartnerScoreMinus30
	
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_STEEL, TagStrategy_PartnerScoreMinus30
	
	LoadTypeFrom LOAD_DEFENDER_TYPE_1
    IfLoadedEqualTo TYPE_POISON, TagStrategy_PartnerScoreMinus30
	
    LoadTypeFrom LOAD_DEFENDER_TYPE_2
    IfLoadedEqualTo TYPE_POISON, TagStrategy_PartnerScoreMinus30

    GoTo ScorePlus5

TagStrategy_PartnerUsingHelpingHand:
    ; If we do not have a partner, score -30
    ;
    ; If our partner has more than 50% HP or would move first in the turn, 75% chance of score +2,
    ; 25% chance of score -1
    ;
    ; Else, no score changes
    IfHPPercentEqualTo AI_BATTLER_ATTACKER_PARTNER, 0, ScoreMinus30
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 50, TagStrategy_PartnerUsingHelpingHand_TryScorePlus2
    LoadBattlerSpeedRank AI_BATTLER_ATTACKER_PARTNER
    IfLoadedLessThan 1, TagStrategy_PartnerUsingHelpingHand_TryScorePlus2
    GoTo TagStrategy_PartnerUsingHelpingHand_End

TagStrategy_PartnerUsingHelpingHand_TryScorePlus2:
    IfRandomLessThan 64, ScoreMinus1
    AddToMoveScore 2

TagStrategy_PartnerUsingHelpingHand_End:
    PopOrEnd 

TagStrategy_PartnerSwagger:
    ; If our partner is holding neither a Persim Berry nor a Lum Berry, score -30
    ;
    ; If our partner is at less than +2 Attack, score +3
    ;
    ; Otherwise, no score changes
    ;
    ; Curiously, this does not consider if our partner''s ability is Own Tempo.
    IfHeldItemEqualTo AI_BATTLER_DEFENDER, ITEM_PERSIM_BERRY, TagStrategy_PartnerSwagger_TryScorePlus3
    IfHeldItemEqualTo AI_BATTLER_DEFENDER, ITEM_LUM_BERRY, TagStrategy_PartnerSwagger_TryScorePlus3
    GoTo TagStrategy_PartnerScoreMinus30

TagStrategy_PartnerSwagger_TryScorePlus3:
    IfStatStageGreaterThan AI_BATTLER_DEFENDER, BATTLE_STAT_ATTACK, 7, TagStrategy_PartnerSwagger_End
    AddToMoveScore 3

TagStrategy_PartnerSwagger_End:
    PopOrEnd 

TagStrategy_PartnerTrick:
    PopOrEnd 

TagStrategy_PartnerGastroAcid:
    ; If our partner''s ability is already suppressed, score -30
    ;
    ; If our partner has Truant or Slow Start, score +5
    ;
    ; Otherwise, no score changes
    IfMoveEffect AI_BATTLER_ATTACKER_PARTNER, MOVE_EFFECT_ABILITY_SUPPRESSED, TagStrategy_PartnerScoreMinus30

    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_TRUANT
    IfLoadedEqualTo AI_HAVE, TagStrategy_PartnerGastroAcid_ScorePlus5

    CheckBattlerAbility AI_BATTLER_ATTACKER_PARTNER, ABILITY_SLOW_START
    IfLoadedEqualTo AI_HAVE, TagStrategy_PartnerGastroAcid_ScorePlus5

    GoTo TagStrategy_PartnerGastroAcid_End

TagStrategy_PartnerGastroAcid_ScorePlus5:
    AddToMoveScore 5

TagStrategy_PartnerGastroAcid_End:
    PopOrEnd 

TagStrategy_PartnerAcupressure:
    ; Else if our partner has any stat at +6 stages, score -30
    ;
    ; Else if our partner''s HP is 50% or lower, score -1
    ;
    ; Else if our partner''s HP is 91% or higher, 68.75% chance of score +2, 31.25% chance of no score change
    ;
    ; Else 31.25% chance of score +2, 68.75% chance of no score change
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_ATTACK, 12, TagStrategy_PartnerScoreMinus30
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_DEFENSE, 12, TagStrategy_PartnerScoreMinus30
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_SPEED, 12, TagStrategy_PartnerScoreMinus30
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_SP_ATTACK, 12, TagStrategy_PartnerScoreMinus30
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_SP_DEFENSE, 12, TagStrategy_PartnerScoreMinus30
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_EVASION, 12, TagStrategy_PartnerScoreMinus30
    IfStatStageEqualTo AI_BATTLER_ATTACKER_PARTNER, BATTLE_STAT_ACCURACY, 12, TagStrategy_PartnerScoreMinus30
    GoTo TagStrategy_PartnerAcupressure_CheckHP

TagStrategy_PartnerAcupressure_CheckHP:
    IfHPPercentLessThan AI_BATTLER_ATTACKER_PARTNER, 51, TagStrategy_PartnerAcupressure_ScoreMinus1
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER_PARTNER, 90, TagStrategy_PartnerAcupressure_TryScorePlus2
    IfRandomLessThan 128, TagStrategy_PartnerAcupressure_End

TagStrategy_PartnerAcupressure_TryScorePlus2:
    IfRandomLessThan 80, TagStrategy_PartnerAcupressure_End
    AddToMoveScore 2
    GoTo TagStrategy_PartnerAcupressure_End

TagStrategy_PartnerAcupressure_ScoreMinus1:
    AddToMoveScore -1

TagStrategy_PartnerAcupressure_End:
    PopOrEnd 

TagStrategy_PartnerScoreMinus30:
    AddToMoveScore -30
    PopOrEnd 
	
TagStrategy_PopOrEnd:
    PopOrEnd 

CheckHP_Main:
    IfTargetIsPartner TagStrategy_Partner

    ; Which moves apply to the routine depends on the attacker''s HP percentage
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 70, CheckHP_GT70Percent ; >70%
    IfHPPercentGreaterThan AI_BATTLER_ATTACKER, 30, CheckHP_31To70Percent ; 31-70%
    LoadCurrentMoveEffect 
    IfLoadedInTable CheckHP_DiscourageAtLowHP, CheckHP_TryScoreMinus2 ; 1-30%
    GoTo CheckHP_Target

CheckHP_GT70Percent:
    LoadCurrentMoveEffect 
    IfLoadedInTable CheckHP_DiscourageAtHighHP, CheckHP_TryScoreMinus2
    GoTo CheckHP_Target

CheckHP_31To70Percent:
    LoadCurrentMoveEffect 
    IfLoadedInTable CheckHP_DiscourageAtMediumHP, CheckHP_TryScoreMinus2
    GoTo CheckHP_Target

CheckHP_TryScoreMinus2:
    ; ~80.5% of the time, score -2
    IfRandomLessThan 50, CheckHP_Target
    AddToMoveScore -2

CheckHP_Target:
    ; The second round is similar to the first, but looks at the target''s HP instead of
    ; the attacker''s.
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 70, CheckHP_Target_GT70Percent
    IfHPPercentGreaterThan AI_BATTLER_DEFENDER, 30, CheckHP_Target_31To70Percent
    LoadCurrentMoveEffect 
    IfLoadedInTable CheckHP_Target_DiscourageAtLowHP, CheckHP_Target_TryScoreMinus2
    GoTo CheckHP_Terminate

CheckHP_Target_GT70Percent:
    LoadCurrentMoveEffect 
    IfLoadedInTable CheckHP_Target_DiscourageAtHighHP, CheckHP_Target_TryScoreMinus2
    GoTo CheckHP_Terminate

CheckHP_Target_31To70Percent:
    LoadCurrentMoveEffect 
    IfLoadedInTable CheckHP_Target_DiscourageAtMediumHP, CheckHP_Target_TryScoreMinus2
    GoTo CheckHP_Terminate

CheckHP_Target_TryScoreMinus2:
    ; ~80.5% of the time, score -2
    IfRandomLessThan 50, CheckHP_Terminate
    AddToMoveScore -2

CheckHP_Terminate:
    PopOrEnd 

CheckHP_DiscourageAtHighHP:
    TableEntry BATTLE_EFFECT_RESTORE_HALF_HP
    TableEntry BATTLE_EFFECT_REST
    TableEntry BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER
    TableEntry BATTLE_EFFECT_INCREASE_POWER_WITH_LESS_HP
    TableEntry BATTLE_EFFECT_SURVIVE_WITH_1_HP
    TableEntry BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN
    TableEntry BATTLE_EFFECT_FAINT_AND_ATK_SP_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_REMOVE_ALL_PP_ON_DEFEAT
    TableEntry BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE
    TableEntry BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON
    TableEntry BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON
    TableEntry TABLE_END

CheckHP_DiscourageAtMediumHP:
    TableEntry BATTLE_EFFECT_ATK_UP
    TableEntry BATTLE_EFFECT_DEF_UP
    TableEntry BATTLE_EFFECT_SPEED_UP
    TableEntry BATTLE_EFFECT_SP_ATK_UP
    TableEntry BATTLE_EFFECT_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ACC_UP
    TableEntry BATTLE_EFFECT_EVA_UP
    TableEntry BATTLE_EFFECT_ATK_DOWN
    TableEntry BATTLE_EFFECT_DEF_DOWN
    TableEntry BATTLE_EFFECT_SPEED_DOWN
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN
    TableEntry BATTLE_EFFECT_ACC_DOWN
    TableEntry BATTLE_EFFECT_EVA_DOWN
    TableEntry BATTLE_EFFECT_BIDE
    TableEntry BATTLE_EFFECT_CONVERSION
    TableEntry BATTLE_EFFECT_SET_LIGHT_SCREEN
    TableEntry BATTLE_EFFECT_PREVENT_STAT_REDUCTION
    TableEntry BATTLE_EFFECT_CRIT_UP_2
    TableEntry BATTLE_EFFECT_ATK_UP_2
    TableEntry BATTLE_EFFECT_DEF_UP_2
    TableEntry BATTLE_EFFECT_SPEED_UP_2
    TableEntry BATTLE_EFFECT_SP_ATK_UP_2
    TableEntry BATTLE_EFFECT_SP_DEF_UP_2
    TableEntry BATTLE_EFFECT_ACC_UP_2
    TableEntry BATTLE_EFFECT_EVA_UP_2
    TableEntry BATTLE_EFFECT_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_SPEED_DOWN_2
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_EVA_DOWN_2
    TableEntry BATTLE_EFFECT_ACC_DOWN_2
    TableEntry BATTLE_EFFECT_CONVERSION2
    TableEntry BATTLE_EFFECT_PREVENT_STATUS
    TableEntry BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP
    TableEntry BATTLE_EFFECT_ATK_DEF_DOWN
    TableEntry BATTLE_EFFECT_DEF_SPD_UP
    TableEntry BATTLE_EFFECT_ATK_DEF_UP
    TableEntry BATTLE_EFFECT_SP_ATK_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ATK_SPD_UP
    TableEntry BATTLE_EFFECT_PREVENT_CRITS
    TableEntry BATTLE_EFFECT_SWAP_ATK_SP_ATK_STAT_CHANGES
    TableEntry BATTLE_EFFECT_SWAP_DEF_SP_DEF_STAT_CHANGES
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER
    TableEntry TABLE_END

CheckHP_DiscourageAtLowHP:
    TableEntry BATTLE_EFFECT_ATK_UP
    TableEntry BATTLE_EFFECT_DEF_UP
    TableEntry BATTLE_EFFECT_SPEED_UP
    TableEntry BATTLE_EFFECT_SP_ATK_UP
    TableEntry BATTLE_EFFECT_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ACC_UP
    TableEntry BATTLE_EFFECT_EVA_UP
    TableEntry BATTLE_EFFECT_ATK_DOWN
    TableEntry BATTLE_EFFECT_DEF_DOWN
    TableEntry BATTLE_EFFECT_SPEED_DOWN
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN
    TableEntry BATTLE_EFFECT_ACC_DOWN
    TableEntry BATTLE_EFFECT_EVA_DOWN
    TableEntry BATTLE_EFFECT_BIDE
    TableEntry BATTLE_EFFECT_CONVERSION
    TableEntry BATTLE_EFFECT_SET_LIGHT_SCREEN
    TableEntry BATTLE_EFFECT_PREVENT_STAT_REDUCTION
    TableEntry BATTLE_EFFECT_CRIT_UP_2
    TableEntry BATTLE_EFFECT_ATK_UP_2
    TableEntry BATTLE_EFFECT_DEF_UP_2
    TableEntry BATTLE_EFFECT_SPEED_UP_2
    TableEntry BATTLE_EFFECT_SP_ATK_UP_2
    TableEntry BATTLE_EFFECT_SP_DEF_UP_2
    TableEntry BATTLE_EFFECT_ACC_UP_2
    TableEntry BATTLE_EFFECT_EVA_UP_2
    TableEntry BATTLE_EFFECT_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_SPEED_DOWN_2
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_EVA_DOWN_2
    TableEntry BATTLE_EFFECT_ACC_DOWN_2
    TableEntry BATTLE_EFFECT_RAISE_ATK_WHEN_HIT
    TableEntry BATTLE_EFFECT_CONVERSION2
    TableEntry BATTLE_EFFECT_NEXT_ATTACK_ALWAYS_HITS
    TableEntry BATTLE_EFFECT_PREVENT_STATUS
    TableEntry BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP
    TableEntry BATTLE_EFFECT_COPY_STAT_CHANGES
    TableEntry BATTLE_EFFECT_MIRROR_COAT
    TableEntry BATTLE_EFFECT_DECREASE_POWER_WITH_LESS_USER_HP
    TableEntry BATTLE_EFFECT_ATK_DEF_DOWN
    TableEntry BATTLE_EFFECT_DEF_SPD_UP
    TableEntry BATTLE_EFFECT_ATK_DEF_UP
    TableEntry BATTLE_EFFECT_SP_ATK_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ATK_SPD_UP
    TableEntry BATTLE_EFFECT_HALVE_ELECTRIC_DAMAGE
    TableEntry BATTLE_EFFECT_HALVE_FIRE_DAMAGE
    TableEntry BATTLE_EFFECT_RANDOM_STAT_UP_2
    TableEntry BATTLE_EFFECT_METAL_BURST
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER
    TableEntry TABLE_END

CheckHP_Target_DiscourageAtHighHP:
    TableEntry TABLE_END

CheckHP_Target_DiscourageAtMediumHP:
    TableEntry BATTLE_EFFECT_ATK_UP
    TableEntry BATTLE_EFFECT_DEF_UP
    TableEntry BATTLE_EFFECT_SPEED_UP
    TableEntry BATTLE_EFFECT_SP_ATK_UP
    TableEntry BATTLE_EFFECT_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ACC_UP
    TableEntry BATTLE_EFFECT_EVA_UP
    TableEntry BATTLE_EFFECT_ATK_DOWN
    TableEntry BATTLE_EFFECT_DEF_DOWN
    TableEntry BATTLE_EFFECT_SPEED_DOWN
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN
    TableEntry BATTLE_EFFECT_ACC_DOWN
    TableEntry BATTLE_EFFECT_EVA_DOWN
    TableEntry BATTLE_EFFECT_PREVENT_STAT_REDUCTION
    TableEntry BATTLE_EFFECT_CRIT_UP_2
    TableEntry BATTLE_EFFECT_ATK_UP_2
    TableEntry BATTLE_EFFECT_DEF_UP_2
    TableEntry BATTLE_EFFECT_SPEED_UP_2
    TableEntry BATTLE_EFFECT_SP_ATK_UP_2
    TableEntry BATTLE_EFFECT_SP_DEF_UP_2
    TableEntry BATTLE_EFFECT_ACC_UP_2
    TableEntry BATTLE_EFFECT_EVA_UP_2
    TableEntry BATTLE_EFFECT_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_SPEED_DOWN_2
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_EVA_DOWN_2
    TableEntry BATTLE_EFFECT_ACC_DOWN_2
    TableEntry BATTLE_EFFECT_STATUS_POISON
    TableEntry BATTLE_EFFECT_AVERAGE_HP
    TableEntry BATTLE_EFFECT_ALL_FAINT_3_TURNS
    TableEntry BATTLE_EFFECT_PREVENT_STATUS
    TableEntry BATTLE_EFFECT_ATK_DEF_DOWN
    TableEntry BATTLE_EFFECT_DEF_SPD_UP
    TableEntry BATTLE_EFFECT_ATK_DEF_UP
    TableEntry BATTLE_EFFECT_SP_ATK_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ATK_SPD_UP
    TableEntry BATTLE_EFFECT_RANDOM_STAT_UP_2
    TableEntry BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_HP
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER
    TableEntry TABLE_END

CheckHP_Target_DiscourageAtLowHP:
    TableEntry BATTLE_EFFECT_STATUS_SLEEP
    TableEntry BATTLE_EFFECT_HALVE_DEFENSE ; done
    TableEntry BATTLE_EFFECT_ATK_UP
    TableEntry BATTLE_EFFECT_DEF_UP
    TableEntry BATTLE_EFFECT_SPEED_UP
    TableEntry BATTLE_EFFECT_SP_ATK_UP
    TableEntry BATTLE_EFFECT_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ACC_UP
    TableEntry BATTLE_EFFECT_EVA_UP
    TableEntry BATTLE_EFFECT_ATK_DOWN
    TableEntry BATTLE_EFFECT_DEF_DOWN
    TableEntry BATTLE_EFFECT_SPEED_DOWN
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN
    TableEntry BATTLE_EFFECT_ACC_DOWN
    TableEntry BATTLE_EFFECT_EVA_DOWN
    TableEntry BATTLE_EFFECT_BIDE
    TableEntry BATTLE_EFFECT_CONVERSION
    TableEntry BATTLE_EFFECT_STATUS_BADLY_POISON
    TableEntry BATTLE_EFFECT_SET_LIGHT_SCREEN
    TableEntry BATTLE_EFFECT_ONE_HIT_KO
    TableEntry BATTLE_EFFECT_HALVE_HP
    TableEntry BATTLE_EFFECT_HALVE_HP
    TableEntry BATTLE_EFFECT_PREVENT_STAT_REDUCTION
    TableEntry BATTLE_EFFECT_CRIT_UP_2
    TableEntry BATTLE_EFFECT_STATUS_CONFUSE
    TableEntry BATTLE_EFFECT_ATK_UP_2
    TableEntry BATTLE_EFFECT_DEF_UP_2
    TableEntry BATTLE_EFFECT_SPEED_UP_2
    TableEntry BATTLE_EFFECT_SP_ATK_UP_2
    TableEntry BATTLE_EFFECT_SP_DEF_UP_2
    TableEntry BATTLE_EFFECT_ACC_UP_2
    TableEntry BATTLE_EFFECT_EVA_UP_2
    TableEntry BATTLE_EFFECT_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_SPEED_DOWN_2
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_EVA_DOWN_2
    TableEntry BATTLE_EFFECT_ACC_DOWN_2
    TableEntry BATTLE_EFFECT_STATUS_POISON
    TableEntry BATTLE_EFFECT_STATUS_PARALYZE
    TableEntry BATTLE_EFFECT_AVERAGE_HP
    TableEntry BATTLE_EFFECT_CONVERSION2
    TableEntry BATTLE_EFFECT_NEXT_ATTACK_ALWAYS_HITS
    TableEntry BATTLE_EFFECT_DECREASE_LAST_MOVE_PP
    TableEntry BATTLE_EFFECT_ALL_FAINT_3_TURNS
    TableEntry BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION
    TableEntry BATTLE_EFFECT_DOUBLE_POWER_EACH_TURN
    TableEntry BATTLE_EFFECT_INFATUATE
    TableEntry BATTLE_EFFECT_PREVENT_STATUS
    TableEntry BATTLE_EFFECT_COPY_STAT_CHANGES
    TableEntry BATTLE_EFFECT_MIRROR_COAT
    TableEntry BATTLE_EFFECT_STATUS_BURN
    TableEntry BATTLE_EFFECT_ATK_DEF_DOWN
    TableEntry BATTLE_EFFECT_DEF_SPD_UP
    TableEntry BATTLE_EFFECT_ATK_DEF_UP
    TableEntry BATTLE_EFFECT_SP_ATK_SP_DEF_UP
    TableEntry BATTLE_EFFECT_ATK_SPD_UP
    TableEntry BATTLE_EFFECT_RANDOM_STAT_UP_2
    TableEntry BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_HP
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER
    TableEntry TABLE_END

Weather_Main:
    IfTargetIsPartner Terminate

    ; If it is not the first turn of the battle, break.
    LoadTurnCount 
    IfLoadedNotEqualTo 0, Weather_Terminate

    ; For each weather, don''t try to set it if it''s already active from the field.
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_SUN, Weather_Sun
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_RAIN, Weather_Rain
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_SANDSTORM, Weather_Sand
    IfCurrentMoveEffectEqualTo BATTLE_EFFECT_WEATHER_HAIL, Weather_Hail

Weather_Sun:
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SUNNY, Weather_Terminate
    GoTo Weather_ScorePlus5

Weather_Rain:
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_RAINING, Weather_Terminate
    GoTo Weather_ScorePlus5

Weather_Sand:
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_SANDSTORM, Weather_Terminate
    GoTo Weather_ScorePlus5

Weather_Hail:
    LoadCurrentWeather 
    IfLoadedEqualTo AI_WEATHER_HAILING, Weather_Terminate
    GoTo Weather_ScorePlus5

Weather_ScorePlus5:
    ; On the attacker''s first turn only, score +5.
    LoadIsFirstTurnInBattle AI_BATTLER_ATTACKER
    IfLoadedEqualTo FALSE, Weather_Terminate
    AddToMoveScore 5

Weather_Terminate:
    PopOrEnd 

Harrassment_Main:
    IfTargetIsPartner Terminate

    ; If the move is not judged to be a Harrassment move within the context
    ; of this routine, break.
    LoadCurrentMoveEffect 
    IfLoadedNotInTable Harrassment_Effects, Harrassment_Terminate

    ; 50% of the time, score +2.
    IfRandomLessThan 128, Harrassment_Terminate
    AddToMoveScore 2

Harrassment_Terminate:
    PopOrEnd 

Harrassment_Effects:
    TableEntry BATTLE_EFFECT_STATUS_SLEEP
    TableEntry BATTLE_EFFECT_ATK_DOWN
    TableEntry BATTLE_EFFECT_DEF_DOWN
    TableEntry BATTLE_EFFECT_ACC_DOWN
    TableEntry BATTLE_EFFECT_EVA_DOWN
    TableEntry BATTLE_EFFECT_STATUS_CONFUSE
    TableEntry BATTLE_EFFECT_ATK_DOWN_2
    TableEntry BATTLE_EFFECT_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_SPEED_DOWN_2
    TableEntry BATTLE_EFFECT_SP_DEF_DOWN_2
    TableEntry BATTLE_EFFECT_STATUS_POISON
    TableEntry BATTLE_EFFECT_STATUS_PARALYZE
    TableEntry BATTLE_EFFECT_STATUS_LEECH_SEED
    TableEntry BATTLE_EFFECT_ENCORE
    TableEntry BATTLE_EFFECT_DECREASE_LAST_MOVE_PP
    TableEntry BATTLE_EFFECT_SET_SPIKES
    TableEntry BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION
    TableEntry BATTLE_EFFECT_INFATUATE
    TableEntry BATTLE_EFFECT_TORMENT
    TableEntry BATTLE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION
    TableEntry BATTLE_EFFECT_STATUS_BURN
    TableEntry BATTLE_EFFECT_NATURE_POWER
    TableEntry BATTLE_EFFECT_STATUS_SLEEP_NEXT_TURN
    TableEntry BATTLE_EFFECT_REMOVE_HELD_ITEM
    TableEntry BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE
    TableEntry BATTLE_EFFECT_SECRET_POWER
    TableEntry BATTLE_EFFECT_CONFUSE_ALL
    TableEntry BATTLE_EFFECT_ATK_DEF_DOWN
    TableEntry BATTLE_EFFECT_CAMOUFLAGE
    TableEntry BATTLE_EFFECT_PREVENT_ITEM_USE
    TableEntry BATTLE_EFFECT_TRANSFER_STATUS
    TableEntry BATTLE_EFFECT_TOXIC_SPIKES
    TableEntry BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN
    TableEntry BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER
    TableEntry TABLE_END

RoamingPokemon_Main:
    ; If the Roamer is trapped, break from this routine
    ; Otherwise, override all other possible moves and Escape
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_BIND, RoamingPokemon_Trapped
    IfVolatileStatus AI_BATTLER_ATTACKER, VOLATILE_CONDITION_MEAN_LOOK, RoamingPokemon_Trapped
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_SHADOW_TAG, RoamingPokemon_Trapped
    LoadAbility AI_BATTLER_ATTACKER
    IfLoadedEqualTo ABILITY_LEVITATE, RoamingPokemon_NotTrapped
    LoadAbility AI_BATTLER_DEFENDER
    IfLoadedEqualTo ABILITY_ARENA_TRAP, RoamingPokemon_Trapped

RoamingPokemon_NotTrapped:
    Escape 

RoamingPokemon_Trapped:
    PopOrEnd 

Safari_Main:
    Dummy3E 1
    Dummy3F 
    Escape 

CatchTutorial_Main:
    ; If the target is at 20% or less HP, flee from the battle
    IfHPPercentEqualTo AI_BATTLER_DEFENDER, 20, CatchTutorial_Escape
    IfHPPercentLessThan AI_BATTLER_DEFENDER, 20, CatchTutorial_Escape
    PopOrEnd 

CatchTutorial_Escape:
    Escape 

Terminate:
    PopOrEnd 

    .endif
	
.ifndef ASM_BATTLE_SCRIPT_INC

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