#pragma once
#include "config.h"

namespace Compatibility
{
extern bool BFCO;
extern bool BFCO_PowerCharge;
extern ActionList::Animation BFCO_NormalAttackSpecial;
extern ActionList::Animation BFCO_PowerAttackSpecial;
extern ActionList::Animation BFCO_NormalAttackJump;
extern ActionList::Animation BFCO_PowerAttackJump1H;
extern ActionList::Animation BFCO_PowerAttackJump2H;
extern ActionList::Animation BFCO_SwimSheathe;
extern ActionList::Animation BFCO_NormalAttackSwim;
extern ActionList::Animation BFCO_PowerAttackSwim;
extern ActionList::Animation BFCO_ComboAttack;

extern bool MCO;

extern bool ELDEN;
extern bool CanUseWarAsh;

extern bool normalAttackWin;
extern bool powerAttackWin;

bool CanNormalAttack();
bool CanPowerAttack();
bool CanRecovery();

bool IsWarAsh(RE::TESForm *form);

void init();
} // namespace Compatibility