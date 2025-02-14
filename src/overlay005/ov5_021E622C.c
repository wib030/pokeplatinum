#include <nitro.h>
#include <string.h>

#include "strbuf.h"
#include "trainer_info.h"
#include "struct_decls/struct_02026218_decl.h"
#include "struct_decls/struct_02026224_decl.h"
#include "struct_decls/struct_02026310_decl.h"
#include "struct_defs/chatot_cry.h"
#include "struct_decls/struct_0202CD88_decl.h"
#include "pokemon.h"
#include "struct_decls/struct_party_decl.h"
#include "savedata.h"

#include "field/field_system.h"

#include "message.h"
#include "message_util.h"
#include "string_template.h"
#include "unk_02017038.h"
#include "heap.h"
#include "unk_0201D15C.h"
#include "strbuf.h"
#include "unk_02025E08.h"
#include "trainer_info.h"
#include "unk_020261E4.h"
#include "unk_0202CC64.h"
#include "unk_0202CD50.h"
#include "unk_020559DC.h"
#include "constants/species.h"
#include "pokemon.h"
#include "party.h"
#include "item.h"
#include "unk_02092494.h"
#include "overlay005/ov5_021E622C.h"

const u16 EggMove_Table[] = {
    (SPECIES_BULBASAUR) + 20000, // Bulbasaur line
    MOVE_LIGHT_SCREEN,
    MOVE_SKULL_BASH,
    MOVE_SAFEGUARD,
    MOVE_CHARM,
    MOVE_PETAL_DANCE,
    MOVE_MAGICAL_LEAF,
    MOVE_GRASS_WHISTLE,
    MOVE_CURSE,
    MOVE_INGRAIN,
    MOVE_NATURE_POWER,
    MOVE_AMNESIA,
    MOVE_LEAF_STORM,
    (SPECIES_CHARMANDER) + 20000, // Charmander line
    MOVE_BELLY_DRUM,
    MOVE_ANCIENT_POWER,
    MOVE_ROCK_SLIDE,
    MOVE_BITE,
    MOVE_OUTRAGE,
    MOVE_BEAT_UP,
    MOVE_SWORDS_DANCE,
    MOVE_DRAGON_DANCE,
    MOVE_CRUNCH,
    MOVE_DRAGON_RUSH,
    MOVE_METAL_CLAW,
    MOVE_FLARE_BLITZ,
    (SPECIES_SQUIRTLE) + 20000, // Squirtle line
    MOVE_MIRROR_COAT,
    MOVE_HAZE,
    MOVE_MIST,
    MOVE_FORESIGHT,
    MOVE_FLAIL,
    MOVE_REFRESH,
    MOVE_MUD_SPORT,
    MOVE_YAWN,
    MOVE_MUDDY_WATER,
    MOVE_FAKE_OUT,
    MOVE_AQUA_RING,
    MOVE_AQUA_JET,
    (SPECIES_PIDGEY) + 20000, // Pidgey line
    MOVE_PURSUIT,
    MOVE_FAINT_ATTACK,
    MOVE_FORESIGHT,
    MOVE_STEEL_WING,
    MOVE_AIR_CUTTER,
    MOVE_AIR_SLASH,
    MOVE_BRAVE_BIRD,
    MOVE_UPROAR,
    (SPECIES_RATTATA) + 20000, // Rattata line
    MOVE_SCREECH,
    MOVE_FLAME_WHEEL,
    MOVE_FURY_SWIPES,
    MOVE_BITE,
    MOVE_COUNTER,
    MOVE_REVERSAL,
    MOVE_UPROAR,
    MOVE_SWAGGER,
    MOVE_LAST_RESORT,
    MOVE_ME_FIRST,
    (SPECIES_SPEAROW) + 20000, // Spearow line
    MOVE_FAINT_ATTACK,
    MOVE_FALSE_SWIPE,
    MOVE_SCARY_FACE,
    MOVE_QUICK_ATTACK,
    MOVE_TRI_ATTACK,
    MOVE_ASTONISH,
    MOVE_SKY_ATTACK,
    MOVE_WHIRLWIND,
    MOVE_UPROAR,
    (SPECIES_EKANS) + 20000, // Ekans line
    MOVE_PURSUIT,
    MOVE_SLAM,
    MOVE_SPITE,
    MOVE_BEAT_UP,
    MOVE_POISON_FANG,
    MOVE_SCARY_FACE,
    MOVE_POISON_TAIL,
    MOVE_DISABLE,
    (SPECIES_SANDSHREW) + 20000, // Sandshrew line
    MOVE_FLAIL,
    MOVE_SAFEGUARD,
    MOVE_COUNTER,
    MOVE_RAPID_SPIN,
    MOVE_ROCK_SLIDE,
    MOVE_METAL_CLAW,
    MOVE_SWORDS_DANCE,
    MOVE_CRUSH_CLAW,
    MOVE_NIGHT_SLASH,
    (SPECIES_NIDORAN_F) + 20000, // Nidoran-female line
    MOVE_SUPERSONIC,
    MOVE_DISABLE,
    MOVE_TAKE_DOWN,
    MOVE_FOCUS_ENERGY,
    MOVE_CHARM,
    MOVE_COUNTER,
    MOVE_BEAT_UP,
    MOVE_PURSUIT,
    (SPECIES_NIDORAN_M) + 20000, // Nidoran-male line
    MOVE_COUNTER,
    MOVE_DISABLE,
    MOVE_SUPERSONIC,
    MOVE_TAKE_DOWN,
    MOVE_AMNESIA,
    MOVE_CONFUSION,
    MOVE_BEAT_UP,
    MOVE_SUCKER_PUNCH,
    (SPECIES_VULPIX) + 20000, // Vulpix line
    MOVE_FAINT_ATTACK,
    MOVE_HYPNOSIS,
    MOVE_FLAIL,
    MOVE_SPITE,
    MOVE_DISABLE,
    MOVE_HOWL,
    MOVE_PSYCH_UP,
    MOVE_HEAT_WAVE,
    MOVE_FLARE_BLITZ,
    MOVE_EXTRASENSORY,
    MOVE_ENERGY_BALL,
    (SPECIES_ZUBAT) + 20000, // Zubat line
    MOVE_QUICK_ATTACK,
    MOVE_PURSUIT,
    MOVE_FAINT_ATTACK,
    MOVE_GUST,
    MOVE_WHIRLWIND,
    MOVE_CURSE,
    MOVE_NASTY_PLOT,
    MOVE_HYPNOSIS,
    MOVE_ZEN_HEADBUTT,
    MOVE_BRAVE_BIRD,
    (SPECIES_ODDISH) + 20000, // Oddish line
    MOVE_SWORDS_DANCE,
    MOVE_RAZOR_LEAF,
    MOVE_FLAIL,
    MOVE_SYNTHESIS,
    MOVE_CHARM,
    MOVE_INGRAIN,
    MOVE_TICKLE,
    (SPECIES_PARAS) + 20000, // Paras line
    MOVE_FALSE_SWIPE,
    MOVE_SCREECH,
    MOVE_COUNTER,
    MOVE_PSYBEAM,
    MOVE_FLAIL,
    MOVE_SWEET_SCENT,
    MOVE_LIGHT_SCREEN,
    MOVE_PURSUIT,
    MOVE_METAL_CLAW,
    MOVE_BUG_BITE,
    MOVE_CROSS_POISON,
    (SPECIES_VENONAT) + 20000, // Venonat line
    MOVE_BATON_PASS,
    MOVE_SCREECH,
    MOVE_GIGA_DRAIN,
    MOVE_SIGNAL_BEAM,
    MOVE_AGILITY,
    MOVE_MORNING_SUN,
    MOVE_TOXIC_SPIKES,
    MOVE_BUG_BITE,
    (SPECIES_DIGLETT) + 20000, // Diglett line
    MOVE_FAINT_ATTACK,
    MOVE_SCREECH,
    MOVE_ANCIENT_POWER,
    MOVE_PURSUIT,
    MOVE_BEAT_UP,
    MOVE_UPROAR,
    MOVE_ROCK_SLIDE,
    MOVE_MUD_BOMB,
    MOVE_ASTONISH,
    (SPECIES_MEOWTH) + 20000, // Meowth line
    MOVE_SPITE,
    MOVE_CHARM,
    MOVE_HYPNOSIS,
    MOVE_AMNESIA,
    MOVE_PSYCH_UP,
    MOVE_ASSIST,
    MOVE_ODOR_SLEUTH,
    MOVE_FLAIL,
    MOVE_LAST_RESORT,
    MOVE_PUNISHMENT,
    (SPECIES_PSYDUCK) + 20000, // Psyduck line
    MOVE_HYPNOSIS,
    MOVE_PSYBEAM,
    MOVE_FORESIGHT,
    MOVE_LIGHT_SCREEN,
    MOVE_FUTURE_SIGHT,
    MOVE_PSYCHIC,
    MOVE_CROSS_CHOP,
    MOVE_REFRESH,
    MOVE_CONFUSE_RAY,
    MOVE_YAWN,
    MOVE_MUD_BOMB,
    (SPECIES_MANKEY) + 20000, // Mankey line
    MOVE_ROCK_SLIDE,
    MOVE_FORESIGHT,
    MOVE_MEDITATE,
    MOVE_COUNTER,
    MOVE_REVERSAL,
    MOVE_BEAT_UP,
    MOVE_REVENGE,
    MOVE_SMELLING_SALT,
    MOVE_CLOSE_COMBAT,
    (SPECIES_GROWLITHE) + 20000, // Growlithe line
    MOVE_BODY_SLAM,
    MOVE_SAFEGUARD,
    MOVE_CRUNCH,
    MOVE_THRASH,
    MOVE_FIRE_SPIN,
    MOVE_HOWL,
    MOVE_HEAT_WAVE,
    MOVE_DOUBLE_EDGE,
    MOVE_FLARE_BLITZ,
    (SPECIES_POLIWAG) + 20000, // Poliwag line
    MOVE_MIST,
    MOVE_SPLASH,
    MOVE_BUBBLE_BEAM,
    MOVE_HAZE,
    MOVE_MIND_READER,
    MOVE_WATER_SPORT,
    MOVE_ICE_BALL,
    MOVE_MUD_SHOT,
    MOVE_REFRESH,
    MOVE_ENDEAVOR,
    (SPECIES_ABRA) + 20000, // Abra line
    MOVE_ENCORE,
    MOVE_BARRIER,
    MOVE_KNOCK_OFF,
    MOVE_FIRE_PUNCH,
    MOVE_THUNDER_PUNCH,
    MOVE_ICE_PUNCH,
    MOVE_POWER_TRICK,
    MOVE_GUARD_SWAP,
    (SPECIES_MACHOP) + 20000, // Machop line
    MOVE_LIGHT_SCREEN,
    MOVE_MEDITATE,
    MOVE_ROLLING_KICK,
    MOVE_ENCORE,
    MOVE_SMELLING_SALT,
    MOVE_COUNTER,
    MOVE_ROCK_SLIDE,
    MOVE_CLOSE_COMBAT,
    MOVE_FIRE_PUNCH,
    MOVE_THUNDER_PUNCH,
    MOVE_ICE_PUNCH,
    MOVE_BULLET_PUNCH,
    (SPECIES_BELLSPROUT) + 20000, // Bellsprout line
    MOVE_SWORDS_DANCE,
    MOVE_ENCORE,
    MOVE_REFLECT,
    MOVE_SYNTHESIS,
    MOVE_LEECH_LIFE,
    MOVE_INGRAIN,
    MOVE_MAGICAL_LEAF,
    MOVE_WORRY_SEED,
    MOVE_TICKLE,
    (SPECIES_TENTACOOL) + 20000, // Tentacool line
    MOVE_AURORA_BEAM,
    MOVE_MIRROR_COAT,
    MOVE_RAPID_SPIN,
    MOVE_HAZE,
    MOVE_SAFEGUARD,
    MOVE_CONFUSE_RAY,
    MOVE_KNOCK_OFF,
    MOVE_ACUPRESSURE,
    (SPECIES_GEODUDE) + 20000, // Geodude line
    MOVE_MEGA_PUNCH,
    MOVE_ROCK_SLIDE,
    MOVE_BLOCK,
    MOVE_HAMMER_ARM,
    MOVE_FLAIL,
    (SPECIES_PONYTA) + 20000, // Ponyta line
    MOVE_FLAME_WHEEL,
    MOVE_THRASH,
    MOVE_DOUBLE_KICK,
    MOVE_HYPNOSIS,
    MOVE_CHARM,
    MOVE_DOUBLE_EDGE,
    MOVE_HORN_DRILL,
    (SPECIES_SLOWPOKE) + 20000, // Slowpoke line
    MOVE_SAFEGUARD,
    MOVE_BELLY_DRUM,
    MOVE_FUTURE_SIGHT,
    MOVE_STOMP,
    MOVE_MUD_SPORT,
    MOVE_SLEEP_TALK,
    MOVE_SNORE,
    MOVE_ME_FIRST,
    MOVE_BLOCK,
    MOVE_ZEN_HEADBUTT,
    (SPECIES_FARFETCHD) + 20000, // Farfetch'd
    MOVE_STEEL_WING,
    MOVE_FORESIGHT,
    MOVE_MIRROR_MOVE,
    MOVE_GUST,
    MOVE_QUICK_ATTACK,
    MOVE_FLAIL,
    MOVE_FEATHER_DANCE,
    MOVE_CURSE,
    MOVE_COVET,
    MOVE_MUD_SLAP,
    MOVE_NIGHT_SLASH,
    (SPECIES_DODUO) + 20000, // Doduo line
    MOVE_QUICK_ATTACK,
    MOVE_SUPERSONIC,
    MOVE_HAZE,
    MOVE_FAINT_ATTACK,
    MOVE_FLAIL,
    MOVE_ENDEAVOR,
    MOVE_MIRROR_MOVE,
    MOVE_BRAVE_BIRD,
    (SPECIES_SEEL) + 20000, // Seel line
    MOVE_LICK,
    MOVE_PERISH_SONG,
    MOVE_DISABLE,
    MOVE_HORN_DRILL,
    MOVE_SLAM,
    MOVE_ENCORE,
    MOVE_FAKE_OUT,
    MOVE_ICICLE_SPEAR,
    MOVE_SIGNAL_BEAM,
    (SPECIES_GRIMER) + 20000, // Grimer line
    MOVE_HAZE,
    MOVE_MEAN_LOOK,
    MOVE_LICK,
    MOVE_IMPRISON,
    MOVE_CURSE,
    MOVE_SHADOW_PUNCH,
    MOVE_EXPLOSION,
    MOVE_SHADOW_SNEAK,
    MOVE_STOCKPILE,
    MOVE_SWALLOW,
    MOVE_SPIT_UP,
    (SPECIES_SHELLDER) + 20000, // Shellder line
    MOVE_BUBBLE_BEAM,
    MOVE_TAKE_DOWN,
    MOVE_BARRIER,
    MOVE_RAPID_SPIN,
    MOVE_SCREECH,
    MOVE_ICICLE_SPEAR,
    MOVE_MUD_SHOT,
    (SPECIES_GASTLY) + 20000, // Gastly line
    MOVE_PSYWAVE,
    MOVE_PERISH_SONG,
    MOVE_HAZE,
    MOVE_ASTONISH,
    MOVE_WILL_O_WISP,
    MOVE_GRUDGE,
    MOVE_EXPLOSION,
    MOVE_FIRE_PUNCH,
    MOVE_ICE_PUNCH,
    MOVE_THUNDER_PUNCH,
    (SPECIES_ONIX) + 20000, // Onix line
    MOVE_ROCK_SLIDE,
    MOVE_FLAIL,
    MOVE_EXPLOSION,
    MOVE_BLOCK,
    MOVE_DEFENSE_CURL,
    MOVE_ROLLOUT,
    MOVE_ROCK_BLAST,
    (SPECIES_DROWZEE) + 20000, // Drowzee line
    MOVE_BARRIER,
    MOVE_ASSIST,
    MOVE_ROLE_PLAY,
    MOVE_FIRE_PUNCH,
    MOVE_THUNDER_PUNCH,
    MOVE_ICE_PUNCH,
    MOVE_NASTY_PLOT,
    MOVE_FLATTER,
    MOVE_PSYCHO_CUT,
    (SPECIES_KRABBY) + 20000, // Krabby line
    MOVE_DIG,
    MOVE_HAZE,
    MOVE_AMNESIA,
    MOVE_FLAIL,
    MOVE_SLAM,
    MOVE_KNOCK_OFF,
    MOVE_SWORDS_DANCE,
    MOVE_TICKLE,
    MOVE_ANCIENT_POWER,
    (SPECIES_EXEGGCUTE) + 20000, // Exeggcute line
    MOVE_SYNTHESIS,
    MOVE_MOONLIGHT,
    MOVE_REFLECT,
    MOVE_ANCIENT_POWER,
    MOVE_PSYCH_UP,
    MOVE_INGRAIN,
    MOVE_CURSE,
    MOVE_NATURE_POWER,
    MOVE_LUCKY_CHANT,
    MOVE_LEAF_STORM,
    (SPECIES_CUBONE) + 20000, // Cubone line
    MOVE_ROCK_SLIDE,
    MOVE_ANCIENT_POWER,
    MOVE_BELLY_DRUM,
    MOVE_SCREECH,
    MOVE_SKULL_BASH,
    MOVE_PERISH_SONG,
    MOVE_SWORDS_DANCE,
    MOVE_DOUBLE_KICK,
    MOVE_IRON_HEAD,
    (SPECIES_LICKITUNG) + 20000, // Lickitung line
    MOVE_BELLY_DRUM,
    MOVE_MAGNITUDE,
    MOVE_BODY_SLAM,
    MOVE_CURSE,
    MOVE_SMELLING_SALT,
    MOVE_SLEEP_TALK,
    MOVE_SNORE,
    MOVE_SUBSTITUTE,
    MOVE_AMNESIA,
    MOVE_HAMMER_ARM,
    (SPECIES_KOFFING) + 20000, // Koffing line
    MOVE_SCREECH,
    MOVE_PSYWAVE,
    MOVE_PSYBEAM,
    MOVE_DESTINY_BOND,
    MOVE_PAIN_SPLIT,
    MOVE_WILL_O_WISP,
    MOVE_GRUDGE,
    MOVE_SPITE,
    MOVE_CURSE,
    (SPECIES_RHYHORN) + 20000, // Rhyhorn line
    MOVE_REVERSAL,
    MOVE_ROCK_SLIDE,
    MOVE_COUNTER,
    MOVE_MAGNITUDE,
    MOVE_SWORDS_DANCE,
    MOVE_CURSE,
    MOVE_CRUSH_CLAW,
    MOVE_DRAGON_RUSH,
    MOVE_ICE_FANG,
    MOVE_FIRE_FANG,
    MOVE_THUNDER_FANG,
    (SPECIES_CHANSEY) + 20000, // Chansey line
    MOVE_PRESENT,
    MOVE_METRONOME,
    MOVE_HEAL_BELL,
    MOVE_AROMATHERAPY,
    MOVE_SUBSTITUTE,
    MOVE_COUNTER,
    MOVE_HELPING_HAND,
    MOVE_GRAVITY,
    (SPECIES_TANGELA) + 20000, // Tangela line
    MOVE_FLAIL,
    MOVE_CONFUSION,
    MOVE_MEGA_DRAIN,
    MOVE_REFLECT,
    MOVE_AMNESIA,
    MOVE_LEECH_SEED,
    MOVE_NATURE_POWER,
    MOVE_ENDEAVOR,
    MOVE_LEAF_STORM,
    (SPECIES_KANGASKHAN) + 20000, // Kangaskhan
    MOVE_STOMP,
    MOVE_FORESIGHT,
    MOVE_FOCUS_ENERGY,
    MOVE_SAFEGUARD,
    MOVE_DISABLE,
    MOVE_COUNTER,
    MOVE_CRUSH_CLAW,
    MOVE_SUBSTITUTE,
    MOVE_DOUBLE_EDGE,
    MOVE_ENDEAVOR,
    MOVE_HAMMER_ARM,
    (SPECIES_HORSEA) + 20000, // Horsea line
    MOVE_FLAIL,
    MOVE_AURORA_BEAM,
    MOVE_OCTAZOOKA,
    MOVE_DISABLE,
    MOVE_SPLASH,
    MOVE_DRAGON_RAGE,
    MOVE_DRAGON_BREATH,
    MOVE_SIGNAL_BEAM,
    (SPECIES_GOLDEEN) + 20000, // Goldeen line
    MOVE_PSYBEAM,
    MOVE_HAZE,
    MOVE_HYDRO_PUMP,
    MOVE_SLEEP_TALK,
    MOVE_MUD_SPORT,
    MOVE_MUD_SLAP,
    MOVE_AQUA_TAIL,
    (SPECIES_MR_MIME) + 20000, // Mr. Mime
    MOVE_FUTURE_SIGHT,
    MOVE_HYPNOSIS,
    MOVE_MIMIC,
    MOVE_PSYCH_UP,
    MOVE_FAKE_OUT,
    MOVE_TRICK,
    MOVE_CONFUSE_RAY,
    MOVE_WAKE_UP_SLAP,
    MOVE_TEETER_DANCE,
    (SPECIES_SCYTHER) + 20000, // Scyther line
    MOVE_COUNTER,
    MOVE_SAFEGUARD,
    MOVE_BATON_PASS,
    MOVE_RAZOR_WIND,
    MOVE_REVERSAL,
    MOVE_LIGHT_SCREEN,
    MOVE_ENDURE,
    MOVE_SILVER_WIND,
    MOVE_BUG_BUZZ,
    MOVE_NIGHT_SLASH,
    (SPECIES_PINSIR) + 20000, // Pinsir
    MOVE_FURY_ATTACK,
    MOVE_FLAIL,
    MOVE_FALSE_SWIPE,
    MOVE_FAINT_ATTACK,
    MOVE_QUICK_ATTACK,
    MOVE_CLOSE_COMBAT,
    MOVE_FEINT,
    (SPECIES_LAPRAS) + 20000, // Lapras
    MOVE_FORESIGHT,
    MOVE_SUBSTITUTE,
    MOVE_TICKLE,
    MOVE_REFRESH,
    MOVE_DRAGON_DANCE,
    MOVE_CURSE,
    MOVE_SLEEP_TALK,
    MOVE_HORN_DRILL,
    MOVE_ANCIENT_POWER,
    MOVE_WHIRLPOOL,
    MOVE_FISSURE,
    (SPECIES_EEVEE) + 20000, // Eevee line
    MOVE_CHARM,
    MOVE_FLAIL,
    MOVE_ENDURE,
    MOVE_CURSE,
    MOVE_TICKLE,
    MOVE_WISH,
    MOVE_YAWN,
    MOVE_FAKE_TEARS,
    MOVE_COVET,
    (SPECIES_OMANYTE) + 20000, // Omanyte line
    MOVE_BUBBLE_BEAM,
    MOVE_AURORA_BEAM,
    MOVE_SLAM,
    MOVE_SUPERSONIC,
    MOVE_HAZE,
    MOVE_ROCK_SLIDE,
    MOVE_SPIKES,
    MOVE_KNOCK_OFF,
    MOVE_WRING_OUT,
    MOVE_TOXIC_SPIKES,
    (SPECIES_KABUTO) + 20000, // Kabuto line
    MOVE_BUBBLE_BEAM,
    MOVE_AURORA_BEAM,
    MOVE_RAPID_SPIN,
    MOVE_DIG,
    MOVE_FLAIL,
    MOVE_KNOCK_OFF,
    MOVE_CONFUSE_RAY,
    MOVE_MUD_SHOT,
    MOVE_ICY_WIND,
    MOVE_SCREECH,
    (SPECIES_AERODACTYL) + 20000, // Aerodactyl
    MOVE_WHIRLWIND,
    MOVE_PURSUIT,
    MOVE_FORESIGHT,
    MOVE_STEEL_WING,
    MOVE_DRAGON_BREATH,
    MOVE_CURSE,
    MOVE_ASSURANCE,
    (SPECIES_SNORLAX) + 20000, // Snorlax
    MOVE_LICK,
    MOVE_CHARM,
    MOVE_DOUBLE_EDGE,
    MOVE_CURSE,
    MOVE_FISSURE,
    MOVE_SUBSTITUTE,
    MOVE_WHIRLWIND,
    MOVE_PURSUIT,
    (SPECIES_DRATINI) + 20000, // Dratini line
    MOVE_LIGHT_SCREEN,
    MOVE_MIST,
    MOVE_HAZE,
    MOVE_SUPERSONIC,
    MOVE_DRAGON_BREATH,
    MOVE_DRAGON_DANCE,
    MOVE_DRAGON_RUSH,
    (SPECIES_CHIKORITA) + 20000, // Chikorita line
    MOVE_VINE_WHIP,
    MOVE_LEECH_SEED,
    MOVE_COUNTER,
    MOVE_ANCIENT_POWER,
    MOVE_FLAIL,
    MOVE_NATURE_POWER,
    MOVE_INGRAIN,
    MOVE_GRASS_WHISTLE,
    MOVE_LEAF_STORM,
    MOVE_AROMATHERAPY,
    MOVE_WRING_OUT,
    (SPECIES_CYNDAQUIL) + 20000, // Cyndaquil line
    MOVE_FURY_SWIPES,
    MOVE_QUICK_ATTACK,
    MOVE_REVERSAL,
    MOVE_THRASH,
    MOVE_FORESIGHT,
    MOVE_COVET,
    MOVE_HOWL,
    MOVE_CRUSH_CLAW,
    MOVE_DOUBLE_EDGE,
    MOVE_DOUBLE_KICK,
    MOVE_FLARE_BLITZ,
    (SPECIES_TOTODILE) + 20000, // Totodile line
    MOVE_CRUNCH,
    MOVE_THRASH,
    MOVE_HYDRO_PUMP,
    MOVE_ANCIENT_POWER,
    MOVE_ROCK_SLIDE,
    MOVE_MUD_SPORT,
    MOVE_WATER_SPORT,
    MOVE_DRAGON_CLAW,
    MOVE_ICE_PUNCH,
    MOVE_METAL_CLAW,
    MOVE_DRAGON_DANCE,
    (SPECIES_SENTRET) + 20000, // Sentret line
    MOVE_DOUBLE_EDGE,
    MOVE_PURSUIT,
    MOVE_SLASH,
    MOVE_FOCUS_ENERGY,
    MOVE_REVERSAL,
    MOVE_SUBSTITUTE,
    MOVE_TRICK,
    MOVE_ASSIST,
    MOVE_LAST_RESORT,
    MOVE_CHARM,
    MOVE_COVET,
    (SPECIES_HOOTHOOT) + 20000, // Hoothoot line
    MOVE_MIRROR_MOVE,
    MOVE_SUPERSONIC,
    MOVE_FAINT_ATTACK,
    MOVE_WING_ATTACK,
    MOVE_WHIRLWIND,
    MOVE_SKY_ATTACK,
    MOVE_FEATHER_DANCE,
    MOVE_AGILITY,
    MOVE_NIGHT_SHADE,
    (SPECIES_LEDYBA) + 20000, // Ledyba line
    MOVE_PSYBEAM,
    MOVE_BIDE,
    MOVE_SILVER_WIND,
    MOVE_BUG_BUZZ,
    MOVE_SCREECH,
    MOVE_ENCORE,
    MOVE_KNOCK_OFF,
    MOVE_BUG_BITE,
    (SPECIES_SPINARAK) + 20000, // Spinarak line
    MOVE_PSYBEAM,
    MOVE_DISABLE,
    MOVE_SONIC_BOOM,
    MOVE_BATON_PASS,
    MOVE_PURSUIT,
    MOVE_SIGNAL_BEAM,
    MOVE_TOXIC_SPIKES,
    MOVE_POISON_JAB,
    (SPECIES_CHINCHOU) + 20000, // Chinchou line
    MOVE_FLAIL,
    MOVE_SCREECH,
    MOVE_AMNESIA,
    MOVE_PSYBEAM,
    MOVE_WHIRLPOOL,
    MOVE_AGILITY,
    MOVE_MIST,
    (SPECIES_PICHU) + 20000, // Pichu line
    MOVE_REVERSAL,
    MOVE_BIDE,
    MOVE_PRESENT,
    MOVE_ENCORE,
    MOVE_DOUBLE_SLAP,
    MOVE_WISH,
    MOVE_CHARGE,
    MOVE_FAKE_OUT,
    MOVE_THUNDER_PUNCH,
    MOVE_TICKLE,
    (SPECIES_CLEFFA) + 20000, // Cleffa line
    MOVE_PRESENT,
    MOVE_METRONOME,
    MOVE_AMNESIA,
    MOVE_BELLY_DRUM,
    MOVE_SPLASH,
    MOVE_MIMIC,
    MOVE_WISH,
    MOVE_SUBSTITUTE,
    MOVE_FAKE_TEARS,
    MOVE_COVET,
    (SPECIES_IGGLYBUFF) + 20000, // Igglybuff line
    MOVE_PERISH_SONG,
    MOVE_PRESENT,
    MOVE_FAINT_ATTACK,
    MOVE_WISH,
    MOVE_FAKE_TEARS,
    MOVE_LAST_RESORT,
    MOVE_COVET,
    MOVE_GRAVITY,
    (SPECIES_TOGEPI) + 20000, // Togepi line
    MOVE_PRESENT,
    MOVE_MIRROR_MOVE,
    MOVE_PECK,
    MOVE_FORESIGHT,
    MOVE_FUTURE_SIGHT,
    MOVE_SUBSTITUTE,
    MOVE_PSYCH_UP,
    MOVE_NASTY_PLOT,
    MOVE_PSYCHO_SHIFT,
    MOVE_LUCKY_CHANT,
    (SPECIES_NATU) + 20000, // Natu line
    MOVE_HAZE,
    MOVE_DRILL_PECK,
    MOVE_QUICK_ATTACK,
    MOVE_FAINT_ATTACK,
    MOVE_STEEL_WING,
    MOVE_PSYCH_UP,
    MOVE_FEATHER_DANCE,
    MOVE_REFRESH,
    MOVE_ZEN_HEADBUTT,
    MOVE_SUCKER_PUNCH,
    (SPECIES_MAREEP) + 20000, // Mareep line
    MOVE_TAKE_DOWN,
    MOVE_BODY_SLAM,
    MOVE_SAFEGUARD,
    MOVE_SCREECH,
    MOVE_REFLECT,
    MOVE_ODOR_SLEUTH,
    MOVE_CHARGE,
    MOVE_FLATTER,
    MOVE_SAND_ATTACK,
    (SPECIES_MARILL) + 20000, // Marill line
    MOVE_LIGHT_SCREEN,
    MOVE_PRESENT,
    MOVE_AMNESIA,
    MOVE_FUTURE_SIGHT,
    MOVE_BELLY_DRUM,
    MOVE_PERISH_SONG,
    MOVE_SUPERSONIC,
    MOVE_SUBSTITUTE,
    MOVE_AQUA_JET,
    MOVE_SUPERPOWER,
    MOVE_REFRESH,
    (SPECIES_SUDOWOODO) + 20000, // Sudowoodo
    MOVE_SELFDESTRUCT,
    MOVE_HEADBUTT,
    MOVE_HARDEN,
    MOVE_DEFENSE_CURL,
    MOVE_ROLLOUT,
    MOVE_SAND_TOMB,
    (SPECIES_HOPPIP) + 20000, // Hoppip line
    MOVE_CONFUSION,
    MOVE_ENCORE,
    MOVE_DOUBLE_EDGE,
    MOVE_REFLECT,
    MOVE_AMNESIA,
    MOVE_HELPING_HAND,
    MOVE_PSYCH_UP,
    MOVE_AROMATHERAPY,
    MOVE_WORRY_SEED,
    (SPECIES_AIPOM) + 20000, // Aipom line
    MOVE_COUNTER,
    MOVE_SCREECH,
    MOVE_PURSUIT,
    MOVE_AGILITY,
    MOVE_SPITE,
    MOVE_SLAM,
    MOVE_DOUBLE_SLAP,
    MOVE_BEAT_UP,
    MOVE_FAKE_OUT,
    MOVE_COVET,
    MOVE_BOUNCE,
    (SPECIES_SUNKERN) + 20000, // Sunkern line
    MOVE_GRASS_WHISTLE,
    MOVE_ENCORE,
    MOVE_LEECH_SEED,
    MOVE_NATURE_POWER,
    MOVE_CURSE,
    MOVE_HELPING_HAND,
    MOVE_INGRAIN,
    MOVE_SWEET_SCENT,
    (SPECIES_YANMA) + 20000, // Yanma line
    MOVE_WHIRLWIND,
    MOVE_REVERSAL,
    MOVE_LEECH_LIFE,
    MOVE_SIGNAL_BEAM,
    MOVE_SILVER_WIND,
    MOVE_FEINT,
    MOVE_FAINT_ATTACK,
    MOVE_PURSUIT,
    (SPECIES_WOOPER) + 20000, // Wooper line
    MOVE_BODY_SLAM,
    MOVE_ANCIENT_POWER,
    MOVE_SAFEGUARD,
    MOVE_CURSE,
    MOVE_MUD_SPORT,
    MOVE_STOCKPILE,
    MOVE_SWALLOW,
    MOVE_SPIT_UP,
    MOVE_COUNTER,
    MOVE_ENCORE,
    MOVE_DOUBLE_KICK,
    (SPECIES_MURKROW) + 20000, // Murkrow line
    MOVE_WHIRLWIND,
    MOVE_DRILL_PECK,
    MOVE_MIRROR_MOVE,
    MOVE_WING_ATTACK,
    MOVE_SKY_ATTACK,
    MOVE_CONFUSE_RAY,
    MOVE_FEATHER_DANCE,
    MOVE_PERISH_SONG,
    MOVE_PSYCHO_SHIFT,
    MOVE_SCREECH,
    MOVE_FAINT_ATTACK,
    (SPECIES_MISDREAVUS) + 20000, // Misdreavus line
    MOVE_SCREECH,
    MOVE_DESTINY_BOND,
    MOVE_PSYCH_UP,
    MOVE_IMPRISON,
    MOVE_MEMENTO,
    MOVE_SUCKER_PUNCH,
    MOVE_SHADOW_SNEAK,
    MOVE_CURSE,
    MOVE_SPITE,
    MOVE_OMINOUS_WIND,
    (SPECIES_GIRAFARIG) + 20000, // Girafarig
    MOVE_TAKE_DOWN,
    MOVE_AMNESIA,
    MOVE_FORESIGHT,
    MOVE_FUTURE_SIGHT,
    MOVE_BEAT_UP,
    MOVE_PSYCH_UP,
    MOVE_WISH,
    MOVE_MAGIC_COAT,
    MOVE_DOUBLE_KICK,
    MOVE_MIRROR_COAT,
    (SPECIES_PINECO) + 20000, // Pineco line
    MOVE_REFLECT,
    MOVE_PIN_MISSILE,
    MOVE_FLAIL,
    MOVE_SWIFT,
    MOVE_COUNTER,
    MOVE_SAND_TOMB,
    MOVE_REVENGE,
    MOVE_DOUBLE_EDGE,
    MOVE_TOXIC_SPIKES,
    (SPECIES_DUNSPARCE) + 20000, // Dunsparce
    MOVE_BIDE,
    MOVE_ANCIENT_POWER,
    MOVE_ROCK_SLIDE,
    MOVE_BITE,
    MOVE_HEADBUTT,
    MOVE_ASTONISH,
    MOVE_CURSE,
    MOVE_TRUMP_CARD,
    MOVE_MAGIC_COAT,
    MOVE_SNORE,
    (SPECIES_GLIGAR) + 20000, // Gligar line
    MOVE_METAL_CLAW,
    MOVE_WING_ATTACK,
    MOVE_RAZOR_WIND,
    MOVE_COUNTER,
    MOVE_SAND_TOMB,
    MOVE_AGILITY,
    MOVE_BATON_PASS,
    MOVE_DOUBLE_EDGE,
    MOVE_FEINT,
    MOVE_NIGHT_SLASH,
    MOVE_CROSS_POISON,
    (SPECIES_SNUBBULL) + 20000, // Snubbull line
    MOVE_METRONOME,
    MOVE_FAINT_ATTACK,
    MOVE_REFLECT,
    MOVE_PRESENT,
    MOVE_CRUNCH,
    MOVE_HEAL_BELL,
    MOVE_SNORE,
    MOVE_SMELLING_SALT,
    MOVE_CLOSE_COMBAT,
    MOVE_ICE_FANG,
    MOVE_FIRE_FANG,
    MOVE_THUNDER_FANG,
    (SPECIES_QWILFISH) + 20000, // Qwilfish line
    MOVE_FLAIL,
    MOVE_HAZE,
    MOVE_BUBBLE_BEAM,
    MOVE_SUPERSONIC,
    MOVE_ASTONISH,
    MOVE_SIGNAL_BEAM,
    MOVE_POISON_JAB,
    (SPECIES_SHUCKLE) + 20000, // Shuckle
    MOVE_SWEET_SCENT,
    MOVE_KNOCK_OFF,
    MOVE_HELPING_HAND,
    MOVE_ACUPRESSURE,
    MOVE_SAND_TOMB,
    MOVE_MUD_SLAP,
    (SPECIES_HERACROSS) + 20000, // Heracross
    MOVE_HARDEN,
    MOVE_BIDE,
    MOVE_FLAIL,
    MOVE_FALSE_SWIPE,
    MOVE_REVENGE,
    MOVE_PURSUIT,
    MOVE_DOUBLE_EDGE,
    (SPECIES_SNEASEL) + 20000, // Sneasel line
    MOVE_COUNTER,
    MOVE_SPITE,
    MOVE_FORESIGHT,
    MOVE_REFLECT,
    MOVE_BITE,
    MOVE_CRUSH_CLAW,
    MOVE_FAKE_OUT,
    MOVE_DOUBLE_HIT,
    MOVE_PUNISHMENT,
    MOVE_PURSUIT,
    MOVE_ICE_SHARD,
    MOVE_ICE_PUNCH,
    (SPECIES_TEDDIURSA) + 20000, // Teddiursa line
    MOVE_CRUNCH,
    MOVE_TAKE_DOWN,
    MOVE_SEISMIC_TOSS,
    MOVE_COUNTER,
    MOVE_METAL_CLAW,
    MOVE_FAKE_TEARS,
    MOVE_YAWN,
    MOVE_SLEEP_TALK,
    MOVE_CROSS_CHOP,
    MOVE_DOUBLE_EDGE,
    MOVE_CLOSE_COMBAT,
    MOVE_NIGHT_SLASH,
    (SPECIES_SLUGMA) + 20000, // Slugma line
    MOVE_ACID_ARMOR,
    MOVE_HEAT_WAVE,
    MOVE_CURSE,
    MOVE_SMOKE_SCREEN,
    MOVE_MEMENTO,
    MOVE_STOCKPILE,
    MOVE_SPIT_UP,
    MOVE_SWALLOW,
    (SPECIES_SWINUB) + 20000, // Swinub line
    MOVE_TAKE_DOWN,
    MOVE_BITE,
    MOVE_BODY_SLAM,
    MOVE_ROCK_SLIDE,
    MOVE_ANCIENT_POWER,
    MOVE_MUD_SHOT,
    MOVE_ICICLE_SPEAR,
    MOVE_DOUBLE_EDGE,
    MOVE_FISSURE,
    MOVE_CURSE,
    (SPECIES_CORSOLA) + 20000, // Corsola
    MOVE_ROCK_SLIDE,
    MOVE_SCREECH,
    MOVE_MIST,
    MOVE_AMNESIA,
    MOVE_BARRIER,
    MOVE_INGRAIN,
    MOVE_CONFUSE_RAY,
    MOVE_ICICLE_SPEAR,
    MOVE_NATURE_POWER,
    MOVE_AQUA_RING,
    (SPECIES_REMORAID) + 20000, // Remoraid line
    MOVE_AURORA_BEAM,
    MOVE_OCTAZOOKA,
    MOVE_SUPERSONIC,
    MOVE_HAZE,
    MOVE_SCREECH,
    MOVE_THUNDER_WAVE,
    MOVE_ROCK_BLAST,
    MOVE_SNORE,
    MOVE_FLAIL,
    (SPECIES_DELIBIRD) + 20000, // Delibird
    MOVE_AURORA_BEAM,
    MOVE_QUICK_ATTACK,
    MOVE_FUTURE_SIGHT,
    MOVE_SPLASH,
    MOVE_RAPID_SPIN,
    MOVE_ICE_BALL,
    MOVE_ICE_SHARD,
    MOVE_ICE_PUNCH,
    (SPECIES_MANTINE) + 20000, // Mantine
    MOVE_TWISTER,
    MOVE_HYDRO_PUMP,
    MOVE_HAZE,
    MOVE_SLAM,
    MOVE_MUD_SPORT,
    MOVE_ROCK_SLIDE,
    MOVE_MIRROR_COAT,
    MOVE_WATER_SPORT,
    MOVE_SPLASH,
    (SPECIES_SKARMORY) + 20000, // Skarmory
    MOVE_DRILL_PECK,
    MOVE_PURSUIT,
    MOVE_WHIRLWIND,
    MOVE_SKY_ATTACK,
    MOVE_CURSE,
    MOVE_BRAVE_BIRD,
    MOVE_ASSURANCE,
    (SPECIES_HOUNDOUR) + 20000, // Houndour line
    MOVE_FIRE_SPIN,
    MOVE_RAGE,
    MOVE_PURSUIT,
    MOVE_COUNTER,
    MOVE_SPITE,
    MOVE_REVERSAL,
    MOVE_BEAT_UP,
    MOVE_WILL_O_WISP,
    MOVE_FIRE_FANG,
    MOVE_THUNDER_FANG,
    MOVE_NASTY_PLOT,
    MOVE_PUNISHMENT,
    (SPECIES_PHANPY) + 20000, // Phanpy line
    MOVE_FOCUS_ENERGY,
    MOVE_BODY_SLAM,
    MOVE_ANCIENT_POWER,
    MOVE_SNORE,
    MOVE_COUNTER,
    MOVE_FISSURE,
    MOVE_ENDEAVOR,
    MOVE_ICE_SHARD,
    (SPECIES_STANTLER) + 20000, // Stantler
    MOVE_SPITE,
    MOVE_DISABLE,
    MOVE_BITE,
    MOVE_SWAGGER,
    MOVE_PSYCH_UP,
    MOVE_EXTRASENSORY,
    MOVE_THRASH,
    MOVE_DOUBLE_KICK,
    MOVE_ZEN_HEADBUTT,
    (SPECIES_TYROGUE) + 20000, // Tyrogue line
    MOVE_RAPID_SPIN,
    MOVE_HI_JUMP_KICK,
    MOVE_MACH_PUNCH,
    MOVE_MIND_READER,
    MOVE_HELPING_HAND,
    MOVE_COUNTER,
    MOVE_VACUUM_WAVE,
    MOVE_BULLET_PUNCH,
    (SPECIES_SMOOCHUM) + 20000, // Smoochum line
    MOVE_MEDITATE,
    MOVE_PSYCH_UP,
    MOVE_FAKE_OUT,
    MOVE_WISH,
    MOVE_ICE_PUNCH,
    MOVE_MIRACLE_EYE,
    (SPECIES_ELEKID) + 20000, // Elekid line
    MOVE_KARATE_CHOP,
    MOVE_BARRIER,
    MOVE_ROLLING_KICK,
    MOVE_MEDITATE,
    MOVE_CROSS_CHOP,
    MOVE_FIRE_PUNCH,
    MOVE_ICE_PUNCH,
    MOVE_DYNAMIC_PUNCH,
    (SPECIES_MAGBY) + 20000, // Magby line
    MOVE_KARATE_CHOP,
    MOVE_MEGA_PUNCH,
    MOVE_BARRIER,
    MOVE_SCREECH,
    MOVE_CROSS_CHOP,
    MOVE_THUNDER_PUNCH,
    MOVE_MACH_PUNCH,
    MOVE_DYNAMIC_PUNCH,
    MOVE_FLARE_BLITZ,
    (SPECIES_MILTANK) + 20000, // Miltank line
    MOVE_PRESENT,
    MOVE_REVERSAL,
    MOVE_SEISMIC_TOSS,
    MOVE_ENDURE,
    MOVE_PSYCH_UP,
    MOVE_CURSE,
    MOVE_HELPING_HAND,
    MOVE_SLEEP_TALK,
    MOVE_DIZZY_PUNCH,
    MOVE_HAMMER_ARM,
    MOVE_DOUBLE_EDGE,
    MOVE_PUNISHMENT,
    (SPECIES_LARVITAR) + 20000, // Larvitar line
    MOVE_PURSUIT,
    MOVE_STOMP,
    MOVE_OUTRAGE,
    MOVE_FOCUS_ENERGY,
    MOVE_ANCIENT_POWER,
    MOVE_DRAGON_DANCE,
    MOVE_CURSE,
    MOVE_IRON_DEFENSE,
    MOVE_ASSURANCE,
    MOVE_IRON_HEAD,
    (SPECIES_TREECKO) + 20000, // Treecko line
    242,
    300,
    283,
    73,
    225,
    306,
    388,
    24,
    320,
    235,
    345,
    437,
    (SPECIES_TORCHIC) + 20000, // Torchic line
    68,
    179,
    203,
    207,
    157,
    265,
    306,
    226,
    97,
    400,
    387,
    364,
    (SPECIES_MUDKIP) + 20000, // Mudkip line
    287,
    253,
    174,
    23,
    301,
    243,
    68,
    246,
    250,
    44,
    38,
    426,
    (SPECIES_POOCHYENA) + 20000, // Poochyena line
    310,
    305,
    343,
    43,
    281,
    389,
    423,
    424,
    422,
    382,
    (SPECIES_ZIGZAGOON) + 20000, // Zigzagoon line
    204,
    228,
    164,
    321,
    271,
    270,
    189,
    (SPECIES_LOTAD) + 20000, // Lotad line
    235,
    75,
    230,
    73,
    175,
    55,
    321,
    (SPECIES_SEEDOT) + 20000, // Seedot line
    73,
    133,
    98,
    13,
    36,
    206,
    388,
    417,
    (SPECIES_TAILLOW) + 20000, // Taillow line
    228,
    48,
    287,
    119,
    99,
    143,
    18,
    413,
    (SPECIES_WINGULL) + 20000, // Wingull line
    54,
    239,
    97,
    16,
    346,
    392,
    282,
    (SPECIES_RALTS) + 20000, // Ralts line
    50,
    261,
    212,
    262,
    194,
    288,
    425,
    109,
    (SPECIES_SURSKIT) + 20000, // Surskit line
    193,
    341,
    60,
    56,
    170,
    324,
    450,
    (SPECIES_SHROOMISH) + 20000, //Shroomish line
    313,
    207,
    204,
    206,
    270,
    388,
    358,
    402,
    (SPECIES_SLAKOTH) + 20000, // Slakoth line
    228,
    163,
    34,
    173,
    306,
    174,
    214,
    359,
    400,
    (SPECIES_NINCADA) + 20000, // Nincada line
    203,
    185,
    16,
    318,
    405,
    400,
    450,
    (SPECIES_WHISMUR) + 20000, // Whismur line
    36,
    173,
    207,
    326,
    265,
    108,
    283,
    (SPECIES_MAKUHITA) + 20000, // Makuhita line
    185,
    197,
    193,
    270,
    238,
    279,
    223,
    68,
    358,
    418,
    (SPECIES_AZURILL) + 20000, //Azurill
    227,
    47,
    287,
    21,
    321,
    313,
    (SPECIES_NOSEPASS) + 20000, // Nosepass line
    222,
    205,
    153,
    38,
    335,
    (SPECIES_SKITTY) + 20000, // Skitty line
    270,
    244,
    253,
    313,
    273,
    226,
    164,
    321,
    387,
    252,
    428,
    389,
    (SPECIES_SABLEYE) + 20000, // Sableye
    244,
    105,
    236,
    417,
    260,
    (SPECIES_MAWILE) + 20000, // Mawile
    14,
    206,
    305,
    244,
    246,
    321,
    389,
    423,
    424,
    422,
    386,
    (SPECIES_ARON) + 20000, // Aron line
    283,
    34,
    23,
    265,
    174,
    103,
    442,
    407,
    (SPECIES_MEDITITE) + 20000, // Meditite line
    7,
    9,
    8,
    193,
    252,
    226,
    223,
    384,
    385,
    427,
    418,
    (SPECIES_ELECTRIKE) + 20000, // Electrike line
    242,
    29,
    253,
    174,
    129,
    435,
    423,
    424,
    422,
    (SPECIES_PLUSLE) + 20000, // Plusle
    164,
    273,
    47,
    186,
    (SPECIES_MINUN) + 20000, // Minun
    164,
    273,
    47,
    186,
    (SPECIES_VOLBEAT) + 20000, // Volbeat
    226,
    318,
    271,
    227,
    405,
    (SPECIES_ILLUMISE) + 20000, // Illumise
    226,
    318,
    74,
    227,
    405,
    (SPECIES_ROSELIA) + 20000, // Roselia line
    191,
    235,
    42,
    178,
    79,
    75,
    170,
    437,
    (SPECIES_GULPIN) + 20000, // Gulpin line
    138,
    151,
    123,
    220,
    174,
    194,
    (SPECIES_CARVANHA) + 20000, // Carvanha line
    56,
    38,
    37,
    246,
    (SPECIES_WAILMER) + 20000, // Wailmer line
    38,
    37,
    207,
    173,
    214,
    174,
    90,
    321,
    111,
    34,
    (SPECIES_NUMEL) + 20000, // Numel line
    336,
    184,
    34,
    205,
    111,
    23,
    281,
    246,
    426,
    257,
    (SPECIES_TORKOAL) + 20000, // Torkoal
    284,
    203,
    214,
    281,
    89,
    90,
    (SPECIES_SPOINK) + 20000, // Spoink line
    248,
    326,
    164,
    271,
    428,
    133,
    243,
    (SPECIES_SPINDA) + 20000, // Spinda
    227,
    157,
    274,
    50,
    226,
    273,
    271,
    265,
    252,
    272,
    427,
    (SPECIES_TRAPINCH) + 20000, // Trapinch line
    116,
    98,
    16,
    175,
    210,
    (SPECIES_CACNEA) + 20000, // Cacnea line
    320,
    51,
    298,
    223,
    68,
    67,
    265,
    345,
    402,
    (SPECIES_SWABLU) + 20000, // Swablu line
    97,
    114,
    228,
    99,
    297,
    407,
    (SPECIES_ZANGOOSE) + 20000, // Zangoose
    175,
    24,
    13,
    68,
    46,
    174,
    154,
    400,
    232,
    458,
    (SPECIES_SEVIPER) + 20000, // Seviper
    254,
    256,
    255,
    34,
    184,
    372,
    400,
    (SPECIES_BARBOACH) + 20000, // Barboach line
    37,
    250,
    209,
    56,
    175,
    36,
    (SPECIES_CORPHISH) + 20000, // Corphish line
    300,
    283,
    34,
    246,
    282,
    276,
    232,
    (SPECIES_LILEEP) + 20000, // Lileep line
    112,
    105,
    243,
    157,
    378,
    321,
    (SPECIES_ANORITH) + 20000, // Anorith line
    229,
    282,
    14,
    157,
    103,
    28,
    440,
    (SPECIES_FEEBAS) + 20000, // Feebas line
    243,
    225,
    300,
    95,
    113,
    109,
    54,
    114,
    321,
    (SPECIES_CASTFORM) + 20000, // Castform
    248,
    244,
    381,
    50,
    133,
    466,
    (SPECIES_KECLEON) + 20000, // Kecleon
    50,
    277,
    271,
    252,
    417,
    146,
    (SPECIES_SHUPPET) + 20000, // Shuppet line
    50,
    194,
    193,
    310,
    286,
    228,
    425,
    371,
    109,
    (SPECIES_DUSKULL) + 20000, // Duskull line
    286,
    194,
    220,
    288,
    262,
    185,
    466,
    (SPECIES_TROPIUS) + 20000, // Tropius
    29,
    21,
    13,
    73,
    267,
    437,
    235,
    174,
    348,
    (SPECIES_CHIMECHO) + 20000, //Chimecho
    50,
    174,
    95,
    138,
    273,
    248,
    (SPECIES_ABSOL) + 20000, // Absol
    226,
    185,
    38,
    277,
    174,
    164,
    212,
    428,
    386,
    389,
    372,
    382,
    (SPECIES_SNORUNT) + 20000, // Snorunt line
    335,
    191,
    205,
    50,
    117,
    (SPECIES_SPHEAL) + 20000, // Spheal line
    346,
    254,
    256,
    255,
    281,
    157,
    174,
    90,
    324,
    (SPECIES_CLAMPERL) + 20000, // Clamperl line
    287,
    300,
    34,
    48,
    112,
    109,
    392,
    (SPECIES_RELICANTH) + 20000, // Relicanth
    222,
    130,
    346,
    133,
    214,
    157,
    401,
    173,
    189,
    (SPECIES_LUVDISC) + 20000, // Luvdisc
    150,
    48,
    346,
    300,
    445,
    392,
    (SPECIES_BAGON) + 20000, // Bagon line
    56,
    37,
    82,
    239,
    349,
    424,
    421,
    407,
    (SPECIES_TURTWIG) + 20000, // Turtwig line
    388,
    74,
    321,
    34,
    38,
    328,
    402,
    37,
    133,
    276,
    (SPECIES_CHIMCHAR) + 20000, // Chimchar line
    7,
    9,
    24,
    227,
    257,
    116,
    270,
    252,
    299,
    68,
    (SPECIES_PIPLUP) + 20000, // Piplup line
    458,
    48,
    281,
    300,
    189,
    173,
    175,
    97,
    392,
    56,
    (SPECIES_STARLY) + 20000, // Starly line
    297,
    31,
    228,
    310,
    28,
    193,
    38,
    (SPECIES_BIDOOF) + 20000, // Bidoof line
    98,
    346,
    38,
    154,
    111,
    205,
    316,
    401,
    (SPECIES_SHINX) + 20000, // Shinx line
    423,
    424,
    422,
    98,
    336,
    36,
    (SPECIES_BUDEW) + 20000, // Budew
    191,
    235,
    42,
    178,
    79,
    75,
    170,
    437,
    326,
    (SPECIES_CRANIDOS) + 20000, // Cranidos line
    242,
    37,
    38,
    43,
    21,
    23,
    18,
    359,
    (SPECIES_SHIELDON) + 20000, // Shieldon line
    29,
    184,
    116,
    38,
    350,
    34,
    103,
    174,
    90,
    (SPECIES_PACHIRISU) + 20000, // Pachirisu line
    343,
    44,
    313,
    111,
    205,
    260,
    175,
    (SPECIES_BUIZEL) + 20000, // Buizel line
    189,
    29,
    154,
    163,
    316,
    3,
    210,
    226,
    (SPECIES_CHERUBI) + 20000, // Cherubi line
    75,
    230,
    321,
    267,
    320,
    312,
    (SPECIES_SHELLOS) + 20000, // Shellos line
    68,
    243,
    254,
    256,
    255,
    281,
    262,
    174,
    133,
    90,
    (SPECIES_DRIFLOON) + 20000, // Drifloon line
    262,
    34,
    194,
    50,
    114,
    95,
    (SPECIES_BUNEARY) + 20000, // Buneary line
    313,
    252,
    227,
    186,
    458,
    213,
    67,
    327,
    415,
    9,
    8,
    7,
    (SPECIES_GLAMEOW) + 20000, // Glameow line
    44,
    39,
    98,
    28,
    313,
    372,
    (SPECIES_CHINGLING) + 20000, // Chingling
    50,
    174,
    95,
    138,
    273,
    248,
    105,
    (SPECIES_STUNKY) + 20000, // Stunky line
    228,
    43,
    123,
    38,
    242,
    184,
    310,
    386,
    (SPECIES_BONSLY) + 20000, // Bonsly
    120,
    29,
    106,
    111,
    205,
    328,
    (SPECIES_MIME_JR) + 20000, // Mime Jr.
    248,
    95,
    102,
    244,
    252,
    271,
    109,
    358,
    298,
    361,
    204,
    (SPECIES_HAPPINY) + 20000, // Happiny
    217,
    118,
    215,
    312,
    164,
    68,
    270,
    356,
    387,
    (SPECIES_CHATOT) + 20000, // Chatot
    227,
    101,
    97,
    417,
    48,
    (SPECIES_SPIRITOMB) + 20000, // Spiritomb
    194,
    220,
    108,
    286,
    288,
    425,
    (SPECIES_GIBLE) + 20000, // Gible line
    225,
    200,
    239,
    184,
    38,
    37,
    232,
    328,
    34,
    442,
    (SPECIES_MUNCHLAX) + 20000, // Munchlax
    122,
    204,
    38,
    174,
    164,
    18,
    228,
    428,
    (SPECIES_RIOLU) + 20000, // Riolu line
    238,
    197,
    44,
    170,
    327,
    136,
    97,
    410,
    242,
    67,
    334,
    299,
    418,
    (SPECIES_HIPPOPOTAS) + 20000, // Hippopotas line
    254,
    256,
    255,
    174,
    303,
    34,
    328,
    (SPECIES_SKORUPI) + 20000, // Skorupi line
    185,
    103,
    28,
    163,
    109,
    18,
    97,
    228,
    400,
    (SPECIES_CROAGUNK) + 20000, // Croagunk line
    382,
    364,
    223,
    29,
    410,
    96,
    252,
    358,
    265,
    238,
    418,
    (SPECIES_CARNIVINE) + 20000, // Carnivine line
    79,
    78,
    75,
    21,
    235,
    345,
    73,
    388,
    (SPECIES_FINNEON) + 20000, // Finneon line
    186,
    204,
    175,
    401,
    150,
    60,
    321,
    97,
    (SPECIES_MANTYKE) + 20000, // Mantyke
    239,
    56,
    114,
    21,
    300,
    157,
    243,
    346,
    150,
    324,
    (SPECIES_SNOVER) + 20000, // Snover line
    73,
    345,
    402,
    74,
    38,
    54,
    23,
    0xffff,
};

