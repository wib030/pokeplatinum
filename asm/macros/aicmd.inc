    .equ FALSE, 0
    .equ TRUE, 1

    .macro IfRandomLessThan i, jump
    .long 0
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfRandomGreaterThan i, jump
    .long 1
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfRandomEqualTo i, jump
    .long 2
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfRandomNotEqualTo i, jump
    .long 3
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro AddToMoveScore i
    .long 4
    .long \i
    .endm

    .macro IfHPPercentLessThan battler, i, jump
    .long 5
    .long \battler
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHPPercentGreaterThan battler, i, jump
    .long 6
    .long \battler
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHPPercentEqualTo battler, i, jump
    .long 7
    .long \battler
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHPPercentNotEqualTo battler, i, jump
    .long 8
    .long \battler
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatus battler, status, jump
    .long 9
    .long \battler
    .long \status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfNotStatus battler, status, jump
    .long 10
    .long \battler
    .long \status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfVolatileStatus battler, volatile_status, jump
    .long 11
    .long \battler
    .long \volatile_status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfNotVolatileStatus battler, volatile_status, jump
    .long 12
    .long \battler
    .long \volatile_status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveEffect battler, move_effects, jump
    .long 13
    .long \battler
    .long \move_effects
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfNotMoveEffect battler, move_effects, jump
    .long 14
    .long \battler
    .long \move_effects
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfSideCondition battler, side_conditions, jump
    .long 15
    .long \battler
    .long \side_conditions
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfNotSideCondition battler, side_conditions, jump
    .long 16
    .long \battler
    .long \side_conditions
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedLessThan loaded_val, jump
    .long 17
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedGreaterThan loaded_val, jump
    .long 18
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedEqualTo loaded_val, jump
    .long 19
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedNotEqualTo loaded_val, jump
    .long 20
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedMask loaded_val, jump
    .long 21
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedNotMask loaded_val, jump
    .long 22
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveEqualTo move, jump
    .long 23
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveNotEqualTo move, jump
    .long 24
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedinTable table, jump
    .long 25
    .long (\table-.) / 4 - 2
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfLoadedNotinTable table, jump
    .long 26
    .long (\table-.) / 4 - 2
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfAttackerHasDamagingMoves jump
    .long 27
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfAttackerHasNoDamagingMoves jump
    .long 28
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadTurnCount
    .long 29
    .endm

    .macro LoadTypeFrom load_type_target
    .long 30
    .long \load_type_target
    .endm

    .macro LoadMovePower
    .long 31
    .endm

    .macro FlagMoveDamageScore bool
    .long 32
    .long \bool
    .endm

    .macro LoadBattlerPreviousMove battler
    .long 33
    .long \battler
    .endm

    .macro IfTempEqualTo loaded_val, jump
    .long 34
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTempNotEqualTo loaded_val, jump
    .long 35
    .long \loaded_val
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfSpeedCompareEqualTo cmp, jump
    .long 36
    .long \cmp
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfSpeedCompareNotEqualTo cmp, jump
    .long 37
    .long \cmp
    .long (\jump-.) / 4 - 1
    .endm

    .macro CountAlivePartyBattlers battler
    .long 38
    .long \battler
    .endm

    .macro LoadCurrentMove
    .long 39
    .endm

    .macro LoadCurrentMoveEffect
    .long 40
    .endm

    .macro LoadBattlerAbility battler
    .long 41
    .long \battler
    .endm

    .macro CalcMaxEffectiveness
    .long 42
    .endm

    .macro IfMoveEffectivenessEquals modifier, jump
    .long 43
    .long \modifier
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfPartyMemberStatus battler, pokemon_status, jump
    .long 44
    .long \battler
    .long \pokemon_status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfPartyMemberNotStatus battler, pokemon_status, jump
    .long 45
    .long \battler
    .long \pokemon_status
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadCurrentWeather
    .long 46
    .endm

    .macro IfCurrentMoveEffectEqualTo battle_effect, jump
    .long 47
    .long \battle_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCurrentMoveEffectNotEqualTo battle_effect, jump
    .long 48
    .long \battle_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatStageLessThan battler, stat, i, jump
    .long 49
    .long \battler
    .long \stat
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatStageGreaterThan battler, stat, i, jump
    .long 50
    .long \battler
    .long \stat
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfStatStageEqualTo battler, stat, i, jump
    .long 51
    .long \battler
    .long \stat
    .long \i
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCurrentMoveKills roll_damage, jump
    .long 52
    .long \roll_damage
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCurrentMoveDoesNotKill roll_damage, jump
    .long 53
    .long \roll_damage
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveKnown battler, move, jump
    .long 54
    .long \battler
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveNotKnown battler, move, jump
    .long 55
    .long \battler
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveEffectKnown battler, battle_effect, jump
    .long 56
    .long \battler
    .long \battle_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveEffectNotKnown battler, battle_effect, jump
    .long 57
    .long \battler
    .long \battle_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerUnderEffect battler, check_effect, jump
    .long 58
    .long \battler
    .long \check_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCurrentMoveMatchesEffect check_effect, jump
    .long 59
    .long \check_effect
    .long (\jump-.) / 4 - 1
    .endm

    .macro Escape
    .long 60
    .endm

    .macro Dummy3E i
    .long 61
    .long \i
    .endm

    .macro Dummy3F
    .long 62
    .endm

    .macro LoadHeldItem battler
    .long 63
    .long \battler
    .endm

    .macro LoadHeldItemEffect battler
    .long 64
    .long \battler
    .endm

    .macro LoadGender battler
    .long 65
    .long \battler
    .endm

    .macro LoadIsFirstTurnInBattle battler
    .long 66
    .long \battler
    .endm

    .macro LoadStockpileCount battler
    .long 67
    .long \battler
    .endm

    .macro LoadBattleType
    .long 68
    .endm

    .macro LoadRecycleItem battler
    .long 69
    .long \battler
    .endm

    .macro LoadTypeOfLoadedMove
    .long 70
    .endm

    .macro LoadPowerOfLoadedMove
    .long 71
    .endm

    .macro LoadEffectOfLoadedMove
    .long 72
    .endm

    .macro LoadProtectChain battler
    .long 73
    .long \battler
    .endm

    .macro PushAndGoTo jump
    .long 74
    .long (\jump-.) / 4 - 1
    .endm

    .macro GoTo jump
    .long 75
    .long (\jump-.) / 4 - 1
    .endm

    .macro PopOrEnd
    .long 76
    .endm

    .macro IfLevel check_level, jump
    .long 77
    .long \check_level
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTargetIsTaunted jump
    .long 78
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTargetIsNotTaunted jump
    .long 79
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTargetIsPartner jump
    .long 80
    .long (\jump-.) / 4 - 1
    .endm

    .macro FlagBattlerIsType battler, type
    .long 81
    .long \battler
    .long \type
    .endm

    .macro CheckBattlerAbility battler, ability
    .long 82
    .long \battler
    .long \ability
    .endm

    .macro IfActivatedFlashFire battler, jump
    .long 83
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHeldItemEqualTo battler, item, jump
    .long 84
    .long \battler
    .long \item
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfFieldConditionsMask field_conditions, jump
    .long 85
    .long \field_conditions
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadSpikesLayers battler, side_conditions
    .long 86
    .long \battler
    .long \side_conditions
    .endm

    .macro IfAnyPartyMemberIsWounded battler, jump
    .long 87
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfAnyPartyMemberUsedPP battler, jump
    .long 88
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadFlingPower battler
    .long 89
    .long \battler
    .endm

    .macro LoadCurrentMovePP
    .long 90
    .endm

    .macro IfCanUseLastResort battler, jump
    .long 91
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadCurrentMoveClass
    .long 92
    .endm

    .macro LoadDefenderLastUsedMoveClass
    .long 93
    .endm

    .macro LoadBattlerSpeedRank battler
    .long 94
    .long \battler
    .endm

    .macro LoadBattlerTurnCount battler
    .long 95
    .long \battler
    .endm

    .macro IfPartyMemberDealsMoreDamage roll_damage, jump
    .long 96
    .long \roll_damage
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHasSuperEffectiveMove jump
    .long 97
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerDealsMoreDamage battler, roll_damage, jump
    .long 98
    .long \battler
    .long \roll_damage
    .long (\jump-.) / 4 - 1
    .endm

    .macro SumPositiveStatStages battler
    .long 99
    .long \battler
    .endm

    .macro DiffStatStages battler, battle_stat
    .long 100
    .long \battler
    .long \battle_stat
    .endm

    .macro IfBattlerHasHigherStat battler, battle_stat, jump
    .long 101
    .long \battler
    .long \battle_stat
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasLowerStat battler, battle_stat, jump
    .long 102
    .long \battler
    .long \battle_stat
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasEqualStat battler, battle_stat, jump
    .long 103
    .long \battler
    .long \battle_stat
    .long (\jump-.) / 4 - 1
    .endm

    .macro CheckIfHighestDamageWithPartner roll_damage
    .long 104
    .long \roll_damage
    .endm

    .macro IfBattlerFainted battler, jump
    .long 105
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerNotFainted battler, jump
    .long 106
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadAbility battler
    .long 107
    .long \battler
    .endm

    .macro IfBattlerIsPhysicalAttacker battler, jump
    .long 108
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerIsSpecialAttacker battler, jump
    .long 109
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasStatusMove battler, jump
    .long 110
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasNoPhysicalAttack battler, jump
    .long 111
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasNoSpecialAttack battler, jump
    .long 112
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasNoStatusAttack battler, jump
    .long 113
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfToxicSpikesClearerAliveInParty battler, jump
    .long 114
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadWeight battler
    .long 115
    .long \battler
    .endm

    .macro IfWishActive battler, jump
    .long 116
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfPartyMemberHasBattleEffect battler, move, jump
    .long 117
    .long \battler
    .long \move
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfShouldTaunt battler, jump
    .long 118
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadMoveAccuracy
    .long 119
    .endm

    .macro IfSameAbilities battler1, battler2, jump
    .long 120
    .long \battler1
    .long \battler2
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHasBaseAbility battler, jump
    .long 121
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm
	
	.macro IfDestinyBondFails battler, jump
    .long 122
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfEnemyCanChunkOrKO jump
    .long 123
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadBattlerCritStage battler1
    .long 124
    .long \battler1
    .endm

    .macro IfCanHazeOrPhaze battler1, jump
    .long 125
    .long \battler1
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfHasStatusThreat battler1, jump
    .long 126
    .long \battler1
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerDetersBoosting battler1, jump
    .long 127
    .long \battler1
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadSleepTurns battler
    .long 128
    .long \battler
    .endm

    .macro IfCurrentMoveRevealed jump
    .long 129
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCanChunkOrKOEnemy jump
    .long 130
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerDetersContactMove battler1, jump
    .long 131
    .long \battler1
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfTrapped battler, jump
    .long 132
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerStatDropped battler, jump
    .long 133
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm
	
	.macro IfCanKOEnemy jump
    .long 134
    .long (\jump-.) / 4 - 1
    .endm
	
	.macro IfEnemyCanKO jump
    .long 135
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfShouldEncore battler, jump
    .long 136
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm
	
	.macro IfEnemySleepClauseActive jump
    .long 137
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfMoveBlockedBySoundproof jump
    .long 138
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerDetersStatus battler, status, jump
    .long 139
    .long \battler
    .long \status
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfBattlerHasInvulnerableMove battler, jump
    .long 140
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfAbilityInPlay ability, jump
    .long 141
    .long \ability
    .long (\jump-.) / 4 - 1
    .endm

    .macro IfCanBreakSashOrSturdy battler, jump
    .long 142
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadFlingEffect battler
    .long 143
    .long \battler
    .endm

    .macro IfHasSubstituteIncentive battler, jump
    .long 144
    .long \battler
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadMoveClass
    .long 145
    .endm
	
	.macro IfBattlerHasBounceableMove battler1, jump
    .long 146
    .long \battler1
    .long (\jump-.) / 4 - 1
    .endm

    .macro LoadBattlerIgnorableAbility battler
    .long 147
    .long \battler
    .endm

    .equ TABLE_END, 0xFFFFFFFF

    .macro TableEntry entry
    .long \entry
    .endm

    .macro LabelDistance dst, src
    .long (\dst-\src) / 4
    .endm
