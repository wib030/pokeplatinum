#ifndef POKEPLATINUM_BATTLE_TRAINER_AI_OVERFLOW_H
#define POKEPLATINUM_BATTLE_TRAINER_AI_OVERFLOW_H

#include "struct_decls/battle_system.h"
#include "battle/battle_context.h"

/**
* @brief Workaround function to make the trainer AI evaluate moves beyond its filesize limit.
*
* @param battleSys
* @param battleCtx
* @return Nothing, but the function itself updates the AI move scores.
*/
void TrainerAI_EvalMoreMoves_ExpertSingles(BattleSystem* battleSys, BattleContext* battleCtx);

/**
* @brief Script to add a value to the AI Context move score.
*
* @param battleSys
* @param battleCtx
* @val							The value to add to the move score.
* @return Nothing, but the move score is incremented or decremented externally.
*/
void AI_AddToMoveScore(BattleSystem* battleSys, BattleContext* battleCtx, int val);

/**
* @brief Random number generation for AI overflow move analysis.
*
* @param battleSys
* @return A random integer in [0, 255].
*/
int AI_GetRandomNumber(BattleSystem* battleSys);

/**
* @brief Check if the currently used move KOs its target.
*
* @param battleSys
* @param battleCtx
* @param useDamageroll		Whether to use max, min, or random move roll.
* @return TRUE/FALSE whether the move KOs its target or not.
*/
BOOL AI_CurrentMoveKills(BattleSystem* battleSys, BattleContext* battleCtx, int useDamageRoll);

/**
* @brief Rank all of the AI Context attacker's moves by their damage output.
*
* @param battleSys
* @param battleCtx
* @param varyDamage		Whether to use max, min, or random move roll.
* @return A bitmask flag indicating if the move is highest, not highest, or non-comparable damage.
*/
int AI_FlagMoveDamageScore(BattleSystem* battleSys, BattleContext* battleCtx, int varyDamage);

/**
* @brief Get a battler's ability from the AI Context.
*
* @param battleSys
* @param battleCtx
* @param battler		The AI Context battler.
* @return The battler's Ability ID.
*/
u8 AI_GetBattlerAbility(BattleSystem* battleSys, BattleContext* battleCtx, int battler);

/**
* @brief Get the AI Context's current move's effectiveness flag.
*
* @param battleSys
* @param battleCtx
* @return The AI Context's current move's move effectiveness flag.
*/
u32 AI_GetMoveEffectiveness(BattleSystem* battleSys, BattleContext* battleCtx);

/**
* @brief Get the AI Context battler's HP percentage.
*
* @param battleSys
* @param battleCtx
* @param battler	The AI Context battler.
* @return The given AI Context battler's current HP percent, factoring for Wonder Guard.
*/
u32 AI_GetBattlerHPPercent(BattleSystem* battleSys, BattleContext* battleCtx, u8 battler);