typedef struct {
    int unk_00[4];
    int unk_10[4];
    int unk_20[4];
    u16 unk_30[50];
    u16 unk_94[16];
} UnkStruct_ov5_021E6948;

void ov5_021E6DE8(Pokemon * param0, u16 param1, UnkStruct_02026310 * param2, u32 param3, u8 param4);
void sub_020262C0(UnkStruct_02026224 * param0);
static u8 ov5_021E70FC(UnkStruct_02026310 * param0);
static int ov5_021E6F6C(Party * param0);
static u8 ov5_021E6FF0(BoxPokemon ** param0);
void ov5_021E6B40(UnkStruct_02026310 * param0);
int ov5_021E6630(UnkStruct_02026310 * param0, u8 param1, StringTemplate * param2);
u8 ov5_021E6640(UnkStruct_02026310 * param0, int param1, StringTemplate * param2);
u16 ov5_021E73A0(Party * param0, int param1, StringTemplate * param2);
u8 ov5_021E73C8(UnkStruct_02026310 * param0);
void ov5_021E72BC(UnkStruct_02026310 * param0, StringTemplate * param1);
static void ov5_021E62C4(Party * param0, int param1, UnkStruct_02026218 * param2, SaveData * param3);
static int ov5_021E7110(FieldSystem * fieldSystem);

