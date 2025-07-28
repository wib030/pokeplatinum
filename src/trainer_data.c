#include <nitro.h>
#include <string.h>

#include "constants/battle.h"
#include "constants/pokemon.h"
#include "constants/trainer.h"

#include "data/trainer_class_genders.h"

#include "struct_decls/struct_02006C24_decl.h"
#include "savedata.h"
#include "struct_defs/trainer_data.h"
#include "overlay006/battle_params.h"

#include "heap.h"
#include "message.h"
#include "narc.h"
#include "party.h"
#include "pokemon.h"
#include "strbuf.h"
#include "trainer_data.h"
#include "unk_020021B0.h"
#include "unk_0201D15C.h"
#include "savedata_misc.h"

static void TrainerData_BuildParty(BattleParams *battleParams, int battler, int heapID, const SaveData *save);

void TrainerData_Encounter(BattleParams *battleParams, const SaveData *save, int heapID)
{
    TrainerData trdata;
    MessageLoader *msgLoader = MessageLoader_Init(MESSAGE_LOADER_NARC_HANDLE, NARC_INDEX_MSGDATA__PL_MSG, 618, heapID);
    const charcode_t *rivalName = MiscSaveBlock_RivalName(SaveData_MiscSaveBlockConst(save));

    for (int i = 0; i < MAX_BATTLERS; i++) {
        if (!battleParams->trainerIDs[i]) {
            continue;
        }
            
        TrainerData_Load(battleParams->trainerIDs[i], &trdata);
        battleParams->trainerData[i] = trdata;

        if (trdata.class == TRAINER_CLASS_RIVAL) {
            GF_strcpy(battleParams->trainerData[i].name, rivalName);
        } else {
            Strbuf *trainerName = MessageLoader_GetNewStrbuf(msgLoader, battleParams->trainerIDs[i]);
            Strbuf_ToChars(trainerName, battleParams->trainerData[i].name, NPC_TRAINER_NAME_LEN);
            Strbuf_Free(trainerName);
        }

        TrainerData_BuildParty(battleParams, i, heapID, save);
    }

    battleParams->battleType |= trdata.battleType;
    MessageLoader_Free(msgLoader);
}

u32 TrainerData_LoadParam(int trainerID, enum TrainerDataParam paramID)
{
    u32 result;
    TrainerData trdata;

    TrainerData_Load(trainerID, &trdata);

    switch (paramID) {
    case TRDATA_TYPE:
        result = trdata.type;
        break;

    case TRDATA_CLASS:
        result = trdata.class;
        break;

    case TRDATA_SPRITE:
        result = trdata.sprite;
        break;

    case TRDATA_PARTY_SIZE:
        result = trdata.partySize;
        break;

    case TRDATA_ITEM_1:
    case TRDATA_ITEM_2:
    case TRDATA_ITEM_3:
    case TRDATA_ITEM_4:
        result = trdata.items[paramID - TRDATA_ITEM_1];
        break;

    case TRDATA_AI_MASK:
        result = trdata.aiMask;
        break;

    case TRDATA_BATTLE_TYPE:
        result = trdata.battleType;
        break;
    }

    return result;
}

BOOL TrainerData_HasMessageType(int trainerID, enum TrainerMessageType msgType, int heapID)
{
    NARC *narc; // must declare up here to match
    u16 offset, data[2];

    BOOL result = FALSE;
    int size = NARC_GetMemberSizeByIndexPair(NARC_INDEX_POKETOOL__TRMSG__TRTBL, 0);
    NARC_ReadFromMemberByIndexPair(&offset, NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS, 0, trainerID * 2, 2);
    narc = NARC_ctor(NARC_INDEX_POKETOOL__TRMSG__TRTBL, heapID);

    while (offset != size) {
        NARC_ReadFromMember(narc, 0, offset, 4, data);

        if (data[0] == trainerID && data[1] == msgType) {
            result = TRUE;
            break;
        }

        if (data[0] != trainerID) {
            break;
        }

        offset += 4;
    }

    NARC_dtor(narc);
    return result;
}

