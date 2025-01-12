#pragma once
#include "utils.h"

namespace Style
{
enum AttackType : std::uint8_t
{
    None = 0,
    RightHand,
    LeftHand,
    DualHand,
    VanillaLMB,
    VanillaRMB
};
enum Styles : std::uint8_t
{
    Unknown = 0,
    TwoHand,
    Bow,
    DualSword,
    DualFist,
    DualMagic,
    DualStaff,
    ShieldSword,
    ShieldFist,
    ShieldMagic,
    ShieldStaff,
    TorchSword,
    TorchFist,
    TorchMagic,
    TorchStaff,
    SwordFist,
    SwordMagic,
    SwordStaff,
    FistSword,
    FistMagic,
    FistStaff,
    MagicSword,
    MagicFist,
    MagicStaff,
    StaffSword,
    StaffFist,
    StaffMagic,
};
typedef struct
{
    // Key-Input
    uint32_t normalAttack;
    uint32_t powerAttack;
    uint32_t otherAttack;
    uint32_t block;
    // Pre-Input
    uint32_t preInputTime;
    uint32_t intervalTime;
    // Normal
    AttackType normalAttackType;
    bool repeatNormalAttack;
    bool sheatheNormalAttack;
    // Power
    AttackType powerAttackType;
    bool repeatPowerAttack;
    bool sheathePowerAttack;
    // Other
    AttackType otherAttackType;
    bool sheatheOtherAttack;
    // Alt
    bool isAltTypeEnable;
    bool isUsingHold;
    uint32_t attackTypeModifier;
    AttackType altNormalAttackType;
    AttackType altPowerAttackType;
    AttackType altOtherAttackType;
} CombatStyle;

extern Styles currentStyle;
extern std::unordered_map<Styles, CombatStyle> styleMap;

const char *GetStyleName(Styles style);
bool IsSheild();
bool IsSpellLeft();
bool IsSpellRight();
bool IsStaffLeft();
bool IsStaffRight();
} // namespace Style
