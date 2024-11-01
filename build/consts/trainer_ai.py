# THIS FILE WAS GENERATED WITH CONSTGEN; DO NOT MANUALLY MODIFY IT
# CONSTANTS ORIGIN FILE: ../consts/trainer_ai.json

import enum

class LoadTypeTarget(enum.Enum):
    LOAD_DEFENDER_TYPE_1 = 0
    LOAD_ATTACKER_TYPE_1 = 1
    LOAD_DEFENDER_TYPE_2 = 2
    LOAD_ATTACKER_TYPE_2 = 3
    LOAD_MOVE_TYPE = 4
    LOAD_DEFENDER_PARTNER_TYPE_1 = 5
    LOAD_ATTACKER_PARTNER_TYPE_1 = 6
    LOAD_DEFENDER_PARTNER_TYPE_2 = 7
    LOAD_ATTACKER_PARTNER_TYPE_2 = 8

class AIWeather(enum.Enum):
    AI_WEATHER_CLEAR = 0
    AI_WEATHER_SUNNY = 1
    AI_WEATHER_RAINING = 2
    AI_WEATHER_SANDSTORM = 3
    AI_WEATHER_HAILING = 4
    AI_WEATHER_DEEP_FOG = 5

class CheckEffect(enum.Enum):
    CHECK_DISABLE = 0
    CHECK_ENCORE = 1

class CheckLevel(enum.Enum):
    CHECK_HIGHER_THAN_TARGET = 0
    CHECK_LOWER_THAN_TARGET = 1
    CHECK_EQUAL_TO_TARGET = 2

class AIFlag(enum.IntFlag):
    AI_FLAG_NONE = 0
    AI_FLAG_BASIC = enum.auto()
    AI_FLAG_EVAL_ATTACK = enum.auto()
    AI_FLAG_EXPERT = enum.auto()
    AI_FLAG_SETUP_FIRST_TURN = enum.auto()
    AI_FLAG_RISKY = enum.auto()
    AI_FLAG_DAMAGE_PRIORITY = enum.auto()
    AI_FLAG_BATON_PASS = enum.auto()
    AI_FLAG_TAG_STRATEGY = enum.auto()
    AI_FLAG_CHECK_HP = enum.auto()
    AI_FLAG_WEATHER = enum.auto()
    AI_FLAG_HARRASSMENT = enum.auto()
    AI_FLAG_UNUSED_11 = enum.auto()
    AI_FLAG_UNUSED_12 = enum.auto()
    AI_FLAG_UNUSED_13 = enum.auto()
    AI_FLAG_UNUSED_14 = enum.auto()
    AI_FLAG_UNUSED_15 = enum.auto()
    AI_FLAG_UNUSED_16 = enum.auto()
    AI_FLAG_UNUSED_17 = enum.auto()
    AI_FLAG_UNUSED_18 = enum.auto()
    AI_FLAG_UNUSED_19 = enum.auto()
    AI_FLAG_UNUSED_20 = enum.auto()
    AI_FLAG_UNUSED_21 = enum.auto()
    AI_FLAG_UNUSED_22 = enum.auto()
    AI_FLAG_UNUSED_23 = enum.auto()
    AI_FLAG_UNUSED_24 = enum.auto()
    AI_FLAG_UNUSED_25 = enum.auto()
    AI_FLAG_JOKE = enum.auto()
    AI_FLAG_PRESCIENT = enum.auto()
    AI_FLAG_OMNISCIENT = enum.auto()
    AI_FLAG_ROAMING_POKEMON = enum.auto()
    AI_FLAG_SAFARI = enum.auto()
    AI_FLAG_CATCH_TUTORIAL = enum.auto()


