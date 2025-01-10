#pragma once
#include "custom.h"
#include "style.h"

namespace Config
{

#define NameToStr(n) str(std::string(#n))

// Language
extern std::string locale;

// Features
extern bool enableHoldSprint;
extern bool enableHoldSneak;
extern bool enableSprintAttack;
extern uint32_t enableSheatheAttack;
extern bool enableReverseHorseAttack;

// Vanilla Input
extern uint32_t normalAttack;
extern uint32_t powerAttack;
extern uint32_t otherAttack;
extern uint32_t block;

extern uint32_t altTweenMenu;
extern uint32_t altTogglePOV;

// BFCO
extern uint32_t BFCO_SpecialAttackModifier;
extern uint32_t BFCO_ComboAttack;

// Expand Input
extern uint32_t warAshModifier;
extern uint32_t warAsh;

extern uint32_t zoomModifier;
extern uint32_t altZoomIn;
extern uint32_t altZoomOut;

// Modifier Input
extern std::unordered_map<uint32_t, uint32_t> needModifier;

char *str(std::string ss);
void loadInI();
void saveInI();
} // namespace Config