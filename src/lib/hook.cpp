#include "hook.h"

namespace Hook
{
using AttackType = Style::AttackType;
typedef struct
{
    ActionList::Animation action;
    uint64_t time;
} ActionQueue;
static ActionQueue queue;
static uint8_t isInQueue = 0;
static uint64_t startTime = 0;

static uint32_t preInputTime = 0;
static uint32_t intervalTime = 0;

static bool vanillaRMB = false;

typedef bool _doAction_t(RE::TESActionData *);
REL::Relocation<_doAction_t> doAction{RELOCATION_ID(40551, 41557)};
void doAnimation(ActionList::Animation action)
{
    startTime = TimeUtils::GetTime();
    switch (action.type)
    {
    case ActionList::ActionType::Idle:
        SKSE::GetTaskInterface()->AddTask([action]() {
            VarUtils::player->GetActorRuntimeData().currentProcess->PlayIdle(VarUtils::player, action.idle, nullptr);
        });
        break;
    case ActionList::ActionType::Action:
        SKSE::GetTaskInterface()->AddTask([action]() {
            std::unique_ptr<TESActionData> data(TESActionData::Create());
            data->source = NiPointer<TESObjectREFR>(VarUtils::player);
            data->action = action.action;
            doAction(data.get());
        });
        break;
    case ActionList::ActionType::AniamtionEvent:
        SKSE::GetTaskInterface()->AddTask([action]() {
            VarUtils::player->AsActorValueOwner()->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage,
                                                                     RE::ActorValue::kStamina, -0.0f);
            VarUtils::player->NotifyAnimationGraph(action.event);
        });
        break;
    }
}
Style::Styles DetectStyle()
{
    uint8_t leftWeapen = 0;
    uint8_t rightWeapen = 0;
    uint8_t leftMagic = 0;
    uint8_t rightMagic = 0;
    int32_t gripMode = 0;
    if (Compatibility::GRIP)
        VarUtils::player->GetGraphVariableInt("iDynamicGripMode", gripMode);
    if (gripMode == 1)
        return Style::Styles::TwoHand;
    else if (gripMode == 3)
        return Style::Styles::DualSword;
    bool shield = false;
    RE::TESForm *lHand = VarUtils::player->GetActorRuntimeData().currentProcess->GetEquippedLeftHand();
    if (lHand)
    {
        if (lHand->formType == RE::FormType::Armor)
            shield = true;
        else if (lHand->formType == RE::FormType::Spell || lHand->formType == RE::FormType::Scroll)
            leftMagic = 1;
        else if (lHand->formType == RE::FormType::Light)
            leftWeapen = 2;
        else if (lHand->formType == RE::FormType::Weapon)
        {
            if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kBow ||
                lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow)
                return Style::Styles::Bow;
            else if ((lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandSword ||
                      lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kTwoHandAxe) &&
                     gripMode != 2)
                return Style::Styles::TwoHand;
            else if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
                leftMagic = 2;
            else if (lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandSword ||
                     lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandDagger ||
                     lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandAxe ||
                     lHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandMace)
                leftWeapen = 1;
        }
    }
    RE::TESForm *rHand = VarUtils::player->GetActorRuntimeData().currentProcess->GetEquippedRightHand();
    if (rHand)
    {
        if (rHand->formType == RE::FormType::Spell || rHand->formType == RE::FormType::Scroll)
            rightMagic = 1;
        else if (rHand->formType == RE::FormType::Weapon)
        {
            if (rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kStaff)
                rightMagic = 2;
            else if (rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandSword ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandDagger ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandAxe ||
                     rHand->As<RE::TESObjectWEAP>()->GetWeaponType() == RE::WEAPON_TYPE::kOneHandMace || gripMode == 2)
                rightWeapen = 1;
        }
    }
    if (shield)
    {
        if (rightMagic == 1)
            return Style::Styles::ShieldMagic;
        else if (rightMagic == 2)
            return Style::Styles::ShieldStaff;
        if (rightWeapen == 1)
            return Style::Styles::ShieldSword;
        else if (rightWeapen == 0)
            return Style::Styles::ShieldFist;
    }
    else if (leftMagic == 1)
    {
        if (rightMagic == 1)
            return Style::Styles::DualMagic;
        else if (rightMagic == 2)
            return Style::Styles::MagicStaff;
        if (rightWeapen == 1)
            return Style::Styles::MagicSword;
        else if (rightWeapen == 0)
            return Style::Styles::MagicFist;
    }
    else if (leftMagic == 2)
    {
        if (rightMagic == 1)
            return Style::Styles::StaffMagic;
        else if (rightMagic == 2)
            return Style::Styles::DualStaff;
        if (rightWeapen == 1)
            return Style::Styles::StaffSword;
        else if (rightWeapen == 0)
            return Style::Styles::StaffFist;
    }
    else if (leftWeapen == 0)
    {
        if (rightMagic == 1)
            return Style::Styles::FistMagic;
        else if (rightMagic == 2)
            return Style::Styles::FistStaff;
        if (rightWeapen == 1)
            return Style::Styles::FistSword;
        else if (rightWeapen == 0)
            return Style::Styles::DualFist;
    }
    else if (leftWeapen == 1)
    {
        if (rightMagic == 1)
            return Style::Styles::SwordMagic;
        else if (rightMagic == 2)
            return Style::Styles::SwordStaff;
        if (rightWeapen == 1)
            return Style::Styles::DualSword;
        else if (rightWeapen == 0)
            return Style::Styles::SwordFist;
    }
    else if (leftWeapen == 2)
    {
        if (rightMagic == 1)
            return Style::Styles::TorchMagic;
        else if (rightMagic == 2)
            return Style::Styles::TorchStaff;
        if (rightWeapen == 1)
            return Style::Styles::TorchSword;
        else if (rightWeapen == 0)
            return Style::Styles::TorchFist;
    }
    return Style::Styles::Unknown;
}
bool CanDo()
{
    if (VarUtils::ui->numPausesGame > 0 || PlayerStatus::IsInKillmove() || PlayerStatus::IsRiding() ||
        VarUtils::ui->IsMenuOpen("Dialogue Menu") ||
        VarUtils::player->AsActorState()->actorState1.sitSleepState != SIT_SLEEP_STATE::kNormal ||
        (VarUtils::ctrlMap->enabledControls.underlying() &
         ((uint32_t)UserEvents::USER_EVENT_FLAG::kMovement & (uint32_t)UserEvents::USER_EVENT_FLAG::kLooking)) !=
            ((uint32_t)UserEvents::USER_EVENT_FLAG::kMovement & (uint32_t)UserEvents::USER_EVENT_FLAG::kLooking))
        return false;
    return true;
}
bool CanBash()
{
    if (Style::currentStyle == Style::Styles::TwoHand || Style::currentStyle == Style::Styles::DualSword ||
        Style::currentStyle == Style::Styles::DualFist || Style::currentStyle == Style::Styles::DualMagic ||
        Style::currentStyle == Style::Styles::FistSword || Style::currentStyle == Style::Styles::FistMagic ||
        Style::currentStyle == Style::Styles::FistStaff || Style::currentStyle == Style::Styles::ShieldSword ||
        Style::currentStyle == Style::Styles::ShieldFist || Style::currentStyle == Style::Styles::ShieldMagic ||
        Style::currentStyle == Style::Styles::ShieldStaff || Style::currentStyle == Style::Styles::ShieldMagic ||
        Style::currentStyle == Style::Styles::MagicSword || Style::currentStyle == Style::Styles::MagicFist ||
        Style::currentStyle == Style::Styles::MagicStaff || Style::currentStyle == Style::Styles::StaffSword)
        return true;
    return false;
}
void ActionPreInput(std::function<void()> func)
{
    while ((TimeUtils::GetTime() - queue.time) / 1000.0 < preInputTime)
    {
        // Process NormalAttack
        if (isInQueue == 1 && Compatibility::CanNormalAttack())
        {
            func();
            break;
        }
        // Process PowerAttack
        else if (isInQueue == 2 && Compatibility::CanPowerAttack())
        {
            func();
            if (Compatibility::BFCO &&
                (queue.action == ActionList::PowerAttackRight || queue.action == ActionList::PowerAttackLeft ||
                 queue.action == ActionList::PowerAttackDual))
            {
                std::thread([]() {
                    while (KeyUtils::GetKeyState(Config::powerAttack))
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    SKSE::GetTaskInterface()->AddTask(
                        []() { VarUtils::player->NotifyAnimationGraph("BFCOAttackstart_1"); });
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }).join();
            }
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    isInQueue = 0;
}
void NormalAttack(AttackType type)
{
    if (Config::enableSprintAttack && PlayerStatus::IsSprinting() && !PlayerStatus::IsAttacking())
    {
        if ((TimeUtils::GetTime() - startTime) / 1000.0 < intervalTime)
            return;
        doAnimation(ActionList::NormalAttackSprint);
        return;
    }
    ActionList::Animation target;
    switch (type)
    {
    case AttackType::RightHand:
        target = ActionList::NormalAttackRight;
        break;
    case AttackType::LeftHand:
        target = ActionList::NormalAttackLeft;
        break;
    case AttackType::DualHand:
        target = ActionList::NormalAttackDual;
        break;
    }
    if (Compatibility::BFCO)
    {
        if ((TimeUtils::GetTime() - startTime) / 1000.0 < intervalTime)
            return;
        if (PlayerStatus::IsSwiming())
        {
            VarUtils::player->NotifyAnimationGraph("BfcoSwimStopFast");
            doAnimation(Compatibility::BFCO_NormalAttackSwim);
        }
        if (PlayerStatus::IsJumping() && !PlayerStatus::IsAttacking())
        {
            VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
            doAnimation(Compatibility::BFCO_NormalAttackJump);
            return;
        }
        if (KeyUtils::GetKeyState(Config::BFCO_SpecialAttackModifier))
        {
            if (!PlayerStatus::IsAttacking())
                doAnimation(Compatibility::BFCO_NormalAttackSpecial);
            else if (PlayerStatus::IsAttacking() && !Compatibility::CanNormalAttack())
            {
                queue.action = Compatibility::BFCO_NormalAttackSpecial;
                queue.time = TimeUtils::GetTime();
                if (!isInQueue)
                {
                    isInQueue = 1;
                    std::thread(ActionPreInput, []() { doAnimation(queue.action); }).detach();
                }
                else
                    isInQueue = 1;
            }
            return;
        }
    }
    if (Compatibility::MCO || Compatibility::BFCO)
    {
        if ((TimeUtils::GetTime() - startTime) / 1000.0 < intervalTime)
            return;
        if (PlayerStatus::IsAttacking() && !Compatibility::CanNormalAttack())
        {
            queue.action = target;
            queue.time = TimeUtils::GetTime();
            if (!isInQueue)
            {
                isInQueue = 1;
                std::thread(ActionPreInput, []() { doAnimation(queue.action); }).detach();
            }
            else
                isInQueue = 1;
            return;
        }
        else
        {
            if (isInQueue)
                return;
            doAnimation(target);
        }
    }
    else
        doAnimation(target);
}
void PowerAttack(AttackType type)
{
    if (Config::enableSprintAttack && PlayerStatus::IsSprinting() && !PlayerStatus::IsAttacking())
    {
        if ((TimeUtils::GetTime() - startTime) / 1000.0 < intervalTime)
            return;
        if (Style::currentStyle == Style::Styles::TwoHand)
            doAnimation(ActionList::PowerAttackSprint2H);
        else
            doAnimation(ActionList::PowerAttackSprint1H);
        return;
    }
    ActionList::Animation target;
    switch (type)
    {
    case AttackType::RightHand:
        target = ActionList::PowerAttackRight;
        break;
    case AttackType::LeftHand:
        target = ActionList::PowerAttackLeft;
        break;
    case AttackType::DualHand:
        target = ActionList::PowerAttackDual;
        break;
    }
    if (Compatibility::BFCO)
    {
        if ((TimeUtils::GetTime() - startTime) / 1000.0 < intervalTime)
            return;
        if (PlayerStatus::IsSwiming())
        {
            VarUtils::player->NotifyAnimationGraph("BfcoSwimStopFast");
            doAnimation(Compatibility::BFCO_PowerAttackSwim);
        }
        if (PlayerStatus::IsJumping() && !PlayerStatus::IsAttacking())
        {
            VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
            if (Style::currentStyle == Style::Styles::TwoHand)
                doAnimation(Compatibility::BFCO_PowerAttackJump2H);
            else
                doAnimation(Compatibility::BFCO_PowerAttackJump1H);
            return;
        }
        if (KeyUtils::GetKeyState(Config::BFCO_SpecialAttackModifier))
        {
            if (!PlayerStatus::IsAttacking())
                doAnimation(Compatibility::BFCO_PowerAttackSpecial);
            else if (PlayerStatus::IsAttacking() && !Compatibility::CanPowerAttack())
            {
                queue.action = Compatibility::BFCO_PowerAttackSpecial;
                queue.time = TimeUtils::GetTime();
                if (!isInQueue)
                {
                    isInQueue = 2;
                    std::thread(ActionPreInput, []() { doAnimation(queue.action); }).detach();
                }
                else
                    isInQueue = 2;
            }
            return;
        }
    }
    if (Compatibility::MCO || Compatibility::BFCO)
    {
        if ((TimeUtils::GetTime() - startTime) / 1000.0 < intervalTime)
            return;
        if (PlayerStatus::IsAttacking() && !Compatibility::CanNormalAttack())
        {
            queue.action = target;
            queue.time = TimeUtils::GetTime();
            if (!isInQueue)
            {
                isInQueue = 2;
                std::thread(ActionPreInput, []() { doAnimation(queue.action); }).detach();
            }
            else
                isInQueue = 2;
            return;
        }
        else
        {
            if (isInQueue)
                return;
            doAnimation(target);
            if (Compatibility::BFCO)
            {
                std::thread([]() {
                    while (KeyUtils::GetKeyState(Config::powerAttack))
                        std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    SKSE::GetTaskInterface()->AddTask(
                        []() { VarUtils::player->NotifyAnimationGraph("BFCOAttackstart_1"); });
                }).detach();
            }
        }
    }
    else
        doAnimation(target);
}
void Block()
{
    while (PlayerStatus::IsSheathe() && !PlayerStatus::IsDodging() && !PlayerStatus::IsBlocking() &&
           KeyUtils::GetKeyState(Config::block))
    {
        SKSE::GetTaskInterface()->AddTask([]() {
            if (VarUtils::player->GetActorRuntimeData().currentProcess->PlayIdle(VarUtils::player,
                                                                                 ActionList::BlockStart.idle, nullptr))
            {
                vanillaRMB = false;
                VarUtils::player->AsActorState()->actorState2.wantBlocking = true;
            }
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}
void SheatheAttack()
{
    // NormalAttack();
}
void Recover()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        Style::currentStyle = DetectStyle();
        preInputTime = Style::styleMap[Style::currentStyle].preInputTime;
        intervalTime = Style::styleMap[Style::currentStyle].intervalTime;
        if (Style::styleMap[Style::currentStyle].isUsingHold)
            Style::styleMap[Style::currentStyle].isAltTypeEnable =
                KeyUtils::GetKeyState(Style::styleMap[Style::currentStyle].attackTypeModifier);
        if ((PlayerStatus::IsBlocking() || PlayerStatus::IsBashing()) &&
            ((!KeyUtils::GetKeyState(Config::block) && !vanillaRMB) ||
             (vanillaRMB && !KeyUtils::GetKeyState(KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->leftAttack)))))
            SKSE::GetTaskInterface()->AddTask([]() {
                if (VarUtils::player->NotifyAnimationGraph("blockStop"))
                    VarUtils::player->AsActorState()->actorState2.wantBlocking = false;
            });
        if (Config::enableHoldSprint)
            VarUtils::player->GetPlayerRuntimeData().playerFlags.isSprinting =
                KeyUtils::GetKeyState(KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sprint));
    }
}

//
// AnimationGraphEventSink
//
AnimationGraphEventSink *AnimationGraphEventSink::GetSingleton()
{
    static AnimationGraphEventSink singleton;
    return &singleton;
}
void AnimationGraphEventSink::Install()
{
    VarUtils::player->AddAnimationGraphEventSink(AnimationGraphEventSink::GetSingleton());
}
RE::BSEventNotifyControl AnimationGraphEventSink::ProcessEvent(
    const RE::BSAnimationGraphEvent *a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent> *a_eventSource)
{
    if (!a_event || !a_eventSource)
        return RE::BSEventNotifyControl::kContinue;
    // BFCO & MCO
    if (Compatibility::BFCO || Compatibility::MCO)
    {
        if (a_event->tag == "MCO_WinOpen" || a_event->tag == "BFCO_NextWinStart")
            Compatibility::normalAttackWin = true;
        else if (a_event->tag == "MCO_PowerWinOpen" || a_event->tag == "BFCO_NextPowerWinStart")
            Compatibility::powerAttackWin = true;
        else if (a_event->tag == "MCO_WinClose")
            Compatibility::normalAttackWin = false;
        else if (a_event->tag == "MCO_PowerWinClose")
            Compatibility::powerAttackWin = false;
        else if (a_event->tag == "BFCO_DIY_EndLoop")
        {
            Compatibility::normalAttackWin = false;
            Compatibility::powerAttackWin = false;
        }
    }
    // TK Dodge
    if (a_event->tag == "TKDodgeForward" || a_event->tag == "TKDodgeBack" || a_event->tag == "TKDodgeLeft" ||
        a_event->tag == "TKDodgeRight" || a_event->tag == "TKDR_DodgeStart")
        PlayerStatus::dodge = true;
    else if (a_event->tag == "TKDodgeStop" || a_event->tag == "TKDR_DodgeEnd")
        PlayerStatus::dodge = false;

    return RE::BSEventNotifyControl::kContinue;
}

//
// MenuOpenHandler
//
MenuOpenHandler *MenuOpenHandler::that;
MenuOpenHandler::FnCanProcess MenuOpenHandler::FnCP;
MenuOpenHandler::FnProcessButton MenuOpenHandler::FnPB;
bool MenuOpenHandler::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn); // altTweenMenu
        if (Custom::enableCustomInput && evn->IsDown())
            Custom::inputQueue.push_back(Custom::NewInput{code, TimeUtils::GetTime()});
        if (code == KeyUtils::KeyBoard::K && KeyUtils::GetKeyState(KeyUtils::KeyBoard::RightShift) && evn->IsDown())
            GUI::showSettings = !GUI::showSettings;
        if ((code == KeyUtils::Mouse::MouseWheelUp || code == KeyUtils::Mouse::MouseWheelDown) && evn->IsDown())
        {
            KeyUtils::MouseWheelTime = TimeUtils::GetTime();
            if (code == KeyUtils::Mouse::MouseWheelUp)
                KeyUtils::MouseWheelStatus = 1;
            else
                KeyUtils::MouseWheelStatus = -1;
            if (!KeyUtils::TracingMouse)
            {
                KeyUtils::TracingMouse = true;
                std::thread([]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    if ((TimeUtils::GetTime() - KeyUtils::MouseWheelTime) / 1000.0 > 10)
                        KeyUtils::MouseWheelStatus = 0;
                    KeyUtils::TracingMouse = false;
                }).detach();
            }
        }
        while (Stances::enableStances)
        {
            if (Stances::StancesModfier)
                if (!KeyUtils::GetKeyState(Stances::StancesModfier))
                    break;
            if (code == Stances::ChangeToLow)
                Stances::ChangeStanceTo(Stances::Stances::Low);
            else if (code == Stances::ChangeToMid)
                Stances::ChangeStanceTo(Stances::Stances::Mid);
            else if (code == Stances::ChangeToHigh)
                Stances::ChangeStanceTo(Stances::Stances::High);
            else if (code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon))
                Stances::ChangeStanceTo(Stances::Stances::Sheathe);
            break;
        }
        while (code == Config::warAsh && Compatibility::ELDEN && Compatibility::CanUseWarAsh)
        {
            if (Config::warAshModifier)
                if (!KeyUtils::GetKeyState(Config::warAshModifier))
                    break;
            auto spells = RE::MagicFavorites::GetSingleton()->spells;
            for (auto spell = spells.begin(); spell != spells.end(); spell++)
                if (Compatibility::IsWarAsh(*spell))
                {
                    VarUtils::player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)
                        ->CastSpellImmediate((RE::MagicItem *)(*spell), true, nullptr, 0, false, 0, nullptr);
                    Compatibility::CanUseWarAsh = false;
                    std::thread([]() {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                        Compatibility::CanUseWarAsh = true;
                    }).detach();
                    break;
                }
            break;
        }
        if (Config::altTweenMenu && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->tweenMenu))
            return false;
        if (code == Config::altTweenMenu)
        {
            a_event->AsButtonEvent()->userEvent = VarUtils::userEvent->tweenMenu;
            return true;
        }
    }
    return (this->*FnCP)(a_event);
}
bool MenuOpenHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool MenuOpenHandler::ProcessButton(ButtonEvent *a_event)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event);
}
bool MenuOpenHandler::PB(ButtonEvent *a_event)
{
    return (that->*FnPB)(a_event);
}
void MenuOpenHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_MenuOpenHandler[0]};
    that = stl::unrestricted_cast<MenuOpenHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &MenuOpenHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(5, &MenuOpenHandler::ProcessButton));
}

