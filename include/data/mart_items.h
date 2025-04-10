#include "constants/items.h"
#include "constants/seals.h"

typedef struct {
    u16 itemID;
    u16 requiredBadges;
} PokeMartCommonItem;

const PokeMartCommonItem PokeMartCommonItems[] = {
	{ ITEM_POKE_BALL, 0x1 },
	{ ITEM_GREAT_BALL, 0x2 },
	{ ITEM_ULTRA_BALL, 0x3 },
	{ ITEM_POTION, 0x1 },
	{ ITEM_SUPER_POTION, 0x2 },
	{ ITEM_HYPER_POTION, 0x4 },
	{ ITEM_MAX_POTION, 0x5 },
	{ ITEM_FULL_RESTORE, 0x6 },
	{ ITEM_REVIVE, 0x3 },
	{ ITEM_MAX_REVIVE, 0x5 },
	{ ITEM_ANTIDOTE, 0x1 },
	{ ITEM_PARLYZ_HEAL, 0x1 },
	{ ITEM_AWAKENING, 0x1 },
	{ ITEM_BURN_HEAL, 0x1 },
	{ ITEM_FULL_HEAL, 0x3 },
	{ ITEM_ESCAPE_ROPE, 0x1 },
	{ ITEM_REPEL, 0x1 },
	{ ITEM_SUPER_REPEL, 0x2 },
	{ ITEM_MAX_REPEL, 0x3 },
	{ ITEM_ORAN_BERRY, 0x2 },
	{ ITEM_SITRUS_BERRY, 0x3 },
	{ ITEM_CHERI_BERRY, 0x2 },
	{ ITEM_CHESTO_BERRY, 0x2 },
	{ ITEM_PECHA_BERRY, 0x2 },
	{ ITEM_RAWST_BERRY, 0x2 },
	{ ITEM_LEPPA_BERRY, 0x4 },
	{ ITEM_LUM_BERRY, 0x4 },
	{ ITEM_LIECHI_BERRY, 0x3 },
	{ ITEM_GANLON_BERRY, 0x3 },
	{ ITEM_SALAC_BERRY, 0x3 },
	{ ITEM_PETAYA_BERRY, 0x3 },
	{ ITEM_APICOT_BERRY, 0x3 },
	{ ITEM_LANSAT_BERRY, 0x3 },
	{ ITEM_STARF_BERRY, 0x5 },
	{ ITEM_JABOCA_BERRY, 0x4 },
	{ ITEM_ROWAP_BERRY, 0x4 }
};

const u16 JubilifeMartSpecialties[] = {
	ITEM_GRASS_MAIL,
	ITEM_FLAME_MAIL,
	ITEM_BUBBLE_MAIL,
	ITEM_BLOOM_MAIL,
	ITEM_TUNNEL_MAIL,
	ITEM_STEEL_MAIL,
	ITEM_HEART_MAIL,
	ITEM_SNOW_MAIL,
	ITEM_SPACE_MAIL,
	ITEM_AIR_MAIL,
	ITEM_MOSAIC_MAIL,
	ITEM_BRICK_MAIL,
	0xffff
};

const u16 OreburghMartSpecialties[] = {
	ITEM_HEAL_BALL,
	ITEM_REPEAT_BALL,
	0xffff
};

const u16 FloaromaMartSpecialties[] = {
	ITEM_HEAL_BALL,
	ITEM_NET_BALL,
	0xffff
};

const u16 EternaMartSpecialties[] = {
	ITEM_HEAL_BALL,
	ITEM_NET_BALL,
	ITEM_NEST_BALL,
	0xffff
};

const u16 EternaHerbShopStock[] = {
	ITEM_HEAL_POWDER,
	ITEM_ENERGYPOWDER,
	ITEM_ENERGY_ROOT,
	ITEM_REVIVAL_HERB,
	0xffff
};

const u16 HearthomeMartSpecialties[] = {
	ITEM_FIRE_STONE,
	ITEM_WATER_STONE,
	ITEM_THUNDERSTONE,
	ITEM_LEAF_STONE,
	ITEM_MOON_STONE,
	ITEM_SUN_STONE,
	ITEM_SHINY_STONE,
	ITEM_DUSK_STONE,
	ITEM_DAWN_STONE,
	ITEM_OVAL_STONE,
	0xffff
};

const u16 SolaceonMartSpecialties[] = {
	ITEM_NET_BALL,
	ITEM_NEST_BALL,
	ITEM_QUICK_BALL,
	ITEM_DUSK_BALL,
	0xffff
};

const u16 PastoriaMartSpecialties[] = {
	ITEM_DEEPSEASCALE,
	ITEM_DEEPSEATOOTH,
	ITEM_DRAGON_SCALE,
	ITEM_ELECTIRIZER,
	ITEM_KINGS_ROCK,
	ITEM_MAGMARIZER,
	ITEM_METAL_COAT,
	ITEM_PROTECTOR,
	ITEM_RAZOR_CLAW,
	ITEM_RAZOR_FANG,
	ITEM_REAPER_CLOTH,
	0xffff
};

