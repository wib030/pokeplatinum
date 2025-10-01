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
void ExpertAI_EvalMoreMoves_Singles(BattleSystem* battleSys, BattleContext* battleCtx);

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

/**
* @brief Check if the AI Context attacker KOs the AI Context defender
*
* @param battleSys
* @param battleCtx
* @param attacker	The AI Context attacker.
* @param defender	The AI Context defender.
* @return TRUE/FALSE whether or not the attacker KOs the defender.
*/
BOOL ExpertAI_AttackerKOsDefender(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, int defender);

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
s32 ExpertAI_CalcAllDamage(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, u16* moves, s32* damageVals, u16 heldItem, u8* ivs, int ability, int embargoTurns, int varyDamage);

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
s32 ExpertAI_CalcDamage(BattleSystem* battleSys, BattleContext* battleCtx, u16 move, u16 heldItem, u8* ivs, int attacker, int ability, int embargoTurns, u8 variance);

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
int ExpertAI_MoveType(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int move);

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
BOOL ExpertExpertAI_CanUseMove(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int moveSlot, int opMask);

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
int ExpertAI_CheckInvalidMoves(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int invalidMoves, int opMask);

/* @brief Check if the current move has been revealed (i.e. used at all this fight).
 *
 * @param battleSys
 * @param battleCtx
 * @return  TRUE / FALSE whether the AI Context current move is at full PP or not.
 */
BOOL AICmd_IfCurrentMoveRevealedOverflow(BattleSystem* battleSys, BattleContext* battleCtx);

/* @brief Check if the given Move Effect is known by the given Battler.
 *
 * @param battleSys
 * @param battleCtx
 * @param battler    The battlemon ID of the battler to check.
 * @param moveeffec  The Move Effect to check.
 * @return  TRUE / FALSE whether the AI Context current move is at full PP or not.
 */
BOOL AI_IfMoveEffectKnown(BattleSystem* battleSys, BattleContext* battleCtx, int battler, int moveEffect);

BOOL BattleAI_IsMoveBlockedByWebMaster(BattleSystem* battleSys, BattleContext* battleCtx, int attacker, int defender, u16 move);

/* @brief Check if the battler is a special attacker
 *
 * @param battleSys
 * @param battleCtx
 * @param battler    The battlemon ID of the battler to check.
 * @return  TRUE / FALSE whether the given battler is a special attacker or not.
 */
BOOL ExpertAI_IsBattlerSpecialAttacker(BattleSystem* battleSys, BattleContext* battleCtx, int battler);

/* @brief Check if the battler is a physical attacker
 *
 * @param battleSys
 * @param battleCtx
 * @param battler    The battlemon ID of the battler to check.
 * @return  TRUE / FALSE whether the given battler is a physical attacker or not.
 */
BOOL ExpertAI_IsBattlerPhysicalAttacker(BattleSystem* battleSys, BattleContext* battleCtx, int battler);

/* @brief Calculate a move to use when predicting player switch.
 *
 * @param battleSys
 * @param attacker			 The attacking battlemon ID, provided in battle_display.c when invoked
 * @param currentMoveSlot    The slot of the current best move.
 * @return  The slot of the best move after considering predicting a switch.
 */
u8 ExpertAI_CalcSwitchAttack_Singles(BattleSystem* battleSys, u8 attacker, u8 currentMoveSlot);

#endif // POKEPLATINUM_BATTLE_TRAINER_AI_OVERFLOW_H