//
// AutoMoveHandler
//
AutoMoveHandler *AutoMoveHandler::that;
AutoMoveHandler::FnProcessButton AutoMoveHandler::FnPB;
bool AutoMoveHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);

    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
void AutoMoveHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_AutoMoveHandler[0]};
    that = stl::unrestricted_cast<AutoMoveHandler *>(vtable.address());
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &AutoMoveHandler::ProcessButton));
}

//
// FirstPersonState
//
FirstPersonState *FirstPersonState::that;
FirstPersonState::FnCanProcess FirstPersonState::FnCP;
FirstPersonState::FnProcessButton FirstPersonState::FnPB;
bool FirstPersonState::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
        // altTogglePOV
        if (Config::altTogglePOV && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV))
            return false;
        if (code == Config::altTogglePOV)
            return true;
        // altZoomIn
        if (Config::altZoomIn && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->zoomIn))
            return false;
        if (code == Config::altZoomIn)
            return true;
        // altZoomOut
        if (Config::altZoomOut && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->zoomOut))
            return false;
        if (code == Config::altZoomOut)
            return true;
    }
    return (this->*FnCP)(a_event);
}
bool FirstPersonState::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool FirstPersonState::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    // altTogglePOV
    if (code == Config::altTogglePOV)
        a_event->userEvent = VarUtils::userEvent->togglePOV;
    // altZoomIn
    if (code == Config::altZoomIn)
        a_event->userEvent = VarUtils::userEvent->zoomIn;
    // altZoomOut
    if (code == Config::altZoomOut)
        a_event->userEvent = VarUtils::userEvent->zoomOut;

    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool FirstPersonState::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void FirstPersonState::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_FirstPersonState[1]};
    that = stl::unrestricted_cast<FirstPersonState *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &FirstPersonState::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &FirstPersonState::ProcessButton));
}