const u16 VeilstoneDeptStoreStock_1F_RIGHT[] = {
	ITEM_POTION,
	ITEM_SUPER_POTION,
	ITEM_HYPER_POTION,
	ITEM_MAX_POTION,
	ITEM_REVIVE,
	ITEM_ANTIDOTE,
	ITEM_PARLYZ_HEAL,
	ITEM_BURN_HEAL,
	ITEM_AWAKENING,
	ITEM_FULL_HEAL,
	0xffff
};

const u16 VeilstoneDeptStoreStock_1F_LEFT[] = {
	ITEM_POKE_BALL,
	ITEM_GREAT_BALL,
	ITEM_ULTRA_BALL,
	ITEM_ESCAPE_ROPE,
	ITEM_POKE_DOLL,
	ITEM_REPEL,
	ITEM_SUPER_REPEL,
	ITEM_MAX_REPEL,
	0xffff
};

const u16 VeilstoneDeptStoreStock_2F_UP[] = {
	ITEM_X_SPEED,
	ITEM_X_ATTACK,
	ITEM_X_DEFENSE,
	ITEM_GUARD_SPEC_,
	ITEM_DIRE_HIT,
	ITEM_X_ACCURACY,
	ITEM_X_SPECIAL,
	ITEM_X_SP__DEF,
	0xffff
};

const u16 VeilstoneDeptStoreStock_2F_MID[] = {
	ITEM_PROTEIN,
	ITEM_IRON,
	ITEM_CALCIUM,
	ITEM_ZINC,
	ITEM_CARBOS,
	ITEM_HP_UP,
	0xffff
};

const u16 VeilstoneDeptStoreStock_3F_UP[] = {
	ITEM_ADAMANT_ORB,
	ITEM_LUSTROUS_ORB,
	ITEM_GRISEOUS_ORB,
	ITEM_STICKY_BARB,
	ITEM_DAMP_ROCK,
	ITEM_HEAT_ROCK,
	ITEM_ICY_ROCK,
	ITEM_SMOOTH_ROCK,
	ITEM_POWER_ANKLET,
	ITEM_POWER_BAND,
	ITEM_POWER_BELT,
	ITEM_POWER_BRACER,
	ITEM_FLAME_PLATE,
	ITEM_SPLASH_PLATE,
	ITEM_ZAP_PLATE,
	ITEM_MEADOW_PLATE,
	ITEM_ICICLE_PLATE,
	ITEM_FIST_PLATE,
	ITEM_TOXIC_PLATE,
	ITEM_EARTH_PLATE,
	ITEM_SKY_PLATE,
	ITEM_MIND_PLATE,
	ITEM_INSECT_PLATE,
	ITEM_STONE_PLATE,
	ITEM_SPOOKY_PLATE,
	ITEM_DRACO_PLATE,
	ITEM_DREAD_PLATE,
	0xffff
};

const u16 VeilstoneDeptStoreStock_3F_DOWN[] = {
	ITEM_AIR_BALLOON,
	ITEM_WEAKNESS_POLICY,
	ITEM_RED_CARD,
	ITEM_ASSAULT_VEST,
	ITEM_SAFETY_GOGGLES,
	ITEM_LOADED_GLOVES,
	ITEM_LOADED_DICE,
	ITEM_LIFE_ORB,
	ITEM_LEFTOVERS,
	ITEM_CHOICE_BAND,
	ITEM_CHOICE_SPECS,
	ITEM_CHOICE_SCARF,
	ITEM_FOCUS_SASH,
	ITEM_POWER_HERB,
	ITEM_WHITE_HERB,
	ITEM_LIGHT_CLAY,
	ITEM_FLAME_ORB,
	ITEM_TOXIC_ORB,
	0xffff
};

const u16 CelesticMartSpecialties[] = {
	ITEM_DUSK_BALL,
	ITEM_QUICK_BALL,
	ITEM_TIMER_BALL,
	0xffff
};

const u16 SnowpointMartSpecialties[] = {
	ITEM_DUSK_BALL,
	ITEM_QUICK_BALL,
	ITEM_TIMER_BALL,
	0xffff
};

const u16 CanalaveMartSpecialties[] = {
	ITEM_QUICK_BALL,
	ITEM_TIMER_BALL,
	ITEM_REPEAT_BALL,
	0xffff
};

const u16 SunyshoreMartSpecialties[] = {
	ITEM_LUXURY_BALL,
	ITEM_PREMIER_BALL,
	ITEM_QUICK_BALL,
	ITEM_DUSK_BALL,
	ITEM_NET_BALL,
	ITEM_REPEAT_BALL,
	ITEM_UPGRADE,
	ITEM_DUBIOUS_DISC,
	0xffff
};