static BoxPokemon * ov5_021E622C (UnkStruct_02026310 * param0, int param1)
{
    return sub_02026220(sub_02026218(param0, param1));
}

static UnkStruct_02026310 * Unk_ov5_02202124;

u8 ov5_021E6238 (UnkStruct_02026310 * param0)
{
    u8 v0, v1;
    BoxPokemon * v2;

    v0 = 0;

    for (v1 = 0; v1 < 2; v1++) {
        v2 = sub_02026220(sub_02026218(param0, v1));

        if (BoxPokemon_GetValue(v2, MON_DATA_SPECIES, NULL) != 0) {
            v0++;
        }
    }

    return v0;
}

int ov5_021E6270 (UnkStruct_02026310 * param0)
{
    u8 v0;
    BoxPokemon * v1;

    Unk_ov5_02202124 = param0;

    for (v0 = 0; v0 < 2; v0++) {
        v1 = sub_02026220(sub_02026218(param0, v0));

        if (BoxPokemon_GetValue(v1, MON_DATA_SPECIES, NULL) == 0) {
            return v0;
        }
    }

    return -1;
}

static int ov5_021E62B0 (BoxPokemon * param0)
{
    int v0 = BoxPokemon_GetValue(param0, MON_DATA_HELD_ITEM, NULL);
    return Item_IsMail(v0);
}