void TrainerData_LoadMessage(int trainerID, enum TrainerMessageType msgType, Strbuf *strbuf, int heapID)
{
    NARC *narc; // must declare up here to match
    u16 offset, data[2];

    int size = NARC_GetMemberSizeByIndexPair(NARC_INDEX_POKETOOL__TRMSG__TRTBL, 0);
    NARC_ReadFromMemberByIndexPair(&offset, NARC_INDEX_POKETOOL__TRMSG__TRTBLOFS, 0, trainerID * 2, 2);
    narc = NARC_ctor(NARC_INDEX_POKETOOL__TRMSG__TRTBL, heapID);

    while (offset != size) {
        NARC_ReadFromMember(narc, 0, offset, 4, data);

        if (data[0] == trainerID && data[1] == msgType) {
            MessageBank_GetStrbufFromNARC(NARC_INDEX_MSGDATA__PL_MSG, 617, offset / 4, heapID, strbuf);
            break;
        }

        offset += 4;
    }

    NARC_dtor(narc);

    if (offset == size) {
        Strbuf_Clear(strbuf);
    }
}

void TrainerData_Load(int trainerID, TrainerData *trdata)
{
    NARC_ReadWholeMemberByIndexPair(trdata, NARC_INDEX_POKETOOL__TRAINER__TRDATA, trainerID);
}

void TrainerData_LoadParty(int trainerID, void *trparty)
{
    NARC_ReadWholeMemberByIndexPair(trparty, NARC_INDEX_POKETOOL__TRAINER__TRPOKE, trainerID);
}

u8 TrainerClass_Gender(int trclass)
{
    return sTrainerClassGender[trclass];
}

/**
 * @brief Build the party for a trainer as loaded in the BattleParams struct.
 * 
 * @param battleParams  The parent BattleParams struct containing trainer data.
 * @param battler       Which battler's party is to be loaded.
 * @param heapID        Heap on which to perform any allocations.
 */