const u16 PokemonLeagueMartSpecialties[] = {
	ITEM_QUICK_BALL,
	ITEM_DUSK_BALL,
	ITEM_NET_BALL,
	ITEM_REPEAT_BALL,
	ITEM_TIMER_BALL,
	ITEM_LUXURY_BALL,
	ITEM_NEST_BALL,
	ITEM_HEAL_BALL,
	ITEM_DIVE_BALL,
	0xffff
};

const u16 VeilstoneDeptStoreStock_B1F_DOWN_LEFT[] = {
	ITEM_FIGY_BERRY,
	ITEM_WIKI_BERRY,
	ITEM_MAGO_BERRY,
	ITEM_AGUAV_BERRY,
	ITEM_IAPAPA_BERRY,
	ITEM_POMEG_BERRY,
	ITEM_KELPSY_BERRY,
	ITEM_QUALOT_BERRY,
	ITEM_HONDEW_BERRY,
	ITEM_GREPA_BERRY,
	ITEM_TAMATO_BERRY,
	0xffff
};

const u16 *PokeMartSpecialties[] = {
	JubilifeMartSpecialties,
	OreburghMartSpecialties,
	FloaromaMartSpecialties,
	EternaMartSpecialties,
	EternaHerbShopStock,
	HearthomeMartSpecialties,
	SolaceonMartSpecialties,
	PastoriaMartSpecialties,
	VeilstoneDeptStoreStock_1F_RIGHT,
	VeilstoneDeptStoreStock_1F_LEFT,
	VeilstoneDeptStoreStock_2F_UP,
	VeilstoneDeptStoreStock_2F_MID,
	VeilstoneDeptStoreStock_3F_UP,
	VeilstoneDeptStoreStock_3F_DOWN,
	CelesticMartSpecialties,
	SnowpointMartSpecialties,
	CanalaveMartSpecialties,
	SunyshoreMartSpecialties,
	PokemonLeagueMartSpecialties,
	VeilstoneDeptStoreStock_B1F_DOWN_LEFT
};

const u16 VeilstoneDeptStoreStock_4F_UP[] = {
	0x7,
	0x16,
	0x19,
	0x1A,
	0x1B,
	0xffff
};

const u16 VeilstoneDeptStoreStock_4F_DOWN[] = {
	0x73,
	0x74,
	0x75,
	0x77,
	0x78,
	0x79,
	0xffff
};

const u16 *VeilstoneDeptStoreDecorationStocks[] = {
	VeilstoneDeptStoreStock_4F_UP,
	VeilstoneDeptStoreStock_4F_DOWN
};

const u16 SunyshoreMarketStockMonday[] = {
	HEART_SEAL_A,
	STAR_SEAL_B,
	FIRE_SEAL_A,
	SONG_SEAL_A,
	LINE_SEAL_C,
	ELE_SEAL_B,
	PARTY_SEAL_D,
	0xffff
};

const u16 SunyshoreMarketStockTuesday[] = {
	HEART_SEAL_B,
	STAR_SEAL_C,
	FIRE_SEAL_B,
	FLORA_SEAL_A,
	SONG_SEAL_B,
	LINE_SEAL_D,
	ELE_SEAL_C,
	0xffff
};

const u16 SunyshoreMarketStockWednesday[] = {
	HEART_SEAL_C,
	STAR_SEAL_D,
	FIRE_SEAL_C,
	FLORA_SEAL_B,
	SONG_SEAL_C,
	SMOKE_SEAL_A,
	ELE_SEAL_D,
	0xffff
};

const u16 SunyshoreMarketStockThursday[] = {
	HEART_SEAL_D,
	FOAMY_SEAL_A,
	FIRE_SEAL_D,
	FLORA_SEAL_C,
	SONG_SEAL_D,
	STAR_SEAL_E,
	SMOKE_SEAL_B,
	0xffff
};

const u16 SunyshoreMarketStockFriday[] = {
	FOAMY_SEAL_B,
	PARTY_SEAL_A,
	FLORA_SEAL_D,
	SONG_SEAL_E,
	HEART_SEAL_E,
	STAR_SEAL_F,
	SMOKE_SEAL_C,
	0xffff
};

const u16 SunyshoreMarketStockSaturday[] = {
	FOAMY_SEAL_C,
	PARTY_SEAL_B,
	FLORA_SEAL_E,
	SONG_SEAL_F,
	HEART_SEAL_F,
	LINE_SEAL_A,
	SMOKE_SEAL_D,
	0xffff
};

const u16 SunyshoreMarketStockSunday[] = {
	STAR_SEAL_A,
	SONG_SEAL_G,
	FOAMY_SEAL_D,
	FLORA_SEAL_F,
	LINE_SEAL_B,
	ELE_SEAL_A,
	PARTY_SEAL_C,
	0xffff
};

const u16 *SunyshoreMarketDailyStocks[] = {
	SunyshoreMarketStockMonday,
	SunyshoreMarketStockTuesday,
	SunyshoreMarketStockWednesday,
	SunyshoreMarketStockThursday,
	SunyshoreMarketStockFriday,
	SunyshoreMarketStockSaturday,
	SunyshoreMarketStockSunday
};