static void ov5_021E62C4 (Party * param0, int param1, UnkStruct_02026218 * param2, SaveData * param3)
{
    int v0;
    Pokemon * v1 = Party_GetPokemonBySlotIndex(param0, param1);
    const u16 * v2;
    u16 v3[10 + 1];
    UnkStruct_02026224 * v4 = sub_02026224(param2);
    BoxPokemon * v5 = sub_02026220(param2);
    TrainerInfo * v6 = SaveData_GetTrainerInfo(param3);

    v2 = TrainerInfo_Name(v6);
    Pokemon_GetValue(v1, MON_DATA_NICKNAME, v3);

    if (ov5_021E62B0(Pokemon_GetBoxPokemon(v1))) {
        Pokemon_GetValue(v1, MON_DATA_170, sub_02026230(v4));
    }

    BoxPokemon_FromPokemon(v1, v5);
    BoxPokemon_SetShayminForm(v5, 0);
    sub_02026258(param2, 0);
    Party_RemovePokemonBySlotIndex(param0, param1);

    if (Party_HasSpecies(param0, 441) == 0) {
        ChatotCry * v7 = GetChatotCryDataFromSave(param3);
        ResetChatotCryDataStatus(v7);
    }
}

void ov5_021E6358 (Party * param0, int param1, UnkStruct_02026310 * param2, SaveData * param3)
{
    int v0;
    UnkStruct_0202CD88 * v1 = sub_0202CD88(param3);

    sub_0202CF28(v1, 1 + 39);
    v0 = ov5_021E6270(param2);
    ov5_021E62C4(param0, param1, sub_02026218(param2, v0), param3);
}