//
// HeldStateHandler - AttackBlockHandler
//
AttackBlockHandler *AttackBlockHandler::that;
AttackBlockHandler::FnCanProcess AttackBlockHandler::FnCP;
AttackBlockHandler::FnProcessButton AttackBlockHandler::FnPB;
bool AttackBlockHandler::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
        if (!Style::styleMap[Style::currentStyle].isUsingHold && evn->IsDown() &&
            code == Style::styleMap[Style::currentStyle].attackTypeModifier)
            Style::styleMap[Style::currentStyle].isAltTypeEnable =
                !Style::styleMap[Style::currentStyle].isAltTypeEnable;
        if (!PlayerStatus::IsSheathe())
            return false;
        if (code == Config::normalAttack || code == Config::powerAttack || code == Config::otherAttack ||
            code == Config::block || code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon))
            return true;
        if (code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->rightAttack) ||
            code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->leftAttack))
            return false;
        if (Compatibility::BFCO && code == Config::BFCO_ComboAttack)
            return true;
    }
    return false;
}
bool AttackBlockHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool AttackBlockHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (CanDo() && Config::normalAttack && Config::powerAttack && Config::block)
    {
        if (VarUtils::player->IsBlocking() || PlayerStatus::IsBashing())
        {
            if (CanBash())
            {
                if (code == Config::normalAttack)
                {
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    (this->*FnPB)(a_event, a_data);
                    VarUtils::player->NotifyAnimationGraph("bashRelease");
                    return true;
                }
                else if (code == Config::powerAttack)
                {
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
            }
            else
                return true;
        }
        if (code == Config::block && a_event->IsDown())
            std::thread(Block).detach();
        // BFCO ComboAttack
        if (Compatibility::BFCO && code == Config::BFCO_ComboAttack && PlayerStatus::IsAttacking())
        {
            VarUtils::player->NotifyAnimationGraph("BFCOAttackStart_Comb");
            doAnimation(Compatibility::BFCO_ComboAttack);
        }
        if (code == Config::normalAttack)
        {
            if (Style::styleMap[Style::currentStyle].isAltTypeEnable)
            {
                if (Style::styleMap[Style::currentStyle].altNormalAttackType == AttackType::RightHand ||
                    Style::styleMap[Style::currentStyle].altNormalAttackType == AttackType::LeftHand ||
                    Style::styleMap[Style::currentStyle].altNormalAttackType == AttackType::DualHand)
                {
                    if (a_event->IsDown() && !Style::styleMap[Style::currentStyle].repeatNormalAttack)
                        NormalAttack(Style::styleMap[Style::currentStyle].altNormalAttackType);
                    else if (Style::styleMap[Style::currentStyle].repeatNormalAttack)
                        NormalAttack(Style::styleMap[Style::currentStyle].altNormalAttackType);
                }
                else if (Style::styleMap[Style::currentStyle].altNormalAttackType == AttackType::VanillaLMB)
                {
                    if (Compatibility::BFCO && Style::currentStyle == Style::Styles::Bow && PlayerStatus::IsJumping() &&
                        !PlayerStatus::IsAttacking())
                    {
                        VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
                        doAnimation(Compatibility::BFCO_BowAttackJump);
                        return true;
                    }
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
                else if (Style::styleMap[Style::currentStyle].altNormalAttackType == AttackType::VanillaRMB)
                {
                    if (Compatibility::BFCO && Style::currentStyle == Style::Styles::Bow && PlayerStatus::IsJumping() &&
                        !PlayerStatus::IsAttacking())
                    {
                        VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
                        doAnimation(Compatibility::BFCO_BowAttackJump);
                        return true;
                    }
                    a_event->userEvent = VarUtils::userEvent->leftAttack;
                    (this->*FnPB)(a_event, a_data);
                    if (PlayerStatus::IsBlocking())
                        vanillaRMB = true;
                    return true;
                }
            }
            else
            {
                if (Style::styleMap[Style::currentStyle].normalAttackType == AttackType::RightHand ||
                    Style::styleMap[Style::currentStyle].normalAttackType == AttackType::LeftHand ||
                    Style::styleMap[Style::currentStyle].normalAttackType == AttackType::DualHand)
                {
                    if (a_event->IsDown() && !Style::styleMap[Style::currentStyle].repeatNormalAttack)
                        NormalAttack(Style::styleMap[Style::currentStyle].normalAttackType);
                    else if (Style::styleMap[Style::currentStyle].repeatNormalAttack)
                        NormalAttack(Style::styleMap[Style::currentStyle].normalAttackType);
                }
                else if (Style::styleMap[Style::currentStyle].normalAttackType == AttackType::VanillaLMB)
                {
                    if (Compatibility::BFCO && Style::currentStyle == Style::Styles::Bow && PlayerStatus::IsJumping() &&
                        !PlayerStatus::IsAttacking())
                    {
                        VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
                        doAnimation(Compatibility::BFCO_BowAttackJump);
                        return true;
                    }
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
                else if (Style::styleMap[Style::currentStyle].normalAttackType == AttackType::VanillaRMB)
                {
                    if (Compatibility::BFCO && Style::currentStyle == Style::Styles::Bow && PlayerStatus::IsJumping() &&
                        !PlayerStatus::IsAttacking())
                    {
                        VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
                        doAnimation(Compatibility::BFCO_BowAttackJump);
                        return true;
                    }
                    a_event->userEvent = VarUtils::userEvent->leftAttack;
                    (this->*FnPB)(a_event, a_data);
                    if (PlayerStatus::IsBlocking())
                        vanillaRMB = true;
                    return true;
                }
            }
            a_event->userEvent = "";
        }
        else if (code == Config::powerAttack)
        {
            if (Style::styleMap[Style::currentStyle].isAltTypeEnable)
            {
                if (Style::styleMap[Style::currentStyle].altPowerAttackType == AttackType::RightHand ||
                    Style::styleMap[Style::currentStyle].altPowerAttackType == AttackType::LeftHand ||
                    Style::styleMap[Style::currentStyle].altPowerAttackType == AttackType::DualHand)
                {
                    if (a_event->IsDown() && !Style::styleMap[Style::currentStyle].repeatPowerAttack)
                        PowerAttack(Style::styleMap[Style::currentStyle].altPowerAttackType);
                    else if (Style::styleMap[Style::currentStyle].repeatPowerAttack)
                        PowerAttack(Style::styleMap[Style::currentStyle].altPowerAttackType);
                }
                else if (Style::styleMap[Style::currentStyle].altPowerAttackType == AttackType::VanillaLMB)
                {
                    if (Compatibility::BFCO && Style::currentStyle == Style::Styles::Bow && PlayerStatus::IsJumping() &&
                        !PlayerStatus::IsAttacking())
                    {
                        VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
                        doAnimation(Compatibility::BFCO_BowPowerAttackJump);
                        return true;
                    }
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
                else if (Style::styleMap[Style::currentStyle].altPowerAttackType == AttackType::VanillaRMB)
                {
                    if (Compatibility::BFCO && Style::currentStyle == Style::Styles::Bow && PlayerStatus::IsJumping() &&
                        !PlayerStatus::IsAttacking())
                    {
                        VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
                        doAnimation(Compatibility::BFCO_BowPowerAttackJump);
                        return true;
                    }
                    a_event->userEvent = VarUtils::userEvent->leftAttack;
                    (this->*FnPB)(a_event, a_data);
                    if (PlayerStatus::IsBlocking())
                        vanillaRMB = true;
                    return true;
                }
                else
                    return false;
            }
            else
            {
                if (Style::styleMap[Style::currentStyle].powerAttackType == AttackType::RightHand ||
                    Style::styleMap[Style::currentStyle].powerAttackType == AttackType::LeftHand ||
                    Style::styleMap[Style::currentStyle].powerAttackType == AttackType::DualHand)
                {
                    if (a_event->IsDown() && !Style::styleMap[Style::currentStyle].repeatPowerAttack)
                        PowerAttack(Style::styleMap[Style::currentStyle].powerAttackType);
                    else if (Style::styleMap[Style::currentStyle].repeatPowerAttack)
                        PowerAttack(Style::styleMap[Style::currentStyle].powerAttackType);
                }
                else if (Style::styleMap[Style::currentStyle].powerAttackType == AttackType::VanillaLMB)
                {
                    if (Compatibility::BFCO && Style::currentStyle == Style::Styles::Bow && PlayerStatus::IsJumping() &&
                        !PlayerStatus::IsAttacking())
                    {
                        VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
                        doAnimation(Compatibility::BFCO_BowPowerAttackJump);
                        return true;
                    }
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
                else if (Style::styleMap[Style::currentStyle].powerAttackType == AttackType::VanillaRMB)
                {
                    if (Compatibility::BFCO && Style::currentStyle == Style::Styles::Bow && PlayerStatus::IsJumping() &&
                        !PlayerStatus::IsAttacking())
                    {
                        VarUtils::player->NotifyAnimationGraph("BfcoJumpStop");
                        doAnimation(Compatibility::BFCO_BowPowerAttackJump);
                        return true;
                    }
                    a_event->userEvent = VarUtils::userEvent->leftAttack;
                    (this->*FnPB)(a_event, a_data);
                    if (PlayerStatus::IsBlocking())
                        vanillaRMB = true;
                    return true;
                }
            }
            a_event->userEvent = "";
        }
        else if (code == Config::otherAttack)
        {
            if (Style::styleMap[Style::currentStyle].isAltTypeEnable)
            {
                if (Style::styleMap[Style::currentStyle].altOtherAttackType == AttackType::VanillaLMB)
                {
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
                else if (Style::styleMap[Style::currentStyle].altOtherAttackType == AttackType::VanillaRMB)
                {
                    a_event->userEvent = VarUtils::userEvent->leftAttack;
                    (this->*FnPB)(a_event, a_data);
                    if (PlayerStatus::IsBlocking())
                        vanillaRMB = true;
                    return true;
                }
                else
                    return false;
            }
            else
            {
                if (Style::styleMap[Style::currentStyle].otherAttackType == AttackType::VanillaLMB)
                {
                    a_event->userEvent = VarUtils::userEvent->rightAttack;
                    return (this->*FnPB)(a_event, a_data);
                }
                else if (Style::styleMap[Style::currentStyle].otherAttackType == AttackType::VanillaRMB)
                {
                    a_event->userEvent = VarUtils::userEvent->leftAttack;
                    (this->*FnPB)(a_event, a_data);
                    if (PlayerStatus::IsBlocking())
                        vanillaRMB = true;
                    return true;
                }
            }
            a_event->userEvent = "";
        }
        return (this->*FnPB)(a_event, a_data);
    }
    if (PlayerStatus::IsRiding() && Config::enableReverseHorseAttack)
    {
        if (code == Config::normalAttack || code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->rightAttack))
            a_event->userEvent = VarUtils::userEvent->leftAttack;
        else if (code == Config::powerAttack || code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->leftAttack))
            a_event->userEvent = VarUtils::userEvent->rightAttack;
    }
    return (this->*FnPB)(a_event, a_data);
}
bool AttackBlockHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void AttackBlockHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_AttackBlockHandler[0]};
    that = stl::unrestricted_cast<AttackBlockHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &AttackBlockHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &AttackBlockHandler::ProcessButton));
}

