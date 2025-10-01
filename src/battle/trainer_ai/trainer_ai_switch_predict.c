#include <nitro.h>
#include <string.h>

#include "pch/global_pch.h"
#include "assert.h"

#include "battle/trainer_ai_switch_predict.h"

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

u8 ExpertAI_CalcSwitchAttack_Singles(BattleSystem* battleSys, u8 attacker, u8 currentMoveSlot)
{
    BattleContext* battleCtx;
    u8 defender;
    u8 predictMoveSlot;
    u8 predictMoveSlotsFlags;
    u8 monsImmune;
    u8 monsLowDamage;
    u8 monType1;
    u8 monType2;
    u8 monAbility;
    u8 monSide;
    u8 currentMoveType;
    u8 predictMoveType;
    u8 slot1;
    u8 slot2;
    u8 numMaxScoreMoves;
    u8 maxScoreMoves[4];
    u8 maxScoreMoveSlots[4];
    u8 ivs[STAT_MAX];
    u16 currentMove;
    u16 predictMove;
    u16 monSpecies;
    u32 currentMoveStatusFlags;
    u32 predictMoveStatusFlags;
    int i;
    int j;
    int stat;
    int partySize;
    int monCurHP;
    int monMaxHP;
    int currentMoveDamage;
    int predictMoveDamage;
    int bestPredictMoveDamage;
    Pokemon* mon;

    if (battleSys->battleType & BATTLE_TYPE_DOUBLES) {
        return currentMoveSlot;
    }

    battleCtx = BattleSystem_Context(battleSys);

    currentMove = battleCtx->battleMons[attacker].moves[currentMoveSlot];

    if (currentMove == MOVE_NONE)
    {
        return currentMoveSlot;
    }

    defender = BattleSystem_RandomOpponent(battleSys, battleCtx, attacker);

    if (battleCtx->battleMons[defender].curHP == 0
        && battleCtx->battleMons[defender].maxHP == 0)
    {
        return currentMoveSlot;
    }

    predictMoveSlot = currentMoveSlot;

    for (stat = STAT_HP; stat < STAT_MAX; stat++) {
        ivs[stat] = BattleMon_Get(battleCtx, attacker, BATTLEMON_HP_IV + stat, NULL);
    }

    currentMoveDamage = ExpertAI_CalcDamage(battleSys,
        battleCtx,
        currentMove,
        battleCtx->battleMons[attacker].heldItem,
        ivs,
        attacker,
        Battler_Ability(battleCtx, attacker),
        battleCtx->battleMons[attacker].moveEffectsData.embargoTurns,
        DAMAGE_VARIANCE_MIN_ROLL);

    if ((currentMoveDamage * 100 / battleCtx->battleMons[defender].maxHP) < (battleCtx->battleMons[defender].maxHP * 80 / 100))
    {
        return currentMoveSlot;
    }

    monsImmune = 0;
    monsLowDamage = 0;
    predictMoveSlotsFlags = 0;

    currentMoveType = ExpertAI_MoveType(battleSys, battleCtx, attacker, currentMove);

    partySize = BattleSystem_PartyCount(battleSys, defender);
    monSide = Battler_Side(battleSys, defender);

    slot1 = defender;
    if ((BattleSystem_BattleType(battleSys) & BATTLE_TYPE_TAG)
        || (BattleSystem_BattleType(battleSys) & BATTLE_TYPE_2vs2)) {
        slot2 = slot1;
    }
    else {
        slot2 = BattleSystem_Partner(battleSys, defender);
    }

    for (i = 0; i < partySize; i++)
    {
        mon = BattleSystem_PartyPokemon(battleSys, defender, i);
        monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);
        monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
        monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
        monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
        monCurHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
        monMaxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);


        // mon must be existent, not an egg, alive, and not currently out
        if (monSpecies != SPECIES_NONE
            && monSpecies != SPECIES_EGG
            && monCurHP
            && battleCtx->selectedPartySlot[slot1] != i
            && battleCtx->selectedPartySlot[slot2] != i)
        {
            currentMoveStatusFlags = 0;

            currentMoveDamage = BattleSystem_CalcPartyMemberMoveDamage(battleSys,
                battleCtx,
                currentMove,
                battleCtx->sideConditionsMask[monSide],
                battleCtx->fieldConditionsMask,
                0,
                currentMoveType,
                attacker,
                defender,
                1,
                defender,
                i);

            currentMoveDamage = PartyMon_ApplyTypeChart(battleSys,
                battleCtx,
                currentMove,
                currentMoveType,
                attacker,
                defender,
                currentMoveDamage,
                defender,
                i,
                &currentMoveStatusFlags);

            if ((currentMoveStatusFlags & MOVE_STATUS_IMMUNE)
                && ((currentMoveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                currentMoveDamage = 0;
                monsImmune |= FlagIndex(i);
            }

            if (currentMoveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                currentMoveDamage /= 2;
            }

            if (currentMoveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                currentMoveDamage = currentMoveDamage * 5 / 4;
            }

            currentMoveDamage = currentMoveDamage * 100 / monMaxHP;

            if (currentMoveDamage < 20)
            {
                monsLowDamage |= FlagIndex(i);
            }
        }
    }

    if (monsImmune)
    {
        bestPredictMoveDamage = 0;

        for (i = 0; i < partySize; i++)
        {
            if (monsImmune & FlagIndex(i))
            {
                mon = BattleSystem_PartyPokemon(battleSys, defender, i);
                monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);
                monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
                monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
                monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
                monCurHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
                monMaxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);

                for (j = 0; j < LEARNED_MOVES_MAX; j++)
                {
                    predictMove = battleCtx->battleMons[attacker].moves[j];

                    if (ExpertAI_CanUseMove(battleSys, battleCtx, attacker, j, CHECK_INVALID_ALL_BUT_TORMENT)
                        && predictMove != currentMove)
                    {
                        predictMoveDamage = 0;
                        predictMoveStatusFlags = 0;

                        predictMoveType = ExpertAI_MoveType(battleSys, battleCtx, attacker, predictMove);

                        predictMoveDamage = BattleSystem_CalcPartyMemberMoveDamage(battleSys,
                            battleCtx,
                            predictMove,
                            battleCtx->sideConditionsMask[monSide],
                            battleCtx->fieldConditionsMask,
                            0,
                            predictMoveType,
                            attacker,
                            defender,
                            1,
                            defender,
                            i);

                        if (predictMoveDamage)
                        {
                            predictMoveDamage = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                predictMove,
                                predictMoveType,
                                attacker,
                                defender,
                                predictMoveDamage,
                                defender,
                                i,
                                &predictMoveStatusFlags);

                            if ((predictMoveStatusFlags & MOVE_STATUS_IMMUNE)
                                && ((predictMoveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                                predictMoveDamage = 0;
                            }

                            if (predictMoveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                                predictMoveDamage /= 2;
                            }

                            if (predictMoveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                                predictMoveDamage = predictMoveDamage * 5 / 4;
                            }

                            predictMoveDamage = predictMoveDamage * 100 / monMaxHP;

                            if (predictMoveDamage > 45)
                            {
                                if (predictMoveDamage > bestPredictMoveDamage)
                                {
                                    bestPredictMoveDamage = predictMoveDamage;
                                    predictMoveSlot = j;
                                }
                            }
                        }
                    }
                }
            }
        }

        if (bestPredictMoveDamage)
        {
            return predictMoveSlot;
        }
    }

    if (monsLowDamage)
    {
        for (i = 0; i < partySize; i++)
        {
            if (monsLowDamage & FlagIndex(i))
            {
                mon = BattleSystem_PartyPokemon(battleSys, defender, i);
                monSpecies = Pokemon_GetValue(mon, MON_DATA_SPECIES_EGG, NULL);
                monType1 = Pokemon_GetValue(mon, MON_DATA_TYPE_1, NULL);
                monType2 = Pokemon_GetValue(mon, MON_DATA_TYPE_2, NULL);
                monAbility = Pokemon_GetValue(mon, MON_DATA_ABILITY, NULL);
                monCurHP = Pokemon_GetValue(mon, MON_DATA_CURRENT_HP, NULL);
                monMaxHP = Pokemon_GetValue(mon, MON_DATA_MAX_HP, NULL);

                for (j = 0; j < LEARNED_MOVES_MAX; j++)
                {
                    predictMove = battleCtx->battleMons[attacker].moves[j];

                    if (ExpertAI_CanUseMove(battleSys, battleCtx, attacker, j, CHECK_INVALID_ALL_BUT_TORMENT)
                        && predictMove != currentMove)
                    {
                        predictMoveDamage = 0;
                        predictMoveStatusFlags = 0;

                        predictMoveType = ExpertAI_MoveType(battleSys, battleCtx, attacker, predictMove);

                        predictMoveDamage = BattleSystem_CalcPartyMemberMoveDamage(battleSys,
                            battleCtx,
                            predictMove,
                            battleCtx->sideConditionsMask[monSide],
                            battleCtx->fieldConditionsMask,
                            0,
                            predictMoveType,
                            attacker,
                            defender,
                            1,
                            defender,
                            i);

                        if (predictMoveDamage)
                        {
                            predictMoveDamage = PartyMon_ApplyTypeChart(battleSys,
                                battleCtx,
                                predictMove,
                                predictMoveType,
                                attacker,
                                defender,
                                predictMoveDamage,
                                defender,
                                i,
                                &predictMoveStatusFlags);

                            if ((predictMoveStatusFlags & MOVE_STATUS_IMMUNE)
                                && ((predictMoveStatusFlags & MOVE_STATUS_IGNORE_IMMUNITY) == FALSE)) {

                                predictMoveDamage = 0;
                            }

                            if (predictMoveStatusFlags & MOVE_STATUS_TYPE_RESIST_ABILITY) {
                                predictMoveDamage /= 2;
                            }

                            if (predictMoveStatusFlags & MOVE_STATUS_TYPE_WEAKNESS_ABILITY) {
                                predictMoveDamage = predictMoveDamage * 5 / 4;
                            }

                            predictMoveDamage = predictMoveDamage * 100 / monMaxHP;

                            if (predictMoveDamage > 57)
                            {
                                if (predictMoveDamage > bestPredictMoveDamage)
                                {
                                    bestPredictMoveDamage = predictMoveDamage;
                                    predictMoveSlot = j;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return predictMoveSlot;
}