static void ov5_021E638C (UnkStruct_02026310 * param0)
{
    UnkStruct_02026218 * v0, * v1;
    BoxPokemon * v2, * v3;

    v0 = sub_02026218(param0, 0);
    v1 = sub_02026218(param0, 1);
    v2 = sub_02026220(v0);
    v3 = sub_02026220(v1);

    if (BoxPokemon_GetValue(v2, MON_DATA_SPECIES, NULL) == 0) {
        if (BoxPokemon_GetValue(v3, MON_DATA_SPECIES, NULL) != 0) {
            sub_020262A8(v0, v1);
            sub_020262F4(v1);
        }
    }
}

static void ov5_021E63E0 (Pokemon * param0)
{
    int v0, v1 = 0, v2;
    u16 v3;
    u16 v4;

    for (v0 = 0; v0 < 100; v0++) {
        if (Pokemon_ShouldLevelUp(param0)) {
            v1 = 0;

            while ((v4 = Pokemon_LevelUpMove(param0, &v1, &v3)) != 0) {
                if (v4 == 0xffff) {
                    Pokemon_ReplaceMove(param0, v3);
                }
            }
        } else {
            break;
        }
    }

    Pokemon_CalcLevelAndStats(param0);
}

static int ov5_021E6444 (Party * param0, UnkStruct_02026218 * param1, StringTemplate * param2)
{
    Pokemon * v0 = Pokemon_New(4);
    BoxPokemon * v1 = sub_02026220(param1);
    UnkStruct_02026224 * v2 = sub_02026224(param1);
    u32 v3;
    u16 v4;

    StringTemplate_SetNickname(param2, 0, v1);
    v4 = BoxPokemon_GetValue(v1, MON_DATA_SPECIES, NULL);
    Pokemon_FromBoxPokemon(v1, v0);

    if (Pokemon_GetValue(v0, MON_DATA_LEVEL, NULL) != 100) {
        v3 = Pokemon_GetValue(v0, MON_DATA_EXP, NULL);
        v3 += sub_02026228(param1);
        Pokemon_SetValue(v0, 8, (u8 *)&v3);
        ov5_021E63E0(v0);
    }

    if (ov5_021E62B0(v1)) {
        Pokemon_SetValue(v0, 170, sub_02026230(v2));
    }

    Party_AddPokemon(param0, v0);
    BoxPokemon_Init(v1);
    sub_02026258(param1, 0);
    Heap_FreeToHeap(v0);

    return v4;
}