//
// HeldStateHandler - SprintHandler
//
SprintHandler *SprintHandler::that;
SprintHandler::FnCanProcess SprintHandler::FnCP;
SprintHandler::FnProcessButton SprintHandler::FnPB;
bool SprintHandler::CanProcess(InputEvent *a_event)
{
    return (this->*FnCP)(a_event);
}
bool SprintHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool SprintHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (this->*FnPB)(a_event, a_data);
}
bool SprintHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void SprintHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_SprintHandler[0]};
    that = stl::unrestricted_cast<SprintHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &SprintHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &SprintHandler::ProcessButton));
}

//
// HeldStateHandler - TogglePOVHandler
//
TogglePOVHandler *TogglePOVHandler::that;
TogglePOVHandler::FnCanProcess TogglePOVHandler::FnCP;
TogglePOVHandler::FnProcessButton TogglePOVHandler::FnPB;
bool TogglePOVHandler::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
        // altTogglePOV
        if (Config::altTogglePOV && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV))
            return false;
        if (code == Config::altTogglePOV)
            return true;
    }
    return (this->*FnCP)(a_event);
}
bool TogglePOVHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool TogglePOVHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    // altTogglePOV
    if (code == Config::altTogglePOV)
        a_event->userEvent = VarUtils::userEvent->togglePOV;

    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool TogglePOVHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void TogglePOVHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_TogglePOVHandler[0]};
    that = stl::unrestricted_cast<TogglePOVHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &TogglePOVHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &TogglePOVHandler::ProcessButton));
}

