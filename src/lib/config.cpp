#include "config.h"

namespace Config
{
CSimpleIniA ini;
const static std::string ini_path = "Data/SKSE/Plugins/KeyLogicExpansion.ini";

// Features
bool &enableCustomInput = std::ref(Custom::enableCustomInput);
bool &enableStances = std::ref(Stances::enableStances);
bool enableHoldSprint;
bool enableHoldSneak;
bool enableSprintAttack;
uint32_t enableSheatheAttack;
bool enableReverseHorseAttack;

// Stances
uint32_t &StancesModifier = std::ref(Stances::StancesModfier);
uint32_t &ChangeToLow = std::ref(Stances::ChangeToLow);
uint32_t &ChangeToMid = std::ref(Stances::ChangeToMid);
uint32_t &ChangeToHigh = std::ref(Stances::ChangeToHigh);

// Vanilla Input
uint32_t normalAttack;
uint32_t powerAttack;
uint32_t otherAttack;
uint32_t block;

uint32_t altTweenMenu;
uint32_t altTogglePOV;

// BFCO
uint32_t BFCO_SpecialAttackModifier;
uint32_t BFCO_ComboAttack;

// Expand Input
uint32_t warAsh;
uint32_t warAshModifier;

uint32_t altZoomIn;
uint32_t altZoomOut;
uint32_t zoomModifier;

// Modifier Input
std::unordered_map<uint32_t, uint32_t> needModifier;

static std::string *s = nullptr;
char *str(std::string ss)
{
    if (s)
    {
        delete (s);
        s = new std::string(ss);
    }
    else
        s = new std::string(ss);
    if (s->at(0) >= 'a')
        s->at(0) = std::toupper(s->at(0));
    return s->data();
}

void setVar()
{
    // Features
    enableCustomInput = ini.GetBoolValue("Features", NameToStr(enableCustomInput));
    enableStances = ini.GetBoolValue("Features", NameToStr(enableStances));
    enableHoldSprint = ini.GetBoolValue("Features", NameToStr(enableHoldSprint));
    enableHoldSneak = ini.GetBoolValue("Features", NameToStr(enableHoldSneak));
    enableSprintAttack = ini.GetBoolValue("Features", NameToStr(enableSprintAttack));
    enableReverseHorseAttack = ini.GetBoolValue("Features", NameToStr(enableReverseHorseAttack));
    enableSheatheAttack = ini.GetLongValue("Features", NameToStr(enableSheatheAttack));

    // Stances
    Stances::StancesList.push_back(nullptr);
    Stances::StancesList.push_back(FormUtils::GetForm((ini.GetValue("Stances", "Low"))));
    Stances::StancesList.push_back(FormUtils::GetForm((ini.GetValue("Stances", "Mid"))));
    Stances::StancesList.push_back(FormUtils::GetForm((ini.GetValue("Stances", "High"))));
    Stances::StancesList.push_back(FormUtils::GetForm((ini.GetValue("Stances", "Sheathe"))));
    StancesModifier = ini.GetLongValue("Stances", "StancesModifier");
    ChangeToLow = ini.GetLongValue("Stances", "ChangeToLow");
    ChangeToMid = ini.GetLongValue("Stances", "ChangeToMid");
    ChangeToHigh = ini.GetLongValue("Stances", "ChangeToHigh");

    // Vanilla Input
    normalAttack = ini.GetLongValue("Vanilla", NameToStr(normalAttack));
    powerAttack = ini.GetLongValue("Vanilla", NameToStr(powerAttack));
    otherAttack = ini.GetLongValue("Vanilla", NameToStr(otherAttack));
    block = ini.GetLongValue("Vanilla", NameToStr(block));

    altTweenMenu = ini.GetLongValue("Vanilla", NameToStr(altTweenMenu));
    altTogglePOV = ini.GetLongValue("Vanilla", NameToStr(altTogglePOV));

    // BFCO
    BFCO_SpecialAttackModifier = ini.GetLongValue("BFCO", NameToStr(BFCO_SpecialAttackModifier));
    BFCO_ComboAttack = ini.GetLongValue("BFCO", NameToStr(BFCO_ComboAttack));

    // Expand Input
    warAsh = ini.GetLongValue("Expand", NameToStr(warAsh));
    warAshModifier = ini.GetLongValue("Expand", NameToStr(warAshModifier));

    altZoomIn = ini.GetLongValue("Expand", NameToStr(altZoomIn));
    altZoomOut = ini.GetLongValue("Expand", NameToStr(altZoomOut));
    zoomModifier = ini.GetLongValue("Expand", NameToStr(zoomModifier));

    // Combat Style
    for (int i = Style::Styles::Unknown; i <= Style::Styles::StaffMagic; i++)
    {
        Style::styleMap.insert(std::make_pair(
            (Style::Styles)i,
            Style::CombatStyle{
                // PreInput
                (uint32_t)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "PreInputTime"),
                (uint32_t)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "IntervalTime"),
                // Normal
                (Style::AttackType)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "NormalAttackType"),
                ini.GetBoolValue(Style::GetStyleName((Style::Styles)i), "RepeatNormalAttack"),
                ini.GetBoolValue(Style::GetStyleName((Style::Styles)i), "SheatheNormalAttack"),
                // Power
                (Style::AttackType)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "PowerAttackType"),
                ini.GetBoolValue(Style::GetStyleName((Style::Styles)i), "RepeatPowerAttack"),
                ini.GetBoolValue(Style::GetStyleName((Style::Styles)i), "SheathePowerAttack"),
                // Other
                (Style::AttackType)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "OtherAttackType"),
                ini.GetBoolValue(Style::GetStyleName((Style::Styles)i), "SheatheOtherAttack"),
                // Alt
                ini.GetBoolValue(Style::GetStyleName((Style::Styles)i), "IsAltTypeEnable"),
                ini.GetBoolValue(Style::GetStyleName((Style::Styles)i), "IsUsingHold"),
                (uint32_t)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "AttackTypeModifier"),
                (Style::AttackType)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "AltNormalAttackType"),
                (Style::AttackType)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "AltPowerAttackType"),
                (Style::AttackType)ini.GetLongValue(Style::GetStyleName((Style::Styles)i), "AltOtherAttackType")}));
    }

    // Modifier Input
    std::list<CSimpleIniA::Entry> list;
    ini.GetAllKeys("Modifier", std::ref(list));
    for (auto item : list)
    {
        auto trigger = KeyUtils::GetVanillaKeyMap(item.pItem);
        auto modifier = ini.GetLongValue("Modifier", item.pItem);
        if (item.pItem == VarUtils::userEvent->tweenMenu && altTweenMenu)
            trigger = altTweenMenu;
        else if (item.pItem == VarUtils::userEvent->togglePOV && altTogglePOV)
            trigger = altTogglePOV;
        if (trigger)
            needModifier[trigger] = modifier;
    }
    if (altZoomIn)
        needModifier[altZoomIn] = zoomModifier;
    else
        needModifier[KeyUtils::Mouse::MouseWheelUp] = zoomModifier;
    if (altZoomOut)
        needModifier[altZoomOut] = zoomModifier;
    else
        needModifier[KeyUtils::Mouse::MouseWheelDown] = zoomModifier;

    // Custom Input
    if (enableCustomInput)
        Custom::LoadCustom();

    if (s)
        delete s;
    s = nullptr;
}