u16 ov5_021E64F8 (Party * param0, StringTemplate * param1, UnkStruct_02026310 * param2, u8 param3)
{
    u16 v0;
    UnkStruct_02026218 * v1 = sub_02026218(param2, param3);

    v0 = ov5_021E6444(param0, v1, param1);
    ov5_021E638C(param2);

    return v0;
}

int ov5_021E6520 (BoxPokemon * param0, u32 param1)
{
    Pokemon * v0 = Pokemon_New(4);
    BoxPokemon * v1 = Pokemon_GetBoxPokemon(v0);
    int v2;
    u32 v3;

    BoxPokemon_Copy(param0, v1);

    v3 = BoxPokemon_GetValue(v1, MON_DATA_EXP, NULL);
    v3 += param1;

    BoxPokemon_SetValue(v1, 8, (u8 *)&v3);
    v2 = BoxPokemon_GetLevel(v1);
    Heap_FreeToHeap(v0);

    return v2;
}

int ov5_021E6568 (UnkStruct_02026218 * param0)
{
    u8 v0, v1;
    BoxPokemon * v2;

    v2 = sub_02026220(param0);
    v0 = BoxPokemon_GetLevel(v2);
    v1 = ov5_021E6520(v2, sub_02026228(param0));

    return v1 - v0;
}

int ov5_021E6590 (UnkStruct_02026218 * param0)
{
    u8 v0;
    BoxPokemon * v1;

    v1 = sub_02026220(param0);
    v0 = ov5_021E6520(v1, sub_02026228(param0));

    return v0;
}

u8 ov5_021E65B0 (UnkStruct_02026218 * param0, StringTemplate * param1)
{
    int v0;
    Strbuf* v1;
    u16 v2[10 + 1];
    BoxPokemon * v3 = sub_02026220(param0);

    v0 = ov5_021E6568(param0);

    StringTemplate_SetNumber(param1, 1, v0, 3, 0, 1);
    StringTemplate_SetNickname(param1, 0, v3);

    return v0;
}

int ov5_021E65EC (UnkStruct_02026218 * param0, StringTemplate * param1)
{
    u16 v0;
    BoxPokemon * v1 = sub_02026220(param0);

    v0 = ov5_021E6568(param0);
    StringTemplate_SetNickname(param1, 0, v1);

    v0 = v0 * 100 + 100;
    StringTemplate_SetNumber(param1, 1, v0, 5, 0, 1);

    return v0;
}

int ov5_021E6630 (UnkStruct_02026310 * param0, u8 param1, StringTemplate * param2)
{
    UnkStruct_02026218 * v0;

    v0 = sub_02026218(param0, param1);
    return ov5_021E65EC(v0, param2);
}

u8 ov5_021E6640 (UnkStruct_02026310 * param0, int param1, StringTemplate * param2)
{
    UnkStruct_02026218 * v0 = sub_02026218(param0, param1);
    BoxPokemon * v1 = sub_02026220(v0);

    if (BoxPokemon_GetValue(v1, MON_DATA_SPECIES, NULL)) {
        return ov5_021E65B0(v0, param2);
    }

    return 0;
}

static void ov5_021E6668 (UnkStruct_02026310 * param0, BoxPokemon * param1[])
{
    param1[0] = ov5_021E622C(param0, 0);
    param1[1] = ov5_021E622C(param0, 1);
}

static int ov5_021E6684 (UnkStruct_02026310 * param0)
{
    int v0;
    int v1[2], v2 = -1, v3, mateLevel[2];
    BoxPokemon * v4[2];

    ov5_021E6668(param0, v4);

    for (v0 = 0; v0 < 2; v0++) {
        if (BoxPokemon_GetGender(v4[v0]) == 1) {
            v2 = v0;
        }
    }
	
	if (BoxPokemon_GetGender(v4[0]) == BoxPokemon_GetGender(v4[1]))
	{
        mateLevel[0] = BoxPokemon_GetLevel(v4[0]);
		mateLevel[1] = BoxPokemon_GetLevel(v4[1]);
		
		if (mateLevel[0] < mateLevel[1])
		{
			v2 = 0;
		}
		else
		{
			v2 = 1;
		}
		
		if (mateLevel[0] == mateLevel[1])
		{
			if (LCRNG_Next() >= (0xffff / 2))
			{
				v2 = 0;
			}
			else
			{
				v2 = 1;
			}
		}
    }

    for (v3 = 0, v0 = 0; v0 < 2; v0++) {
        if ((v1[v0] = BoxPokemon_GetValue(v4[v0], MON_DATA_SPECIES, NULL)) == 132) {
            v3++;
            v2 = v0;
        }
    }

    if (v3 == 2) {
        if (LCRNG_Next() >= (0xffff / 2)) {
            v2 = 0;
        } else {
            v2 = 1;
        }
        for (v0 = 0; v0 < 2; v0++) { // 229 == ITEM_EVERSTONE
            if (BoxPokemon_GetValue(v4[v0], MON_DATA_HELD_ITEM, NULL) == 229) {
                v2 = v0;
            }
        }
    }
    // 229 == ITEM_EVERSTONE
    if (BoxPokemon_GetValue(v4[v2], MON_DATA_HELD_ITEM, NULL) == 229) {
        if (LCRNG_Next() <= (0xffff / 20)) {
            return -1;
        }
    } else {
        return -1;
    }

    return v2;
}

void ov5_021E6720 (UnkStruct_02026310 * param0)
{
    u32 v0 = 0, v1;
    int v2, v3;
    int v4 = 0;

    if ((v2 = ov5_021E6684(param0)) < 0) {
        sub_02026270(param0, MTRNG_Next());
    } else {
        BoxPokemon * v5 = ov5_021E622C(param0, v2);

        v0 = BoxPokemon_GetValue(v5, MON_DATA_PERSONALITY, NULL);
        v3 = Pokemon_GetNatureOf(v0);

        while (TRUE) {
            v1 = MTRNG_Next();

            if ((v3 == Pokemon_GetNatureOf(v1)) && (v1 != 0)) {
                break;
            }

            if (++v4 > 2400) {
                break;
            }
        }

        sub_02026270(param0, v1);
    }
}

static void ov5_021E6778 (u8 * param0, u8 param1)
{
    int v0, v1;
    u8 v2[6];

    param0[param1] = 0xff;

    for (v0 = 0; v0 < 6; v0++) {
        v2[v0] = param0[v0];
    }

    v1 = 0;

    for (v0 = 0; v0 < 6; v0++) {
        if (v2[v0] != 0xff) {
            param0[v1++] = v2[v0];
        }
    }
}

static void ov5_021E67B0 (Pokemon * param0, UnkStruct_02026310 * param1)
{
    u8 v0[3], v1, v2[6], v3[3], v4;

    for (v1 = 0; v1 < 6; v1++) {
        v2[v1] = v1;
    }

    for (v1 = 0; v1 < 3; v1++) {
        v0[v1] = v2[LCRNG_Next() % (6 - v1)];
        ov5_021E6778(v2, v1);
    }

    for (v1 = 0; v1 < 3; v1++) {
        v3[v1] = LCRNG_Next() % 2;
    }

    for (v1 = 0; v1 < 3; v1++) {
        BoxPokemon * v5 = ov5_021E622C(param1, v3[v1]);

        switch (v0[v1]) {
        case 0:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_HP_IV, NULL);
            Pokemon_SetValue(param0, 70, (u8 *)&v4);
            break;
        case 1:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_ATK_IV, NULL);
            Pokemon_SetValue(param0, 71, (u8 *)&v4);
            break;
        case 2:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_DEF_IV, NULL);
            Pokemon_SetValue(param0, 72, (u8 *)&v4);
            break;
        case 3:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_SPEED_IV, NULL);
            Pokemon_SetValue(param0, 73, (u8 *)&v4);
            break;
        case 4:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_SPATK_IV, NULL);
            Pokemon_SetValue(param0, 74, (u8 *)&v4);
            break;
        case 5:
            v4 = BoxPokemon_GetValue(v5, MON_DATA_SPDEF_IV, NULL);
            Pokemon_SetValue(param0, 75, (u8 *)&v4);
            break;
        }
    }
}

static u8 ov5_021E68D8 (Pokemon * param0, u16 * param1)
{
    u16 v0, v1, v2, v3;

    v2 = 0;
    v1 = 0;
    v0 = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);

    for (v3 = 0; v3 < 1909; v3++) {
        if (EggMove_Table[v3] == (20000 + v0)) {
            v1 = v3 + 1;
            break;
        }
    }

    for (v3 = 0; v3 < 16; v3++) {
        if (EggMove_Table[v1 + v3] > 20000) {
            break;
        } else {
            param1[v3] = EggMove_Table[v1 + v3];
            v2++;
        }
    }

    return v2;
}

static void ov5_021E6948 (Pokemon * param0, BoxPokemon * param1, BoxPokemon * param2)
{
    u16 v0, v1, v2, v3, v4, v5, v6;
    UnkStruct_ov5_021E6948 * v7 = Heap_AllocFromHeap(4, sizeof(UnkStruct_ov5_021E6948));

    v2 = 0;

    MI_CpuClearFast(v7, sizeof(UnkStruct_ov5_021E6948));

    v3 = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);
    v6 = Pokemon_GetValue(param0, MON_DATA_FORM, NULL);
    v4 = Pokemon_LoadLevelUpMoveIdsOf(v3, v6, v7->unk_30);

    for (v0 = 0; v0 < 4; v0++) {
        v7->unk_00[v0] = BoxPokemon_GetValue(param1, 54 + v0, NULL);
        v7->unk_20[v0] = BoxPokemon_GetValue(param2, 54 + v0, NULL);
    }

    v5 = ov5_021E68D8(param0, v7->unk_94);

    for (v0 = 0; v0 < 4; v0++) {
        if (v7->unk_00[v0] != 0) {
            for (v1 = 0; v1 < v5; v1++) {
                if (v7->unk_00[v0] == v7->unk_94[v1]) {
                    if (Pokemon_AddMove(param0, v7->unk_00[v0]) == 0xffff) {
                        Pokemon_ReplaceMove(param0, v7->unk_00[v0]);
                    }
                    break;
                }
            }
        } else {
            break;
        }
    }

    for (v0 = 0; v0 < 4; v0++) {
        if (v7->unk_00[v0] != 0) {
            for (v1 = 0; v1 < 100; v1++) {
                if (v7->unk_00[v0] == Item_MoveForTMHM(328 + v1)) {
                    if (CanPokemonFormLearnTM(v3, v6, v1)) {
                        if (Pokemon_AddMove(param0, v7->unk_00[v0]) == 0xffff) {
                            Pokemon_ReplaceMove(param0, v7->unk_00[v0]);
                        }
                    }
                }
            }
        }
    }

    for (v0 = 0; v0 < 4; v0++) {
        if (v7->unk_00[v0] == 0) {
            break;
        }

        for (v1 = 0; v1 < 4; v1++) {
            if ((v7->unk_00[v0] == v7->unk_20[v1]) && (v7->unk_00[v0] != 0)) {
                v7->unk_10[v2++] = v7->unk_00[v0];
            }
        }
    }

    for (v0 = 0; v0 < 4; v0++) {
        if (v7->unk_10[v0] == 0) {
            break;
        }

        for (v1 = 0; v1 < v4; v1++) {
            if (v7->unk_30[v1] != 0) {
                if (v7->unk_10[v0] == v7->unk_30[v1]) {
                    if (Pokemon_AddMove(param0, v7->unk_10[v0]) == 0xffff) {
                        Pokemon_ReplaceMove(param0, v7->unk_10[v0]);
                    }
                    break;
                }
            }
        }
    }

    Heap_FreeToHeap(v7);
}