//
// MovementHandler
//
MovementHandler *MovementHandler::that;
MovementHandler::FnCanProcess MovementHandler::FnCP;
MovementHandler::FnProcessButton MovementHandler::FnPB;
bool MovementHandler::CanProcess(InputEvent *a_event)
{
    return (this->*FnCP)(a_event);
}
bool MovementHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool MovementHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (this->*FnPB)(a_event, a_data);
}
bool MovementHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void MovementHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_MovementHandler[0]};
    that = stl::unrestricted_cast<MovementHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &MovementHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &MovementHandler::ProcessButton));
}

//
// ReadyWeaponHandler
//
ReadyWeaponHandler *ReadyWeaponHandler::that;
ReadyWeaponHandler::FnCanProcess ReadyWeaponHandler::FnCP;
ReadyWeaponHandler::FnProcessButton ReadyWeaponHandler::FnPB;
bool ReadyWeaponHandler::CanProcess(InputEvent *a_event)
{
    if (Stances::enableStances)
        if (KeyUtils::GetKeyState(Stances::StancesModfier))
            return false;
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
        if (code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon))
            return (this->*FnCP)(a_event);
        if (!PlayerStatus::IsSheathe())
        {
            if (code == Config::normalAttack && Style::styleMap[Style::currentStyle].sheatheNormalAttack)
                return true;
            else if (code == Config::powerAttack && Style::styleMap[Style::currentStyle].sheathePowerAttack)
                return true;
            else if (code == Config::otherAttack && Style::styleMap[Style::currentStyle].sheatheOtherAttack)
                return true;
        }
    }
    return false;
}
bool ReadyWeaponHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool ReadyWeaponHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    if (Compatibility::BFCO && PlayerStatus::IsSwiming() && !PlayerStatus::IsSheathe())
    {
        doAnimation(Compatibility::BFCO_SwimSheathe);
        return true;
    }
    a_event->userEvent = VarUtils::userEvent->readyWeapon;
    if (PlayerStatus::IsAttacking() && Style::currentStyle != Style::Styles::Bow)
        return false;
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool ReadyWeaponHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void ReadyWeaponHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_ReadyWeaponHandler[0]};
    that = stl::unrestricted_cast<ReadyWeaponHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &ReadyWeaponHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &ReadyWeaponHandler::ProcessButton));
}

