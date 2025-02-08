#include <nitro.h>
#include <string.h>

#include "consts/abilities.h"
#include "consts/gender.h"

#include "constants/battle.h"
#include "constants/flavor.h"
#include "constants/heap.h"
#include "constants/items.h"
#include "constants/sound.h"
#include "constants/species.h"
#include "constants/string.h"
#include "constants/trainer.h"

#include "struct_decls/struct_party_decl.h"
#include "struct_decls/battle_system.h"
#include "struct_decls/struct_02098700_decl.h"
#include "struct_defs/fraction.h"
#include "battle/struct_ov16_0225BFFC_decl.h"

#include "battle/battle_context.h"
#include "battle/battle_controller.h"
#include "battle/battle_message.h"
#include "battle/battle_mon.h"
#include "battle/common.h"
#include "battle/scripts/sub_seq.naix"

#include "unk_020021B0.h"
#include "narc.h"
#include "heap.h"
#include "strbuf.h"
#include "trainer_info.h"
#include "unk_020366A0.h"
#include "pokemon.h"
#include "move_table.h"
#include "trainer_data.h"
#include "party.h"
#include "item.h"
#include "unk_0208C098.h"
#include "unk_02098700.h"
#include "unk_02098988.h"
#include "flags.h"
#include "battle/ov16_0223DF00.h"
#include "battle/battle_lib.h"
#include "battle/battle_display.h"
#include "battle/battle_io.h"
#include "data/terrain/to_move.h"
#include "data/battle/weight_to_power.h"

static BOOL BasicTypeMulApplies(BattleContext *battleCtx, int attacker, int defender, int chartEntry);
static int MapSideEffectToSubscript(BattleContext *battleCtx, enum SideEffectType type, u32 effect);
static int ApplyTypeMultiplier(BattleContext *battleCtx, int attacker, int mul, int damage, BOOL update, u32 *moveStatus);
static BOOL NoImmunityOverrides(BattleContext *battleCtx, int itemEffect, int chartEntry);
static void UpateMoveStatusForTypeMul(int mul, u32 *moveStatusMask);
static BOOL MoveIsOnDamagingTurn(BattleContext *battleCtx, int move);
static u8 Battler_MonType(BattleContext *battleCtx, int battler, enum BattleMonParam paramID);
static void BattleAI_ClearKnownMoves(BattleContext *battleCtx, u8 battler);
static void BattleAI_ClearKnownAbility(BattleContext *battleCtx, u8 battler);
static void BattleAI_ClearKnownItem(BattleContext *battleCtx, u8 battler);
static int ChooseTraceTarget(BattleSystem *battleSys, BattleContext *battleCtx, int defender1, int defender2);
static BOOL MoveCannotTriggerAnticipation(BattleContext *battleCtx, int move);
static int CalcMoveType(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int item, int move);

static const Fraction sStatStageBoosts[];

void BattleSystem_InitBattleMon(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int partySlot)
{
    Pokemon *mon = BattleSystem_PartyPokemon(battleSys, battler, partySlot);

    battleCtx->battleMons[battler].species = Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL);
    battleCtx->battleMons[battler].attack = Pokemon_GetValue(mon, MON_DATA_ATK, NULL);
    battleCtx->battleMons[battler].defense = Pokemon_GetValue(mon, MON_DATA_DEF, NULL);
    battleCtx->battleMons[battler].speed = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
    battleCtx->battleMons[battler].spAttack = Pokemon_GetValue(mon, MON_DATA_SP_ATK, NULL);
    battleCtx->battleMons[battler].spDefense = Pokemon_GetValue(mon, MON_DATA_SP_DEF, NULL);

    int i;
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        battleCtx->battleMons[battler].moves[i] = Pokemon_GetValue(mon, MON_DATA_MOVE1 + i, NULL);
        battleCtx->battleMons[battler].ppCur[i] = Pokemon_GetValue(mon, MON_DATA_MOVE1_CUR_PP + i, NULL);
        battleCtx->battleMons[battler].ppUps[i] = Pokemon_GetValue(mon, MON_DATA_MOVE1_PP_UPS + i, NULL);
    }

    battleCtx->battleMons[battler].hpIV = Pokemon_GetValue(mon, MON_DATA_HP_IV, NULL);
    battleCtx->battleMons[battler].attackIV = Pokemon_GetValue(mon, MON_DATA_ATK_IV, NULL);
    battleCtx->battleMons[battler].defenseIV = Pokemon_GetValue(mon, MON_DATA_DEF_IV, NULL);
    battleCtx->battleMons[battler].speedIV = Pokemon_GetValue(mon, MON_DATA_SPEED_IV, NULL);
    battleCtx->battleMons[battler].spAttackIV = Pokemon_GetValue(mon, MON_DATA_SPATK_IV, NULL);
    battleCtx->battleMons[battler].spDefenseIV = Pokemon_GetValue(mon, MON_DATA_SPDEF_IV, NULL);
    battleCtx->battleMons[battler].isEgg = Pokemon_GetValue(mon, MON_DATA_IS_EGG, NULL);
    battleCtx->battleMons[battler].hasNickname = Pokemon_GetValue(mon, MON_DATA_HAS_NICKNAME, NULL);

    if ((battleCtx->battleStatusMask & SYSCTL_BATON_PASS) == FALSE) {
        for (i = 0; i < BATTLE_STAT_MAX; i++) {
            battleCtx->battleMons[battler].statBoosts[i] = 6;
        }
    }

    battleCtx->battleMons[battler].weatherAbilityAnnounced = FALSE;
    battleCtx->battleMons[battler].intimidateAnnounced = FALSE;
    battleCtx->battleMons[battler].traceAnnounced = FALSE;
    battleCtx->battleMons[battler].downloadAnnounced = FALSE;
    battleCtx->battleMons[battler].anticipationAnnounced = FALSE;
    battleCtx->battleMons[battler].forewarnAnnounced = FALSE;
    battleCtx->battleMons[battler].slowStartAnnounced = FALSE;
    battleCtx->battleMons[battler].slowStartFinished = FALSE;
    battleCtx->battleMons[battler].friskAnnounced = FALSE;
    battleCtx->battleMons[battler].moldBreakerAnnounced = FALSE;
    battleCtx->battleMons[battler].pressureAnnounced = FALSE;
	battleCtx->battleMons[battler].defiantFlag = FALSE;
	battleCtx->battleMons[battler].wpolicyFlag = FALSE;
	battleCtx->battleMons[battler].sheerForceFlag = FALSE;
	battleCtx->battleMons[battler].airBalloonAnnounced = FALSE;
	battleCtx->battleMons[battler].imposterFlag = FALSE;
	battleCtx->battleMons[battler].rivalryFlag = FALSE;
	battleCtx->battleMons[battler].colorChangeFlag = FALSE;
    battleCtx->battleMons[battler].meditateCritBoostFlag = FALSE;
	battleCtx->battleMons[battler].unownEnergyStrongFlag = FALSE;
	battleCtx->battleMons[battler].unownEnergyWeakFlag = FALSE;
	battleCtx->battleMons[battler].randomAbilityAnnounced = FALSE;
    battleCtx->battleMons[battler].type1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
    battleCtx->battleMons[battler].type2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
    battleCtx->battleMons[battler].gender = Pokemon_GetGender(mon);
    battleCtx->battleMons[battler].isShiny = Pokemon_IsShiny(mon);

    if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_NO_ABILITIES) {
        battleCtx->battleMons[battler].ability = ABILITY_NONE;
        battleCtx->battleMons[battler].status = MON_CONDITION_NONE;
        battleCtx->battleMons[battler].heldItem = ITEM_NONE;
    } else {
        battleCtx->battleMons[battler].ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
        battleCtx->battleMons[battler].status = Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL);
        battleCtx->battleMons[battler].heldItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
    }

    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_NO_ABILITIES) && Battler_Side(battleSys, battler) == BATTLER_US) {
        battleCtx->battleMons[battler].formNum = 0;
    } else {
        battleCtx->battleMons[battler].formNum = Pokemon_GetValue(mon, MON_DATA_FORM, NULL);
    }

    battleCtx->battleMons[battler].level = Pokemon_GetValue(mon, MON_DATA_LEVEL, NULL);
    battleCtx->battleMons[battler].friendship = Pokemon_GetValue(mon, MON_DATA_FRIENDSHIP, NULL);
    battleCtx->battleMons[battler].curHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
    battleCtx->battleMons[battler].maxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);
    battleCtx->battleMons[battler].exp = Pokemon_GetValue(mon, MON_DATA_EXP, NULL);
    battleCtx->battleMons[battler].personality = Pokemon_GetValue(mon, MON_DATA_PERSONALITY, NULL);
    battleCtx->battleMons[battler].OTId = Pokemon_GetValue(mon, MON_DATA_OT_ID, NULL);
    battleCtx->battleMons[battler].OTGender = Pokemon_GetValue(mon, MON_DATA_OT_GENDER, NULL);
    battleCtx->battleMons[battler].capturedBall = Pokemon_GetValue(mon, MON_DATA_POKEBALL, NULL);

    Pokedex_SetupGiratina(battleCtx->battleMons[battler].formNum);

    HeightWeightData *heightWeightData = Pokedex_HeightWeightData(HEAP_ID_BATTLE);
    Pokedex_HeightWeightData_Load(heightWeightData, 0, HEAP_ID_BATTLE);

    battleCtx->battleMons[battler].weight = Pokedex_HeightWeightData_Weight(heightWeightData, battleCtx->battleMons[battler].species);

    Pokedex_HeightWeightData_Release(heightWeightData);
    Pokedex_HeightWeightData_Free(heightWeightData);
    Pokemon_GetValue(mon, MON_DATA_NICKNAME, battleCtx->battleMons[battler].nickname);
    Pokemon_GetValue(mon, MON_DATA_OTNAME, battleCtx->battleMons[battler].OTName);

    battleCtx->battleMons[battler].timesDamaged = 0;
    battleCtx->battleMons[battler].trainerMessageFlags = 0;

    int side = Battler_Side(battleSys, battler);
    if (battleCtx->sideConditions[side].knockedOffItemsMask & FlagIndex(battleCtx->selectedPartySlot[battler])) {
        battleCtx->battleMons[battler].heldItem = ITEM_NONE;
        battleCtx->battleMons[battler].moveEffectsData.canUnburden = FALSE;
    } else if (battleCtx->battleMons[battler].heldItem) {
        battleCtx->battleMons[battler].moveEffectsData.canUnburden = TRUE;
    }
}

void BattleSystem_ReloadPokemon(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int partySlot)
{
    Pokemon *mon = BattleSystem_PartyPokemon(battleSys, battler, partySlot);

    battleCtx->battleMons[battler].attack = Pokemon_GetValue(mon, MON_DATA_ATK, NULL);
    battleCtx->battleMons[battler].defense = Pokemon_GetValue(mon, MON_DATA_DEF, NULL);
    battleCtx->battleMons[battler].speed = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
    battleCtx->battleMons[battler].spAttack = Pokemon_GetValue(mon, MON_DATA_SP_ATK, NULL);
    battleCtx->battleMons[battler].spDefense = Pokemon_GetValue(mon, MON_DATA_SP_DEF, NULL);
    battleCtx->battleMons[battler].level = Pokemon_GetValue(mon, MON_DATA_LEVEL, NULL);
    battleCtx->battleMons[battler].friendship = Pokemon_GetValue(mon, MON_DATA_FRIENDSHIP, NULL);
    battleCtx->battleMons[battler].curHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
    battleCtx->battleMons[battler].maxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);

    if ((battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_TRANSFORM) == FALSE) {
        for (int i = 0; i < LEARNED_MOVES_MAX; i++) {
            if ((battleCtx->battleMons[battler].moveEffectsData.mimickedMoveSlot & FlagIndex(i)) == FALSE) {
                battleCtx->battleMons[battler].moves[i] = Pokemon_GetValue(mon, MON_DATA_MOVE1 + i, NULL);
                battleCtx->battleMons[battler].ppCur[i] = Pokemon_GetValue(mon, MON_DATA_MOVE1_CUR_PP + i, NULL);
                battleCtx->battleMons[battler].ppUps[i] = Pokemon_GetValue(mon, MON_DATA_MOVE1_PP_UPS + i, NULL);
            }
        }

        battleCtx->battleMons[battler].exp = Pokemon_GetValue(mon, MON_DATA_EXP, NULL);
    }
}

void BattleSystem_LoadScript(BattleContext *battleCtx, int narc, int file)
{
    GF_ASSERT(NARC_GetMemberSizeByIndexPair(narc, file) < BATTLE_SCRIPT_SIZE_MAX * sizeof(u32));

    battleCtx->scriptNarc = narc;
    battleCtx->scriptFile = file;
    battleCtx->scriptCursor = 0;

    NARC_ReadWholeMemberByIndexPair(&battleCtx->battleScript, narc, file);
}

void BattleSystem_CallScript(BattleContext *battleCtx, int narc, int file)
{
    GF_ASSERT(NARC_GetMemberSizeByIndexPair(narc, file) < 400 * 4);
    GF_ASSERT(battleCtx->scriptStackPointer < 4);

    battleCtx->scriptStackNarc[battleCtx->scriptStackPointer] = battleCtx->scriptNarc;
    battleCtx->scriptStackFile[battleCtx->scriptStackPointer] = battleCtx->scriptFile;
    battleCtx->scriptStackCursor[battleCtx->scriptStackPointer] = battleCtx->scriptCursor;
    battleCtx->scriptStackPointer++;
    battleCtx->scriptNarc = narc;
    battleCtx->scriptFile = file;
    battleCtx->scriptCursor = 0;

    NARC_ReadWholeMemberByIndexPair(&battleCtx->battleScript, narc, file);
}

BOOL BattleSystem_PopScript(BattleContext *battleCtx)
{
    if (battleCtx->scriptStackPointer) {
        battleCtx->scriptStackPointer--;
        BattleSystem_LoadScript(battleCtx, battleCtx->scriptStackNarc[battleCtx->scriptStackPointer], battleCtx->scriptStackFile[battleCtx->scriptStackPointer]);

        battleCtx->scriptCursor = battleCtx->scriptStackCursor[battleCtx->scriptStackPointer];
        return FALSE;
    }
    
    return TRUE;
}

void BattleIO_EnqueueVal(BattleContext *battleCtx, int id, int battler, int val)
{
    int i;

    for (i = 0; i < BATTLE_IO_QUEUE_SIZE; i++) {
        if (battleCtx->ioQueue[id][battler][i] == 0) {
            battleCtx->ioQueue[id][battler][i] = val;
            break;
        }
    }

    GF_ASSERT(i < BATTLE_IO_QUEUE_SIZE);
}

void BattleIO_DequeueVal(BattleContext *battleCtx, int id, int battler, int val)
{
    int i;

    GF_ASSERT(val != 0);

    for (i = 0; i < BATTLE_IO_QUEUE_SIZE; i++) {
        if (battleCtx->ioQueue[id][battler][i] == val) {
            battleCtx->ioQueue[id][battler][i] = 0;
            break;
        }
    }

    GF_ASSERT(i < BATTLE_IO_QUEUE_SIZE);
}

BOOL BattleIO_QueueIsEmpty(BattleContext *battleCtx)
{
    int linkedPlayer, battler, j;
    int sumData = 0;

    for (linkedPlayer = 0; linkedPlayer < MAX_LINK_BATTLERS; linkedPlayer++) {
        for (battler = 0; battler < MAX_BATTLERS; battler++) {
            for (j = 0; j < BATTLE_IO_QUEUE_SIZE; j++) {
                sumData += battleCtx->ioQueue[linkedPlayer][battler][j];
            }
        }
    }

    if (sumData == 0) {
        battleCtx->linkBattleTimeout = 0;
    }

    return sumData == 0;
}

void BattleIO_UpdateTimeout(BattleContext *battleCtx)
{
    battleCtx->linkBattleTimeout++;
    if (battleCtx->linkBattleTimeout > LINK_BATTLE_TIMEOUT) {
        Link_SetErrorState(LINK_BATTLE_RESET_SAVEPOINT);
    }
}

void BattleIO_ClearBuffer(BattleContext *battleCtx, int battler)
{
    for (int i = 0; i < BATTLE_IO_BUFFER_SIZE; i++) {
        battleCtx->ioBuffer[battler][i] = 0;
    }
}

int BattleMon_Get(BattleContext *battleCtx, int battler, enum BattleMonParam paramID, void *buf)
{
    BattleMon *battleMon = &battleCtx->battleMons[battler];

    switch (paramID) {
    case BATTLEMON_SPECIES:
        return battleMon->species;

    case BATTLEMON_ATTACK:
        return battleMon->attack;

    case BATTLEMON_DEFENSE:
        return battleMon->defense;

    case BATTLEMON_SPEED:
        return battleMon->speed;

    case BATTLEMON_SP_ATTACK:
        return battleMon->spAttack;

    case BATTLEMON_SP_DEFENSE:
        return battleMon->spDefense;

    case BATTLEMON_MOVE_1:
    case BATTLEMON_MOVE_2:
    case BATTLEMON_MOVE_3:
    case BATTLEMON_MOVE_4:
        return battleMon->moves[paramID - BATTLEMON_MOVE_1];

    case BATTLEMON_HP_IV:
        return battleMon->hpIV;

    case BATTLEMON_ATTACK_IV:
        return battleMon->attackIV;

    case BATTLEMON_DEFENSE_IV:
        return battleMon->defenseIV;

    case BATTLEMON_SPEED_IV:
        return battleMon->speedIV;

    case BATTLEMON_SP_ATTACK_IV:
        return battleMon->spAttackIV;

    case BATTLEMON_SP_DEFENSE_IV:
        return battleMon->spDefenseIV;

    case BATTLEMON_IS_EGG:
        return battleMon->isEgg;

    case BATTLEMON_HAS_NICKNAME:
        return battleMon->hasNickname;

    case BATTLEMON_HP_STAGE:
    case BATTLEMON_ATTACK_STAGE:
    case BATTLEMON_DEFENSE_STAGE:
    case BATTLEMON_SPEED_STAGE:
    case BATTLEMON_SP_ATTACK_STAGE:
    case BATTLEMON_SP_DEFENSE_STAGE:
    case BATTLEMON_ACCURACY_STAGE:
    case BATTLEMON_EVASION_STAGE:
        return battleMon->statBoosts[paramID - BATTLEMON_HP_STAGE];

    case BATTLEMON_ABILITY:
        return battleMon->ability;

    case BATTLEMON_TYPE_1:
    case BATTLEMON_TYPE_2:
        return Battler_MonType(battleCtx, battler, paramID);

    case BATTLEMON_GENDER:
        return battleMon->gender;

    case BATTLEMON_IS_SHINY:
        return battleMon->isShiny;

    case BATTLEMON_CUR_PP_1:
    case BATTLEMON_CUR_PP_2:
    case BATTLEMON_CUR_PP_3:
    case BATTLEMON_CUR_PP_4:
        return battleMon->ppCur[paramID - BATTLEMON_CUR_PP_1];

    case BATTLEMON_PP_UPS_1:
    case BATTLEMON_PP_UPS_2:
    case BATTLEMON_PP_UPS_3:
    case BATTLEMON_PP_UPS_4:
        return battleMon->ppUps[paramID - BATTLEMON_PP_UPS_1];

    case BATTLEMON_MAX_PP_1:
    case BATTLEMON_MAX_PP_2:
    case BATTLEMON_MAX_PP_3:
    case BATTLEMON_MAX_PP_4:
        return MoveTable_CalcMaxPP(battleMon->moves[paramID - BATTLEMON_MAX_PP_1], battleMon->ppUps[paramID - BATTLEMON_MAX_PP_1]);

    case BATTLEMON_LEVEL:
        return battleMon->level;

    case BATTLEMON_FRIENDSHIP:
        return battleMon->friendship;

    case BATTLEMON_NICKNAME:
        {
            int i;
            charcode_t *nickname = buf;

            for (i = 0; i < MON_NAME_LEN + 1; i++) {
                nickname[i] = battleMon->nickname[i];
            }
        }
        break;

    case BATTLEMON_NICKNAME_STRBUF:
        Strbuf_CopyChars((Strbuf *)buf, battleMon->nickname);
        break;

    case BATTLEMON_CUR_HP:
        return battleMon->curHP;

    case BATTLEMON_MAX_HP:
        return battleMon->maxHP;

    case BATTLEMON_OT_NAME:
        {
            int i;
            charcode_t *OTName = buf;

            for (i = 0; i < MON_NAME_LEN + 1; i++) {
                OTName[i] = battleMon->OTName[i];
            }
        }
        break;

    case BATTLEMON_EXP:
        return battleMon->exp;

    case BATTLEMON_PERSONALITY:
        return battleMon->personality;

    case BATTLEMON_STATUS:
        return battleMon->status;

    case BATTLEMON_VOLATILE_STATUS:
        return battleMon->statusVolatile;

    case BATTLEMON_OT_ID:
        return battleMon->OTId;

    case BATTLEMON_HELD_ITEM:
        return battleMon->heldItem;

    case BATTLEMON_TIMES_DAMAGED:
        return battleMon->timesDamaged;

    case BATTLEMON_TRAINER_MESSAGE_FLAGS:
        return battleMon->trainerMessageFlags;

    case BATTLEMON_OT_GENDER:
        return battleMon->OTGender;

    case BATTLEMON_MOVE_EFFECTS_MASK:
        return battleMon->moveEffectsMask;

    case BATTLEMON_MOVE_EFFECTS_TEMP:
        return battleMon->moveEffectsTemp;

    case BATTLEMON_DISABLED_TURNS:
        return battleMon->moveEffectsData.disabledTurns;

    case BATTLEMON_ENCORED_TURNS:
        return battleMon->moveEffectsData.encoredTurns;

    case BATTLEMON_CHARGED_TURNS:
        return battleMon->moveEffectsData.chargedTurns;

    case BATTLEMON_TAUNTED_TURNS:
        return battleMon->moveEffectsData.tauntedTurns;

    case BATTLEMON_SUCCESSFUL_PROTECT_TURNS:
        return battleMon->moveEffectsData.protectSuccessTurns;

    case BATTLEMON_PERISH_SONG_TURNS:
        return battleMon->moveEffectsData.perishSongTurns;

    case BATTLEMON_ROLLOUT_COUNT:
        return battleMon->moveEffectsData.rolloutCount;

    case BATTLEMON_FURY_CUTTER_COUNT:
        return battleMon->moveEffectsData.furyCutterCount;

    case BATTLEMON_STOCKPILE_COUNT:
        return battleMon->moveEffectsData.stockpileCount;

    case BATTLEMON_STOCKPILE_DEF_BOOSTS:
        return battleMon->moveEffectsData.stockpileDefBoosts;

    case BATTLEMON_STOCKPILE_SPDEF_BOOSTS:
        return battleMon->moveEffectsData.stockpileSpDefBoosts;

    case BATTLEMON_TRUANT:
        return battleMon->moveEffectsData.truant;

    case BATTLEMON_FLASH_FIRE:
        return battleMon->moveEffectsData.flashFire;

    case BATTLEMON_LOCK_ON_TARGET:
        return battleMon->moveEffectsData.lockOnTarget;

    case BATTLEMON_MIMICKED_MOVE_SLOT:
        return battleMon->moveEffectsData.mimickedMoveSlot;

    case BATTLEMON_BIND_TARGET:
        return battleMon->moveEffectsData.bindTarget;

    case BATTLEMON_MEAN_LOOK_TARGET:
        return battleMon->moveEffectsData.meanLookTarget;

    case BATTLEMON_LAST_RESORT_COUNT:
        return battleMon->moveEffectsData.lastResortCount;

    case BATTLEMON_MAGNET_RISE_TURNS:
        return battleMon->moveEffectsData.magnetRiseTurns;

    case BATTLEMON_HEAL_BLOCK_TURNS:
        return battleMon->moveEffectsData.healBlockTurns;

    case BATTLEMON_EMBARGO_TURNS:
        return battleMon->moveEffectsData.embargoTurns;

    case BATTLEMON_CAN_UNBURDEN:
        return battleMon->moveEffectsData.canUnburden;

    case BATTLEMON_METRONOME_TURNS:
        return battleMon->moveEffectsData.metronomeTurns;

    case BATTLEMON_MICLE_BERRY:
        return battleMon->moveEffectsData.micleBerry;

    case BATTLEMON_CUSTAP_BERRY:
        return battleMon->moveEffectsData.custapBerry;

    case BATTLEMON_QUICK_CLAW:
        return battleMon->moveEffectsData.quickClaw;

    case BATTLEMON_RECHARGE_TURN_NUMBER:
        return battleMon->moveEffectsData.rechargeTurnNumber;

    case BATTLEMON_FAKE_OUT_TURN_NUMBER:
        return battleMon->moveEffectsData.fakeOutTurnNumber;

    case BATTLEMON_SLOW_START_TURN_NUMBER:
        return battleMon->moveEffectsData.slowStartTurnNumber;

    case BATTLEMON_SUBSTITUTE_HP:
        return battleMon->moveEffectsData.substituteHP;

    case BATTLEMON_TRANSFORMED_PERSONALITY:
        return battleMon->moveEffectsData.transformedPID;

    case BATTLEMON_DISABLED_MOVE:
        return battleMon->moveEffectsData.disabledMove;

    case BATTLEMON_ENCORED_MOVE:
        return battleMon->moveEffectsData.encoredMove;

    case BATTLEMON_BINDING_MOVE:
        return battleMon->moveEffectsData.bindingMove;

    case BATTLEMON_ITEM_HP_RECOVERY:
        return battleMon->moveEffectsData.itemHPRecovery;

    case BATTLEMON_SLOW_START_ANNOUNCED:
        return battleMon->slowStartAnnounced;

    case BATTLEMON_SLOW_START_FINISHED:
        return battleMon->slowStartFinished;

    case BATTLEMON_FORM_NUM:
        return battleMon->formNum;

    case BATTLEMON_TEMP:
        return BattleMon_Get(battleCtx, battler, battleCtx->scriptTemp, buf);

    case BATTLEMON_MEDITATE_CRIT_RATE_BOOST:
        return battleMon->meditateCritBoostFlag;
		
	case BATTLEMON_UNOWN_ENERGY_STRONG_FLAG:
        return battleMon->unownEnergyStrongFlag;
		
	case BATTLEMON_UNOWN_ENERGY_WEAK_FLAG:
        return battleMon->unownEnergyWeakFlag;
		
	case BATTLEMON_HEAL_INVERSION_TURNS:
        return battleMon->moveEffectsData.healInversionTurns;

    default:
        GF_ASSERT(FALSE);
        break;
    }

    return 0;
}

void BattleMon_Set(BattleContext *battleCtx, int battler, enum BattleMonParam paramID, const void *buf)
{
    BattleMon *mon = &battleCtx->battleMons[battler];

    switch (paramID) {
    case BATTLEMON_SPECIES:
        mon->species = *(u16 *)buf;
        break;

    case BATTLEMON_ATTACK:
        mon->attack = *(u16 *)buf;
        break;

    case BATTLEMON_DEFENSE:
        mon->defense = *(u16 *)buf;
        break;

    case BATTLEMON_SPEED:
        mon->speed = *(u16 *)buf;
        break;

    case BATTLEMON_SP_ATTACK:
        mon->spAttack = *(u16 *)buf;
        break;

    case BATTLEMON_SP_DEFENSE:
        mon->spDefense = *(u16 *)buf;
        break;

    case BATTLEMON_MOVE_1:
    case BATTLEMON_MOVE_2:
    case BATTLEMON_MOVE_3:
    case BATTLEMON_MOVE_4:
        mon->moves[paramID - BATTLEMON_MOVE_1] = *(u16 *)buf;
        break;

    case BATTLEMON_HP_IV:
        mon->hpIV = *(u8 *)buf;
        break;

    case BATTLEMON_ATTACK_IV:
        mon->attackIV = *(u8 *)buf;
        break;

    case BATTLEMON_DEFENSE_IV:
        mon->defenseIV = *(u8 *)buf;
        break;

    case BATTLEMON_SPEED_IV:
        mon->speedIV = *(u8 *)buf;
        break;

    case BATTLEMON_SP_ATTACK_IV:
        mon->spAttackIV = *(u8 *)buf;
        break;

    case BATTLEMON_SP_DEFENSE_IV:
        mon->spDefenseIV = *(u8 *)buf;
        break;

    case BATTLEMON_IS_EGG:
        mon->isEgg = *(u8 *)buf;
        break;

    case BATTLEMON_HAS_NICKNAME:
        mon->hasNickname = *(u8 *)buf;
        break;

    case BATTLEMON_HP_STAGE:
    case BATTLEMON_ATTACK_STAGE:
    case BATTLEMON_DEFENSE_STAGE:
    case BATTLEMON_SPEED_STAGE:
    case BATTLEMON_SP_ATTACK_STAGE:
    case BATTLEMON_SP_DEFENSE_STAGE:
    case BATTLEMON_ACCURACY_STAGE:
    case BATTLEMON_EVASION_STAGE:
        mon->statBoosts[paramID - BATTLEMON_HP_STAGE] = *(s8 *)buf;
        break;

    case BATTLEMON_ABILITY:
        mon->ability = *(u8 *)buf;
        break;

    case BATTLEMON_TYPE_1:
        mon->type1 = *(u8 *)buf;
        break;

    case BATTLEMON_TYPE_2:
        mon->type2 = *(u8 *)buf;
        break;

    case BATTLEMON_GENDER:
        mon->gender = *(u8 *)buf;
        break;

    case BATTLEMON_IS_SHINY:
        mon->isShiny = *(u8 *)buf;
        break;

    case BATTLEMON_CUR_PP_1:
    case BATTLEMON_CUR_PP_2:
    case BATTLEMON_CUR_PP_3:
    case BATTLEMON_CUR_PP_4:
        mon->ppCur[paramID - BATTLEMON_CUR_PP_1] = *(u8 *)buf;
        break;

    case BATTLEMON_PP_UPS_1:
    case BATTLEMON_PP_UPS_2:
    case BATTLEMON_PP_UPS_3:
    case BATTLEMON_PP_UPS_4:
        mon->ppUps[paramID - BATTLEMON_PP_UPS_1] = *(u8 *)buf;
        break;
        
    case BATTLEMON_MAX_PP_1:
    case BATTLEMON_MAX_PP_2:
    case BATTLEMON_MAX_PP_3:
    case BATTLEMON_MAX_PP_4:
        GF_ASSERT(FALSE);
        break;

    case BATTLEMON_LEVEL:
        mon->level = *(u8 *)buf;
        break;

    case BATTLEMON_FRIENDSHIP:
        mon->friendship = *(u8 *)buf;
        break;
        
    case BATTLEMON_NICKNAME:
        for (int i = 0; i < MON_NAME_LEN + 1; i++) {
            mon->nickname[i] = ((u16 *)buf)[i];
        }
        break;

    case BATTLEMON_CUR_HP:
        mon->curHP = *(s16 *)buf;
        break;

    case BATTLEMON_MAX_HP:
        mon->maxHP = *(u16 *)buf;
        break;

    case BATTLEMON_OT_NAME:
        for (int i = 0; i < MON_NAME_LEN + 1; i++) {
            mon->OTName[i] = ((u16 *)buf)[i];
        }
        break;

    case BATTLEMON_EXP:
        mon->exp = *(u32 *)buf;
        break;

    case BATTLEMON_PERSONALITY:
        mon->personality = *(u32 *)buf;
        break;
    
    case BATTLEMON_STATUS:
        mon->status = *(u32 *)buf;
        break;

    case BATTLEMON_VOLATILE_STATUS:
        mon->statusVolatile = *(u32 *)buf;
        break;

    case BATTLEMON_OT_ID:
        mon->OTId = *(u32 *)buf;
        break;

    case BATTLEMON_HELD_ITEM:
        mon->heldItem = *(u16 *)buf;
        break;

    case BATTLEMON_TIMES_DAMAGED:
        mon->timesDamaged = *(u8 *)buf;
        break;

    case BATTLEMON_TRAINER_MESSAGE_FLAGS:
        mon->trainerMessageFlags = *(u8 *)buf;
        break;

    case BATTLEMON_OT_GENDER:
        mon->OTGender = *(u8 *)buf;
        break;
    case BATTLEMON_MOVE_EFFECTS_MASK:
        mon->moveEffectsMask = *(u32 *)buf;
        break;

    case BATTLEMON_MOVE_EFFECTS_TEMP:
        mon->moveEffectsTemp = *(u32 *)buf;
        break;

    case BATTLEMON_DISABLED_TURNS:
        mon->moveEffectsData.disabledTurns = *(u8 *)buf;
        break;

    case BATTLEMON_ENCORED_TURNS:
        mon->moveEffectsData.encoredTurns = *(u8 *)buf;
        break;

    case BATTLEMON_CHARGED_TURNS:
        mon->moveEffectsData.chargedTurns = *(u8 *)buf;
        break;

    case BATTLEMON_TAUNTED_TURNS:
        mon->moveEffectsData.tauntedTurns = *(u8 *)buf;
        break;

    case BATTLEMON_SUCCESSFUL_PROTECT_TURNS:
        mon->moveEffectsData.protectSuccessTurns = *(u8 *)buf;
        break;

    case BATTLEMON_PERISH_SONG_TURNS:
        mon->moveEffectsData.perishSongTurns = *(u8 *)buf;
        break;

    case BATTLEMON_ROLLOUT_COUNT:
        mon->moveEffectsData.rolloutCount = *(u8 *)buf;
        break;

    case BATTLEMON_FURY_CUTTER_COUNT:
        mon->moveEffectsData.furyCutterCount = *(u8 *)buf;
        break;

    case BATTLEMON_STOCKPILE_COUNT:
        mon->moveEffectsData.stockpileCount = *(u8 *)buf;
        break;

    case BATTLEMON_STOCKPILE_DEF_BOOSTS:
        mon->moveEffectsData.stockpileDefBoosts = *(u8 *)buf;
        break;

    case BATTLEMON_STOCKPILE_SPDEF_BOOSTS:
        mon->moveEffectsData.stockpileSpDefBoosts = *(u8 *)buf;
        break;

    case BATTLEMON_TRUANT:
        mon->moveEffectsData.truant = *(u8 *)buf;
        break;

    case BATTLEMON_FLASH_FIRE:
        mon->moveEffectsData.flashFire = *(u8 *)buf;
        break;

    case BATTLEMON_LOCK_ON_TARGET:
        mon->moveEffectsData.lockOnTarget = *(u8 *)buf;
        break;

    case BATTLEMON_MIMICKED_MOVE_SLOT:
        mon->moveEffectsData.mimickedMoveSlot = *(u8 *)buf;
        break;

    case BATTLEMON_BIND_TARGET:
        mon->moveEffectsData.bindTarget = *(u8 *)buf;
        break;

    case BATTLEMON_MEAN_LOOK_TARGET:
        mon->moveEffectsData.meanLookTarget = *(u8 *)buf;
        break;

    case BATTLEMON_LAST_RESORT_COUNT:
        mon->moveEffectsData.lastResortCount = *(u8 *)buf;
        break;

    case BATTLEMON_MAGNET_RISE_TURNS:
        mon->moveEffectsData.magnetRiseTurns = *(u8 *)buf;
        break;

    case BATTLEMON_HEAL_BLOCK_TURNS:
        mon->moveEffectsData.healBlockTurns = *(u8 *)buf;
        break;

    case BATTLEMON_EMBARGO_TURNS:
        mon->moveEffectsData.embargoTurns = *(u8 *)buf;
        break;

    case BATTLEMON_CAN_UNBURDEN:
        mon->moveEffectsData.canUnburden = *(u8 *)buf;
        break;

    case BATTLEMON_METRONOME_TURNS:
        mon->moveEffectsData.metronomeTurns = *(u8 *)buf;
        break;

    case BATTLEMON_MICLE_BERRY:
        mon->moveEffectsData.micleBerry = *(u8 *)buf;
        break;

    case BATTLEMON_CUSTAP_BERRY:
        mon->moveEffectsData.custapBerry = *(u8 *)buf;
        break;

    case BATTLEMON_QUICK_CLAW:
        mon->moveEffectsData.quickClaw = *(u8 *)buf;
        break;

    case BATTLEMON_RECHARGE_TURN_NUMBER:
        mon->moveEffectsData.rechargeTurnNumber = *(u32 *)buf;
        break;

    case BATTLEMON_FAKE_OUT_TURN_NUMBER:
        mon->moveEffectsData.fakeOutTurnNumber = *(u32 *)buf;
        break;

    case BATTLEMON_SLOW_START_TURN_NUMBER:
        mon->moveEffectsData.slowStartTurnNumber = *(u32 *)buf;
        break;

    case BATTLEMON_SUBSTITUTE_HP:
        mon->moveEffectsData.substituteHP = *(u32 *)buf;
        break;

    case BATTLEMON_TRANSFORMED_PERSONALITY:
        mon->moveEffectsData.transformedPID = *(u32 *)buf;
        break;

    case BATTLEMON_DISABLED_MOVE:
        mon->moveEffectsData.disabledMove = *(u16 *)buf;
        break;

    case BATTLEMON_ENCORED_MOVE:
        mon->moveEffectsData.encoredMove = *(u16 *)buf;
        break;

    case BATTLEMON_BINDING_MOVE:
        mon->moveEffectsData.bindingMove = *(u16 *)buf;
        break;

    case BATTLEMON_ITEM_HP_RECOVERY:
        mon->moveEffectsData.itemHPRecovery = *(u32 *)buf;
        break;

    case BATTLEMON_SLOW_START_ANNOUNCED:
        mon->slowStartAnnounced = *(u8 *)buf;
        break;

    case BATTLEMON_SLOW_START_FINISHED:
        mon->slowStartFinished = *(u8 *)buf;
        break;

    case BATTLEMON_FORM_NUM:
        mon->formNum = *(u8 *)buf;
        break;

    case BATTLEMON_TEMP:
        BattleMon_Set(battleCtx, battler, battleCtx->scriptTemp, buf);
        break;
		
	case BATTLEMON_MEDITATE_CRIT_RATE_BOOST:
        mon->meditateCritBoostFlag = *(u8 *)buf;
		break;
		
	case BATTLEMON_UNOWN_ENERGY_STRONG_FLAG:
        mon->unownEnergyStrongFlag = *(u8 *)buf;
		break;
		
	case BATTLEMON_UNOWN_ENERGY_WEAK_FLAG:
        mon->unownEnergyWeakFlag = *(u8 *)buf;
		break;
		
	case BATTLEMON_HEAL_INVERSION_TURNS:
        mon->moveEffectsData.healInversionTurns = *(u8 *)buf;
        break;

    default:
        GF_ASSERT(FALSE);
        break;
    }
}

void Battler_AddVal(BattleContext *battleCtx, int battler, enum BattleMonParam paramID, int val)
{
    BattleMon_AddVal(&battleCtx->battleMons[battler], paramID, val);
}

void BattleMon_AddVal(BattleMon *mon, enum BattleMonParam paramID, int val)
{
    switch (paramID) {
    case BATTLEMON_ATTACK:
        mon->attack += val;
        break;

    case BATTLEMON_DEFENSE:
        mon->defense += val;
        break;

    case BATTLEMON_SPEED:
        mon->speed += val;
        break;

    case BATTLEMON_SP_ATTACK:
        mon->spAttack += val;
        break;

    case BATTLEMON_SP_DEFENSE:
        mon->spDefense += val;
        break;

    case BATTLEMON_HP_IV:
        mon->hpIV += val;
        break;

    case BATTLEMON_ATTACK_IV:
        mon->attackIV += val;
        break;

    case BATTLEMON_DEFENSE_IV:
        mon->defenseIV += val;
        break;

    case BATTLEMON_SPEED_IV:
        mon->speedIV += val;
        break;

    case BATTLEMON_SP_ATTACK_IV:
        mon->spAttackIV += val;
        break;

    case BATTLEMON_SP_DEFENSE_IV:
        mon->spDefenseIV += val;
        break;

    case BATTLEMON_HP_STAGE:
    case BATTLEMON_ATTACK_STAGE:
    case BATTLEMON_DEFENSE_STAGE:
    case BATTLEMON_SPEED_STAGE:
    case BATTLEMON_SP_ATTACK_STAGE:
    case BATTLEMON_SP_DEFENSE_STAGE:
    case BATTLEMON_ACCURACY_STAGE:
    case BATTLEMON_EVASION_STAGE:
        if (mon->statBoosts[paramID - BATTLEMON_HP_STAGE] + val < 0) {
            mon->statBoosts[paramID - BATTLEMON_HP_STAGE] = 0;
        } else if (mon->statBoosts[paramID - BATTLEMON_HP_STAGE] + val > 12) {
            mon->statBoosts[paramID - BATTLEMON_HP_STAGE] = 12;
        } else {
            mon->statBoosts[paramID - BATTLEMON_HP_STAGE] += val;
        }
        break;

    case BATTLEMON_CUR_PP_1:
    case BATTLEMON_CUR_PP_2:
    case BATTLEMON_CUR_PP_3:
    case BATTLEMON_CUR_PP_4:
        int maxPP = MoveTable_CalcMaxPP(mon->moves[paramID - BATTLEMON_CUR_PP_1], mon->ppUps[paramID - BATTLEMON_CUR_PP_1]);
        if (mon->ppCur[paramID - BATTLEMON_CUR_PP_1] + val > maxPP) {
            mon->ppCur[paramID - BATTLEMON_CUR_PP_1] = maxPP;
        } else {
            mon->ppCur[paramID - BATTLEMON_CUR_PP_1] += val;
        }
        break;

    case BATTLEMON_PP_UPS_1:
    case BATTLEMON_PP_UPS_2:
    case BATTLEMON_PP_UPS_3:
    case BATTLEMON_PP_UPS_4:
        mon->ppUps[paramID - BATTLEMON_PP_UPS_1] += val;
        break;

    case BATTLEMON_LEVEL:
        mon->level += val;
        break;

    case BATTLEMON_FRIENDSHIP:
        int friendship = mon->friendship;

        if (friendship + val > 255) {
            friendship = 255;
        } else if (friendship + val < 0) {
            friendship = 0;
        } else {
            friendship += val;
        }

        mon->friendship = friendship;
        break;

    case BATTLEMON_CUR_HP:
        if (mon->curHP + val > mon->maxHP) {
            mon->curHP = mon->maxHP;
        } else {
            mon->curHP += val;
        }
        break;

    case BATTLEMON_MAX_HP:
        mon->maxHP += val;
        break;

    case BATTLEMON_EXP:
        mon->exp += val;
        break;

    case BATTLEMON_PERSONALITY:
        mon->personality += val;
        break;

    case BATTLEMON_DISABLED_TURNS:
        mon->moveEffectsData.disabledTurns += val;
        break;

    case BATTLEMON_ENCORED_TURNS:
        mon->moveEffectsData.encoredTurns += val;
        break;

    case BATTLEMON_CHARGED_TURNS:
        mon->moveEffectsData.chargedTurns += val;
        break;

    case BATTLEMON_TAUNTED_TURNS:
        mon->moveEffectsData.tauntedTurns += val;
        break;

    case BATTLEMON_SUCCESSFUL_PROTECT_TURNS:
        mon->moveEffectsData.protectSuccessTurns += val;
        break;

    case BATTLEMON_PERISH_SONG_TURNS:
        mon->moveEffectsData.perishSongTurns += val;
        break;

    case BATTLEMON_ROLLOUT_COUNT:
        mon->moveEffectsData.rolloutCount += val;
        break;

    case BATTLEMON_FURY_CUTTER_COUNT:
        mon->moveEffectsData.furyCutterCount += val;
        break;

    case BATTLEMON_STOCKPILE_COUNT:
        mon->moveEffectsData.stockpileCount += val;
        break;

    case BATTLEMON_STOCKPILE_DEF_BOOSTS:
        mon->moveEffectsData.stockpileDefBoosts += val;
        break;

    case BATTLEMON_STOCKPILE_SPDEF_BOOSTS:
        mon->moveEffectsData.stockpileSpDefBoosts += val;
        break;

    case BATTLEMON_LAST_RESORT_COUNT:
        mon->moveEffectsData.lastResortCount += val;
        break;

    case BATTLEMON_MAGNET_RISE_TURNS:
        mon->moveEffectsData.magnetRiseTurns += val;
        break;

    case BATTLEMON_HEAL_BLOCK_TURNS:
        mon->moveEffectsData.healBlockTurns += val;
        break;

    case BATTLEMON_RECHARGE_TURN_NUMBER:
        mon->moveEffectsData.rechargeTurnNumber += val;
        break;

    case BATTLEMON_FAKE_OUT_TURN_NUMBER:
        mon->moveEffectsData.fakeOutTurnNumber += val;
        break;

    case BATTLEMON_SLOW_START_TURN_NUMBER:
        mon->moveEffectsData.slowStartTurnNumber += val;
        break;

    case BATTLEMON_SUBSTITUTE_HP:
        mon->moveEffectsData.substituteHP += val;
        break;

    case BATTLEMON_ITEM_HP_RECOVERY:
        mon->moveEffectsData.itemHPRecovery += val;
        break;

    case BATTLEMON_SLOW_START_ANNOUNCED:
        mon->slowStartAnnounced += val;
        break;

    case BATTLEMON_SLOW_START_FINISHED:
        mon->slowStartFinished += val;
        break;

    case BATTLEMON_FORM_NUM:
        mon->formNum += val;
        break;
		
	case BATTLEMON_MEDITATE_CRIT_RATE_BOOST:
        mon->meditateCritBoostFlag += val;
		break;
		
	case BATTLEMON_UNOWN_ENERGY_STRONG_FLAG:
        mon->unownEnergyStrongFlag += val;
		break;
		
	case BATTLEMON_UNOWN_ENERGY_WEAK_FLAG:
        mon->unownEnergyWeakFlag += val;
		break;
		
	case BATTLEMON_HEAL_INVERSION_TURNS:
        mon->moveEffectsData.healInversionTurns += val;
        break;

    default:
        GF_ASSERT(FALSE);
        break;
    }
}

static const u8 sSpeedHalvingItemEffects[] = {
    HOLD_EFFECT_EVS_UP_SPEED_DOWN,
    HOLD_EFFECT_SPEED_DOWN_GROUNDED,
    HOLD_EFFECT_LVLUP_HP_EV_UP,
    HOLD_EFFECT_LVLUP_ATK_EV_UP,
    HOLD_EFFECT_LVLUP_DEF_EV_UP,
    HOLD_EFFECT_LVLUP_SPEED_EV_UP,
    HOLD_EFFECT_LVLUP_SPATK_EV_UP,
    HOLD_EFFECT_LVLUP_SPDEF_EV_UP
};

static inline int CompareSpeed_ApplySimple(BattleContext *battleCtx, int battler, int stage)
{
    if (Battler_Ability(battleCtx, battler) == ABILITY_SIMPLE) {
        stage = 6 + ((stage - 6) * 2);

        if (stage > 12) {
            stage = 12;
        }

        if (stage < 0) {
            stage = 0;
        }
    }

    return stage;
}

u8 BattleSystem_CompareBattlerSpeed(BattleSystem *battleSys, BattleContext *battleCtx, int battler1, int battler2, BOOL ignoreQuickClaw)
{
    u8 result = COMPARE_SPEED_FASTER;
    u32 battler1Speed, battler2Speed;
    u16 battler1Move = 0, battler2Move = 0;
    u8 battler1ItemEffect, battler1ItemParam;
    u8 battler2ItemEffect, battler2ItemParam;
    s8 battler1Priority = 0, battler2Priority = 0;
    u8 battler1QuickClaw = 0, battler2QuickClaw = 0;
    u8 battler1LaggingTail = 0, battler2LaggingTail = 0;
    int battler1Action, battler2Action;
    int battler1MoveSlot, battler2MoveSlot;
    int battler1Ability, battler2Ability;
    int battler1SpeedStage, battler2SpeedStage;
	int battler1FlingEffect, battler2FlingEffect;
    int i;

    // If either battler is dead, short-circuit to preferring the other
    if (battleCtx->battleMons[battler1].curHP == 0 && battleCtx->battleMons[battler2].curHP) {
        return 1;
    }

    if (battleCtx->battleMons[battler1].curHP && battleCtx->battleMons[battler2].curHP == 0) {
        return 0;
    }

    battler1Ability = Battler_Ability(battleCtx, battler1);
    battler2Ability = Battler_Ability(battleCtx, battler2);
    battler1ItemEffect = Battler_HeldItemEffect(battleCtx, battler1);
    battler1ItemParam = Battler_HeldItemPower(battleCtx, battler1, ITEM_POWER_CHECK_ALL);
    battler2ItemEffect = Battler_HeldItemEffect(battleCtx, battler2);
    battler2ItemParam = Battler_HeldItemPower(battleCtx, battler2, ITEM_POWER_CHECK_ALL);
    battler1SpeedStage = battleCtx->battleMons[battler1].statBoosts[BATTLE_STAT_SPEED];
    battler2SpeedStage = battleCtx->battleMons[battler2].statBoosts[BATTLE_STAT_SPEED];
	battler1FlingEffect = Battler_ItemFlingEffect(battleCtx, battler1);
	battler2FlingEffect = Battler_ItemFlingEffect(battleCtx, battler2);

    battler1SpeedStage = CompareSpeed_ApplySimple(battleCtx, battler1, battler1SpeedStage);
    battler2SpeedStage = CompareSpeed_ApplySimple(battleCtx, battler2, battler2SpeedStage);

    battler1Speed = battleCtx->battleMons[battler1].speed * sStatStageBoosts[battler1SpeedStage].numerator / sStatStageBoosts[battler1SpeedStage].denominator;
    battler2Speed = battleCtx->battleMons[battler2].speed * sStatStageBoosts[battler2SpeedStage].numerator / sStatStageBoosts[battler2SpeedStage].denominator;
	
	battler1Action = battleCtx->battlerActions[battler1][BATTLE_ACTION_SELECTED_COMMAND];
    battler2Action = battleCtx->battlerActions[battler2][BATTLE_ACTION_SELECTED_COMMAND];
    battler1MoveSlot = battleCtx->moveSlot[battler1];
    battler2MoveSlot = battleCtx->moveSlot[battler2];
	battler1Move = BattleMon_Get(battleCtx, battler1, BATTLEMON_MOVE_1 + battler1MoveSlot, NULL);
	battler2Move = BattleMon_Get(battleCtx, battler2, BATTLEMON_MOVE_1 + battler2MoveSlot, NULL);

    if (ANY_WEATHER) {
        // Rain
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
            switch (battler1Ability) {
                default:
                    break;

                case ABILITY_SWIFT_SWIM:
                    battler1Speed = battler1Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler1Speed = battler1Speed * 3 / 4;
                    break;
            }
            switch (battler2Ability) {
                default:
                    break;

                case ABILITY_SWIFT_SWIM:
                    battler2Speed = battler2Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler2Speed = battler2Speed * 3 / 4;
                    break;
            }
        }

        // Hail
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING) {
            switch (battler1Ability) {
                default:
                    break;

                case ABILITY_SLUSH_RUSH:
                    battler1Speed = battler1Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler1Speed = battler1Speed * 3 / 4;
                    break;
            }
            switch (battler2Ability) {
                default:
                    break;

                case ABILITY_SLUSH_RUSH:
                    battler2Speed = battler2Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler2Speed = battler2Speed * 3 / 4;
                    break;
            }
        }

        // Sun
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
            switch (battler1Ability) {
                default:
                    break;

                case ABILITY_CHLOROPLAST:
                case ABILITY_CHLOROPHYLL:
                    battler1Speed = battler1Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler1Speed = battler1Speed * 11 / 10;
                    break;
            }
            switch (battler2Ability) {
                default:
                    break;

                case ABILITY_CHLOROPLAST:
                case ABILITY_CHLOROPHYLL:
                    battler2Speed = battler2Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler2Speed = battler2Speed * 11 / 10;
                    break;
            }
        }

        // Sand
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SANDSTORM) {
            switch (battler1Ability) {
                default:
                    break;

                case ABILITY_FORECAST:
                    battler1Speed = battler1Speed * 3 / 4;
                    break;
            }
            switch (battler2Ability) {
                default:
                    break;

                case ABILITY_FORECAST:
                    battler2Speed = battler2Speed * 3 / 4;
                    break;
            }
        }
    }
    switch (battler1ItemEffect) {
        default:
            break;

        case HOLD_EFFECT_EVS_UP_SPEED_DOWN:
        case HOLD_EFFECT_SPEED_DOWN_GROUNDED:
        case HOLD_EFFECT_LVLUP_HP_EV_UP:
        case HOLD_EFFECT_LVLUP_ATK_EV_UP:
        case HOLD_EFFECT_LVLUP_DEF_EV_UP:
        case HOLD_EFFECT_LVLUP_SPEED_EV_UP:
        case HOLD_EFFECT_LVLUP_SPATK_EV_UP:
        case HOLD_EFFECT_LVLUP_SPDEF_EV_UP:
            battler1Speed /= 2;
            break;

        case HOLD_EFFECT_CHOICE_SPEED:
            battler1Speed = battler1Speed * 3 / 2;
            break;

        case HOLD_EFFECT_DITTO_SPEED_UP:
            if (battleCtx->battleMons[battler1].species == SPECIES_DITTO) {
                battler1Speed *= 2;
            }
            break;

        case HOLD_EFFECT_SOMETIMES_PRIORITY:
            if (battleCtx->speedRand[battler1] % (100 / battler1ItemParam) == 0) {
                battler1QuickClaw = 1;

                if (ignoreQuickClaw == FALSE) {
                    battleCtx->battleMons[battler1].moveEffectsData.quickClaw = 1;
                }
            }
            break;

        case HOLD_EFFECT_PINCH_PRIORITY:
            if (battler1Ability == ABILITY_GLUTTONY) {
                battler1ItemParam /= 2;
            }

            if (battleCtx->battleMons[battler1].curHP <= (battleCtx->battleMons[battler1].maxHP / battler1ItemParam)) {
                battler1QuickClaw = 1;

                if (ignoreQuickClaw == FALSE) {
                    battleCtx->battleMons[battler1].moveEffectsData.custapBerry = 1;
                }
            }
            break;

        case HOLD_EFFECT_PRIORITY_DOWN:
            battler1LaggingTail = 1;
            break;
    }

    switch (battler1Ability) {
        default:
            break;

        case ABILITY_QUICK_FEET:
            if (battleCtx->battleMons[battler1].status & MON_CONDITION_ANY) {
                if (battleCtx->battleMons[battler1].status & MON_CONDITION_PARALYSIS) {
                    battler1Speed *= 2;
                }
                else {
                    battler1Speed = battler1Speed * 3 / 2;
                }
            }
            break;

        case ABILITY_SLOW_START:
            if (battleCtx->totalTurns - battleCtx->battleMons[battler1].moveEffectsData.slowStartTurnNumber < 5) {
                battler1Speed /= 2;
            }
            break;

        case ABILITY_UNBURDEN:
            if (battleCtx->battleMons[battler1].moveEffectsData.canUnburden
                && battleCtx->battleMons[battler1].heldItem == ITEM_NONE) {
                battler1Speed *= 2;
            }
            break;
			
		case ABILITY_COWARD:
            if (MOVE_DATA(battler1Move).power == 0)
			{
                battler1Speed *= 2;
            }
            break;
    }

    if ((battleCtx->battleMons[battler1].status & MON_CONDITION_PARALYSIS)
        && battler1Ability != ABILITY_QUICK_FEET) {
    
        battler1Speed /= 2;
    }

    if (battleCtx->sideConditionsMask[Battler_Side(battleSys, battler1)] & SIDE_CONDITION_TAILWIND) {
        battler1Speed *= 2;
    }

    
    switch (battler2ItemEffect) {
        default:
            break;

        case HOLD_EFFECT_EVS_UP_SPEED_DOWN:
        case HOLD_EFFECT_SPEED_DOWN_GROUNDED:
        case HOLD_EFFECT_LVLUP_HP_EV_UP:
        case HOLD_EFFECT_LVLUP_ATK_EV_UP:
        case HOLD_EFFECT_LVLUP_DEF_EV_UP:
        case HOLD_EFFECT_LVLUP_SPEED_EV_UP:
        case HOLD_EFFECT_LVLUP_SPATK_EV_UP:
        case HOLD_EFFECT_LVLUP_SPDEF_EV_UP:
            battler2Speed /= 2;
            break;

        case HOLD_EFFECT_CHOICE_SPEED:
            battler2Speed = battler2Speed * 3 / 2;
            break;

        case HOLD_EFFECT_DITTO_SPEED_UP:
            if (battleCtx->battleMons[battler2].species == SPECIES_DITTO) {
                battler2Speed *= 2;
            }
            break;

        case HOLD_EFFECT_SOMETIMES_PRIORITY:
            if (battleCtx->speedRand[battler2] % (100 / battler2ItemParam) == 0) {
                battler2QuickClaw = 1;

                if (ignoreQuickClaw == FALSE) {
                    battleCtx->battleMons[battler2].moveEffectsData.quickClaw = 1;
                }
            }
            break;

        case HOLD_EFFECT_PINCH_PRIORITY:
            if (battler2Ability == ABILITY_GLUTTONY) {
                battler2ItemParam /= 2;
            }

            if (battleCtx->battleMons[battler2].curHP <= (battleCtx->battleMons[battler2].maxHP / battler2ItemParam)) {
                battler2QuickClaw = 1;

                if (ignoreQuickClaw == FALSE) {
                    battleCtx->battleMons[battler2].moveEffectsData.custapBerry = 1;
                }
            }
            break;

        case HOLD_EFFECT_PRIORITY_DOWN:
            battler2LaggingTail = 1;
            break;
    }

    switch (battler2Ability) {
        default:
            break;

        case ABILITY_QUICK_FEET:
            if (battleCtx->battleMons[battler2].status & MON_CONDITION_ANY) {
                if (battleCtx->battleMons[battler2].status & MON_CONDITION_PARALYSIS) {
                    battler2Speed *= 2;
                }
                else {
                    battler2Speed = battler2Speed * 3 / 2;
                }
            }
            break;

        case ABILITY_SLOW_START:
            if (battleCtx->totalTurns - battleCtx->battleMons[battler2].moveEffectsData.slowStartTurnNumber < 5) {
                battler2Speed /= 2;
            }
            break;

        case ABILITY_UNBURDEN:
            if (battleCtx->battleMons[battler2].moveEffectsData.canUnburden
                && battleCtx->battleMons[battler2].heldItem == ITEM_NONE) {
                battler2Speed *= 2;
            }
            break;
			
		case ABILITY_COWARD:
            if (MOVE_DATA(battler2Move).power == 0)
			{
                battler2Speed *= 2;
            }
            break;
    }
	
	battler1Priority = MOVE_DATA(battler1Move).priority;
    battler2Priority = MOVE_DATA(battler2Move).priority;
	
	if ((battler1Move == MOVE_FLING)
	&& (battler1FlingEffect == FLING_EFFECT_TRICK_ROOM))
	{
		MOVE_DATA(battler1Move).priority = -7;
	}
	
	if ((battler2Move == MOVE_FLING)
	&& (battler2FlingEffect == FLING_EFFECT_TRICK_ROOM))
	{
		MOVE_DATA(battler2Move).priority = -7;
	}
	
	if ((battler1Move == MOVE_MAGNET_RISE)
	&& (battler1Ability == ABILITY_MAGNET_PULL))
	{
		MOVE_DATA(battler1Move).priority += 1;
	}
	
	if ((battler2Move == MOVE_MAGNET_RISE)
	&& (battler2Ability == ABILITY_MAGNET_PULL))
	{
		MOVE_DATA(battler2Move).priority += 1;
	}

    battleCtx->monSpeedValues[battler1] = battler1Speed;
    battleCtx->monSpeedValues[battler2] = battler2Speed;

    if (ignoreQuickClaw == FALSE) {
        battler1Action = battleCtx->battlerActions[battler1][BATTLE_ACTION_SELECTED_COMMAND];
        battler2Action = battleCtx->battlerActions[battler2][BATTLE_ACTION_SELECTED_COMMAND];
        battler1MoveSlot = battleCtx->moveSlot[battler1];
        battler2MoveSlot = battleCtx->moveSlot[battler2];

        if (battler1Action == PLAYER_INPUT_FIGHT) {
            if (battleCtx->turnFlags[battler1].struggling) {
                battler1Move = MOVE_STRUGGLE;
            } else {
                battler1Move = BattleMon_Get(battleCtx, battler1, BATTLEMON_MOVE_1 + battler1MoveSlot, NULL);
            }
        }

        if (battler2Action == PLAYER_INPUT_FIGHT) {
            if (battleCtx->turnFlags[battler2].struggling) {
                battler2Move = MOVE_STRUGGLE;
            } else {
                battler2Move = BattleMon_Get(battleCtx, battler2, BATTLEMON_MOVE_1 + battler2MoveSlot, NULL);
            }
        }

        battler1Priority = MOVE_DATA(battler1Move).priority;
        battler2Priority = MOVE_DATA(battler2Move).priority;
    }

    if (battler1Priority == battler2Priority) {
        if (battler1QuickClaw && battler2QuickClaw) {
            if (battler1Speed < battler2Speed) {
                result = COMPARE_SPEED_SLOWER;
            } else if (battler1Speed == battler2Speed && (BattleSystem_RandNext(battleSys) & 1)) {
                result = COMPARE_SPEED_TIE;
            }
        } else if (battler1QuickClaw == FALSE && battler2QuickClaw) {
            result = COMPARE_SPEED_SLOWER;
        } else if (battler1QuickClaw && battler2QuickClaw == FALSE) {
            result = COMPARE_SPEED_FASTER;
        } else if (battler1LaggingTail && battler2LaggingTail) {
            if (battler1Speed > battler2Speed) {
                result = COMPARE_SPEED_SLOWER;
            } else if (battler1Speed == battler2Speed && (BattleSystem_RandNext(battleSys) & 1)) {
                result = COMPARE_SPEED_TIE;
            }
        } else if (battler1LaggingTail && battler2LaggingTail == FALSE) {
            result = COMPARE_SPEED_SLOWER;
        } else if (battler1LaggingTail == FALSE && battler2LaggingTail) {
            result = COMPARE_SPEED_FASTER;
        } else if (battler1Ability == ABILITY_STALL && battler2Ability == ABILITY_STALL) {
            if (battler1Speed > battler2Speed) {
                result = COMPARE_SPEED_SLOWER;
            } else if (battler1Speed == battler2Speed && (BattleSystem_RandNext(battleSys) & 1)) {
                result = COMPARE_SPEED_TIE;
            }
        } else if (battler1Ability == ABILITY_STALL && battler2Ability != ABILITY_STALL) {
            result = COMPARE_SPEED_SLOWER;
        } else if (battler1Ability != ABILITY_STALL && battler2Ability == ABILITY_STALL) {
            result = COMPARE_SPEED_FASTER;
        } else if (battleCtx->fieldConditionsMask & FIELD_CONDITION_TRICK_ROOM) {
            if (battler1Speed > battler2Speed) {
                result = COMPARE_SPEED_SLOWER;
            }

            if (battler1Speed == battler2Speed && (BattleSystem_RandNext(battleSys) & 1)) {
                result = COMPARE_SPEED_TIE;
            }
        } else {
            if (battler1Speed < battler2Speed) {
                result = COMPARE_SPEED_SLOWER;
            }

            if (battler1Speed == battler2Speed && (BattleSystem_RandNext(battleSys) & 1)) {
                result = COMPARE_SPEED_TIE;
            }
        }
    } else if (battler1Priority < battler2Priority) {
        result = COMPARE_SPEED_SLOWER;
    }

    return result;
}


u8 BattleSystem_ComparePartyMonSpeed(BattleSystem *battleSys, BattleContext *battleCtx, int battler1, int battler2, int partyIndicator, int partySlot, BOOL ignoreQuickClaw)
{
    u8 result = COMPARE_SPEED_FASTER;
    u32 battler1Speed, battler2Speed;
    u16 battler1Move = 0, battler2Move = 0;
    u8 battler1ItemEffect, battler1ItemParam;
    u8 battler2ItemEffect, battler2ItemParam;
    s8 battler1Priority = 0, battler2Priority = 0;
    s8 battler1MaxPriority = 0, battler2MaxPriority = 0;
    u8 battler1QuickClaw = 0, battler2QuickClaw = 0;
    u8 battler1LaggingTail = 0, battler2LaggingTail = 0;
    u16 battler1Item, battler2Item;
    u32 moveStatusFlags;
    int battler1Action, battler2Action;
    int battler1MoveSlot, battler2MoveSlot;
	int battler1MoveType, battler2MoveType;
    int battler1Ability, battler2Ability;
    int battler1SpeedStage, battler2SpeedStage;
	int battler1FlingEffect, battler2FlingEffect;
    int i;
    int battler1MoveScore, battler2MoveScore;
    Pokemon *mon;

    mon = BattleSystem_PartyPokemon(battleSys, partyIndicator, partySlot);

    // If either battler is dead, short-circuit to preferring the other
    if (battleCtx->battleMons[battler1].curHP == 0 && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) != 0) {
        return 1;
    }

    if (battleCtx->battleMons[battler1].curHP && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) == 0) {
        return 0;
    }

    battler1Ability = Battler_Ability(battleCtx, battler1);
    battler2Ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    battler1Item = BattleMon_Get(battleCtx, battler1, BATTLEMON_HELD_ITEM, NULL);
    battler1ItemEffect = Battler_HeldItemEffect(battleCtx, battler1);
    battler1ItemParam = Battler_HeldItemPower(battleCtx, battler1, ITEM_POWER_CHECK_ALL);
    battler2Item = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
    battler2ItemEffect = BattleSystem_GetItemData(battleCtx, battler2Item, ITEM_PARAM_HOLD_EFFECT);
    battler2ItemParam = BattleSystem_GetItemData(battleCtx, battler2Item, ITEM_PARAM_HOLD_EFFECT_PARAM);
	battler1FlingEffect = Battler_ItemFlingEffect(battleCtx, battler1);
	battler2FlingEffect = BattleSystem_GetItemData(battleCtx, battler2Item, ITEM_PARAM_FLING_EFFECT);

    battler1SpeedStage = battleCtx->battleMons[battler1].statBoosts[BATTLE_STAT_SPEED];
    battler2SpeedStage = 6;

    // Factor in Sticky Web
    if (battleCtx->sideConditionsMask[Battler_Side(battleSys, partyIndicator)] & SIDE_CONDITION_STICKY_WEB) {
        if (battler2Ability != ABILITY_LEVITATE
        && battler2Ability != ABILITY_CLEAR_BODY
        && battler2Ability != ABILITY_WHITE_SMOKE
        && Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL) != TYPE_FLYING
        && Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL) != TYPE_FLYING) {
            if (battler2Ability == ABILITY_SIMPLE) {
                battler2SpeedStage -= 2;
            }
            else {
                battler2SpeedStage -= 1;
            }
        }
    }

    battler1SpeedStage = CompareSpeed_ApplySimple(battleCtx, battler1, battler1SpeedStage);

    battler1Speed = battleCtx->battleMons[battler1].speed * sStatStageBoosts[battler1SpeedStage].numerator / sStatStageBoosts[battler1SpeedStage].denominator;
    battler2Speed = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL) * sStatStageBoosts[battler2SpeedStage].numerator / sStatStageBoosts[battler2SpeedStage].denominator;

    if (ANY_WEATHER) {
        // Rain
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
            switch (battler1Ability) {
                default:
                    break;

                case ABILITY_SWIFT_SWIM:
                    battler1Speed = battler1Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler1Speed = battler1Speed * 3 / 4;
                    break;
            }
            switch (battler2Ability) {
                default:
                    break;

                case ABILITY_SWIFT_SWIM:
                    battler2Speed = battler2Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler2Speed = battler2Speed * 3 / 4;
                    break;
            }
        }

        // Hail
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING) {
            switch (battler1Ability) {
                default:
                    break;

                case ABILITY_SLUSH_RUSH:
                    battler1Speed = battler1Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler1Speed = battler1Speed * 3 / 4;
                    break;
            }
            switch (battler2Ability) {
                default:
                    break;

                case ABILITY_SLUSH_RUSH:
                    battler2Speed = battler2Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler2Speed = battler2Speed * 3 / 4;
                    break;
            }
        }

        // Sun
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
            switch (battler1Ability) {
                default:
                    break;

                case ABILITY_CHLOROPLAST:
                case ABILITY_CHLOROPHYLL:
                    battler1Speed = battler1Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler1Speed = battler1Speed * 11 / 10;
                    break;
            }
            switch (battler2Ability) {
                default:
                    break;

                case ABILITY_CHLOROPLAST:
                case ABILITY_CHLOROPHYLL:
                    battler2Speed = battler2Speed * 5 / 3;
                    break;

                case ABILITY_FORECAST:
                    battler2Speed = battler2Speed * 11 / 10;
                    break;
            }
        }

        // Sand
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SANDSTORM) {
            switch (battler1Ability) {
                default:
                    break;

                case ABILITY_FORECAST:
                    battler1Speed = battler1Speed * 3 / 4;
                    break;
            }
            switch (battler2Ability) {
                default:
                    break;

                case ABILITY_FORECAST:
                    battler2Speed = battler2Speed * 3 / 4;
                    break;
            }
        }
    }

    switch (battler1ItemEffect) {
        default:
            break;

        case HOLD_EFFECT_EVS_UP_SPEED_DOWN:
        case HOLD_EFFECT_SPEED_DOWN_GROUNDED:
        case HOLD_EFFECT_LVLUP_HP_EV_UP:
        case HOLD_EFFECT_LVLUP_ATK_EV_UP:
        case HOLD_EFFECT_LVLUP_DEF_EV_UP:
        case HOLD_EFFECT_LVLUP_SPEED_EV_UP:
        case HOLD_EFFECT_LVLUP_SPATK_EV_UP:
        case HOLD_EFFECT_LVLUP_SPDEF_EV_UP:
            battler1Speed /= 2;
            break;

        case HOLD_EFFECT_CHOICE_SPEED:
            battler1Speed = battler1Speed * 3 / 2;
            break;

        case HOLD_EFFECT_DITTO_SPEED_UP:
            if (battleCtx->battleMons[battler1].species == SPECIES_DITTO) {
                battler1Speed *= 2;
            }
            break;

        case HOLD_EFFECT_SOMETIMES_PRIORITY:
            if (battleCtx->speedRand[battler1] % (100 / battler1ItemParam) == 0) {
                battler1QuickClaw = 1;

                if (ignoreQuickClaw == FALSE) {
                    battleCtx->battleMons[battler1].moveEffectsData.quickClaw = 1;
                }
            }
            break;

        case HOLD_EFFECT_PINCH_PRIORITY:
            if (battler1Ability == ABILITY_GLUTTONY) {
                battler1ItemParam /= 2;
            }

            if (battleCtx->battleMons[battler1].curHP <= (battleCtx->battleMons[battler1].maxHP / battler1ItemParam)) {
                battler1QuickClaw = 1;

                if (ignoreQuickClaw == FALSE) {
                    battleCtx->battleMons[battler1].moveEffectsData.custapBerry = 1;
                }
            }
            break;

        case HOLD_EFFECT_PRIORITY_DOWN:
            battler1LaggingTail = 1;
            break;
    }

    switch (battler1Ability) {
        default:
            break;

        case ABILITY_QUICK_FEET:
            if (battleCtx->battleMons[battler1].status & MON_CONDITION_ANY) {
                if (battleCtx->battleMons[battler1].status & MON_CONDITION_PARALYSIS) {
                    battler1Speed *= 2;
                }
                else {
                    battler1Speed = battler1Speed * 3 / 2;
                }
            }
            break;

        case ABILITY_SLOW_START:
            if (battleCtx->totalTurns - battleCtx->battleMons[battler1].moveEffectsData.slowStartTurnNumber < 5) {
                battler1Speed /= 2;
            }
            break;

        case ABILITY_UNBURDEN:
            if (battleCtx->battleMons[battler1].moveEffectsData.canUnburden
                && battleCtx->battleMons[battler1].heldItem == ITEM_NONE) {
                battler1Speed *= 2;
            }
            break;
    }

    if ((battleCtx->battleMons[battler1].status & MON_CONDITION_PARALYSIS)
        && battler1Ability != ABILITY_QUICK_FEET) {
    
        battler1Speed /= 2;
    }

    if (battleCtx->sideConditionsMask[Battler_Side(battleSys, battler1)] & SIDE_CONDITION_TAILWIND) {
        battler1Speed *= 2;
    }

    
    switch (battler2ItemEffect) {
        default:
            break;

        case HOLD_EFFECT_EVS_UP_SPEED_DOWN:
        case HOLD_EFFECT_SPEED_DOWN_GROUNDED:
        case HOLD_EFFECT_LVLUP_HP_EV_UP:
        case HOLD_EFFECT_LVLUP_ATK_EV_UP:
        case HOLD_EFFECT_LVLUP_DEF_EV_UP:
        case HOLD_EFFECT_LVLUP_SPEED_EV_UP:
        case HOLD_EFFECT_LVLUP_SPATK_EV_UP:
        case HOLD_EFFECT_LVLUP_SPDEF_EV_UP:
            battler2Speed /= 2;
            break;

        case HOLD_EFFECT_CHOICE_SPEED:
            battler2Speed = battler2Speed * 3 / 2;
            break;

        case HOLD_EFFECT_DITTO_SPEED_UP:
            if (Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL) == SPECIES_DITTO) {
                battler2Speed *= 2;
            }
            break;

        case HOLD_EFFECT_SOMETIMES_PRIORITY:
            if (battleCtx->speedRand[partyIndicator] % (100 / battler2ItemParam) == 0) {
                battler2QuickClaw = 1;
            }
            break;

        case HOLD_EFFECT_PRIORITY_DOWN:
            battler2LaggingTail = 1;
            break;
    }

    switch (battler2Ability) {
        default:
            break;

        case ABILITY_QUICK_FEET:
            if (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY) {
                if (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_PARALYSIS) {
                    battler2Speed *= 2;
                }
                else {
                    battler2Speed = battler2Speed * 3 / 2;
                }
            }
            break;

        case ABILITY_SLOW_START:
            battler2Speed /= 2;
            break;
    }

    if ((Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_PARALYSIS)
        && battler2Ability != ABILITY_QUICK_FEET) {
    
        battler2Speed /= 2;
    }

    if (battleCtx->sideConditionsMask[Battler_Side(battleSys, partyIndicator)] & SIDE_CONDITION_TAILWIND) {
        battler2Speed *= 2;
    }

    
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        battler1Move = BattleMon_Get(battleCtx, battler1, BATTLEMON_MOVE_1 + i, NULL);
        battler1MoveType = CalcMoveType(battleSys, battleCtx, battler1, battler1Item, battler1Move);

        switch (battler1Move) {
            default:
                break;

            case MOVE_FLING:
                if (battler1FlingEffect == FLING_EFFECT_TRICK_ROOM) {
                    MOVE_DATA(battler1Move).priority = -7;
                }
                break;

            case MOVE_MAGNET_RISE:
                if (battler1Ability == ABILITY_MAGNET_PULL) {
                    MOVE_DATA(battler1Move).priority += 1;
                }
                break;

            case MOVE_FAKE_OUT:
                if (battleCtx->totalTurns > battleCtx->battleMons[battler1].moveEffectsData.fakeOutTurnNumber) {
                    MOVE_DATA(battler1Move).priority = 0;
                }
                break;
        }

        if (MOVE_DATA(battler1Move).priority > 0) {
            if (MOVE_DATA(battler1Move).power > 0) {
                moveStatusFlags = 0;

                battler1MoveScore = BattleSystem_CalcPartyMemberMoveDamage(
                                    battleSys,
                                    battleCtx,
                                    battler1Move,
                                    battleCtx->sideConditionsMask[Battler_Side(battleSys, partyIndicator)],
                                    battleCtx->fieldConditionsMask,
                                    0,
                                    battler1MoveType,
                                    battler1,
                                    battler2,
                                    1,
                                    partyIndicator,
                                    partySlot);

                battler1MoveScore = PartyMon_ApplyTypeChart(battleSys,
                                    battleCtx,
                                    battler1Move,
                                    battler1MoveType,
                                    battler1,
                                    battler2,
                                    battler1MoveScore,
                                    partyIndicator,
                                    partySlot,
                                    &moveStatusFlags);

                if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                    && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                    battler1MoveScore = 0;
                }

                if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                    battler1MoveScore /= 2;
                }

                if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                    battler1MoveScore = battler1MoveScore * 3 / 2;
                }

                if (battler1MoveScore > BattleMon_Get(battleCtx, battler1, BATTLEMON_CUR_HP, NULL) * 2 / 5) {
                    if (battler1MaxPriority < MOVE_DATA(battler1Move).priority) {
                        battler1MaxPriority = MOVE_DATA(battler1Move).priority;
                    }
                }
            }
        }
    }

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        battler2Move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + i, NULL);
        battler2MoveType = CalcMoveType(battleSys, battleCtx, battler2, battler2Item, battler2Move);

        switch (battler2Move) {
            default:
                break;

            case MOVE_FLING:
                if (battler2FlingEffect == FLING_EFFECT_TRICK_ROOM) {
                    MOVE_DATA(battler2Move).priority = -7;
                }
                break;

            case MOVE_MAGNET_RISE:
                if (battler2Ability == ABILITY_MAGNET_PULL) {
                    MOVE_DATA(battler2Move).priority += 1;
                }
                break;
        }

        if (MOVE_DATA(battler2Move).priority > 0) {
            if (MOVE_DATA(battler2Move).power > 0) {
                moveStatusFlags = 0;

                battler2MoveScore = BattleSystem_CalcPartyMemberMoveDamage(
                                    battleSys,
                                    battleCtx,
                                    battler2Move,
                                    battleCtx->sideConditionsMask[Battler_Side(battleSys, battler1)],
                                    battleCtx->fieldConditionsMask,
                                    0,
                                    battler2MoveType,
                                    battler2,
                                    battler1,
                                    1,
                                    partyIndicator,
                                    partySlot);

                battler2MoveScore = PartyMon_ApplyTypeChart(battleSys,
                                    battleCtx,
                                    battler2Move,
                                    battler2MoveType,
                                    battler2,
                                    battler1,
                                    battler2MoveScore,
                                    partyIndicator,
                                    partySlot,
                                    &moveStatusFlags);

                if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                    && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                    battler2MoveScore = 0;
                }

                if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                    battler2MoveScore /= 2;
                }

                if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                    battler2MoveScore = battler2MoveScore * 3 / 2;
                }

                if (battler2MoveScore > Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL) * 2 / 5) {
                    if (battler2MaxPriority < MOVE_DATA(battler2Move).priority) {
                        battler2MaxPriority = MOVE_DATA(battler2Move).priority;
                    }
                }
            }
        }
    }

    if (battler1MaxPriority == battler2MaxPriority) {
        if (battleCtx->fieldConditionsMask & FIELD_CONDITION_TRICK_ROOM) {
            if (battler1Speed > battler2Speed) {
                result = COMPARE_SPEED_SLOWER;
            }
        }
        else {
            if (battler1Speed < battler2Speed) {
                result = COMPARE_SPEED_SLOWER;
            }
        }

        if (battler1Speed == battler2Speed) {
            result = COMPARE_SPEED_TIE;
        }

        if (battler1LaggingTail && battler2LaggingTail == FALSE) {
            result = COMPARE_SPEED_SLOWER;
        }
    }

    if (battler1MaxPriority < battler2MaxPriority) {
        result = COMPARE_SPEED_SLOWER;
    }

    // Default value is COMPARE_SPEED_FASTER
    return result;
}

void BattleSystem_ClearSideExpGain(BattleContext *battleCtx, int battler)
{
    battleCtx->sideGetExpMask[(battler >> 1) & 1] = 0;
}

void BattleSystem_FlagBattlerExpGain(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int side = 0;
    u32 battleType = BattleSystem_BattleType(battleSys);

    while (side <= 2) {
        if ((battleCtx->battlersSwitchingMask & FlagIndex(side)) == FALSE
                && (battleCtx->battlersSwitchingMask & FlagIndex(battler)) == FALSE
                && battleCtx->battleMons[battler].curHP) {
            battleCtx->sideGetExpMask[(battler >> 1) & 1] |= FlagIndex(battleCtx->selectedPartySlot[side]);
        }

        side += 2;

        if (battleType == BATTLE_TYPE_AI_PARTNER || battleType == BATTLE_TYPE_TRAINER_WITH_AI_PARTNER) {
            break;
        }
    }
}

BOOL BattleSystem_TriggerPrimaryEffect(BattleSystem *battleSys, BattleContext *battleCtx, int *effect)
{
    BOOL result = FALSE;

    if (battleCtx->sideEffectDirectFlags & MOVE_SIDE_EFFECT_ON_HIT) {
        *effect = MapSideEffectToSubscript(battleCtx, 1, battleCtx->sideEffectDirectFlags);
        battleCtx->sideEffectDirectFlags = 0;

        if ((battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE) {
            result = TRUE;
        }
    } else if (battleCtx->sideEffectDirectFlags) {
        *effect = MapSideEffectToSubscript(battleCtx, 1, battleCtx->sideEffectDirectFlags);

        if (battleCtx->battleMons[battleCtx->sideEffectMon].curHP
                && ((battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
                    // Brick Break's effect still applies even if the target is immune.
                    || ((battleCtx->sideEffectDirectFlags & MOVE_SIDE_EFFECT_BREAK_SCREENS)
                        && (battleCtx->moveStatusFlags & MOVE_STATUS_DID_NOT_AFFECT))
                    // Various other moves can still apply their effect even if the target
                    // is semi-invulnerable.
                    || ((battleCtx->sideEffectDirectFlags & MOVE_SIDE_EFFECT_CHECK_HP)
                        && (battleCtx->moveStatusFlags & MOVE_STATUS_GENERAL_MISS)))) {
            result = TRUE;
        }

        battleCtx->sideEffectDirectFlags = 0;
    }

    return result;
}

static inline void SetupSideEffect(BattleContext *battleCtx, int *effect, int source)
{
    *effect = MapSideEffectToSubscript(battleCtx, source, battleCtx->sideEffectIndirectFlags);
    battleCtx->sideEffectIndirectFlags = 0;
}

BOOL BattleSystem_TriggerSecondaryEffect(BattleSystem *battleSys, BattleContext *battleCtx, int *effect)
{
    BOOL result = FALSE;
    u16 effectChance;
	int attackerSide = Battler_Side(battleSys, battleCtx->attacker);

    if (battleCtx->sideEffectIndirectFlags & MOVE_SIDE_EFFECT_ON_HIT) {
        SetupSideEffect(battleCtx, effect, SIDE_EFFECT_TYPE_INDIRECT);

        if ((battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE) {
            result = TRUE;
        }
    } else if (battleCtx->sideEffectIndirectFlags & MOVE_SIDE_EFFECT_CHECK_SUBSTITUTE) {
        SetupSideEffect(battleCtx, effect, SIDE_EFFECT_TYPE_INDIRECT);

        if (Battler_SubstituteWasHit(battleCtx, battleCtx->sideEffectMon) == FALSE
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE) {
            result = TRUE;
        }
    } else if (battleCtx->sideEffectIndirectFlags & MOVE_SIDE_EFFECT_CHECK_HP_AND_SUBSTITUTE) {
        SetupSideEffect(battleCtx, effect, SIDE_EFFECT_TYPE_INDIRECT);

        if (battleCtx->battleMons[battleCtx->sideEffectMon].curHP
                && Battler_SubstituteWasHit(battleCtx, battleCtx->sideEffectMon) == FALSE
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE) {
            result = TRUE;
        }
    } else if (battleCtx->sideEffectIndirectFlags & MOVE_SIDE_EFFECT_CHECK_HP) {
        SetupSideEffect(battleCtx, effect, SIDE_EFFECT_TYPE_INDIRECT);

        if (battleCtx->battleMons[battleCtx->sideEffectMon].curHP) {
            result = TRUE;
        }
    } else if (battleCtx->sideEffectIndirectFlags & MOVE_SIDE_EFFECT_PROBABILISTIC) {
		effectChance = CURRENT_MOVE_DATA.effectChance;
		
		if (battleCtx->sideConditionsMask[attackerSide] & SIDE_CONDITION_LUCKY_CHANT)
		{
			effectChance = effectChance + 10;
			
			if (effectChance > 100)
			{
				effectChance = 100;
			}
		}
		
        if (Battler_Ability(battleCtx, battleCtx->attacker) == ABILITY_SERENE_GRACE)
		{
			if (effectChance >= 50)
			{
				effectChance = 100;
			}
			else
			{
				effectChance *= 2;
			}
        }

        GF_ASSERT(effectChance != 0);

        if (BattleSystem_RandNext(battleSys) % 100 < effectChance) {
            battleCtx->battleStatusMask |= SYSCTL_APPLY_SECONDARY_EFFECT;
        }

        SetupSideEffect(battleCtx, effect, SIDE_EFFECT_TYPE_INDIRECT);
        if (battleCtx->battleMons[battleCtx->sideEffectMon].curHP == 0) {
            battleCtx->battleStatusMask &= ~SYSCTL_APPLY_SECONDARY_EFFECT;
        }

        result = TRUE;
    } else if (battleCtx->sideEffectIndirectFlags) {
        effectChance = CURRENT_MOVE_DATA.effectChance;
		
		if (battleCtx->sideConditionsMask[attackerSide] & SIDE_CONDITION_LUCKY_CHANT)
		{
			effectChance = effectChance + 10;
			
			if (effectChance > 100)
			{
				effectChance = 100;
			}
		}
		
        if (Battler_Ability(battleCtx, battleCtx->attacker) == ABILITY_SERENE_GRACE)
		{
			if (effectChance >= 50)
			{
				effectChance = 100;
			}
			else
			{
				effectChance *= 2;
			}
        }

        GF_ASSERT(effectChance != 0);

        if (BattleSystem_RandNext(battleSys) % 100 < effectChance) {
            SetupSideEffect(battleCtx, effect, SIDE_EFFECT_TYPE_INDIRECT);

            if (battleCtx->battleMons[battleCtx->sideEffectMon].curHP
                    && Battler_SubstituteWasHit(battleCtx, battleCtx->sideEffectMon) == FALSE
                    && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE) {
                result = TRUE;
            }
        }
    } else if (battleCtx->sideEffectAbilityFlags) {
        *effect = MapSideEffectToSubscript(battleCtx, SIDE_EFFECT_TYPE_ABILITY, battleCtx->sideEffectAbilityFlags);
        battleCtx->sideEffectAbilityFlags = 0;

        if (battleCtx->battleMons[battleCtx->sideEffectMon].curHP) {
            result = TRUE;
        }
    }

    return result;
}

int BattleSystem_Defender(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, u16 move, BOOL randomize, int inRange)
{
    int defender = BATTLER_NONE;

    int range;
    if (move) {
        range = MOVE_DATA(move).range;
    } else {
        range = inRange;
    }

    if (range == RANGE_ADJACENT_OPPONENTS) { // e.g., Acid, Blizzard
        int maxBattlers = BattleSystem_MaxBattlers(battleSys);
        BattlerData *battlerData = BattleSystem_BattlerData(battleSys, attacker);
        u8 attackerType = Battler_Type(battlerData);

        // Assign the first possible living target based on speed order
        for (battleCtx->battlerCounter = 0; battleCtx->battlerCounter < maxBattlers; battleCtx->battlerCounter++) {
            int battler = battleCtx->monSpeedOrder[battleCtx->battlerCounter];

            // Check that this battler is an enemy of the attacker
            if (battleCtx->battleMons[battler].curHP != 0) {
                battlerData = BattleSystem_BattlerData(battleSys, battler);
                if (((attackerType & BATTLER_TYPE_SOLO_ENEMY) && (Battler_Type(battlerData) & BATTLER_TYPE_SOLO_ENEMY) == FALSE)
                        || ((attackerType & BATTLER_TYPE_SOLO_ENEMY) == FALSE) && (Battler_Type(battlerData) & BATTLER_TYPE_SOLO_ENEMY)) {
                    defender = battler;
                    break;
                }
            }
        }

        if (battleCtx->battlerCounter != maxBattlers) {
            battleCtx->battlerCounter++;
        }
    } else if (range == RANGE_ALL_ADJACENT) { // e.g., Earthquake, Surf
        int maxBattlers = BattleSystem_MaxBattlers(battleSys);

        // Assign the first possible living target based on speed order
        for (battleCtx->battlerCounter = 0; battleCtx->battlerCounter < maxBattlers; battleCtx->battlerCounter++) {
            int battler = battleCtx->monSpeedOrder[battleCtx->battlerCounter];

            // Only care that this battler is not the attacker
            if (battleCtx->battleMons[battler].curHP != 0 && battler != attacker) {
                defender = battler;
                break;
            }
        }

        if (battleCtx->battlerCounter != maxBattlers) {
            battleCtx->battlerCounter++;
        }
    } else if (range == RANGE_USER_OR_ALLY && randomize == TRUE) { // e.g., Acupressure
        if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES)
                && BattleSystem_RandNext(battleSys) % 2 == 0) {
            defender = BattleSystem_Partner(battleSys, attacker);
            if (battleCtx->battleMons[defender].curHP == 0) {
                defender = attacker;
            }
        } else {
            defender = attacker;
        }
    } else if (range == RANGE_SINGLE_TARGET_ME_FIRST && randomize == TRUE) { // e.g., Me First
        defender = BattleSystem_RandomOpponent(battleSys, battleCtx, attacker);
    } else if (range == RANGE_OPPONENT_SIDE) { // e.g., Spikes, Stealth Rock
        defender = BattleSystem_RandomOpponent(battleSys, battleCtx, attacker);
    } else if (range == RANGE_USER // e.g., Swords Dance
            || range == RANGE_USER_SIDE // e.g., Light Screen, Reflect
            || range == RANGE_SINGLE_TARGET_SPECIAL // e.g., Counter, Mirror Coat
            || range == RANGE_FIELD) { // e.g., Sunny Day
        defender = attacker;
    } else if (range == RANGE_ALLY) { // e.g., Helping Hand
        if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
            defender = BattleSystem_Partner(battleSys, attacker);
        } else {
            defender = attacker;
        }
    } else if (range == RANGE_USER_OR_ALLY) { // e.g., Acupressure
        if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
            defender = battleCtx->battlerActions[attacker][BATTLE_ACTION_CHOOSE_TARGET];
            if (battleCtx->battleMons[defender].curHP == 0) {
                defender = attacker;
            }
        } else {
            defender = attacker;
        }
    } else if (range == RANGE_RANDOM_OPPONENT || randomize == TRUE) { // e.g., Outrage, Thrash, any other reason the move should be randomly targeted
        int opponents[2];
        int battleType = BattleSystem_BattleType(battleSys);
        int enemySide = Battler_Side(battleSys, attacker) ^ 1;

        opponents[0] = BattleSystem_EnemyInSlot(battleSys, attacker, ENEMY_IN_SLOT_RIGHT);
        opponents[1] = BattleSystem_EnemyInSlot(battleSys, attacker, ENEMY_IN_SLOT_LEFT);

        if (battleType & BATTLE_TYPE_DOUBLES) {
            if (battleCtx->sideConditions[enemySide].followMe
                    && battleCtx->battleMons[battleCtx->sideConditions[enemySide].followMeUser].curHP) {
                // If Follow Me is active and the user is still alive, re-point all targets toward them
                defender = battleCtx->sideConditions[enemySide].followMeUser;
            } else if (battleCtx->battleMons[opponents[0]].curHP
                    && battleCtx->battleMons[opponents[1]].curHP) {
                defender = opponents[BattleSystem_RandNext(battleSys) & 1];
            } else if (battleCtx->battleMons[opponents[0]].curHP) {
                defender = opponents[0];
            } else if (battleCtx->battleMons[opponents[1]].curHP) {
                defender = opponents[1];
            }
        } else if (battleCtx->battleMons[attacker ^ 1].curHP) {
            defender = attacker ^ 1;
        }
    } else { // the usual single-target moves, e.g., Flamethrower, Thunderbolt
        int enemySide = Battler_Side(battleSys, attacker) ^ 1;
        int target = battleCtx->battlerActions[attacker][BATTLE_ACTION_CHOOSE_TARGET];
        int maxBattlers = BattleSystem_MaxBattlers(battleSys);

        if (battleCtx->sideConditions[enemySide].followMe
                && battleCtx->battleMons[battleCtx->sideConditions[enemySide].followMeUser].curHP) {
            // If Follow Me is active and the user is still alive, re-point all targets toward them
            defender = battleCtx->sideConditions[enemySide].followMeUser;
        } else if (battleCtx->battleMons[target].curHP) {
            defender = target;
        } else {
            // If the original target is no longer alive, try to target their partner instead
            target = BattleSystem_RandomOpponent(battleSys, battleCtx, attacker);
            if (battleCtx->battleMons[target].curHP) {
                defender = target;
            }
        }
    }

    return defender;
}

void BattleSystem_CheckRedirectionAbilities(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, u16 move)
{
    int battler, moveType; // must declare these first to match
    u16 attackerItem;

    attackerItem = Battler_HeldItem(battleCtx, attacker);

    if (battleCtx->defender == BATTLER_NONE
            || Battler_Ability(battleCtx, attacker) == ABILITY_NORMALIZE
            || Battler_Ability(battleCtx, attacker) == ABILITY_MOLD_BREAKER) {
        return;
    }

    int defSide = Battler_Side(battleSys, attacker) ^ 1;
    if (battleCtx->sideConditions[defSide].followMe && FOLLOW_ME_MON(defSide).curHP) {
        return;
    }

    moveType = CalcMoveType(battleSys, battleCtx, attacker, attackerItem, move);
    if (moveType == TYPE_NORMAL) {
        moveType = MOVE_DATA(move).type;
    }

    int maxBattlers = BattleSystem_MaxBattlers(battleSys);
    if (moveType == TYPE_ELECTRIC
            && (MOVE_DATA(move).range == RANGE_SINGLE_TARGET || MOVE_DATA(move).range == RANGE_RANDOM_OPPONENT)
            && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
            && BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_EXCEPT_ME, attacker, ABILITY_LIGHTNING_ROD)) {
        for (int i = 0; i < maxBattlers; i++) {
            battler = battleCtx->monSpeedOrder[i];

            if (Battler_Ability(battleCtx, battler) == ABILITY_LIGHTNING_ROD
                    && battleCtx->battleMons[battler].curHP
                    && attacker != battler) {
                break;
            }
        }

        if (battler != battleCtx->defender) {
            battleCtx->selfTurnFlags[battler].lightningRodActivated = TRUE;
            battleCtx->defender = battler;
        }
    } else if (moveType == TYPE_WATER
            && (MOVE_DATA(move).range == RANGE_SINGLE_TARGET || MOVE_DATA(move).range == RANGE_RANDOM_OPPONENT)
            && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
            && BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_EXCEPT_ME, attacker, ABILITY_STORM_DRAIN)) {
        for (int i = 0; i < maxBattlers; i++) {
            battler = battleCtx->monSpeedOrder[i];

            if (Battler_Ability(battleCtx, battler) == ABILITY_STORM_DRAIN
                    && battleCtx->battleMons[battler].curHP
                    && attacker != battler) {
                break;
            }
        }

        if (battler != battleCtx->defender) {
            battleCtx->selfTurnFlags[battler].stormDrainActivated = TRUE;
            battleCtx->defender = battler;
        }
    }
}

BOOL BattleSystem_TriggerRedirectionAbilities(BattleSystem *battleSys, BattleContext *battleCtx)
{
    BOOL result = FALSE;

    if ((battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE && DEFENDER_SELF_TURN_FLAGS.lightningRodActivated) {
        battleCtx->selfTurnFlags[battleCtx->defender].lightningRodActivated = FALSE;

        LOAD_SUBSEQ(subscript_lightning_rod_redirected);
        battleCtx->commandNext = battleCtx->command;
        battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;

        result = TRUE;
    }

    if ((battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE && DEFENDER_SELF_TURN_FLAGS.stormDrainActivated) {
        battleCtx->selfTurnFlags[battleCtx->defender].stormDrainActivated = FALSE;

        LOAD_SUBSEQ(subscript_lightning_rod_redirected);
        battleCtx->commandNext = battleCtx->command;
        battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;

        result = TRUE;
    }

    return result;
}

void BattleMon_CopyToParty(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    if (battleCtx->battleMons[battler].heldItem == ITEM_NONE) {
        BattleAI_ClearKnownItem(battleCtx, battler);
    }

    BattleIO_UpdatePartyMon(battleSys, battleCtx, battler);
}

void Battler_LockMoveChoice(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    battleCtx->battleMons[battler].statusVolatile |= VOLATILE_CONDITION_MOVE_LOCKED;
    battleCtx->moveLockedInto[battler] = battleCtx->moveCur;
}

void Battler_UnlockMoveChoice(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    battleCtx->battleMons[battler].statusVolatile &= ~VOLATILE_CONDITION_MOVE_LOCKED;
    battleCtx->battleMons[battler].statusVolatile &= ~VOLATILE_CONDITION_BIDE;
    battleCtx->battleMons[battler].moveEffectsMask &= ~MOVE_EFFECT_SEMI_INVULNERABLE;
    battleCtx->battleMons[battler].moveEffectsData.rolloutCount = 0;
    battleCtx->battleMons[battler].moveEffectsData.furyCutterCount = 0;
}

enum BattleAnimation Battler_StatusCondition(BattleContext *battleCtx, int battler)
{
    if (battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) {
        return BATTLE_ANIMATION_ASLEEP;
    } else if (battleCtx->battleMons[battler].status & MON_CONDITION_POISON) {
        return BATTLE_ANIMATION_POISONED;
    } else if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN) {
        return BATTLE_ANIMATION_BURNED;
    } else if (battleCtx->battleMons[battler].status & MON_CONDITION_FREEZE) {
        return BATTLE_ANIMATION_FROZEN;
    } else if (battleCtx->battleMons[battler].status & MON_CONDITION_PARALYSIS) {
        return BATTLE_ANIMATION_PARALYZED;
    } else if (battleCtx->battleMons[battler].status & MON_CONDITION_TOXIC) {
        return BATTLE_ANIMATION_POISONED;
    }

    return BATTLE_ANIMATION_NONE;
}

enum {
    CHECK_TRMSG_START = 0,

    CHECK_TRMSG_FIRST_DAMAGE = CHECK_TRMSG_START,
    CHECK_TRMSG_ACTIVE_BATTLER_HALF_HP,
    CHECK_TRMSG_LAST_BATTLER,
    CHECK_TRMSG_LAST_BATTLER_HALF_HP,
    
    CHECK_TRMSG_END,
};

BOOL BattleSystem_CheckTrainerMessage(BattleSystem *battleSys, BattleContext *battleCtx)
{
    int battleType = BattleSystem_BattleType(battleSys);

    if (battleType & BATTLE_TYPE_NO_TRAINER_MESSAGES) {
        return FALSE;
    }

    if ((battleType & BATTLE_TYPE_TRAINER) == FALSE) {
        return FALSE;
    }

    if (battleType & BATTLE_TYPE_DOUBLES) {
        return FALSE;
    }

    int trID = Battler_TrainerID(battleSys, BATTLER_THEM);
    int state = CHECK_TRMSG_START;

    do {
        switch (state) {
        case CHECK_TRMSG_FIRST_DAMAGE:
            if (battleCtx->battleMons[BATTLER_THEM].timesDamaged == 1
                    && (battleCtx->battleStatusMask2 & SYSCTL_FIRST_DAMAGE_MSG_SHOWN) == FALSE
                    && TrainerData_HasMessageType(trID, TRMSG_FIRST_DAMAGE, HEAP_ID_BATTLE)) {
                battleCtx->battleStatusMask2 |= SYSCTL_FIRST_DAMAGE_MSG_SHOWN;
                battleCtx->msgTemp = TRMSG_FIRST_DAMAGE;
                return TRUE;
            }

            state++;
            break;

        case CHECK_TRMSG_ACTIVE_BATTLER_HALF_HP:
            if ((battleCtx->battleMons[BATTLER_THEM].trainerMessageFlags & TRMSG_ACTIVE_BATTLER_HALF_HP_FLAG) == FALSE
                    && battleCtx->battleMons[BATTLER_THEM].curHP <= battleCtx->battleMons[BATTLER_THEM].maxHP / 2
                    && TrainerData_HasMessageType(trID, TRMSG_ACTIVE_BATTLER_HALF_HP, HEAP_ID_BATTLE)) {
                battleCtx->battleMons[BATTLER_THEM].trainerMessageFlags |= TRMSG_ACTIVE_BATTLER_HALF_HP_FLAG;
                battleCtx->msgTemp = TRMSG_ACTIVE_BATTLER_HALF_HP;
                return TRUE;
            }

            state++;
            break;

        case CHECK_TRMSG_LAST_BATTLER:
            if ((battleCtx->battleMons[BATTLER_THEM].trainerMessageFlags & TRMSG_LAST_BATTLER_FLAG) == FALSE) {
                Party *party = BattleSystem_Party(battleSys, BATTLER_THEM);
                int alive = 0;

                for (int i = 0; i < Party_GetCurrentCount(party); i++) {
                    Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

                    if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL)) {
                        alive++;
                    }
                }

                if (alive == 1 && TrainerData_HasMessageType(trID, TRMSG_LAST_BATTLER, HEAP_ID_BATTLE)) {
                    battleCtx->battleMons[BATTLER_THEM].trainerMessageFlags |= TRMSG_LAST_BATTLER_FLAG;
                    battleCtx->msgTemp = TRMSG_LAST_BATTLER;
                    return TRUE;
                }
            }

            state++;
            break;

        case CHECK_TRMSG_LAST_BATTLER_HALF_HP:
            if ((battleCtx->battleMons[BATTLER_THEM].trainerMessageFlags & TRMSG_LAST_BATTLER_HALF_HP_FLAG) == FALSE) {
                Party *party = BattleSystem_Party(battleSys, BATTLER_THEM);
                int alive = 0;

                for (int i = 0; i < Party_GetCurrentCount(party); i++) {
                    Pokemon *mon = Party_GetPokemonBySlotIndex(party, i);

                    if (Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL)) {
                        alive++;
                    }
                }

                if (alive == 1
                        && battleCtx->battleMons[BATTLER_THEM].curHP <= battleCtx->battleMons[BATTLER_THEM].maxHP / 2
                        && TrainerData_HasMessageType(trID, TRMSG_LAST_BATTLER_HALF_HP, HEAP_ID_BATTLE)) {
                    battleCtx->battleMons[BATTLER_THEM].trainerMessageFlags |= TRMSG_LAST_BATTLER_HALF_HP_FLAG;
                    battleCtx->msgTemp = TRMSG_LAST_BATTLER_HALF_HP;
                    return TRUE;
                }
            }

            state++;
            break;

        case CHECK_TRMSG_END:
            break;
        }
    } while (state != CHECK_TRMSG_END);

    return FALSE;
}

void BattleContext_Init(BattleContext *battleCtx)
{
    battleCtx->damage = 0;
    battleCtx->criticalMul = 1;
    battleCtx->criticalBoosts = 0;
    battleCtx->movePower = 0;
    battleCtx->powerMul = 10;
    battleCtx->moveType = TYPE_NORMAL;
    battleCtx->moveEffectChance = 0;
    battleCtx->moveStatusFlags = 0;
    battleCtx->faintedMon = BATTLER_NONE;
    battleCtx->sideEffectDirectFlags = MOVE_SUBSCRIPT_PTR_NONE;
    battleCtx->sideEffectIndirectFlags = MOVE_SUBSCRIPT_PTR_NONE;
    battleCtx->sideEffectAbilityFlags = MOVE_SUBSCRIPT_PTR_NONE;
    battleCtx->sideEffectType = SIDE_EFFECT_TYPE_NONE;
    battleCtx->sideEffectParam = MOVE_SUBSCRIPT_PTR_NONE;
    battleCtx->sideEffectMon = BATTLER_NONE;
    battleCtx->multiHitCounter = 0;
    battleCtx->multiHitNumHits = 0;
    battleCtx->battlerCounter = 0;
    battleCtx->multiHitLoop = 0;
    battleCtx->afterMoveMessageType = 0;
    battleCtx->multiHitCheckFlags = 0;
    battleCtx->multiHitAccuracyCheck = 0;
    battleCtx->fieldConditionCheckState = 0;
    battleCtx->monConditionCheckState = 0;
    battleCtx->sideConditionCheckState = 0;
    battleCtx->turnStartCheckState = 0;
    battleCtx->afterMoveHitCheckState = 0;
    battleCtx->afterMoveMessageState = 0;
    battleCtx->afterMoveEffectState = 0;
    battleCtx->beforeMoveCheckState = 0;
    battleCtx->tryMoveCheckState = 0;
    battleCtx->statusCheckState = 0;
    battleCtx->abilityCheckState = 0;
    battleCtx->battleStatusMask &= SYSCTL_INIT;
    battleCtx->battleStatusMask2 &= SYSCTL_INIT2;
    battleCtx->magnitude = 0;

    for (int i = 0; i < MAX_BATTLERS; i++) {
        MI_CpuClearFast(&battleCtx->selfTurnFlags[i], sizeof(struct SelfTurnFlags));
        battleCtx->aiSwitchedPartySlot[i] = 6;
    }
}

void BattleContext_InitCounters(BattleSystem *battleSys, BattleContext *battleCtx)
{
    for (int i = 0; i < MAX_BATTLERS; i++) {
        battleCtx->moveHitBattler[i] = BATTLER_NONE;
        battleCtx->switchedPartySlot[i] = 6;
        battleCtx->speedRand[i] = BattleSystem_RandNext(battleSys);
    }

    battleCtx->prizeMoneyMul = 1;
    battleCtx->meFirstTurnOrder = 1;

    int battleType = BattleSystem_BattleType(battleSys);
    if ((battleType & BATTLE_TYPE_DOUBLES) == FALSE) {
        battleCtx->battlersSwitchingMask |= FlagIndex(BATTLER_PLAYER_SLOT_2);
        battleCtx->battlersSwitchingMask |= FlagIndex(BATTLER_ENEMY_SLOT_2);
    }

    battleCtx->safariCatchStage = 6;
    battleCtx->safariEscapeCount = 6;
}

void BattleSystem_UpdateAfterSwitch(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    // declare C89-style to match
    int i, maxBattlers;
    u32 battleType;
    u8 *addr;
    MoveEffectsData moveEffects;

    moveEffects = battleCtx->battleMons[battler].moveEffectsData;
    maxBattlers = BattleSystem_MaxBattlers(battleSys);
    battleType = BattleSystem_BattleType(battleSys);

    // Forcefully end the battler's turn after the replacement
    battleCtx->battlerActions[battler][BATTLE_ACTION_PICK_COMMAND] = BATTLE_CONTROL_MOVE_END;

    if ((battleCtx->battleStatusMask & SYSCTL_BATON_PASS) == FALSE) {
        // Clear any Mean Look or Lock On effects from other active battlers
        for (i = 0; i < maxBattlers; i++) {
            if ((battleCtx->battleMons[i].statusVolatile & VOLATILE_CONDITION_MEAN_LOOK)
                    && (battleCtx->battleMons[i].moveEffectsData.meanLookTarget == battler)) {
                battleCtx->battleMons[i].statusVolatile &= ~VOLATILE_CONDITION_MEAN_LOOK;
            }

            if ((battleCtx->battleMons[i].moveEffectsMask & MOVE_EFFECT_LOCK_ON)
                    && (battleCtx->battleMons[i].moveEffectsData.lockOnTarget == battler)) {
                battleCtx->battleMons[i].moveEffectsMask &= ~MOVE_EFFECT_LOCK_ON;
                battleCtx->battleMons[i].moveEffectsData.lockOnTarget = 0;
            }
        }

        battleCtx->battleMons[battler].statusVolatile = VOLATILE_CONDITION_NONE;
        battleCtx->battleMons[battler].moveEffectsMask = MOVE_EFFECT_NONE;
    } else {
        // Baton Pass maintains Focus Energy, Mean Look, Confusion, Curse, Substitute,
        // and a variety of move effects (see constants/battle/moves.h)
        battleCtx->battleMons[battler].statusVolatile &= VOLATILE_CONDITION_BATON_PASSED;
        battleCtx->battleMons[battler].moveEffectsMask &= MOVE_EFFECT_BATON_PASSED;

        for (i = 0; i < maxBattlers; i++) {
            if ((battleCtx->battleMons[i].moveEffectsMask & MOVE_EFFECT_LOCK_ON)
                    && battleCtx->battleMons[i].moveEffectsData.lockOnTarget == battler) {
                // When transferring Lock On due to Baton Pass, its effect timer is refreshed
                battleCtx->battleMons[i].moveEffectsMask &= ~MOVE_EFFECT_LOCK_ON;
                battleCtx->battleMons[i].moveEffectsMask |= MOVE_EFFECT_LOCK_ON_INITIAL_DURATION;
            }
        }
    }

    // Clear the effects of Attract and Bind sourced from the replaced battler
    for (i = 0; i < maxBattlers; i++) {
        if (battleCtx->battleMons[i].statusVolatile & (FlagIndex(battler) << VOLATILE_CONDITION_ATTRACT_SHIFT)) {
            battleCtx->battleMons[i].statusVolatile &= FLAG_NEGATE(FlagIndex(battler) << VOLATILE_CONDITION_ATTRACT_SHIFT);
        }

        if ((battleCtx->battleMons[i].statusVolatile & VOLATILE_CONDITION_BIND)
                && battleCtx->battleMons[i].moveEffectsData.bindTarget == battler) {
            battleCtx->battleMons[i].statusVolatile &= ~VOLATILE_CONDITION_BIND;
        }
    }

    // Clear all move effects for the battler
    addr = (u8 *)&battleCtx->battleMons[battler].moveEffectsData; // doesn't match with &moveEffectsData
    for (i = 0; i < sizeof(MoveEffectsData); i++) {
        addr[i] = 0;
    }

    // Update the move effects for Baton Pass, if applicable
    if (battleCtx->battleStatusMask & SYSCTL_BATON_PASS) {
        battleCtx->battleMons[battler].moveEffectsData.substituteHP = moveEffects.substituteHP;
        battleCtx->battleMons[battler].moveEffectsData.lockOnTarget = moveEffects.lockOnTarget;
        battleCtx->battleMons[battler].moveEffectsData.perishSongTurns = moveEffects.perishSongTurns;
        battleCtx->battleMons[battler].moveEffectsData.meanLookTarget = moveEffects.meanLookTarget;
        battleCtx->battleMons[battler].moveEffectsData.magnetRiseTurns = moveEffects.magnetRiseTurns;
        battleCtx->battleMons[battler].moveEffectsData.embargoTurns = moveEffects.embargoTurns;
        battleCtx->battleMons[battler].moveEffectsData.healBlockTurns = moveEffects.healBlockTurns;
		battleCtx->battleMons[battler].moveEffectsData.healInversionTurns = moveEffects.healInversionTurns;
    }

    battleCtx->battleMons[battler].moveEffectsData.fakeOutTurnNumber = battleCtx->totalTurns + 1;
    battleCtx->battleMons[battler].moveEffectsData.slowStartTurnNumber = battleCtx->totalTurns + 1;
    battleCtx->battleMons[battler].moveEffectsData.truant = (battleCtx->totalTurns + 1) & 1;
    battleCtx->moveProtect[battler] = MOVE_NONE;
    battleCtx->moveHit[battler] = MOVE_NONE;
    battleCtx->moveHitBattler[battler] = BATTLER_NONE;
    battleCtx->moveHitType[battler] = MOVE_NONE;
    battleCtx->movePrevByBattler[battler] = MOVE_NONE;
    battleCtx->moveCopied[battler] = MOVE_NONE;
    battleCtx->moveCopiedHit[battler][0] = MOVE_NONE;
    battleCtx->moveCopiedHit[battler][1] = MOVE_NONE;
    battleCtx->moveCopiedHit[battler][2] = MOVE_NONE;
    battleCtx->moveCopiedHit[battler][3] = MOVE_NONE;
    battleCtx->moveSketched[battler] = MOVE_NONE;
    battleCtx->conversion2Move[battler] = MOVE_NONE;
    battleCtx->conversion2Battler[battler] = MOVE_NONE;
    battleCtx->conversion2Type[battler] = MOVE_NONE;
    battleCtx->metronomeMove[battler] = MOVE_NONE;
    battleCtx->fieldConditionsMask &= FLAG_NEGATE(FlagIndex(battler) << FIELD_CONDITION_UPROAR_SHIFT);

    if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_POWER_TRICK) {
        i = battleCtx->battleMons[battler].attack;
        battleCtx->battleMons[battler].attack = battleCtx->battleMons[battler].defense;
        battleCtx->battleMons[battler].defense = i;
    }

    for (i = 0; i < maxBattlers; i++) {
        if (i != battler && Battler_Side(battleSys, i) != Battler_Side(battleSys, battler)) {
            battleCtx->moveCopied[i] = MOVE_NONE;
        }

        battleCtx->moveCopiedHit[i][battler] = MOVE_NONE;
    }

    BattleAI_ClearKnownMoves(battleCtx, battler);
    BattleAI_ClearKnownAbility(battleCtx, battler);
    BattleAI_ClearKnownItem(battleCtx, battler);
}

void BattleSystem_CleanupFaintedMon(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int maxBattlers = BattleSystem_MaxBattlers(battleSys);
    int i;

    for (i = BATTLE_STAT_HP; i < BATTLE_STAT_MAX; i++) {
        battleCtx->battleMons[battler].statBoosts[i] = 6;
    }

    battleCtx->battleMons[battler].statusVolatile = 0;
    battleCtx->battleMons[battler].moveEffectsMask = 0;

    // Negate Mean Look, Attract, and Bind flags
    for (i = 0; i < maxBattlers; i++) {
        if ((battleCtx->battleMons[i].statusVolatile & VOLATILE_CONDITION_MEAN_LOOK)
                && battleCtx->battleMons[i].moveEffectsData.meanLookTarget == battler) {
            battleCtx->battleMons[i].statusVolatile &= ~VOLATILE_CONDITION_MEAN_LOOK;
        }

        if (battleCtx->battleMons[i].statusVolatile & (FlagIndex(battler) << VOLATILE_CONDITION_ATTRACT_SHIFT)) {
            battleCtx->battleMons[i].statusVolatile &= (FlagIndex(battler) << VOLATILE_CONDITION_ATTRACT_SHIFT) ^ 0xFFFFFFFF;
        }

        if ((battleCtx->battleMons[i].statusVolatile & VOLATILE_CONDITION_BIND)
                && battleCtx->battleMons[i].moveEffectsData.bindTarget == battler) {
            battleCtx->battleMons[i].statusVolatile &= ~VOLATILE_CONDITION_BIND;
        }
    }

    u8 *addr = (u8 *)&battleCtx->battleMons[battler].moveEffectsData;
    for (i = 0; i < sizeof(MoveEffectsData); i++) {
        addr[i] = 0;
    }

    addr = (u8 *)&battleCtx->turnFlags[battler];
    for (i = 0; i < sizeof(struct TurnFlags); i++) {
        addr[i] = 0;
    }

    battleCtx->battleMons[battler].moveEffectsData.fakeOutTurnNumber = battleCtx->totalTurns + 1;
    battleCtx->battleMons[battler].moveEffectsData.slowStartTurnNumber = battleCtx->totalTurns + 1;
    battleCtx->battleMons[battler].moveEffectsData.truant = (battleCtx->totalTurns + 1) & 1;
    battleCtx->moveProtect[battler] = MOVE_NONE;
    battleCtx->moveHit[battler] = MOVE_NONE;
    battleCtx->moveHitBattler[battler] = BATTLER_NONE;
    battleCtx->moveHitType[battler] = MOVE_NONE;
    battleCtx->movePrevByBattler[battler] = MOVE_NONE;
    battleCtx->moveCopied[battler] = MOVE_NONE;
    battleCtx->moveCopiedHit[battler][0] = MOVE_NONE;
    battleCtx->moveCopiedHit[battler][1] = MOVE_NONE;
    battleCtx->moveCopiedHit[battler][2] = MOVE_NONE;
    battleCtx->moveCopiedHit[battler][3] = MOVE_NONE;
    battleCtx->moveSketched[battler] = MOVE_NONE;
    battleCtx->conversion2Move[battler] = MOVE_NONE;
    battleCtx->conversion2Battler[battler] = MOVE_NONE;
    battleCtx->conversion2Type[battler] = MOVE_NONE;
    battleCtx->metronomeMove[battler] = MOVE_NONE;
    battleCtx->fieldConditionsMask &= (FlagIndex(battler) << FIELD_CONDITION_UPROAR_SHIFT) ^ 0xFFFFFFFF;

    for (i = 0; i < maxBattlers; i++) {
        if (i != battler && Battler_Side(battleSys, i) != Battler_Side(battleSys, battler)) {
            battleCtx->moveCopied[i] = MOVE_NONE;
        }
        battleCtx->moveCopiedHit[i][battler] = 0;
    }

    battleCtx->battlerStatusFlags[battler] &= ~BATTLER_STATUS_SWITCHING;
    BattleAI_ClearKnownMoves(battleCtx, battler);
    BattleAI_ClearKnownAbility(battleCtx, battler);
    BattleAI_ClearKnownItem(battleCtx, battler);
}

void BattleSystem_SetupNextTurn(BattleSystem *battleSys, BattleContext *battleCtx)
{
    for (int i = 0; i < MAX_BATTLERS; i++) {
        MI_CpuClearFast(&battleCtx->turnFlags[i], sizeof(struct TurnFlags));
        MI_CpuClearFast(&battleCtx->moveFailFlags[i], sizeof(struct MoveFailFlags));

        battleCtx->battleMons[i].statusVolatile &= ~VOLATILE_CONDITION_FLINCH;

        if (battleCtx->battleMons[i].moveEffectsData.rechargeTurnNumber + 1 < battleCtx->totalTurns) {
            battleCtx->battleMons[i].statusVolatile &= ~VOLATILE_CONDITION_RECHARGING;
        }

        if ((battleCtx->battleMons[i].status & MON_CONDITION_SLEEP)
                && (battleCtx->battleMons[i].statusVolatile & VOLATILE_CONDITION_MOVE_LOCKED)) {
            Battler_UnlockMoveChoice(battleSys, battleCtx, i);
        }

        if ((battleCtx->battleMons[i].status & MON_CONDITION_SLEEP)
                && (battleCtx->battleMons[i].statusVolatile & VOLATILE_CONDITION_THRASH)) {
            battleCtx->battleMons[i].statusVolatile &= ~VOLATILE_CONDITION_THRASH;
        }
    }

    battleCtx->sideConditions[BATTLER_US].followMe = FALSE;
    battleCtx->sideConditions[BATTLER_THEM].followMe = FALSE;
}

int BattleSystem_CheckInvalidMoves(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int invalidMoves, int opMask)
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
                && battleCtx->battleMons[battler].moveEffectsData.encoredMove != battleCtx->battleMons[battler].moves[i]) {
            invalidMoves |= FlagIndex(i);
        }

        if ((itemEffect == HOLD_EFFECT_CHOICE_ATK || itemEffect == HOLD_EFFECT_CHOICE_SPEED || itemEffect == HOLD_EFFECT_CHOICE_SPATK)
                && (opMask & CHECK_INVALID_CHOICE_ITEM)) {
            if (Battler_SlotForMove(&battleCtx->battleMons[battler], battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove) == LEARNED_MOVES_MAX) {
                battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove = MOVE_NONE;
            } else if (battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove
                    && battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove != battleCtx->battleMons[battler].moves[i]) {
                invalidMoves |= FlagIndex(i);
            }
        }
    }

    return invalidMoves;
}

BOOL BattleSystem_CanUseMove(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int moveSlot, BattleMessage *msgOut)
{
    BOOL result = TRUE;

    if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_DISABLED) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_NICKNAME_MOVE;
        msgOut->id = 609; // "{0}'s {1} is disabled!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, battler);
        msgOut->params[1] = battleCtx->battleMons[battler].moves[moveSlot];
        result = FALSE;
    }
	else if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_ASS_VEST) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_NICKNAME_MOVE;
        msgOut->id = 1288; // "{0} can't use {1}!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, battler);
        msgOut->params[1] = battleCtx->battleMons[battler].moves[moveSlot];
        result = FALSE;
    }
	else if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_TORMENTED) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_NICKNAME;
        msgOut->id = 612; // "{0} can't use the same move twice in a row due to the torment!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, battler);
        result = FALSE;
    }
	else if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_TAUNTED) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_NICKNAME_MOVE;
        msgOut->id = 613; // "{0} can't use {1} after the taunt!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, battler);
        msgOut->params[1] = battleCtx->battleMons[battler].moves[moveSlot];
        result = FALSE;
    }
	else if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_IMPRISONED) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_NICKNAME_MOVE;
        msgOut->id = 616; // "{0} can't use the sealed {1}!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, battler);
        msgOut->params[1] = battleCtx->battleMons[battler].moves[moveSlot];
        result = FALSE;
    }
	else if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_GRAVITY) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_NICKNAME_MOVE;
        msgOut->id = 1001; // "{0} can't use {1} because of gravity!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, battler);
        msgOut->params[1] = battleCtx->battleMons[battler].moves[moveSlot];
        result = FALSE;
    }
	else if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_HEAL_BLOCK) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_NICKNAME_MOVE_MOVE;
        msgOut->id = 1057; // "{0} can't use {2} because of {1}!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, battler);
        msgOut->params[1] = MOVE_HEAL_BLOCK;
        msgOut->params[2] = battleCtx->battleMons[battler].moves[moveSlot];
        result = FALSE;
    }
	else if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_CHOICE_ITEM) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_ITEM_MOVE;
        msgOut->id = 911; // "The {0} allows the use of only {1}!"
        msgOut->params[0] = battleCtx->battleMons[battler].heldItem;
        msgOut->params[1] = battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove;
        result = FALSE;
    }
	else if (BattleSystem_CheckInvalidMoves(battleSys, battleCtx, battler, 0, CHECK_INVALID_NO_PP) & FlagIndex(moveSlot)) {
        msgOut->tags = TAG_NONE;
        msgOut->id = 823; // "There's no PP left for this move!"
        result = FALSE;
    }

    return result;
}

int Battler_SlotForMove(BattleMon *mon, u16 move)
{
    int i;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (mon->moves[i] == move) {
            break;
        }
    }

    return i;
}

/**
 * @brief The type matchup table. This lists the matchups which are deemed to
 * have a Super Effective, Not Very Effective, or Immune result. The table can
 * be read as such:
 * 
 *   attacking type, defending type, matchup multiplier
 * 
 * The matchup multiplier is assumed to be divided by 10 at a later stage for
 * any computations which make use of it (hence the values 5 and 20).
 */
static const u8 sTypeMatchupMultipliers[][3] = {
    { TYPE_NORMAL,   TYPE_ROCK,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_NORMAL,   TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIRE,     TYPE_FIRE,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIRE,     TYPE_WATER,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIRE,     TYPE_GRASS,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_FIRE,     TYPE_ICE,       TYPE_MULTI_SUPER_EFF    },
    { TYPE_FIRE,     TYPE_BUG,       TYPE_MULTI_SUPER_EFF    },
    { TYPE_FIRE,     TYPE_ROCK,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIRE,     TYPE_DRAGON,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIRE,     TYPE_STEEL,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_WATER,    TYPE_FIRE,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_WATER,    TYPE_WATER,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_WATER,    TYPE_GRASS,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_WATER,    TYPE_GROUND,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_WATER,    TYPE_ROCK,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_WATER,    TYPE_DRAGON,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ELECTRIC, TYPE_WATER,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_ELECTRIC, TYPE_ELECTRIC,  TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ELECTRIC, TYPE_GRASS,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ELECTRIC, TYPE_GROUND,    TYPE_MULTI_IMMUNE       },
    { TYPE_ELECTRIC, TYPE_FLYING,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_ELECTRIC, TYPE_DRAGON,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GRASS,    TYPE_FIRE,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GRASS,    TYPE_WATER,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_GRASS,    TYPE_GRASS,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GRASS,    TYPE_POISON,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GRASS,    TYPE_GROUND,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_GRASS,    TYPE_FLYING,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GRASS,    TYPE_BUG,       TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GRASS,    TYPE_ROCK,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_GRASS,    TYPE_DRAGON,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GRASS,    TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ICE,      TYPE_WATER,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ICE,      TYPE_GRASS,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_ICE,      TYPE_ICE,       TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ICE,      TYPE_GROUND,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_ICE,      TYPE_FLYING,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_ICE,      TYPE_DRAGON,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_ICE,      TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ICE,      TYPE_FIRE,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIGHTING, TYPE_NORMAL,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_FIGHTING, TYPE_ICE,       TYPE_MULTI_SUPER_EFF    },
    { TYPE_FIGHTING, TYPE_POISON,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIGHTING, TYPE_FLYING,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIGHTING, TYPE_PSYCHIC,   TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIGHTING, TYPE_BUG,       TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FIGHTING, TYPE_ROCK,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_FIGHTING, TYPE_DARK,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_FIGHTING, TYPE_STEEL,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_POISON,   TYPE_GRASS,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_POISON,   TYPE_POISON,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_POISON,   TYPE_GROUND,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_POISON,   TYPE_ROCK,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_POISON,   TYPE_GHOST,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_POISON,   TYPE_STEEL,     TYPE_MULTI_IMMUNE       },
    { TYPE_GROUND,   TYPE_FIRE,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_GROUND,   TYPE_ELECTRIC,  TYPE_MULTI_SUPER_EFF    },
    { TYPE_GROUND,   TYPE_GRASS,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GROUND,   TYPE_POISON,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_GROUND,   TYPE_FLYING,    TYPE_MULTI_IMMUNE       },
    { TYPE_GROUND,   TYPE_BUG,       TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GROUND,   TYPE_ROCK,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_GROUND,   TYPE_STEEL,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_FLYING,   TYPE_ELECTRIC,  TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FLYING,   TYPE_GRASS,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_FLYING,   TYPE_FIGHTING,  TYPE_MULTI_SUPER_EFF    },
    { TYPE_FLYING,   TYPE_BUG,       TYPE_MULTI_SUPER_EFF    },
    { TYPE_FLYING,   TYPE_ROCK,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_FLYING,   TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_PSYCHIC,  TYPE_FIGHTING,  TYPE_MULTI_SUPER_EFF    },
    { TYPE_PSYCHIC,  TYPE_POISON,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_PSYCHIC,  TYPE_PSYCHIC,   TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_PSYCHIC,  TYPE_DARK,      TYPE_MULTI_IMMUNE       },
    { TYPE_PSYCHIC,  TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_BUG,      TYPE_FIRE,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_BUG,      TYPE_GRASS,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_BUG,      TYPE_FIGHTING,  TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_BUG,      TYPE_POISON,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_BUG,      TYPE_FLYING,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_BUG,      TYPE_PSYCHIC,   TYPE_MULTI_SUPER_EFF    },
    { TYPE_BUG,      TYPE_GHOST,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_BUG,      TYPE_DARK,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_BUG,      TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ROCK,     TYPE_FIRE,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_ROCK,     TYPE_ICE,       TYPE_MULTI_SUPER_EFF    },
    { TYPE_ROCK,     TYPE_FIGHTING,  TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ROCK,     TYPE_GROUND,    TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_ROCK,     TYPE_FLYING,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_ROCK,     TYPE_BUG,       TYPE_MULTI_SUPER_EFF    },
    { TYPE_ROCK,     TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GHOST,    TYPE_NORMAL,    TYPE_MULTI_IMMUNE       },
    { TYPE_GHOST,    TYPE_PSYCHIC,   TYPE_MULTI_SUPER_EFF    },
    { TYPE_GHOST,    TYPE_DARK,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_GHOST,    TYPE_GHOST,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_DRAGON,   TYPE_DRAGON,    TYPE_MULTI_SUPER_EFF    },
    { TYPE_DRAGON,   TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_DARK,     TYPE_FIGHTING,  TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_DARK,     TYPE_PSYCHIC,   TYPE_MULTI_SUPER_EFF    },
    { TYPE_DARK,     TYPE_GHOST,     TYPE_MULTI_SUPER_EFF    },
    { TYPE_DARK,     TYPE_DARK,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_STEEL,    TYPE_FIRE,      TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_STEEL,    TYPE_WATER,     TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_STEEL,    TYPE_ELECTRIC,  TYPE_MULTI_NOT_VERY_EFF },
    { TYPE_STEEL,    TYPE_ICE,       TYPE_MULTI_SUPER_EFF    },
    { TYPE_STEEL,    TYPE_ROCK,      TYPE_MULTI_SUPER_EFF    },
    { TYPE_STEEL,    TYPE_STEEL,     TYPE_MULTI_NOT_VERY_EFF },

    { 0xFE, 0xFE, TYPE_MULTI_IMMUNE },

    // These values are separated from the remainder of the table to support
    // the Foresight effect, which removes these immunities from consideration.
    { TYPE_NORMAL,   TYPE_GHOST,    TYPE_MULTI_IMMUNE        },
    { TYPE_FIGHTING, TYPE_GHOST,    TYPE_MULTI_IMMUNE        },

    { 0xFF, 0xFF, TYPE_MULTI_IMMUNE },
};

/**
 * @brief Check if the basic type multiplier applies.
 * 
 * @param battleCtx 
 * @param attacker 
 * @param defender 
 * @param chartEntry    Index of the entry into the type-chart
 * @return TRUE if there are no active effects to override the given chart-entry,
 * FALSE if the chart-entry should be overriden
 */
static BOOL BasicTypeMulApplies(BattleContext *battleCtx, int attacker, int defender, int chartEntry)
{
    int itemEffect = Battler_HeldItemEffect(battleCtx, defender);
    BOOL result = TRUE;

    if ((itemEffect == HOLD_EFFECT_SPEED_DOWN_GROUNDED || (battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_INGRAIN))
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_FLYING
            && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE) {
        result = FALSE;
    }

    if (battleCtx->turnFlags[defender].roosting
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_FLYING) {
        result = FALSE;
    }

    if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY)
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_FLYING
            && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE) {
        result = FALSE;
    }

    if ((battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_MIRACLE_EYE)
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_DARK
            && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE) {
        result = FALSE;
    }
	
	if ((Battler_Ability(battleCtx, attacker) == ABILITY_CORROSION)
	&& MOVE_DATA(battleCtx->moveCur).type == TYPE_POISON
	&& sTypeMatchupMultipliers[chartEntry][1] == TYPE_STEEL
    && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE)
	{
		result = FALSE;
	}

    return result;
}

/**
 * @brief Check if the basic type multiplier applies.
 * 
 * @param battleCtx 
 * @param attacker         The battlemon whose party will be considered.
 * @param defender 
 * @param chartEntry       Index of the entry into the type-chart
 * @param move             Current move being used, pass from parent function.
 * @param partyIndicator   The id of the battle mon whose party should be referenced.
 * @param partySlot        Party slot index of party mon.
 *
 * @return TRUE if there are no active effects to override the given chart-entry,
 * FALSE if the chart-entry should be overriden
 */
static BOOL BasicTypeMulApplies_PartyMon(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender, int chartEntry, int move, int partyIndicator, int partySlot)
{
    int itemEffect = Battler_HeldItemEffect(battleCtx, defender);
    BOOL result = TRUE;
    Pokemon *mon;

    mon = BattleSystem_PartyPokemon(battleSys, partyIndicator, partySlot);

    if ((itemEffect == HOLD_EFFECT_SPEED_DOWN_GROUNDED || (battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_INGRAIN))
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_FLYING
            && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE) {
        result = FALSE;
    }

    if (battleCtx->turnFlags[defender].roosting
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_FLYING) {
        result = FALSE;
    }

    if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY)
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_FLYING
            && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE) {
        result = FALSE;
    }

    if ((battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_MIRACLE_EYE)
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_DARK
            && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE) {
        result = FALSE;
    }
	
	if ((Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) == ABILITY_CORROSION)
	&& MOVE_DATA(move).type == TYPE_POISON
	&& sTypeMatchupMultipliers[chartEntry][1] == TYPE_STEEL
    && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE)
	{
		result = FALSE;
	}

    return result;
}

static u16 sSoundMoves[] = {
    MOVE_GROWL,
    MOVE_ROAR,
    MOVE_SING,
    MOVE_SUPERSONIC,
    MOVE_SCREECH,
    MOVE_SNORE,
    MOVE_UPROAR,
    MOVE_METAL_SOUND,
    MOVE_GRASS_WHISTLE,
    MOVE_HYPER_VOICE,
    MOVE_BUG_BUZZ,
    MOVE_CHATTER,
    MOVE_PERISH_SONG,
};

static u16 sPowderMoves[] = {
		MOVE_POISON_POWDER,
		MOVE_SLEEP_POWDER,
		MOVE_STUN_SPORE,
		MOVE_SPORE,
		MOVE_COTTON_SPORE,
};

int BattleSystem_ApplyTypeChart(BattleSystem *battleSys, BattleContext *battleCtx, int move, int inType, int attacker, int defender, int damage, u32 *moveStatusMask)
{
    int chartEntry;
    int totalMul;
    u8 moveType;
    u32 movePower;
    u8 attackerItemEffect;
    u8 defenderItemEffect;
    u8 attackerItemPower;
    u8 defenderItemPower;
	
	int powderMove = FALSE;

    totalMul = 1;

    if (move == MOVE_STRUGGLE) {
        return damage;
    }
	
	for (int i = 0; i < NELEMS(sPowderMoves); i++)
	{
		if (sPowderMoves[i] == move)
		{
			powderMove = TRUE;
			break;
		}
	}

    attackerItemEffect = Battler_HeldItemEffect(battleCtx, attacker);
    attackerItemPower = Battler_HeldItemPower(battleCtx, attacker, ITEM_POWER_CHECK_ALL);
    defenderItemEffect = Battler_HeldItemEffect(battleCtx, defender);
    defenderItemPower = Battler_HeldItemPower(battleCtx, defender, ITEM_POWER_CHECK_ALL);

    if (Battler_Ability(battleCtx, attacker) == ABILITY_NORMALIZE) {
        moveType = TYPE_NORMAL;
    } else if (inType) {
        moveType = inType;
    } else {
        moveType = MOVE_DATA(move).type;
    }

    movePower = MOVE_DATA(move).power;

    if ((battleCtx->battleStatusMask & SYSCTL_IGNORE_TYPE_CHECKS) == FALSE && MON_HAS_TYPE(attacker, moveType)) {
        if (Battler_Ability(battleCtx, attacker) == ABILITY_ADAPTABILITY) {
            damage *= 2;
        } else {
            damage = damage * 15 / 10;
        }
    }

    if ((moveType == TYPE_GROUND)
    && ((Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_LEVITATE) == TRUE) || ((defenderItemEffect == HOLD_EFFECT_LEVITATE_POPPED_IF_HIT) && ((battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY) == FALSE)))
    && defenderItemEffect != HOLD_EFFECT_SPEED_DOWN_GROUNDED
    && (battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_INGRAIN) == FALSE)
	{ 
        *moveStatusMask |= MOVE_STATUS_LEVITATED;
    }
    else if ((moveType == TYPE_GROUND)
	&& battleCtx->battleMons[defender].moveEffectsData.magnetRiseTurns
    && (battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_INGRAIN) == FALSE
    && defenderItemEffect != HOLD_EFFECT_SPEED_DOWN_GROUNDED)
	{      
        *moveStatusMask |= MOVE_STATUS_MAGNET_RISE;
    }
	else if ((powderMove == TRUE)
	&& (MON_HAS_TYPE(battleCtx->defender, TYPE_GRASS) || (defenderItemEffect == HOLD_EFFECT_NO_WEATHER_CHIP_POWDER)))
	{
		*moveStatusMask |= MOVE_STATUS_INEFFECTIVE;
	}
    else {
        chartEntry = 0;

        while (sTypeMatchupMultipliers[chartEntry][0] != 0xFF) {
            if (sTypeMatchupMultipliers[chartEntry][0] == 0xFE) {
                // The Ghost-type immunities are listed separately and ignored as a batch
                if ((battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_FORESIGHT)
                        || Battler_Ability(battleCtx, attacker) == ABILITY_SCRAPPY
						|| ((attackerItemEffect == HOLD_EFFECT_NORMAL_HIT_GHOST) && (moveType == TYPE_NORMAL))) {
                    break;
                } else {
                    chartEntry++;
                    continue;
                }
            }

            if (sTypeMatchupMultipliers[chartEntry][0] == moveType) {
                if (sTypeMatchupMultipliers[chartEntry][1] == BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL)
                        && BasicTypeMulApplies(battleCtx, attacker, defender, chartEntry) == TRUE) {
                    damage = ApplyTypeMultiplier(battleCtx, attacker, sTypeMatchupMultipliers[chartEntry][2], damage, movePower, moveStatusMask);

                    if (sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_SUPER_EFF) {
                        totalMul *= 2;
                    }
                }

                if (sTypeMatchupMultipliers[chartEntry][1] == BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL)
                        && BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL) != BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL)
                        && BasicTypeMulApplies(battleCtx, attacker, defender, chartEntry) == TRUE) {
                    damage = ApplyTypeMultiplier(battleCtx, attacker, sTypeMatchupMultipliers[chartEntry][2], damage, movePower, moveStatusMask);

                    if (sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_SUPER_EFF) {
                        totalMul *= 2;
                    }
                }
            }

            chartEntry++;
        }
    }

    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_WONDER_GUARD) == TRUE
            && MoveIsOnDamagingTurn(battleCtx, move)
            && ((*moveStatusMask & MOVE_STATUS_SUPER_EFFECTIVE) == FALSE
                || (*moveStatusMask & MOVE_STATUS_BASIC_EFFECTIVENESS) == MOVE_STATUS_BASIC_EFFECTIVENESS)
            && movePower) {
        *moveStatusMask |= MOVE_STATUS_WONDER_GUARD;
    } else if ((battleCtx->battleStatusMask & SYSCTL_IGNORE_TYPE_CHECKS) == FALSE
            && (battleCtx->battleStatusMask & SYSCTL_IGNORE_IMMUNITIES) == FALSE) {
        if ((*moveStatusMask & MOVE_STATUS_SUPER_EFFECTIVE) && movePower) {
            if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_FILTER) == TRUE
                    || Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_SOLID_ROCK) == TRUE) {
                damage = BattleSystem_Divide(damage * 3, 4);
            }

            if (attackerItemEffect == HOLD_EFFECT_POWER_UP_SE) {
                damage = damage * (100 + attackerItemPower) / 100;
            }
			
			if ((defenderItemEffect == HOLD_EFFECT_WEAK_RAISE_SPA_ATK)
			&& (DEFENDING_MON.curHP)
			&& (movePower)
			&& (DEFENDING_MON.wpolicyFlag == FALSE))
			{
				DEFENDING_MON.wpolicyFlag = TRUE;
			}
        }

        if ((*moveStatusMask & MOVE_STATUS_NOT_VERY_EFFECTIVE) && movePower) {
            if (Battler_Ability(battleCtx, attacker) == ABILITY_TINTED_LENS) {
                damage *= 2;
            }
        }
    } else {
        *moveStatusMask &= ~MOVE_STATUS_SUPER_EFFECTIVE;
        *moveStatusMask &= ~MOVE_STATUS_NOT_VERY_EFFECTIVE;
    }

    
    if (moveType == TYPE_WATER)
	{
        if ((Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_WATER_ABSORB) == TRUE)
            || (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_DRY_SKIN) == TRUE)) {

            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_HEAL_ABILITY;
        }

        if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_STORM_DRAIN == TRUE)) {

            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_RAISE_STAT_ABILITY;
        }
    }
    if (moveType == TYPE_ELECTRIC)
	{
        if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_VOLT_ABSORB) == TRUE) {

            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_HEAL_ABILITY;
        }
         
        if ((Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_LIGHTNING_ROD) == TRUE)
            || Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_MOTOR_DRIVE) == TRUE) {

            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_RAISE_STAT_ABILITY;
        }
    }
    if (moveType == TYPE_FIRE)
	{
        if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_FLASH_FIRE) == TRUE)
        {
            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_TYPE_BOOST_ABILITY;
        }

        if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_DRY_SKIN) == TRUE) 
        {
            *moveStatusMask |= MOVE_STATUS_TYPE_WEAKNESS_ABILITY;
        }

        if ((Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_HEATPROOF) == TRUE)
            || (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_THICK_FAT) == TRUE)) 
        {
            *moveStatusMask |= MOVE_STATUS_TYPE_RESIST_ABILITY;
        }
    }
    if (moveType == TYPE_ICE) 
    {
        if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_THICK_FAT) == TRUE)
        {
            *moveStatusMask |= MOVE_STATUS_TYPE_RESIST_ABILITY;
        }
    }
    if (moveType == TYPE_NORMAL)
    {
        if (MON_HAS_TYPE(defender, TYPE_GHOST))
        {
            if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_SCRAPPY) == TRUE)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY_ABILITY;
            }

            if (battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_FORESIGHT)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY;
            }

            if (attackerItemEffect == HOLD_EFFECT_NORMAL_HIT_GHOST)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY_ITEM;
            }
        }
    }
    if (moveType == TYPE_FIGHTING)
    {
        if (MON_HAS_TYPE(defender, TYPE_GHOST))
        {
            if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_SCRAPPY) == TRUE)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY_ABILITY;
            }

            if (battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_FORESIGHT)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY;
            }
        }
    }
    if (moveType == TYPE_POISON)
    {
        if (MON_HAS_TYPE(defender, TYPE_STEEL))
        {
            if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_CORROSION) == TRUE)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY_ABILITY;
            }
        }
    }

    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_SOUNDPROOF) == TRUE) {

        for (int i = 0; i < NELEMS(sSoundMoves); i++)
	    {
		    if (sSoundMoves[i] == move)
		    {
			    *moveStatusMask |= MOVE_STATUS_INEFFECTIVE;
			    break;
		    }
	    }
    }

    return damage;
}

int PartyMon_ApplyTypeChart(BattleSystem *battleSys, BattleContext *battleCtx, int move, int inType, int attacker, int defender, int damage, int partyIndicator, int partySlot, u32 *moveStatusMask)
{
    int chartEntry;
    int totalMul;
    u8 moveType;
    u32 movePower;
    u8 attackerItemEffect;
    u8 defenderItemEffect;
    u8 attackerItemPower;
    u8 defenderItemPower;
    Pokemon *mon;
    u8 monAbility;
    u16 monItem;

    mon = BattleSystem_PartyPokemon(battleSys, partyIndicator, partySlot);
    monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    monItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
	
	int powderMove = FALSE;

    totalMul = 1;

    if (move == MOVE_STRUGGLE) {
        return damage;
    }
	
	for (int i = 0; i < NELEMS(sPowderMoves); i++)
	{
		if (move == sPowderMoves[i])
		{
			powderMove = TRUE;
			break;
		}
	}

    attackerItemEffect = BattleSystem_GetItemData(battleCtx, monItem, ITEM_PARAM_HOLD_EFFECT);
    attackerItemPower = BattleSystem_GetItemData(battleCtx, monItem, ITEM_PARAM_HOLD_EFFECT_PARAM);
    defenderItemEffect = Battler_HeldItemEffect(battleCtx, defender);
    defenderItemPower = Battler_HeldItemPower(battleCtx, defender, ITEM_POWER_CHECK_ALL);

    if (monAbility == ABILITY_NORMALIZE) {
        moveType = TYPE_NORMAL;
    } else if (inType) {
        moveType = inType;
    } else {
        moveType = MOVE_DATA(move).type;
    }

    movePower = MOVE_DATA(move).power;

    if ((battleCtx->battleStatusMask & SYSCTL_IGNORE_TYPE_CHECKS) == FALSE && MON_HAS_TYPE(attacker, moveType)) {
        if (monAbility == ABILITY_ADAPTABILITY) {
            damage *= 2;
        } else {
            damage = damage * 15 / 10;
        }
    }

    if ((moveType == TYPE_GROUND)
    && ((PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_LEVITATE) == TRUE) || ((defenderItemEffect == HOLD_EFFECT_LEVITATE_POPPED_IF_HIT) && ((battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY) == FALSE)))
    && defenderItemEffect != HOLD_EFFECT_SPEED_DOWN_GROUNDED
    && (battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_INGRAIN) == FALSE)
	{ 
        *moveStatusMask |= MOVE_STATUS_LEVITATED;
    }
    else if ((moveType == TYPE_GROUND)
	&& battleCtx->battleMons[defender].moveEffectsData.magnetRiseTurns
    && (battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_INGRAIN) == FALSE
    && defenderItemEffect != HOLD_EFFECT_SPEED_DOWN_GROUNDED)
	{      
        *moveStatusMask |= MOVE_STATUS_MAGNET_RISE;
    }
	else if ((powderMove == TRUE)
	&& ((Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL) == TYPE_GRASS || Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL) == TYPE_GRASS)
    || (defenderItemEffect == HOLD_EFFECT_NO_WEATHER_CHIP_POWDER)))
	{
		*moveStatusMask |= MOVE_STATUS_INEFFECTIVE;
	}
    else {
        chartEntry = 0;

        while (sTypeMatchupMultipliers[chartEntry][0] != 0xFF) {
            if (sTypeMatchupMultipliers[chartEntry][0] == 0xFE) {
                // The Ghost-type immunities are listed separately and ignored as a batch
                if ((battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_FORESIGHT)
                        || monAbility == ABILITY_SCRAPPY
						|| ((attackerItemEffect == HOLD_EFFECT_NORMAL_HIT_GHOST) && (moveType == TYPE_NORMAL))) {
                    break;
                } else {
                    chartEntry++;
                    continue;
                }
            }

            if (sTypeMatchupMultipliers[chartEntry][0] == moveType) {
                if (sTypeMatchupMultipliers[chartEntry][1] == BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL)
                        && BasicTypeMulApplies_PartyMon(battleSys, battleCtx, attacker, defender, chartEntry, move, partyIndicator, partySlot) == TRUE) {
                    damage = ApplyTypeMultiplier(battleCtx, attacker, sTypeMatchupMultipliers[chartEntry][2], damage, movePower, moveStatusMask);

                    if (sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_SUPER_EFF) {
                        totalMul *= 2;
                    }
                }

                if (sTypeMatchupMultipliers[chartEntry][1] == BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL)
                        && BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL) != BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL)
                        && BasicTypeMulApplies_PartyMon(battleSys, battleCtx, attacker, defender, chartEntry, move, partyIndicator, partySlot) == TRUE) {
                    damage = ApplyTypeMultiplier(battleCtx, attacker, sTypeMatchupMultipliers[chartEntry][2], damage, movePower, moveStatusMask);

                    if (sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_SUPER_EFF) {
                        totalMul *= 2;
                    }
                }
            }

            chartEntry++;
        }
    }

    if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_WONDER_GUARD) == TRUE
            && ((*moveStatusMask & MOVE_STATUS_SUPER_EFFECTIVE) == FALSE
                || (*moveStatusMask & MOVE_STATUS_BASIC_EFFECTIVENESS) == MOVE_STATUS_BASIC_EFFECTIVENESS)
            && movePower) {
        *moveStatusMask |= MOVE_STATUS_WONDER_GUARD;
    } else if ((battleCtx->battleStatusMask & SYSCTL_IGNORE_TYPE_CHECKS) == FALSE
            && (battleCtx->battleStatusMask & SYSCTL_IGNORE_IMMUNITIES) == FALSE) {
        if ((*moveStatusMask & MOVE_STATUS_SUPER_EFFECTIVE) && movePower) {
            if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_FILTER) == TRUE
                    || PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_SOLID_ROCK) == TRUE) {
                damage = BattleSystem_Divide(damage * 3, 4);
            }

            if (attackerItemEffect == HOLD_EFFECT_POWER_UP_SE) {
                damage = damage * (100 + attackerItemPower) / 100;
            }
        }

        if ((*moveStatusMask & MOVE_STATUS_NOT_VERY_EFFECTIVE) && movePower) {
            if (Battler_Ability(battleCtx, attacker) == ABILITY_TINTED_LENS) {
                damage *= 2;
            }
        }
    } else {
        *moveStatusMask &= ~MOVE_STATUS_SUPER_EFFECTIVE;
        *moveStatusMask &= ~MOVE_STATUS_NOT_VERY_EFFECTIVE;
    }

    
    if (moveType == TYPE_WATER)
	{
        if ((PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_WATER_ABSORB) == TRUE)
            || (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_DRY_SKIN) == TRUE)) {

            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_HEAL_ABILITY;
        }

        if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_STORM_DRAIN == TRUE)) {

            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_RAISE_STAT_ABILITY;
        }
    }
    if (moveType == TYPE_ELECTRIC)
	{
        if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_VOLT_ABSORB) == TRUE) {

            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_HEAL_ABILITY;
        }
         
        if ((PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_LIGHTNING_ROD) == TRUE)
            || PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_MOTOR_DRIVE) == TRUE) {

            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_RAISE_STAT_ABILITY;
        }
    }
    if (moveType == TYPE_FIRE)
	{
        if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_FLASH_FIRE) == TRUE)
        {
            *moveStatusMask |= MOVE_STATUS_TYPE_IMMUNE_TYPE_BOOST_ABILITY;
        }

        if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_DRY_SKIN) == TRUE) 
        {
            *moveStatusMask |= MOVE_STATUS_TYPE_WEAKNESS_ABILITY;
        }

        if ((PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_HEATPROOF) == TRUE)
            || (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_THICK_FAT) == TRUE)) 
        {
            *moveStatusMask |= MOVE_STATUS_TYPE_RESIST_ABILITY;
        }
    }
    if (moveType == TYPE_ICE) 
    {
        if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_THICK_FAT) == TRUE)
        {
            *moveStatusMask |= MOVE_STATUS_TYPE_RESIST_ABILITY;
        }
    }
    if (moveType == TYPE_NORMAL)
    {
        if (MON_HAS_TYPE(defender, TYPE_GHOST))
        {
            if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_SCRAPPY) == TRUE)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY_ABILITY;
            }

            if (battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_FORESIGHT)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY;
            }

            if (attackerItemEffect == HOLD_EFFECT_NORMAL_HIT_GHOST)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY_ITEM;
            }
        }
    }
    if (moveType == TYPE_FIGHTING)
    {
        if (MON_HAS_TYPE(defender, TYPE_GHOST))
        {
            if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_SCRAPPY) == TRUE)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY_ABILITY;
            }

            if (battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_FORESIGHT)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY;
            }
        }
    }
    if (moveType == TYPE_POISON)
    {
        if (MON_HAS_TYPE(defender, TYPE_STEEL))
        {
            if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_CORROSION) == TRUE)
            {
                *moveStatusMask |= MOVE_STATUS_TYPE_IGNORE_IMMUNITY_ABILITY;
            }
        }
    }

    if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_SOUNDPROOF) == TRUE) {

        for (int i = 0; i < NELEMS(sSoundMoves); i++)
	    {
		    if (sSoundMoves[i] == move)
		    {
			    *moveStatusMask |= MOVE_STATUS_INEFFECTIVE;
			    break;
		    }
	    }
    }

    return damage;
}

void BattleSystem_CalcEffectiveness(BattleContext *battleCtx, int move, int inType, int attackerAbility, int defenderAbility, int defenderItemEffect, int defenderType1, int defenderType2, u32 *moveStatusMask)
{
    int chartEntry;
    u8 moveType;

    if (move == MOVE_STRUGGLE) {
        return;
    }

    if (attackerAbility == ABILITY_NORMALIZE) {
        moveType = TYPE_NORMAL;
    } else if (inType) {
        moveType = inType;
    } else {
        moveType = MOVE_DATA(move).type;
    }
	
    if (attackerAbility != ABILITY_MOLD_BREAKER
            && (defenderAbility == ABILITY_LEVITATE)
            && moveType == TYPE_GROUND
            && (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY) == FALSE
            && defenderItemEffect != HOLD_EFFECT_SPEED_DOWN_GROUNDED) {
        *moveStatusMask |= MOVE_STATUS_INEFFECTIVE;
    }
	else if (defenderItemEffect == HOLD_EFFECT_LEVITATE_POPPED_IF_HIT
            && moveType == TYPE_GROUND
            && (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY) == FALSE
            && defenderItemEffect != HOLD_EFFECT_SPEED_DOWN_GROUNDED) {
        *moveStatusMask |= MOVE_STATUS_INEFFECTIVE;
    } else {
        chartEntry = 0;

        while (sTypeMatchupMultipliers[chartEntry][0] != 0xFF) {
            if (sTypeMatchupMultipliers[chartEntry][0] == 0xFE) {
                if (attackerAbility == ABILITY_SCRAPPY) {
                    break;
                } else {
                    chartEntry++;
                    continue;
                }
            }

            if (sTypeMatchupMultipliers[chartEntry][0] == moveType) {
                if (sTypeMatchupMultipliers[chartEntry][1] == defenderType1
                        && NoImmunityOverrides(battleCtx, defenderItemEffect, chartEntry) == TRUE) {
                    UpateMoveStatusForTypeMul(sTypeMatchupMultipliers[chartEntry][2], moveStatusMask);
                }

                if (sTypeMatchupMultipliers[chartEntry][1] == defenderType2
                        && defenderType1 != defenderType2
                        && NoImmunityOverrides(battleCtx, defenderItemEffect, chartEntry) == TRUE) {
                    UpateMoveStatusForTypeMul(sTypeMatchupMultipliers[chartEntry][2], moveStatusMask);
                }
            }

            chartEntry++;
        }
    }

    if (attackerAbility != ABILITY_MOLD_BREAKER
            && defenderAbility == ABILITY_WONDER_GUARD
            && MoveIsOnDamagingTurn(battleCtx, move)
            && ((*moveStatusMask & MOVE_STATUS_SUPER_EFFECTIVE) == FALSE
                || (*moveStatusMask & MOVE_STATUS_BASIC_EFFECTIVENESS) == MOVE_STATUS_BASIC_EFFECTIVENESS)) {
        *moveStatusMask |= MOVE_STATUS_INEFFECTIVE;
    }

    return;
}

/**
 * @brief Determines if there are no immunity-overriding effects in play.
 * 
 * This only checks for the basic immunity-overriding effects: Iron Ball
 * and Gravity for Ground-type attacks against  Flying-type Pokemon.
 * 
 * @param battleCtx 
 * @param itemEffect 
 * @param chartEntry 
 * @return TRUE if immunities should be respected, FALSE if not. 
 */
static BOOL NoImmunityOverrides(BattleContext *battleCtx, int itemEffect, int chartEntry)
{
    BOOL result = TRUE;

    if (itemEffect == HOLD_EFFECT_SPEED_DOWN_GROUNDED
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_FLYING
            && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE) {
        result = FALSE;
    }

    if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY)
            && sTypeMatchupMultipliers[chartEntry][1] == TYPE_FLYING
            && sTypeMatchupMultipliers[chartEntry][2] == TYPE_MULTI_IMMUNE) {
        result = FALSE;
    }

    return result;
}

/**
 * @brief Update the given move status mask according to a type multiplier.
 * 
 * @param mul 
 * @param moveStatusMask 
 */
static void UpateMoveStatusForTypeMul(int mul, u32 *moveStatusMask)
{
    switch (mul) {
    case TYPE_MULTI_IMMUNE:
        *moveStatusMask |= MOVE_STATUS_INEFFECTIVE;
        *moveStatusMask &= ~MOVE_STATUS_NOT_VERY_EFFECTIVE;
        *moveStatusMask &= ~MOVE_STATUS_SUPER_EFFECTIVE;
        break;

    case TYPE_MULTI_NOT_VERY_EFF:
        if (*moveStatusMask & MOVE_STATUS_SUPER_EFFECTIVE) {
            *moveStatusMask &= ~MOVE_STATUS_SUPER_EFFECTIVE;
        } else {
            *moveStatusMask |= MOVE_STATUS_NOT_VERY_EFFECTIVE;
        }
        break;

    case TYPE_MULTI_SUPER_EFF:
        if (*moveStatusMask & MOVE_STATUS_NOT_VERY_EFFECTIVE) {
            *moveStatusMask &= ~MOVE_STATUS_NOT_VERY_EFFECTIVE;
        } else {
            *moveStatusMask |= MOVE_STATUS_SUPER_EFFECTIVE;
        }
        break;
    }
}

BOOL BattleContext_MoveFailed(BattleContext *battleCtx, int battler)
{
    if (battleCtx->moveFailFlags[battler].paralyzed
            || battleCtx->moveFailFlags[battler].noEffect
            || battleCtx->moveFailFlags[battler].imprisoned
            || battleCtx->moveFailFlags[battler].infatuated
            || battleCtx->moveFailFlags[battler].disabled
            || battleCtx->moveFailFlags[battler].taunted
            || battleCtx->moveFailFlags[battler].flinched
            || battleCtx->moveFailFlags[battler].gravity
            || battleCtx->moveFailFlags[battler].confused) {
        return TRUE;
    }

    return FALSE;
}

u8 BattleSystem_CountAliveBattlers(BattleSystem *battleSys, BattleContext *battleCtx, BOOL sameSide, int defender)
{
    u8 count = 0;
    int maxBattlers = BattleSystem_MaxBattlers(battleSys);
    
    // no clue why they used a switch statement for this, but changing it to an if-else doesn't match
    switch (sameSide) {
    case FALSE:
        for (int i = 0; i < maxBattlers; i++) {
            if (i != defender && battleCtx->battleMons[i].curHP) {
                count++;
            }
        }
        break;

    case TRUE:
        for (int i = 0; i < maxBattlers; i++) {
            if (Battler_Side(battleSys, i) == Battler_Side(battleSys, defender)
                    && battleCtx->battleMons[i].curHP) {
                count++;
            }
        }
        break;
    }

    return count;
}

int BattleSystem_NicknameTag(BattleContext *battleSys, int battler)
{
    return battler | (battleSys->selectedPartySlot[battler] << 8);
}

u16 Battler_SelectedMove(BattleContext *battleCtx, int battler)
{
    u16 move = MOVE_NONE;

    if (battleCtx->battlerActions[battler][BATTLE_ACTION_SELECTED_COMMAND] == PLAYER_INPUT_FIGHT
            && battleCtx->battlerActions[battler][BATTLE_ACTION_TEMP_VALUE]) {
        move = battleCtx->battleMons[battler].moves[battleCtx->battlerActions[battler][BATTLE_ACTION_TEMP_VALUE] - 1];
    }

    return move;
}

int BattleSystem_CountAbility(BattleSystem *battleSys, BattleContext *battleCtx, enum CountAbilityMode mode, int battler, int ability)
{
    int result = 0;
    int i;
    int maxBattlers = BattleSystem_MaxBattlers(battleSys);

    switch (mode) {
    case COUNT_ALL_BATTLERS_OUR_SIDE:
        for (i = 0; i < maxBattlers; i++) {
            if (Battler_Side(battleSys, i) == Battler_Side(battleSys, battler)
                    && Battler_Ability(battleCtx, i) == ability) {
                result++;
            }
        }
        break;
    
    case COUNT_ALIVE_BATTLERS_OUR_SIDE:
        for (i = 0; i < maxBattlers; i++) {
            if (Battler_Side(battleSys, i) == Battler_Side(battleSys, battler)
                    && battleCtx->battleMons[i].curHP
                    && Battler_Ability(battleCtx, i) == ability) {
                result++;
            }
        }
        break;

    case COUNT_ALL_BATTLERS_THEIR_SIDE:
        for (i = 0; i < maxBattlers; i++) {
            if (Battler_Side(battleSys, i) != Battler_Side(battleSys, battler)
                    && Battler_Ability(battleCtx, i) == ability) {
                result++;
            }
        }
        break;

    case COUNT_ALIVE_BATTLERS_THEIR_SIDE:
        for (i = 0; i < maxBattlers; i++) {
            if (Battler_Side(battleSys, i) != Battler_Side(battleSys, battler)
                    && battleCtx->battleMons[i].curHP
                    && Battler_Ability(battleCtx, i) == ability) {
                result++;
            }
        }
        break;

    case COUNT_ALIVE_BATTLERS_THEIR_SIDE_FLAG:
        for (i = 0; i < maxBattlers; i++) {
            if (Battler_Side(battleSys, i) != Battler_Side(battleSys, battler)
                    && battleCtx->battleMons[i].curHP
                    && Battler_Ability(battleCtx, i) == ability) {
                result |= FlagIndex(i);
            }
        }
        break;

    case COUNT_ALL_BATTLERS:
        for (i = 0; i < maxBattlers; i++) {
            if (Battler_Ability(battleCtx, i) == ability) {
                result++;
            }
        }
        break;

    case COUNT_ALL_BATTLERS_EXCEPT_ME:
        for (i = 0; i < maxBattlers; i++) {
            if (i != battler
                    && Battler_Ability(battleCtx, i) == ability) {
                result++;
            }
        }
        break;

    case COUNT_ALL_BATTLERS_EXCEPT_ME_RETURN_BATTLER:
        for (i = 0; i < maxBattlers; i++) {
            if (i != battler
                    && Battler_Ability(battleCtx, i) == ability) {
                result = i + 1;
                break;
            }
        }
        break;

    case COUNT_ALIVE_BATTLERS:
        for (i = 0; i < maxBattlers; i++) {
            if (Battler_Ability(battleCtx, i) == ability
                    && battleCtx->battleMons[i].curHP) {
                result++;
            }
        }
        break;

    case COUNT_ALIVE_BATTLERS_EXCEPT_ME:
        for (i = 0; i < maxBattlers; i++) {
            if (i != battler
                    && Battler_Ability(battleCtx, i) == ability
                    && battleCtx->battleMons[i].curHP) {
                result++;
            }
        }
        break;
    }

    return result;
}

BOOL Move_IsMultiTurn(BattleContext *battleCtx, int move)
{
    switch (MOVE_DATA(move).effect) {
    case BATTLE_EFFECT_BIDE:
    case BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT:
    case BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT_FLINCH:
    case BATTLE_EFFECT_CHARGE_TURN_DEF_UP:
    case BATTLE_EFFECT_SKIP_CHARGE_TURN_IN_SUN:
    case BATTLE_EFFECT_FLY:
    case BATTLE_EFFECT_DIVE:
    case BATTLE_EFFECT_DIG:
    case BATTLE_EFFECT_BOUNCE:
    case BATTLE_EFFECT_SHADOW_FORCE:
        return TRUE;
    }

    return FALSE;
}

BOOL BattleSystem_TypeMatchup(BattleSystem *battleSys, int idx, u8 *moveType, u8 *vsType, u8 *multi)
{
    BOOL result = TRUE;
    if (idx >= NELEMS(sTypeMatchupMultipliers)) {
        idx = BattleSystem_RandNext(battleSys) % NELEMS(sTypeMatchupMultipliers);
        result = FALSE;
    }

    *moveType = sTypeMatchupMultipliers[idx][0];
    *vsType = sTypeMatchupMultipliers[idx][1];
    *multi = sTypeMatchupMultipliers[idx][2];

    return result;
}

int BattleSystem_TypeMatchupMultiplier(u8 attackingType, u8 defendingType1, u8 defendingType2)
{
    int i = 0;
    int mul = 40;

    while (sTypeMatchupMultipliers[i][0] != 0xFF) {
        if (sTypeMatchupMultipliers[i][0] == attackingType) {
            if (sTypeMatchupMultipliers[i][1] == defendingType1) {
                mul = mul * sTypeMatchupMultipliers[i][2] / 10;
            }

            if (sTypeMatchupMultipliers[i][1] == defendingType2
                    && defendingType1 != defendingType2) {
                mul = mul * sTypeMatchupMultipliers[i][2] / 10;
            }
        }

        i++;
    }

    return mul;
}

BOOL Move_IsInvoker(u16 move)
{
    // declaring this as a single return didn't match
    if (move == MOVE_NONE
            || move == MOVE_SLEEP_TALK
            || move == MOVE_COPYCAT
            || move == MOVE_ASSIST
            || move == MOVE_ME_FIRST
            || move == MOVE_MIRROR_MOVE
            || move == MOVE_METRONOME) {
        return TRUE;
    }

    return FALSE;
}

BOOL Move_IsGhostCurse(BattleContext *battleCtx, u16 move, int battler)
{
    return move == MOVE_CURSE && MON_HAS_TYPE(battler, TYPE_GHOST);
}

BOOL BattleSystem_CanStealItem(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result = FALSE;
    int side = Battler_Side(battleSys, battler);

    if (battleCtx->battleMons[battler].heldItem
            && (battleCtx->sideConditions[side].knockedOffItemsMask & FlagIndex(battleCtx->selectedPartySlot[battler])) == FALSE
            && Item_IsMail(battleCtx->battleMons[battler].heldItem) == FALSE) {
        result = TRUE;
    }

    return result;
}

BOOL BattleSystem_NotHoldingMail(BattleContext *battleCtx, int battler)
{
    return Item_IsMail(battleCtx->battleMons[battler].heldItem) == FALSE;
}

BOOL BattleSystem_CanWhirlwind(BattleSystem *battleSys, BattleContext *battleCtx)
{
    BOOL result = FALSE;

    if (ATTACKING_MON.level >= DEFENDING_MON.level) {
        result = TRUE;
    } else {
        int tmp = (((BattleSystem_RandNext(battleSys) & 0xFF) * (ATTACKING_MON.level + DEFENDING_MON.level)) >> 8) + 1;
        if (tmp > battleCtx->battleMons[battleCtx->defender].level / 4) {
            result = TRUE;
        }
    }

    return result;
}

u8 Battler_Ability(BattleContext *battleCtx, int battler)
{
    if ((battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_ABILITY_SUPPRESSED)
            && battleCtx->battleMons[battler].ability != ABILITY_MULTITYPE) {
        return ABILITY_NONE;
    } 
    
    if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY)
            && battleCtx->battleMons[battler].ability == ABILITY_LEVITATE) {
        return ABILITY_NONE;
    } 
    
    if ((battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_INGRAIN)
            && battleCtx->battleMons[battler].ability == ABILITY_LEVITATE) {
        return ABILITY_NONE;
    }
    
    return battleCtx->battleMons[battler].ability;
}

BOOL Battler_IgnorableAbility(BattleContext *battleCtx, int attacker, int defender, int ability)
{
    BOOL result = FALSE;

    if (Battler_Ability(battleCtx, attacker) != ABILITY_MOLD_BREAKER) {
        if (Battler_Ability(battleCtx, defender) == ability) {
            result = TRUE;
        }
    } else if (Battler_Ability(battleCtx, defender) == ability
            && battleCtx->selfTurnFlags[attacker].moldBreakerActivated == FALSE) {
        battleCtx->selfTurnFlags[attacker].moldBreakerActivated = TRUE;
        battleCtx->battleStatusMask |= SYSCTL_APPLY_MOLD_BREAKER;
    }

    return result;
}

BOOL PartyMon_IgnorableAbility(BattleContext *battleCtx, Pokemon *mon, int defender, int ability)
{
    BOOL result = FALSE;

    if (Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL) != ABILITY_MOLD_BREAKER) {
        if (Battler_Ability(battleCtx, defender) == ability) {
            result = TRUE;
        }
    }

    return result;
}

BOOL BattleSystem_AnyReplacementMons(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    // Declarations here are done C89-style to match.
    BOOL result;
    Party *party;
    Pokemon *pokemon;
    int partySize;
    int aliveMons = 0, neededAliveMons;
    int start, end;
    int selectedSlot1, selectedSlot2;
    u32 battleType;

    result = FALSE;
    battleType = BattleSystem_BattleType(battleSys);
    party = BattleSystem_Party(battleSys, battler);
    partySize = BattleSystem_PartyCount(battleSys, battler);

    if ((battleType & BATTLE_TYPE_2vs2)
            || ((battleType & BATTLE_TYPE_TAG)
                && (BattleSystem_BattlerSlot(battleSys, battler) & BATTLER_THEM))) {
        // have to copy these 4 identical assignments across each branch to match
        start = 0;
        end = partySize;
        neededAliveMons = 1;
        selectedSlot1 = battleCtx->selectedPartySlot[battler];
        selectedSlot2 = battleCtx->selectedPartySlot[battler];
    } else if (battleType & BATTLE_TYPE_DOUBLES) {
        start = 0;
        end = partySize;
        neededAliveMons = 1;
        selectedSlot1 = battleCtx->selectedPartySlot[battler];
        selectedSlot2 = battleCtx->selectedPartySlot[BattleSystem_Partner(battleSys, battler)];
    } else {
        start = 0;
        end = partySize;
        neededAliveMons = 1;
        selectedSlot1 = battleCtx->selectedPartySlot[battler];
        selectedSlot2 = battleCtx->selectedPartySlot[battler];
    }

    for (int i = start; i < end; i++) {
        pokemon = Party_GetPokemonBySlotIndex(party, i);
        if (Pokemon_GetValue(pokemon, MON_DATA_SPECIES, NULL)
                && Pokemon_GetValue(pokemon, MON_DATA_IS_EGG, NULL) == FALSE
                && Pokemon_GetValue(pokemon, MON_DATA_CURRENT_HP, NULL)
                && selectedSlot1 != i
                && selectedSlot2 != i) {
            aliveMons++;
        }
    }

    if (aliveMons >= neededAliveMons) {
        result = TRUE;
    }

    return result;
}

BOOL Battler_IsTrappedMsg(BattleSystem *battleSys, BattleContext *battleCtx, int battler, BattleMessage *msgOut)
{
    int tmp;
    int maxBattlers;
    u8 side;
    int itemEffect;
    u32 battleType;

    battleType = BattleSystem_BattleType(battleSys);
    itemEffect = Battler_HeldItemEffect(battleCtx, battler);

    if (itemEffect == HOLD_EFFECT_FLEE
            || (battleType & BATTLE_TYPE_NO_EXPERIENCE)
            || Battler_Ability(battleCtx, battler) == ABILITY_RUN_AWAY) {
        return FALSE;
    }

    side = Battler_Side(battleSys, battler);
    maxBattlers = BattleSystem_MaxBattlers(battleSys);

    if ((tmp = BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_EXCEPT_ME, battler, ABILITY_SHADOW_TAG))
            && Battler_Ability(battleCtx, battler) != ABILITY_SHADOW_TAG) {
        if (msgOut == NULL) {
            return TRUE;
        }

        msgOut->tags = TAG_NICKNAME_ABILITY;
        msgOut->id = 39; // "{0} prevents escape with {1}!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, tmp);
        msgOut->params[1] = ABILITY_SHADOW_TAG;
        return TRUE;
    }

    if ((tmp = BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_THEIR_SIDE, battler, ABILITY_ARENA_TRAP))) {
        if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY) == FALSE && itemEffect != HOLD_EFFECT_SPEED_DOWN_GROUNDED) {
            if (Battler_Ability(battleCtx, battler) != ABILITY_LEVITATE
					&& Battler_HeldItemEffect(battleCtx, battleCtx->defender) != HOLD_EFFECT_LEVITATE_POPPED_IF_HIT
                    && battleCtx->battleMons[battler].moveEffectsData.magnetRiseTurns == 0
                    && MON_IS_NOT_TYPE(battler, TYPE_FLYING)) {
                if (msgOut == NULL) {
                    return TRUE;
                }

                msgOut->tags = TAG_NICKNAME_ABILITY;
                msgOut->id = 39; // "{0} prevents escape with {1}!"
                msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, tmp);
                msgOut->params[1] = ABILITY_ARENA_TRAP;
                return TRUE;
            }
        } else {
            if (msgOut == NULL) {
                return TRUE;
            }

            msgOut->tags = TAG_NICKNAME_ABILITY;
            msgOut->id = 39; // "{0} prevents escape with {1}!"
            msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, tmp);
            msgOut->params[1] = ABILITY_ARENA_TRAP;
            return TRUE;
        }
    }

    if ((tmp = BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_THEIR_SIDE, battler, ABILITY_MAGNET_PULL))
            && MON_HAS_TYPE(battler, TYPE_STEEL)) {
        if (msgOut == NULL) {
            return TRUE;
        }

        msgOut->tags = TAG_NICKNAME_ABILITY;
        msgOut->id = 39; // "{0} prevents escape with {1}!"
        msgOut->params[0] = BattleSystem_NicknameTag(battleCtx, tmp);
        msgOut->params[1] = ABILITY_MAGNET_PULL;
        return TRUE;
    }

    if ((battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_TRAPPED)
            || (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_INGRAIN)) {
        if (msgOut == NULL) {
            return TRUE;
        }

        msgOut->tags = TAG_NONE;
        msgOut->id = 794; // "Can't escape!"
        return TRUE;
    }

    return FALSE;
}

BOOL Battler_CanEscape(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    u32 battleType = BattleSystem_BattleType(battleSys);
    int itemEffect = Battler_HeldItemEffect(battleCtx, battler);
    BOOL result = FALSE;

    if (itemEffect == HOLD_EFFECT_FLEE) {
        battleCtx->turnFlags[battler].fleeing = 1;
        result = TRUE;
    } else if (battleType & BATTLE_TYPE_NO_EXPERIENCE) {
        result = TRUE;
    } else if (Battler_Ability(battleCtx, battler) == ABILITY_RUN_AWAY) {
        battleCtx->turnFlags[battler].fleeing = 2;
        result = TRUE;
    } else {
        if (battleCtx->battleMons[battler].speed < battleCtx->battleMons[battler ^ 1].speed) {
            u8 escape = battleCtx->battleMons[battler].speed * 128 / battleCtx->battleMons[battler ^ 1].speed + battleCtx->runAttempts * 30;

            if (escape > BattleSystem_RandNext(battleSys) % 256) {
                result = TRUE;
            }
        } else {
            result = TRUE;
        }

        if (result == FALSE) {
            BattleIO_IncrementRecord(battleSys, battler, 0, (((70 + 1)) + 27));
        }

        battleCtx->runAttempts++;
    }

    return result;
}

BOOL Battler_CheckTruant(BattleContext * battleCtx, int battler)
{
    BOOL result = FALSE;

    if (Battler_Ability(battleCtx, battler) == ABILITY_TRUANT
            && battleCtx->battleMons[battler].moveEffectsData.truant != (battleCtx->totalTurns & 1)) {
        result = TRUE;
    }

    return result;
}

BOOL Move_Imprisoned(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int move)
{
    // must declare C89-style to match
    int i, maxBattlers, side, j;
    BOOL result;

    result = FALSE;
    maxBattlers = BattleSystem_MaxBattlers(battleSys);
    side = Battler_Side(battleSys, battler);

    for (i = 0; i < maxBattlers; i++) {
        if (side != Battler_Side(battleSys, i)
                && (battleCtx->battleMons[i].moveEffectsMask & MOVE_EFFECT_IMPRISON)) {
            for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                if (move == battleCtx->battleMons[i].moves[j]) {
                    break;
                }
            }

            if (j != LEARNED_MOVES_MAX) {
                result = TRUE;
            }
        }
    }

    return result;
}

BOOL BattleSystem_AnyBattlersWithMoveEffect(BattleSystem *battleSys, BattleContext *battleCtx, int effectMask)
{
    BOOL result = FALSE;
    int maxBattlers = BattleSystem_MaxBattlers(battleSys);

    for (int i = 0; i < maxBattlers; i++) {
        if (battleCtx->battleMons[i].moveEffectsMask & effectMask) {
            result = TRUE;
            break;
        }
    }

    return result;
}

void BattleSystem_SetupLoop(BattleSystem *battleSys, BattleContext *battleCtx)
{
    battleCtx->moveStatusFlags = 0;
    battleCtx->criticalMul = 1;
    battleCtx->battleStatusMask &= ~SYSCTL_REUSE_LAST_MOVE;
}

void BattleSystem_SortMonSpeedOrder(BattleSystem *battleSys, BattleContext *battleCtx)
{
    int maxBattlers = BattleSystem_MaxBattlers(battleSys);

    for (int i = 0; i < maxBattlers; i++) {
        battleCtx->monSpeedOrder[i] = i;
    }

    for (int j = 0; j < maxBattlers - 1; j++) {
        for (int k = j + 1; k < maxBattlers; k++) {
            int mon1 = battleCtx->monSpeedOrder[j];
            int mon2 = battleCtx->monSpeedOrder[k];

            if (BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, mon1, mon2, TRUE)) {
                battleCtx->monSpeedOrder[j] = mon2;
                battleCtx->monSpeedOrder[k] = mon1;
            }
        }
    }
}

static const u16 sMovesAffectedByGravity[] = {
    MOVE_FLY,
    MOVE_BOUNCE,
    MOVE_JUMP_KICK,
    MOVE_HI_JUMP_KICK,
    MOVE_SPLASH,
    MOVE_MAGNET_RISE,
};

BOOL Move_FailsInHighGravity(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int move)
{
    BOOL result = FALSE;

    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY) {
        for (int i = 0; i < NELEMS(sMovesAffectedByGravity); i++) {
            if (sMovesAffectedByGravity[i] == move) {
                result = TRUE;
                break;
            }
        }
    }

    return result;
}

static const u16 sMovesAffectedByHealBlock[] = {
    MOVE_RECOVER,
    MOVE_SOFTBOILED,
    MOVE_REST,
    MOVE_MILK_DRINK,
    MOVE_MORNING_SUN,
    MOVE_SYNTHESIS,
    MOVE_MOONLIGHT,
    MOVE_SWALLOW,
    MOVE_HEAL_ORDER,
    MOVE_SLACK_OFF,
    MOVE_ROOST,
    MOVE_LUNAR_DANCE,
    MOVE_HEALING_WISH,
    MOVE_WISH,
};

BOOL Move_HealBlocked(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int move)
{
    BOOL result = FALSE;

    if (battleCtx->battleMons[battler].moveEffectsData.healBlockTurns) {
        for (int i = 0; i < NELEMS(sMovesAffectedByHealBlock); i++) {
            if (sMovesAffectedByHealBlock[i] == move) {
                result = TRUE;
                break;
            }
        }
    }

    return result;
}

void BattleSystem_UpdateLastResort(BattleSystem *battleSys, BattleContext *battleCtx)
{
    if (battleCtx->moveTemp == MOVE_LAST_RESORT
            || ATTACKING_MON.moveEffectsData.lastResortCount == 4) {
        return;
    }

    int i;
    for (i = 0; i < ATTACKING_MON.moveEffectsData.lastResortCount; i++) {
        if (ATTACKING_MON.moveEffectsData.lastResortMoves[i] == battleCtx->moveTemp) {
            return;
        }
    }

    ATTACKING_MON.moveEffectsData.lastResortMoves[i] = battleCtx->moveTemp;
    ATTACKING_MON.moveEffectsData.lastResortCount++;
}

int Battler_CountMoves(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int i;
    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (battleCtx->battleMons[battler].moves[i] == MOVE_NONE) {
            break;
        }
    }

    return i;
}

int BattleSystem_TriggerImmunityAbility(BattleContext *battleCtx, int attacker, int defender)
{
    int subscript = NULL, moveType;

    if (Battler_Ability(battleCtx, attacker) == ABILITY_NORMALIZE) {
        moveType = TYPE_NORMAL;
    } else if (battleCtx->moveType) {
        moveType = battleCtx->moveType;
    } else {
        moveType = CURRENT_MOVE_DATA.type;
    }

    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_VOLT_ABSORB) == TRUE
            && moveType == TYPE_ELECTRIC
            && attacker != defender
            && CURRENT_MOVE_DATA.range != RANGE_USER_SIDE
            && CURRENT_MOVE_DATA.range != RANGE_OPPONENT_SIDE
            && CURRENT_MOVE_DATA.range != RANGE_FIELD) {
        battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 4);
        subscript = subscript_ability_restores_hp;
    }

    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_WATER_ABSORB) == TRUE
            && moveType == TYPE_WATER
            && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE // do not proc on first turn of Dive
            && attacker != defender
            && CURRENT_MOVE_DATA.range != RANGE_USER_SIDE
            && CURRENT_MOVE_DATA.range != RANGE_OPPONENT_SIDE
            && CURRENT_MOVE_DATA.range != RANGE_FIELD) {
        battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 4);
        subscript = subscript_ability_restores_hp;
    }

    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_FLASH_FIRE) == TRUE
            && moveType == TYPE_FIRE
            && (battleCtx->battleMons[defender].status & MON_CONDITION_FREEZE) == FALSE
            && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
            && CURRENT_MOVE_DATA.range != RANGE_USER_SIDE
            && CURRENT_MOVE_DATA.range != RANGE_OPPONENT_SIDE
            && CURRENT_MOVE_DATA.range != RANGE_FIELD) {
        subscript = subscript_absorb_and_boost_fire_type_moves;
    }

    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_SOUNDPROOF) == TRUE) {
        for (int i = 0; i < NELEMS(sSoundMoves); i++) {
            if (sSoundMoves[i] == battleCtx->moveCur) {
                subscript = subscript_blocked_by_soundproof;
                break;
            }
        }
    }
	
	if (Battler_HeldItemEffect(battleCtx, battleCtx->defender) == HOLD_EFFECT_NO_WEATHER_CHIP_POWDER)
	{
        for (int i = 0; i < NELEMS(sPowderMoves); i++)
		{
			if (sPowderMoves[i] == battleCtx->moveCur)
			{
				subscript = subscript_blocked_by_goggles;
				break;
			}
		}
	}
	
	if (MON_HAS_TYPE(battleCtx->defender, TYPE_GRASS))
	{
        for (int i = 0; i < NELEMS(sPowderMoves); i++)
		{
			if (sPowderMoves[i] == battleCtx->moveCur)
			{
				subscript = subscript_grass_powder_immune;
				break;
			}
		}
	}

    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_MOTOR_DRIVE) == TRUE
            && moveType == TYPE_ELECTRIC
            && attacker != defender) {
        subscript = subscript_absorb_and_speed_up_1_stage;
    }
	
	if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_LIGHTNING_ROD) == TRUE
            && moveType == TYPE_ELECTRIC
            && attacker != defender) {
        subscript = subscript_lightning_rod_boost;
    }
	
	if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_STORM_DRAIN) == TRUE
            && moveType == TYPE_WATER
            && attacker != defender) {
        subscript = subscript_lightning_rod_boost;
    }

    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_DRY_SKIN) == TRUE
            && moveType == TYPE_WATER
            && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
            && CURRENT_MOVE_DATA.power) {
        battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[defender].maxHP, 4);
        subscript = subscript_ability_restores_hp;
    }

    return subscript;
}

BOOL BattleSystem_TriggerTurnEndAbility(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result = FALSE;
    int subscript;

    switch (Battler_Ability(battleCtx, battler)) {
    case ABILITY_SPEED_BOOST:
        if (battleCtx->battleMons[battler].curHP
                && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SPEED] < 12
                && battleCtx->battleMons[battler].moveEffectsData.fakeOutTurnNumber != battleCtx->totalTurns + 1) {
            battleCtx->sideEffectParam = MOVE_SUBSCRIPT_PTR_SPEED_UP_1_STAGE;
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
            battleCtx->sideEffectMon = battler;
            subscript = subscript_update_stat_stage;
            result = TRUE;
        }
        break;

    case ABILITY_SHED_SKIN:
        if ((battleCtx->battleMons[battler].status & MON_CONDITION_ANY)
                && battleCtx->battleMons[battler].curHP
                && BattleSystem_RandNext(battleSys) % 10 < 4) {
            if (battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) {
                battleCtx->msgTemp = MSGCOND_SLEEP;
            } else if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY_POISON) {
                battleCtx->msgTemp = MSGCOND_POISON;
            } else if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN) {
                battleCtx->msgTemp = MSGCOND_BURN;
            } else if (battleCtx->battleMons[battler].status & MON_CONDITION_PARALYSIS) {
                battleCtx->msgTemp = MSGCOND_PARALYSIS;
            } else {
                battleCtx->msgTemp = MSGCOND_FREEZE;
            }

            battleCtx->msgBattlerTemp = battler;
            subscript = subscript_ability_restore_status;
            result = TRUE;
        }
        break;
    }

    if (result == TRUE) {
        LOAD_SUBSEQ(subscript);
        battleCtx->commandNext = battleCtx->command;
        battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;
    }

    return result;
}

int BattleSystem_Divide(int dividend, int divisor)
{
    if (dividend == 0) {
        return dividend;
    }

    int signedFloor;
    if (dividend < 0) {
        signedFloor = -1;
    } else {
        signedFloor = 1;
    }

    dividend /= divisor;

    if (dividend == 0) {
        dividend = signedFloor;
    }

    return dividend;
}

enum {
    SWITCH_IN_CHECK_STATE_START = 0,

    SWITCH_IN_CHECK_STATE_FIELD_WEATHER = SWITCH_IN_CHECK_STATE_START,
    SWITCH_IN_CHECK_STATE_TRACE,
	SWITCH_IN_CHECK_STATE_RANDOM_ABILITY,
    SWITCH_IN_CHECK_STATE_WEATHER_ABILITIES,
	SWITCH_IN_CHECK_STATE_FORECAST,
    SWITCH_IN_CHECK_STATE_INTIMIDATE,
    SWITCH_IN_CHECK_STATE_DOWNLOAD,
    SWITCH_IN_CHECK_STATE_ANTICIPATION,
    SWITCH_IN_CHECK_STATE_FOREWARN,
    SWITCH_IN_CHECK_STATE_FRISK,
    SWITCH_IN_CHECK_STATE_SLOW_START,
    SWITCH_IN_CHECK_STATE_MOLD_BREAKER,
    SWITCH_IN_CHECK_STATE_PRESSURE,
    SWITCH_IN_CHECK_STATE_FORM_CHANGE,
    SWITCH_IN_CHECK_STATE_AMULET_COIN,
    SWITCH_IN_CHECK_STATE_FORBIDDEN_STATUS,
    SWITCH_IN_CHECK_STATE_HELD_ITEM_STATUS,
	SWITCH_IN_CHECK_STATE_AIR_BALLOON,
	SWITCH_IN_CHECK_STATE_IMPOSTER,

    SWITCH_IN_CHECK_STATE_DONE,
};

enum {
    SWITCH_IN_CHECK_RESULT_CONTINUE = 0,
    SWITCH_IN_CHECK_RESULT_BREAK,
    SWITCH_IN_CHECK_RESULT_DONE,
};

int BattleSystem_TriggerEffectOnSwitch(BattleSystem *battleSys, BattleContext *battleCtx)
{
    // must declare C89-style to match
    int i;
    int subscript;
    int result;
    int battler, battlero;
    int maxBattlers;
	int imposter1Pos, imposter2Pos;
	int abilityMax = ABILITY_ROCK_SOLID;
	int abilityChosen;
	int randomAbilityActivated = FALSE;

    maxBattlers = BattleSystem_MaxBattlers(battleSys);
    subscript = NULL;
    result = SWITCH_IN_CHECK_RESULT_CONTINUE;

    do {
        switch (battleCtx->switchInCheckState) {
        case SWITCH_IN_CHECK_STATE_FIELD_WEATHER:
            if (battleCtx->fieldWeatherChecked == FALSE) {
                switch (BattleSystem_FieldWeather(battleSys)) {
                case OVERWORLD_WEATHER_RAINING:
                case OVERWORLD_WEATHER_HEAVY_RAIN:
                case OVERWORLD_WEATHER_THUNDERSTORM:
                    subscript = subscript_overworld_rain;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;

                case OVERWORLD_WEATHER_SNOWING:
                case OVERWORLD_WEATHER_HEAVY_SNOW:
                case OVERWORLD_WEATHER_BLIZZARD:
                    subscript = subscript_overworld_hail;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;

                case OVERWORLD_WEATHER_SANDSTORM:
                    subscript = subscript_overworld_sand;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;

                case OVERWORLD_WEATHER_FOG:
                case OVERWORLD_WEATHER_DEEP_FOG:
                    subscript = subscript_overworld_fog;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;

                case OVERWORLD_WEATHER_HARSH_SUN:
                    subscript = subscript_overworld_sun;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;

                case OVERWORLD_WEATHER_TRICK_ROOM:
                    subscript = subscript_overworld_trick_room;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;

                default:
                    break;
                }

                if (result == SWITCH_IN_CHECK_RESULT_BREAK) {
                    battleCtx->fieldWeatherChecked = TRUE;
                }
            }

            battleCtx->switchInCheckState++;
            break;

        case SWITCH_IN_CHECK_STATE_TRACE:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];
                int defender1 = BattleSystem_EnemyInSlot(battleSys, battler, ENEMY_IN_SLOT_RIGHT);
                int defender2 = BattleSystem_EnemyInSlot(battleSys, battler, ENEMY_IN_SLOT_LEFT);

                battleCtx->msgDefender = ChooseTraceTarget(battleSys, battleCtx, defender1, defender2);

                if (battleCtx->battleMons[battler].traceAnnounced == FALSE
                        && battleCtx->msgDefender != BATTLER_NONE
                        && battleCtx->battleMons[battler].curHP
                        && battleCtx->battleMons[battler].heldItem != ITEM_GRISEOUS_ORB
                        && battleCtx->battleMons[battleCtx->msgDefender].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_TRACE) {
                    battleCtx->battleMons[battler].traceAnnounced = TRUE;
                    battleCtx->msgBattlerTemp = battler;
                    subscript = subscript_trace;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;
			
		case SWITCH_IN_CHECK_STATE_RANDOM_ABILITY:
			for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];
				randomAbilityActivated = FALSE;

                if (battleCtx->battleMons[battler].randomAbilityAnnounced == FALSE
                && battleCtx->battleMons[battler].curHP
                && (Battler_Ability(battleCtx, battler) == ABILITY_RANDOM_SELECT || Battler_Ability(battleCtx, battler) == ABILITY_GENETIC_FREAK))
				{
					do
					{
						abilityChosen = BattleSystem_RandNext(battleSys) % abilityMax;
						
						if ((abilityChosen != ABILITY_NONE)
						&& (abilityChosen != ABILITY_RANDOM_SELECT)
						&& (abilityChosen != ABILITY_GENETIC_FREAK)
						&& (abilityChosen != ABILITY_TRACE)
						&& (abilityChosen != ABILITY_MULTITYPE)
						&& (abilityChosen != ABILITY_IMPOSTER)
						&& (abilityChosen != ABILITY_FLOWER_GIFT)
						&& (abilityChosen != ABILITY_WONDER_GUARD)
						&& (abilityChosen != ABILITY_KLUTZ)
						&& (abilityChosen != ABILITY_STALL)
						&& (abilityChosen != ABILITY_SHELL_ARMOR)
						&& (abilityChosen != ABILITY_SOLID_ROCK)
						&& (abilityChosen != ABILITY_FORECAST))
						{
							randomAbilityActivated = TRUE;
						}
					} while (randomAbilityActivated == FALSE);
					
					battleCtx->battleMons[battler].randomAbilityAnnounced = TRUE;
                    battleCtx->msgBattlerTemp = battler;
					battleCtx->calcTemp = abilityChosen;
					subscript = subscript_random_ability;
					result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;
                }
            }
			
			if (i == maxBattlers)
			{
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_WEATHER_ABILITIES:
			int baseCastform = 0;
			int castformChecking, side;
			
			for (int j = 0; j < BattleSystem_MaxBattlers(battleSys); j++)
			{
				castformChecking = battleCtx->monSpeedOrder[j];

				if (battleCtx->battleMons[castformChecking].curHP
				&& battleCtx->battleMons[castformChecking].species == SPECIES_CASTFORM
				&& Battler_Ability(battleCtx, castformChecking) == ABILITY_FORECAST
				&& battleCtx->battleMons[castformChecking].formNum == 0)
				{
					baseCastform = 1;
					side = Battler_Side(battleSys, castformChecking);
				}
			}
				
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];
				
                if (battleCtx->battleMons[battler].weatherAbilityAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP) {
                    switch (Battler_Ability(battleCtx, battler)) {
                    case ABILITY_DRIZZLE:
						battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
						if (baseCastform == 0)
						{
							if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) == FALSE) {
								subscript = subscript_drizzle;
								result = SWITCH_IN_CHECK_RESULT_BREAK;
							}
						}
						else
						{
							if (side != Battler_Side(battleSys, battler))
							{
								subscript = subscript_forecast_ignores;
								result = SWITCH_IN_CHECK_RESULT_BREAK;
							}
							else
							{
								if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) == FALSE)
								{
									subscript = subscript_drizzle;
									result = SWITCH_IN_CHECK_RESULT_BREAK;
								}
							}
						}
                        break;

                    case ABILITY_SAND_STREAM:
						battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
						if (baseCastform == 0)
						{
							if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_SANDSTORM) == FALSE) {
								subscript = subscript_sand_stream;
								result = SWITCH_IN_CHECK_RESULT_BREAK;
							}
						}
						else
						{
							if (side != Battler_Side(battleSys, battler))
							{
								subscript = subscript_forecast_ignores;
								result = SWITCH_IN_CHECK_RESULT_BREAK;
							}
							else
							{
								if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_SANDSTORM) == FALSE)
								{
									subscript = subscript_sand_stream;
									result = SWITCH_IN_CHECK_RESULT_BREAK;
								}
							}
						}
                        break;

                    case ABILITY_DROUGHT:
						battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
						if (baseCastform == 0)
						{
							if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) == FALSE) {
								subscript = subscript_drought;
								result = SWITCH_IN_CHECK_RESULT_BREAK;
							}
						}
						else
						{
							if (side != Battler_Side(battleSys, battler))
							{
								subscript = subscript_forecast_ignores;
								result = SWITCH_IN_CHECK_RESULT_BREAK;
							}
							else
							{
								if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) == FALSE)
								{
									subscript = subscript_drought;
									result = SWITCH_IN_CHECK_RESULT_BREAK;
								}
							}
						}
                        break;

                    case ABILITY_SNOW_WARNING:
						battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
						if (baseCastform == 0)
						{
							if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING) == FALSE) {
								subscript = subscript_snow_warning;
								result = SWITCH_IN_CHECK_RESULT_BREAK;
							}
						}
						else
						{
							if (side != Battler_Side(battleSys, battler))
							{
								subscript = subscript_forecast_ignores;
								result = SWITCH_IN_CHECK_RESULT_BREAK;
							}
							else
							{
								if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING) == FALSE)
								{
									subscript = subscript_snow_warning;
									result = SWITCH_IN_CHECK_RESULT_BREAK;
								}
							}
						}
                        break;
						
					 case ABILITY_ROCHE_RADIUS:
						battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
						
						if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY) == FALSE)
						{
							battleCtx->sideEffectMon = battler;
							subscript = subscript_gravity_ability_start;
							result = SWITCH_IN_CHECK_RESULT_BREAK;
						}
						break;
                    }
                }
                if (result == SWITCH_IN_CHECK_RESULT_BREAK) {
                    battleCtx->msgBattlerTemp = battler;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;
			
		case SWITCH_IN_CHECK_STATE_FORECAST:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];
				
                if (battleCtx->battleMons[battler].weatherAbilityAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP) {
                    switch (Battler_Ability(battleCtx, battler)) {
					case ABILITY_FORECAST:
					
						switch (battleCtx->battleMons[battler].heldItem)
						{
							case ITEM_DAMP_ROCK:
								battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
							
								if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) == FALSE)
								{
									subscript = subscript_drizzle;
									result = SWITCH_IN_CHECK_RESULT_BREAK;
								}
							break;
							
							case ITEM_HEAT_ROCK:
								battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
							
								if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) == FALSE)
								{
									subscript = subscript_drought;
									result = SWITCH_IN_CHECK_RESULT_BREAK;
								}
							break;
							
							case ITEM_ICY_ROCK:
								battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
							
								if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING) == FALSE)
								{
									subscript = subscript_snow_warning;
									result = SWITCH_IN_CHECK_RESULT_BREAK;
								}
							break;
							
							case ITEM_SMOOTH_ROCK:
								battleCtx->battleMons[battler].weatherAbilityAnnounced = TRUE;
							
								if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_SANDSTORM) == FALSE)
								{
									subscript = subscript_sand_stream;
									result = SWITCH_IN_CHECK_RESULT_BREAK;
								}
							break;
						}
						break;
                    }
                }
                if (result == SWITCH_IN_CHECK_RESULT_BREAK) {
                    battleCtx->msgBattlerTemp = battler;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_INTIMIDATE:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (battleCtx->battleMons[battler].intimidateAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_INTIMIDATE) {
                    battleCtx->battleMons[battler].intimidateAnnounced = TRUE;
                    battleCtx->msgBattlerTemp = battler;
                    subscript = subscript_intimidate;
                    result = TRUE;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_DOWNLOAD:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (battleCtx->battleMons[battler].downloadAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_DOWNLOAD) {
                    int j; // must declare here to match
                    int sumDef = 0, sumSpDef = 0;

                    for (j = 0; j < maxBattlers; j++) {
                        if (Battler_Side(battleSys, battler) != Battler_Side(battleSys, j)
                                && (battleCtx->battleMons[j].statusVolatile & VOLATILE_CONDITION_SUBSTITUTE) == FALSE
                                && battleCtx->battleMons[j].curHP) {
                            sumDef += battleCtx->battleMons[j].defense
                                    * sStatStageBoosts[battleCtx->battleMons[j].statBoosts[BATTLE_STAT_DEFENSE]].numerator
                                    / sStatStageBoosts[battleCtx->battleMons[j].statBoosts[BATTLE_STAT_DEFENSE]].denominator;
                            sumSpDef += battleCtx->battleMons[j].spDefense
                                    * sStatStageBoosts[battleCtx->battleMons[j].statBoosts[BATTLE_STAT_SP_DEFENSE]].numerator
                                    / sStatStageBoosts[battleCtx->battleMons[j].statBoosts[BATTLE_STAT_SP_DEFENSE]].denominator;
                        }
                    }

                    battleCtx->battleMons[battler].downloadAnnounced = TRUE;

                    if (sumDef + sumSpDef != 0) {
                        if (sumDef >= sumSpDef) {
                            battleCtx->sideEffectParam = MOVE_SUBSCRIPT_PTR_SP_ATTACK_UP_1_STAGE;
                        } else {
                            battleCtx->sideEffectParam = MOVE_SUBSCRIPT_PTR_ATTACK_UP_1_STAGE;
                        }

                        battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
                        battleCtx->sideEffectMon = battler;
                        subscript = subscript_update_stat_stage;
                        result = SWITCH_IN_CHECK_RESULT_BREAK;
                        break;
                    }
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_ANTICIPATION:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (battleCtx->battleMons[battler].anticipationAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_ANTICIPATION) {
                    int j, k;
                    u16 move;
                    u32 effectiveness;

                    battleCtx->battleMons[battler].anticipationAnnounced = TRUE;

                    for (j = 0; j < maxBattlers; j++) {
                        if (Battler_Side(battleSys, battler) != Battler_Side(battleSys, j) && battleCtx->battleMons[j].curHP) {
                            for (k = 0; k < 4; k++) {
                                move = battleCtx->battleMons[j].moves[k];

                                if (move) {
                                    effectiveness = 0;
                                    battleCtx->damage = BattleSystem_ApplyTypeChart(battleSys, battleCtx, move, NULL, j, battler, battleCtx->damage, &effectiveness);

                                    if ((effectiveness & MOVE_STATUS_INEFFECTIVE) == FALSE
                                            && MoveCannotTriggerAnticipation(battleCtx, move) == FALSE
                                            && ((effectiveness & MOVE_STATUS_SUPER_EFFECTIVE) 
                                                || (MOVE_DATA(move).effect == BATTLE_EFFECT_ONE_HIT_KO
                                                    && battleCtx->battleMons[battler].level <= battleCtx->battleMons[j].level))) {
                                        result = SWITCH_IN_CHECK_RESULT_BREAK;
                                        break;
                                    }
                                }
                            }

                            if (result == SWITCH_IN_CHECK_RESULT_BREAK) {
                                break;
                            }
                        }
                    }

                    if (result == SWITCH_IN_CHECK_RESULT_BREAK) {
                        battleCtx->msgBattlerTemp = battler;
                        subscript = subscript_anticipation;
                    }
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_FOREWARN:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (battleCtx->battleMons[battler].forewarnAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_FOREWARN) {
                    int j, k;
                    u16 move;
                    u32 movePower, maxPower;
                    int sumEnemyHP;

                    battleCtx->battleMons[battler].forewarnAnnounced = TRUE;
                    maxPower = 0;
                    sumEnemyHP = 0;

                    for (j = 0; j < maxBattlers; j++) {
                        if (Battler_Side(battleSys, battler) != Battler_Side(battleSys, j)
                                && battleCtx->battleMons[j].curHP) {
                            sumEnemyHP += battleCtx->battleMons[j].curHP;

                            for (k = 0; k < 4; k++) {
                                move = battleCtx->battleMons[j].moves[k];
                                movePower = MOVE_DATA(move).power;

                                if (movePower == 1) {
                                    switch (MOVE_DATA(move).effect) {
                                    case BATTLE_EFFECT_ONE_HIT_KO:
                                        if (maxPower < 150 || (maxPower == 150 && (BattleSystem_RandNext(battleSys) & 1))) {
                                            maxPower = 150;
                                            battleCtx->msgMoveTemp = move;
                                        }
                                        break;

                                    case BATTLE_EFFECT_COUNTER:
                                    case BATTLE_EFFECT_MIRROR_COAT:
                                    case BATTLE_EFFECT_METAL_BURST:
                                        if (maxPower < 120 || (maxPower == 120 && (BattleSystem_RandNext(battleSys) & 1))) {
                                            maxPower = 120;
                                            battleCtx->msgMoveTemp = move;
                                        }
                                        break;

                                    default:
                                        if (maxPower < 80 || (maxPower == 80 && (BattleSystem_RandNext(battleSys) & 1))) {
                                            maxPower = 80;
                                            battleCtx->msgMoveTemp = move;
                                        }
                                        break;
                                    }
                                } else if (maxPower < movePower || (maxPower == movePower && (BattleSystem_RandNext(battleSys) & 1))) {
                                    maxPower = movePower;
                                    battleCtx->msgMoveTemp = move;
                                }
                            }
                        }
                    }

                    if (maxPower) {
                        battleCtx->msgBattlerTemp = battler;

                        subscript = subscript_forewarn;
                        result = SWITCH_IN_CHECK_RESULT_BREAK;
                    } else if (sumEnemyHP) {
                        j = BattleSystem_RandomOpponent(battleSys, battleCtx, battler);
                        k = Battler_CountMoves(battleSys, battleCtx, j);

                        battleCtx->msgMoveTemp = battleCtx->battleMons[j].moves[BattleSystem_RandNext(battleSys) % k];
                        battleCtx->msgBattlerTemp = battler;

                        subscript = subscript_forewarn;
                        result = SWITCH_IN_CHECK_RESULT_BREAK;
                    }
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_FRISK:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (battleCtx->battleMons[battler].friskAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_FRISK) {
                    battleCtx->battleMons[battler].friskAnnounced = TRUE;

                    if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
                        int enemies[] = {
                            BattleSystem_EnemyInSlot(battleSys, battler, ENEMY_IN_SLOT_RIGHT),
                            BattleSystem_EnemyInSlot(battleSys, battler, ENEMY_IN_SLOT_LEFT),
                        };

                        if (battleCtx->battleMons[enemies[0]].curHP
                                && battleCtx->battleMons[enemies[0]].heldItem
                                && battleCtx->battleMons[enemies[1]].curHP
                                && battleCtx->battleMons[enemies[1]].heldItem) {
                            battleCtx->msgItemTemp = battleCtx->battleMons[enemies[BattleSystem_RandNext(battleSys) & 1]].heldItem;
                            result = SWITCH_IN_CHECK_RESULT_BREAK;
                        } else if (battleCtx->battleMons[enemies[0]].curHP
                                && battleCtx->battleMons[enemies[0]].heldItem) {
                            battleCtx->msgItemTemp = battleCtx->battleMons[enemies[0]].heldItem;
                            result = SWITCH_IN_CHECK_RESULT_BREAK;
                        } else if (battleCtx->battleMons[enemies[1]].curHP
                                && battleCtx->battleMons[enemies[1]].heldItem) {
                            battleCtx->msgItemTemp = battleCtx->battleMons[enemies[1]].heldItem;
                            result = SWITCH_IN_CHECK_RESULT_BREAK;
                        }
                    } else if (battleCtx->battleMons[battler ^ 1].curHP && battleCtx->battleMons[battler ^ 1].heldItem) {
                        battleCtx->msgItemTemp = battleCtx->battleMons[battler ^ 1].heldItem;
                        result = SWITCH_IN_CHECK_RESULT_BREAK;
                    }
                }

                if (result == SWITCH_IN_CHECK_RESULT_BREAK) {
                    battleCtx->msgBattlerTemp = battler;
                    subscript = subscript_frisk;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_SLOW_START:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (battleCtx->battleMons[battler].slowStartAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_SLOW_START
                        && battleCtx->totalTurns <= battleCtx->battleMons[battler].moveEffectsData.slowStartTurnNumber) {
                    battleCtx->battleMons[battler].slowStartAnnounced = TRUE;
                    battleCtx->msgBattlerTemp = battler;
                    subscript = subscript_slow_start;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;
                }

                if ((battleCtx->battleMons[battler].slowStartFinished == 0) && (battleCtx->battleMons[battler].curHP) && (Battler_Ability(battleCtx, battler) == 112) && ((battleCtx->totalTurns - battleCtx->battleMons[battler].moveEffectsData.slowStartTurnNumber) == 5)) {
                    battleCtx->battleMons[battler].slowStartFinished = 1;
                    battleCtx->msgBattlerTemp = battler;
                    subscript = subscript_slow_start_end;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;
    
        case SWITCH_IN_CHECK_STATE_MOLD_BREAKER:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (battleCtx->battleMons[battler].moldBreakerAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_MOLD_BREAKER) {
                    battleCtx->battleMons[battler].moldBreakerAnnounced = TRUE;
                    battleCtx->msgBattlerTemp = battler;
                    subscript = subscript_mold_breaker;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_PRESSURE:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (battleCtx->battleMons[battler].pressureAnnounced == FALSE
                        && battleCtx->battleMons[battler].curHP
                        && Battler_Ability(battleCtx, battler) == ABILITY_PRESSURE) {
                    battleCtx->battleMons[battler].pressureAnnounced = TRUE;
                    battleCtx->msgBattlerTemp = battler;
                    subscript = subscript_pressure;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_FORM_CHANGE:
            if (BattleSystem_TriggerFormChange(battleSys, battleCtx, &subscript) == TRUE) {
                result = SWITCH_IN_CHECK_RESULT_BREAK;
            } else {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_AMULET_COIN:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (BattleSystem_GetItemData(battleCtx, battleCtx->battleMons[battler].heldItem, ITEM_PARAM_HOLD_EFFECT) == HOLD_EFFECT_MONEY_UP) {
                    battleCtx->prizeMoneyMul = 2;
                }
            }

            battleCtx->switchInCheckState++;
            break;

        case SWITCH_IN_CHECK_STATE_FORBIDDEN_STATUS:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (BattleSystem_RecoverStatusByAbility(battleSys, battleCtx, battler, TRUE) == TRUE) {
                    subscript = subscript_ability_forbids_status;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_HELD_ITEM_STATUS:
            for (i = 0; i < maxBattlers; i++) {
                battler = battleCtx->monSpeedOrder[i];

                if (BattleSystem_TriggerHeldItemOnStatus(battleSys, battleCtx, battler, &subscript) == TRUE) {
                    battleCtx->msgBattlerTemp = battler;
                    result = SWITCH_IN_CHECK_RESULT_BREAK;
                    break;
                }
            }

            if (i == maxBattlers) {
                battleCtx->switchInCheckState++;
            }
            break;
			
		case SWITCH_IN_CHECK_STATE_AIR_BALLOON:
			for (i = 0; i < maxBattlers; i++)
			{
                battler = battleCtx->monSpeedOrder[i];
				
                if ((BattleSystem_GetItemData(battleCtx, battleCtx->battleMons[battler].heldItem, ITEM_PARAM_HOLD_EFFECT) == HOLD_EFFECT_LEVITATE_POPPED_IF_HIT)
				&& ((battleCtx->battleMons[battler].moveEffectsData.embargoTurns == 0))
				&& (battleCtx->battleMons[battler].airBalloonAnnounced == FALSE))
				{
					battleCtx->battleMons[battler].airBalloonAnnounced = TRUE;
					battleCtx->msgBattlerTemp = battler;
					subscript = subscript_air_balloon;
					result = SWITCH_IN_CHECK_RESULT_BREAK;
					break;
				}
            }
			
			if (i == maxBattlers)
			{
                battleCtx->switchInCheckState++;
            }
            break;
			
		case SWITCH_IN_CHECK_STATE_IMPOSTER:
			for (i = 0; i < maxBattlers; i++)
			{
                battler = battleCtx->monSpeedOrder[i];
				
                if ((Battler_Ability(battleCtx, battler) == ABILITY_IMPOSTER)
				&& (battleCtx->battleMons[battler].imposterFlag == FALSE))
				{
					imposter1Pos = BattleSystem_BattlerSlot(battleSys, battler);
					
					switch (BattleSystem_BattlerSlot(battleSys, battler))
					{
							case BATTLER_TYPE_SOLO_ENEMY:
								imposter2Pos = BATTLER_TYPE_SOLO_PLAYER;
								break;
								
							case BATTLER_TYPE_SOLO_PLAYER:
								imposter2Pos = BATTLER_TYPE_SOLO_ENEMY;
								break;
								
							case BATTLER_TYPE_ENEMY_SIDE_SLOT_1:
								imposter2Pos = BATTLER_TYPE_PLAYER_SIDE_SLOT_2;
								break;
								
							case BATTLER_TYPE_ENEMY_SIDE_SLOT_2:
								imposter2Pos = BATTLER_TYPE_PLAYER_SIDE_SLOT_1;
								break;
								
							case BATTLER_TYPE_PLAYER_SIDE_SLOT_1:
								imposter2Pos = BATTLER_TYPE_ENEMY_SIDE_SLOT_2;
								break;
								
							case BATTLER_TYPE_PLAYER_SIDE_SLOT_2:
								imposter2Pos = BATTLER_TYPE_ENEMY_SIDE_SLOT_1;
								break;
								
							default:
								break;
					}
						
					for (int o = 0; o < maxBattlers; o++)
					{
						battlero = battleCtx->monSpeedOrder[o];
						
						if ((battler != battlero)
						&& (Battler_Side(battleSys, battlero) != Battler_Side(battleSys, battler))
						&& (imposter2Pos == BattleSystem_BattlerSlot(battleSys, battlero)))
						{
							battleCtx->defender = battlero;
							break;
						}
					}
					
					battleCtx->battleMons[battler].imposterFlag = TRUE;
					battleCtx->attacker = battler;
					subscript = subscript_imposter_transform;
					result = SWITCH_IN_CHECK_RESULT_BREAK;
					break;
				}
            }
			
			if (i == maxBattlers)
			{
                battleCtx->switchInCheckState++;
            }
            break;

        case SWITCH_IN_CHECK_STATE_DONE:
            battleCtx->switchInCheckState = 0;
            result = SWITCH_IN_CHECK_RESULT_DONE;
            break;
        }
    } while (result == SWITCH_IN_CHECK_RESULT_CONTINUE);

    return subscript;
}

int BattleSystem_RandomOpponent(BattleSystem *battleSys, BattleContext *battleCtx, int attacker)
{
    int opponents[2];
    u32 battleType = BattleSystem_BattleType(battleSys);

    int chosen;
    if (battleType & BATTLE_TYPE_DOUBLES) {
        opponents[0] = BattleSystem_EnemyInSlot(battleSys, attacker, ENEMY_IN_SLOT_RIGHT);
        opponents[1] = BattleSystem_EnemyInSlot(battleSys, attacker, ENEMY_IN_SLOT_LEFT);
        
        int rnd = BattleSystem_RandNext(battleSys) & 1;
        chosen = opponents[rnd];

        if (battleCtx->battleMons[chosen].curHP == 0) {
            chosen = opponents[rnd ^ 1];
        }
    } else {
        chosen = attacker ^ 1;
    }

    return chosen;
}

BOOL BattleSystem_TriggerAbilityOnHit(BattleSystem *battleSys, BattleContext *battleCtx, int *subscript)
{
    BOOL result = FALSE;

    // These two sentinels must be separate to match
    if (battleCtx->defender == BATTLER_NONE) {
        return result;
    }

    if (Battler_SubstituteWasHit(battleCtx, battleCtx->defender) == TRUE) {
        return result;
    }

    switch (Battler_Ability(battleCtx, battleCtx->defender))
	{
    case ABILITY_STATIC:
        if (ATTACKING_MON.curHP
                && ATTACKING_MON.status == MON_CONDITION_NONE
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
                && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))
                && BattleSystem_RandNext(battleSys) % 10 < 3) {
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
            battleCtx->sideEffectMon = battleCtx->attacker;
            battleCtx->msgBattlerTemp = battleCtx->defender;

            *subscript = subscript_paralyze;
            result = TRUE;
        }
        break;

//    case ABILITY_COLOR_CHANGE:
//        u8 moveType;
//
//        if (Battler_Ability(battleCtx, battleCtx->attacker) == ABILITY_NORMALIZE) {
//            moveType = TYPE_NORMAL;
//        } else if (battleCtx->moveType) {
//            moveType = battleCtx->moveType;
//        } else {
//            moveType = CURRENT_MOVE_DATA.type;
//        }
//
//        if (DEFENDING_MON.curHP
//                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
//                && battleCtx->moveCur != MOVE_STRUGGLE
//                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
//                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
//                && CURRENT_MOVE_DATA.power
//                && BattleMon_Get(battleCtx, battleCtx->defender, 27, NULL) != moveType
//                && BattleMon_Get(battleCtx, battleCtx->defender, 28, NULL) != moveType) {
//            *subscript = subscript_color_change;
//            battleCtx->msgTemp = moveType;
//            result = TRUE;
//        }
//        break;

    case ABILITY_ROUGH_SKIN:
        if (ATTACKING_MON.curHP
                && Battler_Ability(battleCtx, battleCtx->attacker) != ABILITY_MAGIC_GUARD
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
                && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP * -1, 8);
            battleCtx->msgBattlerTemp = battleCtx->attacker;

            *subscript = subscript_rough_skin;
            result = TRUE;
        }
        break;

    case ABILITY_EFFECT_SPORE:
        if (ATTACKING_MON.curHP
                && ATTACKING_MON.status == MON_CONDITION_NONE
				&& (battleCtx->battleMons[battleCtx->attacker].type1 != TYPE_GRASS)
				&& (battleCtx->battleMons[battleCtx->attacker].type2 != TYPE_GRASS)
				&& (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_WEATHER_CHIP_POWDER)
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
                && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))
                && BattleSystem_RandNext(battleSys) % 10 < 3) {
            switch (BattleSystem_RandNext(battleSys) % 3) {
            case 0:
            default:
                *subscript = subscript_poison;
                break;
            case 1:
                *subscript = subscript_paralyze;
                break;
            case 2:
                *subscript = subscript_fall_asleep;
                break;
            }

            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
            battleCtx->sideEffectMon = battleCtx->attacker;
            battleCtx->msgBattlerTemp = battleCtx->defender;

            result = TRUE;
        }
        break;

    case ABILITY_POISON_POINT:
        if (ATTACKING_MON.curHP
                && ATTACKING_MON.status == MON_CONDITION_NONE
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
                && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))
                && BattleSystem_RandNext(battleSys) % 10 < 3) {
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
            battleCtx->sideEffectMon = battleCtx->attacker;
            battleCtx->msgBattlerTemp = battleCtx->defender;

            *subscript = subscript_poison;
            result = TRUE;
        }
        break;

    case ABILITY_FLAME_BODY:
        if (ATTACKING_MON.curHP
                && ATTACKING_MON.status == MON_CONDITION_NONE
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
                && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))
                && BattleSystem_RandNext(battleSys) % 10 < 3) {
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
            battleCtx->sideEffectMon = battleCtx->attacker;
            battleCtx->msgBattlerTemp = battleCtx->defender;

            *subscript = subscript_burn;
            result = TRUE;
        }
        break;
		
	case ABILITY_DEFIANT:
			if (DEFENDING_MON.curHP
				&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
				&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
				&& DEFENDING_MON.defiantFlag == TRUE) {
			*subscript = subscript_defiant_activate;
			result = TRUE;
			DEFENDING_MON.defiantFlag = FALSE;
		}
		break;
		
	case ABILITY_COMPETITIVE:
			if (DEFENDING_MON.curHP
				&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
				&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
				&& DEFENDING_MON.defiantFlag == TRUE) {
			*subscript = subscript_comp_activate;
			result = TRUE;
			DEFENDING_MON.defiantFlag = FALSE;
		}
		break;

    case ABILITY_CUTE_CHARM:
        if (ATTACKING_MON.curHP
                && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_ATTRACT) == FALSE
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
                && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))
                && DEFENDING_MON.curHP
                && BattleSystem_RandNext(battleSys) % 10 < 3) {
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
            battleCtx->sideEffectMon = battleCtx->attacker;
            battleCtx->msgBattlerTemp = battleCtx->defender;

            *subscript = subscript_infatuate;
            result = TRUE;
        }
        break;
		
	case ABILITY_FRESH_MILK:
        if (ATTACKING_MON.curHP
                && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_ATTRACT) == FALSE
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
                && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))
                && DEFENDING_MON.curHP
                && BattleSystem_RandNext(battleSys) % 10 < 3) {
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
            battleCtx->sideEffectMon = battleCtx->attacker;
            battleCtx->msgBattlerTemp = battleCtx->defender;

            *subscript = subscript_infatuate;
            result = TRUE;
        }
        break;

    case ABILITY_AFTERMATH:
        if (battleCtx->defender == battleCtx->faintedMon
                && Battler_Ability(battleCtx, battleCtx->attacker) != ABILITY_MAGIC_GUARD
                && BattleSystem_CountAbility(battleSys, battleCtx, 8, 0, ABILITY_DAMP) == 0
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && ATTACKING_MON.curHP
                && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP * -1, 4);
            battleCtx->msgBattlerTemp = battleCtx->attacker;

            *subscript = subscript_aftermath;
            result = TRUE;
        }
        break;
			
	case ABILITY_FREE_SAMPLE:
			if (DEFENDING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
			&& ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))
			&& BattleSystem_RandNext(battleSys) % 10 < 3)
			{
				*subscript = subscript_pluck_ability_alt;
				result = TRUE;
			}
			break;
			
	case ABILITY_COTTON_DOWN:
			if (ATTACKING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
			&& ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH)))
			{
				*subscript = subscript_cotton_down_activate;
				battleCtx->sideEffectMon = battleCtx->attacker;
				result = TRUE;
			}
			break;
			
	case ABILITY_STEADFAST:
			if (DEFENDING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
			&& ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH)))
			{
				*subscript = subscript_steadfast_activate;
				battleCtx->sideEffectMon = battleCtx->defender;
				result = TRUE;
			}
			break;
    }

    return result;
}

BOOL BattleSystem_RecoverStatusByAbility(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int skipLoad)
{
    BOOL result = FALSE;

    switch (Battler_Ability(battleCtx, battler)) {
    case ABILITY_IMMUNITY:
        if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY_POISON) {
            battleCtx->msgTemp = MSGCOND_POISON;
            result = TRUE;
        }
        break;

    case ABILITY_OWN_TEMPO:
        if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION) {
            battleCtx->msgTemp = MSGCOND_CONFUSION;
            result = TRUE;
        }
        break;

    case ABILITY_LIMBER:
        if (battleCtx->battleMons[battler].status & MON_CONDITION_PARALYSIS) {
            battleCtx->msgTemp = MSGCOND_PARALYSIS;
            result = TRUE;
        }
        break;

    case ABILITY_INSOMNIA:
    case ABILITY_VITAL_SPIRIT:
        if (battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) {
            battleCtx->msgTemp = MSGCOND_SLEEP;
            result = TRUE;
        }
        break;

    case ABILITY_WATER_VEIL:
        if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN) {
            battleCtx->msgTemp = MSGCOND_BURN;
            result = TRUE;
        }
        break;

    case ABILITY_MAGMA_ARMOR:
        if (battleCtx->battleMons[battler].status & MON_CONDITION_FREEZE) {
            battleCtx->msgTemp = MSGCOND_FREEZE;
            result = TRUE;
        }
        break;

    case ABILITY_OBLIVIOUS:
        if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_ATTRACT) {
            battleCtx->msgTemp = MSGCOND_INFATUATION;
            result = TRUE;
        }
        break;

    case ABILITY_UNBURDEN:
        if (battleCtx->battleMons[battler].heldItem) {
            battleCtx->battleMons[battler].moveEffectsData.canUnburden = TRUE;
        }
        break;
    }

    if (result == TRUE) {
        battleCtx->msgBattlerTemp = battler;
        battleCtx->msgAbilityTemp = Battler_Ability(battleCtx, battler);

        if (skipLoad == FALSE) {
            LOAD_SUBSEQ(subscript_ability_forbids_status);
            battleCtx->commandNext = battleCtx->command;
            battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;
        }
    }

    return result;
}

BOOL Ability_ForbidsStatus(BattleContext *battleSys, int ability, int status)
{
    BOOL result = FALSE;

    switch (ability) {
    case ABILITY_IMMUNITY:
        if (status & MON_CONDITION_ANY_POISON) {
            result = TRUE;
        }
        break;

    case ABILITY_LIMBER:
        if (status & MON_CONDITION_PARALYSIS) {
            result = TRUE;
        }
        break;

    case ABILITY_INSOMNIA:
    case ABILITY_VITAL_SPIRIT:
        if (status & MON_CONDITION_SLEEP) {
            result = TRUE;
        }
        break;

    case ABILITY_WATER_VEIL:
        if (status & MON_CONDITION_BURN) {
            result = TRUE;
        }
        break;
        
    case ABILITY_MAGMA_ARMOR:
        if (status & MON_CONDITION_FREEZE) {
            result = TRUE;
        }
        break;
    }

    return result;
}

BOOL BattleSystem_SynchronizeStatus(BattleSystem *battleSys, BattleContext *battleCtx, int controllerCommand)
{
    BOOL result = FALSE;
    int nextSeq = 0;

    if (battleCtx->defender != BATTLER_NONE
            && Battler_Ability(battleCtx, battleCtx->defender) == ABILITY_SYNCHRONIZE
            && battleCtx->defender == battleCtx->sideEffectMon
            && (battleCtx->battleStatusMask & SYSCTL_TRY_SYNCHRONIZE_STATUS)) {
        battleCtx->msgBattlerTemp = battleCtx->defender;
        battleCtx->sideEffectMon = battleCtx->attacker;
        result = TRUE;
    } else if (Battler_Ability(battleCtx, battleCtx->attacker) == ABILITY_SYNCHRONIZE
            && battleCtx->attacker == battleCtx->sideEffectMon
            && (battleCtx->battleStatusMask & SYSCTL_TRY_SYNCHRONIZE_STATUS)) {
        // This will try to synchronize a status applied from an ability or item held
        // by the defender, e.g. Static.
        battleCtx->msgBattlerTemp = battleCtx->attacker;
        battleCtx->sideEffectMon = battleCtx->defender;
        result = TRUE;
    }

    if (result == TRUE) {
        if (battleCtx->battleMons[battleCtx->msgBattlerTemp].status & MON_CONDITION_POISON) {
            nextSeq = subscript_poison;
        } else if (battleCtx->battleMons[battleCtx->msgBattlerTemp].status & MON_CONDITION_BURN) {
            nextSeq = subscript_burn;
        } else if (battleCtx->battleMons[battleCtx->msgBattlerTemp].status & MON_CONDITION_PARALYSIS) {
            nextSeq = subscript_paralyze;
        } else if (battleCtx->battleMons[battleCtx->msgBattlerTemp].status & MON_CONDITION_SLEEP) {
            nextSeq = subscript_fall_asleep;
        } else if (battleCtx->battleMons[battleCtx->msgBattlerTemp].status & MON_CONDITION_FREEZE) {
            nextSeq = subscript_freeze;
        } else if (battleCtx->battleMons[battleCtx->msgBattlerTemp].status & MON_CONDITION_TOXIC) {
            nextSeq = subscript_badly_poison;
        }


        if (nextSeq) {
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;

            LOAD_SUBSEQ(nextSeq);
            battleCtx->commandNext = controllerCommand;
            battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;

            return result;
        }
    }

    // Check for Freeze application to Shaymin-Sky (which would revert to Shaymin-Land).
    result = BattleSystem_TriggerFormChange(battleSys, battleCtx, &nextSeq);
    if (result == TRUE) {
        LOAD_SUBSEQ(nextSeq);
        battleCtx->commandNext = controllerCommand;
        battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;

        return result;
    }

    if (battleCtx->defender != BATTLER_NONE
            && Battler_HeldItemEffect(battleCtx, battleCtx->defender) == HOLD_EFFECT_RECIPROCATE_INFAT
            && battleCtx->defender == battleCtx->sideEffectMon
            && (DEFENDER_SELF_TURN_FLAGS.statusFlags & SELF_TURN_FLAG_INFATUATED)) {
        battleCtx->msgBattlerTemp = battleCtx->defender;
        battleCtx->sideEffectMon = battleCtx->attacker;
        result = TRUE;
    } else if (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) == HOLD_EFFECT_RECIPROCATE_INFAT
            && battleCtx->attacker == battleCtx->sideEffectMon
            && (ATTACKER_SELF_TURN_FLAGS.statusFlags & SELF_TURN_FLAG_INFATUATED)) {
        // This should only happen if the defender triggered Cute Charm.
        battleCtx->msgBattlerTemp = battleCtx->attacker;
        battleCtx->sideEffectMon = battleCtx->defender;
        result = TRUE;
    }

    if (result == TRUE) {
        nextSeq = subscript_infatuate;
        battleCtx->sideEffectType = SIDE_EFFECT_TYPE_HELD_ITEM;

        LOAD_SUBSEQ(nextSeq);
        battleCtx->commandNext = controllerCommand;
        battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;

        return result; // This line has to stay to match.
    }

    return FALSE;
}

BOOL BattleSystem_TriggerHeldItem(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result = FALSE;
    int subscript;
    int itemEffect = Battler_HeldItemEffect(battleCtx, battler);
    int itemPower = Battler_HeldItemPower(battleCtx, battler, ITEM_POWER_CHECK_ALL);

    if (battleCtx->battleMons[battler].curHP) {
        switch (itemEffect) {
        case HOLD_EFFECT_HP_RESTORE:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = itemPower;
                subscript = subscript_held_item_hp_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_PCT_RESTORE:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP * itemPower, 100);
                subscript = subscript_held_item_hp_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PRZ_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_PARALYSIS) {
                subscript = subscript_held_item_prz_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_SLP_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) {
                subscript = subscript_held_item_slp_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PSN_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY_POISON) {
                subscript = subscript_held_item_psn_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_BRN_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN) {
                subscript = subscript_held_item_brn_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_FRZ_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_FREEZE) {
                subscript = subscript_held_item_frz_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PP_RESTORE: {
            int i;
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (battleCtx->battleMons[battler].moves[i] && battleCtx->battleMons[battler].ppCur[i] == 0) {
                    break;
                }
            }

            if (i != LEARNED_MOVES_MAX) {
                BattleMon_AddVal(&battleCtx->battleMons[battler], BATTLEMON_CUR_PP_1 + i, itemPower);
                BattleMon_CopyToParty(battleSys, battleCtx, battler);
                battleCtx->msgMoveTemp = battleCtx->battleMons[battler].moves[i];
                subscript = subscript_held_item_pp_restore;
                result = TRUE;
            }

            break;
        }

        case HOLD_EFFECT_CONFUSE_RESTORE:
            if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION) {
                subscript = subscript_held_item_cnf_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_STATUS_RESTORE:
            if ((battleCtx->battleMons[battler].status & MON_CONDITION_ANY)
                    || (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION)) {
                if (battleCtx->battleMons[battler].status & MON_CONDITION_PARALYSIS) {
                    subscript = subscript_held_item_prz_restore;
                }

                if (battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) {
                    subscript = subscript_held_item_slp_restore;
                }

                if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY_POISON) {
                    subscript = subscript_held_item_psn_restore;
                }

                if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN) {
                    subscript = subscript_held_item_brn_restore;
                }

                if (battleCtx->battleMons[battler].status & MON_CONDITION_FREEZE) {
                    subscript = subscript_held_item_frz_restore;
                }

                if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION) {
                    subscript = subscript_held_item_cnf_restore;
                }

                if ((battleCtx->battleMons[battler].status & MON_CONDITION_ANY)
                        && (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION)) {
                    subscript = subscript_held_item_multi_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_SPICY:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_SPICY;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_SPICY) == -1) {
                    subscript = subscript_held_item_dislike_flavor;
                } else {
                    subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_DRY:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_DRY;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_DRY) == -1) {
                    subscript = subscript_held_item_dislike_flavor;
                } else {
                    subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_SWEET:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_SWEET;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_SWEET) == -1) {
                    subscript = subscript_held_item_dislike_flavor;
                } else {
                    subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_BITTER:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_BITTER;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_BITTER) == -1) {
                    subscript = subscript_held_item_dislike_flavor;
                } else {
                    subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_SOUR:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_SOUR;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_SOUR) == -1) {
                    subscript = subscript_held_item_dislike_flavor;
                } else {
                    subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;
        
        case HOLD_EFFECT_PINCH_ATK_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_ATTACK;
                subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_DEF_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_DEFENSE] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_DEFENSE;
                subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_SPEED_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SPEED] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_SPEED;
                subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_SPATK_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_ATTACK] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_SP_ATTACK;
                subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_SPDEF_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_DEFENSE] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_SP_DEFENSE;
                subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_CRITRATE_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_FOCUS_ENERGY) == FALSE) {
                subscript = subscript_held_item_raise_crit;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_RANDOM_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= (battleCtx->battleMons[battler].maxHP / itemPower)) {
                int i;
                for (i = 0; i < 5; i++) {
                    if (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK + i] < 12) {
                        break;
                    }
                }

                if (i != 5) {
                    do {
                        i = BattleSystem_RandNext(battleSys) % 5;
                    } while (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK + i] == 12);

                    battleCtx->msgTemp = BATTLE_STAT_ATTACK + i;
                    subscript = subscript_held_item_sharply_raise_stat;
                    result = TRUE;
                }
            }
            break;

        case HOLD_EFFECT_STATDOWN_RESTORE: {
            for (int i = BATTLE_STAT_HP; i < BATTLE_STAT_MAX; i++) {
                if (battleCtx->battleMons[battler].statBoosts[i] < 6) {
                    battleCtx->battleMons[battler].statBoosts[i] = 6;
                    result = TRUE;
                }
            }

            if (result == TRUE) {
                subscript = subscript_held_item_statdown_restore;
            }
            break;
        }
            
        case HOLD_EFFECT_HEAL_INFATUATION:
            if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_ATTRACT) {
                battleCtx->msgTemp = MSGCOND_INFATUATION;
                subscript = subscript_held_item_heal_infatuation;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_ACC_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower) {
                subscript = subscript_held_item_temp_acc_up;
                result = TRUE;
            }
            break;
        }
		
        if (result == TRUE) {
            battleCtx->msgBattlerTemp = battler;
            battleCtx->msgItemTemp = Battler_HeldItem(battleCtx, battler);

            LOAD_SUBSEQ(subscript);
            battleCtx->commandNext = battleCtx->command;
            battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;
        }
    }

    return result;
}

BOOL BattleSystem_TriggerLeftovers(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result = 0;
    int subscript;
    int itemEffect = Battler_HeldItemEffect(battleCtx, battler);
    int itemPower = Battler_HeldItemPower(battleCtx, battler, ITEM_POWER_CHECK_ALL);

    if (battleCtx->battleMons[battler].curHP) {
        switch (itemEffect) {
        case HOLD_EFFECT_HP_RESTORE_GRADUAL:
            if ((battleCtx->battleMons[battler].curHP < (battleCtx->battleMons[battler].maxHP))
			|| (battleCtx->battleMons[battler].moveEffectsData.healInversionTurns > 0)) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
                subscript = subscript_restore_a_little_hp;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_PSN_TYPE:
            if (MON_HAS_TYPE(battler, TYPE_POISON)) {
                if ((battleCtx->battleMons[battler].curHP < (battleCtx->battleMons[battler].maxHP))
				|| (battleCtx->battleMons[battler].moveEffectsData.healInversionTurns > 0)) {
                    battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, 16);
                    subscript = subscript_restore_a_little_hp;
                    result = TRUE;
                }
            } else if (Battler_Ability(battleCtx, battler) != ABILITY_MAGIC_GUARD) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP * -1, 8);
                subscript = subscript_lose_hp_from_item_with_message;
                result = TRUE;
            }
            break;
        }

        if (result == TRUE) {
            battleCtx->msgBattlerTemp = battler;
            battleCtx->msgItemTemp = Battler_HeldItem(battleCtx, battler);
            
            LOAD_SUBSEQ(subscript);
            battleCtx->commandNext = battleCtx->command;
            battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;
        }
    }

    return result;
}

BOOL BattleSystem_TriggerHeldItemOnStatus(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int *subscript)
{
    BOOL result = FALSE;
    int itemEffect = Battler_HeldItemEffect(battleCtx, battler);
    int itemPower = Battler_HeldItemPower(battleCtx, battler, ITEM_POWER_CHECK_ALL);

    if (battleCtx->battleMons[battler].curHP) {
        switch (itemEffect) {
        case HOLD_EFFECT_HP_RESTORE:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = itemPower;
                *subscript = subscript_held_item_hp_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_PCT_RESTORE:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP * itemPower, 100);
                *subscript = subscript_held_item_hp_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PRZ_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_PARALYSIS) {
                *subscript = subscript_held_item_prz_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_SLP_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) {
                *subscript = subscript_held_item_slp_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PSN_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY_POISON) {
                *subscript = subscript_held_item_psn_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_BRN_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN) {
                *subscript = subscript_held_item_brn_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_FRZ_RESTORE:
            if (battleCtx->battleMons[battler].status & MON_CONDITION_FREEZE) {
                *subscript = subscript_held_item_frz_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PP_RESTORE: {
            int i;
            for (i = 0; i < LEARNED_MOVES_MAX; i++) {
                if (battleCtx->battleMons[battler].moves[i] && battleCtx->battleMons[battler].ppCur[i] == 0) {
                    break;
                }
            }

            if (i != LEARNED_MOVES_MAX) {
                BattleMon_AddVal(&battleCtx->battleMons[battler], BATTLEMON_CUR_PP_1 + i, itemPower);
                BattleMon_CopyToParty(battleSys, battleCtx, battler);
                battleCtx->msgMoveTemp = battleCtx->battleMons[battler].moves[i];
                *subscript = subscript_held_item_pp_restore;
                result = TRUE;
            }

            break;
        }

        case HOLD_EFFECT_CONFUSE_RESTORE:
            if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION) {
                *subscript = subscript_held_item_cnf_restore;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_STATUS_RESTORE:
            if ((battleCtx->battleMons[battler].status & MON_CONDITION_ANY)
                    || (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION)) {
                if (battleCtx->battleMons[battler].status & MON_CONDITION_PARALYSIS) {
                    *subscript = subscript_held_item_prz_restore;
                }

                if (battleCtx->battleMons[battler].status & MON_CONDITION_SLEEP) {
                    *subscript = subscript_held_item_slp_restore;
                }

                if (battleCtx->battleMons[battler].status & MON_CONDITION_ANY_POISON) {
                    *subscript = subscript_held_item_psn_restore;
                }

                if (battleCtx->battleMons[battler].status & MON_CONDITION_BURN) {
                    *subscript = subscript_held_item_brn_restore;
                }

                if (battleCtx->battleMons[battler].status & MON_CONDITION_FREEZE) {
                    *subscript = subscript_held_item_frz_restore;
                }

                if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION) {
                    *subscript = subscript_held_item_cnf_restore;
                }

                if ((battleCtx->battleMons[battler].status & MON_CONDITION_ANY)
                        && (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_CONFUSION)) {
                    *subscript = subscript_held_item_multi_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_STATDOWN_RESTORE: {
            for (int i = BATTLE_STAT_HP; i < BATTLE_STAT_MAX; i++) {
                if (battleCtx->battleMons[battler].statBoosts[i] < 6) {
                    battleCtx->battleMons[battler].statBoosts[i] = 6;
                    result = TRUE;
                }
            }

            if (result == TRUE) {
                *subscript = subscript_held_item_statdown_restore;
            }
            break;
        }
            
        case HOLD_EFFECT_HEAL_INFATUATION:
            if (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_ATTRACT) {
                battleCtx->msgTemp = MSGCOND_INFATUATION;
                *subscript = subscript_held_item_heal_infatuation;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_ACC_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower) {
                *subscript = subscript_held_item_temp_acc_up;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_SPICY:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_SPICY;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_SPICY) == -1) {
                    *subscript = subscript_held_item_dislike_flavor;
                } else {
                    *subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_DRY:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_DRY;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_DRY) == -1) {
                    *subscript = subscript_held_item_dislike_flavor;
                } else {
                    *subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_SWEET:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_SWEET;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_SWEET) == -1) {
                    *subscript = subscript_held_item_dislike_flavor;
                } else {
                    *subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_BITTER:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_BITTER;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_BITTER) == -1) {
                    *subscript = subscript_held_item_dislike_flavor;
                } else {
                    *subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_HP_RESTORE_SOUR:
            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / 2) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP, itemPower);
                battleCtx->msgTemp = FLAVOR_SOUR;

                if (Pokemon_GetFlavorAffinityOf(battleCtx->battleMons[battler].personality, FLAVOR_SOUR) == -1) {
                    *subscript = subscript_held_item_dislike_flavor;
                } else {
                    *subscript = subscript_held_item_hp_restore;
                }

                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_ATK_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_ATTACK;
                *subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_DEF_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_DEFENSE] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_DEFENSE;
                *subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_SPEED_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SPEED] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_SPEED;
                *subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_SPATK_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_ATTACK] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_SP_ATTACK;
                *subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_SPDEF_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_SP_DEFENSE] < 12) {
                battleCtx->msgTemp = BATTLE_STAT_SP_DEFENSE;
                *subscript = subscript_held_item_sharply_raise_stat;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_CRITRATE_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= battleCtx->battleMons[battler].maxHP / itemPower
                    && (battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_FOCUS_ENERGY) == FALSE) {
                *subscript = subscript_held_item_raise_crit;
                result = TRUE;
            }
            break;

        case HOLD_EFFECT_PINCH_RANDOM_UP:
            if (Battler_Ability(battleCtx, battler) == ABILITY_GLUTTONY) {
                itemPower /= 2;
            }

            if (battleCtx->battleMons[battler].curHP <= (battleCtx->battleMons[battler].maxHP / itemPower)) {
                int i;
                for (i = 0; i < 5; i++) {
                    if (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK + i] < 12) {
                        break;
                    }
                }

                if (i != 5) {
                    do {
                        i = BattleSystem_RandNext(battleSys) % 5;
                    } while (battleCtx->battleMons[battler].statBoosts[BATTLE_STAT_ATTACK + i] == 12);

                    battleCtx->msgTemp = BATTLE_STAT_ATTACK + i;
                    *subscript = subscript_held_item_sharply_raise_stat;
                    result = TRUE;
                }
            }
            break;
        }

        if (result == TRUE) {
            battleCtx->msgItemTemp = Battler_HeldItem(battleCtx, battler);
        }
    }

    return result;
}

BOOL BattleSystem_TriggerDetrimentalHeldItem(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result = FALSE;
    int subscript;
    int itemEffect = Battler_HeldItemEffect(battleCtx, battler);
    int itemPower = Battler_HeldItemPower(battleCtx, battler, 0);

    if (battleCtx->battleMons[battler].curHP) {
        switch (itemEffect) {
        case HOLD_EFFECT_PSN_USER:
            battleCtx->sideEffectMon = battler;
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_HELD_ITEM;
            subscript = subscript_badly_poison;
            result = TRUE;
            break;
            
        case HOLD_EFFECT_BRN_USER:
            battleCtx->sideEffectMon = battler;
            battleCtx->sideEffectType = SIDE_EFFECT_TYPE_HELD_ITEM;
            subscript = subscript_burn;
            result = TRUE;
            break;

        case HOLD_EFFECT_DMG_USER_CONTACT_XFR:
            if (Battler_Ability(battleCtx, battler) != ABILITY_MAGIC_GUARD) {
                battleCtx->hpCalcTemp = BattleSystem_Divide(battleCtx->battleMons[battler].maxHP * -1, itemPower);
                subscript = subscript_lose_hp_from_item_with_message;
                result = TRUE;
            }
            break;
        }

        if (result == TRUE) {
            battleCtx->msgBattlerTemp = battler;
            battleCtx->msgItemTemp = Battler_HeldItem(battleCtx, battler);

            LOAD_SUBSEQ(subscript);
            battleCtx->commandNext = battleCtx->command;
            battleCtx->command = BATTLE_CONTROL_EXEC_SCRIPT;
        }
    }

    return result;
}

u16 Battler_HeldItem(BattleContext *battleCtx, int battler)
{
    if (battleCtx->battleMons[battler].moveEffectsData.embargoTurns) {
        return ITEM_NONE;
    }

    return battleCtx->battleMons[battler].heldItem;
}

BOOL Battler_MovedThisTurn(BattleContext *battleCtx, int battler)
{
    return battleCtx->battlerActions[battler][BATTLE_ACTION_PICK_COMMAND] == BATTLE_CONTROL_MOVE_END;
}

BOOL BattleSystem_TriggerHeldItemOnHit(BattleSystem *battleSys, BattleContext *battleCtx, int *subscript)
{
    BOOL result = FALSE;

    if (battleCtx->defender == BATTLER_NONE) {
        return result;
    }

    if (Battler_SubstituteWasHit(battleCtx, battleCtx->defender) == TRUE) {
        return result;
    }

    int itemEffect = Battler_HeldItemEffect(battleCtx, battleCtx->defender);
    int itemPower = Battler_HeldItemPower(battleCtx, battleCtx->defender, 0);
    int side = Battler_Side(battleSys, battleCtx->attacker);

    switch (itemEffect) {
    case HOLD_EFFECT_DMG_USER_CONTACT_XFR:
        if (ATTACKING_MON.curHP
                && ATTACKING_MON.heldItem == ITEM_NONE
                && (battleCtx->sideConditions[side].knockedOffItemsMask & FlagIndex(battleCtx->selectedPartySlot[battleCtx->attacker])) == FALSE
                && battleCtx->moveCur != MOVE_KNOCK_OFF
                && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))) {
            *subscript = subscript_transfer_sticky_barb;
            result = TRUE;
        }
        break;

    case HOLD_EFFECT_RECOIL_PHYSICAL:
        if (ATTACKING_MON.curHP
                && Battler_Ability(battleCtx, battleCtx->attacker) != ABILITY_MAGIC_GUARD
                && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
                && DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP * -1, itemPower);
            *subscript = subscript_held_item_recoil_when_hit;
            result = TRUE;
        }
        break;

    case HOLD_EFFECT_RECOIL_SPECIAL:
        if (ATTACKING_MON.curHP
                && Battler_Ability(battleCtx, battleCtx->attacker) != ABILITY_MAGIC_GUARD
                && DEFENDER_SELF_TURN_FLAGS.specialDamageTaken) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP * -1, itemPower);
            *subscript = subscript_held_item_recoil_when_hit;
            result = TRUE;
        }
        break;

    case HOLD_EFFECT_HP_RESTORE_SE:
        if (DEFENDING_MON.curHP && (battleCtx->moveStatusFlags & MOVE_STATUS_SUPER_EFFECTIVE)) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(DEFENDING_MON.maxHP, itemPower);
            *subscript = subscript_held_item_hp_restore;
            battleCtx->msgBattlerTemp = battleCtx->defender;
            battleCtx->msgItemTemp = battleCtx->battleMons[battleCtx->defender].heldItem;
            result = TRUE;
        }
        break;
		
	case HOLD_EFFECT_WEAK_RAISE_SPA_ATK:
			if (DEFENDING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_SUPER_EFFECTIVE)
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (DEFENDING_MON.wpolicyFlag == TRUE))
			{
				*subscript = subscript_weakness_policy_activate;
				battleCtx->msgBattlerTemp = battleCtx->defender;
				battleCtx->msgItemTemp = battleCtx->battleMons[battleCtx->defender].heldItem;
				result = TRUE;
				DEFENDING_MON.wpolicyFlag = FALSE;
			}
			break;
			
	case HOLD_EFFECT_LEVITATE_POPPED_IF_HIT:
			if (DEFENDING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken))
			{
				*subscript = subscript_air_balloon_burst;
				battleCtx->msgBattlerTemp = battleCtx->defender;
				battleCtx->msgItemTemp = battleCtx->battleMons[battleCtx->defender].heldItem;
				result = TRUE;	
			}
			break;
			
	case HOLD_EFFECT_SWITCH_ATTACKER_HIT:
			if (DEFENDING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (ATTACKING_MON.sheerForceFlag == FALSE)
			&& (BattleSystem_AnyReplacementMons(battleSys, battleCtx, battleCtx->attacker) == TRUE)
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken))
			{
				*subscript = subscript_hold_up_card;
				result = TRUE;	
			}
			break;

    default:
        break;
    }

    return result;
}

s32 Battler_HeldItemEffect(BattleContext *battleCtx, int battler)
{
    u16 item = Battler_HeldItem(battleCtx, battler);
    return BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HOLD_EFFECT);
}

s32 Battler_HeldItemPower(BattleContext *battleCtx, int battler, enum HeldItemPowerOp opcode)
{
    u16 item;
    switch (opcode) {
    case ITEM_POWER_CHECK_ALL:
        item = Battler_HeldItem(battleCtx, battler);
        break;

    case ITEM_POWER_CHECK_EMBARGO:
        if (battleCtx->battleMons[battler].moveEffectsData.embargoTurns) {
            return 0;
        }

    case ITEM_POWER_CHECK_NONE:
        item = battleCtx->battleMons[battler].heldItem;
        break;
    }

    return BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HOLD_EFFECT_PARAM);
}

s32 Battler_NaturalGiftPower(BattleContext *battleCtx, int battler)
{
    u16 item = Battler_HeldItem(battleCtx, battler);
    return BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_NATURAL_GIFT_POWER);
}

s32 Battler_NaturalGiftType(BattleContext *battleCtx, int battler)
{
    u16 item = Battler_HeldItem(battleCtx, battler);
    return BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_NATURAL_GIFT_TYPE);
}

s32 Battler_ItemPluckEffect(BattleContext *battleCtx, int battler)
{
    return BattleSystem_GetItemData(battleCtx, battleCtx->battleMons[battler].heldItem, ITEM_PARAM_PLUCK_EFFECT);
}

s32 Battler_ItemFlingEffect(BattleContext *battleCtx, int battler)
{
    if (battleCtx->battleMons[battler].moveEffectsData.embargoTurns) {
        return FLING_EFFECT_NONE;
    }

    return BattleSystem_GetItemData(battleCtx, battleCtx->battleMons[battler].heldItem, ITEM_PARAM_FLING_EFFECT);
}

s32 Battler_ItemFlingPower(BattleContext *battleCtx, int battler)
{
    if (battleCtx->battleMons[battler].moveEffectsData.embargoTurns) {
        return 0;
    }

    return BattleSystem_GetItemData(battleCtx, battleCtx->battleMons[battler].heldItem, ITEM_PARAM_FLING_POWER);
}

static inline BOOL BattlerIsGrounded(BattleContext *battleCtx, int battler)
{
    return ((Battler_Ability(battleCtx, battler) != ABILITY_LEVITATE
					&& Battler_HeldItemEffect(battleCtx, battler) != HOLD_EFFECT_LEVITATE_POPPED_IF_HIT
                    && battleCtx->battleMons[battler].moveEffectsData.magnetRiseTurns == 0
                    && MON_IS_NOT_TYPE(battler, TYPE_FLYING))
            || Battler_HeldItemEffect(battleCtx, battler) == HOLD_EFFECT_SPEED_DOWN_GROUNDED
            || (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY));
}

static inline int CountAbilityTheirSide(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int ability)
{
    return BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_THEIR_SIDE, battler, ability);
} 

BOOL Battler_IsTrapped(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int result = FALSE;

    if (Battler_HeldItemEffect(battleCtx, battler) == HOLD_EFFECT_SWITCH) {
        return FALSE;
    }

    if ((battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_TRAPPED)
            || (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_INGRAIN)) {
        result = TRUE;
    }

    if ((Battler_Ability(battleCtx, battler) != ABILITY_SHADOW_TAG && CountAbilityTheirSide(battleSys, battleCtx, battler, ABILITY_SHADOW_TAG))
            || (MON_HAS_TYPE(battler, TYPE_STEEL) && CountAbilityTheirSide(battleSys, battleCtx, battler, ABILITY_MAGNET_PULL))) {
        result = TRUE;
    }

    if (BattlerIsGrounded(battleCtx, battler) && CountAbilityTheirSide(battleSys, battleCtx, battler, ABILITY_ARENA_TRAP)) {
        result = TRUE;
    }

    return result;
}

BOOL BattleSystem_PluckBerry(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result = FALSE;
    int nextSeq = 0;
    int effect = Battler_ItemPluckEffect(battleCtx, battler);
    int power = Battler_HeldItemPower(battleCtx, battler, 1);

    if (Battler_SubstituteWasHit(battleCtx, battleCtx->defender) == TRUE) {
        return result;
    }

    switch (effect) {
    case PLUCK_EFFECT_HP_RESTORE:
        if (ATTACKING_MON.curHP != ATTACKING_MON.maxHP) {
            battleCtx->hpCalcTemp = power;
            nextSeq = subscript_held_item_hp_restore;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_HP_PCT_RESTORE:
        if (ATTACKING_MON.curHP != ATTACKING_MON.maxHP) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP * power, 100);
            nextSeq = subscript_held_item_hp_restore;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_PRZ_RESTORE:
        if (ATTACKING_MON.status & MON_CONDITION_PARALYSIS) {
            nextSeq = subscript_held_item_prz_restore;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_SLP_RESTORE:
        if (ATTACKING_MON.status & MON_CONDITION_SLEEP) {
            nextSeq = subscript_held_item_slp_restore;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_PSN_RESTORE:
        if (ATTACKING_MON.status & MON_CONDITION_ANY_POISON) {
            nextSeq = subscript_held_item_psn_restore;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_BRN_RESTORE:
        if (ATTACKING_MON.status & MON_CONDITION_BURN) {
            nextSeq = subscript_held_item_brn_restore;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_FRZ_RESTORE:
        if (ATTACKING_MON.status & MON_CONDITION_FREEZE) {
            nextSeq = subscript_held_item_frz_restore;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_PP_RESTORE: {
        int diff, i, maxDiff, slot; // must declare in this order to match.

        // Find the move that has the highest difference between its current
        // and maximum PP.
        maxDiff = 0;
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (ATTACKING_MON.moves[i]) {
                diff = MoveTable_CalcMaxPP(ATTACKING_MON.moves[i], ATTACKING_MON.ppUps[i]) - ATTACKING_MON.ppCur[i];

                if (diff > maxDiff) {
                    maxDiff = diff;
                    slot = i;
                }
            }
        }

        BattleMon_AddVal(&ATTACKING_MON, BATTLEMON_CUR_PP_1 + slot, power);
        BattleMon_CopyToParty(battleSys, battleCtx, battleCtx->attacker);
        battleCtx->msgMoveTemp = ATTACKING_MON.moves[slot];
        nextSeq = subscript_held_item_pp_restore;

        result = TRUE;
        break;
    }

    case PLUCK_EFFECT_CNF_RESTORE:
        if (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_CONFUSION) {
            nextSeq = subscript_held_item_cnf_restore;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_ALL_RESTORE:
        if ((ATTACKING_MON.status & MON_CONDITION_ANY) || (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_CONFUSION)) {
            if (ATTACKING_MON.status & MON_CONDITION_PARALYSIS) {
                nextSeq = subscript_held_item_prz_restore;
            }

            if (ATTACKING_MON.status & MON_CONDITION_SLEEP) {
                nextSeq = subscript_held_item_slp_restore;
            }

            if (ATTACKING_MON.status & MON_CONDITION_ANY_POISON) {
                nextSeq = subscript_held_item_psn_restore;
            }

            if (ATTACKING_MON.status & MON_CONDITION_BURN) {
                nextSeq = subscript_held_item_brn_restore;
            }

            if (ATTACKING_MON.status & MON_CONDITION_FREEZE) {
                nextSeq = subscript_held_item_frz_restore;
            }

            if (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_CONFUSION) {
                nextSeq = subscript_held_item_cnf_restore;
            }

            if ((ATTACKING_MON.status & MON_CONDITION_ANY)
                    && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_CONFUSION)) {
                nextSeq = subscript_held_item_multi_restore;
            }
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_HP_RESTORE_SPICY:
        if (ATTACKING_MON.curHP != ATTACKING_MON.maxHP) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP, power);
            battleCtx->msgTemp = FLAVOR_SPICY;

            if (Pokemon_GetFlavorAffinityOf(ATTACKING_MON.personality, FLAVOR_SPICY) == -1) {
                nextSeq = subscript_held_item_dislike_flavor;
            } else {
                nextSeq = subscript_held_item_hp_restore;
            }
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_HP_RESTORE_DRY:
        if (ATTACKING_MON.curHP != ATTACKING_MON.maxHP) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP, power);
            battleCtx->msgTemp = FLAVOR_DRY;

            if (Pokemon_GetFlavorAffinityOf(ATTACKING_MON.personality, FLAVOR_DRY) == -1) {
                nextSeq = subscript_held_item_dislike_flavor;
            } else {
                nextSeq = subscript_held_item_hp_restore;
            }
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_HP_RESTORE_SWEET:
        if (ATTACKING_MON.curHP != ATTACKING_MON.maxHP) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP, power);
            battleCtx->msgTemp = FLAVOR_SWEET;

            if (Pokemon_GetFlavorAffinityOf(ATTACKING_MON.personality, FLAVOR_SWEET) == -1) {
                nextSeq = subscript_held_item_dislike_flavor;
            } else {
                nextSeq = subscript_held_item_hp_restore;
            }
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_HP_RESTORE_BITTER:
        if (ATTACKING_MON.curHP != ATTACKING_MON.maxHP) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP, power);
            battleCtx->msgTemp = FLAVOR_BITTER;

            if (Pokemon_GetFlavorAffinityOf(ATTACKING_MON.personality, FLAVOR_BITTER) == -1) {
                nextSeq = subscript_held_item_dislike_flavor;
            } else {
                nextSeq = subscript_held_item_hp_restore;
            }
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_HP_RESTORE_SOUR:
        if (ATTACKING_MON.curHP != ATTACKING_MON.maxHP) {
            battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP, power);
            battleCtx->msgTemp = FLAVOR_SOUR;

            if (Pokemon_GetFlavorAffinityOf(ATTACKING_MON.personality, FLAVOR_SOUR) == -1) {
                nextSeq = subscript_held_item_dislike_flavor;
            } else {
                nextSeq = subscript_held_item_hp_restore;
            }
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_ATK_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_ATTACK] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_ATTACK;
            nextSeq = subscript_held_item_sharply_raise_stat;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_DEF_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_DEFENSE] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_DEFENSE;
            nextSeq = subscript_held_item_sharply_raise_stat;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_SPEED_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_SPEED] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SPEED;
            nextSeq = subscript_held_item_sharply_raise_stat;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_SPATK_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_SP_ATTACK] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SP_ATTACK;
            nextSeq = subscript_held_item_sharply_raise_stat;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_SPDEF_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_SP_DEFENSE] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SP_DEFENSE;
            nextSeq = subscript_held_item_sharply_raise_stat;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_RANDOM_UP2: {
        int stat;
        for (stat = 0; stat < 5; stat++) {
            if (ATTACKING_MON.statBoosts[BATTLE_STAT_ATTACK + stat] < 12) {
                break;
            }
        }

        if (stat != 5) {
            do {
                stat = BattleSystem_RandNext(battleSys) % 5;
            } while (ATTACKING_MON.statBoosts[BATTLE_STAT_ATTACK + stat] == 12);

            battleCtx->msgTemp = BATTLE_STAT_ATTACK + stat;
            nextSeq = subscript_held_item_sharply_raise_stat;
        }

        result = TRUE;
        break;
    }

    case PLUCK_EFFECT_CRIT_UP:
        if ((ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_FOCUS_ENERGY) == FALSE) {
            nextSeq = subscript_held_item_raise_crit;
        }

        result = TRUE;
        break;

    case PLUCK_EFFECT_TEMP_ACC_UP:
        nextSeq = subscript_held_item_temp_acc_up;
        result = TRUE;
        break;

    default:
        if (Item_IsBerry(battleCtx->battleMons[battler].heldItem) == TRUE) {
            result = TRUE;
        }
        break;
    }

    if (result == TRUE) {
        if (ATTACKING_MON.moveEffectsMask & MOVE_EFFECT_EMBARGO) {
            battleCtx->scriptTemp = 0;
        } else {
            battleCtx->scriptTemp = nextSeq;
        }

        battleCtx->msgItemTemp = battleCtx->battleMons[battler].heldItem;
        ATTACKER_SELF_TURN_FLAGS.statusFlags |= SELF_TURN_FLAG_PLUCK_BERRY;
    }

    return result;
}

BOOL BattleSystem_FlingItem(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int effect = Battler_ItemFlingEffect(battleCtx, battler);
    int effectPower = Battler_HeldItemPower(battleCtx, battler, ITEM_PARAM_HOLD_EFFECT_PARAM);

    battleCtx->movePower = Battler_ItemFlingPower(battleCtx, battler);
    battleCtx->flingScript = 0;
    battleCtx->sideEffectType = SIDE_EFFECT_TYPE_NONE;

    if (battleCtx->movePower == 0) {
        return FALSE;
    }

    switch (effect) {
    case FLING_EFFECT_HP_RESTORE:
        battleCtx->flingTemp = effectPower;
        battleCtx->flingScript = subscript_held_item_hp_restore;
        break;

    case FLING_EFFECT_HP_PCT_RESTORE:
        battleCtx->flingTemp = BattleSystem_Divide(DEFENDING_MON.maxHP * effectPower, 100);
        battleCtx->flingScript = subscript_held_item_hp_restore;
        break;

    case FLING_EFFECT_PRZ_RESTORE:
        if (DEFENDING_MON.status & MON_CONDITION_PARALYSIS) {
            battleCtx->flingScript = subscript_held_item_prz_restore;
        }
        break;

    case FLING_EFFECT_SLP_RESTORE:
        if (DEFENDING_MON.status & MON_CONDITION_SLEEP) {
            battleCtx->flingScript = subscript_held_item_slp_restore;
        }
        break;

    case FLING_EFFECT_PSN_RESTORE:
        if (DEFENDING_MON.status & MON_CONDITION_ANY_POISON) {
            battleCtx->flingScript = subscript_held_item_psn_restore;
        }
        break;

    case FLING_EFFECT_BRN_RESTORE:
        if (DEFENDING_MON.status & MON_CONDITION_BURN) {
            battleCtx->flingScript = subscript_held_item_brn_restore;
        }
        break;

    case FLING_EFFECT_FRZ_RESTORE:
        if (DEFENDING_MON.status & MON_CONDITION_FREEZE) {
            battleCtx->flingScript = subscript_held_item_frz_restore;
        }
        break;

    case FLING_EFFECT_PP_RESTORE: {
        int diff, i, maxDiff, slot; // must declare in this order to match.

        // Find the move that has the highest difference between its current
        // and maximum PP.
        maxDiff = 0;
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {
            if (DEFENDING_MON.moves[i]) {
                diff = MoveTable_CalcMaxPP(DEFENDING_MON.moves[i], DEFENDING_MON.ppUps[i]) - DEFENDING_MON.ppCur[i];

                if (diff > maxDiff) {
                    maxDiff = diff;
                    slot = i;
                }
            }
        }

        if (maxDiff) {
            BattleMon_AddVal(&DEFENDING_MON, BATTLEMON_CUR_PP_1 + slot, effectPower);
            BattleMon_CopyToParty(battleSys, battleCtx, battleCtx->defender);
            battleCtx->msgMoveTemp = DEFENDING_MON.moves[slot];
            battleCtx->flingScript = subscript_held_item_pp_restore;
        }

        break;
    }

    case FLING_EFFECT_CNF_RESTORE:
        if (DEFENDING_MON.statusVolatile & VOLATILE_CONDITION_CONFUSION) {
            battleCtx->flingScript = subscript_held_item_cnf_restore;
        }
        break;

    case FLING_EFFECT_ALL_RESTORE:
        if ((DEFENDING_MON.status & MON_CONDITION_ANY) || (DEFENDING_MON.statusVolatile & VOLATILE_CONDITION_CONFUSION)) {
            if (DEFENDING_MON.status & MON_CONDITION_PARALYSIS) {
                battleCtx->flingScript = subscript_held_item_prz_restore;
            }

            if (DEFENDING_MON.status & MON_CONDITION_SLEEP) {
                battleCtx->flingScript = subscript_held_item_slp_restore;
            }

            if (DEFENDING_MON.status & MON_CONDITION_ANY_POISON) {
                battleCtx->flingScript = subscript_held_item_psn_restore;
            }

            if (DEFENDING_MON.status & MON_CONDITION_BURN) {
                battleCtx->flingScript = subscript_held_item_brn_restore;
            }

            if (DEFENDING_MON.status & MON_CONDITION_FREEZE) {
                battleCtx->flingScript = subscript_held_item_frz_restore;
            }

            if (DEFENDING_MON.statusVolatile & VOLATILE_CONDITION_CONFUSION) {
                battleCtx->flingScript = subscript_held_item_cnf_restore;
            }

            if ((DEFENDING_MON.status & MON_CONDITION_ANY)
                    && (DEFENDING_MON.statusVolatile & VOLATILE_CONDITION_CONFUSION)) {
                battleCtx->flingScript = subscript_held_item_multi_restore;
            }
        }
        break;
		
	case FLING_EFFECT_TRICK_ROOM:
            battleCtx->flingScript = subscript_trick_room_start_fling;
        break;
		
	case FLING_EFFECT_GRAVITY:
			battleCtx->flingScript = subscript_gravity_start_fling;
		break;
		
	case FLING_EFFECT_INFATUATION:
			battleCtx->sideEffectMon = battleCtx->defender;
			battleCtx->flingScript = subscript_infatuate_fling;
        break;
		
	case FLING_EFFECT_HAZE:
			battleCtx->flingScript = subscript_reset_all_stat_stages;
		break;
		
	case FLING_EFFECT_RAIN:
			battleCtx->flingScript = subscript_drizzle;
		break;
		
	case FLING_EFFECT_SUN:
			battleCtx->flingScript = subscript_drought;
		break;
		
	case FLING_EFFECT_SAND:
			battleCtx->flingScript = subscript_sand_stream;
		break;
		
	case FLING_EFFECT_HAIL:
			battleCtx->flingScript = subscript_snow_warning;
		break;
		
	case FLING_EFFECT_DEFOG:
			battleCtx->flingScript = subscript_defog;
		break;
		
	case FLING_EFFECT_WAKE_UP_SLAP:
			if (DEFENDING_MON.status & MON_CONDITION_SLEEP)
			{
				battleCtx->flingScript = subscript_heal_target_sleep;
			}
		break;
		
	case FLING_EFFECT_NIGHTMARE:
			if ((DEFENDING_MON.status & MON_CONDITION_SLEEP)
			&& (battleCtx->battleMons[battleCtx->defender].statusVolatile != VOLATILE_CONDITION_NIGHTMARE))
			{
				battleCtx->flingScript = subscript_nightmare_start;
			}
		break;
		
	case FLING_EFFECT_CONFUSION:
			battleCtx->sideEffectMon = battler;
			battleCtx->sideEffectType = SIDE_EFFECT_TYPE_INDIRECT;
			battleCtx->flingScript = subscript_confuse;
		break;
		
	case FLING_EFFECT_CHIP:
			battleCtx->flingScript = subscript_apply_chip;
		break;
		
	case FLING_EFFECT_LOWER_EVASION:
			battleCtx->sideEffectMon = battleCtx->defender;
			battleCtx->flingScript = subscript_fling_lower_evasion;
		break;
		
	case FLING_EFFECT_LOWER_ACC:
			battleCtx->sideEffectMon = battleCtx->defender;
			battleCtx->flingScript = subscript_fling_lower_acc;
		break;
		
	case FLING_EFFECT_INFLICT_CURSE:
			battleCtx->flingScript = subscript_curse_ghost;
		break;
		
	case FLING_EFFECT_INFLICT_INGRAIN:
			battleCtx->flingScript = subscript_ingrain_fling;
		break;
		
	case FLING_EFFECT_USER_ATK_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_ATTACK] < 12) {
            battleCtx->flingScript = subscript_held_item_user_raise_atk;
        }
        break;

    case FLING_EFFECT_USER_DEF_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_DEFENSE] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_DEFENSE;
            battleCtx->flingScript = subscript_held_item_user_raise_def;
        }
        break;

    case FLING_EFFECT_USER_SPEED_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_SPEED] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SPEED;
            battleCtx->flingScript = subscript_held_item_user_raise_spe;
        }
        break;

    case FLING_EFFECT_USER_SPATK_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_SP_ATTACK] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SP_ATTACK;
            battleCtx->flingScript = subscript_held_item_user_raise_spa;
        }
        break;

    case FLING_EFFECT_USER_SPDEF_UP:
        if (ATTACKING_MON.statBoosts[BATTLE_STAT_SP_DEFENSE] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SP_DEFENSE;
            battleCtx->flingScript = subscript_held_item_user_raise_spd;
        }
        break;

    case FLING_EFFECT_HP_RESTORE_SPICY:
        battleCtx->flingTemp = BattleSystem_Divide(DEFENDING_MON.maxHP, effectPower);
        battleCtx->msgTemp = FLAVOR_SPICY;

        if (Pokemon_GetFlavorAffinityOf(DEFENDING_MON.personality, FLAVOR_SPICY) == -1) {
            battleCtx->flingScript = subscript_held_item_dislike_flavor;
        } else {
            battleCtx->flingScript = subscript_held_item_hp_restore;
        }
        break;

    case FLING_EFFECT_HP_RESTORE_DRY:
        battleCtx->flingTemp = BattleSystem_Divide(DEFENDING_MON.maxHP, effectPower);
        battleCtx->msgTemp = FLAVOR_DRY;

        if (Pokemon_GetFlavorAffinityOf(DEFENDING_MON.personality, FLAVOR_DRY) == -1) {
            battleCtx->flingScript = subscript_held_item_dislike_flavor;
        } else {
            battleCtx->flingScript = subscript_held_item_hp_restore;
        }
        break;

    case FLING_EFFECT_HP_RESTORE_SWEET:
        battleCtx->flingTemp = BattleSystem_Divide(DEFENDING_MON.maxHP, effectPower);
        battleCtx->msgTemp = FLAVOR_SWEET;

        if (Pokemon_GetFlavorAffinityOf(DEFENDING_MON.personality, FLAVOR_SWEET) == -1) {
            battleCtx->flingScript = subscript_held_item_dislike_flavor;
        } else {
            battleCtx->flingScript = subscript_held_item_hp_restore;
        }
        break;

    case FLING_EFFECT_HP_RESTORE_BITTER:
        battleCtx->flingTemp = BattleSystem_Divide(DEFENDING_MON.maxHP, effectPower);
        battleCtx->msgTemp = FLAVOR_BITTER;

        if (Pokemon_GetFlavorAffinityOf(DEFENDING_MON.personality, FLAVOR_BITTER) == -1) {
            battleCtx->flingScript = subscript_held_item_dislike_flavor;
        } else {
            battleCtx->flingScript = subscript_held_item_hp_restore;
        }
        break;

    case FLING_EFFECT_HP_RESTORE_SOUR:
        battleCtx->flingTemp = BattleSystem_Divide(DEFENDING_MON.maxHP, effectPower);
        battleCtx->msgTemp = FLAVOR_SOUR;

        if (Pokemon_GetFlavorAffinityOf(DEFENDING_MON.personality, FLAVOR_SOUR) == -1) {
            battleCtx->flingScript = subscript_held_item_dislike_flavor;
        } else {
            battleCtx->flingScript = subscript_held_item_hp_restore;
        }
        break;

    case FLING_EFFECT_STATDOWN_RESTORE:
        for (int i = BATTLE_STAT_HP; i < BATTLE_STAT_MAX; i++) {
            if (DEFENDING_MON.statBoosts[i] < 6) {
                DEFENDING_MON.statBoosts[i] = 6;
                battleCtx->flingScript = subscript_held_item_statdown_restore;
            }
        }
        break;

    case FLING_EFFECT_HEAL_INFATUATION:
        if (DEFENDING_MON.statusVolatile & VOLATILE_CONDITION_ATTRACT) {
            battleCtx->msgTemp = MSGCOND_INFATUATION;
            battleCtx->flingScript = subscript_held_item_heal_infatuation;
        }
        break;

    case FLING_EFFECT_FLINCH:
        battleCtx->sideEffectMon = battler;
        battleCtx->sideEffectType = SIDE_EFFECT_TYPE_INDIRECT;
        battleCtx->flingScript = subscript_flinch_mon;
        break;

    case FLING_EFFECT_PARALYZE:
        battleCtx->sideEffectMon = battler;
        battleCtx->sideEffectType = SIDE_EFFECT_TYPE_INDIRECT;
        battleCtx->flingScript = subscript_paralyze;
        break;

    case FLING_EFFECT_POISON:
        battleCtx->sideEffectMon = battler;
        battleCtx->sideEffectType = SIDE_EFFECT_TYPE_INDIRECT;
        battleCtx->flingScript = subscript_poison;
        break;

    case FLING_EFFECT_BADLY_POISON:
        battleCtx->sideEffectMon = battler;
        battleCtx->sideEffectType = SIDE_EFFECT_TYPE_INDIRECT;
        battleCtx->flingScript = subscript_badly_poison;
        break;

    case FLING_EFFECT_BURN:
        battleCtx->sideEffectMon = battler;
        battleCtx->sideEffectType = SIDE_EFFECT_TYPE_INDIRECT;
        battleCtx->flingScript = subscript_burn;
        break;

    case FLING_EFFECT_ATK_UP:
        if (DEFENDING_MON.statBoosts[BATTLE_STAT_ATTACK] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_ATTACK;
            battleCtx->flingScript = subscript_held_item_raise_stat;
        }
        break;

    case FLING_EFFECT_DEF_UP:
        if (DEFENDING_MON.statBoosts[BATTLE_STAT_DEFENSE] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_DEFENSE;
            battleCtx->flingScript = subscript_held_item_raise_stat;
        }
        break;

    case FLING_EFFECT_SPEED_UP:
        if (DEFENDING_MON.statBoosts[BATTLE_STAT_SPEED] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SPEED;
            battleCtx->flingScript = subscript_held_item_raise_stat;
        }
        break;

    case FLING_EFFECT_SPATK_UP:
        if (DEFENDING_MON.statBoosts[BATTLE_STAT_SP_ATTACK] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SP_ATTACK;
            battleCtx->flingScript = subscript_held_item_raise_stat;
        }
        break;

    case FLING_EFFECT_SPDEF_UP:
        if (DEFENDING_MON.statBoosts[BATTLE_STAT_SP_DEFENSE] < 12) {
            battleCtx->msgTemp = BATTLE_STAT_SP_DEFENSE;
            battleCtx->flingScript = subscript_held_item_raise_stat;
        }
        break;

    case FLING_EFFECT_RANDOM_UP2: {
        int stat;

        for (stat = 0; stat < 5; stat++) {
            if (DEFENDING_MON.statBoosts[BATTLE_STAT_ATTACK + stat] < 12) {
                break;
            }
        }

        if (stat != 5) {
            do {
                stat = BattleSystem_RandNext(battleSys) % 5;
            } while (DEFENDING_MON.statBoosts[BATTLE_STAT_ATTACK + stat] == 12);

            battleCtx->msgTemp = BATTLE_STAT_ATTACK + stat;
            battleCtx->flingScript = subscript_held_item_sharply_raise_stat;
        }

        break;
    }

    case FLING_EFFECT_CRIT_UP:
        if ((DEFENDING_MON.statusVolatile & VOLATILE_CONDITION_FOCUS_ENERGY) == FALSE) {
            battleCtx->flingScript = subscript_held_item_raise_crit;
        }
        break;

    case FLING_EFFECT_TEMP_ACC_UP:
        battleCtx->flingScript = subscript_held_item_temp_acc_up;
        break;

    default:
        break;
    }

    if (DEFENDING_MON.moveEffectsMask & MOVE_EFFECT_EMBARGO) {
        battleCtx->flingScript = 0;
    } else {
        battleCtx->msgItemTemp = battleCtx->battleMons[battler].heldItem;

        if (battleCtx->sideEffectType == SIDE_EFFECT_TYPE_NONE && battleCtx->flingScript) {
            ATTACKER_SELF_TURN_FLAGS.statusFlags |= SELF_TURN_FLAG_PLUCK_BERRY;
        }

        battleCtx->msgBattlerTemp = battleCtx->defender;
    }

    return TRUE;
}

void BattleSystem_UpdateMetronomeCount(BattleSystem *battleSys, BattleContext *battleCtx)
{
    if (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) == HOLD_EFFECT_BOOST_REPEATED) {
        if ((ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_THRASH) == FALSE
                && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_UPROAR) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_LAST_OF_MULTI_TURN) == FALSE
                && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_MOVE_LOCKED) == FALSE) {
            if (battleCtx->metronomeMove[battleCtx->attacker] == battleCtx->moveTemp) {
                if (ATTACKING_MON.moveEffectsData.metronomeTurns < 10) {
                    ATTACKING_MON.moveEffectsData.metronomeTurns++;
                }
            } else {
                ATTACKING_MON.moveEffectsData.metronomeTurns = 0;
                battleCtx->metronomeMove[battleCtx->attacker] = battleCtx->moveTemp;
            }
        }
    } else {
        ATTACKING_MON.moveEffectsData.metronomeTurns = 0;
    }
}

void BattleSystem_VerifyMetronomeCount(BattleSystem *battleSys, BattleContext *battleCtx)
{
    if (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) == HOLD_EFFECT_BOOST_REPEATED) {
        if ((battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS)
                && battleCtx->metronomeMove[battleCtx->attacker] == battleCtx->moveTemp
                && ATTACKING_MON.moveEffectsData.metronomeTurns
                && ATTACKER_SELF_TURN_FLAGS.repeatedMoveCount == 0
                && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_THRASH) == FALSE
                && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_UPROAR) == FALSE
                && (battleCtx->battleStatusMask & SYSCTL_LAST_OF_MULTI_TURN) == FALSE
                && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_MOVE_LOCKED) == FALSE) {
            ATTACKING_MON.moveEffectsData.metronomeTurns--;
        }
    } else {
        ATTACKING_MON.moveEffectsData.metronomeTurns = 0;
    }
}

enum PokemonCryMod Battler_CryModulation(BattleContext *battleCtx, int battler, int battlerType, BOOL encounter)
{
    BOOL doubles;
    if (encounter == TRUE
            && (battlerType == BATTLER_TYPE_PLAYER_SIDE_SLOT_1 || battlerType == BATTLER_TYPE_ENEMY_SIDE_SLOT_1)) {
        doubles = TRUE;
    } else {
        doubles = FALSE;
    }

    int cryMod = POKECRY_NORMAL;
    if (doubles == TRUE) { // conditional must stay to match, even though it's superfluous
        cryMod = POKECRY_NORMAL;
    }

    int hpColor = HealthBar_Color(battleCtx->battleMons[battler].curHP, battleCtx->battleMons[battler].maxHP, (8 * 6));

    if ((battleCtx->battleMons[battler].status & MON_CONDITION_ANY)
            || (hpColor != BARCOLOR_MAX && hpColor != BARCOLOR_GREEN)) {
        cryMod = POKECRY_PINCH_NORMAL;
    }

    return cryMod;
}

BOOL Battler_CanPickCommand(BattleContext *battleSys, int battler)
{
    BOOL result = TRUE;

    if ((battleSys->battleMons[battler].statusVolatile & VOLATILE_CONDITION_RECHARGING)
            || (battleSys->battleMons[battler].statusVolatile & VOLATILE_CONDITION_THRASH)
            || (battleSys->battleMons[battler].statusVolatile & VOLATILE_CONDITION_UPROAR)
            || (battleSys->battleMons[battler].statusVolatile & VOLATILE_CONDITION_MOVE_LOCKED)) {
        result = FALSE;
    }

    return result;
}

void BattleSystem_SetPokemonCatchData(BattleSystem *battleSys, BattleContext *battleCtx, Pokemon *mon)
{
    TrainerInfo *trInfo = BattleSystem_TrainerInfo(battleSys, BATTLER_US);
    int mapHeader = BattleSystem_MapHeader(battleSys);
    int terrain = BattleSystem_Terrain(battleSys);

    int ball;
    if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_PAL_PARK) {
        ball = Pokemon_GetValue(mon, MON_DATA_POKEBALL, NULL);
    } else {
        ball = battleCtx->msgItemTemp;
    }

    Pokemon_SetCatchData(mon, trInfo, ball, mapHeader, terrain, HEAP_ID_BATTLE);
}

u8 BattleContext_IOBufferVal(BattleContext *battleCtx, int battler)
{
    return battleCtx->ioBuffer[battler][0];
}

BOOL Battler_SubstituteWasHit(BattleContext *battleCtx, int battler)
{
    BOOL result = FALSE;

    if (battleCtx->selfTurnFlags[battler].statusFlags & SELF_TURN_FLAG_SUBSTITUTE_HIT) {
        result = TRUE;
    }

    return result;
}

BOOL BattleSystem_TrainerIsOT(BattleSystem *battleSys, BattleContext *battleCtx)
{
    TrainerInfo *trInfo = BattleSystem_TrainerInfo(battleSys, BATTLER_US);
    u32 trID = TrainerInfo_ID(trInfo);
    u32 trGender = TrainerInfo_Gender(trInfo);
    const charcode_t *trName = TrainerInfo_Name(trInfo);

    if (trID == ATTACKING_MON.OTId
            && trGender == ATTACKING_MON.OTGender
            && GF_strncmp(trName, ATTACKING_MON.OTName, TRAINER_NAME_LEN) == 0) {
        return TRUE;
    }

    return FALSE;
}

BOOL BattleSystem_PokemonIsOT(BattleSystem *battleSys, Pokemon *mon)
{
    TrainerInfo *trInfo = BattleSystem_TrainerInfo(battleSys, BATTLER_US);
    u32 trID = TrainerInfo_ID(trInfo);
    u32 trGender = TrainerInfo_Gender(trInfo);
    const charcode_t *trName = TrainerInfo_Name(trInfo);

    charcode_t monOTName[TRAINER_NAME_LEN + 1];
    Pokemon_GetValue(mon, MON_DATA_OTNAME, monOTName);

    if (trID == Pokemon_GetValue(mon, MON_DATA_OT_ID, NULL)
            && trGender == Pokemon_GetValue(mon, MON_DATA_OT_GENDER, NULL)
            && GF_strncmp(trName, monOTName, TRAINER_NAME_LEN) == 0) {
        return TRUE;
    }

    return FALSE;
}

BOOL BattleSystem_TriggerFormChange(BattleSystem *battleSys, BattleContext *battleCtx, int *subscript)
{
    int i;
    int arceusForm;
    BOOL result = FALSE;

    for (i = 0; i < BattleSystem_MaxBattlers(battleSys); i++) {
        battleCtx->msgBattlerTemp = battleCtx->monSpeedOrder[i];

        if (battleCtx->battleMons[battleCtx->msgBattlerTemp].species == SPECIES_CASTFORM
                && battleCtx->battleMons[battleCtx->msgBattlerTemp].curHP
                && Battler_Ability(battleCtx, battleCtx->msgBattlerTemp) == ABILITY_FORECAST) {
            if (NO_CLOUD_NINE) {
                if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_CASTFORM) == FALSE
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 != TYPE_FLYING
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 != TYPE_FLYING) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 = TYPE_FLYING;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 = TYPE_FLYING;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 0;
					battleCtx->msgTemp = TYPE_FLYING;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                } else if (WEATHER_IS_SUN
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 != TYPE_FIRE
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 != TYPE_FIRE) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 = TYPE_FIRE;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 = TYPE_FIRE;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 1;
					battleCtx->msgTemp = TYPE_FIRE;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                } else if (WEATHER_IS_RAIN
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 != TYPE_WATER
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 != TYPE_WATER) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 = TYPE_WATER;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 = TYPE_WATER;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 2;
					battleCtx->msgTemp = TYPE_WATER;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                } else if (WEATHER_IS_HAIL
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 != TYPE_ICE
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 != TYPE_ICE) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 = TYPE_ICE;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 = TYPE_ICE;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 3;
					battleCtx->msgTemp = TYPE_ICE;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                } else if (WEATHER_IS_SAND
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 != TYPE_ROCK
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 != TYPE_ROCK) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 = TYPE_ROCK;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 = TYPE_ROCK;
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 4;
					battleCtx->msgTemp = TYPE_ROCK;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                }
            } else if (battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 != TYPE_FLYING
                    && battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 != TYPE_FLYING) {
                battleCtx->battleMons[battleCtx->msgBattlerTemp].type1 = TYPE_FLYING;
                battleCtx->battleMons[battleCtx->msgBattlerTemp].type2 = TYPE_FLYING;
                battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 0;
				battleCtx->msgTemp = TYPE_FLYING;
                *subscript = subscript_form_change;
                result = TRUE;
                break;
            }
        }

        if (battleCtx->battleMons[battleCtx->msgBattlerTemp].species == SPECIES_CHERRIM
                && battleCtx->battleMons[battleCtx->msgBattlerTemp].curHP) {
            if (NO_CLOUD_NINE) {
                if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_CASTFORM) == FALSE
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum == 1) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 0;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                } else if (WEATHER_IS_SUN && battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum == 0) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 1;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                } else if (WEATHER_IS_RAIN && battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum == 1) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 0;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                } else if (WEATHER_IS_HAIL && battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum == 1) {
                    battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 0;
                    *subscript = subscript_form_change;
                    result = TRUE;
                    break;
                }
            } else if (battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum == 1) {
                battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 0;
                *subscript = subscript_form_change;
                result = TRUE;
                break;
            }
        }

        if (battleCtx->battleMons[battleCtx->msgBattlerTemp].species == SPECIES_ARCEUS
                && battleCtx->battleMons[battleCtx->msgBattlerTemp].curHP
                && Battler_Ability(battleCtx, battleCtx->msgBattlerTemp) == ABILITY_MULTITYPE) {
            arceusForm = Pokemon_GetArceusTypeOf(Item_LoadParam(battleCtx->battleMons[battleCtx->msgBattlerTemp].heldItem, ITEM_PARAM_HOLD_EFFECT, HEAP_ID_BATTLE));

            if (battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum != arceusForm) {
                battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = arceusForm;
                *subscript = subscript_form_change;
                result = TRUE;
                break;
            }
        }

        if (battleCtx->battleMons[battleCtx->msgBattlerTemp].species == SPECIES_GIRATINA
                && battleCtx->battleMons[battleCtx->msgBattlerTemp].curHP
                && battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum == 1
                && ((battleCtx->battleMons[battleCtx->msgBattlerTemp].statusVolatile & VOLATILE_CONDITION_TRANSFORM)
                    || ((BattleSystem_BattleStatus(battleSys) & BATTLE_STATUS_DISTORTION) == FALSE
                        && battleCtx->battleMons[battleCtx->msgBattlerTemp].heldItem != ITEM_GRISEOUS_ORB))) {
            if (battleCtx->battleMons[battleCtx->msgBattlerTemp].statusVolatile & VOLATILE_CONDITION_TRANSFORM) {
                Pokemon *mon = Pokemon_New(HEAP_ID_BATTLE);

                int target;
                if (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_DOUBLES) {
                    target = battleCtx->battlerActions[battleCtx->msgBattlerTemp][BATTLE_ACTION_CHOOSE_TARGET];
                } else {
                    target = battleCtx->msgBattlerTemp ^ 1;
                }

                Pokemon_Copy(BattleSystem_PartyPokemon(battleSys, target, battleCtx->selectedPartySlot[target]), mon);
                
                // Don't copy the Griseous Orb
                int tmp = ITEM_NONE;
                Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &tmp);
                
                // Force Giratina-Altered form
                tmp = 0;
                Pokemon_SetValue(mon, MON_DATA_FORM, &tmp);
                Pokemon_SetGiratinaForm(mon);

                battleCtx->battleMons[battleCtx->msgBattlerTemp].attack = Pokemon_GetValue(mon, MON_DATA_ATK, 0);
                battleCtx->battleMons[battleCtx->msgBattlerTemp].defense = Pokemon_GetValue(mon, MON_DATA_DEF, 0);
                battleCtx->battleMons[battleCtx->msgBattlerTemp].speed = Pokemon_GetValue(mon, MON_DATA_SPEED, 0);
                battleCtx->battleMons[battleCtx->msgBattlerTemp].spAttack = Pokemon_GetValue(mon, MON_DATA_SP_ATK, 0);
                battleCtx->battleMons[battleCtx->msgBattlerTemp].spDefense = Pokemon_GetValue(mon, MON_DATA_SP_DEF, 0);
                battleCtx->battleMons[battleCtx->msgBattlerTemp].ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, 0);
                battleCtx->battleMons[battleCtx->msgBattlerTemp].formNum = 0;
                battleCtx->battleStatusMask2 |= SYSCTL_FORM_CHANGE;

                BattleIO_UpdatePartyMon(battleSys, battleCtx, battleCtx->msgBattlerTemp);
                Heap_FreeToHeap(mon);

                *subscript = subscript_form_change;
                result = TRUE;
                break;
            } else {
                *subscript = subscript_giratina_form_change;
                result = TRUE;
                break;
            }
        }
    }

    return result;
}

void BattleSystem_InitPartyOrder(BattleSystem *battleSys, BattleContext *battleCtx)
{
    for (int i = 0; i < BattleSystem_MaxBattlers(battleSys); i++) {
        for (int j = 0; j < MAX_PARTY_SIZE; j++) {
            battleCtx->partyOrder[i][j] = j;
        }

        BattleSystem_SwitchSlots(battleSys, battleCtx, i, battleCtx->selectedPartySlot[i]);
    }
}

void BattleSystem_SwitchSlots(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int partySlot)
{
    // declare C89-style to match
    int i;
    int tmp;
    int targetSlot;
    u32 battleType = BattleSystem_BattleType(battleSys);

    if (((battleType & BATTLE_TYPE_DOUBLES) && (battleType & BATTLE_TYPE_2vs2_TAG) == FALSE)
            || ((battleType & BATTLE_TYPE_TAG) && (BattleSystem_BattlerSlot(battleSys, battler) & BATTLER_TYPE_SOLO_ENEMY) == FALSE)) {
        if (BattleSystem_BattlerSlot(battleSys, battler) == BATTLER_TYPE_PLAYER_SIDE_SLOT_2
                || BattleSystem_BattlerSlot(battleSys, battler) == BATTLER_TYPE_ENEMY_SIDE_SLOT_2) {
            targetSlot = 1;
        } else {
            targetSlot = 0;
        }

        battler &= 1;
    } else {
        targetSlot = 0;
    }

    for (i = 0; i < MAX_PARTY_SIZE; i++) {
        if (battleCtx->partyOrder[battler][i] == partySlot) {
            break;
        }
    }

    tmp = battleCtx->partyOrder[battler][targetSlot];
    battleCtx->partyOrder[battler][targetSlot] = battleCtx->partyOrder[battler][i];
    battleCtx->partyOrder[battler][i] = tmp;
}

typedef struct ItemEffectTypePair {
    u8 itemEffect;
    u8 type;
} ItemEffectTypePair;

static const ItemEffectTypePair sTypeBoostingItems[] = {
    { HOLD_EFFECT_STRENGTHEN_BUG,      TYPE_BUG      },
    { HOLD_EFFECT_STRENGTHEN_STEEL,    TYPE_STEEL    },
    { HOLD_EFFECT_STRENGTHEN_GROUND,   TYPE_GROUND   },
    { HOLD_EFFECT_STRENGTHEN_ROCK,     TYPE_ROCK     },
    { HOLD_EFFECT_STRENGTHEN_GRASS,    TYPE_GRASS    },
    { HOLD_EFFECT_STRENGTHEN_DARK,     TYPE_DARK     },
    { HOLD_EFFECT_STRENGTHEN_FIGHT,    TYPE_FIGHTING },
    { HOLD_EFFECT_STRENGTHEN_ELECTRIC, TYPE_ELECTRIC },
    { HOLD_EFFECT_STRENGTHEN_WATER,    TYPE_WATER    },
    { HOLD_EFFECT_STRENGTHEN_FLYING,   TYPE_FLYING   },
    { HOLD_EFFECT_STRENGTHEN_POISON,   TYPE_POISON   },
    { HOLD_EFFECT_STRENGTHEN_ICE,      TYPE_ICE      },
    { HOLD_EFFECT_STRENGTHEN_GHOST,    TYPE_GHOST    },
    { HOLD_EFFECT_STRENGTHEN_PSYCHIC,  TYPE_PSYCHIC  },
    { HOLD_EFFECT_STRENGTHEN_FIRE,     TYPE_FIRE     },
    { HOLD_EFFECT_STRENGTHEN_DRAGON,   TYPE_DRAGON   },
    { HOLD_EFFECT_STRENGTHEN_NORMAL,   TYPE_NORMAL   },
    { HOLD_EFFECT_ARCEUS_FIRE,         TYPE_FIRE     },
    { HOLD_EFFECT_ARCEUS_WATER,        TYPE_WATER    },
    { HOLD_EFFECT_ARCEUS_ELECTRIC,     TYPE_ELECTRIC },
    { HOLD_EFFECT_ARCEUS_GRASS,        TYPE_GRASS    },
    { HOLD_EFFECT_ARCEUS_ICE,          TYPE_ICE      },
    { HOLD_EFFECT_ARCEUS_FIGHTING,     TYPE_FIGHTING },
    { HOLD_EFFECT_ARCEUS_POISON,       TYPE_POISON   },
    { HOLD_EFFECT_ARCEUS_GROUND,       TYPE_GROUND   },
    { HOLD_EFFECT_ARCEUS_FLYING,       TYPE_FLYING   },
    { HOLD_EFFECT_ARCEUS_PSYCHIC,      TYPE_PSYCHIC  },
    { HOLD_EFFECT_ARCEUS_BUG,          TYPE_BUG      },
    { HOLD_EFFECT_ARCEUS_ROCK,         TYPE_ROCK     },
    { HOLD_EFFECT_ARCEUS_GHOST,        TYPE_GHOST    },
    { HOLD_EFFECT_ARCEUS_DRAGON,       TYPE_DRAGON   },
    { HOLD_EFFECT_ARCEUS_DARK,         TYPE_DARK     },
    { HOLD_EFFECT_ARCEUS_STEEL,        TYPE_STEEL    }
};

static const Fraction sStatStageBoosts[] = {
    { 10, 40 }, // -6
    { 10, 35 }, // -5
    { 10, 30 }, // -4
    { 10, 25 }, // -3
    { 10, 20 }, // -2
    { 10, 15 }, // -1
    { 10, 10 }, // neutral
    { 15, 10 }, // +1
    { 20, 10 }, // +2
    { 25, 10 }, // +3
    { 30, 10 }, // +4
    { 35, 10 }, // +5
    { 40, 10 }, // +6
};

static const u16 sPunchingMoves[] = {
    MOVE_ICE_PUNCH,
    MOVE_FIRE_PUNCH,
    MOVE_THUNDER_PUNCH,
    MOVE_MACH_PUNCH,
    MOVE_FOCUS_PUNCH,
    MOVE_DIZZY_PUNCH,
    MOVE_DYNAMIC_PUNCH,
    MOVE_HAMMER_ARM,
    MOVE_COMET_PUNCH,
    MOVE_METEOR_MASH,
    MOVE_SHADOW_PUNCH,
    MOVE_DRAIN_PUNCH,
    MOVE_BULLET_PUNCH,
	MOVE_GUILLOTINE,
    MOVE_SKY_UPPERCUT,
    MOVE_BEAT_UP
};

static const u16 sBitingMoves[] = {
    MOVE_BITE,
	MOVE_CRUNCH,
	MOVE_FIRE_FANG,
	MOVE_HYPER_FANG,
	MOVE_ICE_FANG,
	MOVE_POISON_FANG,
	MOVE_THUNDER_FANG
};

static const u16 sSlicingMoves[] = {
    MOVE_AERIAL_ACE,
	MOVE_AIR_CUTTER,
	MOVE_AIR_SLASH,
	MOVE_FURY_CUTTER,
	MOVE_LEAF_BLADE,
	MOVE_NIGHT_SLASH,
	MOVE_PSYCHO_CUT,
	MOVE_SLASH,
	MOVE_X_SCISSOR,
	MOVE_CROSS_POISON,
	MOVE_CUT,
	MOVE_RAZOR_LEAF
};

static const u16 sPulseMoves[] = {
    MOVE_AURA_SPHERE,
	MOVE_DARK_PULSE,
	MOVE_DRAGON_PULSE,
	MOVE_WATER_PULSE
};

typedef struct DamageCalcParams {
    u16 species;
    s16 curHP;
    u16 maxHP;
    u16 _padding06;

    int heldItemEffect;
    int heldItemPower;

    u32 statusMask;

    u8 ability;
    u8 gender;
    u8 type1;
    u8 type2;
} DamageCalcParams;

int BattleSystem_CalcPartyMemberMoveDamage(
    BattleSystem *battleSys,
    BattleContext *battleCtx,
    int move,
    u32 sideConditions,
    u32 fieldConditions,
    u16 inPower,
    u8 inType,
    u8 attacker,
    u8 defender,
    u8 criticalMul,
    u8 partyIndicator,
    int partySlot)
{
    // vars have to all be declared C89-style to match
    int i;
    int effect;
    s32 damage = 0;
    s32 stageDivisor = 0;
    u8 moveType;
    u8 moveClass;
    u16 attackStat;
    u16 defenseStat;
    u16 spAttackStat;
    u16 spDefenseStat;
    u16 speedStat;
    u16 defenderSpeedStat;
    s8 attackStage;
    s8 defenseStage;
    s8 spAttackStage;
    s8 spDefenseStage;
    u8 attackerLevel;
    u16 movePower;
    u16 itemTmp;
    u32 battleType;
    u32 moveStatusFlags;
    DamageCalcParams attackerParams;
    DamageCalcParams defenderParams;
    Pokemon *mon;
    u16 cumStatBoosts;
    u16 monHeldItem;
    int terrain;
    int naturePowerMove;
    int rnd;
	u32 effectiveness;
    int multiHitChance;
    int multiHitHits;
    
    mon = BattleSystem_PartyPokemon(battleSys, partyIndicator, partySlot);

    GF_ASSERT(criticalMul == 1 || criticalMul > 1);

    attackStat = Pokemon_GetValue(mon, MON_DATA_ATK, NULL);
    defenseStat = BattleMon_Get(battleCtx, defender, BATTLEMON_DEFENSE, NULL);
    spAttackStat = Pokemon_GetValue(mon, MON_DATA_SP_ATK, NULL);
    spDefenseStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SP_DEFENSE, NULL);
    speedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
    defenderSpeedStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SPEED, NULL);

    attackStage = 0;
    defenseStage = BattleMon_Get(battleCtx, defender, BATTLEMON_DEFENSE_STAGE, NULL) - 6;
    spAttackStage = 0;
    spDefenseStage = BattleMon_Get(battleCtx, defender, BATTLEMON_SP_DEFENSE_STAGE, NULL) - 6;
    attackerLevel = Pokemon_GetValue(mon, MON_DATA_LEVEL, NULL);

    attackerParams.species = Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL);
    defenderParams.species = BattleMon_Get(battleCtx, defender, BATTLEMON_SPECIES, NULL);
    attackerParams.curHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
    defenderParams.curHP = BattleMon_Get(battleCtx, defender, BATTLEMON_CUR_HP, NULL);
    attackerParams.maxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);
    defenderParams.maxHP = BattleMon_Get(battleCtx, defender, BATTLEMON_MAX_HP, NULL);
    // attackerParams.statusMask = BattleMon_Get(battleCtx, attacker, BATTLEMON_STATUS, NULL);
    attackerParams.statusMask = Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL);
    defenderParams.statusMask = BattleMon_Get(battleCtx, defender, BATTLEMON_STATUS, NULL);
    attackerParams.ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    defenderParams.ability = Battler_Ability(battleCtx, defender);
    attackerParams.gender = Pokemon_GetValue(mon, MON_DATA_GENDER, NULL);
    defenderParams.gender = BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL);
    attackerParams.type1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
    defenderParams.type1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL);
    attackerParams.type2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
    defenderParams.type2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL);

    monHeldItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
    attackerParams.heldItemEffect = BattleSystem_GetItemData(battleCtx, monHeldItem, ITEM_PARAM_HOLD_EFFECT);
    attackerParams.heldItemPower = BattleSystem_GetItemData(battleCtx, monHeldItem, ITEM_PARAM_HOLD_EFFECT_PARAM);

    itemTmp = Battler_HeldItem(battleCtx, defender);
    defenderParams.heldItemEffect = BattleSystem_GetItemData(battleCtx, itemTmp, ITEM_PARAM_HOLD_EFFECT);
    defenderParams.heldItemPower = BattleSystem_GetItemData(battleCtx, itemTmp, ITEM_PARAM_HOLD_EFFECT_PARAM);

    battleType = BattleSystem_BattleType(battleSys);

    effect = MOVE_DATA(move).effect;
    moveClass = MOVE_DATA(move).class;
    cumStatBoosts = 0;

    if (inPower != 0){
        movePower = inPower;
    }
    else {
        movePower = MOVE_DATA(move).power;
        // Assign power; prefer the input power (used by variable-power moves, e.g. Gyro Ball)
        if (movePower == 1
            || effect == BATTLE_EFFECT_NATURE_POWER
            || effect == BATTLE_EFFECT_AVERAGE_HP) {
            switch (effect) {
                default:
                    break;

                case BATTLE_EFFECT_BEAT_UP:
                    Pokemon *partyMon;
                    int partyMonSpecies;
                    int partyMonForm;
                    int partyMonLevel;
                    int side;
                    u8 partyMonAbility;
                    u8 partyMonType1;
                    u8 partyMonType2;
                    u8 partyMonInType;
                    u8 partyMonItemEffect;
                    u8 partyMonItemPower;
                    u16 partyMonItem;
                    s32 cumDamage = 0;

                    for (i = 0; i < MAX_PARTY_SIZE; i++) {
                        partyMon = BattleSystem_PartyPokemon(battleSys, attacker, i);
                        partyMonSpecies = Pokemon_GetValue(partyMon, MON_DATA_SPECIES, NULL);
                        partyMonForm = Pokemon_GetValue(partyMon, MON_DATA_FORM, NULL);
                        partyMonLevel = Pokemon_GetValue(partyMon, MON_DATA_LEVEL, NULL);
                        partyMonAbility = Pokemon_GetValue(partyMon, MON_DATA_ABILITY, NULL);
                        partyMonType1 = Pokemon_GetValue(partyMon, MON_DATA_TYPE_1, NULL);
                        partyMonType2 = Pokemon_GetValue(partyMon, MON_DATA_TYPE_2, NULL);
                        partyMonItem = Pokemon_GetValue(partyMon, MON_DATA_HELD_ITEM, NULL);
                        partyMonItemEffect = BattleSystem_GetItemData(battleCtx, partyMonItem, ITEM_PARAM_HOLD_EFFECT);
                        partyMonItemPower = BattleSystem_GetItemData(battleCtx, partyMonItem, ITEM_PARAM_HOLD_EFFECT_PARAM);

                        if (Pokemon_GetValue(partyMon, MON_DATA_CURRENT_HP, NULL) != 0
                        && Pokemon_GetValue(partyMon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                        && Pokemon_GetValue(partyMon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                        && ((Pokemon_GetValue(partyMon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_INCAPACITATED) == FALSE)) {
                            damage = movePower * ((partyMonLevel * 2 / 5) + 2) * PokemonPersonalData_GetFormValue(partyMonSpecies, partyMonForm, MON_DATA_PERSONAL_BASE_ATK);
                            damage /= (50 * PokemonPersonalData_GetFormValue(defenderParams.species, battleCtx->battleMons[defender].formNum, MON_DATA_PERSONAL_BASE_DEF));

                            switch (partyMonAbility) {
                                default:
                                    break;

                                case ABILITY_HUGE_POWER:
                                case ABILITY_PURE_POWER:
                                    damage *= 2;
                                    break;

                                case ABILITY_PLUS:
                                    if (attackerParams.ability == ABILITY_MINUS) {
                                        damage = damage * 3 / 2;
                                    }
                                    break;

                                case ABILITY_FORECAST:
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
                                        damage /= 2;
                                        break;
                                    }
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                                        damage = damage * 3 / 2;
                                        break;
                                    }
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING) {
                                        damage = damage * 3 / 2;
                                        break;
                                    }
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SANDSTORM) {
                                        damage = damage * 3 / 2;
                                        break;
                                    }
                                    break;

                                case ABILITY_SOLAR_POWER:
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                                        damage = damage * 3 / 2;
                                    }
                                    break;

                                case ABILITY_HUSTLE:
                                    damage = damage * 3 / 2;
                                    break;
                            }

                            // Held item effects that directly affect attack stat
                            switch (partyMonItemEffect) {
                                default:
                                    break;

                                case HOLD_EFFECT_PIKA_SPATK_UP:
                                    if (partyMonSpecies == SPECIES_PIKACHU) {
                                        damage *= 2;
                                    }
                                    break;

                                case HOLD_EFFECT_CUBONE_ATK_UP:
                                    if (partyMonSpecies == SPECIES_CUBONE
                                        || partyMonSpecies == SPECIES_MAROWAK) {

                                        damage *= 2;
                                    }
                                    break;
                            }

                            // End of use of direct attack stat usage here
                            damage += 2;

                            // All other abilities
                            switch (partyMonAbility) {
                                default:
                                    break;

                                case ABILITY_GUTS:
                                    if (Pokemon_GetValue(partyMon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY) {
                                        damage = damage * 3 / 2;
                                    }
                                    break;

                                case ABILITY_RIVALRY:
                                    if (defenderParams.gender != GENDER_NONE
                                    && Pokemon_GetValue(partyMon, MON_DATA_GENDER, NULL) == defenderParams.gender) {
                                        damage = damage * 3 / 2;
                                    }
                                    break;

                                case ABILITY_IRON_FIST:
                                    damage = damage * 13 / 10;
                                    break;

                                case ABILITY_TECHNICIAN:
                                    damage = damage * 3 / 2;
                                    break;

                                case ABILITY_NORMALIZE:
                                    partyMonInType = TYPE_NORMAL;
                                    break;
                            }
	
                            // All other held item effects
                            switch (partyMonItemEffect) {
                                default:
                                    break;

                                case HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH:
                                    damage = damage * 6 / 5;
                                    break;

                                case HOLD_EFFECT_CHOICE_ATK:
                                    damage = damage * 3 / 2;
                                    break;

                                case HOLD_EFFECT_STRENGTHEN_DARK:
                                case HOLD_EFFECT_ARCEUS_DARK:
                                    if (partyMonInType == TYPE_DARK) {
                                        damage = damage * (100 + partyMonItemPower) / 100;
                                    }
                                    break;

                                case HOLD_EFFECT_STRENGTHEN_NORMAL:
                                    if (partyMonInType == TYPE_NORMAL) {
                                        damage = damage * (100 + partyMonItemPower) / 100;
                                    }
                                    break;

                                case HOLD_EFFECT_POWER_UP_PHYS:
                                    damage = damage * (100 + partyMonItemPower) / 100;
                                    break;
                            }
							
							if ((battleCtx->battleStatusMask & SYSCTL_IGNORE_TYPE_CHECKS) == FALSE && (partyMonType1 == inType || partyMonType2 == inType)) {
								if (partyMonAbility == ABILITY_ADAPTABILITY) {
									damage *= 2;
								} else {
									damage = damage * 3 / 2;
								}
							}

                            effectiveness = BattleSystem_TypeMatchupMultiplier(inType, DEFENDING_MON.type1, DEFENDING_MON.type2);
	
							damage = damage * effectiveness / 40;
                        }

                        cumDamage += damage;
                    }

                    damage = cumDamage;
                    return damage;
                    break;

                case BATTLE_EFFECT_40_DAMAGE_FLAT:
                    damage = 40;
                    return damage;
                    break;

                case BATTLE_EFFECT_10_DAMAGE_FLAT:
                    damage = 10;
                    return damage;
                    break;

                case BATTLE_EFFECT_LEVEL_DAMAGE_FLAT:
                    damage = attackerLevel;
                    return damage;
                    break;

                case BATTLE_EFFECT_HALVE_HP:
                    damage = defenderParams.curHP / 2;
                    return damage;
                    break;

                case BATTLE_EFFECT_SET_HP_EQUAL_TO_USER:
                    if (defenderParams.curHP > attackerParams.curHP) {
                        damage = defenderParams.curHP - attackerParams.curHP;
                    }
                    else {
                        damage = 0;
                    }
                    return damage;
                    break;

                case BATTLE_EFFECT_POWER_BASED_ON_LOW_SPEED:
                    movePower = 1 + 25 * defenderSpeedStat / speedStat;
                    if (battleCtx->movePower > 150) {
                        battleCtx->movePower = 150;
                    }
                    break;

                case BATTLE_EFFECT_FLING:
                    movePower = BattleSystem_GetItemData(battleCtx, monHeldItem, ITEM_PARAM_FLING_POWER);
                    break;

                case BATTLE_EFFECT_NATURAL_GIFT:
                    movePower = BattleSystem_GetItemData(battleCtx, monHeldItem, ITEM_PARAM_NATURAL_GIFT_POWER);
                    break;

                case BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_STAT_UP:
                    for (i = BATTLE_STAT_HP; i < BATTLE_STAT_MAX; i++) {

                        if (battleCtx->battleMons[defender].statBoosts[i] > 6) {
                            cumStatBoosts += battleCtx->battleMons[defender].statBoosts[i] - 6;
                        }
                    }

                    movePower = 60 + 20 * cumStatBoosts;
                    break;

                case BATTLE_EFFECT_NATURE_POWER:
                    terrain = BattleSystem_Terrain(battleSys);

                    if (terrain > TERRAIN_SPECIAL) {
                        terrain = TERRAIN_SPECIAL;
                    }

                    naturePowerMove = sTerrainMove[terrain];

                    damage = BattleSystem_CalcPartyMemberMoveDamage(
                            battleSys,
                            battleCtx,
                            naturePowerMove,
                            sideConditions,
                            fieldConditions,
                            inPower,
                            inType,
                            attacker,
                            defender,
                            criticalMul,
                            partyIndicator,
                            partySlot);

                    return damage;
                    break;

                case BATTLE_EFFECT_RANDOM_POWER_MAYBE_HEAL:
                    rnd = BattleSystem_RandNext(battleSys) % 256;
                    // 80 percentile
                    if (rnd >= 204) {
                        movePower = 0;
                        break;
                    }
                    // 70 percentile
                    if (rnd >= 178) {
                        movePower = 120;
                        break;
                    }
                    // 40 percentile
                    if (rnd >= 102) {
                        movePower = 80;
                        break;
                    }
                    movePower = 40;
                    break;
                
                case BATTLE_EFFECT_POWER_BASED_ON_FRIENDSHIP:
                    movePower = Pokemon_GetValue(mon, MON_DATA_FRIENDSHIP, NULL) * 2 / 5;
                    break;

                case BATTLE_EFFECT_POWER_BASED_ON_LOW_FRIENDSHIP:
                    movePower = (255 - Pokemon_GetValue(mon, MON_DATA_FRIENDSHIP, NULL)) * 2 / 5;
                    break;

                case BATTLE_EFFECT_INCREASE_POWER_WITH_WEIGHT:
                    for (i = 0; sWeightToPower[i][0] != 0xFFFF; i++) {
                        if (sWeightToPower[i][0] >= battleCtx->battleMons[defender].weight) {
                            break;
                        }
                    }
                    if (sWeightToPower[i][0] != 0xFFFF) {
                        movePower = sWeightToPower[i][1];
                    } else {
                        movePower = 150;
                    }
                    break;

                case BATTLE_EFFECT_AVERAGE_HP:
                    if(defenderParams.curHP > attackerParams.curHP) {
                        movePower = (defenderParams.curHP - attackerParams.curHP) / 2;
                    }
                    else {
                        movePower = 0;
                    }
                    return movePower;
                    break;

                case BATTLE_EFFECT_HIT_IN_3_TURNS:
                    if (moveClass == CLASS_PHYSICAL) {
                        attackStage = battleCtx->fieldConditions.futureSightAttackingStatStage[defender];
                    }
                    if (moveClass == CLASS_SPECIAL) {
                        spAttackStage = battleCtx->fieldConditions.futureSightAttackingStatStage[defender];
                    }
                    break;

                case BATTLE_EFFECT_SPIKES_MULTI_HIT:
                case BATTLE_EFFECT_MULTI_HIT:
                    multiHitChance = BattleSystem_RandNext(battleSys) % 10;
                    multiHitHits = 2;

                    if (multiHitChance < 7) { // 70% chance for 2 or 3 hits
                    multiHitHits += multiHitChance & 1; // 2 or 3 hits
                    }
                    else { // 30% chance for 4 or 5 hits
                        multiHitHits += (multiHitChance & 1) + 2; // 4 or 5 hits
                    }

                    if (attackerParams.heldItemEffect == HOLD_EFFECT_THREE_FOUR_FIVE_DICE) {
                        multiHitHits = (BattleSystem_RandNext(battleSys) % 3) + 3;
                    }
                    if (attackerParams.ability == ABILITY_SKILL_LINK) {
                        multiHitHits = 5;
                    }
                    if (battleCtx->sideConditionsMask[Battler_Side(battleSys, attacker)] & SIDE_CONDITION_LUCKY_CHANT) {
                        if (multiHitHits < 3) {
                            multiHitHits = 3;
                        }
                    }

                    movePower *= multiHitHits;
                    break;

                case BATTLE_EFFECT_HIT_THREE_TIMES:
                    rnd = BattleSystem_RandNext(battleSys) % 10;
                    if (rnd != 0) {
                        movePower += movePower * 2;
                    }

                    rnd = BattleSystem_RandNext(battleSys) % 10;
                    if (rnd != 0) {
                        movePower += movePower * 3;
                    }
                    break;
                    
                case BATTLE_EFFECT_HIT_TWICE:
                    movePower *= 2;
                    break;
            }
        }
    }

    // Attacker ability
    switch (attackerParams.ability) {
        default:
            break;

        case ABILITY_NORMALIZE:
            moveType = TYPE_NORMAL;

            if (inType == TYPE_NORMAL) {
                moveType = MOVE_DATA(move).type;
            }
            else {
                moveType = inType & 0x3F;
            }

            if (moveType == TYPE_NORMAL
            && move != MOVE_STRUGGLE) {
                movePower = movePower * 6 / 5;
            }
            break;

        case ABILITY_TECHNICIAN:
            if (move != MOVE_STRUGGLE
                && movePower <= 60) {
                    movePower = movePower * 3 / 2;
                }
            break;

        case ABILITY_PURE_POWER:
        case ABILITY_HUGE_POWER:
            attackStat *= 2;
            break;

        case ABILITY_SLOW_START:
            attackStat /= 2;
            break;

        case ABILITY_HUSTLE:
            attackStat = attackStat * 3 / 2;
            break;

        case ABILITY_GUTS:
            if (attackerParams.statusMask & MON_CONDITION_FACADE_BOOST) {
                attackStat = attackStat * 3 / 2;
            }
            break;

        case ABILITY_FLARE_BOOST:
            if (attackerParams.statusMask & MON_CONDITION_BURN) {
                spAttackStat = spAttackStat * 3 / 2;
            }
            break;

        case ABILITY_PLUS:
            if (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_OUR_SIDE, attacker, ABILITY_MINUS)) {
            spAttackStat = spAttackStat * 3 / 2;
		    attackStat = attackStat * 3 / 2;
            }
            break;

        case ABILITY_OVERGROW:
            if (moveType == TYPE_GRASS) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower = movePower * 3 / 2;
                }
            }
            break;

        case ABILITY_BLAZE:
            if (moveType == TYPE_FIRE) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower = movePower * 3 / 2;
                }
            }
            break;

        case ABILITY_TORRENT:
            if (moveType == TYPE_WATER) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower = movePower * 3 / 2;
                }
            }
            break;

        case ABILITY_SWARM:
            if (moveType == TYPE_BUG) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower = movePower * 3 / 2;
                }
            }
            break;

        case ABILITY_HEADACHE:
            if (moveType == TYPE_PSYCHIC) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower *= 2;
                }
            }
            break;

        case ABILITY_CHLOROPLAST:
            if (moveType == TYPE_GRASS) {
                movePower = movePower * 3 / 2;
            }
            break;

        case ABILITY_SIMPLE:
            attackStage *= 2;
            if (attackStage < -6) {
                attackStage = -6;
            }
            if (attackStage > 6) {
                attackStage = 6;
            }

            spAttackStage *= 2;
            if (spAttackStage < -6) {
                spAttackStage = -6;
            }
            if (spAttackStage > 6) {
                spAttackStage = 6;
            }
            break;

        case ABILITY_UNAWARE:
            defenseStage = 0;
            spDefenseStage = 0;
            break;

        case ABILITY_RIVALRY:
            if (attackerParams.gender == defenderParams.gender
                && attackerParams.gender != GENDER_NONE) {
                    movePower = movePower * 3 / 2;
		            battleCtx->battleMons[battleCtx->attacker].rivalryFlag = FALSE;
                }
            else {
                battleCtx->battleMons[battleCtx->attacker].rivalryFlag = TRUE;
            }
            break;

        case ABILITY_SHEER_FORCE:
            if (battleCtx->battleMons[battleCtx->attacker].sheerForceFlag == TRUE) {
                movePower = movePower * 13 / 10;
            }
            break;

        case ABILITY_IRON_FIST:
            for (i = 0; i < NELEMS(sPunchingMoves); i++) {
                if (move == sPunchingMoves[i]) {
                    movePower = movePower * 13 / 10;
                    break;
                }
            }
            break;

        case ABILITY_STRONG_JAW:
            for (i = 0; i < NELEMS(sBitingMoves); i++) {
                if (move == sBitingMoves[i]) {
                    movePower = movePower * 3 / 2;
                    break;
                }
            }
            break;

        case ABILITY_SHARPNESS:
            for (i = 0; i < NELEMS(sSlicingMoves); i++) {
                if (sSlicingMoves[i] == move) {
                    movePower = movePower * 3 / 2;
                    break;
                }
            }
            break;

        case ABILITY_MEGA_LAUNCHER:
            for (i = 0; i < NELEMS(sPulseMoves); i++) {
                if (sPulseMoves[i] == move) {
                    movePower = movePower * 3 / 2;
                    break;
                }
            }
            break;

        case ABILITY_TIDAL_FORCE:
            if (fieldConditions & FIELD_CONDITION_GRAVITY) {
                movePower = movePower * 13 / 10;
            }
            break;

        case ABILITY_FORECAST:
            // Castform offensive boosts
		    // Rainy form
            if (NO_CLOUD_NINE) {
		        if (fieldConditions & FIELD_CONDITION_RAINING)
		        {
			        attackStat = attackStat / 2;
			        spAttackStat = spAttackStat * 4 / 3;
		        }
		        // Sunny form
		        if (fieldConditions & FIELD_CONDITION_SUNNY)
		        {
			        attackStat = attackStat * 3 / 2;
			        spAttackStat = spAttackStat * 3 / 2;
		        }
		        // Snowy form
		        if (fieldConditions & FIELD_CONDITION_HAILING)
		        {
			        attackStat = attackStat * 3 / 2;
			        spAttackStat = spAttackStat * 5 / 3;
		        }
		        // Sandy form
		        if ((fieldConditions & FIELD_CONDITION_SANDSTORM) && attackerParams.ability == ABILITY_FORECAST)
		        {
			        attackStat = attackStat * 3 / 2;
			        spAttackStat = spAttackStat * 4 / 3;
		        }
            }
            break;

        case ABILITY_SOLAR_POWER:
            if (NO_CLOUD_NINE) {
                if (fieldConditions & FIELD_CONDITION_SUNNY) {
                    spAttackStat = spAttackStat * 3 / 2;
			        attackStat = attackStat * 3 / 2;
                }
            }
            break;

        case ABILITY_SAND_FORCE:
            if (NO_CLOUD_NINE) {
                if (fieldConditions & FIELD_CONDITION_SANDSTORM) {
                    attackStat = attackStat * 13 / 10;
			        spAttackStat = spAttackStat * 13 / 10;
                }
            }
            break;
    }

    // Defender ignorable ability
    if (PartyMon_IgnorableAbility(battleCtx, mon, defender, defenderParams.ability) == TRUE) {

        switch (defenderParams.ability) {
            default:
                break;

            case ABILITY_MINUS:
                if (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_OUR_SIDE, defender, ABILITY_PLUS)) {
                spDefenseStat = spDefenseStat * 3 / 2;
		        defenseStat = defenseStat * 3 / 2;
                }
                break;

            case ABILITY_THICK_FAT:
                if (moveType == TYPE_FIRE || moveType == TYPE_ICE) {
                    movePower /= 2;
                }
                break;

            case ABILITY_MARVEL_SCALE:
                if (defenderParams.statusMask) {
                    defenseStat = defenseStat * 3 / 2;
                }
                break;

            case ABILITY_HEATPROOF:
                if (moveType == TYPE_FIRE) {
                    movePower /= 2;
                }
                break;

            case ABILITY_DRY_SKIN:
                if (moveType == TYPE_FIRE) {
                    movePower = movePower * 5 / 4;
                }
                break;

            case ABILITY_SIMPLE:
                defenseStage *= 2;
                if (defenseStage < -6) {
                    defenseStage = -6;
                }
                if (defenseStage > 6) {
                    defenseStage = 6;
                }

                spDefenseStage *= 2;
                if (spDefenseStage < -6) {
                    spDefenseStage = -6;
                }
                if (spDefenseStage > 6) {
                    spDefenseStage = 6;
                }
                break;

            case ABILITY_UNAWARE:
                attackStage = 0;
                spAttackStage = 0;
                break;

            case ABILITY_FLOWER_GIFT:
                if (NO_CLOUD_NINE) {
                    if (fieldConditions & FIELD_CONDITION_SUNNY) {
                        defenseStat = defenseStat * 3 / 2;
                        spDefenseStat = spDefenseStat * 3 / 2;
                    }
                }
                break;
        }
    }
    else {
        switch (defenderParams.ability) {
            default:
                break;

            case ABILITY_FORECAST:
            // Castform defensive boosts
                if (NO_CLOUD_NINE) {
                    // Rainy form
		            if (fieldConditions & FIELD_CONDITION_RAINING)
		            {
			            defenseStat = defenseStat * 3 / 2;
			            spDefenseStat = spDefenseStat * 3 / 2;
		            }
                    // Sunny form
                    if (fieldConditions & FIELD_CONDITION_SUNNY)
		            {
			            defenseStat = defenseStat * 3 / 4;
			            spDefenseStat = spDefenseStat * 3 / 4;
		            }
                    // Snowy form
                    if (fieldConditions & FIELD_CONDITION_HAILING)
		            {
			            defenseStat = defenseStat * 3 / 2;
			            spDefenseStat = spDefenseStat * 4 / 3;
		            }
                    // Sandy form
		            if (fieldConditions & FIELD_CONDITION_SANDSTORM)
		            {
			            defenseStat = defenseStat * 5 / 3;
			            spDefenseStat = spDefenseStat * 5 / 3;
		            }
                }
                break;
        }
    }

    // Attacker item
    switch (attackerParams.heldItemEffect) {
        default:
            break;

        case HOLD_EFFECT_CHOICE_ATK:
            attackStat = attackStat * 3 / 2;
            break;

        case HOLD_EFFECT_CHOICE_SPATK:
            spAttackStat = spAttackStat * 3 / 2;
            break;

        case HOLD_EFFECT_LATI_SPECIAL:
            if ((battleType & BATTLE_TYPE_FRONTIER) == FALSE) {
                if (attackerParams.species == SPECIES_LATIOS || attackerParams.species == SPECIES_LATIAS) {
                    spAttackStat = spAttackStat * 3 / 2;
                }
            }
            break;

        case HOLD_EFFECT_CLAMPERL_SPATK:
            if (attackerParams.species == SPECIES_CLAMPERL) {
                spAttackStat *= 2;
            }
            break;

        case HOLD_EFFECT_PIKA_SPATK_UP:
            if (attackerParams.species == SPECIES_PIKACHU) {
                attackStat *= 2;
                spAttackStat *= 2;
            }
            break;

        case HOLD_EFFECT_CUBONE_ATK_UP:
            if (attackerParams.species == SPECIES_CUBONE
                || attackerParams.species == SPECIES_MAROWAK) {

                attackStat *= 2;
            }
            break;

        case HOLD_EFFECT_DIALGA_BOOST:
            if ((moveType == TYPE_DRAGON || moveType == TYPE_STEEL)
                && attackerParams.species == SPECIES_DIALGA) {

                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_PALKIA_BOOST:
            if ((moveType == TYPE_DRAGON || moveType == TYPE_WATER)
                && attackerParams.species == SPECIES_PALKIA) {

                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_GIRATINA_BOOST:
            if ((moveType == TYPE_DRAGON || moveType == TYPE_GHOST)
                && attackerParams.species == SPECIES_GIRATINA) {

                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_POWER_UP_PHYS:
            if (moveClass == CLASS_PHYSICAL) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_POWER_UP_SPEC:
            if (moveClass == CLASS_SPECIAL) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH:
            for (i = 0; i < NELEMS(sPunchingMoves); i++) {
                if (sPunchingMoves[i] == move) {
                    movePower = movePower * 6 / 5;
                    break;
                }
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_BUG:
        case HOLD_EFFECT_ARCEUS_BUG:
            if (moveType == TYPE_BUG) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_STEEL:
        case HOLD_EFFECT_ARCEUS_STEEL:
            if (moveType == TYPE_STEEL) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_GROUND:
        case HOLD_EFFECT_ARCEUS_GROUND:
            if (moveType == TYPE_GROUND) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_ROCK:
        case HOLD_EFFECT_ARCEUS_ROCK:
            if (moveType == TYPE_ROCK) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_GRASS:
        case HOLD_EFFECT_ARCEUS_GRASS:
            if (moveType == TYPE_GRASS) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_DARK:
        case HOLD_EFFECT_ARCEUS_DARK:
            if (moveType == TYPE_DARK) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_FIGHT:
        case HOLD_EFFECT_ARCEUS_FIGHTING:
            if (moveType == TYPE_FIGHTING) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_ELECTRIC:
        case HOLD_EFFECT_ARCEUS_ELECTRIC:
            if (moveType == TYPE_ELECTRIC) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_WATER:
        case HOLD_EFFECT_ARCEUS_WATER:
            if (moveType == TYPE_WATER) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_FLYING:
        case HOLD_EFFECT_ARCEUS_FLYING:
            if (moveType == TYPE_FLYING) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_POISON:
        case HOLD_EFFECT_ARCEUS_POISON:
            if (moveType == TYPE_POISON) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_ICE:
        case HOLD_EFFECT_ARCEUS_ICE:
            if (moveType == TYPE_ICE) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_GHOST:
        case HOLD_EFFECT_ARCEUS_GHOST:
            if (moveType == TYPE_GHOST) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_PSYCHIC:
        case HOLD_EFFECT_ARCEUS_PSYCHIC:
            if (moveType == TYPE_PSYCHIC) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;


        case HOLD_EFFECT_STRENGTHEN_FIRE:
        case HOLD_EFFECT_ARCEUS_FIRE:
            if (moveType == TYPE_FIRE) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_DRAGON:
        case HOLD_EFFECT_ARCEUS_DRAGON:
            if (moveType == TYPE_DRAGON) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_NORMAL:
            if (moveType == TYPE_NORMAL) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;
    }

    // Defender item
    switch (defenderParams.heldItemEffect) {
        default:
            break;

        case HOLD_EFFECT_LATI_SPECIAL:
            if ((battleType & BATTLE_TYPE_FRONTIER) == FALSE) {
                if (attackerParams.species == SPECIES_LATIOS || attackerParams.species == SPECIES_LATIAS) {
                    spDefenseStat = spDefenseStat * 3 / 2;
                }
            }
            break;

        case HOLD_EFFECT_RAISE_SPD_NO_STATUS:
            spDefenseStat = spDefenseStat * 3 / 2;
            break;

        case HOLD_EFFECT_CLAMPERL_SPDEF:
            if (defenderParams.species == SPECIES_CLAMPERL) {
                spDefenseStat *= 2;
            }
            break;

        case HOLD_EFFECT_DITTO_DEF_UP:
            if (defenderParams.species == SPECIES_DITTO) {
                defenseStat *= 2;
            }
            break;
    }

    GF_ASSERT(battleCtx->powerMul >= 10);
    movePower = movePower * battleCtx->powerMul / 10;

    switch (moveType) {
        default:
            break;

        case TYPE_ELECTRIC:
            if (BattleSystem_AnyBattlersWithMoveEffect(battleSys, battleCtx, MOVE_EFFECT_MUD_SPORT)) {
                movePower /= 2;
            }
            break;

        case TYPE_FIRE:
            if (BattleSystem_AnyBattlersWithMoveEffect(battleSys, battleCtx, MOVE_EFFECT_WATER_SPORT)) {
                movePower /= 2;
            }
            break;
    }

    if (battleCtx->turnFlags[attacker].helpingHand) {
        movePower = movePower * 15 / 10;
    }

    // Focus Punch
    if (effect == BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT) {
        defenseStage = 0;
        spDefenseStage = 0;

        if (attackStage < 0) {
            attackStage = 0;
        }
    }

    attackStage += 6;
    defenseStage += 6;
    spAttackStage += 6;
    spDefenseStage += 6;
	
	if (move == MOVE_KNOCK_OFF && DEFENDING_MON.heldItem != ITEM_NONE)
	{
		movePower = movePower * 15 / 10;
	}

    if (NO_CLOUD_NINE) {

        if ((fieldConditions & FIELD_CONDITION_SANDSTORM)
                && (defenderParams.type1 == TYPE_ROCK || defenderParams.type2 == TYPE_ROCK)) {
            spDefenseStat = spDefenseStat * 15 / 10;
        }

        if ((fieldConditions & FIELD_CONDITION_SUNNY)
                && BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_OUR_SIDE, attacker, ABILITY_FLOWER_GIFT)
				&& attackerParams.ability != ABILITY_FLOWER_GIFT) {
            attackStat = attackStat * 15 / 10;
			spAttackStat = spAttackStat * 15 / 10;
        }
    }

    //if (MOVE_DATA(move).effect == BATTLE_EFFECT_HALVE_DEFENSE) {
    //    defenseStat = defenseStat / 2;
    //}

    if (moveClass == CLASS_PHYSICAL) {
        if (criticalMul > 1) {
            if (attackStage > 6) {
                damage = attackStat * sStatStageBoosts[attackStage].numerator;
                damage /= sStatStageBoosts[attackStage].denominator;
            } else {
                damage = attackStat;
            }
        } else {
            damage = attackStat * sStatStageBoosts[attackStage].numerator;
            damage /= sStatStageBoosts[attackStage].denominator;
        }

        damage *= movePower;
        damage *= (attackerLevel * 2 / 5 + 2);

        if (criticalMul > 1) {
            if (defenseStage < 6) {
                stageDivisor = defenseStat * sStatStageBoosts[defenseStage].numerator;
                stageDivisor /= sStatStageBoosts[defenseStage].denominator;
            } else {
                stageDivisor = defenseStat;
            }
        } else {
            stageDivisor = defenseStat * sStatStageBoosts[defenseStage].numerator;
            stageDivisor /= sStatStageBoosts[defenseStage].denominator;
        }

        damage /= stageDivisor;
        damage /= 50;

        if ((attackerParams.statusMask & MON_CONDITION_BURN) && (attackerParams.ability != ABILITY_GUTS) && (battleCtx->moveCur != MOVE_FACADE)) {
			damage /= 2;
		}

        if ((sideConditions & SIDE_CONDITION_REFLECT) != FALSE
                && criticalMul == 1
                && effect != BATTLE_EFFECT_REMOVE_SCREENS
                && effect != BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT) {
            if ((battleType & BATTLE_TYPE_DOUBLES)
                    && BattleSystem_CountAliveBattlers(battleSys, battleCtx, TRUE, defender) == 2) {
                damage = damage * 2 / 3;
            } else {
                damage /= 2;
            }
        }
    } else if (moveClass == CLASS_SPECIAL) {
        if (criticalMul > 1) {
            if (spAttackStage > 6) {
                damage = spAttackStat * sStatStageBoosts[spAttackStage].numerator;
                damage /= sStatStageBoosts[spAttackStage].denominator;
            } else {
                damage = spAttackStat;
            }
        } else {
            damage = spAttackStat * sStatStageBoosts[spAttackStage].numerator;
            damage /= sStatStageBoosts[spAttackStage].denominator;
        }

        damage *= movePower;
        damage *= (attackerLevel * 2 / 5 + 2);

        if (criticalMul > 1) {
            if (spDefenseStage < 6) {
                stageDivisor = spDefenseStat * sStatStageBoosts[spDefenseStage].numerator;
                stageDivisor /= sStatStageBoosts[spDefenseStage].denominator;
            } else {
                stageDivisor = spDefenseStat;
            }
        } else {
            stageDivisor = spDefenseStat * sStatStageBoosts[spDefenseStage].numerator;
            stageDivisor /= sStatStageBoosts[spDefenseStage].denominator;
        }
		
		if ((move == MOVE_HYDRO_CANNON)
		|| (move == MOVE_KINESIS))
		{
				if (criticalMul > 1) {
				if (defenseStage < 6) {
					stageDivisor = defenseStat * sStatStageBoosts[defenseStage].numerator;
					stageDivisor /= sStatStageBoosts[defenseStage].denominator;
				} else {
					stageDivisor = defenseStat;
				}
			} else {
				stageDivisor = defenseStat * sStatStageBoosts[defenseStage].numerator;
				stageDivisor /= sStatStageBoosts[defenseStage].denominator;
			}
		}

        damage /= stageDivisor;
        damage /= 50;

        if ((sideConditions & SIDE_CONDITION_LIGHT_SCREEN) != FALSE
                && criticalMul == 1
                && MOVE_DATA(move).effect != BATTLE_EFFECT_REMOVE_SCREENS) {
            if ((battleType & BATTLE_TYPE_DOUBLES)
                    && BattleSystem_CountAliveBattlers(battleSys, battleCtx, TRUE, defender) == 2) {
                damage = damage * 2 / 3;
            } else {
                damage /= 2;
            }
        }
    }

    if (battleType & BATTLE_TYPE_DOUBLES) {

        if (MOVE_DATA(move).range == RANGE_ADJACENT_OPPONENTS
            && BattleSystem_CountAliveBattlers(battleSys, battleCtx, TRUE, defender) >= 2)
        {
            damage = damage * 3 / 4;
        }

        if (MOVE_DATA(move).range == RANGE_ALL_ADJACENT
            && BattleSystem_CountAliveBattlers(battleSys, battleCtx, FALSE, defender) >= 2)
        {
            damage = damage * 3/4;
        }
    }

    if (NO_CLOUD_NINE) {
        if (fieldConditions & FIELD_CONDITION_RAINING) {
            switch (moveType) {
            case TYPE_FIRE:
                damage /= 2;
                break;
            case TYPE_WATER:
                damage = damage * 15 / 10;
                break;
            }
        }

        if ((fieldConditions & FIELD_CONDITION_SOLAR_DOWN) && move == MOVE_SOLAR_BEAM) {
            damage /= 2;
        }

        if (fieldConditions & FIELD_CONDITION_SUNNY) {
            switch (moveType) {
            case TYPE_FIRE:
                damage = damage * 15 / 10;
                break;
            case TYPE_WATER:
                damage /= 2;
                break;
            }
        }
    }

    return damage + 2;
}

int BattleSystem_CalcMoveDamage(BattleSystem *battleSys,
    BattleContext *battleCtx,
    int move,
    u32 sideConditions,
    u32 fieldConditions,
    u16 inPower,
    u8 inType,
    u8 attacker,
    u8 defender,
    u8 criticalMul)
{
    // vars have to all be declared C89-style to match
    int i;
    int effect;
    s32 damage = 0;
    s32 stageDivisor = 0;
    u8 moveType;
    u8 moveClass;
    u16 attackStat;
    u16 defenseStat;
    u16 spAttackStat;
    u16 spDefenseStat;
    s8 attackStage;
    s8 defenseStage;
    s8 spAttackStage;
    s8 spDefenseStage;
    u8 attackerLevel;
    u16 movePower;
    u16 itemTmp;
    u32 battleType;
    u32 moveStatusFlags;
    DamageCalcParams attackerParams;
    DamageCalcParams defenderParams;
    u16 cumStatBoosts;
    int terrain;
    int naturePowerMove;
    int rnd;
	u32 effectiveness;

    GF_ASSERT(criticalMul == 1 || criticalMul > 1);

    attackStat = BattleMon_Get(battleCtx, attacker, BATTLEMON_ATTACK, NULL);
    defenseStat = BattleMon_Get(battleCtx, defender, BATTLEMON_DEFENSE, NULL);
    spAttackStat = BattleMon_Get(battleCtx, attacker, BATTLEMON_SP_ATTACK, NULL);
    spDefenseStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SP_DEFENSE, NULL);
    attackStage = BattleMon_Get(battleCtx, attacker, BATTLEMON_ATTACK_STAGE, NULL) - 6;
    defenseStage = BattleMon_Get(battleCtx, defender, BATTLEMON_DEFENSE_STAGE, NULL) - 6;
    spAttackStage = BattleMon_Get(battleCtx, attacker, BATTLEMON_SP_ATTACK_STAGE, NULL) - 6;
    spDefenseStage = BattleMon_Get(battleCtx, defender, BATTLEMON_SP_DEFENSE_STAGE, NULL) - 6;
    attackerLevel = BattleMon_Get(battleCtx, attacker, BATTLEMON_LEVEL, NULL);

    attackerParams.species = BattleMon_Get(battleCtx, attacker, BATTLEMON_SPECIES, NULL);
    defenderParams.species = BattleMon_Get(battleCtx, defender, BATTLEMON_SPECIES, NULL);
    attackerParams.curHP = BattleMon_Get(battleCtx, attacker, BATTLEMON_CUR_HP, NULL);
    defenderParams.curHP = BattleMon_Get(battleCtx, defender, BATTLEMON_CUR_HP, NULL);
    attackerParams.maxHP = BattleMon_Get(battleCtx, attacker, BATTLEMON_MAX_HP, NULL);
    defenderParams.maxHP = BattleMon_Get(battleCtx, defender, BATTLEMON_MAX_HP, NULL);
    attackerParams.statusMask = BattleMon_Get(battleCtx, attacker, BATTLEMON_STATUS, NULL);
    defenderParams.statusMask = BattleMon_Get(battleCtx, defender, BATTLEMON_STATUS, NULL);
    attackerParams.ability = Battler_Ability(battleCtx, attacker);
    defenderParams.ability = Battler_Ability(battleCtx, defender);
    attackerParams.gender = BattleMon_Get(battleCtx, attacker, BATTLEMON_GENDER, NULL);
    defenderParams.gender = BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL);
    attackerParams.type1 = BattleMon_Get(battleCtx, attacker, BATTLEMON_TYPE_1, NULL);
    defenderParams.type1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL);
    attackerParams.type2 = BattleMon_Get(battleCtx, attacker, BATTLEMON_TYPE_2, NULL);
    defenderParams.type2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL);

    itemTmp = Battler_HeldItem(battleCtx, attacker);
    attackerParams.heldItemEffect = BattleSystem_GetItemData(battleCtx, itemTmp, ITEM_PARAM_HOLD_EFFECT);
    attackerParams.heldItemPower = BattleSystem_GetItemData(battleCtx, itemTmp, ITEM_PARAM_HOLD_EFFECT_PARAM);

    itemTmp = Battler_HeldItem(battleCtx, defender);
    defenderParams.heldItemEffect = BattleSystem_GetItemData(battleCtx, itemTmp, ITEM_PARAM_HOLD_EFFECT);
    defenderParams.heldItemPower = BattleSystem_GetItemData(battleCtx, itemTmp, ITEM_PARAM_HOLD_EFFECT_PARAM);

    battleType = BattleSystem_BattleType(battleSys);

    effect = MOVE_DATA(move).effect;
    moveClass = MOVE_DATA(move).class;
    cumStatBoosts = 0;

    if (inPower != 0){
        movePower = inPower;
    }
    else {
        movePower = MOVE_DATA(move).power;
        // Assign power; prefer the input power (used by variable-power moves, e.g. Gyro Ball)
        if (movePower == 1
            || effect == BATTLE_EFFECT_NATURE_POWER
            || effect == BATTLE_EFFECT_AVERAGE_HP) {
            switch (effect) {
                default:
                    break;

                case BATTLE_EFFECT_BEAT_UP:
                    Pokemon *partyMon;
                    int partyMonSpecies;
                    int partyMonForm;
                    int partyMonLevel;
                    int side;
                    u8 partyMonAbility;
                    u8 partyMonType1;
                    u8 partyMonType2;
                    u8 partyMonInType;
                    u8 partyMonItemEffect;
                    u8 partyMonItemPower;
                    u16 partyMonItem;
                    s32 cumDamage = 0;

                    for (i = 0; i < MAX_PARTY_SIZE; i++) {
                        partyMon = BattleSystem_PartyPokemon(battleSys, attacker, i);
                        partyMonSpecies = Pokemon_GetValue(partyMon, MON_DATA_SPECIES, NULL);
                        partyMonForm = Pokemon_GetValue(partyMon, MON_DATA_FORM, NULL);
                        partyMonLevel = Pokemon_GetValue(partyMon, MON_DATA_LEVEL, NULL);
                        partyMonAbility = Pokemon_GetValue(partyMon, MON_DATA_ABILITY, NULL);
                        partyMonType1 = Pokemon_GetValue(partyMon, MON_DATA_TYPE_1, NULL);
                        partyMonType2 = Pokemon_GetValue(partyMon, MON_DATA_TYPE_2, NULL);
                        partyMonItem = Pokemon_GetValue(partyMon, MON_DATA_HELD_ITEM, NULL);
                        partyMonItemEffect = BattleSystem_GetItemData(battleCtx, partyMonItem, ITEM_PARAM_HOLD_EFFECT);
                        partyMonItemPower = BattleSystem_GetItemData(battleCtx, partyMonItem, ITEM_PARAM_HOLD_EFFECT_PARAM);

                        if (Pokemon_GetValue(partyMon, MON_DATA_CURRENT_HP, NULL) != 0
                        && Pokemon_GetValue(partyMon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_NONE
                        && Pokemon_GetValue(partyMon, MON_DATA_SPECIES_EGG, NULL) != SPECIES_EGG
                        && ((Pokemon_GetValue(partyMon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_INCAPACITATED) == FALSE)) {
                            damage = movePower * ((partyMonLevel * 2 / 5) + 2) * PokemonPersonalData_GetFormValue(partyMonSpecies, partyMonForm, MON_DATA_PERSONAL_BASE_ATK);
                            damage /= (50 * PokemonPersonalData_GetFormValue(defenderParams.species, battleCtx->battleMons[defender].formNum, MON_DATA_PERSONAL_BASE_DEF));

                            switch (partyMonAbility) {
                                default:
                                    break;

                                case ABILITY_HUGE_POWER:
                                case ABILITY_PURE_POWER:
                                    damage *= 2;
                                    break;

                                case ABILITY_PLUS:
                                    if (attackerParams.ability == ABILITY_MINUS) {
                                        damage = damage * 3 / 2;
                                    }
                                    break;

                                case ABILITY_FORECAST:
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
                                        damage /= 2;
                                        break;
                                    }
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                                        damage = damage * 3 / 2;
                                        break;
                                    }
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_HAILING) {
                                        damage = damage * 3 / 2;
                                        break;
                                    }
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SANDSTORM) {
                                        damage = damage * 3 / 2;
                                        break;
                                    }
                                    break;

                                case ABILITY_SOLAR_POWER:
                                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                                        damage = damage * 3 / 2;
                                    }
                                    break;

                                case ABILITY_HUSTLE:
                                    damage = damage * 3 / 2;
                                    break;
                            }

                            // Held item effects that directly affect attack stat
                            switch (partyMonItemEffect) {
                                default:
                                    break;

                                case HOLD_EFFECT_PIKA_SPATK_UP:
                                    if (partyMonSpecies == SPECIES_PIKACHU) {
                                        damage *= 2;
                                    }
                                    break;

                                case HOLD_EFFECT_CUBONE_ATK_UP:
                                    if (partyMonSpecies == SPECIES_CUBONE
                                        || partyMonSpecies == SPECIES_MAROWAK) {

                                        damage *= 2;
                                    }
                                    break;
                            }

                            // End of use of direct attack stat usage here
                            damage += 2;

                            // All other abilities
                            switch (partyMonAbility) {
                                default:
                                    break;

                                case ABILITY_GUTS:
                                    if (Pokemon_GetValue(partyMon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY) {
                                        damage = damage * 3 / 2;
                                    }
                                    break;

                                case ABILITY_RIVALRY:
                                    if (defenderParams.gender != GENDER_NONE
                                    && Pokemon_GetValue(partyMon, MON_DATA_GENDER, NULL) == defenderParams.gender) {
                                        damage = damage * 3 / 2;
                                    }
                                    break;

                                case ABILITY_IRON_FIST:
                                    damage = damage * 13 / 10;
                                    break;

                                case ABILITY_TECHNICIAN:
                                    damage = damage * 3 / 2;
                                    break;

                                case ABILITY_NORMALIZE:
                                    partyMonInType = TYPE_NORMAL;
                                    break;
                            }
	
                            // All other held item effects
                            switch (partyMonItemEffect) {
                                default:
                                    break;

                                case HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH:
                                    damage = damage * 6 / 5;
                                    break;

                                case HOLD_EFFECT_CHOICE_ATK:
                                    damage = damage * 3 / 2;
                                    break;

                                case HOLD_EFFECT_STRENGTHEN_DARK:
                                case HOLD_EFFECT_ARCEUS_DARK:
                                    if (partyMonInType == TYPE_DARK) {
                                        damage = damage * (100 + partyMonItemPower) / 100;
                                    }
                                    break;

                                case HOLD_EFFECT_STRENGTHEN_NORMAL:
                                    if (partyMonInType == TYPE_NORMAL) {
                                        damage = damage * (100 + partyMonItemPower) / 100;
                                    }
                                    break;

                                case HOLD_EFFECT_POWER_UP_PHYS:
                                    damage = damage * (100 + partyMonItemPower) / 100;
                                    break;
                            }

                            damage = PartyMon_ApplyTypeChart(battleSys,
                                                battleCtx,
                                                move,
                                                partyMonInType,
                                                attacker,
                                                defender,
                                                damage,
                                                attacker,
                                                i,
                                                &effectiveness);

                            if ((effectiveness & MOVE_STATUS_IMMUNE)
                                && ((effectiveness & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE))
                            {
                                    damage = 0;
                            }
                        }

                        cumDamage += damage;
                    }

                    damage = cumDamage;
                    return damage;
                    break;


                case BATTLE_EFFECT_40_DAMAGE_FLAT:
                    damage = 40;
                    return damage;
                    break;

                case BATTLE_EFFECT_10_DAMAGE_FLAT:
                    damage = 10;
                    return damage;
                    break;

                case BATTLE_EFFECT_LEVEL_DAMAGE_FLAT:
                    damage = attackerLevel;
                    return damage;
                    break;

                case BATTLE_EFFECT_HALVE_HP:
                    damage = battleCtx->battleMons[defender].curHP / 2;
                    return damage;
                    break;

                case BATTLE_EFFECT_SET_HP_EQUAL_TO_USER:
                    if (defenderParams.curHP > attackerParams.curHP) {
                        damage = defenderParams.curHP - attackerParams.curHP;
                    }
                    else {
                        damage = 0;
                    }
                    return damage;
                    break;

                case BATTLE_EFFECT_POWER_BASED_ON_LOW_SPEED:
                    movePower = 1 + 25 * battleCtx->monSpeedValues[battleCtx->defender] / battleCtx->monSpeedValues[battleCtx->attacker];
                    if (battleCtx->movePower > 150) {
                        battleCtx->movePower = 150;
                    }
                    break;

                case BATTLE_EFFECT_FLING:
                    movePower = Battler_ItemFlingPower(battleCtx, attacker);
                    break;

                case BATTLE_EFFECT_NATURAL_GIFT:
                    movePower = Battler_NaturalGiftPower(battleCtx, attacker);
                    break;

                case BATTLE_EFFECT_INCREASE_POWER_WITH_MORE_STAT_UP:
                    for (i = BATTLE_STAT_HP; i < BATTLE_STAT_MAX; i++) {

                        if (battleCtx->battleMons[defender].statBoosts[i] > 6) {
                            cumStatBoosts += battleCtx->battleMons[defender].statBoosts[i] - 6;
                        }
                    }

                    movePower = 60 + 20 * cumStatBoosts;
                    break;

                case BATTLE_EFFECT_NATURE_POWER:
                    terrain = BattleSystem_Terrain(battleSys);

                    if (terrain > TERRAIN_SPECIAL) {
                        terrain = TERRAIN_SPECIAL;
                    }

                    naturePowerMove = sTerrainMove[terrain];

                    damage = BattleSystem_CalcMoveDamage(battleSys,
                            battleCtx,
                            naturePowerMove,
                            sideConditions,
                            fieldConditions,
                            inPower,
                            inType,
                            attacker,
                            defender,
                            criticalMul);

                    return damage;
                    break;

                case BATTLE_EFFECT_RANDOM_POWER_MAYBE_HEAL:
                    rnd = BattleSystem_RandNext(battleSys) % 256;
                    // 80 percentile
                    if (rnd >= 204) {
                        movePower = 0;
                        break;
                    }
                    // 70 percentile
                    if (rnd >= 178) {
                        movePower = 120;
                        break;
                    }
                    // 40 percentile
                    if (rnd >= 102) {
                        movePower = 80;
                        break;
                    }
                    movePower = 40;
                    break;
                
                case BATTLE_EFFECT_POWER_BASED_ON_FRIENDSHIP:
                    movePower = battleCtx->battleMons[attacker].friendship * 2 / 5;
                    break;

                case BATTLE_EFFECT_POWER_BASED_ON_LOW_FRIENDSHIP:
                    movePower = (255 - battleCtx->battleMons[attacker].friendship) * 2 / 5;
                    break;

                case BATTLE_EFFECT_INCREASE_POWER_WITH_WEIGHT:
                    for (i = 0; sWeightToPower[i][0] != 0xFFFF; i++) {
                        if (sWeightToPower[i][0] >= battleCtx->battleMons[defender].weight) {
                            break;
                        }
                    }
                    if (sWeightToPower[i][0] != 0xFFFF) {
                        movePower = sWeightToPower[i][1];
                    } else {
                        movePower = 150;
                    }
                    break;

                case BATTLE_EFFECT_AVERAGE_HP:
                    if(defenderParams.curHP > attackerParams.curHP) {
                        movePower = (defenderParams.curHP - attackerParams.curHP) / 2;
                    }
                    else {
                        movePower = 0;
                    }
                    return movePower;
                    break;

                case BATTLE_EFFECT_HIT_IN_3_TURNS:
                    if (moveClass == CLASS_PHYSICAL) {
                        attackStage = battleCtx->fieldConditions.futureSightAttackingStatStage[defender];
                    }
                    if (moveClass == CLASS_SPECIAL) {
                        spAttackStage = battleCtx->fieldConditions.futureSightAttackingStatStage[defender];
                    }
                    break;
            }
        }
    }

    // Attacker ability
    switch (attackerParams.ability) {
        default:
            break;

        case ABILITY_NORMALIZE:
            moveType = TYPE_NORMAL;

            if (inType == TYPE_NORMAL) {
                moveType = MOVE_DATA(move).type;
            }
            else {
                moveType = inType & 0x3F;
            }

            if (moveType == TYPE_NORMAL
            && move != MOVE_STRUGGLE) {
                movePower = movePower * 6 / 5;
            }
            break;

        case ABILITY_TECHNICIAN:
            if (move != MOVE_STRUGGLE
                && movePower <= 60) {
                    movePower = movePower * 3 / 2;
                }
            break;

        case ABILITY_PURE_POWER:
        case ABILITY_HUGE_POWER:
            attackStat *= 2;
            break;

        case ABILITY_SLOW_START:
            if (BattleContext_Get(battleSys, battleCtx, BATTLECTX_TOTAL_TURNS, NULL)
                - BattleMon_Get(battleCtx, attacker, BATTLEMON_SLOW_START_TURN_NUMBER, NULL)
                < 5)
            {
                    attackStat /= 2;
            }
            break;

        case ABILITY_HUSTLE:
            attackStat = attackStat * 3 / 2;
            break;

        case ABILITY_GUTS:
            if (attackerParams.statusMask) {
                attackStat = attackStat * 3 / 2;
            }
            break;

        case ABILITY_FLARE_BOOST:
            if (attackerParams.statusMask & MON_CONDITION_BURN) {
                spAttackStat = spAttackStat * 3 / 2;
            }
            break;

        case ABILITY_PLUS:
            if (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_OUR_SIDE, attacker, ABILITY_MINUS)) {
            spAttackStat = spAttackStat * 3 / 2;
		    attackStat = attackStat * 3 / 2;
            }
            break;

        case ABILITY_OVERGROW:
            if (moveType == TYPE_GRASS) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower = movePower * 3 / 2;
                }
            }
            break;

        case ABILITY_BLAZE:
            if (moveType == TYPE_FIRE) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower = movePower * 3 / 2;
                }
            }
            break;

        case ABILITY_TORRENT:
            if (moveType == TYPE_WATER) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower = movePower * 3 / 2;
                }
            }
            break;

        case ABILITY_SWARM:
            if (moveType == TYPE_BUG) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower = movePower * 3 / 2;
                }
            }
            break;

        case ABILITY_HEADACHE:
            if (moveType == TYPE_PSYCHIC) {
                if (attackerParams.curHP <= (attackerParams.maxHP / 2)) {
                    movePower *= 2;
                }
            }
            break;

        case ABILITY_CHLOROPLAST:
            if (moveType == TYPE_GRASS) {
                movePower = movePower * 3 / 2;
            }
            break;

        case ABILITY_SIMPLE:
            attackStage *= 2;
            if (attackStage < -6) {
                attackStage = -6;
            }
            if (attackStage > 6) {
                attackStage = 6;
            }

            spAttackStage *= 2;
            if (spAttackStage < -6) {
                spAttackStage = -6;
            }
            if (spAttackStage > 6) {
                spAttackStage = 6;
            }
            break;

        case ABILITY_UNAWARE:
            defenseStage = 0;
            spDefenseStage = 0;
            break;

        case ABILITY_RIVALRY:
            if (attackerParams.gender == defenderParams.gender
                && attackerParams.gender != GENDER_NONE) {
                    movePower = movePower * 3 / 2;
		            battleCtx->battleMons[battleCtx->attacker].rivalryFlag = FALSE;
                }
            else {
                battleCtx->battleMons[battleCtx->attacker].rivalryFlag = TRUE;
            }
            break;

        case ABILITY_SHEER_FORCE:
            if (battleCtx->battleMons[battleCtx->attacker].sheerForceFlag == TRUE) {
                movePower = movePower * 13 / 10;
            }
            break;

        case ABILITY_IRON_FIST:
            for (i = 0; i < NELEMS(sPunchingMoves); i++) {
                if (move == sPunchingMoves[i]) {
                    movePower = movePower * 13 / 10;
                    break;
                }
            }
            break;

        case ABILITY_STRONG_JAW:
            for (i = 0; i < NELEMS(sBitingMoves); i++) {
                if (move == sBitingMoves[i]) {
                    movePower = movePower * 3 / 2;
                    break;
                }
            }
            break;

        case ABILITY_SHARPNESS:
            for (i = 0; i < NELEMS(sSlicingMoves); i++) {
                if (sSlicingMoves[i] == move) {
                    movePower = movePower * 3 / 2;
                    break;
                }
            }
            break;

        case ABILITY_MEGA_LAUNCHER:
            for (i = 0; i < NELEMS(sPulseMoves); i++) {
                if (sPulseMoves[i] == move) {
                    movePower = movePower * 3 / 2;
                    break;
                }
            }
            break;

        case ABILITY_TIDAL_FORCE:
            if (fieldConditions & FIELD_CONDITION_GRAVITY) {
                movePower = movePower * 13 / 10;
            }
            break;

        case ABILITY_FORECAST:
            // Castform offensive boosts
		    // Rainy form
            if (NO_CLOUD_NINE) {
		        if (fieldConditions & FIELD_CONDITION_RAINING)
		        {
			        attackStat = attackStat / 2;
			        spAttackStat = spAttackStat * 4 / 3;
		        }
		        // Sunny form
		        if (fieldConditions & FIELD_CONDITION_SUNNY)
		        {
			        attackStat = attackStat * 3 / 2;
			        spAttackStat = spAttackStat * 3 / 2;
		        }
		        // Snowy form
		        if (fieldConditions & FIELD_CONDITION_HAILING)
		        {
			        attackStat = attackStat * 3 / 2;
			        spAttackStat = spAttackStat * 5 / 3;
		        }
		        // Sandy form
		        if ((fieldConditions & FIELD_CONDITION_SANDSTORM) && attackerParams.ability == ABILITY_FORECAST)
		        {
			        attackStat = attackStat * 3 / 2;
			        spAttackStat = spAttackStat * 4 / 3;
		        }
            }
            break;

        case ABILITY_SOLAR_POWER:
            if (NO_CLOUD_NINE) {
                if (fieldConditions & FIELD_CONDITION_SUNNY) {
                    spAttackStat = spAttackStat * 3 / 2;
			        attackStat = attackStat * 3 / 2;
                }
            }
            break;

        case ABILITY_SAND_FORCE:
            if (NO_CLOUD_NINE) {
                if (fieldConditions & FIELD_CONDITION_SANDSTORM) {
                    attackStat = attackStat * 13 / 10;
			        spAttackStat = spAttackStat * 13 / 10;
                }
            }
            break;

        // Flash fire will no longer apply when you don't have flash fire ability
        case ABILITY_FLASH_FIRE:
            if (BattleMon_Get(battleCtx, attacker, BATTLEMON_FLASH_FIRE, NULL) && moveType == TYPE_FIRE) {
                movePower = movePower * 3 / 2;
            }
            break;
    }

    // Defender ignorable ability
    if (Battler_IgnorableAbility(battleCtx, attacker, defender, defenderParams.ability) == TRUE) {

        switch (defenderParams.ability) {
            default:
                break;

            case ABILITY_MINUS:
                if (BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_OUR_SIDE, defender, ABILITY_PLUS)) {
                spDefenseStat = spDefenseStat * 3 / 2;
		        defenseStat = defenseStat * 3 / 2;
                }
                break;

            case ABILITY_THICK_FAT:
                if (moveType == TYPE_FIRE || moveType == TYPE_ICE) {
                    movePower /= 2;
                }
                break;

            case ABILITY_MARVEL_SCALE:
                if (defenderParams.statusMask) {
                    defenseStat = defenseStat * 3 / 2;
                }
                break;

            case ABILITY_HEATPROOF:
                if (moveType == TYPE_FIRE) {
                    movePower /= 2;
                }
                break;

            case ABILITY_DRY_SKIN:
                if (moveType == TYPE_FIRE) {
                    movePower = movePower * 5 / 4;
                }
                break;

            case ABILITY_SIMPLE:
                defenseStage *= 2;
                if (defenseStage < -6) {
                    defenseStage = -6;
                }
                if (defenseStage > 6) {
                    defenseStage = 6;
                }

                spDefenseStage *= 2;
                if (spDefenseStage < -6) {
                    spDefenseStage = -6;
                }
                if (spDefenseStage > 6) {
                    spDefenseStage = 6;
                }
                break;

            case ABILITY_UNAWARE:
                attackStage = 0;
                spAttackStage = 0;
                break;

            case ABILITY_FLOWER_GIFT:
                if (NO_CLOUD_NINE) {
                    if (fieldConditions & FIELD_CONDITION_SUNNY) {
                        defenseStat = defenseStat * 3 / 2;
                        spDefenseStat = spDefenseStat * 3 / 2;
                    }
                }
                break;
        }
    }
    else {
        switch (defenderParams.ability) {
            default:
                break;

            case ABILITY_FORECAST:
            // Castform defensive boosts
                if (NO_CLOUD_NINE) {
                    // Rainy form
		            if (fieldConditions & FIELD_CONDITION_RAINING)
		            {
			            defenseStat = defenseStat * 3 / 2;
			            spDefenseStat = spDefenseStat * 3 / 2;
		            }
                    // Sunny form
                    if (fieldConditions & FIELD_CONDITION_SUNNY)
		            {
			            defenseStat = defenseStat * 3 / 4;
			            spDefenseStat = spDefenseStat * 3 / 4;
		            }
                    // Snowy form
                    if (fieldConditions & FIELD_CONDITION_HAILING)
		            {
			            defenseStat = defenseStat * 3 / 2;
			            spDefenseStat = spDefenseStat * 4 / 3;
		            }
                    // Sandy form
		            if (fieldConditions & FIELD_CONDITION_SANDSTORM)
		            {
			            defenseStat = defenseStat * 5 / 3;
			            spDefenseStat = spDefenseStat * 5 / 3;
		            }
                }
                break;
        }
    }

    // Attacker item
    switch (attackerParams.heldItemEffect) {
        default:
            break;

        case HOLD_EFFECT_CHOICE_ATK:
            attackStat = attackStat * 3 / 2;
            break;

        case HOLD_EFFECT_CHOICE_SPATK:
            spAttackStat = spAttackStat * 3 / 2;
            break;

        case HOLD_EFFECT_LATI_SPECIAL:
            if ((battleType & BATTLE_TYPE_FRONTIER) == FALSE) {
                if (attackerParams.species == SPECIES_LATIOS || attackerParams.species == SPECIES_LATIAS) {
                    spAttackStat = spAttackStat * 3 / 2;
                }
            }
            break;

        case HOLD_EFFECT_CLAMPERL_SPATK:
            if (attackerParams.species == SPECIES_CLAMPERL) {
                spAttackStat *= 2;
            }
            break;

        case HOLD_EFFECT_PIKA_SPATK_UP:
            if (attackerParams.species == SPECIES_PIKACHU) {
                attackStat *= 2;
                spAttackStat *= 2;
            }
            break;

        case HOLD_EFFECT_CUBONE_ATK_UP:
            if (attackerParams.species == SPECIES_CUBONE
                || attackerParams.species == SPECIES_MAROWAK) {

                attackStat *= 2;
            }
            break;

        case HOLD_EFFECT_DIALGA_BOOST:
            if ((moveType == TYPE_DRAGON || moveType == TYPE_STEEL)
                && attackerParams.species == SPECIES_DIALGA) {

                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_PALKIA_BOOST:
            if ((moveType == TYPE_DRAGON || moveType == TYPE_WATER)
                && attackerParams.species == SPECIES_PALKIA) {

                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_GIRATINA_BOOST:
            if ((moveType == TYPE_DRAGON || moveType == TYPE_GHOST)
                && attackerParams.species == SPECIES_GIRATINA) {

                if ((BattleMon_Get(battleCtx, attacker, BATTLEMON_VOLATILE_STATUS, NULL) & VOLATILE_CONDITION_TRANSFORM) == FALSE) {
                    movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
                }
            }
            break;

        case HOLD_EFFECT_POWER_UP_PHYS:
            if (moveClass == CLASS_PHYSICAL) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_POWER_UP_SPEC:
            if (moveClass == CLASS_SPECIAL) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH:
            for (i = 0; i < NELEMS(sPunchingMoves); i++) {
                if (sPunchingMoves[i] == move) {
                    movePower = movePower * 6 / 5;
                    break;
                }
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_BUG:
        case HOLD_EFFECT_ARCEUS_BUG:
            if (moveType == TYPE_BUG) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_STEEL:
        case HOLD_EFFECT_ARCEUS_STEEL:
            if (moveType == TYPE_STEEL) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_GROUND:
        case HOLD_EFFECT_ARCEUS_GROUND:
            if (moveType == TYPE_GROUND) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_ROCK:
        case HOLD_EFFECT_ARCEUS_ROCK:
            if (moveType == TYPE_ROCK) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_GRASS:
        case HOLD_EFFECT_ARCEUS_GRASS:
            if (moveType == TYPE_GRASS) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_DARK:
        case HOLD_EFFECT_ARCEUS_DARK:
            if (moveType == TYPE_DARK) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_FIGHT:
        case HOLD_EFFECT_ARCEUS_FIGHTING:
            if (moveType == TYPE_FIGHTING) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_ELECTRIC:
        case HOLD_EFFECT_ARCEUS_ELECTRIC:
            if (moveType == TYPE_ELECTRIC) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_WATER:
        case HOLD_EFFECT_ARCEUS_WATER:
            if (moveType == TYPE_WATER) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_FLYING:
        case HOLD_EFFECT_ARCEUS_FLYING:
            if (moveType == TYPE_FLYING) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_POISON:
        case HOLD_EFFECT_ARCEUS_POISON:
            if (moveType == TYPE_POISON) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_ICE:
        case HOLD_EFFECT_ARCEUS_ICE:
            if (moveType == TYPE_ICE) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_GHOST:
        case HOLD_EFFECT_ARCEUS_GHOST:
            if (moveType == TYPE_GHOST) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_PSYCHIC:
        case HOLD_EFFECT_ARCEUS_PSYCHIC:
            if (moveType == TYPE_PSYCHIC) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;


        case HOLD_EFFECT_STRENGTHEN_FIRE:
        case HOLD_EFFECT_ARCEUS_FIRE:
            if (moveType == TYPE_FIRE) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_DRAGON:
        case HOLD_EFFECT_ARCEUS_DRAGON:
            if (moveType == TYPE_DRAGON) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;

        case HOLD_EFFECT_STRENGTHEN_NORMAL:
            if (moveType == TYPE_NORMAL) {
                movePower = movePower * (100 + attackerParams.heldItemPower) / 100;
            }
            break;
    }

    // Defender item
    switch (defenderParams.heldItemEffect) {
        default:
            break;

        case HOLD_EFFECT_LATI_SPECIAL:
            if ((battleType & BATTLE_TYPE_FRONTIER) == FALSE) {
                if (attackerParams.species == SPECIES_LATIOS || attackerParams.species == SPECIES_LATIAS) {
                    spDefenseStat = spDefenseStat * 3 / 2;
                }
            }
            break;

        case HOLD_EFFECT_RAISE_SPD_NO_STATUS:
            spDefenseStat = spDefenseStat * 3 / 2;
            break;

        case HOLD_EFFECT_CLAMPERL_SPDEF:
            if (defenderParams.species == SPECIES_CLAMPERL) {
                spDefenseStat *= 2;
            }
            break;

        case HOLD_EFFECT_DITTO_DEF_UP:
            if (defenderParams.species == SPECIES_DITTO) {
                defenseStat *= 2;
            }
            break;
    }

    GF_ASSERT(battleCtx->powerMul >= 10);
    movePower = movePower * battleCtx->powerMul / 10;

    switch (moveType) {
        default:
            break;

        case TYPE_ELECTRIC:
            if (battleCtx->battleMons[attacker].moveEffectsMask & MOVE_EFFECT_CHARGE) {
                movePower *= 2;
            }
            if (BattleSystem_AnyBattlersWithMoveEffect(battleSys, battleCtx, MOVE_EFFECT_MUD_SPORT)) {
                movePower /= 2;
            }
            break;

        case TYPE_FIRE:
            if (BattleSystem_AnyBattlersWithMoveEffect(battleSys, battleCtx, MOVE_EFFECT_WATER_SPORT)) {
                movePower /= 2;
            }
            break;
    }

    if (battleCtx->turnFlags[attacker].helpingHand) {
        movePower = movePower * 15 / 10;
    }

    // Focus Punch
    if (effect == BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT) {
        defenseStage = 0;
        spDefenseStage = 0;

        if (attackStage < 0) {
            attackStage = 0;
        }
    }

    attackStage += 6;
    defenseStage += 6;
    spAttackStage += 6;
    spDefenseStage += 6;
	
	if (move == MOVE_KNOCK_OFF && DEFENDING_MON.heldItem != ITEM_NONE)
	{
		movePower = movePower * 15 / 10;
	}

    if (NO_CLOUD_NINE) {

        if ((fieldConditions & FIELD_CONDITION_SANDSTORM)
                && (defenderParams.type1 == TYPE_ROCK || defenderParams.type2 == TYPE_ROCK)) {
            spDefenseStat = spDefenseStat * 15 / 10;
        }

        if ((fieldConditions & FIELD_CONDITION_SUNNY)
                && BattleSystem_CountAbility(battleSys, battleCtx, COUNT_ALIVE_BATTLERS_OUR_SIDE, attacker, ABILITY_FLOWER_GIFT)
				&& attackerParams.ability != ABILITY_FLOWER_GIFT) {
            attackStat = attackStat * 15 / 10;
			spAttackStat = spAttackStat * 15 / 10;
        }
    }

    //if (MOVE_DATA(move).effect == BATTLE_EFFECT_HALVE_DEFENSE) {
    //    defenseStat = defenseStat / 2;
    //}

    if (moveClass == CLASS_PHYSICAL) {
        if (criticalMul > 1) {
            if (attackStage > 6) {
                damage = attackStat * sStatStageBoosts[attackStage].numerator;
                damage /= sStatStageBoosts[attackStage].denominator;
            } else {
                damage = attackStat;
            }
        } else {
            damage = attackStat * sStatStageBoosts[attackStage].numerator;
            damage /= sStatStageBoosts[attackStage].denominator;
        }

        damage *= movePower;
        damage *= (attackerLevel * 2 / 5 + 2);

        if (criticalMul > 1) {
            if (defenseStage < 6) {
                stageDivisor = defenseStat * sStatStageBoosts[defenseStage].numerator;
                stageDivisor /= sStatStageBoosts[defenseStage].denominator;
            } else {
                stageDivisor = defenseStat;
            }
        } else {
            stageDivisor = defenseStat * sStatStageBoosts[defenseStage].numerator;
            stageDivisor /= sStatStageBoosts[defenseStage].denominator;
        }

        damage /= stageDivisor;
        damage /= 50;

        if ((attackerParams.statusMask & MON_CONDITION_BURN) && (attackerParams.ability != ABILITY_GUTS) && (battleCtx->moveCur != MOVE_FACADE)) {
			damage /= 2;
		}

        if ((sideConditions & SIDE_CONDITION_REFLECT) != FALSE
                && criticalMul == 1
                && effect != BATTLE_EFFECT_REMOVE_SCREENS
                && effect != BATTLE_EFFECT_HIT_LAST_WHIFF_IF_HIT) {
            if ((battleType & BATTLE_TYPE_DOUBLES)
                    && BattleSystem_CountAliveBattlers(battleSys, battleCtx, TRUE, defender) == 2) {
                damage = damage * 2 / 3;
            } else {
                damage /= 2;
            }
        }
    } else if (moveClass == CLASS_SPECIAL) {
        if (criticalMul > 1) {
            if (spAttackStage > 6) {
                damage = spAttackStat * sStatStageBoosts[spAttackStage].numerator;
                damage /= sStatStageBoosts[spAttackStage].denominator;
            } else {
                damage = spAttackStat;
            }
        } else {
            damage = spAttackStat * sStatStageBoosts[spAttackStage].numerator;
            damage /= sStatStageBoosts[spAttackStage].denominator;
        }

        damage *= movePower;
        damage *= (attackerLevel * 2 / 5 + 2);

        if (criticalMul > 1) {
            if (spDefenseStage < 6) {
                stageDivisor = spDefenseStat * sStatStageBoosts[spDefenseStage].numerator;
                stageDivisor /= sStatStageBoosts[spDefenseStage].denominator;
            } else {
                stageDivisor = spDefenseStat;
            }
        } else {
            stageDivisor = spDefenseStat * sStatStageBoosts[spDefenseStage].numerator;
            stageDivisor /= sStatStageBoosts[spDefenseStage].denominator;
        }
		
		if ((move == MOVE_HYDRO_CANNON)
		|| (move == MOVE_KINESIS))
		{
				if (criticalMul > 1) {
				if (defenseStage < 6) {
					stageDivisor = defenseStat * sStatStageBoosts[defenseStage].numerator;
					stageDivisor /= sStatStageBoosts[defenseStage].denominator;
				} else {
					stageDivisor = defenseStat;
				}
			} else {
				stageDivisor = defenseStat * sStatStageBoosts[defenseStage].numerator;
				stageDivisor /= sStatStageBoosts[defenseStage].denominator;
			}
		}

        damage /= stageDivisor;
        damage /= 50;

        if ((sideConditions & SIDE_CONDITION_LIGHT_SCREEN) != FALSE
                && criticalMul == 1
                && MOVE_DATA(move).effect != BATTLE_EFFECT_REMOVE_SCREENS) {
            if ((battleType & BATTLE_TYPE_DOUBLES)
                    && BattleSystem_CountAliveBattlers(battleSys, battleCtx, TRUE, defender) == 2) {
                damage = damage * 2 / 3;
            } else {
                damage /= 2;
            }
        }
    }

    if (battleType & BATTLE_TYPE_DOUBLES) {

        if (MOVE_DATA(move).range == RANGE_ADJACENT_OPPONENTS
            && BattleSystem_CountAliveBattlers(battleSys, battleCtx, TRUE, defender) >= 2)
        {
            damage = damage * 3 / 4;
        }

        if (MOVE_DATA(move).range == RANGE_ALL_ADJACENT
            && BattleSystem_CountAliveBattlers(battleSys, battleCtx, FALSE, defender) >= 2)
        {
            damage = damage * 3 / 4;
        }
    }

    if (NO_CLOUD_NINE) {
        if (fieldConditions & FIELD_CONDITION_RAINING) {
            switch (moveType) {
            case TYPE_FIRE:
                damage /= 2;
                break;
            case TYPE_WATER:
                damage = damage * 15 / 10;
                break;
            }
        }

        if ((fieldConditions & FIELD_CONDITION_SOLAR_DOWN) && move == MOVE_SOLAR_BEAM) {
            damage /= 2;
        }

        if (fieldConditions & FIELD_CONDITION_SUNNY) {
            switch (moveType) {
            case TYPE_FIRE:
                damage = damage * 15 / 10;
                break;
            case TYPE_WATER:
                damage /= 2;
                break;
            }
        }
    }

    return damage + 2;
}

int BattleSystem_CalcDamageVariance(BattleSystem *battleSys, BattleContext *battleCtx, int damage)
{
    if (damage) {
        damage *= (100 - (BattleSystem_RandNext(battleSys) % 16));
        damage /= 100;

        // Always deal at least 1 damage
        if (damage == 0) {
            damage = 1;
        }
    }

    return damage;
}

// each probability here is implicitly P / Q
static const u8 sCriticalStageRates[][2] = {
//    Q     P
    { 24,   1 }, // neutral
    { 16,   3 }, // +1
    { 2,    1 }, // +2
    { 1,    1 }, // +3
    { 1,    1 }, // +4
};

// This section builds a new table that achieves the above crit rates by
// applying the modulo function twice.
enum { iMax = sizeof(sCriticalStageRates) / sizeof(sCriticalStageRates[0]) };

static const u8 sCriticalStageModuli[iMax][2] = {
    { 24,   24 },
    { 16,   6 },
    { 2,    2 },
    { 1,    1 },
    { 1,    1 },
};

/*
u8 sCriticalStageModuli[iMax][2];

for (int i = 0; i < iMax; i++) {
    sCriticalStageModuli[i][0] = sCriticalStageRates[i][0];
    
    if (sCriticalStageRates[i][1] == 1) {
        sCriticalStageModuli[i][1] = sCriticalStageRates[i][0];
    }
    else {
        if (sCriticalStageRates[i][0] % sCriticalStageRates[i][1] == 0) {
            sCriticalStageModuli[i][1] = sCriticalStageRates[i][0];
        }
        else {
            sCriticalStageModuli[i][1] = 1 + sCriticalStageRates[i][0] / sCriticalStageRates[i][1];
        }
    }
}
*/

int BattleSystem_CalcCriticalMulti(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender, int criticalStage, u32 sideConditions)
{
    // have to declare vars C89-style to match
    u16 effectiveCritStage;
    u16 item;
    int itemEffect;
    u16 attackerSpecies;
    u32 attackerVolStatus;
    u32 defenderMoveEffects;
    int criticalMul = 1;
    int attackerAbility;
	int punchingMove = 0;

    item = Battler_HeldItem(battleCtx, attacker);
    itemEffect = BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HOLD_EFFECT);

    if (itemEffect == HOLD_EFFECT_PUNCH_CRITRATE_UP)
	{
		for (int i = 0; i < NELEMS(sPunchingMoves); i++)
		{
			if (sPunchingMoves[i] == battleCtx->moveCur)
			{
				punchingMove = 1;
			}
		}
	}

    attackerSpecies = battleCtx->battleMons[attacker].species;
    attackerVolStatus = battleCtx->battleMons[attacker].statusVolatile;
    defenderMoveEffects = battleCtx->battleMons[defender].moveEffectsMask;
    attackerAbility = battleCtx->battleMons[attacker].ability;
    effectiveCritStage = (((attackerVolStatus & VOLATILE_CONDITION_FOCUS_ENERGY) != FALSE) * 2)
            + (itemEffect == HOLD_EFFECT_CRITRATE_UP)
            + criticalStage
            + (attackerAbility == ABILITY_SUPER_LUCK)
            + (battleCtx->battleMons[attacker].meditateCritBoostFlag != FALSE)
            + (2 * (itemEffect == HOLD_EFFECT_CHANSEY_CRITRATE_UP && attackerSpecies == SPECIES_CHANSEY))
			+ (4 * (punchingMove == 1))
            + (2 * (itemEffect == HOLD_EFFECT_FARFETCHD_CRITRATE_UP && attackerSpecies == SPECIES_FARFETCHD));

    if (effectiveCritStage > 4) {
        effectiveCritStage = 4;
    }

    if ((BattleSystem_RandNext(battleSys) % sCriticalStageModuli[effectiveCritStage][0]) % sCriticalStageModuli[effectiveCritStage][1] == 0
            && Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_BATTLE_ARMOR) == FALSE
            && (sideConditions & SIDE_CONDITION_LUCKY_CHANT) == FALSE
            && (defenderMoveEffects & MOVE_EFFECT_NO_CRITICAL) == FALSE) {
        criticalMul = 2;
    }

    if (criticalMul == 2 && Battler_Ability(battleCtx, attacker) == ABILITY_SNIPER) {
        criticalMul = 3;
    }

    return criticalMul;
}

int BattleSystem_PartyMonCalcCriticalMulti(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender, int partyIndicator, int partySlot, int criticalStage, u32 sideConditions)
{
    // have to declare vars C89-style to match
    u16 effectiveCritStage;
    u16 item;
    int itemEffect;
    u16 attackerSpecies;
    u32 defenderMoveEffects;
    int criticalMul = 1;
    int attackerAbility;
	int punchingMove = 0;
    Pokemon *mon;

    mon = BattleSystem_PartyPokemon(battleSys, partyIndicator, partySlot);

    item = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
    itemEffect = BattleSystem_GetItemData(battleCtx, item, ITEM_PARAM_HOLD_EFFECT);

    if (itemEffect == HOLD_EFFECT_PUNCH_CRITRATE_UP)
	{
		for (int i = 0; i < NELEMS(sPunchingMoves); i++)
		{
			if (sPunchingMoves[i] == battleCtx->moveCur)
			{
				punchingMove = 1;
			}
		}
	}

    attackerSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL);
    defenderMoveEffects = battleCtx->battleMons[defender].moveEffectsMask;
    attackerAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    effectiveCritStage = ((itemEffect == HOLD_EFFECT_CRITRATE_UP)
            + criticalStage
            + (attackerAbility == ABILITY_SUPER_LUCK)
            + (2 * (itemEffect == HOLD_EFFECT_CHANSEY_CRITRATE_UP && attackerSpecies == SPECIES_CHANSEY))
			+ (4 * (punchingMove == 1))
            + (2 * (itemEffect == HOLD_EFFECT_FARFETCHD_CRITRATE_UP && attackerSpecies == SPECIES_FARFETCHD)));

    if (effectiveCritStage > 4) {
        effectiveCritStage = 4;
    }

    if ((BattleSystem_RandNext(battleSys) % sCriticalStageModuli[effectiveCritStage][0]) % sCriticalStageModuli[effectiveCritStage][1] == 0
            && Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_BATTLE_ARMOR) == FALSE
            && (sideConditions & SIDE_CONDITION_LUCKY_CHANT) == FALSE
            && (defenderMoveEffects & MOVE_EFFECT_NO_CRITICAL) == FALSE) {
        criticalMul = 2;
    }

    if (criticalMul == 2 && Battler_Ability(battleCtx, attacker) == ABILITY_SNIPER) {
        criticalMul = 3;
    }

    return criticalMul;
}

#define FORBIDDEN_BY_MIMIC_DELIM        0xFFFE
#define FORBIDDEN_BY_METRONOME_DELIM    0xFFFF

static const u16 sCannotMetronomeMoves[] = {
    MOVE_METRONOME,
    MOVE_STRUGGLE,
    MOVE_SKETCH,
    MOVE_MIMIC,
    MOVE_CHATTER,
    FORBIDDEN_BY_MIMIC_DELIM,
    MOVE_SLEEP_TALK,
    MOVE_ASSIST,
    MOVE_MIRROR_MOVE,
    MOVE_COUNTER,
    MOVE_MIRROR_COAT,
    MOVE_PROTECT,
    MOVE_DETECT,
    MOVE_ENDURE,
    MOVE_DESTINY_BOND,
    MOVE_THIEF,
    MOVE_FOLLOW_ME,
    MOVE_SNATCH,
    MOVE_HELPING_HAND,
    MOVE_COVET,
    MOVE_TRICK,
    MOVE_FOCUS_PUNCH,
    MOVE_FEINT,
    MOVE_COPYCAT,
    MOVE_ME_FIRST,
    MOVE_SWITCHEROO,
    FORBIDDEN_BY_METRONOME_DELIM,
};

BOOL Move_CanBeMimicked(u16 move)
{
    int i;
    for (i = 0; sCannotMetronomeMoves[i] != FORBIDDEN_BY_MIMIC_DELIM; i++) {
        if (sCannotMetronomeMoves[i] == move) {
            break;
        }
    }

    return sCannotMetronomeMoves[i] == FORBIDDEN_BY_MIMIC_DELIM;
}

BOOL Move_CanBeMetronomed(BattleSystem *battleSys, BattleContext *battleCtx, int battler, u16 move)
{
    int i = 0;

    if (Move_FailsInHighGravity(battleSys, battleCtx, battler, move) == TRUE
            || Move_HealBlocked(battleSys, battleCtx, battler, move) == TRUE) {
        return FALSE;
    }

    for (; sCannotMetronomeMoves[i] != FORBIDDEN_BY_METRONOME_DELIM; i++) {
        if (sCannotMetronomeMoves[i] == move) {
            break;
        }
    }

    return sCannotMetronomeMoves[i] == FORBIDDEN_BY_METRONOME_DELIM;
}

static const u16 sCannotEncoreMoves[] = {
    MOVE_TRANSFORM,
    MOVE_MIMIC,
    MOVE_SKETCH,
    MOVE_MIRROR_MOVE,
    MOVE_ENCORE,
    MOVE_STRUGGLE,
};

BOOL Move_CanBeEncored(BattleContext *battleCtx, u16 move)
{
    int i;
    for (i = 0; i < NELEMS(sCannotEncoreMoves); i++) {
        if (MOVE_DATA(sCannotEncoreMoves[i]).effect == MOVE_DATA(move).effect) {
            break;
        }
    }

    return i == NELEMS(sCannotEncoreMoves);
}

static const u16 sCannotMeFirstMoves[] = {
    MOVE_COUNTER,
    MOVE_MIRROR_COAT,
    MOVE_THIEF,
    MOVE_COVET,
    MOVE_FOCUS_PUNCH,
    MOVE_CHATTER,
};

BOOL Move_MeFirstCanCopy(BattleContext *battleCtx, u16 move)
{
    int i;
    for (i = 0; i < NELEMS(sCannotMeFirstMoves); i++) {
        if (MOVE_DATA(sCannotMeFirstMoves[i]).effect == MOVE_DATA(move).effect) {
            break;
        }
    }

    return i == NELEMS(sCannotMeFirstMoves);
}

s32 BattleSystem_GetItemData(BattleContext *battleCtx, u16 item, enum ItemDataParam paramID)
{
    u16 dataID = Item_FileID(item, ITEM_FILE_TYPE_DATA);
    ItemData *data = ItemTable_Index(battleCtx->aiContext.itemTable, dataID);

    return Item_Get(data, paramID);
}

int BattleSystem_SideToBattler(BattleSystem *battleSys, BattleContext *battleCtx, int side)
{
    int battler;
    int maxBattlers = BattleSystem_MaxBattlers(battleSys);

    for (battler = 0; battler < maxBattlers; battler++) {
        if (Battler_Side(battleSys, battler) == side) {
            break;
        }
    }

    return battler;
}

void BattleSystem_SortMonActionOrder(BattleSystem *battleSys, BattleContext *battleCtx)
{
    int i, j;
    int battler1, battler2;
    int ignoreQuickClaw;
    int maxBattlers = BattleSystem_MaxBattlers(battleSys);

    for (i = 0; i < maxBattlers - 1; i++) {
        for (j = i + 1; j < maxBattlers; j++) {
            battler1 = battleCtx->battlerActionOrder[i];
            battler2 = battleCtx->battlerActionOrder[j];

            if (battleCtx->battlerActions[battler1][BATTLE_ACTION_SELECTED_COMMAND] == battleCtx->battlerActions[battler2][BATTLE_ACTION_SELECTED_COMMAND]) {
                if (battleCtx->battlerActions[battler1][BATTLE_ACTION_SELECTED_COMMAND] == PLAYER_INPUT_FIGHT) {
                    ignoreQuickClaw = FALSE;
                } else {
                    ignoreQuickClaw = TRUE;
                }

                if (BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, battler1, battler2, ignoreQuickClaw)) {
                    battleCtx->battlerActionOrder[i] = battler2;
                    battleCtx->battlerActionOrder[j] = battler1;
                }
            }
        }
    }
}

static const enum BattleAnimation sEffectsAlwaysShown[] = {
    BATTLE_ANIMATION_SUB_OUT,
    BATTLE_ANIMATION_SUB_IN,
    BATTLE_ANIMATION_ITEM_ESCAPE,
    BATTLE_ANIMATION_WEATHER_FOG,
    BATTLE_ANIMATION_WEATHER_RAIN,
    BATTLE_ANIMATION_WEATHER_HAIL,
    BATTLE_ANIMATION_WEATHER_SAND,
    BATTLE_ANIMATION_WEATHER_SUN,
    BATTLE_ANIMATION_SUBSTITUTE_IN,
    BATTLE_ANIMATION_SUBSTITUTE_OUT,
};

BOOL BattleSystem_ShouldShowStatusEffect(BattleContext *battleCtx, int battler, int status)
{
    BOOL result = FALSE;

    if ((battleCtx->battleMons[battler].statusVolatile & VOLATILE_CONDITION_SUBSTITUTE)
            || (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_SEMI_INVULNERABLE)) {
        for (int i = 0; i < NELEMS(sEffectsAlwaysShown); i++) {
            if (sEffectsAlwaysShown[i] == status) {
                result = TRUE;
                break;
            }
        }
    } else {
        result = TRUE;
    }

    return result;
}

BOOL BattleSystem_TriggerHeldItemOnPivotMove(BattleSystem *battleSys, BattleContext *battleCtx, int *subscript)
{
    BOOL result = FALSE;
    int attackerItemEffect = Battler_HeldItemEffect(battleCtx, battleCtx->attacker);
    int attackerItemPower = Battler_HeldItemPower(battleCtx, battleCtx->attacker, ITEM_POWER_CHECK_ALL);
    int defenderItemEffect = Battler_HeldItemEffect(battleCtx, battleCtx->defender);
    int defenderItemPower = Battler_HeldItemPower(battleCtx, battleCtx->defender, ITEM_POWER_CHECK_ALL);
    int attackingSide = Battler_Side(battleSys, battleCtx->attacker);

    if (attackerItemEffect == HOLD_EFFECT_HP_RESTORE_ON_DMG
            && (battleCtx->battleStatusMask & SYSCTL_MOVE_HIT)
            && ATTACKER_SELF_TURN_FLAGS.shellBellDamageDealt
            && battleCtx->attacker != battleCtx->defender
            && ((ATTACKING_MON.curHP < ATTACKING_MON.maxHP) || (ATTACKING_MON.moveEffectsData.healInversionTurns > 0))
			&& (ATTACKING_MON.sheerForceFlag == FALSE)
            && ATTACKING_MON.curHP) {
        battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKER_SELF_TURN_FLAGS.shellBellDamageDealt * -1, attackerItemPower);
        battleCtx->msgBattlerTemp = battleCtx->attacker;
        *subscript = subscript_restore_a_little_hp;
        result = TRUE;
    }

    if (attackerItemEffect == HOLD_EFFECT_HP_DRAIN_ON_ATK
            && Battler_Ability(battleCtx, battleCtx->attacker) != ABILITY_MAGIC_GUARD
            && (battleCtx->battleStatusMask & SYSCTL_MOVE_HIT)
            && CURRENT_MOVE_DATA.class != CLASS_STATUS
			&& (ATTACKING_MON.sheerForceFlag == FALSE)
            && ATTACKING_MON.curHP) {
        battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP * -1, 10);
        battleCtx->msgBattlerTemp = battleCtx->attacker;
        *subscript = subscript_lose_hp_from_item;
        result = TRUE;
    }

    if (defenderItemEffect == HOLD_EFFECT_RECOIL_PHYSICAL
            && battleCtx->battleMons[battleCtx->attacker].curHP
            && Battler_Ability(battleCtx, battleCtx->attacker) != ABILITY_MAGIC_GUARD
            && DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken) {
        battleCtx->hpCalcTemp = BattleSystem_Divide(ATTACKING_MON.maxHP * -1, defenderItemPower);
        *subscript = subscript_held_item_recoil_when_hit;
        result = TRUE;
    }

    if (defenderItemEffect == HOLD_EFFECT_DMG_USER_CONTACT_XFR
            && ATTACKING_MON.curHP
            && ATTACKING_MON.heldItem == ITEM_NONE
            && (battleCtx->sideConditions[attackingSide].knockedOffItemsMask & FlagIndex(battleCtx->selectedPartySlot[battleCtx->attacker])) == FALSE
            && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
            && ((CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT) && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH))) {
        *subscript = subscript_transfer_sticky_barb;
        result = TRUE;
    }
	
	if (defenderItemEffect == HOLD_EFFECT_WEAK_RAISE_SPA_ATK
			&& (DEFENDING_MON.curHP)
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_SUPER_EFFECTIVE)
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
			&& (DEFENDING_MON.wpolicyFlag == TRUE))
			{
				*subscript = subscript_weakness_policy_activate;
				battleCtx->msgBattlerTemp = battleCtx->defender;
				battleCtx->msgItemTemp = battleCtx->battleMons[battleCtx->defender].heldItem;
				result = TRUE;
				DEFENDING_MON.wpolicyFlag = FALSE;
			}
			
	if (defenderItemEffect == HOLD_EFFECT_LEVITATE_POPPED_IF_HIT
			&& (DEFENDING_MON.curHP)
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken))
			{
				*subscript = subscript_air_balloon_burst;
				battleCtx->msgBattlerTemp = battleCtx->defender;
				battleCtx->msgItemTemp = battleCtx->battleMons[battleCtx->defender].heldItem;
				result = TRUE;	
			}
			
	if (defenderItemEffect == HOLD_EFFECT_SWITCH_ATTACKER_HIT
			&& (DEFENDING_MON.curHP)
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (ATTACKING_MON.sheerForceFlag == FALSE)
			&& (BattleSystem_AnyReplacementMons(battleSys, battleCtx, battleCtx->attacker) == TRUE)
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken))
			{
				*subscript = subscript_hold_up_card;
				result = TRUE;
			}

    return result;
}

void BattleSystem_DecPPForPressure(BattleContext *battleCtx, int attacker, int defender)
{
    if (defender != BATTLER_NONE
            && Battler_Ability(battleCtx, defender) == ABILITY_PRESSURE
            && battleCtx->battleMons[attacker].ppCur[battleCtx->moveSlot[attacker]]) {
        battleCtx->battleMons[attacker].ppCur[battleCtx->moveSlot[attacker]]--;
    }
}

BOOL Battle_RecordingStopped(BattleSystem *battleSys, BattleContext *battleCtx)
{
    if (BattleSystem_RecordingStopped(battleSys)) {
        battleCtx->command = BATTLE_CONTROL_SCREEN_WIPE;
        return TRUE;
    }

    return FALSE;
}

int BattleContext_Get(BattleSystem *battleSys, BattleContext *battleCtx, enum BattleContextParam paramID, int battler)
{
    int side;
    switch (paramID) {
    case BATTLECTX_SIDE_CONDITIONS_MASK:
        side = Battler_Side(battleSys, battler);
        return battleCtx->sideConditionsMask[side];

    case BATTLECTX_SIDE_MIST_TURNS:
        side = Battler_Side(battleSys, battler);
        return battleCtx->sideConditions[side].mistTurns;

    case BATTLECTX_SELECTED_PARTY_SLOT:
        return battleCtx->selectedPartySlot[battler];

    case BATTLECTX_TOTAL_TURNS:
        return battleCtx->totalTurns;
        
    case BATTLECTX_LEVEL_UP_MONS:
        return battleCtx->levelUpMons;
        
    case BATTLECTX_SAFARI_ESCAPE_COUNT:
        return battleCtx->safariEscapeCount;
        
    case BATTLECTX_TOTAL_FAINTED_FOR:
        return battleCtx->totalFainted[battler];
        
    case BATTLECTX_TOTAL_DAMAGE_FOR:
        return battleCtx->totalDamage[battler];
        
    case BATTLECTX_ACTION_FOR:
        return battleCtx->battlerActions[battler][BATTLE_ACTION_PICK_COMMAND];
        
    case BATTLECTX_AICTX_DEFENDER:
        return battleCtx->aiContext.defender;
        
    case BATTLECTX_SWITCHING_MASK:
        return battleCtx->battlersSwitchingMask;
        
    case BATTLECTX_AICTX_SELECTED_TARGET:
        return battleCtx->aiContext.selectedTarget[battler];
        
    case BATTLECTX_ACTION_TEMP_FOR:
        return battleCtx->battlerActions[battler][BATTLE_ACTION_TEMP_VALUE];
        
    case BATTLECTX_CONTROL_COMMAND:
        return battleCtx->command;
        
    case BATTLECTX_NEXT_CONTROL_COMMAND:
        return battleCtx->commandNext;
    }

    GF_ASSERT(FALSE);
    return 0;
}

void BattleContext_Set(BattleSystem *battleSys, BattleContext *battleCtx, enum BattleContextParam paramID, int battler, int val)
{
    int side;
    switch (paramID) {
    case BATTLECTX_SIDE_CONDITIONS_MASK:
        side = Battler_Side(battleSys, battler);
        battleCtx->sideConditionsMask[side] = val;
        break;

    case BATTLECTX_SIDE_MIST_TURNS:
        side = Battler_Side(battleSys, battler);
        battleCtx->sideConditions[side].mistTurns = val;
        break;

    case BATTLECTX_SELECTED_PARTY_SLOT:
        battleCtx->selectedPartySlot[battler] = val;
        break;

    case BATTLECTX_TOTAL_TURNS:
        battleCtx->totalTurns = val;
        break;

    case BATTLECTX_AICTX_DEFENDER:
        battleCtx->aiContext.defender = val;
        break;

    case BATTLECTX_AICTX_SELECTED_TARGET:
        battleCtx->aiContext.selectedTarget[battler] = val;
        break;

    default:
        GF_ASSERT(FALSE);
        break;
    }
}

#include "data/move_side_effect_subscripts.h"

/**
 * @brief Map the given side effect to an appropriate subscript.
 * 
 * @param battleCtx 
 * @param type      Type of side effect; see enum SideEffectType
 * @param effect    Effect which should be mapped to a corresponding subscript
 * @return int 
 */
static int MapSideEffectToSubscript(BattleContext *battleCtx, enum SideEffectType type, u32 effect)
{
    battleCtx->sideEffectType = type;
    battleCtx->sideEffectParam = effect & MOVE_SIDE_EFFECT_SUBSCRIPT_POINTER;
    battleCtx->sideEffectFlags = effect & MOVE_SIDE_EFFECT_FLAGS;

    if (effect & MOVE_SIDE_EFFECT_TO_ATTACKER) {
        battleCtx->sideEffectMon = battleCtx->attacker;
    } else if (effect & MOVE_SIDE_EFFECT_TO_DEFENDER) {
        battleCtx->sideEffectMon = battleCtx->defender;
    } else if ((effect & MOVE_SIDE_EFFECT_ON_HIT) || (effect & MOVE_SIDE_EFFECT_CHECK_HP)) {
        battleCtx->sideEffectMon = 0;
    }

    GF_ASSERT(NELEMS(sSideEffectSubscripts) > (effect & MOVE_SIDE_EFFECT_SUBSCRIPT_POINTER));

    return sSideEffectSubscripts[effect & MOVE_SIDE_EFFECT_SUBSCRIPT_POINTER];
}

/**
 * @brief Apply the type multiplier to the given damage value and update the
 * move status mask.
 * 
 * @param battleCtx 
 * @param attacker 
 * @param mul 
 * @param damage 
 * @param update        If TRUE, the move status mask will be updated
 * @param[out] moveStatus 
 * @return The resulting damage value after applying type effectiveness
 */
static int ApplyTypeMultiplier(BattleContext *battleCtx, int attacker, int mul, int damage, BOOL update, u32 *moveStatus)
{
	if ((battleCtx->battleStatusMask & SYSCTL_IGNORE_TYPE_CHECKS) == FALSE
            && (battleCtx->battleStatusMask & SYSCTL_IGNORE_IMMUNITIES) == FALSE
            && damage) {
        damage = BattleSystem_Divide(damage * mul, 10);
    }
	
    switch (mul)
	{
    case TYPE_MULTI_IMMUNE:
        *moveStatus |= MOVE_STATUS_INEFFECTIVE;
        *moveStatus &= ~MOVE_STATUS_NOT_VERY_EFFECTIVE;
        *moveStatus &= ~MOVE_STATUS_SUPER_EFFECTIVE;
        break;

    case TYPE_MULTI_NOT_VERY_EFF:
        if (update) {
            if (*moveStatus & MOVE_STATUS_SUPER_EFFECTIVE) {
                *moveStatus &= ~MOVE_STATUS_SUPER_EFFECTIVE;
            } else {
                *moveStatus |= MOVE_STATUS_NOT_VERY_EFFECTIVE;
            }
        }
        break;

    case TYPE_MULTI_SUPER_EFF:
        if (update) {
            if (*moveStatus & MOVE_STATUS_NOT_VERY_EFFECTIVE) {
                *moveStatus &= ~MOVE_STATUS_NOT_VERY_EFFECTIVE;
            } else {
                *moveStatus |= MOVE_STATUS_SUPER_EFFECTIVE;
            }
        }
        break;
    }

    return damage;
}

/**
 * @brief Checks if a move is on its damaging turn.
 * 
 * @param battleCtx 
 * @param move 
 * @return TRUE if the move is on its damaging turn, FALSE otherwise
 */
static BOOL MoveIsOnDamagingTurn(BattleContext *battleCtx, int move)
{
    switch (MOVE_DATA(move).effect) {
    case BATTLE_EFFECT_BIDE:
    case BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT:
    case BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT_FLINCH:
    case BATTLE_EFFECT_CHARGE_TURN_DEF_UP:
    case BATTLE_EFFECT_SKIP_CHARGE_TURN_IN_SUN:
    case BATTLE_EFFECT_FLY:
    case BATTLE_EFFECT_DIVE:
    case BATTLE_EFFECT_DIG:
    case BATTLE_EFFECT_BOUNCE:
    case BATTLE_EFFECT_SHADOW_FORCE:
        return battleCtx->battleStatusMask & SYSCTL_LAST_OF_MULTI_TURN;
        break;
    }

    return TRUE;
}

/**
 * @brief Get the type of the battler (e.g. Normal, Fighting, etc.).
 * 
 * An additional layer of logic is applied to Arceus to account for Multitype.
 * 
 * @param battleCtx 
 * @param battler 
 * @param paramID   Either BATTLEMON_TYPE_1 or BATTLEMON_TYPE_2
 * @return The battler's type
 */
static u8 Battler_MonType(BattleContext *battleCtx, int battler, enum BattleMonParam paramID)
{
    u8 type;
    if (paramID == BATTLEMON_TYPE_1) {
        type = battleCtx->battleMons[battler].type1;
    } else if (paramID == BATTLEMON_TYPE_2) {
        type = battleCtx->battleMons[battler].type2;
    } else {
        GF_ASSERT(FALSE);
    }

    if (battleCtx->battleMons[battler].species == SPECIES_ARCEUS
            && battleCtx->battleMons[battler].ability == ABILITY_MULTITYPE) {
        switch (BattleSystem_GetItemData(battleCtx, battleCtx->battleMons[battler].heldItem, ITEM_PARAM_HOLD_EFFECT)) {
        case HOLD_EFFECT_ARCEUS_FIRE:
            type = TYPE_FIRE;
            break;

        case HOLD_EFFECT_ARCEUS_WATER:
            type = TYPE_WATER;
            break;

        case HOLD_EFFECT_ARCEUS_ELECTRIC:
            type = TYPE_ELECTRIC;
            break;

        case HOLD_EFFECT_ARCEUS_GRASS:
            type = TYPE_GRASS;
            break;

        case HOLD_EFFECT_ARCEUS_ICE:
            type = TYPE_ICE;
            break;

        case HOLD_EFFECT_ARCEUS_FIGHTING:
            type = TYPE_FIGHTING;
            break;

        case HOLD_EFFECT_ARCEUS_POISON:
            type = TYPE_POISON;
            break;

        case HOLD_EFFECT_ARCEUS_GROUND:
            type = TYPE_GROUND;
            break;

        case HOLD_EFFECT_ARCEUS_FLYING:
            type = TYPE_FLYING;
            break;

        case HOLD_EFFECT_ARCEUS_PSYCHIC:
            type = TYPE_PSYCHIC;
            break;

        case HOLD_EFFECT_ARCEUS_BUG:
            type = TYPE_BUG;
            break;

        case HOLD_EFFECT_ARCEUS_ROCK:
            type = TYPE_ROCK;
            break;

        case HOLD_EFFECT_ARCEUS_GHOST:
            type = TYPE_GHOST;
            break;

        case HOLD_EFFECT_ARCEUS_DRAGON:
            type = TYPE_DRAGON;
            break;

        case HOLD_EFFECT_ARCEUS_DARK:
            type = TYPE_DARK;
            break;

        case HOLD_EFFECT_ARCEUS_STEEL:
            type = TYPE_STEEL;
            break;

        default:
            type = TYPE_NORMAL;
            break;
        }
    }

    return type;
}

/**
 * @brief Clear the AI's knowledge of any moves for the given battler.
 * 
 * @param battleCtx 
 * @param battler 
 */
static void BattleAI_ClearKnownMoves(BattleContext *battleCtx, u8 battler)
{
    u8 partySlot;

    partySlot = battleCtx->selectedPartySlot[battler];

    for (int i = 0; i < LEARNED_MOVES_MAX; i++) {
        
        // Try to load known moves after switch in
        if (battleCtx->aiContext.battlerPartyMoves[battler][partySlot][i] != MOVE_NONE) {
            battleCtx->aiContext.battlerMoves[battler][i] = battleCtx->aiContext.battlerPartyMoves[battler][partySlot][i];
        }
        else {
            battleCtx->aiContext.battlerMoves[battler][i] = MOVE_NONE;
        }
    }
}

/**
 * @brief Clear the AI's knowledge of the given battler's ability.
 * 
 * @param battleCtx 
 * @param battler 
 */
static void BattleAI_ClearKnownAbility(BattleContext *battleCtx, u8 battler)
{
    u8 partySlot;

    partySlot = battleCtx->selectedPartySlot[battler];

    // Try to load known ability on switch in
    if (battleCtx->aiContext.battlerPartyAbilities[battler][partySlot] != ABILITY_NONE) {
        battleCtx->aiContext.battlerAbilities[battler] = battleCtx->aiContext.battlerPartyAbilities[battler][partySlot];
    }
    else {
        battleCtx->aiContext.battlerAbilities[battler] = ABILITY_NONE;
    }
}

/**
 * @brief Clear the AI's knowledge of the given battler's held item.
 * 
 * @param battleCtx 
 * @param battler 
 */
static void BattleAI_ClearKnownItem(BattleContext *battleCtx, u8 battler)
{
    u8 partySlot;

    partySlot = battleCtx->selectedPartySlot[battler];

    // Try to load known item on switch in
    if (battleCtx->aiContext.battlerPartyHeldItems[battler][partySlot] != ITEM_NONE) {
       battleCtx->aiContext.battlerHeldItems[battler] = battleCtx->aiContext.battlerPartyHeldItems[battler][partySlot];
    }
    else {
        battleCtx->aiContext.battlerHeldItems[battler] = ITEM_NONE;
    }
}

/**
 * @brief Choose the target for Trace.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param defender1 
 * @param defender2 
 * @return Either of defender1 or defender2, if at least one has a Traceable
 * ability, or BATTLER_NONE, if there is no valid Trace target.
 */
static int ChooseTraceTarget(BattleSystem *battleSys, BattleContext *battleCtx, int defender1, int defender2)
{
    int trace = BATTLER_NONE;

    if (battleCtx->battleMons[defender1].ability != ABILITY_FORECAST
            && battleCtx->battleMons[defender1].ability != ABILITY_TRACE
            && battleCtx->battleMons[defender1].ability != ABILITY_MULTITYPE
            && battleCtx->battleMons[defender1].curHP
            && battleCtx->battleMons[defender2].curHP
            && battleCtx->battleMons[defender2].ability != ABILITY_FORECAST
            && battleCtx->battleMons[defender2].ability != ABILITY_TRACE
            && battleCtx->battleMons[defender2].ability != ABILITY_MULTITYPE) {
        // Both targets are eligible; choose randomly
        if (BattleSystem_RandNext(battleSys) & 1) {
            trace = defender2;
        } else {
            trace = defender1;
        }
    } else if (battleCtx->battleMons[defender1].ability != ABILITY_FORECAST
            && battleCtx->battleMons[defender1].ability != ABILITY_TRACE
            && battleCtx->battleMons[defender1].curHP
            && battleCtx->battleMons[defender1].ability != ABILITY_MULTITYPE) {
        trace = defender1;
    } else if (battleCtx->battleMons[defender2].ability != ABILITY_FORECAST
            && battleCtx->battleMons[defender2].ability != ABILITY_TRACE
            && battleCtx->battleMons[defender2].curHP
            && battleCtx->battleMons[defender2].ability != ABILITY_MULTITYPE) {
        trace = defender2;
    }

    return trace;
}

static const u16 sMovesCannotTriggerAnticipation[] = {
    BATTLE_EFFECT_40_DAMAGE_FLAT,
    BATTLE_EFFECT_LEVEL_DAMAGE_FLAT,
    BATTLE_EFFECT_RANDOM_DAMAGE_1_TO_150_LEVEL,
    BATTLE_EFFECT_COUNTER,
    BATTLE_EFFECT_MIRROR_COAT,
    BATTLE_EFFECT_METAL_BURST,
};

static BOOL MoveCannotTriggerAnticipation(BattleContext *battleCtx, int move)
{
    for (int i = 0; i < NELEMS(sMovesCannotTriggerAnticipation); i++) {
        if (sMovesCannotTriggerAnticipation[i] == MOVE_DATA(move).effect) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @brief Compute the type of a move which has variable typing.
 * 
 * This routine is functionally identical to Move_CalcVariableType.
 * 
 * @param battleSys 
 * @param battleCtx 
 * @param battler   The battler to use for the calculation.
 * @param item      The attacker's held item. Affects the typing of Natural Gift
 *                  and Judgment.
 * @param move      The move being used.
 * @return The variable-type of the given move.
 */
static int CalcMoveType(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int item, int move)
{
    int type;

    switch (move) {
    case MOVE_NATURAL_GIFT:
        type = Battler_NaturalGiftType(battleCtx, item);
        break;

    case MOVE_JUDGMENT:
        switch (Battler_HeldItemEffect(battleCtx, item)) {
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
        break;

    case MOVE_HIDDEN_POWER:
        type = ((battleCtx->battleMons[battler].hpIV & 1) >> 0)
                | ((battleCtx->battleMons[battler].attackIV & 1) << 1)
                | ((battleCtx->battleMons[battler].defenseIV & 1) << 2)
                | ((battleCtx->battleMons[battler].speedIV & 1) << 3)
                | ((battleCtx->battleMons[battler].spAttackIV & 1) << 4)
                | ((battleCtx->battleMons[battler].spDefenseIV & 1) << 5);
        type = (type * 15 / 63) + 1;

        if (type >= TYPE_MYSTERY) {
            type++;
        }
        break;

    case MOVE_WEATHER_BALL:
        if (NO_CLOUD_NINE
                && battleCtx->fieldConditionsMask & FIELD_CONDITION_WEATHER) {
            if (WEATHER_IS_RAIN) {
                type = TYPE_WATER;
            }

            if (WEATHER_IS_SAND) {
                type = TYPE_ROCK;
            }

            if (WEATHER_IS_SUN) {
                type = TYPE_FIRE;
            }

            if (WEATHER_IS_HAIL) {
                type = TYPE_ICE;
            }
        }
        break;

    default:
        type = TYPE_NORMAL;
        break;
    }

    return type;
}

/**
 * @brief Check if the AI knows all of an opponent's moves.
 * 
 * @param battleCtx 
 * @param battler   The battler to check the moves of.
 * 
 * @return TRUE if all of the battler's moves are known to the AI,
 * FALSE otherwise. 
 */
static BOOL BattleAI_AllMovesKnown(BattleContext *battleCtx, int battler)
{
    int i;
    BOOL result;

    result = FALSE;

    // If there is a choice-locked move, we only need to know that.
    if (battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove) {
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {

            if (battleCtx->aiContext.battlerMoves[battler][i] == battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove) {

                result = TRUE;
                break;
            }
        }
    }
    else {
        for (i = 0; i < LEARNED_MOVES_MAX; i++) {

            if (battleCtx->aiContext.battlerMoves[battler][i] == MOVE_NONE
                && battleCtx->battleMons[battler].moves[i] != MOVE_NONE) {

                break;
            }
        }
    
        if (i == LEARNED_MOVES_MAX) {

            result = TRUE;
        }
    }

    return result;
}


int BattleAI_PostKOSwitchIn(BattleSystem *battleSys, int battler)
{
    // Must keep C89-style declaration to match
    int i, j, k;
    u8 defender, defenderType1, defenderType2, defenderAbility;
    u8 monType1, monType2, monAbility, monItemEffect;
    u8 battlerType1, battlerType2, battlerAbility, side, oppSide;
    u16 monSpecies, monSpeedStat, monItem, defenderItem;
    u16 move;
    int moveType, movePower, moveEffect;
    u8 battlersDisregarded;
    u16 attackScoreType1, attackScoreType2, defendScoreType1, defendScoreType2;
    u8 picked = 6;
    u8 slot1, slot2;
    u32 moveStatusFlags;
    int partySize;
    int score, maxScore, moveScore, defendScore, attackScore;
    int hpPercent, monCurHP, monMaxHP;
    int defenderCurHP, defenderMaxHP, defenderSpeedStat;
    int moveMoveEffect, moveVolatileStatus, moveStatus;
    int hazardsBonus, sackBonus, speedMultiplier;
    int statDiff, statCumDiff, statMaxDiff, statStage, boostedMonStat;
    Pokemon *mon;
    BattleContext *battleCtx;

    battleCtx = BattleSystem_Context(battleSys);
	
    slot1 = battler;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG)
            || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        slot2 = slot1;
    } else {
        slot2 = BattleSystem_Partner(battleSys, battler);
    }

    defender = BattleSystem_RandomOpponent(battleSys, battleCtx, battler);
    partySize = BattleSystem_PartyCount(battleSys, battler);
    battlersDisregarded = 0;
    picked = 6;
    maxScore = 100;
    statMaxDiff = 0;
    side = Battler_Side(battleSys, battler);
    oppSide = Battler_Side(battleSys, defender);

    // Use the AI's list of known moves

    defenderType1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL);
    defenderType2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL);
    defenderAbility = Battler_Ability(battleCtx, defender);
    defenderItem = BattleMon_Get(battleCtx, defender, BATTLEMON_HELD_ITEM, NULL);
    defenderMaxHP = BattleMon_Get(battleCtx, defender, BATTLEMON_MAX_HP, NULL);
    defenderCurHP = BattleMon_Get(battleCtx, defender, BATTLEMON_CUR_HP, NULL);
	defenderSpeedStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SPEED, NULL);

    if (defenderCurHP <= 0) {
        defenderCurHP = 1;
    }
    if (defenderMaxHP <= 0) {
        defenderMaxHP = 1;
    }

    for (i = 0; i < partySize; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);
        monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);
        monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
        monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
        monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);

        if (monSpecies != SPECIES_NONE
            && monSpecies != SPECIES_EGG
            && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL)
            && (battlersDisregarded & FlagIndex(i)) == FALSE
            && battleCtx->selectedPartySlot[slot1] != i
            && battleCtx->selectedPartySlot[slot2] != i
            && i != battleCtx->aiSwitchedPartySlot[slot1]
            && i != battleCtx->aiSwitchedPartySlot[slot2]
            && i != battleCtx->selectedPartySlot[battler]) {

            monItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
            monItemEffect = BattleSystem_GetItemData(battleCtx, monItem, ITEM_PARAM_HOLD_EFFECT);
            monSpeedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);

            hazardsBonus = 0;
            sackBonus = 0;
            monCurHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
            monMaxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);
            if (monCurHP <= 0) {
                monCurHP = 1;
            }
            if (monMaxHP <= 0) {
                monMaxHP = 1;
            }
            hpPercent = (monCurHP * 100) / monMaxHP;
            score = 0;
            defendScore = 0;
            attackScore = 0;

            if (battleCtx->battleStatusMask & SYSCTL_BATON_PASS) {
                statDiff = 0;
                statCumDiff = 0;

                int statParams[STAT_MAX] = {
                    MON_DATA_MAX_HP,
                    MON_DATA_ATK,
                    MON_DATA_DEF,
                    MON_DATA_SP_ATK,
                    MON_DATA_SP_DEF,
                    MON_DATA_SPEED
                };
                for (k = 0; k < STAT_MAX; k++) {
                    statStage = BattleMon_Get(battleCtx, battler, BATTLEMON_HP_STAGE + k, NULL) - 6;
                    boostedMonStat = Pokemon_GetValue(mon, statParams[k], NULL) * (sStatStageBoosts[statStage].numerator / sStatStageBoosts[statStage].denominator);

                    if (statParams[STAT_MAX] = MON_DATA_SPEED) {
                        if (boostedMonStat > battleCtx->battleMons[defender].speed * 3 / 2)
                        {
                            // Currently, just give a boost for speed, since it's unpredictable
                            attackScore += 50;
                        }
                    }
                    else {
                        if (battleCtx->battleMons[battler].statBoosts[k] > 6)
                        {
                            statDiff = boostedMonStat - Pokemon_GetValue(mon, statParams[k], NULL);
                            statCumDiff += statDiff;
                        }
                    }
                }

                if (statCumDiff > statMaxDiff) {
                    statMaxDiff = statCumDiff;
                }
            }

            for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);

                if (move == MOVE_NONE) {
                    break;
                }

                moveScore = 0;
                movePower = MOVE_DATA(move).power;
                moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);
                moveEffect = MOVE_DATA(move).effect;
                moveStatusFlags = 0;

                if (movePower > 0) {
                    moveScore = BattleSystem_CalcPartyMemberMoveDamage(
                                battleSys,
                                battleCtx,
                                move,
                                battleCtx->sideConditionsMask[oppSide],
                                battleCtx->fieldConditionsMask,
                                0,
                                moveType,
                                battler,
                                defender,
                                1,
                                battler,
                                i);

                    moveScore = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                move,
                                moveType,
                                battler,
                                defender,
                                moveScore,
                                battler,
                                i,
                                &moveStatusFlags);

                    if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                        && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                            moveScore = 0;
                    }

                    if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                        moveScore /= 2;
                    }

                    if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                        moveScore = moveScore * 3 / 2;
                    }

                    moveScore = moveScore * 100 / defenderMaxHP;
                }
                else {
                    moveScore = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                move,
                                moveType,
                                battler,
                                defender,
                                0,
                                battler,
                                i,
                                &moveStatusFlags);
                    moveMoveEffect = MapBattleEffectToMoveEffect(battleCtx, moveEffect);
                    moveStatus = MapBattleEffectToStatusCondition(battleCtx, moveEffect);
                    moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, moveEffect);

                    if (((moveStatusFlags & MOVE_STATUS_IMMUNE) == FALSE)
                        || (moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY)) {
                        if (moveStatus != MON_CONDITION_NONE
                            && (battleCtx->battleMons[defender].status & MON_CONDITION_ANY) == FALSE) {
                            if (moveStatus & MON_CONDITION_BURN) {
                                if (((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_BURN) == FALSE)
                                || monAbility == ABILITY_MOLD_BREAKER)
                                && defenderType1 != TYPE_FIRE
                                && defenderType2 != TYPE_FIRE)
                                if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender)) {
                                    moveScore = 100;
                                }
                            }

                            if (moveStatus & MON_CONDITION_PARALYSIS) {
                                if (((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_PARALYSIS) == FALSE)
                                || monAbility == ABILITY_MOLD_BREAKER)
                                && defenderType1 != TYPE_ELECTRIC
                                && defenderType2 != TYPE_ELECTRIC) {

                                    if (AI_ShouldParalyzeCheck(battleSys, battleCtx, defender, monSpeedStat)) {
                                        moveScore = 50;
                                    }
                                }
                            }

                            if (moveStatus & MON_CONDITION_SLEEP) {
                                if ((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_SLEEP) == FALSE)
                                || monAbility == ABILITY_MOLD_BREAKER) {

                                    moveScore = 50;
                                }
                            }

                            if (moveStatus & MON_CONDITION_ANY_POISON) {
                                if (((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_ANY_POISON) == FALSE)
                                || monAbility == ABILITY_MOLD_BREAKER)
                                && defenderType1 != TYPE_POISON
                                && defenderType2 != TYPE_POISON
                                ) {
                                    if ((defenderType1 == TYPE_STEEL
                                        || defenderType2 == TYPE_STEEL)
                                        && monAbility == ABILITY_CORROSION)
                                    {
                                        moveScore = 50;
                                    }
                                    else {
                                        moveScore = 25;
                                    }
                                }
                            }
                        }

                        if (moveVolatileStatus != VOLATILE_CONDITION_NONE) {
                            if (moveVolatileStatus & VOLATILE_CONDITION_CONFUSION) {
                                if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender)) {
                                    moveScore = 20;
                                }
                                else {
                                    moveScore = 10;
                                }
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_ATTRACT) {
                                if (BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL) != Pokemon_GetValue(mon, MON_DATA_GENDER, NULL)
                                    && Pokemon_GetValue(mon, MON_DATA_GENDER, NULL) != GENDER_NONE) {
                                        moveScore = 25;
                                }
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_MOVE_LOCKED) {
                                if (battleCtx->battleMons[defender].moveEffectsData.encoredMove != MOVE_NONE) {
                                    moveScore = 25;
                                }
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_TORMENT) {
                                if ((battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_TORMENT) == FALSE) {
                                    moveScore = 10;
                                }
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_CURSE) {
                                if ((battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_CURSE) == FALSE) {
                                    if (monType1 == TYPE_GHOST || monType2 == TYPE_GHOST) {
                                        moveScore = 49;
                                    }
                                }
                            }
                        }

                        if (moveMoveEffect != MOVE_EFFECT_NONE) {
                            if (moveMoveEffect & MOVE_EFFECT_YAWN) {
                                if ((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_SLEEP) == FALSE)
                                || monAbility == ABILITY_MOLD_BREAKER) {

                                // Yawn is handled by sleep status, so we just give it +2 here
                                    moveScore += 2;
                                }
                            }

                            if (moveMoveEffect & MOVE_EFFECT_LEECH_SEED) {
                                if ((battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_LEECH_SEED) == FALSE) {
                                    if (defenderType1 != TYPE_GRASS && defenderType2 != TYPE_GRASS) {
                                        moveScore = 25;
                                    }
                                }
                            }

                            if (moveMoveEffect & MOVE_EFFECT_PERISH_SONG) {
                                if (defenderAbility != ABILITY_SOUNDPROOF
                                    && battleCtx->battleMons[defender].moveEffectsData.perishSongTurns > 0) {
                                    
                                    moveScore = 15;
                                }
                            }
                        }

                        if (moveEffect == BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN) {
                            if ((defenderAbility == ABILITY_DEFIANT
                                || defenderAbility == ABILITY_COMPETITIVE)
                                && monAbility != ABILITY_UNAWARE) {
                                    moveScore = 0;
                            }
                            else {
                                if (battleCtx->sideConditionsMask[oppSide] & SIDE_CONDITION_HAZARDS_ANY) {
                                    moveScore = 10 * battleCtx->sideConditions[oppSide].spikesLayers;
                                    moveScore += (15 * battleCtx->sideConditions[oppSide].toxicSpikesLayers);
                                    if (battleCtx->sideConditionsMask[oppSide] & SIDE_CONDITION_STEALTH_ROCK) {
                                        moveScore += 15;
                                    }
                                }
                                else {
                                    moveScore = 2;
                                }
                            }
                        }

                        if (moveEffect == BATTLE_EFFECT_PASS_STATS_AND_STATUS) {
                            if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_SLOWER) {
                                moveScore = 150;
                            }
                        }
                    }
                }

                if (moveScore >= 50) {

                    moveScore += 25;
                }
                if (moveScore >= 125) {

                    moveScore += 25;
                }
                if (moveScore > 250) {
                    moveScore = 250;
                }

                attackScore += moveScore;
            }

            for (j = 0; j < LEARNED_MOVES_MAX; j++) {

                if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE) {

                    move = battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove;
                }
                else {
                    move = battleCtx->aiContext.battlerMoves[defender][j];
                }

                if (move == MOVE_NONE) {
                    break;
                }

                moveScore = 0;
                movePower = MOVE_DATA(move).power;
                moveType = CalcMoveType(battleSys, battleCtx, defender, defenderItem, move);
                moveEffect = MOVE_DATA(move).effect;

                if (movePower > 1) {

                    moveScore = BattleSystem_CalcPartyMemberMoveDamage(battleSys,
                                battleCtx,
                                move,
                                battleCtx->sideConditionsMask[oppSide],
                                battleCtx->fieldConditionsMask,
                                0,
                                moveType,
                                defender,
                                battler,
                                1,
                                battler,
                                i);

                    moveStatusFlags = 0;
                    moveScore = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                move,
                                moveType,
                                defender,
                                battler,
                                moveScore,
                                battler,
                                i,
                                &moveStatusFlags);

                    if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                        && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                            moveScore = 0;
                    }

                    if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                        moveScore /= 2;
                    }

                    if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                        moveScore = moveScore * 3 / 2;
                    }

                    // Move score is % of mon's hp damage they'll take
                    moveScore = moveScore * 100 / monMaxHP;

                    // Penalize choices that would result in a KO, except for Sacks
                    if (moveScore > (monCurHP - Battle_CalcHazardsDamage(battleSys, battleCtx, battler, i))) {
                        if (hpPercent < 50) {
                            sackBonus = (monMaxHP - monCurHP) * 100 / monMaxHP;
                            if (moveScore < sackBonus) {
                                moveScore = 0;
                            }
                            else {
                                moveScore -= sackBonus;
                            }
                        }
                    }
                }
                else {
                    moveScore = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                move,
                                moveType,
                                defender,
                                battler,
                                0,
                                battler,
                                i,
                                &moveStatusFlags);
                    moveMoveEffect = MapBattleEffectToMoveEffect(battleCtx, moveEffect);
                    moveStatus = MapBattleEffectToStatusCondition(battleCtx, moveEffect);
                    moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, moveEffect);

                    if (((moveStatusFlags & MOVE_STATUS_IMMUNE) == FALSE)
                        || (moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY)) {
                        if (moveStatus != MON_CONDITION_NONE
                            && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY) == FALSE) {
                            if (moveStatus & MON_CONDITION_BURN) {
                                if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_BURN) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER)
                                && monType1 != TYPE_FIRE
                                && monType2 != TYPE_FIRE)
                                if (Battle_PartyMonIsPhysicalAttacker(battleSys, battleCtx, battler, i)) {
                                    moveScore = 100;
                                }
                            }

                            if (moveStatus & MON_CONDITION_PARALYSIS) {
                                if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_PARALYSIS) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER)
                                && monType1 != TYPE_ELECTRIC
                                && monType2 != TYPE_ELECTRIC) {

                                    if (AI_PartyMonShouldParalyzeCheck(battleSys, battleCtx, battler, i, defenderSpeedStat)) {
                                        moveScore = 50;
                                    }
                                }
                            }

                            if (moveStatus & MON_CONDITION_SLEEP) {
                                if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_SLEEP) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER) {

                                    moveScore = 50;
                                }
                            }

                            if (moveStatus & MON_CONDITION_ANY_POISON) {
                                if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_ANY_POISON) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER)
                                && monType1 != TYPE_POISON
                                && monType2 != TYPE_POISON
                                ) {
                                    if ((monType1 == TYPE_STEEL
                                        || monType2 == TYPE_STEEL)
                                        && defenderAbility == ABILITY_CORROSION)
                                    {
                                        moveScore = 50;
                                    }
                                    else {
                                        moveScore = 25;
                                    }
                                }
                            }
                        }

                        if (moveVolatileStatus != VOLATILE_CONDITION_NONE) {
                            if (moveVolatileStatus & VOLATILE_CONDITION_CONFUSION) {
                                if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender)) {
                                    moveScore = 20;
                                }
                                else {
                                    moveScore = 10;
                                }
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_ATTRACT) {
                                if (BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL) != Pokemon_GetValue(mon, MON_DATA_GENDER, NULL)
                                    && Pokemon_GetValue(mon, MON_DATA_GENDER, NULL) != GENDER_NONE) {
                                        moveScore = 25;
                                }
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_MOVE_LOCKED) {
                                moveScore = 25;
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_TORMENT) {
                                moveScore = 10;
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_CURSE) {
                                if (defenderType1 == TYPE_GHOST || defenderType2 == TYPE_GHOST) {
                                    moveScore = 49;
                                }
                            }
                        }

                        if (moveMoveEffect != MOVE_EFFECT_NONE) {
                            if (moveMoveEffect & MOVE_EFFECT_YAWN) {
                                if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_SLEEP) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER) {

                                // Yawn is handled by sleep status, so we just give it +2 here
                                    moveScore += 2;
                                }
                            }

                            if (moveMoveEffect & MOVE_EFFECT_LEECH_SEED) {
                                if (monType1 != TYPE_GRASS && monType2 != TYPE_GRASS) {
                                    moveScore = 25;
                                }
                            }

                            if (moveMoveEffect & MOVE_EFFECT_PERISH_SONG) {
                                if (monAbility != ABILITY_SOUNDPROOF) {
                                    
                                    moveScore = 15;
                                }
                            }
                        }

                        if (moveEffect == BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN) {
                            if ((monAbility == ABILITY_DEFIANT
                                || monAbility == ABILITY_COMPETITIVE)
                                && defenderAbility != ABILITY_UNAWARE) {
                                    moveScore = 0;
                            }
                            else {
                                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                                    moveScore = 10 * battleCtx->sideConditions[side].spikesLayers;
                                    moveScore += (8 * battleCtx->sideConditions[side].toxicSpikesLayers);
                                    if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_STEALTH_ROCK) {
                                        moveScore += 12;
                                    }
                                }
                                else {
                                    moveScore = 7;
                                }
                            }
                        }
                    }
                }

                // Penalize choices that would lose more than half health from
                // switching in.
                if (moveScore >= 50) {

                    moveScore += 25;
                }
                if (moveScore >= 125) {

                    moveScore += 25;
                }
                // Cap move score to 250
                if (moveScore > 250) {
                    moveScore = 250;
                }

                defendScore += moveScore;

                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                    if (Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL) == MOVE_DEFOG
                        || Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL) == MOVE_RAPID_SPIN) {

                        hazardsBonus = (monMaxHP / 8) + 2;

                        if (defendScore < hazardsBonus) {
                            score = 0;
                        }
                        else {
                            defendScore -= hazardsBonus;
                        }
                    }
                }
                    
                if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE
                    && move == battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove) {
                    break;
                }
            }

            if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_SLOWER) {
                speedMultiplier = 9;
            }
            else {
                if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_TIE) {
                    speedMultiplier = 15;
                }
                else {
                    speedMultiplier = 18;
                }
            }

            attackScore = attackScore * speedMultiplier / 10;

            if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {

                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_TOXIC_SPIKES) {
                    if (monType1 == TYPE_POISON
                    || monType2 == TYPE_POISON) {
                        if (defendScore < hazardsBonus) {
                            score = 0;
                        }
                        else {
                            defendScore -= hazardsBonus;
                        }
                    }

                    if (monType1 == TYPE_STEEL
                    || monType2 == TYPE_STEEL) {
                        if (defendScore < (hazardsBonus / 2)) {
                            defendScore = 0;
                        }
                        else {
                            defendScore -= (hazardsBonus / 2);
                        }
                    }
                }

                if (monAbility != ABILITY_MAGIC_GUARD) {
                    defendScore += Battle_CalcHazardsDamage(battleSys, battleCtx, battler, i) * 200 / monMaxHP;
                }
            }

            if(400 + attackScore > defendScore) {
                score = 400 + attackScore - defendScore;
            }
            else {
                score = 0;
            }

            if (battleCtx->battleStatusMask & SYSCTL_BATON_PASS) {
                // 200 point bonus to mon that benefits most from baton pass
                if(statMaxDiff == statCumDiff) {
                    score += 200;
                }
            }

            if (maxScore < score) {

                maxScore = score;
                picked = i;
            }
            else {

                battlersDisregarded |= FlagIndex(i);
            }
        }
    }

    if (picked != 6) {
        return picked;
    }

    maxScore = 0;
    picked = 6;

    // Stage 2: Loop through all the party slots and score them by how much damage would be done
    // by the maximum non-critical roll of each of their moves if it were used by the battler
    // which just fainted. Choose the Pokemon with the highest such score, breaking ties by
    // party-order.
    for (i = 0; i < partySize; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);
        monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);

        if (monSpecies != SPECIES_NONE
                && monSpecies != SPECIES_EGG
                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL)
                && battleCtx->selectedPartySlot[slot1] != i
                && battleCtx->selectedPartySlot[slot2] != i
                && i != battleCtx->aiSwitchedPartySlot[slot1]
                && i != battleCtx->aiSwitchedPartySlot[slot2]
                && i != battleCtx->selectedPartySlot[battler])
        {
            for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);
                moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);

                if (move && (MOVE_DATA(move).power > 1)) {
                    score = BattleSystem_CalcPartyMemberMoveDamage(
                            battleSys,
                            battleCtx,
                            move,
                            battleCtx->sideConditionsMask[Battler_Side(battleSys, defender)],
                            battleCtx->fieldConditionsMask,
                            0,
                            moveType,
                            battler,
                            defender,
                            1,
                            battler,
                            i);

                    moveStatusFlags = 0;
                    score = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                move,
                                moveType,
                                battler,
                                defender,
                                score,
                                battler,
                                i,
                                &moveStatusFlags);

                    if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                        && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE))
                    {
                        score = 0;
                    }

                    if (((moveStatusFlags & MOVE_STATUS_IMMUNE) == FALSE)
                        && (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY))
                    {
                        score /= 2;
                    }

                    if (((moveStatusFlags & MOVE_STATUS_IMMUNE) == FALSE)
                        && (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY))
                    {
                        score = score * 3 / 2;
                    }
                }

                if (maxScore < score) {
                    maxScore = score;
                    picked = i;
                }
            }
        }
    }

    return picked;
}

int BattleAI_HotSwitchIn(BattleSystem *battleSys, int battler)
{
    // Must keep C89-style declaration to match
    int i, j;
    u8 defender, defenderType1, defenderType2, defenderAbility;
    u8 monType1, monType2, monAbility, monItemEffect;
    u8 battlerType1, battlerType2, battlerAbility, side, oppSide;
    u16 monSpecies, monSpeedStat, monItem, defenderItem;
    u16 move;
    int moveType, movePower, moveEffect, moveStatFlag;
    u8 battlersDisregarded;
    u16 attackScoreType1, attackScoreType2, defendScoreType1, defendScoreType2;
    u8 picked = 6;
    u8 slot1, slot2;
    u32 moveStatusFlags;
    int partySize;
    int score, maxScore, minScore, moveScore, attackScore;
    int hpPercent, monCurHP, monMaxHP;
    int hazardsBonus, sackBonus, speedMultiplier;
	int moveMoveEffect, moveVolatileStatus, moveStatus;
	int defenderCurHP, defenderMaxHP, defenderSpeedStat;
    Pokemon *mon;
    BattleContext *battleCtx;
    BOOL keepPicked;

    battleCtx = BattleSystem_Context(battleSys);
	
    slot1 = battler;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG)
            || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        slot2 = slot1;
    } else {
        slot2 = BattleSystem_Partner(battleSys, battler);
    }

    defender = BattleSystem_RandomOpponent(battleSys, battleCtx, battler);
    partySize = BattleSystem_PartyCount(battleSys, battler);
    battlersDisregarded = 0;
    picked = 6;
    minScore = 200;
    side = Battler_Side(battleSys, battler);
    oppSide = Battler_Side(battleSys, defender);

    // Use the AI's list of known moves

    defenderType1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL);
    defenderType2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL);
    defenderAbility = Battler_Ability(battleCtx, defender);
    defenderItem = BattleMon_Get(battleCtx, defender, BATTLEMON_HELD_ITEM, NULL);
	defenderMaxHP = BattleMon_Get(battleCtx, defender, BATTLEMON_MAX_HP, NULL);
    defenderCurHP = BattleMon_Get(battleCtx, defender, BATTLEMON_CUR_HP, NULL);
	defenderSpeedStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SPEED, NULL);

    if (defenderCurHP <= 0) {
        defenderCurHP = 1;
    }
    if (defenderMaxHP <= 0) {
        defenderMaxHP = 1;
    }

    for (i = 0; i < partySize; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);
        monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);
        monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
        monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
        monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);

        if (monSpecies != SPECIES_NONE
            && monSpecies != SPECIES_EGG
            && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL)
            && (battlersDisregarded & FlagIndex(i)) == FALSE
            && battleCtx->selectedPartySlot[slot1] != i
            && battleCtx->selectedPartySlot[slot2] != i
            && i != battleCtx->aiSwitchedPartySlot[slot1]
            && i != battleCtx->aiSwitchedPartySlot[slot2]
            && i != battleCtx->selectedPartySlot[battler]) {

            monItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
            monItemEffect = BattleSystem_GetItemData(battleCtx, monItem, ITEM_PARAM_HOLD_EFFECT);

            hazardsBonus = 0;
            sackBonus = 0;
            monCurHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
            monMaxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);
            if (monCurHP <= 0) {
                monCurHP = 1;
            }
            if (monMaxHP <= 0) {
                monMaxHP = 1;
            }
            hpPercent = (monCurHP * 100) / monMaxHP;
            score = 0;

            for (j = 0; j < LEARNED_MOVES_MAX; j++) {

                if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE) {

                    move = battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove;
                }
                else {
                    move = battleCtx->aiContext.battlerMoves[defender][j];
                }

                if (move == MOVE_NONE) {
                    break;
                }

                moveScore = 0;
                movePower = MOVE_DATA(move).power;
                moveType = CalcMoveType(battleSys, battleCtx, defender, defenderItem, move);
                moveEffect = MOVE_DATA(move).effect;

                if (movePower > 0) {

                    moveScore = BattleSystem_CalcPartyMemberMoveDamage(battleSys,
                            battleCtx,
                            move,
                            battleCtx->sideConditionsMask[oppSide],
                            battleCtx->fieldConditionsMask,
                            0,
                            moveType,
                            defender,
                            battler,
                            1,
                            battler,
                            i);
                        
                    moveStatusFlags = 0;
                    moveScore = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                move,
                                moveType,
                                defender,
                                battler,
                                moveScore,
                                battler,
                                i,
                                &moveStatusFlags);

                    if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                        && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                            moveScore = 0;
                    }

                    if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                        moveScore /= 2;
                    }

                    if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                        moveScore = moveScore * 3 / 2;
                    }

                    // Move score is % of mon's hp damage they'll take
                    moveScore = moveScore * 100 / monMaxHP;

                    // Penalize choices that would lose more than half health from
                    // switching in.
                    if (moveScore > (Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL) / 2)) {

                        moveScore += 100;
                    }

                    // Penalize choices that would result in a KO, except for Sacks
                    if (moveScore > (monCurHP - Battle_CalcHazardsDamage(battleSys, battleCtx, battler, i))) {
                        if (hpPercent < 50) {
                            sackBonus = (monMaxHP - monCurHP) * 100 / monMaxHP;
                            if (moveScore < sackBonus) {
                                moveScore = 0;
                            }
                            else {
                                moveScore -= sackBonus;
                            }
                        }
                    }
                }

                score += moveScore;

                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                    if (Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL) == MOVE_DEFOG
                        || Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL) == MOVE_RAPID_SPIN) {

                        hazardsBonus = (monMaxHP / 8) + 2;

                        if (score < hazardsBonus) {
                            score = 0;
                        }
                        else {
                            score -= hazardsBonus;
                        }
                    }
                }
                    
                if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE
                    && move == battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove) {
                    break;
                }
            }

            score += BattleAI_CalculateStatusMoveDefendScore(battleSys, battleCtx, defender, battler, battler, i);

            monSpeedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);

            if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_SLOWER) {
                speedMultiplier = 8;
            }
            else {
                if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_TIE) {
                    speedMultiplier = 10;
                }
                else {
                    speedMultiplier = 12;
                }
            }

            if (speedMultiplier != 10) {

                score = score * speedMultiplier / 10;
            }

            if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {

                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_TOXIC_SPIKES) {
                    if (monType1 == TYPE_POISON
                    || monType2 == TYPE_POISON) {
                        if (score < hazardsBonus) {
                            score = 0;
                        }
                        else {
                            score -= hazardsBonus;
                        }
                    }

                    if (monType1 == TYPE_STEEL
                    || monType2 == TYPE_STEEL) {
                        if (score < (hazardsBonus / 2)) {
                            score = 0;
                        }
                        else {
                            score -= (hazardsBonus / 2);
                        }
                    }
                }

                if (monAbility != ABILITY_MAGIC_GUARD) {
                    score += Battle_CalcHazardsDamage(battleSys, battleCtx, battler, i) * 200 / monMaxHP;
                }
            }

            // Status immunity item score boost for yawn switch
            if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_YAWN) {
                if (monItemEffect == HOLD_EFFECT_SLP_RESTORE
                    || monItemEffect == HOLD_EFFECT_STATUS_RESTORE
                    || monItemEffect == HOLD_EFFECT_PSN_USER
                    || monItemEffect == HOLD_EFFECT_BRN_USER) {
                    
                    if (score < 150) {
                        score = 0;
                    }
                    else {
                        score -= 150;
                    }
                }
            }

            if (minScore > score) {

                minScore = score;
                picked = i;
            }
            else {

                battlersDisregarded |= FlagIndex(i);
            }

            if (picked == i) {
                keepPicked = FALSE;
                attackScore = 0;
                moveStatFlag = BATTLE_STAT_FLAG_NONE;
                for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                    move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);

                    if (move == MOVE_NONE) {
                        break;
                    }

                    moveScore = 0;
                    movePower = MOVE_DATA(move).power;
                    moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);
                    moveEffect = MOVE_DATA(move).effect;
                    moveStatusFlags = 0;

                    if (movePower > 0) {
                        moveScore = BattleSystem_CalcPartyMemberMoveDamage(
                                    battleSys,
                                    battleCtx,
                                    move,
                                    battleCtx->sideConditionsMask[oppSide],
                                    battleCtx->fieldConditionsMask,
                                    0,
                                    moveType,
                                    battler,
                                    defender,
                                    1,
                                    battler,
                                    i);

                        moveScore = PartyMon_ApplyTypeChart(battleSys,
                                    battleCtx,
                                    move,
                                    moveType,
                                    battler,
                                    defender,
                                    moveScore,
                                    battler,
                                    i,
                                    &moveStatusFlags);

                        // Boost to pivoting when doing a yawn switch
                        if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_YAWN) {
                            if (moveEffect == BATTLE_EFFECT_HIT_BEFORE_SWITCH) {
                                moveScore += 150;
                            }
                        }

                        if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                            && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                                moveScore = 0;
                        }

                        if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                            moveScore /= 2;
                        }

                        if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                            moveScore = moveScore * 3 / 2;
                        }

                        moveScore = moveScore * 100 / defenderMaxHP;
                    }

                    if (moveScore >= 50) {

                        moveScore += 25;
                    }
                    if (moveScore >= 125) {

                        moveScore += 25;
                    }
                    if (moveScore > 250) {
                        moveScore = 250;
                    }

                    if (moveScore >= 40) {
                        keepPicked = TRUE;
                        break;
                    }

                    attackScore += moveScore;
                }

                attackScore += BattleAI_CalculateStatusMoveAttackScore(battleSys, battleCtx, defender, battler, battler, i);

                if (attackScore / 4 >= 25) {
                    keepPicked = TRUE;
                }

                if (keepPicked == FALSE) {
                    picked = 6;
                    battlersDisregarded |= FlagIndex(i);
                }
            }
        }
    }

    if (picked != 6) {
        return picked;
    }

    maxScore = 0;
    picked = 6;

    // Stage 3: Loop through all the party slots and score them by how much damage would be done
    // by the maximum non-critical roll of each of their moves if it were used by the battler
    // which just fainted. Choose the Pokemon with the highest such score, breaking ties by
    // party-order.
    for (i = 0; i < partySize; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);
        monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);

        if (monSpecies != SPECIES_NONE
                && monSpecies != SPECIES_EGG
                && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL)
                && battleCtx->selectedPartySlot[slot1] != i
                && battleCtx->selectedPartySlot[slot2] != i
                && i != battleCtx->aiSwitchedPartySlot[slot1]
                && i != battleCtx->aiSwitchedPartySlot[slot2]
                && i != battleCtx->selectedPartySlot[battler]) {
            for (j = 0; j < LEARNED_MOVES_MAX; j++) {
                move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL);
                moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);

                if (move && (MOVE_DATA(move).power > 1)) {
                    score = BattleSystem_CalcPartyMemberMoveDamage(
                                    battleSys,
                                    battleCtx,
                                    move,
                                    battleCtx->sideConditionsMask[oppSide],
                                    battleCtx->fieldConditionsMask,
                                    0,
                                    moveType,
                                    battler,
                                    defender,
                                    1,
                                    battler,
                                    i);

                    moveStatusFlags = 0;
                    score = PartyMon_ApplyTypeChart(battleSys,
                                    battleCtx,
                                    move,
                                    moveType,
                                    battler,
                                    defender,
                                    score,
                                    battler,
                                    i,
                                    &moveStatusFlags);

                    if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                         && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) 
                    {
                        score = 0;
                    }

                    if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                        score /= 2;
                    }

                    if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                        score = score * 3 / 2;
                    }
                }

                if (maxScore < score) {
                    maxScore = score;
                    picked = i;
                }
            }
        }
    }

    return picked;
}

int BattleAI_SwitchedSlot(BattleSystem *battleSys, int battler)
{
    BattleContext *battleCtx = BattleSystem_Context(battleSys);
    return battleCtx->aiSwitchedPartySlot[battler];
}

int Move_CalcVariableType(BattleSystem *battleSys, BattleContext *battleCtx, Pokemon *mon, int move)
{
    int type;

    switch (move) {
    case MOVE_NATURAL_GIFT:
        type = BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_NATURAL_GIFT_TYPE);
        break;

    case MOVE_JUDGMENT:
        switch (BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_HOLD_EFFECT)) {
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
        break;

    case MOVE_HIDDEN_POWER:
        type = ((Pokemon_GetValue(mon, MON_DATA_HP_IV, NULL) & 1) >> 0)
                | ((Pokemon_GetValue(mon, MON_DATA_ATK_IV, NULL) & 1) << 1)
                | ((Pokemon_GetValue(mon, MON_DATA_DEF_IV, NULL) & 1) << 2)
                | ((Pokemon_GetValue(mon, MON_DATA_SPEED_IV, NULL) & 1) << 3)
                | ((Pokemon_GetValue(mon, MON_DATA_SPATK_IV, NULL) & 1) << 4)
                | ((Pokemon_GetValue(mon, MON_DATA_SPDEF_IV, NULL) & 1) << 5);
        type = (type * 15 / 63) + 1;

        if (type >= TYPE_MYSTERY) {
            type++;
        }
        break;

    case MOVE_WEATHER_BALL:
        if (NO_CLOUD_NINE) {
            if (battleCtx->fieldConditionsMask & FIELD_CONDITION_WEATHER) {
                if (WEATHER_IS_RAIN) {
                    type = TYPE_WATER;
                }

                if (WEATHER_IS_SAND) {
                    type = TYPE_ROCK;
                }

                if (WEATHER_IS_SUN) {
                    type = TYPE_FIRE;
                }

                if (WEATHER_IS_HAIL) {
                    type = TYPE_ICE;
                }
            }
        }
        break;

    default:
        type = TYPE_NORMAL;
        break;
    }

    return type;
}

int MapBattleEffectToSideCondition(BattleContext *battleCtx, int effect)
{
    u32 sideCondition;

    switch (effect) {
        
        case (BATTLE_EFFECT_SET_REFLECT):
            sideCondition = SIDE_CONDITION_REFLECT;
            break;

        case (BATTLE_EFFECT_SET_LIGHT_SCREEN):
            sideCondition = SIDE_CONDITION_LIGHT_SCREEN;
            break;

        case (BATTLE_EFFECT_SET_SPIKES):
            sideCondition = SIDE_CONDITION_SPIKES;
            break;

        case (BATTLE_EFFECT_PREVENT_STATUS):
            sideCondition = SIDE_CONDITION_SAFEGUARD;
            break;

        case (BATTLE_EFFECT_HIT_IN_3_TURNS):
            sideCondition = SIDE_CONDITION_FUTURE_SIGHT;
            break;

        case (BATTLE_EFFECT_HEAL_IN_3_TURNS):
            sideCondition = SIDE_CONDITION_WISH;
            break;

        case (BATTLE_EFFECT_PREVENT_STAT_REDUCTION):
            sideCondition = SIDE_CONDITION_MIST;
            break;

        case (BATTLE_EFFECT_STEALTH_ROCK):
            sideCondition = SIDE_CONDITION_STEALTH_ROCK;
            break;

        case (BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS):
            sideCondition = SIDE_CONDITION_TAILWIND;
            break;

        case (BATTLE_EFFECT_TOXIC_SPIKES):
            sideCondition = SIDE_CONDITION_TOXIC_SPIKES;
            break;

        case (BATTLE_EFFECT_PREVENT_CRITS):
            sideCondition = SIDE_CONDITION_LUCKY_CHANT;
            break;
			
		case (BATTLE_EFFECT_SET_STICKY_WEB):
            sideCondition = SIDE_CONDITION_STICKY_WEB;
            break;

        default:
            sideCondition = SIDE_CONDITION_NONE;
            break;
    }

    return sideCondition;
}

int MapBattleEffectToFieldCondition(BattleContext *battleCtx, int effect)
{
    u32 fieldCondition;

    switch (effect) {
        
        default:
            fieldCondition = FIELD_CONDITION_NONE;
            break;

        // Rain
        case (BATTLE_EFFECT_WEATHER_RAIN):
            fieldCondition = FIELD_CONDITION_RAINING;
            break;

        // Sand
        case (BATTLE_EFFECT_WEATHER_SANDSTORM):
            fieldCondition = FIELD_CONDITION_SANDSTORM;
            break;
            
        // Sun
        case (BATTLE_EFFECT_WEATHER_SUN):
            fieldCondition = FIELD_CONDITION_SUNNY;
            break;

        // Hail
        case (BATTLE_EFFECT_WEATHER_HAIL):
            fieldCondition = FIELD_CONDITION_HAILING;
            break;

        // Gravity
        case (BATTLE_EFFECT_GRAVITY):
            fieldCondition = FIELD_CONDITION_GRAVITY;
            break;

        // Trick Room
        case (BATTLE_EFFECT_TRICK_ROOM):
            fieldCondition = FIELD_CONDITION_TRICK_ROOM;
            break;

        // Uproar
        case (BATTLE_EFFECT_UPROAR):
            fieldCondition = FIELD_CONDITION_UPROAR;
            break;

    }

    return fieldCondition;
}

int MapBattleEffectToMoveEffect(BattleContext *battleCtx, int effect)
{
    int moveEffect;

    switch (effect) {
        
        case (BATTLE_EFFECT_STATUS_LEECH_SEED):
            moveEffect = MOVE_EFFECT_LEECH_SEED;
            break;

        case (BATTLE_EFFECT_NEXT_ATTACK_ALWAYS_HITS):
            moveEffect = MOVE_EFFECT_LOCK_ON;
            break;

        case (BATTLE_EFFECT_ALL_FAINT_3_TURNS):
            moveEffect = MOVE_EFFECT_PERISH_SONG;
            break;

        case (BATTLE_EFFECT_STATUS_SLEEP_NEXT_TURN):
            moveEffect = MOVE_EFFECT_YAWN;
            break;

        case (BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE):
            moveEffect = MOVE_EFFECT_IMPRISON;
            break;

        case (BATTLE_EFFECT_REMOVE_ALL_PP_ON_DEFEAT):
            moveEffect = MOVE_EFFECT_GRUDGE;
            break;

        case (BATTLE_EFFECT_HALVE_ELECTRIC_DAMAGE):
            moveEffect = MOVE_EFFECT_MUD_SPORT;
            break;

        case (BATTLE_EFFECT_HALVE_FIRE_DAMAGE):
            moveEffect = MOVE_EFFECT_WATER_SPORT;
            break;

        case (BATTLE_EFFECT_SUPRESS_ABILITY):
            moveEffect = MOVE_EFFECT_ABILITY_SUPPRESSED;
            break;

        case (BATTLE_EFFECT_SWAP_ATK_DEF):
            moveEffect = MOVE_EFFECT_POWER_TRICK;
            break;

        case (BATTLE_EFFECT_SP_DEF_UP_DOUBLE_ELECTRIC_POWER):
            moveEffect = MOVE_EFFECT_CHARGE;
            break;

        case (BATTLE_EFFECT_GROUND_TRAP_USER_CONTINUOUS_HEAL):
            moveEffect = MOVE_EFFECT_INGRAIN;
            break;

        case (BATTLE_EFFECT_RESTORE_HP_EVERY_TURN):
            moveEffect = MOVE_EFFECT_AQUA_RING;
            break;

        case (BATTLE_EFFECT_PREVENT_HEALING):
            moveEffect = MOVE_EFFECT_HEAL_BLOCK;
            break;

        case (BATTLE_EFFECT_PREVENT_ITEM_USE):
            moveEffect = MOVE_EFFECT_EMBARGO;
            break;

        case (BATTLE_EFFECT_GIVE_GROUND_IMMUNITY):
            moveEffect = MOVE_EFFECT_MAGNET_RISE;
            break;

        case (BATTLE_EFFECT_CAMOUFLAGE):
            moveEffect = MOVE_EFFECT_CAMOUFLAGE;
            break;

        default:
            moveEffect = MOVE_EFFECT_NONE;
            break;
    }

    return moveEffect;
}

int MapBattleEffectToVolatileStatus(BattleContext *battleCtx, int effect)
{
    int volatileStatus;

    switch (effect) {

        default:
            volatileStatus = VOLATILE_CONDITION_NONE;
            break;
        
        // Confusion
        case BATTLE_EFFECT_CHATTER:
        case BATTLE_EFFECT_CONFUSE_HIT:
        case BATTLE_EFFECT_HURRICANE:
        case BATTLE_EFFECT_CONFUSE_ALL:
        case BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION:
        case BATTLE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION:
        case BATTLE_EFFECT_STATUS_CONFUSE:
            volatileStatus = VOLATILE_CONDITION_CONFUSION;
            break;
        
        // Binding
        case BATTLE_EFFECT_WHIRLPOOL:
        case BATTLE_EFFECT_BIND_HIT:
            volatileStatus = VOLATILE_CONDITION_BIND;
            break;

        // Infatuation
        case BATTLE_EFFECT_INFATUATE_HIT:
        case BATTLE_EFFECT_INFATUATE:
            volatileStatus = VOLATILE_CONDITION_ATTRACT;
            break;

        // Flinch
        case BATTLE_EFFECT_FLINCH_DOUBLE_DAMAGE_FLY_OR_BOUNCE:
        case BATTLE_EFFECT_DAMAGE_WHILE_ASLEEP:
        case BATTLE_EFFECT_CHARGE_TURN_HIGH_CRIT_FLINCH:
        case BATTLE_EFFECT_FLINCH_PARALYZE_HIT:
        case BATTLE_EFFECT_FLINCH_FREEZE_HIT:
        case BATTLE_EFFECT_FLINCH_BURN_HIT:
        case BATTLE_EFFECT_ALWAYS_FLINCH_FIRST_TURN_ONLY:
        case BATTLE_EFFECT_FLINCH_HIT:
            volatileStatus = VOLATILE_CONDITION_FLINCH;
            break;

        // Trap
        case BATTLE_EFFECT_PREVENT_ESCAPE:
            volatileStatus = BATTLE_EFFECT_PREVENT_ESCAPE;
            break;

        // Nightmare
        case BATTLE_EFFECT_STATUS_NIGHTMARE:
            volatileStatus = VOLATILE_CONDITION_NIGHTMARE;
            break;

        // Identified
        case BATTLE_EFFECT_IGNORE_EVASION_REMOVE_GHOST_IMMUNE:
            volatileStatus = VOLATILE_CONDITION_FORESIGHT;
            break;

        // Torment
        case BATTLE_EFFECT_TORMENT:
            volatileStatus = VOLATILE_CONDITION_TORMENT;
            break;

        // Curse
        case BATTLE_EFFECT_CURSE:
            volatileStatus = VOLATILE_CONDITION_CURSE;
            break;
    }

    return volatileStatus;
}

int MapBattleEffectToStatusCondition(BattleContext *battleCtx, int effect)
{
    int statusCondition;

    switch (effect) {

        default:
            statusCondition = MON_CONDITION_NONE;
            break;
        
        // Sleep
        case BATTLE_EFFECT_STATUS_SLEEP_NEXT_TURN:
        case BATTLE_EFFECT_STATUS_SLEEP:
            statusCondition = MON_CONDITION_SLEEP;
            break;

        // Poison
        case BATTLE_EFFECT_HIGH_CRITICAL_POISON_HIT:
        case BATTLE_EFFECT_POISON_MULTI_HIT:
        case BATTLE_EFFECT_BADLY_POISON_HIT:
        case BATTLE_EFFECT_POISON_HIT:
        case BATTLE_EFFECT_STATUS_POISON:
            statusCondition = MON_CONDITION_ANY_POISON;
            break;

        // Burn
        case BATTLE_EFFECT_HIGH_CRITICAL_BURN_HIT:
        case BATTLE_EFFECT_FLINCH_BURN_HIT:
        case BATTLE_EFFECT_THAW_AND_BURN_HIT:
        case BATTLE_EFFECT_RECOIL_BURN_HIT:
        case BATTLE_EFFECT_BURN_HIT:
        case BATTLE_EFFECT_STATUS_BURN:
            statusCondition = MON_CONDITION_BURN;
            break;

        // Freeze
        case BATTLE_EFFECT_BLIZZARD:
        case BATTLE_EFFECT_FLINCH_FREEZE_HIT:
        case BATTLE_EFFECT_FREEZE_HIT:
            statusCondition = MON_CONDITION_FREEZE;
            break;

        // Paralysis
        case BATTLE_EFFECT_BOUNCE:
        case BATTLE_EFFECT_THUNDER:
        case BATTLE_EFFECT_FLINCH_PARALYZE_HIT:
        case BATTLE_EFFECT_RECOIL_PARALYZE_HIT:
        case BATTLE_EFFECT_PARALYZE_HIT:
        case BATTLE_EFFECT_STATUS_PARALYZE:
            statusCondition = MON_CONDITION_PARALYSIS;
            break;

        // Tri Attack
        case BATTLE_EFFECT_TRI_ATTACK:
            statusCondition = (MON_CONDITION_BURN | MON_CONDITION_FREEZE | MON_CONDITION_PARALYSIS);
            break;
    }

    return statusCondition;
}

int MapBattleEffectToStatDrop(BattleContext *battleCtx, int effect)
{
    int battleStatFlag;

    switch (effect) {

        default:
            battleStatFlag = BATTLE_STAT_FLAG_NONE;
            break;
        
        // Attack
        case BATTLE_EFFECT_LOWER_ATTACK_HIT:
        case BATTLE_EFFECT_ATK_DOWN_2:
        case BATTLE_EFFECT_ATK_DOWN:
            battleStatFlag = BATTLE_STAT_FLAG_ATTACK;
            break;

        // Defense
        case BATTLE_EFFECT_LOWER_DEFENSE_HIT:
        case BATTLE_EFFECT_DEF_DOWN_2:
        case BATTLE_EFFECT_DEF_DOWN:
            battleStatFlag = BATTLE_STAT_FLAG_DEFENSE;
            break;

        // Special Attack
        case BATTLE_EFFECT_SP_ATK_DOWN_2_OPPOSITE_GENDER:
        case BATTLE_EFFECT_LOWER_SP_ATK_HIT:
        case BATTLE_EFFECT_SP_ATK_DOWN_2:
        case BATTLE_EFFECT_SP_ATK_DOWN:
            battleStatFlag = BATTLE_STAT_FLAG_SP_ATTACK;
            break;

        // Special Defense
        case BATTLE_EFFECT_LOWER_SP_DEF_2_HIT:
        case BATTLE_EFFECT_SP_DEF_UP_DOUBLE_ELECTRIC_POWER:
        case BATTLE_EFFECT_LOWER_SP_DEF_HIT:
        case BATTLE_EFFECT_SP_DEF_DOWN_2:
        case BATTLE_EFFECT_SP_DEF_DOWN:
            battleStatFlag = BATTLE_STAT_FLAG_SP_DEFENSE;
            break;

        // Speed
        case BATTLE_EFFECT_SPEED_DOWN_HIT:
        case BATTLE_EFFECT_LOWER_SPEED_HIT:
        case BATTLE_EFFECT_SPEED_DOWN_2:
        case BATTLE_EFFECT_SPEED_DOWN:
            battleStatFlag = BATTLE_STAT_FLAG_SPEED;
            break;

        // Accuracy
        case BATTLE_EFFECT_LOWER_ACCURACY_HIT:
        case BATTLE_EFFECT_ACC_DOWN_2:
        case BATTLE_EFFECT_ACC_DOWN:
            battleStatFlag = BATTLE_STAT_FLAG_ACCURACY;
            break;

        // Evasion
        case BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN:
        case BATTLE_EFFECT_LOWER_EVASION_HIT:
        case BATTLE_EFFECT_EVA_DOWN_2:
        case BATTLE_EFFECT_EVA_DOWN:
            battleStatFlag = BATTLE_STAT_FLAG_EVASION;
            break;

        // Attack and Special Attack
        case BATTLE_EFFECT_FAINT_AND_ATK_SP_ATK_DOWN_2:
            battleStatFlag = BATTLE_STAT_FLAG_ATK_AND_SPATK;
            break;

        // Attack and Defense
        case BATTLE_EFFECT_ATK_DEF_DOWN:
            battleStatFlag = BATTLE_STAT_FLAG_ATK_AND_DEF;
            break;
    }

    return battleStatFlag;
}


/**
 * @brief Map the given battle effect to an appropriate stat boost.
 * 
 * @param battleCtx 
 * @param effect    Battle effect which should be mapped to a corresponding stat boost
 * @return The corresponding battle stat flag of the boosted stat
 */
int MapBattleEffectToSelfStatBoost(BattleContext *battleCtx, int effect)
{
    int battleStatFlag;

    switch (effect) {

        default:
            battleStatFlag = BATTLE_STAT_FLAG_NONE;
            break;
        
        // Attack
        case BATTLE_EFFECT_ATK_UP:
        case BATTLE_EFFECT_ATK_UP_2:
        case BATTLE_EFFECT_RAISE_ATK_WHEN_HIT:
        case BATTLE_EFFECT_RAISE_ATTACK_HIT:
        case BATTLE_EFFECT_MAX_ATK_LOSE_HALF_MAX_HP:
            battleStatFlag = BATTLE_STAT_FLAG_ATTACK;
            break;

        // Defense
        case BATTLE_EFFECT_DEF_UP:
        case BATTLE_EFFECT_DEF_UP_2:
        case BATTLE_EFFECT_RAISE_DEF_HIT:
        case BATTLE_EFFECT_CHARGE_TURN_DEF_UP:
        case BATTLE_EFFECT_DEF_UP_DOUBLE_ROLLOUT_POWER:

            battleStatFlag = BATTLE_STAT_FLAG_DEFENSE;
            break;

        // Special Attack
        case BATTLE_EFFECT_SP_ATK_UP:
        case BATTLE_EFFECT_SP_ATK_UP_2:
        case BATTLE_EFFECT_SP_ATK_DOWN_2:
        case BATTLE_EFFECT_RAISE_SP_ATK_HIT:
            battleStatFlag = BATTLE_STAT_FLAG_SP_ATTACK;
            break;

        // Special Defense
        case BATTLE_EFFECT_SP_DEF_UP:
        case BATTLE_EFFECT_SP_DEF_UP_2:
        case BATTLE_EFFECT_SP_DEF_UP_DOUBLE_ELECTRIC_POWER:
            battleStatFlag = BATTLE_STAT_FLAG_SP_DEFENSE;
            break;

        // Speed
        case BATTLE_EFFECT_SPEED_UP:
        case BATTLE_EFFECT_SPEED_UP_2:
        case BATTLE_EFFECT_SPEED_DOWN_2:
        case BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING:
            battleStatFlag = BATTLE_STAT_FLAG_SPEED;
            break;

        // Accuracy
        case BATTLE_EFFECT_ACC_UP:
        case BATTLE_EFFECT_ACC_UP_2:
            battleStatFlag = BATTLE_STAT_FLAG_ACCURACY;
            break;

        // Evasion
        case BATTLE_EFFECT_EVA_UP:
        case BATTLE_EFFECT_EVA_UP_2:
        case BATTLE_EFFECT_EVA_UP_2_MINIMIZE:
            battleStatFlag = BATTLE_STAT_FLAG_EVASION;
            break;

        // Attack and Defense
        case BATTLE_EFFECT_ATK_DEF_UP:
            battleStatFlag = BATTLE_STAT_FLAG_ATK_AND_DEF;
            break;

        // Attack and Speed
        case BATTLE_EFFECT_ATK_SPD_UP:
            battleStatFlag = BATTLE_STAT_FLAG_ATK_AND_SPE;
            break;

        // Defense and Special Defense
        case BATTLE_EFFECT_STOCKPILE:
        case BATTLE_EFFECT_DEF_SPD_UP:
            battleStatFlag = BATTLE_STAT_FLAG_DEF_AND_SPDEF;
            break;

        // Special Attack and Special Defense
        case BATTLE_EFFECT_SP_ATK_SP_DEF_UP:
            battleStatFlag = BATTLE_STAT_FLAG_SPATK_AND_SPDEF;
            break;

        // Omniboost
        case BATTLE_EFFECT_RAISE_ALL_STATS_HIT:
            battleStatFlag = BATTLE_STAT_FLAG_OMNIBOOST;
            break;

        // Acupressure
        case BATTLE_EFFECT_RANDOM_STAT_UP_2:
            battleStatFlag = (BATTLE_STAT_FLAG_OMNIBOOST | BATTLE_STAT_FLAG_EVASION | BATTLE_STAT_FLAG_ACCURACY);
            break;
    }

    return battleStatFlag;
}


static BOOL Battle_AttackerChunksOrKOsDefender(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender)
{
    BOOL result;
    int k;
    int moveType, moveDamage, movePower;
    u8 side;
    u16 move, item;
    u32 effectiveness;

    result = FALSE;

    side = Battler_Side(battleSys, defender);
    
    item = Battler_HeldItem(battleCtx, attacker);

    for (k = 0; k < LEARNED_MOVES_MAX; k++) {
        effectiveness = 0;
        move = battleCtx->battleMons[defender].moves[k];

        if (move == MOVE_NONE) {
            break;
        }

        moveType = CalcMoveType(battleSys, battleCtx, attacker, item, move);
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

    return result;
}

int Battle_CalcHazardsDamage(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int partySlot)
{
    Pokemon *mon;
    u8 side, ability, monType1, monType2, itemEffect;
    u32 spikesCount, toxicSpikesCount;
    BOOL stealthRocks;
    int spikesDivisor, toxicSpikesDivisor, rocksMultiplier;
    int damage;

    side = Battler_Side(battleSys, battler);
    mon = BattleSystem_PartyPokemon(battleSys, battler, partySlot);

    ability = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
    monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
    itemEffect = BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_HOLD_EFFECT);

    damage = 0;

    if (ability == ABILITY_MAGIC_GUARD
        || ((battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) == FALSE)) 
    {
        return damage;
    }

    spikesCount =  battleCtx->sideConditions[side].spikesLayers;
    toxicSpikesCount = battleCtx->sideConditions[side].toxicSpikesLayers;
    
    if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_STEALTH_ROCK) {
        stealthRocks = TRUE;
    }
    else {
        stealthRocks = FALSE;
    }

    switch (spikesCount) {
        default:
            // Set to 255 as safeguard against div by 0
            spikesDivisor = 255;
            break;

        case 1:
            spikesDivisor = 8;
            break;

        case 2:
            spikesDivisor = 6;
            break;

        case 3:
            spikesDivisor = 4;
            break;
    }

    switch (toxicSpikesCount) {
        default:
            // Set to 255 as safeguard against div by 0
            toxicSpikesDivisor = 255;
            break;

        case 1:
            toxicSpikesDivisor = 16;
            break;

        case 2:
            toxicSpikesDivisor = 8;
            break;
    }

    if (stealthRocks == TRUE) {

        rocksMultiplier = BattleSystem_TypeMatchupMultiplier(TYPE_ROCK, monType1, monType2);

        damage += ((Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL) / 8) * rocksMultiplier / 40) * 3 / 4;
        
        if (damage < 1) {
            damage += 1;
        }
    }

    if (spikesCount) {

        if ((monType1 != TYPE_FLYING
        && monType2 != TYPE_FLYING
        && ability != ABILITY_LEVITATE
        && itemEffect != HOLD_EFFECT_LEVITATE_POPPED_IF_HIT)
        || itemEffect == HOLD_EFFECT_SPEED_DOWN_GROUNDED
        || (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY))
        {
            damage += Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL) / spikesDivisor;

            if (damage < 1) {
                damage += 1;
            }
        }
    }

    if (toxicSpikesCount) {
        if (((monType1 != TYPE_FLYING
            && monType2 != TYPE_FLYING
            && ability != ABILITY_LEVITATE
            && itemEffect != HOLD_EFFECT_LEVITATE_POPPED_IF_HIT)
            || itemEffect == HOLD_EFFECT_SPEED_DOWN_GROUNDED
            || (battleCtx->fieldConditionsMask & FIELD_CONDITION_GRAVITY))
            && ability != ABILITY_IMMUNITY
            && ability != ABILITY_POISON_HEAL
            && monType1 != TYPE_POISON
            && monType2 != TYPE_POISON
            && monType1 != TYPE_STEEL
            && monType2 != TYPE_STEEL)
        {
            damage += Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL) / toxicSpikesDivisor;

            if (damage < 1) {
                damage += 1;
            }
        }
    }

    return damage;
}


BOOL Battle_BattleMonIsPhysicalAttacker(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    int i, j;
    u8 moveClass;
    u16 move;
    int moveEffect;
    BOOL result;

    result = FALSE;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = BattleMon_Get(battleCtx, battler, BATTLEMON_MOVE_1 + i, NULL);

        if (move == MOVE_NONE) {
            break;
        }

        moveClass = MOVE_DATA(move).class;
        moveEffect = MOVE_DATA(move).effect;

        if (moveClass == CLASS_PHYSICAL) {
            switch (moveEffect) {
                default:
                    result = TRUE;
                    break;

                // Always utility moves
                case BATTLE_EFFECT_BIDE:
                case BATTLE_EFFECT_COUNTER:
                case BATTLE_EFFECT_40_DAMAGE_FLAT:
                case BATTLE_EFFECT_SET_HP_EQUAL_TO_USER:
                case BATTLE_EFFECT_LEAVE_WITH_1_HP:
                case BATTLE_EFFECT_REMOVE_PROTECT:
                case BATTLE_EFFECT_METAL_BURST:
                case BATTLE_EFFECT_MIRROR_COAT:
                case BATTLE_EFFECT_LEVEL_DAMAGE_FLAT:
                case BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING:
                case BATTLE_EFFECT_HALVE_HP:
                    break;

                // Conditionally utility moves
                case BATTLE_EFFECT_SWITCH_HIT:
                case BATTLE_EFFECT_REMOVE_HELD_ITEM:
                case BATTLE_EFFECT_RAISE_DEF_HIT:
                case BATTLE_EFFECT_SPIKES_MULTI_HIT:
                case BATTLE_EFFECT_INFATUATE_HIT:
                    if (BattleMon_Get(battleCtx, battler, BATTLEMON_ATTACK, NULL) >= (BattleMon_Get(battleCtx, battler, BATTLEMON_SP_ATTACK, NULL) * 2 / 3))
                    {
                        result = TRUE;
                    }
                    break;
            }
        }
    }

    return result;
}

BOOL Battle_PartyMonIsPhysicalAttacker(BattleSystem *battleSys, BattleContext *battleCtx, int battler, int partySlot)
{
    int i, j;
    u8 moveClass;
    u16 move;
    int moveEffect;
    Pokemon *mon;
    BOOL result;

    result = FALSE;

    mon = BattleSystem_PartyPokemon(battleSys, battler, partySlot);

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + i, NULL);

        if (move == MOVE_NONE) {
            break;
        }

        moveClass = MOVE_DATA(move).class;
        moveEffect = MOVE_DATA(move).effect;

        if (moveClass == CLASS_PHYSICAL) {
            switch (moveEffect) {
                default:
                    result = TRUE;
                    break;

                // Always utility moves
                case BATTLE_EFFECT_BIDE:
                case BATTLE_EFFECT_COUNTER:
                case BATTLE_EFFECT_40_DAMAGE_FLAT:
                case BATTLE_EFFECT_SET_HP_EQUAL_TO_USER:
                case BATTLE_EFFECT_LEAVE_WITH_1_HP:
                case BATTLE_EFFECT_REMOVE_PROTECT:
                case BATTLE_EFFECT_METAL_BURST:
                case BATTLE_EFFECT_MIRROR_COAT:
                case BATTLE_EFFECT_LEVEL_DAMAGE_FLAT:
                case BATTLE_EFFECT_REMOVE_HAZARDS_AND_BINDING:
                case BATTLE_EFFECT_HALVE_HP:
                    break;

                // Conditionally utility moves
                case BATTLE_EFFECT_SWITCH_HIT:
                case BATTLE_EFFECT_REMOVE_HELD_ITEM:
                case BATTLE_EFFECT_RAISE_DEF_HIT:
                case BATTLE_EFFECT_SPIKES_MULTI_HIT:
                case BATTLE_EFFECT_INFATUATE_HIT:
                    if (Pokemon_GetValue(mon, MON_DATA_ATK, NULL) >= (Pokemon_GetValue(mon, MON_DATA_SP_ATK, NULL) * 2 / 3)) {
                        result = TRUE;
                    }
                    break;
            }
        }
    }

    return result;
}

BOOL Battle_AbilityDetersStatus(BattleSystem *battleSys, BattleContext *battleCtx, u8 ability, int statusCondition)
{
    BOOL result;

    result = FALSE;

    switch (statusCondition) {
        default:
            break;

        case MON_CONDITION_TOXIC_COUNTER_0:
        case MON_CONDITION_TOXIC_COUNTER_1:
        case MON_CONDITION_TOXIC_COUNTER_2:
        case MON_CONDITION_TOXIC_COUNTER_3:
        case MON_CONDITION_POISON:
        case MON_CONDITION_TOXIC:
        case MON_CONDITION_TOXIC_COUNTER:
        case MON_CONDITION_ANY_POISON:
            switch (ability) {
                default:
                    break;

                case ABILITY_IMMUNITY:
                case ABILITY_POISON_HEAL:
                case ABILITY_MAGIC_GUARD:
                case ABILITY_SHED_SKIN:
                case ABILITY_MARVEL_SCALE:
                case ABILITY_GUTS:
                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;

                case ABILITY_HYDRATION:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
                        result = TRUE;
                    }
                    break;

                case ABILITY_LEAF_GUARD:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                        result = TRUE;
                    }
                    break;
            }
            break;

        case MON_CONDITION_BURN:
            switch (ability) {
                default:
                    break;

                case ABILITY_WATER_VEIL:
                case ABILITY_MAGIC_GUARD:
                case ABILITY_SHED_SKIN:
                case ABILITY_MARVEL_SCALE:
                case ABILITY_GUTS:
                case ABILITY_FLARE_BOOST:
                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;

                case ABILITY_HYDRATION:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
                        result = TRUE;
                    }
                    break;

                case ABILITY_LEAF_GUARD:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                        result = TRUE;
                    }
                    break;
            }
            break;

        case MON_CONDITION_PARALYSIS:
            switch (ability) {
                default:
                    break;

                case ABILITY_LIMBER:
                case ABILITY_SHED_SKIN:
                case ABILITY_MARVEL_SCALE:
                case ABILITY_GUTS:
                case ABILITY_QUICK_FEET:
                case ABILITY_MAGIC_BOUNCE:
                case ABILITY_WONDER_GUARD:
                    result = TRUE;
                    break;

                case ABILITY_HYDRATION:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
                        result = TRUE;
                    }
                    break;

                case ABILITY_LEAF_GUARD:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                        result = TRUE;
                    }
                    break;
            }
            break;

        case MON_CONDITION_SLEEP_0:
        case MON_CONDITION_SLEEP_1:
        case MON_CONDITION_SLEEP_2:
        case MON_CONDITION_SLEEP:
            switch (ability) {
                default:
                    break;

                case ABILITY_INSOMNIA:
                case ABILITY_VITAL_SPIRIT:
                case ABILITY_EARLY_BIRD:
                case ABILITY_SHED_SKIN:
                case ABILITY_MARVEL_SCALE:
                case ABILITY_GUTS:
                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;

                case ABILITY_HYDRATION:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
                        result = TRUE;
                    }
                    break;

                case ABILITY_LEAF_GUARD:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                        result = TRUE;
                    }
                    break;
            }
            break;
    }

    return result;
}

BOOL Battle_AbilityDetersVolatileStatus(BattleSystem *battleSys, BattleContext *battleCtx, u8 ability, int volatileStatus)
{
    BOOL result;

    result = FALSE;

    switch (volatileStatus) {
        default:
            break;

        case VOLATILE_CONDITION_CONFUSION_0:
        case VOLATILE_CONDITION_CONFUSION_1:
        case VOLATILE_CONDITION_CONFUSION_2:
        case VOLATILE_CONDITION_CONFUSION:
            switch (ability) {
                default:
                    break;

                case ABILITY_OWN_TEMPO:
                case ABILITY_TANGLED_FEET:
                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;
            }
            break;

        case VOLATILE_CONDITION_FLINCH:
            switch (ability) {
                default:
                    break;

                case ABILITY_INNER_FOCUS:
                case ABILITY_STEADFAST:
                case ABILITY_SHIELD_DUST:
                    result = TRUE;
                    break;
            }
            break;

        case VOLATILE_CONDITION_CHIP:
            switch (ability) {
                default:
                    break;

                case ABILITY_MAGIC_GUARD:
                    result = TRUE;
                    break;
            }
            break;

        case VOLATILE_CONDITION_CURSE:
            switch (ability) {
                default:
                    break;

                case ABILITY_MAGIC_BOUNCE:
                case ABILITY_MAGIC_GUARD:
                    result = TRUE;
                    break;
            }
            break;
    }

    return result;
}

BOOL Battle_AbilityDetersMoveEffect(BattleSystem *battleSys, BattleContext *battleCtx, u8 ability, int moveEffect)
{
    BOOL result;

    result = FALSE;

    switch (moveEffect) {
        default:
            break;

        case MOVE_EFFECT_LEECH_SEED:
            switch (ability) {
                default:
                    break;

                case ABILITY_MAGIC_GUARD:
                case ABILITY_LIQUID_OOZE:
                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;
            }
            break;

        case MOVE_EFFECT_PERISH_SONG:
            switch (ability) {
                default:
                    break;

                case ABILITY_SOUNDPROOF:
                    result = TRUE;
                    break;
            }
            break;

        case MOVE_EFFECT_ABILITY_SUPPRESSED:
            switch (ability) {
                default:
                    break;

                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;
            }
            break;

        case MOVE_EFFECT_HEAL_BLOCK:
            switch (ability) {
                default:
                    break;

                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;
            }
            break;

        case MOVE_EFFECT_EMBARGO:
            switch (ability) {
                default:
                    break;

                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;
            }
            break;

        case MOVE_EFFECT_MAGNET_RISE:
            switch (ability) {
                default:
                    break;

                case ABILITY_ROCHE_RADIUS:
                    result = TRUE;
                    break;
            }
            break;

        case MOVE_EFFECT_YAWN_0:
        case MOVE_EFFECT_YAWN_1:
        case MOVE_EFFECT_YAWN:
            switch (ability) {
                default:
                    break;

                case ABILITY_INSOMNIA:
                case ABILITY_VITAL_SPIRIT:
                case ABILITY_EARLY_BIRD:
                case ABILITY_SHED_SKIN:
                case ABILITY_MARVEL_SCALE:
                case ABILITY_GUTS:
                case ABILITY_MAGIC_BOUNCE:
                    result = TRUE;
                    break;

                case ABILITY_HYDRATION:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_RAINING) {
                        result = TRUE;
                    }
                    break;

                case ABILITY_LEAF_GUARD:
                    if (battleCtx->fieldConditionsMask & FIELD_CONDITION_SUNNY) {
                        result = TRUE;
                    }
                    break;
            }
            break;
    }

    return result;
}

BOOL AI_ShouldParalyzeCheck(BattleSystem *battleSys, BattleContext *battleCtx, int defender, u16 attackerSpeedStat)
{
    u8 defenderLevel, defenderType1, defenderType2, defenderAbility;
    u16 defenderSpeedStat, defenderDefStat, defenderSpDefStat, move;
    u32 defenderMaxHP;
    int moveEffect, moveStatFlag;
    int i;
    BOOL hasSpeedBoost;
    BOOL result;

    result = FALSE;

    hasSpeedBoost = FALSE;

    defenderSpeedStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SPEED, NULL);
    defenderDefStat = BattleMon_Get(battleCtx, defender, BATTLEMON_DEFENSE, NULL);
    defenderSpDefStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SP_DEFENSE, NULL);
    defenderMaxHP = BattleMon_Get(battleCtx, defender, BATTLEMON_MAX_HP, NULL);
    defenderLevel = BattleMon_Get(battleCtx, defender, BATTLEMON_LEVEL, NULL);
    defenderAbility = BattleMon_Get(battleCtx, defender, BATTLEMON_ABILITY, NULL);
    defenderType1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL);
    defenderType2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL);

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = battleCtx->battleMons[defender].moves[i];

        if (move == MOVE_NONE) {
            break;
        }
        
        moveEffect = MOVE_DATA(move).effect;
        moveStatFlag = MapBattleEffectToSelfStatBoost(battleCtx, moveEffect);

        if ((moveStatFlag & BATTLE_STAT_FLAG_SPEED)
            && moveEffect != BATTLE_EFFECT_RAISE_ALL_STATS_HIT) {

                hasSpeedBoost = TRUE;
                break;
        }
    }

    if (hasSpeedBoost) {
        if (2 * defenderSpeedStat > attackerSpeedStat) {
            result = TRUE;
        }
    }
    else {
        if (defenderMaxHP - defenderLevel + defenderDefStat + defenderSpDefStat > 3 * defenderSpeedStat)
        {
            if ((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_PARALYSIS) == FALSE)
                && defenderType1 != TYPE_ELECTRIC
                && defenderType2 != TYPE_ELECTRIC)
            {
                result = TRUE;
            }
        }
    }

    return result;
}


BOOL AI_PartyMonShouldParalyzeCheck(BattleSystem *battleSys, BattleContext *battleCtx, int defender, int partySlot, u16 attackerSpeedStat)
{
    u8 defenderLevel, defenderType1, defenderType2, defenderAbility;
    u16 defenderSpeedStat, defenderDefStat, defenderSpDefStat, move;
    u32 defenderMaxHP;
    int moveEffect, moveStatFlag;
    int i;
    BOOL hasSpeedBoost;
    BOOL result;
    Pokemon *mon;

    result = FALSE;

    hasSpeedBoost = FALSE;

    mon = BattleSystem_PartyPokemon(battleSys, defender, partySlot);

    defenderSpeedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
    defenderDefStat = Pokemon_GetValue(mon, MON_DATA_DEF, NULL);
    defenderSpDefStat = Pokemon_GetValue(mon, MON_DATA_SP_DEF, NULL);
    defenderMaxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);
    defenderLevel = Pokemon_GetValue(mon, MON_DATA_LEVEL, NULL);
    defenderAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    defenderType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
    defenderType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + i, NULL);

        if (move == MOVE_NONE) {
            break;
        }
        
        moveEffect = MOVE_DATA(move).effect;
        moveStatFlag = MapBattleEffectToSelfStatBoost(battleCtx, moveEffect);

        if ((moveStatFlag & BATTLE_STAT_FLAG_SPEED)
            && moveEffect != BATTLE_EFFECT_RAISE_ALL_STATS_HIT) {

                hasSpeedBoost = TRUE;
                break;
        }
    }

    if (hasSpeedBoost) {
        if (2 * defenderSpeedStat > attackerSpeedStat) {
            result = TRUE;
        }
    }
    else {
        if (defenderMaxHP - defenderLevel + defenderDefStat + defenderSpDefStat > 3 * defenderSpeedStat)
        {
            if ((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_PARALYSIS) == FALSE)
                && defenderType1 != TYPE_ELECTRIC
                && defenderType2 != TYPE_ELECTRIC)
            {
                result = TRUE;
            }
        }
    }

    return result;
}

BOOL AI_ShouldTauntCheck(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender)
{
    u8 speedCompare;
    u16 move;
    int moveEffect;
    int i, numStatusMoves;
    BOOL result;

    // Early exit if enemy mon is already taunted
    if (battleCtx->battleMons[defender].moveEffectsData.tauntedTurns > 0) {
        result = TRUE;
        return result;
    }

    // Early exit for Magic Bounce
    if (Battler_IgnorableAbility(battleCtx, attacker, defender, ABILITY_MAGIC_BOUNCE)) {
        result = FALSE;
        return result;
    }

    result = FALSE;
    numStatusMoves = 0;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = battleCtx->battleMons[defender].moves[i];
        moveEffect = MOVE_DATA(move).effect;

        if (move == MOVE_NONE) {
            break;
        }

        if (MOVE_DATA(move).class == CLASS_STATUS) {

            if (MapBattleEffectToSelfStatBoost(battleCtx, moveEffect) != BATTLE_STAT_FLAG_NONE) {
                result = TRUE;
                break;
            }

            if(MapBattleEffectToStatusCondition(battleCtx, moveEffect) != MON_CONDITION_NONE) {
                result = TRUE;
                break;
            }

            switch (moveEffect) {
                default:
                    numStatusMoves++;
                    break;

                case BATTLE_EFFECT_STEALTH_ROCK:
                case BATTLE_EFFECT_SET_SPIKES:
                case BATTLE_EFFECT_TOXIC_SPIKES:
                case BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN:
                    result = TRUE;
                    numStatusMoves++;
                    break;

                case BATTLE_EFFECT_RESTORE_HALF_HP:
                case BATTLE_EFFECT_REST:
                case BATTLE_EFFECT_AVERAGE_HP:
                case BATTLE_EFFECT_CURE_PARTY_STATUS:
                case BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN:
                case BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE:
                case BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON:
                case BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON:
                case BATTLE_EFFECT_HEAL_IN_3_TURNS:
                case BATTLE_EFFECT_SWALLOW:
                    result = TRUE;
                    numStatusMoves++;
                    break;

                // Volatile statuses
                case BATTLE_EFFECT_CONFUSE_ALL:
                case BATTLE_EFFECT_STATUS_CONFUSE:
                case BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION:
                case BATTLE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION:
                case BATTLE_EFFECT_ENCORE:
                case BATTLE_EFFECT_TORMENT:
                case BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE:
                case BATTLE_EFFECT_DISABLE:
                case BATTLE_EFFECT_MAKE_GLOBAL_TARGET:
                case BATTLE_EFFECT_INFATUATE:
                case BATTLE_EFFECT_SET_SUBSTITUTE:
                    result = TRUE;
                    numStatusMoves++;
                    break;

                case BATTLE_EFFECT_TAUNT:
                    speedCompare = BattleSystem_CompareBattlerSpeed(battleSys, battleCtx, attacker, defender, TRUE);
                    if (speedCompare == COMPARE_SPEED_FASTER || speedCompare == COMPARE_SPEED_TIE) {
                        result = TRUE;
                    }
                    numStatusMoves++;
                    break;

                // Moves we don't need to taunt for
                case BATTLE_EFFECT_DO_NOTHING:
                case BATTLE_EFFECT_COPY_MOVE:
                case BATTLE_EFFECT_COPY_MOVE_FOR_BATTLE:
                case BATTLE_EFFECT_CALL_RANDOM_MOVE:
                case BATTLE_EFFECT_LEARN_MOVE_PERMANENT:
                    break;

                // Moves we will not usually taunt for
                case BATTLE_EFFECT_CONVERSION:
                case BATTLE_EFFECT_CONVERSION2:
                case BATTLE_EFFECT_REMOVE_ALL_PP_ON_DEFEAT:
                case BATTLE_EFFECT_HALVE_ELECTRIC_DAMAGE:
                case BATTLE_EFFECT_CHANGE_TYPE_WITH_WEATHER:
                case BATTLE_EFFECT_HALVE_FIRE_DAMAGE:
                case BATTLE_EFFECT_USE_LAST_USED_MOVE:
                    if (BattleSystem_RandNext(battleSys) % 8 == 0) {
                        numStatusMoves++;
                    }
                    break;

                case BATTLE_EFFECT_PROTECT:
                    // Normally, we wait til after the target has clicked protect to taunt
                    if(battleCtx->battleMons[defender].moveEffectsData.protectSuccessTurns > 0
                    || battleCtx->totalTurns - battleCtx->battleMons[defender].moveEffectsData.fakeOutTurnNumber > 1) {
                        numStatusMoves++;
                    }
                    else {
                        // 10% chance to taunt turn 1 or 2 out
                        if (BattleSystem_RandNext(battleSys) % 10 == 0) {
                            numStatusMoves++;
                        }
                    }
                    break;
            }
        }
    }


    if (numStatusMoves > 1) {
        result = TRUE;
    }
    else {
        if (numStatusMoves == 1) {
            if (BattleSystem_RandNext(battleSys) % 8 == 0) {
                result = TRUE;
            }
        }
    }

    return result;
}

BOOL AI_PartyMonShouldTauntCheck(BattleSystem *battleSys, BattleContext *battleCtx, int attacker, int defender, int partySlot, int partyIndicator)
{
    u8 monItemEffect;
    u16 move, monSpeedStat;
    int moveEffect;
    int i, numStatusMoves;
    Pokemon *mon;
    BOOL result;

    // Early exit if enemy mon is already taunted
    if (battleCtx->battleMons[defender].moveEffectsData.tauntedTurns > 0) {
        result = TRUE;
        return result;
    }

    mon = BattleSystem_PartyPokemon(battleSys, partyIndicator, partySlot);

    // Early exit for Magic Bounce
    if (PartyMon_IgnorableAbility(battleCtx, mon, defender, ABILITY_MAGIC_BOUNCE)) {
        result = FALSE;
        return result;
    }

    result = FALSE;
    numStatusMoves = 0;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = battleCtx->battleMons[defender].moves[i];
        moveEffect = MOVE_DATA(move).effect;

        if (move == MOVE_NONE) {
            break;
        }

        if (MOVE_DATA(move).class == CLASS_STATUS) {

            if (MapBattleEffectToSelfStatBoost(battleCtx, moveEffect) != BATTLE_STAT_FLAG_NONE) {
                result = TRUE;
                break;
            }

            if(MapBattleEffectToStatusCondition(battleCtx, moveEffect) != MON_CONDITION_NONE) {
                result = TRUE;
                break;
            }

            switch (moveEffect) {
                default:
                    numStatusMoves++;
                    break;

                case BATTLE_EFFECT_STEALTH_ROCK:
                case BATTLE_EFFECT_SET_SPIKES:
                case BATTLE_EFFECT_TOXIC_SPIKES:
                case BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN:
                    result = TRUE;
                    numStatusMoves++;
                    break;

                case BATTLE_EFFECT_RESTORE_HALF_HP:
                case BATTLE_EFFECT_REST:
                case BATTLE_EFFECT_AVERAGE_HP:
                case BATTLE_EFFECT_CURE_PARTY_STATUS:
                case BATTLE_EFFECT_HEAL_HALF_MORE_IN_SUN:
                case BATTLE_EFFECT_HEAL_HALF_REMOVE_FLYING_TYPE:
                case BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON:
                case BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON:
                case BATTLE_EFFECT_HEAL_IN_3_TURNS:
                case BATTLE_EFFECT_SWALLOW:
                    result = TRUE;
                    numStatusMoves++;
                    break;

                // Volatile statuses
                case BATTLE_EFFECT_CONFUSE_ALL:
                case BATTLE_EFFECT_STATUS_CONFUSE:
                case BATTLE_EFFECT_ATK_UP_2_STATUS_CONFUSION:
                case BATTLE_EFFECT_SP_ATK_UP_CAUSE_CONFUSION:
                case BATTLE_EFFECT_ENCORE:
                case BATTLE_EFFECT_TORMENT:
                case BATTLE_EFFECT_MAKE_SHARED_MOVES_UNUSEABLE:
                case BATTLE_EFFECT_DISABLE:
                case BATTLE_EFFECT_MAKE_GLOBAL_TARGET:
                case BATTLE_EFFECT_INFATUATE:
                case BATTLE_EFFECT_SET_SUBSTITUTE:
                    result = TRUE;
                    numStatusMoves++;
                    break;

                case BATTLE_EFFECT_TAUNT:
                    monItemEffect = BattleSystem_GetItemData(battleCtx, Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL), ITEM_PARAM_HOLD_EFFECT);
                    monSpeedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
                    switch (monItemEffect) {
                        default:
                            break;

                        case HOLD_EFFECT_LVLUP_ATK_EV_UP:
                        case HOLD_EFFECT_LVLUP_DEF_EV_UP:
                        case HOLD_EFFECT_LVLUP_SPATK_EV_UP:
                        case HOLD_EFFECT_LVLUP_SPDEF_EV_UP:
                        case HOLD_EFFECT_LVLUP_SPEED_EV_UP:
                        case HOLD_EFFECT_LVLUP_HP_EV_UP:
                        case HOLD_EFFECT_EVS_UP_SPEED_DOWN:
                        case HOLD_EFFECT_SPEED_DOWN_GROUNDED:
                            monSpeedStat /= 2;
                            break;

                        case HOLD_EFFECT_DITTO_SPEED_UP:
                            if (Pokemon_GetValue(mon, MON_DATA_SPECIES, NULL) == SPECIES_DITTO) {
                                monSpeedStat *= 2;
                            }
                            break;

                        case HOLD_EFFECT_CHOICE_SPEED:
                            monSpeedStat = monSpeedStat * 3 / 2;
                            break;
                    }

                    if (monSpeedStat >= battleCtx->battleMons[defender].speed) {
                        result = TRUE;
                    }
                    numStatusMoves++;
                    break;

                // Moves we don't need to taunt for
                case BATTLE_EFFECT_DO_NOTHING:
                case BATTLE_EFFECT_COPY_MOVE:
                case BATTLE_EFFECT_COPY_MOVE_FOR_BATTLE:
                case BATTLE_EFFECT_CALL_RANDOM_MOVE:
                case BATTLE_EFFECT_LEARN_MOVE_PERMANENT:
                    break;

                // Moves we will not usually taunt for
                case BATTLE_EFFECT_CONVERSION:
                case BATTLE_EFFECT_CONVERSION2:
                case BATTLE_EFFECT_REMOVE_ALL_PP_ON_DEFEAT:
                case BATTLE_EFFECT_HALVE_ELECTRIC_DAMAGE:
                case BATTLE_EFFECT_CHANGE_TYPE_WITH_WEATHER:
                case BATTLE_EFFECT_HALVE_FIRE_DAMAGE:
                case BATTLE_EFFECT_USE_LAST_USED_MOVE:
                    if (BattleSystem_RandNext(battleSys) % 8 == 0) {
                        numStatusMoves++;
                    }
                    break;

                case BATTLE_EFFECT_PROTECT:
                    // Normally, we wait til after the target has clicked protect to taunt
                    if(battleCtx->battleMons[defender].moveEffectsData.protectSuccessTurns > 0
                    || battleCtx->totalTurns - battleCtx->battleMons[defender].moveEffectsData.fakeOutTurnNumber > 1) {
                        numStatusMoves++;
                    }
                    else {
                        // 10% chance to taunt turn 1 or 2 out
                        if (BattleSystem_RandNext(battleSys) % 10 == 0) {
                            numStatusMoves++;
                        }
                    }
                    break;
            }
        }
    }


    if (numStatusMoves > 1) {
        result = TRUE;
    }
    else {
        if (numStatusMoves == 1) {
            if (BattleSystem_RandNext(battleSys) % 8 == 0) {
                result = TRUE;
            }
        }
    }

    return result;
}

BOOL BattleAI_ValidateSwitch(BattleSystem *battleSys, int battler)
{
    // Must keep C89-style declaration to match
    int i, j;
    u8 defender, defenderType1, defenderType2, defenderAbility;
    u8 monType1, monType2, monAbility, monItemEffect;
    u8 battlerType1, battlerType2, battlerAbility, side, oppSide;
    u16 monSpecies, monSpeedStat, monItem, defenderItem;
    u16 move;
    int moveType, movePower, moveEffect, moveStatFlag;
    u8 battlersDisregarded;
    u8 slot1, slot2;
    u32 moveStatusFlags;
    int partySize;
    int score, maxScore, moveScore, activeScore;
    int hpPercent, monCurHP, monMaxHP;
    int hazardsBonus, sackBonus, speedMultiplier;
	int moveMoveEffect, moveVolatileStatus, moveStatus;
	int defenderCurHP, defenderMaxHP, defenderSpeedStat;
    Pokemon *mon;
    BattleContext *battleCtx;
    BOOL shouldSwitch;

    battleCtx = BattleSystem_Context(battleSys);

    activeScore = 0;
    shouldSwitch = FALSE;
	
    slot1 = battler;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG)
            || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        slot2 = slot1;
    } else {
        slot2 = BattleSystem_Partner(battleSys, battler);
    }

    defender = BattleSystem_RandomOpponent(battleSys, battleCtx, battler);
    partySize = BattleSystem_PartyCount(battleSys, battler);
    battlersDisregarded = 0;
    side = Battler_Side(battleSys, battler);
    oppSide = Battler_Side(battleSys, defender);

    // Use the AI's list of known moves

    defenderType1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL);
    defenderType2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL);
    defenderAbility = Battler_Ability(battleCtx, defender);
    defenderItem = BattleMon_Get(battleCtx, defender, BATTLEMON_HELD_ITEM, NULL);
	defenderMaxHP = BattleMon_Get(battleCtx, defender, BATTLEMON_MAX_HP, NULL);
    defenderCurHP = BattleMon_Get(battleCtx, defender, BATTLEMON_CUR_HP, NULL);
	defenderSpeedStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SPEED, NULL);

    if (defenderCurHP <= 0) {
        defenderCurHP = 1;
    }
    if (defenderMaxHP <= 0) {
        defenderMaxHP = 1;
    }

    i = battleCtx->selectedPartySlot[battler];

    mon = BattleSystem_PartyPokemon(battleSys, battler, i);
    monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);
    monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
    monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
    monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);

    if (monSpecies != SPECIES_NONE
        && monSpecies != SPECIES_EGG
        && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL)) {

        monItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
        monItemEffect = BattleSystem_GetItemData(battleCtx, monItem, ITEM_PARAM_HOLD_EFFECT);

        hazardsBonus = 0;
        sackBonus = 0;
        monCurHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
        monMaxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);
        if (monCurHP <= 0) {
            monCurHP = 1;
        }
        if (monMaxHP <= 0) {
            monMaxHP = 1;
        }
        hpPercent = (monCurHP * 100) / monMaxHP;

        monSpeedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
        // speed bonuses are inverted for defensive score
        if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_SLOWER) {

            speedMultiplier = 9;
        }
        else if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_TIE) {

            speedMultiplier = 10;
        }
        else {

            speedMultiplier = 18;
        }

        for (j = 0; j < LEARNED_MOVES_MAX; j++) {

            if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE) {

                move = battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove;
            }
            else {
                move = battleCtx->aiContext.battlerMoves[defender][j];
            }

            if (move == MOVE_NONE) {
                break;
            }

            moveScore = 0;
            movePower = MOVE_DATA(move).power;
            moveType = CalcMoveType(battleSys, battleCtx, defender, defenderItem, move);
            moveEffect = MOVE_DATA(move).effect;

            if (movePower > 0) {

                moveScore = BattleSystem_CalcPartyMemberMoveDamage(battleSys,
                        battleCtx,
                        move,
                        battleCtx->sideConditionsMask[oppSide],
                        battleCtx->fieldConditionsMask,
                        0,
                        moveType,
                        defender,
                        battler,
                        1,
                        battler,
                        i);
                        
                moveStatusFlags = 0;
                moveScore = PartyMon_ApplyTypeChart(battleSys,
                            battleCtx,
                            move,
                            moveType,
                            defender,
                            battler,
                            moveScore,
                            battler,
                            i,
                            &moveStatusFlags);

                if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                    && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                        moveScore = 0;
                }

                if (moveStatusFlags & (MOVE_STATUS_TYPE_RESIST_ABILITY | MOVE_STATUS_TYPE_WEAKNESS_ABILITY)) {
                    if (defenderAbility != ABILITY_MOLD_BREAKER) {
                        if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                            moveScore /= 2;
                        }

                        if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                            moveScore = moveScore * 3 / 2;
                        }
                    }
                }

                // Move score is % of mon's hp damage they'll take
                moveScore = moveScore * 100 / monMaxHP;

                // Penalize choices that would lose more than half health from
                // switching in.
                if (moveScore > (Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL) / 2)) {

                    moveScore += 100;
                }

                // Penalize choices that would result in a KO, except for Sacks
                if (moveScore > monCurHP) {
                    if (hpPercent < 50) {
                        sackBonus = (monMaxHP - monCurHP) * 100 / monMaxHP;
                        if (moveScore < sackBonus) {
                            moveScore = 0;
                        }
                        else {
                            moveScore -= sackBonus;
                        }
                    }
                }
            }
            else {
                moveScore = PartyMon_ApplyTypeChart(battleSys,
                            battleCtx,
                            move,
                            moveType,
                            defender,
                            battler,
                            0,
                            battler,
                            i,
                            &moveStatusFlags);
                moveMoveEffect = MapBattleEffectToMoveEffect(battleCtx, moveEffect);
                moveStatus = MapBattleEffectToStatusCondition(battleCtx, moveEffect);
                moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, moveEffect);

                if (((moveStatusFlags & MOVE_STATUS_IMMUNE) == FALSE)
                    || (moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY)) {
                    if (moveStatus != MON_CONDITION_NONE
                        && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY) == FALSE) {
                        if (moveStatus & MON_CONDITION_BURN) {
                            if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_BURN) == FALSE)
                            || defenderAbility == ABILITY_MOLD_BREAKER)
                            && monType1 != TYPE_FIRE
                            && monType2 != TYPE_FIRE)
                            if (Battle_PartyMonIsPhysicalAttacker(battleSys, battleCtx, battler, i)) {
                                moveScore = 100;
                            }
                        }

                        if (moveStatus & MON_CONDITION_PARALYSIS) {
                            if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_PARALYSIS) == FALSE)
                            || defenderAbility == ABILITY_MOLD_BREAKER)
                            && monType1 != TYPE_ELECTRIC
                            && monType2 != TYPE_ELECTRIC) {

                                if (AI_PartyMonShouldParalyzeCheck(battleSys, battleCtx, battler, i, defenderSpeedStat)) {
                                    moveScore = 50;
                                }
                            }
                        }

                        if (moveStatus & MON_CONDITION_SLEEP) {
                            if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_SLEEP) == FALSE)
                            || defenderAbility == ABILITY_MOLD_BREAKER) {

                                moveScore = 47;
                            }
                        }

                        if (moveStatus & MON_CONDITION_ANY_POISON) {
                            if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_ANY_POISON) == FALSE)
                            || defenderAbility == ABILITY_MOLD_BREAKER) {

                                moveScore = 25;
                            }
                        }
                    }

                    if (moveVolatileStatus != VOLATILE_CONDITION_NONE) {
                        if (moveVolatileStatus & VOLATILE_CONDITION_CONFUSION) {
                            if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender)) {
                                moveScore = 20;
                            }
                            else {
                                moveScore = 10;
                            }
                        }

                        if (moveVolatileStatus & VOLATILE_CONDITION_ATTRACT) {
                            if (BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL) != Pokemon_GetValue(mon, MON_DATA_GENDER, NULL)
                                && Pokemon_GetValue(mon, MON_DATA_GENDER, NULL) != GENDER_NONE) {
                                    moveScore = 25;
                            }
                        }

                        if (moveVolatileStatus & VOLATILE_CONDITION_MOVE_LOCKED) {
                            moveScore = 25;
                        }

                        if (moveVolatileStatus & VOLATILE_CONDITION_TORMENT) {
                            moveScore = 10;
                        }

                        if (moveVolatileStatus & VOLATILE_CONDITION_CURSE) {
                            if (defenderType1 == TYPE_GHOST || defenderType2 == TYPE_GHOST) {
                                moveScore = 49;
                            }
                        }
                    }

                    if (moveMoveEffect != MOVE_EFFECT_NONE) {
                        if (moveMoveEffect & MOVE_EFFECT_YAWN) {
                            if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_SLEEP) == FALSE)
                            || defenderAbility == ABILITY_MOLD_BREAKER) {

                            // Yawn is handled by sleep status, so we just give it +2 here
                                moveScore += 2;
                            }
                        }

                        if (moveMoveEffect & MOVE_EFFECT_LEECH_SEED) {
                            if (monType1 != TYPE_GRASS && monType2 != TYPE_GRASS) {
                                moveScore = 25;
                            }
                        }

                        if (moveMoveEffect & MOVE_EFFECT_PERISH_SONG) {
                            if (monAbility != ABILITY_SOUNDPROOF) {
                                    
                                moveScore = 15;
                            }
                        }
                    }

                    if (moveEffect == BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN) {
                        if ((monAbility == ABILITY_DEFIANT
                            || monAbility == ABILITY_COMPETITIVE)
                            && defenderAbility != ABILITY_UNAWARE) {
                                moveScore = 0;
                        }
                        else {
                            if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                                moveScore = 10 * battleCtx->sideConditions[side].spikesLayers;
                                moveScore += (8 * battleCtx->sideConditions[side].toxicSpikesLayers);
                                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_STEALTH_ROCK) {
                                    moveScore += 12;
                                }
                            }
                            else {
                                moveScore = 7;
                            }
                        }
                    }
                }
            }

            activeScore += moveScore;

            if (i != battleCtx->selectedPartySlot[battler])
            {
                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                    if (Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL) == MOVE_DEFOG
                        || Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL) == MOVE_RAPID_SPIN) {

                        hazardsBonus = (hpPercent / 4) + 2;

                        if (score < hazardsBonus) {
                            activeScore = 0;
                        }
                        else {
                            activeScore -= hazardsBonus;
                        }
                    }
                }
            }
                    
            if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE
                && move == battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove) {
                break;
            }
        }

        if (speedMultiplier != 10) {

            activeScore = activeScore * speedMultiplier / 10;
        }
	}


    for (i = 0; i < partySize; i++) {
        mon = BattleSystem_PartyPokemon(battleSys, battler, i);
        monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);
        monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
        monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
        monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);

        if (monSpecies != SPECIES_NONE
            && monSpecies != SPECIES_EGG
            && Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL)
            && (battlersDisregarded & FlagIndex(i)) == FALSE
            && battleCtx->selectedPartySlot[slot1] != i
            && battleCtx->selectedPartySlot[slot2] != i
            && i != battleCtx->aiSwitchedPartySlot[slot1]
            && i != battleCtx->aiSwitchedPartySlot[slot2]) {

            monItem = Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL);
            monItemEffect = BattleSystem_GetItemData(battleCtx, monItem, ITEM_PARAM_HOLD_EFFECT);

            hazardsBonus = 0;
            sackBonus = 0;
            monCurHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
            monMaxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);
            if (monCurHP <= 0) {
                monCurHP = 1;
            }
            if (monMaxHP <= 0) {
                monMaxHP = 1;
            }
            hpPercent = (monCurHP * 100) / monMaxHP;
            score = 0;

            monSpeedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
            // speed bonuses are inverted for defensive score
            if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_SLOWER) {

                speedMultiplier = 9;
            }
            else if (BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, battler, i, TRUE) == COMPARE_SPEED_TIE) {

                speedMultiplier = 10;
            }
            else {

                speedMultiplier = 18;
            }

            for (j = 0; j < LEARNED_MOVES_MAX; j++) {

                if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE) {

                    move = battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove;
                }
                else {
                    move = battleCtx->aiContext.battlerMoves[defender][j];
                }

                if (move == MOVE_NONE) {
                    break;
                }

                moveScore = 0;
                movePower = MOVE_DATA(move).power;
                moveType = CalcMoveType(battleSys, battleCtx, defender, defenderItem, move);
                moveEffect = MOVE_DATA(move).effect;

                if (movePower > 0) {

                    moveScore = BattleSystem_CalcPartyMemberMoveDamage(battleSys,
                            battleCtx,
                            move,
                            battleCtx->sideConditionsMask[oppSide],
                            battleCtx->fieldConditionsMask,
                            0,
                            moveType,
                            defender,
                            battler,
                            1,
                            battler,
                            i);
                        
                    moveStatusFlags = 0;
                    moveScore = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                move,
                                moveType,
                                defender,
                                battler,
                                moveScore,
                                battler,
                                i,
                                &moveStatusFlags);

                    if ((moveStatusFlags & MOVE_STATUS_IMMUNE)
                        && ((moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                            moveScore = 0;
                    }

                    if (moveStatusFlags & (MOVE_STATUS_TYPE_RESIST_ABILITY | MOVE_STATUS_TYPE_WEAKNESS_ABILITY)) {
                        if (defenderAbility != ABILITY_MOLD_BREAKER) {
                            if (moveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                                moveScore /= 2;
                            }

                            if (moveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                                moveScore = moveScore * 3 / 2;
                            }
                        }
                    }

                    // Move score is % of mon's hp damage they'll take
                    moveScore = moveScore * 100 / monMaxHP;

                    // Penalize choices that would lose more than half health from
                    // switching in.
                    if (moveScore > (Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL) / 2)) {

                        moveScore += 100;
                    }

                    // Penalize choices that would result in a KO, except for Sacks
                    if (moveScore > (monCurHP - Battle_CalcHazardsDamage(battleSys, battleCtx, battler, i))) {
                        if (hpPercent < 50) {
                            sackBonus = (monMaxHP - monCurHP) * 100 / monMaxHP;
                            if (moveScore < sackBonus) {
                                moveScore = 0;
                            }
                            else {
                                moveScore -= sackBonus;
                            }
                        }
                    }
                }
                else {
                    moveScore = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                move,
                                moveType,
                                defender,
                                battler,
                                0,
                                battler,
                                i,
                                &moveStatusFlags);
                    moveMoveEffect = MapBattleEffectToMoveEffect(battleCtx, moveEffect);
                    moveStatus = MapBattleEffectToStatusCondition(battleCtx, moveEffect);
                    moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, moveEffect);

                    if (((moveStatusFlags & MOVE_STATUS_IMMUNE) == FALSE)
                        || (moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY)) {
                        if (moveStatus != MON_CONDITION_NONE
                            && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY) == FALSE) {
                            if (moveStatus & MON_CONDITION_BURN) {
                                if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_BURN) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER)
                                && monType1 != TYPE_FIRE
                                && monType2 != TYPE_FIRE)
                                if (Battle_PartyMonIsPhysicalAttacker(battleSys, battleCtx, battler, i)) {
                                    moveScore = 100;
                                }
                            }

                            if (moveStatus & MON_CONDITION_PARALYSIS) {
                                if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_PARALYSIS) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER)
                                && monType1 != TYPE_ELECTRIC
                                && monType2 != TYPE_ELECTRIC) {

                                    if (AI_PartyMonShouldParalyzeCheck(battleSys, battleCtx, battler, i, defenderSpeedStat)) {
                                        moveScore = 50;
                                    }
                                }
                            }

                            if (moveStatus & MON_CONDITION_SLEEP) {
                                if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_SLEEP) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER) {

                                    moveScore = 47;
                                }
                            }

                            if (moveStatus & MON_CONDITION_ANY_POISON) {
                                if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_ANY_POISON) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER) {

                                    moveScore = 25;
                                }
                            }
                        }

                        if (moveVolatileStatus != VOLATILE_CONDITION_NONE) {
                            if (moveVolatileStatus & VOLATILE_CONDITION_CONFUSION) {
                                if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender)) {
                                    moveScore = 20;
                                }
                                else {
                                    moveScore = 10;
                                }
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_ATTRACT) {
                                if (BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL) != Pokemon_GetValue(mon, MON_DATA_GENDER, NULL)
                                    && Pokemon_GetValue(mon, MON_DATA_GENDER, NULL) != GENDER_NONE) {
                                        moveScore = 25;
                                }
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_MOVE_LOCKED) {
                                moveScore = 25;
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_TORMENT) {
                                moveScore = 10;
                            }

                            if (moveVolatileStatus & VOLATILE_CONDITION_CURSE) {
                                if (defenderType1 == TYPE_GHOST || defenderType2 == TYPE_GHOST) {
                                    moveScore = 49;
                                }
                            }
                        }

                        if (moveMoveEffect != MOVE_EFFECT_NONE) {
                            if (moveMoveEffect & MOVE_EFFECT_YAWN) {
                                if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_SLEEP) == FALSE)
                                || defenderAbility == ABILITY_MOLD_BREAKER) {

                                // Yawn is handled by sleep status, so we just give it +2 here
                                    moveScore += 2;
                                }
                            }

                            if (moveMoveEffect & MOVE_EFFECT_LEECH_SEED) {
                                if (monType1 != TYPE_GRASS && monType2 != TYPE_GRASS) {
                                    moveScore = 25;
                                }
                            }

                            if (moveMoveEffect & MOVE_EFFECT_PERISH_SONG) {
                                if (monAbility != ABILITY_SOUNDPROOF) {
                                    
                                    moveScore = 15;
                                }
                            }
                        }

                        if (moveEffect == BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN) {
                            if ((monAbility == ABILITY_DEFIANT
                                || monAbility == ABILITY_COMPETITIVE)
                                && defenderAbility != ABILITY_UNAWARE) {
                                    moveScore = 0;
                            }
                            else {
                                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                                    moveScore = 10 * battleCtx->sideConditions[side].spikesLayers;
                                    moveScore += (8 * battleCtx->sideConditions[side].toxicSpikesLayers);
                                    if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_STEALTH_ROCK) {
                                        moveScore += 12;
                                    }
                                }
                                else {
                                    moveScore = 7;
                                }
                            }
                        }
                    }
                }

                score += moveScore;

                if (i != battleCtx->selectedPartySlot[battler])
                {
                    if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                        if (Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL) == MOVE_DEFOG
                            || Pokemon_GetValue(mon, MON_DATA_MOVE1 + j, NULL) == MOVE_RAPID_SPIN) {

                            hazardsBonus = (hpPercent / 4) + 2;

                            if (score < hazardsBonus) {
                                score = 0;
                            }
                            else {
                                score -= hazardsBonus;
                            }
                        }
                    }

                    if (moveScore > 50
                    && hpPercent >= 25) {
                        battlersDisregarded |= FlagIndex(i);
                    }
                }
                    
                if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE
                    && move == battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove) {
                    break;
                }
            }

            if (speedMultiplier != 10) {

                score = score * speedMultiplier / 10;
            }

            if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {

                if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_TOXIC_SPIKES) {
                    hazardsBonus = (hpPercent / 4) + 2;

                    if (monType1 == TYPE_POISON
                    || monType2 == TYPE_POISON) {
                        if (score < hazardsBonus) {
                            score = 0;
                        }
                        else {
                            score -= hazardsBonus;
                        }
                    }

                    if (monType1 == TYPE_STEEL
                    || monType2 == TYPE_STEEL) {
                        if (score < (hazardsBonus / 2)) {
                            score = 0;
                        }
                        else {
                            score -= (hazardsBonus / 2);
                        }
                    }
                }

                if (monAbility != ABILITY_MAGIC_GUARD) {
                    score += Battle_CalcHazardsDamage(battleSys, battleCtx, battler, i) * 200 / monMaxHP;
                }
            }

            if (activeScore >= score) {

                battlersDisregarded |= FlagIndex(i);
            }

            if ((battlersDisregarded & FlagIndex(i)) == FALSE) {
                shouldSwitch = TRUE;
            }
        }
    }

    return shouldSwitch;
}

BOOL BattleSystem_TriggerAttackerAbilityOnHit(BattleSystem *battleSys, BattleContext *battleCtx, int *subscript)
{
    BOOL result = FALSE;

    // These two sentinels must be separate to match
    if (battleCtx->defender == BATTLER_NONE) {
        return result;
    }

    if (Battler_SubstituteWasHit(battleCtx, battleCtx->defender) == TRUE) {
        return result;
    }

	switch (Battler_Ability(battleCtx, battleCtx->attacker))
	{
		case ABILITY_POISON_TOUCH:
			if (DEFENDING_MON.curHP
			&& DEFENDING_MON.status == MON_CONDITION_NONE
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
			&& (CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT)
            && (Battler_HeldItemEffect(battleCtx, battleCtx->attacker) != HOLD_EFFECT_NO_CONTACT_BOOST_PUNCH)
			&& BattleSystem_RandNext(battleSys) % 10 < 3)
			{
				battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
				battleCtx->sideEffectMon = battleCtx->defender;
				battleCtx->msgBattlerTemp = battleCtx->attacker;

				*subscript = subscript_poison;
				result = TRUE;
			}
			break;
			
		case ABILITY_RIVALRY:
			if (ATTACKING_MON.curHP
			&& (ATTACKING_MON.rivalryFlag == TRUE)
            && (ATTACKING_MON.statusVolatile & VOLATILE_CONDITION_ATTRACT) == FALSE
            && (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
            && (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
            && (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
            && (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
            && DEFENDING_MON.curHP
            && BattleSystem_RandNext(battleSys) % 10 < 3)
			{
				battleCtx->sideEffectType = SIDE_EFFECT_TYPE_ABILITY;
				battleCtx->sideEffectMon = battleCtx->attacker;
				battleCtx->msgBattlerTemp = battleCtx->defender;
				
				*subscript = subscript_infatuate_rivalry;
				result = TRUE;
			}
			break;
			
		case ABILITY_FREE_SAMPLE:
			if (DEFENDING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken)
			&& (CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT)
			&& BattleSystem_RandNext(battleSys) % 10 < 3)
			{
				*subscript = subscript_pluck_ability;
				result = TRUE;
			}
			break;
			
		case ABILITY_SHAKEDOWN:
			if (DEFENDING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (battleCtx->battleMons[battleCtx->defender].moveEffectsData.embargoTurns == 0)
			&& (CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT)
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken))
			{
				*subscript = subscript_embargo_start_ability;
				result = TRUE;
			}
			break;
			
		case ABILITY_STRANGLE_WEED:
			if (DEFENDING_MON.curHP
			&& (battleCtx->moveStatusFlags & MOVE_STATUS_NO_EFFECTS) == FALSE
			&& (battleCtx->battleStatusMask & SYSCTL_FIRST_OF_MULTI_TURN) == FALSE
			&& (battleCtx->battleStatusMask2 & SYSCTL_UTURN_ACTIVE) == FALSE
			&& (CURRENT_MOVE_DATA.flags & MOVE_FLAG_MAKES_CONTACT)
			&& (MOVE_DATA(battleCtx->moveCur).type == TYPE_GRASS)
			&& (DEFENDER_SELF_TURN_FLAGS.physicalDamageTaken || DEFENDER_SELF_TURN_FLAGS.specialDamageTaken))
			{
				battleCtx->sideEffectMon = battleCtx->defender;
				battleCtx->msgMoveTemp = MOVE_WRAP;
				*subscript = subscript_bind_start_ability;
				result = TRUE;
			}
			break;
	}

    return result;
}

int BattleAI_CalculateStatusMoveAttackScore(BattleSystem *battleSys, BattleContext *battleCtx, int defender, int battler, int partyIndicator, int partySlot)
{
    int i;
    int score, moveScore;
    int defenderCurHP, defenderMaxHP;
    int moveMoveEffect, moveVolatileStatus, moveStatus, moveStatFlag, moveEffect;
    u8 moveType;
    u8 monType1, monType2, monAbility;
    u8 defenderItemEffect, defenderItemPower, defenderType1, defenderType2, defenderAbility;
	u8 side, oppSide;
    u8 compareSpeedDefenderVsMon;
    u16 move, monSpeedStat, defenderItem;
    u32 moveStatusFlags;

    Pokemon *mon = BattleSystem_PartyPokemon(battleSys, partyIndicator, partySlot);

    score = 0;

    defenderType1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL);
    defenderType2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL);
    defenderAbility = Battler_Ability(battleCtx, defender);

    defenderItem = BattleMon_Get(battleCtx, defender, BATTLEMON_HELD_ITEM, NULL);
    defenderItemEffect = Battler_HeldItemEffect(battleCtx, defender);
    defenderItemPower = Battler_HeldItemPower(battleCtx, defender, ITEM_POWER_CHECK_ALL);

	defenderMaxHP = BattleMon_Get(battleCtx, defender, BATTLEMON_MAX_HP, NULL);
    defenderCurHP = BattleMon_Get(battleCtx, defender, BATTLEMON_CUR_HP, NULL);

    monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
    monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
    monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    monSpeedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
	
	side = Battler_Side(battleSys, battler);
    oppSide = Battler_Side(battleSys, defender);

    compareSpeedDefenderVsMon = BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, partyIndicator, partySlot, TRUE);

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        move = Pokemon_GetValue(mon, MON_DATA_MOVE1 + i, NULL);

        if (MOVE_DATA(move).class == CLASS_STATUS) {
            moveStatusFlags = 0;
            moveType = Move_CalcVariableType(battleSys, battleCtx, mon, move);
			moveEffect = MOVE_DATA(move).effect;
            moveScore = PartyMon_ApplyTypeChart(battleSys,
                        battleCtx,
                        move,
                        moveType,
                        battler,
                        defender,
                        0,
                        partyIndicator,
                        partySlot,
                        &moveStatusFlags);

            moveMoveEffect = MapBattleEffectToMoveEffect(battleCtx, moveEffect);
            moveStatus = MapBattleEffectToStatusCondition(battleCtx, moveEffect);
            moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, moveEffect);
            moveStatFlag = MapBattleEffectToSelfStatBoost(battleCtx, moveEffect);

            if (((moveStatusFlags & MOVE_STATUS_IMMUNE) == FALSE)
                || (moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY)) {

                moveScore = 10;

                if (moveStatus != MON_CONDITION_NONE
                    && (battleCtx->battleMons[defender].status & MON_CONDITION_ANY) == FALSE) {
                    if (moveStatus & MON_CONDITION_BURN) {
                        if (((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_BURN) == FALSE)
                        || monAbility == ABILITY_MOLD_BREAKER)
                        && defenderType1 != TYPE_FIRE
                        && defenderType2 != TYPE_FIRE)
                        if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender)) {
                            moveScore = 75;
                        }
                    }

                    if (moveStatus & MON_CONDITION_PARALYSIS) {
                        if (((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_PARALYSIS) == FALSE)
                        || monAbility == ABILITY_MOLD_BREAKER)
                        && defenderType1 != TYPE_ELECTRIC
                        && defenderType2 != TYPE_ELECTRIC) {

                            if (AI_ShouldParalyzeCheck(battleSys, battleCtx, defender, monSpeedStat)) {
                                moveScore = 50;
                            }
                        }
                    }

                    if (moveStatus & MON_CONDITION_SLEEP) {
                        if ((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_SLEEP) == FALSE)
                        || monAbility == ABILITY_MOLD_BREAKER) {

                            moveScore = 50;
                        }
                    }

                    if (moveStatus & MON_CONDITION_ANY_POISON) {
                        if ((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_ANY_POISON) == FALSE)
                        || monAbility == ABILITY_MOLD_BREAKER) {

                            moveScore = 25;
                        }
                    }
                }

                if (moveVolatileStatus != VOLATILE_CONDITION_NONE) {
                    if (moveVolatileStatus & VOLATILE_CONDITION_CONFUSION) {
                        if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender)) {
                            moveScore = 25;
                        }
                        else {
                            moveScore = 15;
                        }
                    }

                    if (moveVolatileStatus & VOLATILE_CONDITION_ATTRACT) {
                        if (BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL) != Pokemon_GetValue(mon, MON_DATA_GENDER, NULL)
                            && Pokemon_GetValue(mon, MON_DATA_GENDER, NULL) != GENDER_NONE
                            && BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL) != GENDER_NONE) {
                                moveScore = 25;
                        }
                    }

                    if (moveVolatileStatus & VOLATILE_CONDITION_MOVE_LOCKED) {
                        if (battleCtx->battleMons[defender].moveEffectsData.encoredMove != MOVE_NONE) {
                            moveScore = 25;
                        }
                    }

                    if (moveVolatileStatus & VOLATILE_CONDITION_TORMENT) {
                        if ((battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_TORMENT) == FALSE) {
                            moveScore = 15;
                        }
                    }

                    if (moveVolatileStatus & VOLATILE_CONDITION_CURSE) {
                        if ((battleCtx->battleMons[defender].statusVolatile & VOLATILE_CONDITION_CURSE) == FALSE) {
                            if (monType1 == TYPE_GHOST || monType2 == TYPE_GHOST) {
                                moveScore = 49;
                            }
                        }
                    }
                }

                if (moveMoveEffect != MOVE_EFFECT_NONE) {
                    if (moveMoveEffect & MOVE_EFFECT_YAWN) {
                        if ((Battle_AbilityDetersStatus(battleSys, battleCtx, defenderAbility, MON_CONDITION_SLEEP) == FALSE)
                        || monAbility == ABILITY_MOLD_BREAKER) {

                            // Yawn is handled by sleep status, so we just give it -2 here
                            moveScore -= 2;
                        }
                    }

                    if (moveMoveEffect & MOVE_EFFECT_LEECH_SEED) {
                        if ((battleCtx->battleMons[defender].moveEffectsMask & MOVE_EFFECT_LEECH_SEED) == FALSE) {
                            if (defenderType1 != TYPE_GRASS && defenderType2 != TYPE_GRASS) {
                                moveScore = 25;
                            }
                        }
                    }

                    if (moveMoveEffect & MOVE_EFFECT_PERISH_SONG) {
                        if (defenderAbility != ABILITY_SOUNDPROOF
                            && battleCtx->battleMons[defender].moveEffectsData.perishSongTurns > 0) {
                                    
                            moveScore = 15;
                        }
                    }
                }

                // Boost to statusing, pivoting, and phazing when yawn switching
                if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_YAWN) {
                    if (moveEffect != BATTLE_EFFECT_INFATUATE
                    && (moveVolatileStatus != VOLATILE_CONDITION_NONE
                    || moveMoveEffect != MOVE_EFFECT_NONE
                    || moveStatus != MON_CONDITION_NONE)
                    ) {
                        moveScore += 25;
                    }

                    switch (moveEffect) {
                        default:
                            break;

                        // directly counter yawn
                        case BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP:
                        case BATTLE_EFFECT_FLEE_FROM_WILD_BATTLE:
                        case BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP:
                        case BATTLE_EFFECT_STATUS_LEECH_SEED:
                        case BATTLE_EFFECT_DISABLE:
                        case BATTLE_EFFECT_PREVENT_STATUS:
                        case BATTLE_EFFECT_TAUNT:
                        case BATTLE_EFFECT_SWITCH_HELD_ITEMS:
                        case BATTLE_EFFECT_APPLY_MAGIC_COAT:
                        case BATTLE_EFFECT_USE_LAST_USED_MOVE:
                            moveScore += 50;
                            break;

                        // consolation prizes
                        case BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN:
                        case BATTLE_EFFECT_HEAL_IN_3_TURNS:
                        case BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS:
                        case BATTLE_EFFECT_TOXIC_SPIKES:
                        case BATTLE_EFFECT_TRICK_ROOM:
                        case BATTLE_EFFECT_STEALTH_ROCK:
                        case BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON:
                        case BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON:
                        case BATTLE_EFFECT_SET_SPIKES:
                            moveScore += 25;
                            break;
                    }

                    if (moveVolatileStatus & VOLATILE_CONDITION_TORMENT) {
                        moveScore += 50;
                    }
                }

                if (moveEffect == BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN) {
                    if ((defenderAbility == ABILITY_DEFIANT
                        || defenderAbility == ABILITY_COMPETITIVE)
                        && monAbility != ABILITY_UNAWARE) {
                            moveScore = 0;
                    }
                    else {
                        if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                            moveScore = 10 * battleCtx->sideConditions[side].spikesLayers;
                            moveScore += (8 * battleCtx->sideConditions[side].toxicSpikesLayers);
                            if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_STEALTH_ROCK) {
                                moveScore += 20;
                            }
                            if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_STICKY_WEB) {
                                moveScore += 30;
                            }
                        }
                        if (battleCtx->sideConditionsMask[oppSide] & SIDE_CONDITION_ENEMY_DEFOG) {
                            moveScore += 5;
                            if (battleCtx->sideConditionsMask[oppSide] & SIDE_CONDITION_REFLECT) {
                                moveScore += 10;
                            }
                            if (battleCtx->sideConditionsMask[oppSide] & SIDE_CONDITION_LIGHT_SCREEN) {
                                moveScore += 10;
                            }
                        }
                    }
                }

                // Status moves that get more score when we are faster
                if (compareSpeedDefenderVsMon == COMPARE_SPEED_SLOWER
                || compareSpeedDefenderVsMon == COMPARE_SPEED_TIE) {
                    switch (moveEffect) {
                        default:
                            break;

                        case BATTLE_EFFECT_ENCORE:
                            if (battleCtx->movePrevByBattler[defender] != MOVE_NONE
                                && BattleAI_BattleMonHasPriorityMove(battleSys, battleCtx, defender) == FALSE) {
                                if (MOVE_DATA(battleCtx->movePrevByBattler[defender]).class == CLASS_STATUS) {
                                    moveScore += 50;
                                }
                                else {
                                    moveStatusFlags = 0;
                                    PartyMon_ApplyTypeChart(battleSys,
                                                            battleCtx,
                                                            battleCtx->movePrevByBattler[defender],
                                                            CalcMoveType(battleSys, battleCtx, defender, defenderItem, battleCtx->movePrevByBattler[defender]),
                                                            defender,
                                                            battler,
                                                            0,
                                                            partyIndicator,
                                                            partySlot,
                                                            &moveStatusFlags);

                                    if ((((moveStatusFlags & MOVE_STATUS_IMMUNE)
                                        && (moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE))
                                        || (moveStatusFlags & MOVE_STATUS_RESISTED)) {

                                        moveScore += 50;
                                    }
                                }
                            }
                            break;

                        case BATTLE_EFFECT_DISABLE:
                            if (battleCtx->movePrevByBattler[defender] != MOVE_NONE
                                && BattleAI_BattleMonHasPriorityMove(battleSys, battleCtx, defender) == FALSE) {

                                if (defenderItemEffect == HOLD_EFFECT_CHOICE_SPEED
                                || defenderItemEffect == HOLD_EFFECT_CHOICE_ATK
                                || defenderItemEffect == HOLD_EFFECT_CHOICE_SPATK) {
                                    moveScore += 100;
                                }

                                moveStatusFlags = 0;
                                PartyMon_ApplyTypeChart(battleSys,
                                                        battleCtx,
                                                        battleCtx->movePrevByBattler[defender],
                                                        CalcMoveType(battleSys, battleCtx, defender, defenderItem, battleCtx->movePrevByBattler[defender]),
                                                        defender,
                                                        battler,
                                                        0,
                                                        partyIndicator,
                                                        partySlot,
                                                        &moveStatusFlags);

                                if (moveStatusFlags & MOVE_STATUS_WEAK) {
                                    moveScore += 50;
                                }
                            }
                            break;

                        case BATTLE_EFFECT_SWITCH_HELD_ITEMS:
                            if (battleCtx->movePrevByBattler[defender] != MOVE_NONE
                                && BattleAI_BattleMonHasPriorityMove(battleSys, battleCtx, defender) == FALSE) {

                                if (defenderItem != Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL)) {
                                    moveScore += 50;
                                }
                            }
                            break;

                        case BATTLE_EFFECT_TAUNT:
                            if (AI_PartyMonShouldTauntCheck(battleSys, battleCtx, battler, defender, partySlot, partyIndicator)) {
                                moveScore += 50;
                            }
                            break;

                        case BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER:
                            if (BattleAI_BattleMonHasPriorityMove(battleSys, battleCtx, defender) == FALSE) {
                                moveScore += 30;
                            }
                            break;
                    }
                }

                if (moveStatFlag != BATTLE_STAT_FLAG_NONE) {
                    if (defenderAbility != ABILITY_UNAWARE) {
                        if (moveStatFlag & (BATTLE_STAT_FLAG_ATTACK | BATTLE_STAT_FLAG_SP_ATTACK)) {
                            moveScore = 40;
                        }
                        else {
                            if (moveStatFlag & (BATTLE_STAT_FLAG_DEFENSE | BATTLE_STAT_FLAG_SP_DEFENSE | BATTLE_STAT_FLAG_EVASION)) {
                                moveScore = 25;
                            }
                        }

                        if (moveStatFlag & BATTLE_STAT_FLAG_SPEED) {
                            if ((battleCtx->fieldConditionsMask & FIELD_CONDITION_TRICK_ROOM) == FALSE) {
                                moveScore += 9;
                            }
                        }
                    }
                }
            }
        }
        else {
            moveScore = 0;
        }

        if (moveScore >= 50) {

            moveScore += 25;
        }
        if (moveScore >= 125) {

            moveScore += 25;
        }
        if (moveScore > 250) {
            moveScore = 250;
        }

        score += moveScore;
    }

    return score;
}

int BattleAI_CalculateStatusMoveDefendScore(BattleSystem *battleSys, BattleContext *battleCtx, int defender, int battler, int partyIndicator, int partySlot)
{
    int i;
    int score, moveScore;
    int defenderCurHP, defenderMaxHP;
    int moveMoveEffect, moveVolatileStatus, moveStatus, moveStatFlag, moveEffect;
    u8 moveType;
    u8 monType1, monType2, monAbility;
    u8 defenderItemEffect, defenderItemPower, defenderType1, defenderType2, defenderAbility, defenderSpeedStat;
	u8 side, oppSide;
    u8 compareSpeedDefenderVsMon;
    u16 move, monSpeedStat, defenderItem;
    u32 moveStatusFlags;

    Pokemon *mon = BattleSystem_PartyPokemon(battleSys, partyIndicator, partySlot);

    score = 0;

    defenderType1 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_1, NULL);
    defenderType2 = BattleMon_Get(battleCtx, defender, BATTLEMON_TYPE_2, NULL);
    defenderAbility = Battler_Ability(battleCtx, defender);

    defenderItem = BattleMon_Get(battleCtx, defender, BATTLEMON_HELD_ITEM, NULL);
    defenderItemEffect = Battler_HeldItemEffect(battleCtx, defender);
    defenderItemPower = Battler_HeldItemPower(battleCtx, defender, ITEM_POWER_CHECK_ALL);

	defenderMaxHP = BattleMon_Get(battleCtx, defender, BATTLEMON_MAX_HP, NULL);
    defenderCurHP = BattleMon_Get(battleCtx, defender, BATTLEMON_CUR_HP, NULL);
	defenderSpeedStat = BattleMon_Get(battleCtx, defender, BATTLEMON_SPEED, NULL);

    monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
    monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
    monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
    monSpeedStat = Pokemon_GetValue(mon, MON_DATA_SPEED, NULL);
	
	side = Battler_Side(battleSys, battler);
    oppSide = Battler_Side(battleSys, defender);

    compareSpeedDefenderVsMon = BattleSystem_ComparePartyMonSpeed(battleSys, battleCtx, defender, battler, partyIndicator, partySlot, TRUE);

    for (i = 0; i < LEARNED_MOVES_MAX; i++) 
    {
        if (battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove != MOVE_NONE) {

            move = battleCtx->battleMons[defender].moveEffectsData.choiceLockedMove;
        }
        else {
            move = battleCtx->aiContext.battlerMoves[defender][i];
        }

        if (move == MOVE_NONE) {
            break;
        }

        if (MOVE_DATA(move).class == CLASS_STATUS) {
            moveStatusFlags = 0;
            moveType = CalcMoveType(battleSys, battleCtx, defender, defenderItem, move);
			moveEffect = MOVE_DATA(move).effect;
            moveScore = PartyMon_ApplyTypeChart(battleSys,
                        battleCtx,
                        move,
                        moveType,
                        defender,
                        battler,
                        0,
                        partyIndicator,
                        partySlot,
                        &moveStatusFlags);
            moveMoveEffect = MapBattleEffectToMoveEffect(battleCtx, moveEffect);
            moveStatus = MapBattleEffectToStatusCondition(battleCtx, moveEffect);
            moveVolatileStatus = MapBattleEffectToVolatileStatus(battleCtx, moveEffect);
            moveStatFlag = MapBattleEffectToSelfStatBoost(battleCtx, moveEffect);

        if (((moveStatusFlags & MOVE_STATUS_IMMUNE) == FALSE)
            || (moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY)) {

            moveScore = 10;

            if (moveStatus != MON_CONDITION_NONE
                && (Pokemon_GetValue(mon, MON_DATA_STATUS_CONDITION, NULL) & MON_CONDITION_ANY) == FALSE) {
                if (moveStatus & MON_CONDITION_BURN) {
                    if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_BURN) == FALSE)
                    || defenderAbility == ABILITY_MOLD_BREAKER)
                    && monType1 != TYPE_FIRE
                    && monType2 != TYPE_FIRE)
                    if (Battle_PartyMonIsPhysicalAttacker(battleSys, battleCtx, battler, i)) {
                        moveScore = 100;
                    }
                }

                if (moveStatus & MON_CONDITION_PARALYSIS) {
                    if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_PARALYSIS) == FALSE)
                    || defenderAbility == ABILITY_MOLD_BREAKER)
                    && monType1 != TYPE_ELECTRIC
                    && monType2 != TYPE_ELECTRIC) {

                        if (AI_PartyMonShouldParalyzeCheck(battleSys, battleCtx, battler, i, defenderSpeedStat)) {
                            moveScore = 50;
                        }
                    }
                }

                if (moveStatus & MON_CONDITION_SLEEP) {
                    if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_SLEEP) == FALSE)
                    || defenderAbility == ABILITY_MOLD_BREAKER) {

                        moveScore = 50;
                    }
                }

                if (moveStatus & MON_CONDITION_ANY_POISON) {
                    if (((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_ANY_POISON) == FALSE)
                    || defenderAbility == ABILITY_MOLD_BREAKER)
                    && monType1 != TYPE_POISON
                    && monType2 != TYPE_POISON
                    ) {
                        if ((monType1 == TYPE_STEEL
                            || monType2 == TYPE_STEEL)
                            && defenderAbility == ABILITY_CORROSION)
                        {
                            moveScore = 50;
                        }
                        else {
                            moveScore = 25;
                        }
                    }
                }
            }

            if (moveVolatileStatus != VOLATILE_CONDITION_NONE) {
                if (moveVolatileStatus & VOLATILE_CONDITION_CONFUSION) {
                    if (Battle_BattleMonIsPhysicalAttacker(battleSys, battleCtx, defender)) {
                        moveScore = 25;
                    }
                    else {
                        moveScore = 15;
                    }
                }

                if (moveVolatileStatus & VOLATILE_CONDITION_ATTRACT) {
                    if (BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL) != Pokemon_GetValue(mon, MON_DATA_GENDER, NULL)
                        && Pokemon_GetValue(mon, MON_DATA_GENDER, NULL) != GENDER_NONE
                        && BattleMon_Get(battleCtx, defender, BATTLEMON_GENDER, NULL) != GENDER_NONE) {
                            moveScore = 25;
                    }
                }

                if (moveVolatileStatus & VOLATILE_CONDITION_MOVE_LOCKED) {
                    moveScore = 25;
                }

                if (moveVolatileStatus & VOLATILE_CONDITION_TORMENT) {
                    moveScore = 15;
                }

                if (moveVolatileStatus & VOLATILE_CONDITION_CURSE) {
                    if (defenderType1 == TYPE_GHOST || defenderType2 == TYPE_GHOST) {
                        moveScore = 49;
                    }
                }
            }


            if (moveMoveEffect != MOVE_EFFECT_NONE) {
                if (moveMoveEffect & MOVE_EFFECT_YAWN) {
                    if ((Battle_AbilityDetersStatus(battleSys, battleCtx, monAbility, MON_CONDITION_SLEEP) == FALSE)
                    || defenderAbility == ABILITY_MOLD_BREAKER) {

                    // Yawn is handled by sleep status, so we just give it +2 here
                        moveScore -= 2;
                    }
                }

                if (moveMoveEffect & MOVE_EFFECT_LEECH_SEED) {
                    if (monType1 != TYPE_GRASS && monType2 != TYPE_GRASS) {
                        moveScore = 25;
                    }
                }

                if (moveMoveEffect & MOVE_EFFECT_PERISH_SONG) {
                    if (monAbility != ABILITY_SOUNDPROOF) {
                                    
                        moveScore = 15;
                    }
                }
            }

            // Boost to statusing, pivoting, and phazing when yawn switching
            if (battleCtx->battleMons[battler].moveEffectsMask & MOVE_EFFECT_YAWN) {
                if (moveEffect != BATTLE_EFFECT_INFATUATE
                && (moveVolatileStatus != VOLATILE_CONDITION_NONE
                || moveMoveEffect != MOVE_EFFECT_NONE
                || moveStatus != MON_CONDITION_NONE)
                ) {
                    moveScore += 25;
                }

                switch (moveEffect) {
                    default:
                        break;

                    // directly counter yawn
                    case BATTLE_EFFECT_USE_RANDOM_LEARNED_MOVE_SLEEP:
                    case BATTLE_EFFECT_FLEE_FROM_WILD_BATTLE:
                    case BATTLE_EFFECT_RECOVER_DAMAGE_SLEEP:
                    case BATTLE_EFFECT_STATUS_LEECH_SEED:
                    case BATTLE_EFFECT_DISABLE:
                    case BATTLE_EFFECT_PREVENT_STATUS:
                    case BATTLE_EFFECT_TAUNT:
                    case BATTLE_EFFECT_SWITCH_HELD_ITEMS:
                    case BATTLE_EFFECT_APPLY_MAGIC_COAT:
                    case BATTLE_EFFECT_USE_LAST_USED_MOVE:
                        moveScore += 50;
                        break;

                    // consolation prizes
                    case BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN:
                    case BATTLE_EFFECT_HEAL_IN_3_TURNS:
                    case BATTLE_EFFECT_DOUBLE_SPEED_3_TURNS:
                    case BATTLE_EFFECT_TOXIC_SPIKES:
                    case BATTLE_EFFECT_TRICK_ROOM:
                    case BATTLE_EFFECT_STEALTH_ROCK:
                    case BATTLE_EFFECT_FAINT_FULL_RESTORE_NEXT_MON:
                    case BATTLE_EFFECT_FAINT_AND_FULL_HEAL_NEXT_MON:
                    case BATTLE_EFFECT_SET_SPIKES:
                        moveScore += 25;
                        break;
                }

                if (moveVolatileStatus & VOLATILE_CONDITION_TORMENT) {
                    moveScore += 50;
                }
            }

            if (moveEffect == BATTLE_EFFECT_REMOVE_HAZARDS_SCREENS_EVA_DOWN) {
                if ((monAbility == ABILITY_DEFIANT
                    || monAbility == ABILITY_COMPETITIVE)
                    && defenderAbility != ABILITY_UNAWARE) {
                        moveScore = 0;
                }
                else {
                    if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_HAZARDS_ANY) {
                        moveScore = 10 * battleCtx->sideConditions[side].spikesLayers;
                        moveScore += (8 * battleCtx->sideConditions[side].toxicSpikesLayers);
                        if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_STEALTH_ROCK) {
                            moveScore += 20;
                        }
                        if (battleCtx->sideConditionsMask[side] & SIDE_CONDITION_STICKY_WEB) {
                            moveScore += 30;
                        }
                    }
                    if (battleCtx->sideConditionsMask[oppSide] & SIDE_CONDITION_ENEMY_DEFOG) {
                        moveScore += 5;
                        if (battleCtx->sideConditionsMask[oppSide] & SIDE_CONDITION_REFLECT) {
                            moveScore += 10;
                        }
                        if (battleCtx->sideConditionsMask[oppSide] & SIDE_CONDITION_LIGHT_SCREEN) {
                            moveScore += 10;
                        }
                    }
                }
            }

            // Status moves that get more score when we are faster
            if (compareSpeedDefenderVsMon == COMPARE_SPEED_SLOWER
            || compareSpeedDefenderVsMon == COMPARE_SPEED_TIE) {
                switch (moveEffect) {
                    default:
                        break;

                    case BATTLE_EFFECT_ENCORE:
                        if (battleCtx->movePrevByBattler[defender] != MOVE_NONE
                            && BattleAI_BattleMonHasPriorityMove(battleSys, battleCtx, defender) == FALSE) {
                            if (MOVE_DATA(battleCtx->movePrevByBattler[defender]).class == CLASS_STATUS) {
                                moveScore += 50;
                            }
                            else {
                                moveStatusFlags = 0;
                                PartyMon_ApplyTypeChart(battleSys,
                                                        battleCtx,
                                                        battleCtx->movePrevByBattler[defender],
                                                        CalcMoveType(battleSys, battleCtx, defender, defenderItem, battleCtx->movePrevByBattler[defender]),
                                                        defender,
                                                        battler,
                                                        0,
                                                        partyIndicator,
                                                        partySlot,
                                                        &moveStatusFlags);

                                if ((((moveStatusFlags & MOVE_STATUS_IMMUNE)
                                    && (moveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE))
                                    || (moveStatusFlags & MOVE_STATUS_RESISTED)) {

                                    moveScore += 50;
                                }
                            }
                        }
                        break;

                    case BATTLE_EFFECT_DISABLE:
                        if (battleCtx->movePrevByBattler[defender] != MOVE_NONE
                            && BattleAI_BattleMonHasPriorityMove(battleSys, battleCtx, defender) == FALSE) {

                            if (defenderItemEffect == HOLD_EFFECT_CHOICE_SPEED
                            || defenderItemEffect == HOLD_EFFECT_CHOICE_ATK
                            || defenderItemEffect == HOLD_EFFECT_CHOICE_SPATK) {
                                moveScore += 100;
                            }

                            moveStatusFlags = 0;
                            PartyMon_ApplyTypeChart(battleSys,
                                                    battleCtx,
                                                    battleCtx->movePrevByBattler[defender],
                                                    CalcMoveType(battleSys, battleCtx, defender, defenderItem, battleCtx->movePrevByBattler[defender]),
                                                    defender,
                                                    battler,
                                                    0,
                                                    partyIndicator,
                                                    partySlot,
                                                    &moveStatusFlags);

                            if (moveStatusFlags & MOVE_STATUS_WEAK) {
                                moveScore += 50;
                            }
                        }
                        break;

                    case BATTLE_EFFECT_SWITCH_HELD_ITEMS:
                        if (battleCtx->movePrevByBattler[defender] != MOVE_NONE
                            && BattleAI_BattleMonHasPriorityMove(battleSys, battleCtx, defender) == FALSE) {

                            if (defenderItem != Pokemon_GetValue(mon, MON_DATA_HELD_ITEM, NULL)) {
                                moveScore += 50;
                            }
                        }
                        break;

                    case BATTLE_EFFECT_TAUNT:
                        if (AI_PartyMonShouldTauntCheck(battleSys, battleCtx, battler, defender, partySlot, partyIndicator)) {
                            moveScore += 50;
                        }
                        break;

                    case BATTLE_EFFECT_KO_MON_THAT_DEFEATED_USER:
                        if (BattleAI_BattleMonHasPriorityMove(battleSys, battleCtx, defender) == FALSE) {
                            moveScore += 30;
                        }
                        break;
                }
            }
        }
    }
}

BOOL BattleAI_BattleMonHasPriorityMove(BattleSystem *battleSys, BattleContext *battleCtx, int battler)
{
    BOOL result;
    int i;
    int moveEffect;
    u16 move;

    result = FALSE;

    for (i = 0; i < LEARNED_MOVES_MAX; i++) {
        if (battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove != MOVE_NONE) {

            move = battleCtx->battleMons[battler].moveEffectsData.choiceLockedMove;
        }
        else {
            move = battleCtx->aiContext.battlerMoves[battler][i];
        }

        if (move == MOVE_NONE) {
            break;
        }

        moveEffect = MOVE_DATA(move).effect;

        if (MOVE_DATA(move).priority > 0) {
            // Consideration for Fake Out's variable priority
            if (moveEffect == BATTLE_EFFECT_ALWAYS_FLINCH_FIRST_TURN_ONLY) {
                if (battleCtx->totalTurns < battleCtx->battleMons[battler].moveEffectsData.fakeOutTurnNumber) {
                    result = TRUE;
                }
            }
            else {
                result = TRUE;
            }
        }
    }

    return result;
}