void ov5_021E6B40 (UnkStruct_02026310 * param0)
{
    sub_02026270(param0, 0);
    sub_02026278(param0, 0);
}

static const u16 Unk_ov5_021F9F6C[9][3] = {
    {0x168, 0xFF, 0xCA},
    {0x12A, 0xFE, 0xB7},
    {0x1B7, 0x13A, 0x7A},
    {0x1B6, 0x13B, 0xB9},
    {0x1BE, 0x13C, 0x8F},
    {0x1CA, 0x13D, 0xE2},
    {0x196, 0x13E, 0x13B},
    {0x1B8, 0x13F, 0x71},
    {0x1B1, 0x140, 0x166}
};

static u16 ov5_021E6B54 (u16 param0, UnkStruct_02026310 * param1)
{
    u16 v0, v1, v2, v3;
    BoxPokemon * v4[2];

    ov5_021E6668(param1, v4);

    for (v3 = 0; v3 < 9; v3++) {
        if (param0 == Unk_ov5_021F9F6C[v3][0]) {
            v2 = v3;
            break;
        }
    }

    if (v3 == 9) {
        return param0;
    }

    v0 = BoxPokemon_GetValue(v4[0], MON_DATA_HELD_ITEM, NULL);
    v1 = BoxPokemon_GetValue(v4[1], MON_DATA_HELD_ITEM, NULL);

    if ((v0 != Unk_ov5_021F9F6C[v2][1]) && (v1 != Unk_ov5_021F9F6C[v2][1])) {
        param0 = Unk_ov5_021F9F6C[v2][2];
    }

    return param0;
}

static void ov5_021E6BD0 (Pokemon * param0, UnkStruct_02026310 * param1)
{
    int v0, v1;
    BoxPokemon * v2[2];

    ov5_021E6668(param1, v2);

    v0 = BoxPokemon_GetValue(v2[0], MON_DATA_HELD_ITEM, NULL);
    v1 = BoxPokemon_GetValue(v2[1], MON_DATA_HELD_ITEM, NULL);

    if ((v0 == 236) || (v1 == 236)) {
        if (Pokemon_AddMove(param0, 344) == 0xffff) {
            Pokemon_ReplaceMove(param0, 344);
        }
    }
}

static u16 ov5_021E6C20 (UnkStruct_02026310 * param0, u8 param1[])
{
    u16 v0[2], v1, v2, v3, v4, v5;
    BoxPokemon * v6[2];
	int mateLevel[2];

    ov5_021E6668(param0, v6);

    v2 = 0;

    for (v1 = 0; v1 < 2; v1++) {
        if ((v0[v1] = BoxPokemon_GetValue(v6[v1], MON_DATA_SPECIES, NULL)) == 132) {
            param1[0] = v1 ^ 1;
            param1[1] = v1;
        } else if (BoxPokemon_GetGender(v6[v1]) == 1) {
            param1[0] = v1;
            param1[1] = v1 ^ 1;
        }
    }

    v3 = v0[param1[0]];
    v4 = sub_02076F84(v3);

    if (v4 == 29) {
        if (sub_02026248(param0) & 0x8000) {
            v4 = 32;
        } else {
            v4 = 29;
        }
    }

    if (v4 == 314) {
        if (sub_02026248(param0) & 0x8000) {
            v4 = 313;
        } else {
            v4 = 314;
        }
    }

    if (v4 == 490) {
        v4 = 489;
    }

    if ((v0[param1[1]] == 132) && (BoxPokemon_GetGender(v6[param1[0]]) != 1)) {
        v5 = param1[1];
        param1[1] = param1[0];
        param1[0] = v5;
    }

	if (BoxPokemon_GetGender(v6[0]) == BoxPokemon_GetGender(v6[1]))
	{
        mateLevel[0] = BoxPokemon_GetLevel(v6[0]);
		mateLevel[1] = BoxPokemon_GetLevel(v6[1]);
		
		if (mateLevel[0] < mateLevel[1])
		{
			v4 = BoxPokemon_GetValue(v6[0], MON_DATA_SPECIES, NULL);
		}
		else
		{
			v4 = BoxPokemon_GetValue(v6[1], MON_DATA_SPECIES, NULL);
		}
		
		if (mateLevel[0] == mateLevel[1])
		{
			if (LCRNG_Next() >= (0xffff / 2))
			{
				v4 = BoxPokemon_GetValue(v6[0], MON_DATA_SPECIES, NULL);
			}
			else
			{
				v4 = BoxPokemon_GetValue(v6[1], MON_DATA_SPECIES, NULL);
			}
		}
    }

    return v4;
}

void ov5_021E6CF0 (Pokemon * param0, u16 param1, u8 param2, TrainerInfo * param3, int param4, int param5)
{
    u8 v0, v1, v2;
    u16 v3;
    u8 v4 = PokemonPersonalData_GetSpeciesValue(param1, 19);
    Strbuf* v5;

    Pokemon_InitWith(param0, param1, 1, 32, 0, 0, 0, 0);

    v0 = 0;
    v3 = 4;

    Pokemon_SetValue(param0, 155, &v3);
    Pokemon_SetValue(param0, 9, &v4);
    Pokemon_SetValue(param0, 156, &v0);

    if (param2) {
        Pokemon_SetValue(param0, 152, &param2);
    }

    v2 = 1;
    Pokemon_SetValue(param0, 76, &v2);

    v5 = MessageUtil_SpeciesName(SPECIES_EGG, 4);
    Pokemon_SetValue(param0, 119, v5);
    Strbuf_Free(v5);

    if (param4 == 4) {
        u32 v6 = TrainerInfo_ID(param3);
        u32 v7 = TrainerInfo_Gender(param3);
        Strbuf* v8 = TrainerInfo_NameNewStrbuf(param3, 32);

        Pokemon_SetValue(param0, 145, v8);
        Pokemon_SetValue(param0, 7, &v6);
        Pokemon_SetValue(param0, 157, &v7);
        Strbuf_Free(v8);
    }

    sub_0209304C(param0, param3, param4, param5, 0);
}

void ov5_021E6DE8 (Pokemon * param0, u16 param1, UnkStruct_02026310 * param2, u32 param3, u8 param4)
{
    u8 v0;
    u16 v1;
    u32 v2;
    Strbuf* v3;
    u8 v4 = PokemonPersonalData_GetSpeciesValue(param1, 19);

    v2 = sub_02026248(param2);

    if (sub_02026280(param2)) {
        int v5;

        if (Pokemon_IsPersonalityShiny(param3, v2) == 0) {
            for (v5 = 0; v5 < 4; v5++) {
                v2 = ARNG_Next(v2);

                if (Pokemon_IsPersonalityShiny(param3, v2)) {
                    break;
                }
            }
        } else {
            (void)0;
        }
    }

    Pokemon_InitWith(param0, param1, 1, 32, 1, v2, 0, 0);

    v0 = 0;
    v1 = 4;

    Pokemon_SetValue(param0, 155, &v1);
    Pokemon_SetValue(param0, 9, &v4);
    Pokemon_SetValue(param0, 156, &v0);
    Pokemon_SetValue(param0, 112, &param4);

    v3 = MessageUtil_SpeciesName(SPECIES_EGG, 4);

    Pokemon_SetValue(param0, 119, v3);
    Strbuf_Free(v3);
}

void ov5_021E6EA8 (UnkStruct_02026310 * param0, Party * param1, TrainerInfo * param2)
{
    u16 v0;
    u8 v1[2], v2;
    Pokemon * v3 = Pokemon_New(4);

    v0 = ov5_021E6C20(param0, v1);
    v0 = ov5_021E6B54(v0, param0);

    {
        u32 v4 = TrainerInfo_ID(param2);
        BoxPokemon * v5 = ov5_021E622C(param0, v1[0]);
        u8 v6 = BoxPokemon_GetValue(v5, MON_DATA_FORM, NULL);

        ov5_021E6DE8(v3, v0, param0, v4, v6);
    }

    ov5_021E67B0(v3, param0);
    ov5_021E6948(v3, ov5_021E622C(param0, v1[1]), ov5_021E622C(param0, v1[0]));

    sub_0209304C(v3, param2, 3, sub_02017070(1, 0), 4);

    if (v0 == 172) {
        ov5_021E6BD0(v3, param0);
    }

    v2 = 1;
    Pokemon_SetValue(v3, 76, &v2);

    Party_AddPokemon(param1, v3);
    ov5_021E6B40(param0);
    Heap_FreeToHeap(v3);
}

static int ov5_021E6F6C (Party * param0)
{
    u8 v0;
    u8 v1;
    int v2;

    v2 = Party_GetCurrentCount(param0);

    for (v0 = 0; v0 < v2; v0++) {
        if (Pokemon_GetValue(Party_GetPokemonBySlotIndex(param0, v0), MON_DATA_EGG_EXISTS, NULL) == 0) {
            v1 = Pokemon_GetValue(Party_GetPokemonBySlotIndex(param0, v0), MON_DATA_ABILITY, NULL);

            if ((v1 == 40) || (v1 == 49)) {
                return 2;
            }
        }
    }

    return 1;
}

static u8 ov5_021E6FC0 (u16 * param0, u16 * param1)
{
    int v0, v1;

    for (v0 = 0; v0 < 2; v0++) {
        for (v1 = 0; v1 < 2; v1++) {
            if (param0[v0] == param1[v1]) {
                return 1;
            }
        }
    }

    return 0;
}

static u8 ov5_021E6FF0 (BoxPokemon ** param0)
{
    u16 v0[2][2], v1[2];
    u32 v2[2], v3[2], v4, v5;

    for (v5 = 0; v5 < 2; v5++) {
        v1[v5] = BoxPokemon_GetValue(param0[v5], MON_DATA_SPECIES, NULL);
        v2[v5] = BoxPokemon_GetValue(param0[v5], MON_DATA_OT_ID, NULL);
        v4 = BoxPokemon_GetValue(param0[v5], MON_DATA_PERSONALITY, NULL);
        v3[v5] = Pokemon_GetGenderOf(v1[v5], v4);
        v0[v5][0] = PokemonPersonalData_GetSpeciesValue(v1[v5], 22);
        v0[v5][1] = PokemonPersonalData_GetSpeciesValue(v1[v5], 23);

        if (BoxPokemon_GetValue(param0[v5], MON_DATA_HELD_ITEM, NULL) == 217) {
            return 0;
        }
    }

    if ((v0[0][0] == 15) || (v0[1][0] == 15)) {
        return 0;
    }

    if ((v0[0][0] == 13) && (v0[1][0] == 13)) {
        return 0;
    }

    if ((v0[0][0] == 13) || (v0[1][0] == 13)) {
        if (v2[0] == v2[1]) {
            return 20;
        } else {
            return 50;
        }
    }

    if (v3[0] == v3[1]) {
        return 20;
    }

    if ((v3[0] == 2) || (v3[1] == 2)) {
        return 0;
    }

    if (ov5_021E6FC0(v0[0], v0[1]) == 0) {
        return 0;
    }

    if (v1[0] == v1[1]) {
        if (v2[0] != v2[1]) {
            return 70;
        } else {
            return 50;
        }
    } else {
        if (v2[0] != v2[1]) {
            return 50;
        } else {
            return 20;
        }
    }

    return 0;
}