//
// SneakHandler
//
SneakHandler *SneakHandler::that;
SneakHandler::FnCanProcess SneakHandler::FnCP;
SneakHandler::FnProcessButton SneakHandler::FnPB;
bool SneakHandler::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
    }
    return (this->*FnCP)(a_event);
}
bool SneakHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool SneakHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::enableHoldSneak)
        std::thread([a_data]() {
            while (KeyUtils::GetKeyState(KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sneak)) ||
                   PlayerStatus::IsAttacking())
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            while (PlayerStatus::IsSneaking())
            {
                SKSE::GetTaskInterface()->AddTask([a_data]() {
                    std::unique_ptr<ButtonEvent> evn(
                        ButtonEvent::Create(INPUT_DEVICE::kKeyboard, VarUtils::userEvent->sneak, 0, 1, 0));
                    PB(evn.get(), a_data);
                });
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        }).detach();
    return (this->*FnPB)(a_event, a_data);
}
bool SneakHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void SneakHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_SneakHandler[0]};
    that = stl::unrestricted_cast<SneakHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &SneakHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &SneakHandler::ProcessButton));
}

//
// ThirdPersonState
//
ThirdPersonState *ThirdPersonState::that;
ThirdPersonState::FnCanProcess ThirdPersonState::FnCP;
ThirdPersonState::FnProcessButton ThirdPersonState::FnPB;
bool ThirdPersonState::CanProcess(InputEvent *a_event)
{
    if (a_event->GetEventType() == INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        auto code = KeyUtils::GetEventKeyMap(evn);
        // altTogglePOV
        if (Config::altTogglePOV && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV))
            return false;
        if (code == Config::altTogglePOV)
            return true;
        // altZoomIn
        if (Config::altZoomIn && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->zoomIn))
            return false;
        if (code == Config::altZoomIn)
            return true;
        // altZoomOut
        if (Config::altZoomOut && code == KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->zoomOut))
            return false;
        if (code == Config::altZoomOut)
            return true;
    }
    return (this->*FnCP)(a_event);
}
bool ThirdPersonState::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool ThirdPersonState::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    // altTogglePOV
    if (code == Config::altTogglePOV)
        a_event->userEvent = VarUtils::userEvent->togglePOV;
    // altZoomIn
    if (code == Config::altZoomIn)
        a_event->userEvent = VarUtils::userEvent->zoomIn;
    // altZoomOut
    if (code == Config::altZoomOut)
        a_event->userEvent = VarUtils::userEvent->zoomOut;

    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool ThirdPersonState::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void ThirdPersonState::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_ThirdPersonState[1]};
    that = stl::unrestricted_cast<ThirdPersonState *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &ThirdPersonState::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &ThirdPersonState::ProcessButton));
}

