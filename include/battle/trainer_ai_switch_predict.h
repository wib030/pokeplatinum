#ifndef POKEPLATINUM_BATTLE_TRAINER_AI_SWITCH_PREDICT_H
#define POKEPLATINUM_BATTLE_TRAINER_AI_SWITCH_PREDICT_H

#include "struct_decls/battle_system.h"
#include "battle/battle_context.h"

/* @brief Calculate a move to use when predicting player switch.
 *
 * @param battleSys
 * @param attacker			 The attacking battlemon ID, provided in battle_display.c when invoked
 * @param currentMoveSlot    The slot of the current best move.
 * @return  The slot of the best move after considering predicting a switch.
 */
u8 ExpertAI_CalcSwitchAttack_Singles(BattleSystem* battleSys, u8 attacker, u8 currentMoveSlot);

#endif // POKEPLATINUM_BATTLE_TRAINER_AI_SWITCH_PREDICT_H