static void TrainerData_BuildParty(BattleParams *battleParams, int battler, int heapID, const SaveData *save)
{
    // must make declarations C89-style to match
    void *buf;
    int i, j;
    u32 genderMod, rnd, oldSeed;
    u8 ivs;
    Pokemon *mon;
	int monIndex, switchedIndex, capLevel;
	TrainerInfo *trInfo = SaveData_GetTrainerInfo(save);
	int badges = TrainerInfo_BadgeCount(trInfo);
	
	switch (badges)
	{
		case 0:
			capLevel = 13;
			break;

		case 1:
			capLevel = 22;
			break;

		case 2:
			capLevel = 26;
			break;

		case 3:
			capLevel = 31;
			break;

		case 4:
			capLevel = 36;
			break;

		case 5:
			capLevel = 45;
			break;

		case 6:
			capLevel = 51;
			break;

		default:
			capLevel = 58;
			break;
	}
	
    oldSeed = LCRNG_GetSeed();

    // alloc enough space to support the maximum possible data size
    Party_InitWithCapacity(battleParams->parties[battler], MAX_PARTY_SIZE);
    buf = Heap_AllocFromHeap(heapID, sizeof(TrainerMonWithMovesAndItem) * MAX_PARTY_SIZE);
    mon = Pokemon_New(heapID);

    TrainerData_LoadParty(battleParams->trainerIDs[battler], buf);

    // determine which magic gender-specific modifier to use for the RNG function
    genderMod = TrainerClass_Gender(battleParams->trainerData[battler].class) == GENDER_FEMALE
            ? 120 : 136;

    switch (battleParams->trainerData[battler].type) {
    case TRDATATYPE_BASE: {
        TrainerMonBase *trmon = (TrainerMonBase *)buf;
        for (i = 0; i < battleParams->trainerData[battler].partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> 10;

            rnd = trmon[i].dv + trmon[i].level + species + battleParams->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < battleParams->trainerData[battler].class; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].dv * MAX_IVS_SINGLE_STAT / MAX_DV;

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, 0, OTID_NOT_SHINY, 0);
            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Party_AddPokemon(battleParams->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_MOVES: {
        TrainerMonWithMoves *trmon = (TrainerMonWithMoves *)buf;
        for (i = 0; i < battleParams->trainerData[battler].partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> 10;

            rnd = trmon[i].dv + trmon[i].level + species + battleParams->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < battleParams->trainerData[battler].class; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].dv * MAX_IVS_SINGLE_STAT / MAX_DV;

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, 0, OTID_NOT_SHINY, 0);

            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[i].moves[j], j);
            }

            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Party_AddPokemon(battleParams->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_ITEM: {
        TrainerMonWithItem *trmon = (TrainerMonWithItem *)buf;
        for (i = 0; i < battleParams->trainerData[battler].partySize; i++) {
            u16 species = trmon[i].species & 0x3FF;
            u8 form = (trmon[i].species & 0xFC00) >> 10;

            rnd = trmon[i].dv + trmon[i].level + species + battleParams->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < battleParams->trainerData[battler].class; j++) {
                rnd = LCRNG_Next();
            }

            rnd = (rnd << 8) + genderMod;
            ivs = trmon[i].dv * MAX_IVS_SINGLE_STAT / MAX_DV;

            Pokemon_InitWith(mon, species, trmon[i].level, ivs, TRUE, 0, OTID_NOT_SHINY, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[i].item);
            Pokemon_SetBallSeal(trmon[i].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
            Party_AddPokemon(battleParams->parties[battler], mon);
        }

        break;
    }

    case TRDATATYPE_WITH_MOVES_AND_ITEM: {
        TrainerMonWithMovesAndItem *trmon = (TrainerMonWithMovesAndItem *)buf;
		switchedIndex = 7;
        for (i = 0; i < battleParams->trainerData[battler].partySize; i++) {
			monIndex = i;
			
			if (switchedIndex == i)
			{
				monIndex = 0;
			}
			else if (trmon[i].altLead < 7 && (LCRNG_Next() % 100) < 25)
			{
				switchedIndex = trmon[i].altLead;
				monIndex = trmon[i].altLead;
			}
			
            u16 species = trmon[monIndex].species & 0x3FF;
            u8 form = (trmon[monIndex].species & 0xFC00) >> 10;

            rnd = trmon[monIndex].dv + trmon[monIndex].level + species + battleParams->trainerIDs[battler];
            LCRNG_SetSeed(rnd);

            for (j = 0; j < battleParams->trainerData[battler].class; j++) {
                rnd = LCRNG_Next();
            }
			
            rnd = (rnd << 8) + genderMod;
            ivs = trmon[monIndex].dv * MAX_IVS_SINGLE_STAT / MAX_DV;
			
			int monLevel = trmon[monIndex].level;
			int otIdSource = OTID_NOT_SHINY;
			int monPersonality = (LCRNG_Next() | (LCRNG_Next() << 16));
			
			if (trmon[monIndex].copyLevelCap == 1)
			{
				monLevel = capLevel;
			}
			
			if (trmon[monIndex].isShiny == 1)
			{
				otIdSource = OTID_ALWAYS_SHINY;
			}
			
			if (trmon[monIndex].gender < 2)
			{
				PokemonPersonalData *monPersonalData = PokemonPersonalData_FromMonSpecies(species, 0);
				
				do {
					monPersonality = (LCRNG_Next() | (LCRNG_Next() << 16));
				} while (PokemonPersonalData_GetGenderOf(monPersonalData, species, monPersonality) != trmon[monIndex].gender);
					
				PokemonPersonalData_Free(monPersonalData);
			}
			
			//TRUE is whether or not to enable nature, rnd value decides the nature
            Pokemon_InitWith(mon, species, monLevel, ivs, TRUE, monPersonality, otIdSource, 0);
            Pokemon_SetValue(mon, MON_DATA_HELD_ITEM, &trmon[monIndex].item);
			
			if (trmon[monIndex].nature < 25)
			{
				Pokemon_SetValue(mon, MON_DATA_NATURE, &trmon[monIndex].nature);
			}
			
			u32 ability1 = PokemonPersonalData_GetSpeciesValue(mon, MON_DATA_PERSONAL_ABILITY_1);
			u32 ability2 = PokemonPersonalData_GetSpeciesValue(mon, MON_DATA_PERSONAL_ABILITY_2);
			
			//This sets the ability to which one we want
			u16 abilityNum = trmon[monIndex].ability;
			
			switch (abilityNum)
			{
				case 0:
					Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability1);
				break;
				
				case 1:
					Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability2);
				break;
				
				default:
					Pokemon_SetValue(mon, MON_DATA_ABILITY, &ability1);
				break;
			}
			
			if (trmon[monIndex].overrideAbility < 256)
			{
				Pokemon_SetValue(mon, MON_DATA_ABILITY, &trmon[monIndex].overrideAbility);
			}
			
            for (j = 0; j < 4; j++) {
                Pokemon_SetMoveSlot(mon, trmon[monIndex].moves[j], j);
            }
			
			Pokemon_SetValue(mon, MON_DATA_POKEBALL, &trmon[monIndex].ball_type);
            Pokemon_SetBallSeal(trmon[monIndex].cbSeal, mon, heapID);
            Pokemon_SetValue(mon, MON_DATA_FORM, &form);
			Pokemon_SetValue(mon, MON_DATA_FRIENDSHIP, &trmon[monIndex].friendship);
			
			if (trmon[monIndex].status != 0)
			{
				Pokemon_SetValue(mon, MON_DATA_STATUS_CONDITION, &trmon[monIndex].status);
			}
			
			if (trmon[monIndex].ivHP < 32)
			{
				Pokemon_SetValue(mon, MON_DATA_HP_IV, &trmon[monIndex].ivHP);
				Pokemon_SetValue(mon, MON_DATA_ATK_IV, &trmon[monIndex].ivATK);
				Pokemon_SetValue(mon, MON_DATA_DEF_IV, &trmon[monIndex].ivDEF);
				Pokemon_SetValue(mon, MON_DATA_SPEED_IV, &trmon[monIndex].ivSPEED);
				Pokemon_SetValue(mon, MON_DATA_SPATK_IV, &trmon[monIndex].ivSPATK);
				Pokemon_SetValue(mon, MON_DATA_SPDEF_IV, &trmon[monIndex].ivSPDEF);
			}
			
			if (trmon[monIndex].evHP < 253)
			{
				Pokemon_SetValue(mon, MON_DATA_HP_EV, &trmon[monIndex].evHP);
				Pokemon_SetValue(mon, MON_DATA_ATK_EV, &trmon[monIndex].evATK);
				Pokemon_SetValue(mon, MON_DATA_DEF_EV, &trmon[monIndex].evDEF);
				Pokemon_SetValue(mon, MON_DATA_SPEED_EV, &trmon[monIndex].evSPEED);
				Pokemon_SetValue(mon, MON_DATA_SPATK_EV, &trmon[monIndex].evSPATK);
				Pokemon_SetValue(mon, MON_DATA_SPDEF_EV, &trmon[monIndex].evSPDEF);
			}
			
			Pokemon_CalcLevelAndStats(mon);
            Party_AddPokemon(battleParams->parties[battler], mon);
        }

        break;
    }
    }

    Heap_FreeToHeap(buf);
    Heap_FreeToHeap(mon);
    LCRNG_SetSeed(oldSeed);
}
