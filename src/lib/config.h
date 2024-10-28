#pragma once
#include "SimpleIni.h"
#include "utils.h"

namespace Config
{

#define NameToStr(n) str(std::string(#n))

enum CustomEvent
{
    None = 0,
    Spell,
    Perk,
    Command
};

typedef struct
{
    uint32_t trigger;
    uint32_t modifier;
    uint32_t priority;
    CustomEvent event;
    std::string content;
} CustomInput;

// Features
extern bool enableStances;
extern bool enableCustomInput;
extern bool enableHoldSprint;
extern bool enableHoldSneak;
extern uint32_t enableSheatheAttack;
extern bool enableReverseHorseAttack;

// Vanilla Input
extern uint32_t normalAttack;
extern uint32_t powerAttack;
extern uint32_t block;

extern uint32_t altTweenMenu;
extern uint32_t altTogglePOV;

// Expand Input
extern uint32_t warAsh;
extern uint32_t warAshModifier;

extern uint32_t altZoomIn;
extern uint32_t altZoomOut;
extern uint32_t zoomModifier;

// Modifier Input
extern std::unordered_map<uint32_t, uint32_t> needModifier;
extern std::unordered_set<uint32_t> listModifier;
bool isModifier(uint32_t code);

// Custom Input
extern std::vector<CustomInput> custom;

char *str(std::string ss);
void loadInI();
} // namespace Config