//
// ToggleRunHandler
//
ToggleRunHandler *ToggleRunHandler::that;
ToggleRunHandler::FnCanProcess ToggleRunHandler::FnCP;
ToggleRunHandler::FnProcessButton ToggleRunHandler::FnPB;
bool ToggleRunHandler::CanProcess(InputEvent *a_event)
{
    return (this->*FnCP)(a_event);
}
bool ToggleRunHandler::CP(InputEvent *a_event)
{
    return (that->*FnCP)(a_event);
}
bool ToggleRunHandler::ProcessButton(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    auto code = KeyUtils::GetEventKeyMap(a_event);
    if (Config::needModifier[code])
        if (!KeyUtils::GetKeyState(Config::needModifier[code]))
            return false;
    return (this->*FnPB)(a_event, a_data);
}
bool ToggleRunHandler::PB(ButtonEvent *a_event, PlayerControlsData *a_data)
{
    return (that->*FnPB)(a_event, a_data);
}
void ToggleRunHandler::Hook()
{
    REL::Relocation<uintptr_t> vtable{VTABLE_ToggleRunHandler[0]};
    that = stl::unrestricted_cast<ToggleRunHandler *>(vtable.address());
    FnCP = stl::unrestricted_cast<FnCanProcess>(vtable.write_vfunc(1, &ToggleRunHandler::CanProcess));
    FnPB = stl::unrestricted_cast<FnProcessButton>(vtable.write_vfunc(4, &ToggleRunHandler::ProcessButton));
}

void Hook()
{
    MenuOpenHandler::Hook();
    AutoMoveHandler::Hook();
    FirstPersonState::Hook();
    AttackBlockHandler::Hook();
    // SprintHandler::Hook();
    TogglePOVHandler::Hook();
    // MovementHandler::Hook();
    ReadyWeaponHandler::Hook();
    SneakHandler::Hook();
    ThirdPersonState::Hook();
    ToggleRunHandler::Hook();
    std::thread(Recover).detach();
}
} // namespace Hook