static u8 ov5_021E70FC (UnkStruct_02026310 * param0)
{
    BoxPokemon * v0[2];

    ov5_021E6668(param0, v0);
    return ov5_021E6FF0(v0);
}

static const u16 Unk_ov5_021F9F54[] = {
	101,    // New Years's Day
    106,    // Three Kings' Day
	112,	// Junichi Masuda's Birthday
	214,	// Valentine's Day
    227,    // Green Release
	303,	// Hinamatsuri
	401,	// School Entrance Exam Day
	405,    // Easter 2026
    414,    // Lyss Birthday
    430,    // Maisy's Birthday
	501,	// May Day
	509,    // Goku & Piccolo Day
	611,	// Pokemon Daisuki Club?
	707,	// Qixi  (Tanabata)
    807,    // Ellie's birthday
	813,	// 2009 World Championship
	907,	// American Anime Debut
	928,	// DP Anime Debut (and Red and Blue Release)
    1009,   // Yellow Release
    1031,   // Halloween
	1121,	// Ruby Sapphire Release (and Gold and Silver Relase)
	1214,	// Crystal Release
    1224,   // Christmas Eve
    1225,   // Christmas
	1231,   // New Year's Eve
};

static int ov5_021E7110 (FieldSystem * fieldSystem)
{
    int v0 = sub_02055BB8(fieldSystem) * 100 + sub_02055BC4(fieldSystem);
    int v1;

    if (sub_02055C40(fieldSystem)) {
        return 64;
    }

    for (v1 = 0; v1 < NELEMS(Unk_ov5_021F9F54); v1++) {
        if (Unk_ov5_021F9F54[v1] == v0) {
            return 32;
        }
    }

    return 64;
}

BOOL ov5_021E7154 (UnkStruct_02026310 * param0, Party * param1, FieldSystem * fieldSystem)
{
    u32 v0, v1, v2, v3, v4;
    u32 v5 = 0, v6;
    int v7;
    BoxPokemon * v8[2];

    ov5_021E6668(param0, v8);

    v2 = 0;

    for (v0 = 0; v0 < 2; v0++) {
        if (BoxPokemon_GetValue(v8[v0], MON_DATA_SPECIES_EXISTS, NULL) != 0) {
            sub_02026260(sub_02026218(param0, v0), 1);
            v2++;
        }
    }

    if ((sub_02026234(param0) == 0) && (v2 == 2)) {
        if ((sub_02026228(sub_02026218(param0, 1)) & 0xff) == 0xff) {
            v3 = ov5_021E70FC(param0);
            v4 = LCRNG_Next();
            v4 = (v4 * 100) / 0xffff;

            if (v3 > v4) {
                ov5_021E6720(param0);
            }
        }
    }

    v6 = sub_02026250(param0);
    sub_02026278(param0, ++v6);

    if (v6 == ov5_021E7110(fieldSystem)) {
        sub_02026278(param0, 0);
        v7 = ov5_021E6F6C(param1);

        for (v0 = 0; v0 < Party_GetCurrentCount(param1); v0++) {
            Pokemon * v9 = Party_GetPokemonBySlotIndex(param1, v0);

            if (Pokemon_GetValue(v9, MON_DATA_IS_EGG, NULL)) {
                if (Pokemon_GetValue(v9, MON_DATA_IS_DATA_INVALID, NULL)) {
                    continue;
                }

                v1 = Pokemon_GetValue(v9, MON_DATA_FRIENDSHIP, NULL);

                if (v1 != 0) {
                    if (v1 >= v7) {
                        v1 -= v7;
                    } else {
                        v1--;
                    }

                    Pokemon_SetValue(v9, 9, (u8 *)&v1);
                } else {
                    return 1;
                }
            }
        }
    }

    return 0;
}

Pokemon * ov5_021E7278 (Party * param0)
{
    int v0;
    Pokemon * v1;
    int v2 = Party_GetCurrentCount(param0);

    for (v0 = 0; v0 < v2; v0++) {
        v1 = Party_GetPokemonBySlotIndex(param0, v0);

        if (Pokemon_GetValue(v1, MON_DATA_IS_EGG, NULL)
            && (Pokemon_GetValue(v1, MON_DATA_FRIENDSHIP, NULL) == 0)) {
            return v1;
        }
    }

    return NULL;
}

void ov5_021E72BC (UnkStruct_02026310 * param0, StringTemplate * param1)
{
    BoxPokemon * v0[2];
    u16 v1[10 + 1];

    ov5_021E6668(param0, v0);

    if (BoxPokemon_GetValue(v0[0], MON_DATA_SPECIES, NULL) != 0) {
        StringTemplate_SetNickname(param1, 0, v0[0]);
        StringTemplate_SetOTName(param1, 2, v0[0]);
    }

    if (BoxPokemon_GetValue(v0[1], MON_DATA_SPECIES, NULL) != 0) {
        StringTemplate_SetNickname(param1, 1, v0[1]);
    }
}

void ov5_021E7308 (UnkStruct_02026310 * param0, u32 param1, u32 param2, u32 param3, u8 param4, StringTemplate * param5)
{
    UnkStruct_02026218 * v0;
    BoxPokemon * v1;
    u8 v2, v3;
    u16 v4;

    v0 = sub_02026218(param0, param4);
    v1 = ov5_021E622C(param0, param4);

    StringTemplate_SetNickname(param5, param1, v1);

    v2 = ov5_021E6520(v1, sub_02026228(v0));
    StringTemplate_SetNumber(param5, param2, v2, 3, 0, 1);
    v3 = BoxPokemon_GetValue(v1, MON_DATA_GENDER, NULL);

    if (v3 != 2) {
        v4 = BoxPokemon_GetValue(v1, MON_DATA_SPECIES, NULL);

        if (((v4 == 29) || (v4 == 32)) && (BoxPokemon_GetValue(v1, MON_DATA_HAS_NICKNAME, NULL) == 0)) {
            v3 = 2;
        }
    }

    StringTemplate_SetGenderMarker(param5, param3, v3);
}

u16 ov5_021E73A0 (Party * param0, int param1, StringTemplate * param2)
{
    Pokemon * v0 = Party_GetPokemonBySlotIndex(param0, param1);

    StringTemplate_SetNickname(param2, 0, Pokemon_GetBoxPokemon(v0));
    return Pokemon_GetValue(v0, MON_DATA_SPECIES, NULL);
}

u8 ov5_021E73C8 (UnkStruct_02026310 * param0)
{
    u8 v0;

    if (sub_02026234(param0)) {
        return 1;
    }

    if ((v0 = ov5_021E6238(param0))) {
        return v0 + 1;
    }

    return 0;
}

u8 ov5_021E73F0 (u32 param0)
{
    switch (param0) {
    case 0:
        return 3;
    case 20:
        return 2;
    case 50:
        return 1;
    case 70:
        return 0;
    }

    return 0;
}

extern u32 ov5_021E7420 (UnkStruct_02026310 * param0)
{
    u8 v0, v1;

    v0 = ov5_021E70FC(param0);
    v1 = ov5_021E73F0(v0);

    return v1;
}

static void ov5_021E742C (Pokemon * param0, int param1)
{
    u16 v0;
    u16 v1[4];
    u8 v2[4];
    u32 v3, v4;
    u8 v5[6], v6;
    u8 v7, v8, v9, v10, v11, v12, v13, v14;
    Strbuf* v15 = Strbuf_Init(7 + 1, param1);
    Pokemon * v16 = Pokemon_New(param1);

    v0 = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);

    for (v7 = 0; v7 < 4; v7++) {
        v1[v7] = Pokemon_GetValue(param0, MON_DATA_MOVE1 + v7, NULL);
        v2[v7] = Pokemon_GetValue(param0, MON_DATA_MOVE1_CUR_PP + v7, NULL);
    }

    v3 = Pokemon_GetValue(param0, MON_DATA_PERSONALITY, NULL);

    for (v7 = 0; v7 < 6; v7++) {
        v5[v7] = Pokemon_GetValue(param0, MON_DATA_HP_IV + v7, NULL);
    }

    v8 = Pokemon_GetValue(param0, MON_DATA_LANGUAGE, NULL);
    v9 = Pokemon_GetValue(param0, MON_DATA_MET_GAME, NULL);
    v10 = Pokemon_GetValue(param0, MON_DATA_MARKS, NULL);
    v6 = Pokemon_GetValue(param0, MON_DATA_POKERUS, NULL);
    v12 = Pokemon_GetValue(param0, MON_DATA_FATEFUL_ENCOUNTER, NULL);

    Pokemon_GetValue(param0, MON_DATA_OTNAME_STRBUF, v15);

    v14 = Pokemon_GetValue(param0, MON_DATA_OT_GENDER, NULL);
    v4 = Pokemon_GetValue(param0, MON_DATA_OT_ID, NULL);
    v13 = Pokemon_GetValue(param0, MON_DATA_FORM, NULL);

    if (v0 == 490) {
        int v17 = Pokemon_GetValue(param0, MON_DATA_MET_LOCATION, NULL);

        if (v17 == sub_02017070(2, 1)) {
            while (Pokemon_IsPersonalityShiny(v4, v3)) {
                v3 = ARNG_Next(v3);
            }
        }
    }

    Pokemon_InitWith(v16, v0, 1, 32, 1, v3, 0, 0);

    for (v7 = 0; v7 < 4; v7++) {
        Pokemon_SetValue(v16, 54 + v7, &(v1[v7]));
        Pokemon_SetValue(v16, 58 + v7, &(v2[v7]));
    }

    for (v7 = 0; v7 < 6; v7++) {
        Pokemon_SetValue(v16, 70 + v7, &(v5[v7]));
    }

    Pokemon_SetValue(v16, 12, &v8);
    Pokemon_SetValue(v16, 122, &v9);
    Pokemon_SetValue(v16, 11, &v10);

    v11 = 120;

    Pokemon_SetValue(v16, 9, &v11);
    Pokemon_SetValue(v16, 154, &v6);
    Pokemon_SetValue(v16, 110, &v12);
    Pokemon_SetValue(v16, 145, v15);
    Pokemon_SetValue(v16, 157, &v14);
    Pokemon_SetValue(v16, 7, &v4);
    Pokemon_SetValue(v16, 112, &v13);

    {
        u16 v18;
        u8 v19, v20, v21;

        v18 = Pokemon_GetValue(param0, MON_DATA_MET_LOCATION, NULL);
        v19 = Pokemon_GetValue(param0, MON_DATA_MET_YEAR, NULL);
        v20 = Pokemon_GetValue(param0, MON_DATA_MET_MONTH, NULL);
        v21 = Pokemon_GetValue(param0, MON_DATA_MET_DAY, NULL);

        Pokemon_SetValue(v16, 152, &v18);
        Pokemon_SetValue(v16, 146, &v19);
        Pokemon_SetValue(v16, 147, &v20);
        Pokemon_SetValue(v16, 148, &v21);

        v18 = Pokemon_GetValue(param0, MON_DATA_HATCH_LOCATION, NULL);
        v19 = Pokemon_GetValue(param0, MON_DATA_HATCH_YEAR, NULL);
        v20 = Pokemon_GetValue(param0, MON_DATA_HATCH_MONTH, NULL);
        v21 = Pokemon_GetValue(param0, MON_DATA_HATCH_DAY, NULL);

        Pokemon_SetValue(v16, 153, &v18);
        Pokemon_SetValue(v16, 149, &v19);
        Pokemon_SetValue(v16, 150, &v20);
        Pokemon_SetValue(v16, 151, &v21);
    }

    Pokemon_Copy(v16, param0);
    Strbuf_Free(v15);
    Heap_FreeToHeap(v16);
}

void ov5_021E771C (Pokemon * param0, int param1)
{
    u8 v0, v1;
    u8 v2, v3;
    u16 v4;
    u16 v5[11];

    v0 = 70;
    v1 = 0;
    v2 = 4;
    v3 = 0;

    ov5_021E742C(param0, param1);
    Pokemon_SetValue(param0, 76, &v0);

    v4 = Pokemon_GetValue(param0, MON_DATA_SPECIES, NULL);

    MessageLoader_GetSpeciesName(v4, 0, v5);
    Pokemon_SetValue(param0, 117, v5);
    Pokemon_SetValue(param0, 77, &v1);
    Pokemon_SetValue(param0, 155, &v2);
    Pokemon_SetValue(param0, 156, &v3);
    Pokemon_CalcLevelAndStats(param0);
}

u32 ov5_021E7790 (BoxPokemon ** param0)
{
    return ov5_021E73F0(ov5_021E6FF0(param0));
}