void createInI()
{
    // Features
    ini.SetBoolValue("Features", NameToStr(enableCustomInput), false);
    ini.SetBoolValue("Features", NameToStr(enableStances), false);
    ini.SetBoolValue("Features", NameToStr(enableHoldSprint), true);
    ini.SetBoolValue("Features", NameToStr(enableHoldSneak), false);
    ini.SetBoolValue("Features", NameToStr(enableSprintAttack), true);
    ini.SetBoolValue("Features", NameToStr(enableReverseHorseAttack), false);
    ini.SetLongValue("Features", NameToStr(enableSheatheAttack), 0);

    // Stances
    ini.SetValue("Stances", "Low", "Stances - Dynamic Weapon Movesets SE.esp|0x4251A",
                 ";Set this according to your stances mod");
    ini.SetValue("Stances", "Mid", "Stances - Dynamic Weapon Movesets SE.esp|0x42519");
    ini.SetValue("Stances", "High", "Stances - Dynamic Weapon Movesets SE.esp|0x42518");
    ini.SetValue("Stances", "Sheathe", "NULL");
    ini.SetLongValue("Stances", "StancesModifier", KeyUtils::KeyBoard::LeftShift);
    ini.SetLongValue("Stances", "ChangeToLow", KeyUtils::Mouse::MouseWheelDown);
    ini.SetLongValue("Stances", "ChangeToMid", KeyUtils::Mouse::MouseButtonMiddle);
    ini.SetLongValue("Stances", "ChangeToHigh", KeyUtils::Mouse::MouseWheelUp);

    // Vanilla Input
    ini.SetLongValue("Vanilla", NameToStr(normalAttack), KeyUtils::Mouse::MouseButtonLeft);
    ini.SetLongValue("Vanilla", NameToStr(powerAttack), KeyUtils::Mouse::MouseButtonRight);
    ini.SetLongValue("Vanilla", NameToStr(otherAttack), 0);
    ini.SetLongValue("Vanilla", NameToStr(block), KeyUtils::KeyBoard::Tab);

    ini.SetLongValue("Vanilla", NameToStr(altTweenMenu), KeyUtils::KeyBoard::G);
    ini.SetLongValue("Vanilla", NameToStr(altTogglePOV), 0);

    // BFCO
    ini.SetLongValue("BFCO", NameToStr(BFCO_SpecialAttackModifier), KeyUtils::KeyBoard::LeftAlt);
    ini.SetLongValue("BFCO", NameToStr(BFCO_ComboAttack), KeyUtils::KeyBoard::E);

    // Expand Input
    ini.SetLongValue("Expand", NameToStr(warAsh), 0);
    ini.SetLongValue("Expand", NameToStr(warAshModifier), 0);

    ini.SetLongValue("Expand", NameToStr(altZoomIn), 0);
    ini.SetLongValue("Expand", NameToStr(altZoomOut), 0);
    ini.SetLongValue("Expand", NameToStr(zoomModifier), 0);

    // Combat Style
    for (int i = Style::Styles::Unknown; i <= Style::Styles::StaffMagic; i++)
    {
        // PreInput
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "PreInputTime", 100);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "IntervalTime", 30);
        // Normal
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "NormalAttackType", Style::AttackType::RightHand);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "RepeatNormalAttack", true);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "SheatheNormalAttack", false);
        // Power
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "PowerAttackType", Style::AttackType::RightHand);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "RepeatPowerAttack", true);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "SheathePowerAttack", true);
        // Other
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "OtherAttackType", Style::AttackType::None);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "SheatheOtherAttack", false);
        // Alt
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "IsAltTypeEnable", false);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "IsUsingHold", true);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "AttackTypeModifier", KeyUtils::KeyBoard::V);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "AltNormalAttackType", Style::AttackType::VanillaLMB);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "AltPowerAttackType", Style::AttackType::VanillaRMB);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "AltOtherAttackType", Style::AttackType::None);

        switch ((Style::Styles)i)
        {
        case Style::Styles::TwoHand:
        case Style::Styles::Bow:
        case Style::Styles::DualSword:
        case Style::Styles::DualFist:
        case Style::Styles::ShieldSword:
        case Style::Styles::ShieldFist:
        case Style::Styles::TorchSword:
        case Style::Styles::TorchFist:
        case Style::Styles::FistSword:
            break;
        case Style::Styles::MagicSword:
        case Style::Styles::MagicFist:
        case Style::Styles::StaffSword:
        case Style::Styles::StaffFist:
            ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "OtherAttackType", Style::AttackType::VanillaLMB);
            break;
        case Style::Styles::Unknown:
        case Style::Styles::DualMagic:
        case Style::Styles::DualStaff:
        case Style::Styles::ShieldMagic:
        case Style::Styles::ShieldStaff:
        case Style::Styles::TorchMagic:
        case Style::Styles::TorchStaff:
        case Style::Styles::MagicStaff:
        case Style::Styles::StaffMagic:
        case Style::Styles::FistMagic:
        case Style::Styles::FistStaff:
            ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "NormalAttackType", Style::AttackType::VanillaLMB);
            ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "PowerAttackType", Style::AttackType::VanillaRMB);
            break;
        case Style::Styles::SwordFist:
        case Style::Styles::SwordMagic:
        case Style::Styles::SwordStaff:
            ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "NormalAttackType", Style::AttackType::LeftHand);
            ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "PowerAttackType", Style::AttackType::LeftHand);
            ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "OtherAttackType", Style::AttackType::VanillaRMB);
            break;
        }
    }

    // Modifier Input
    ini.SetLongValue("Modifier", VarUtils::userEvent->activate.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->readyWeapon.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->tweenMenu.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->togglePOV.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->shout.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->run.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->toggleRun.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->autoMove.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->favorites.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quicksave.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickload.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->wait.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->journal.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->pause.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickInventory.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickMagic.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickStats.c_str(), 0);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickMap.c_str(), 0);

    // Custom Input

    SI_Error rc = ini.SaveFile(ini_path.c_str());
    if (rc < 0)
        return SKSE::stl::report_and_fail("Cannot load ini.");

    setVar();

    logger::trace("Create ini success.");
}

