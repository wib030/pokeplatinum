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

