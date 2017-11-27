#pragma once

#define GAME_VERSION 3.0
#define MAX_ROUND 200

#define NB_PLAYERS 3

#define SPAWN_WRECK  false
#define LOOTER_COUNT 3
#define REAPER_SKILL_ACTIVE false
#define DESTROYER_SKILL_ACTIVE true
#define DOOF_SKILL_ACTIVE false

#define MAP_RADIUS 6000.0
#define TANKERS_BY_PLAYER_MIN 1.0
#define TANKERS_BY_PLAYER_MAX 3.0

#define WATERTOWN_RADIUS 3000.0

#define TANKER_THRUST 500.0
#define TANKER_EMPTY_MASS 2.5
#define TANKER_MASS_BY_WATER 0.5
#define TANKER_FRICTION 0.40
#define TANKER_RADIUS_BASE 400.0
#define TANKER_RADIUS_BY_SIZE 50.0
#define TANKER_EMPTY_WATER 1.0
#define TANKER_MIN_SIZE 4.0
#define TANKER_MAX_SIZE 10.0
#define TANKER_MIN_RADIUS (TANKER_RADIUS_BASE + TANKER_RADIUS_BY_SIZE * TANKER_MIN_SIZE)
#define TANKER_MAX_RADIUS (TANKER_RADIUS_BASE + TANKER_RADIUS_BY_SIZE * TANKER_MAX_SIZE)
#define TANKER_SPAWN_RADIUS 8000.0
#define TANKER_START_THRUST 2000.0

#define MAX_THRUST 300.0
#define MAX_RAGE 300
#define WIN_SCORE 50

#define REAPER_MASS 0.5
#define REAPER_FRICTION 0.20
#define REAPER_SKILL_DURATION 3.0
#define REAPER_SKILL_COST 30.0
#define REAPER_SKILL_ORDER 0.0
#define REAPER_SKILL_RANGE 2000.0
#define REAPER_SKILL_RADIUS 1000.0
#define REAPER_SKILL_MASS_BONUS 10.0

#define DESTROYER_MASS 1.5
#define DESTROYER_FRICTION 0.30
#define DESTROYER_SKILL_DURATION 1.0
#define DESTROYER_SKILL_COST 60.0
#define DESTROYER_SKILL_ORDER 2.0
#define DESTROYER_SKILL_RANGE 2000.0
#define DESTROYER_SKILL_RADIUS 1000.0
#define DESTROYER_NITRO_GRENADE_POWER 1000.0

#define DOOF_MASS 1.0
#define DOOF_FRICTION 0.25
#define DOOF_RAGE_COEF (1.0 / 100.0)
#define DOOF_SKILL_DURATION 3
#define DOOF_SKILL_COST 30.0
#define DOOF_SKILL_ORDER 1.0
#define DOOF_SKILL_RANGE 2000.0
#define DOOF_SKILL_RADIUS 1000.0

#define LOOTER_RADIUS 400.0
#define LOOTER_REAPER 0
#define LOOTER_DESTROYER 1
#define LOOTER_DOOF 2

#define TYPE_TANKER 3
#define TYPE_WRECK 4
#define TYPE_REAPER_SKILL_EFFECT 5
#define TYPE_DOOF_SKILL_EFFECT 6
#define TYPE_DESTROYER_SKILL_EFFECT 7

#define EPSILON 0.00001
#define MIN_IMPULSE 30.0
#define IMPULSE_COEFF 0.5

#define GLOBAL_ID 0