void loadInI()
{
    ini.SetUnicode();
    if (!std::filesystem::exists(ini_path))
    {
        logger::error("ini file not exist, will generate default.");
        return createInI();
    }
    SI_Error rc = ini.LoadFile(ini_path.c_str());
    if (rc < 0)
        return SKSE::stl::report_and_fail("Cannot load ini.");

    setVar();
}

void saveInI()
{
    ini.SetUnicode();
    SI_Error rc = ini.LoadFile(ini_path.c_str());
    if (rc < 0)
        return SKSE::stl::report_and_fail("Cannot save ini.");

    // Features
    ini.SetBoolValue("Features", NameToStr(enableCustomInput), enableCustomInput);
    ini.SetBoolValue("Features", NameToStr(enableStances), enableStances);
    ini.SetBoolValue("Features", NameToStr(enableHoldSprint), enableHoldSprint);
    ini.SetBoolValue("Features", NameToStr(enableHoldSneak), enableHoldSneak);
    ini.SetBoolValue("Features", NameToStr(enableSprintAttack), enableSprintAttack);
    ini.SetBoolValue("Features", NameToStr(enableReverseHorseAttack), enableReverseHorseAttack);
    ini.SetLongValue("Features", NameToStr(enableSheatheAttack), enableSheatheAttack);

    // Stances
    ini.SetLongValue("Stances", "StancesModifier", StancesModifier);
    ini.SetLongValue("Stances", "ChangeToLow", ChangeToLow);
    ini.SetLongValue("Stances", "ChangeToMid", ChangeToMid);
    ini.SetLongValue("Stances", "ChangeToHigh", ChangeToHigh);

    // Vanilla Input
    ini.SetLongValue("Vanilla", NameToStr(normalAttack), normalAttack);
    ini.SetLongValue("Vanilla", NameToStr(powerAttack), powerAttack);
    ini.SetLongValue("Vanilla", NameToStr(otherAttack), otherAttack);
    ini.SetLongValue("Vanilla", NameToStr(block), block);

    ini.SetLongValue("Vanilla", NameToStr(altTweenMenu), altTweenMenu);
    ini.SetLongValue("Vanilla", NameToStr(altTogglePOV), altTogglePOV);

    // BFCO
    ini.SetLongValue("BFCO", NameToStr(BFCO_SpecialAttackModifier), BFCO_SpecialAttackModifier);
    ini.SetLongValue("BFCO", NameToStr(BFCO_ComboAttack), BFCO_ComboAttack);

    // Expand Input
    ini.SetLongValue("Expand", NameToStr(warAsh), warAsh);
    ini.SetLongValue("Expand", NameToStr(warAshModifier), warAshModifier);

    ini.SetLongValue("Expand", NameToStr(altZoomIn), altZoomIn);
    ini.SetLongValue("Expand", NameToStr(altZoomOut), altZoomOut);
    ini.SetLongValue("Expand", NameToStr(zoomModifier), zoomModifier);

    // Combat Style
    for (int i = Style::Styles::Unknown; i <= Style::Styles::StaffMagic; i++)
    {
        // PreInput
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "PreInputTime",
                         Style::styleMap[(Style::Styles)i].preInputTime);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "IntervalTime",
                         Style::styleMap[(Style::Styles)i].intervalTime);
        // Normal
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "NormalAttackType",
                         Style::styleMap[(Style::Styles)i].normalAttackType);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "RepeatNormalAttack",
                         Style::styleMap[(Style::Styles)i].repeatNormalAttack);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "SheatheNormalAttack",
                         Style::styleMap[(Style::Styles)i].sheatheNormalAttack);
        // Power
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "PowerAttackType",
                         Style::styleMap[(Style::Styles)i].powerAttackType);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "RepeatPowerAttack",
                         Style::styleMap[(Style::Styles)i].repeatPowerAttack);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "SheathePowerAttack",
                         Style::styleMap[(Style::Styles)i].sheathePowerAttack);
        // Other
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "OtherAttackType",
                         Style::styleMap[(Style::Styles)i].otherAttackType);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "SheatheOtherAttack",
                         Style::styleMap[(Style::Styles)i].sheatheOtherAttack);
        // Alt
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "IsAltTypeEnable",
                         Style::styleMap[(Style::Styles)i].isAltTypeEnable);
        ini.SetBoolValue(Style::GetStyleName((Style::Styles)i), "IsUsingHold",
                         Style::styleMap[(Style::Styles)i].isUsingHold);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "AttackTypeModifier",
                         Style::styleMap[(Style::Styles)i].attackTypeModifier);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "AltNormalAttackType",
                         Style::styleMap[(Style::Styles)i].altNormalAttackType);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "AltPowerAttackType",
                         Style::styleMap[(Style::Styles)i].altPowerAttackType);
        ini.SetLongValue(Style::GetStyleName((Style::Styles)i), "AltOtherAttackType",
                         Style::styleMap[(Style::Styles)i].altOtherAttackType);
    }

    // Modifier Input
    ini.SetLongValue("Modifier", VarUtils::userEvent->activate.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->activate)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->readyWeapon.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->tweenMenu.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->tweenMenu)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->togglePOV.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->shout.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->shout)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->run.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->run)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->toggleRun.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->toggleRun)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->autoMove.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->autoMove)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->favorites.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->favorites)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quicksave.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quicksave)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickload.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickload)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->wait.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->wait)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->journal.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->journal)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->pause.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->pause)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickInventory.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickInventory)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickMagic.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickMagic)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickStats.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickStats)]);
    ini.SetLongValue("Modifier", VarUtils::userEvent->quickMap.c_str(),
                     needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickMap)]);

    rc = ini.SaveFile(ini_path.c_str());
    if (rc < 0)
        return SKSE::stl::report_and_fail("Cannot save to ini.");

    Stances::StancesList.clear();
    Style::styleMap.clear();
    needModifier.clear();
    setVar();
}
} // namespace Config