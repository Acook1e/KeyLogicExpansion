#include "utils.h"

namespace VarUtils
{
const SKSE::SerializationInterface *cosave;
RE::Main *main = nullptr;
RE::PlayerCharacter *player = nullptr;
RE::PlayerControls *pCtrl = nullptr;
RE::ControlMap *ctrlMap = nullptr;
RE::UserEvents *userEvent = nullptr;
RE::UI *ui = nullptr;

void init()
{
    logger::trace("Initializing cosave serialization...");
    cosave = SKSE::GetSerializationInterface();
    cosave->SetUniqueID(_byteswap_ulong('AKLE'));
    logger::trace("Cosave serialization initialized.");

    main = RE::Main::GetSingleton();
    player = RE::PlayerCharacter::GetSingleton();
    pCtrl = RE::PlayerControls::GetSingleton();
    ctrlMap = RE::ControlMap::GetSingleton();
    userEvent = RE::UserEvents::GetSingleton();
    ui = RE::UI::GetSingleton();
}
} // namespace VarUtils

namespace KeyUtils
{
bool TracingMouse = false;
uint64_t MouseWheelTime;
float MouseWheelStatus = 0;

uint32_t GetWinKeyMap(uint32_t code)
{
    switch (code)
    {
    case 0:
        return 0;
    // KeyBoard
    case KeyBoard::ESC:
        return VK_ESCAPE;
    case KeyBoard::MAIN1:
        return SKSE::WinAPI::VK_1;
    case KeyBoard::MAIN2:
        return SKSE::WinAPI::VK_2;
    case KeyBoard::MAIN3:
        return SKSE::WinAPI::VK_3;
    case KeyBoard::MAIN4:
        return SKSE::WinAPI::VK_4;
    case KeyBoard::MAIN5:
        return SKSE::WinAPI::VK_5;
    case KeyBoard::MAIN6:
        return SKSE::WinAPI::VK_6;
    case KeyBoard::MAIN7:
        return SKSE::WinAPI::VK_7;
    case KeyBoard::MAIN8:
        return SKSE::WinAPI::VK_8;
    case KeyBoard::MAIN9:
        return SKSE::WinAPI::VK_9;
    case KeyBoard::MAIN0:
        return SKSE::WinAPI::VK_0;
    case KeyBoard::Minus:
        return VK_OEM_MINUS;
    case KeyBoard::Equals:
        return VK_OEM_PLUS;
    case KeyBoard::Backspace:
        return VK_BACK;
    case KeyBoard::Tab:
        return VK_TAB;
    case KeyBoard::Q:
        return SKSE::WinAPI::VK_Q;
    case KeyBoard::W:
        return SKSE::WinAPI::VK_W;
    case KeyBoard::E:
        return SKSE::WinAPI::VK_E;
    case KeyBoard::R:
        return SKSE::WinAPI::VK_R;
    case KeyBoard::T:
        return SKSE::WinAPI::VK_T;
    case KeyBoard::Y:
        return SKSE::WinAPI::VK_Y;
    case KeyBoard::U:
        return SKSE::WinAPI::VK_U;
    case KeyBoard::I:
        return SKSE::WinAPI::VK_I;
    case KeyBoard::O:
        return SKSE::WinAPI::VK_O;
    case KeyBoard::P:
        return SKSE::WinAPI::VK_P;
    case KeyBoard::LeftBracket:
        return VK_OEM_4;
    case KeyBoard::RightBracket:
        return VK_OEM_6;
    case KeyBoard::Enter:
        return VK_RETURN;
    case KeyBoard::LeftCtrl:
        return VK_LCONTROL;
    case KeyBoard::A:
        return SKSE::WinAPI::VK_A;
    case KeyBoard::S:
        return SKSE::WinAPI::VK_S;
    case KeyBoard::D:
        return SKSE::WinAPI::VK_D;
    case KeyBoard::F:
        return SKSE::WinAPI::VK_F;
    case KeyBoard::G:
        return SKSE::WinAPI::VK_G;
    case KeyBoard::H:
        return SKSE::WinAPI::VK_H;
    case KeyBoard::J:
        return SKSE::WinAPI::VK_J;
    case KeyBoard::K:
        return SKSE::WinAPI::VK_K;
    case KeyBoard::L:
        return SKSE::WinAPI::VK_L;
    case KeyBoard::Semicolon:
        return VK_OEM_1;
    case KeyBoard::Apostrophe:
        return VK_OEM_7;
    case KeyBoard::Console:
        return VK_OEM_3;
    case KeyBoard::LeftShift:
        return VK_LSHIFT;
    case KeyBoard::BackSlash:
        return VK_OEM_102;
    case KeyBoard::Z:
        return SKSE::WinAPI::VK_Z;
    case KeyBoard::X:
        return SKSE::WinAPI::VK_X;
    case KeyBoard::C:
        return SKSE::WinAPI::VK_C;
    case KeyBoard::V:
        return SKSE::WinAPI::VK_V;
    case KeyBoard::B:
        return SKSE::WinAPI::VK_B;
    case KeyBoard::N:
        return SKSE::WinAPI::VK_N;
    case KeyBoard::M:
        return SKSE::WinAPI::VK_M;
    case KeyBoard::Comma:
        return VK_OEM_COMMA;
    case KeyBoard::Period:
        return VK_OEM_PERIOD;
    case KeyBoard::Slash:
        return VK_OEM_2;
    case KeyBoard::RightShift:
        return VK_RSHIFT;
    case KeyBoard::NUM_star:
        return VK_MULTIPLY;
    case KeyBoard::LeftAlt:
        return VK_LMENU;
    case KeyBoard::Space:
        return VK_SPACE;
    case KeyBoard::CapsLock:
        return VK_CAPITAL;
    case KeyBoard::F1:
        return VK_F1;
    case KeyBoard::F2:
        return VK_F2;
    case KeyBoard::F3:
        return VK_F3;
    case KeyBoard::F4:
        return VK_F4;
    case KeyBoard::F5:
        return VK_F5;
    case KeyBoard::F6:
        return VK_F6;
    case KeyBoard::F7:
        return VK_F7;
    case KeyBoard::F8:
        return VK_F8;
    case KeyBoard::F9:
        return VK_F9;
    case KeyBoard::F10:
        return VK_F10;
    case KeyBoard::NumLock:
        return VK_NUMLOCK;
    case KeyBoard::SrcollLock:
        return VK_SCROLL;
    case KeyBoard::NUM7:
        return VK_NUMPAD7;
    case KeyBoard::NUM8:
        return VK_NUMPAD8;
    case KeyBoard::NUM9:
        return VK_NUMPAD9;
    case KeyBoard::NUM_Minus:
        return VK_SUBTRACT;
    case KeyBoard::NUM4:
        return VK_NUMPAD4;
    case KeyBoard::NUM5:
        return VK_NUMPAD5;
    case KeyBoard::NUM6:
        return VK_NUMPAD6;
    case KeyBoard::NUM_Plus:
        return VK_ADD;
    case KeyBoard::NUM1:
        return VK_NUMPAD1;
    case KeyBoard::NUM2:
        return VK_NUMPAD2;
    case KeyBoard::NUM3:
        return VK_NUMPAD3;
    case KeyBoard::NUM0:
        return VK_NUMPAD0;
    case KeyBoard::NUM_Dot:
        return VK_DECIMAL;
    case KeyBoard::F11:
        return VK_F11;
    case KeyBoard::F12:
        return VK_F12;
    case KeyBoard::NUM_Enter:
        return VK_RETURN;
    case KeyBoard::RightCtrl:
        return VK_RCONTROL;
    case KeyBoard::NUM_Slash:
        return VK_DIVIDE;
    case KeyBoard::PrtSc:
        return VK_SNAPSHOT;
    case KeyBoard::RightAlt:
        return VK_RMENU;
    case KeyBoard::Pause:
        return VK_PAUSE;
    case KeyBoard::Home:
        return VK_HOME;
    case KeyBoard::ArrowUp:
        return VK_UP;
    case KeyBoard::PageUp:
        return VK_PRIOR;
    case KeyBoard::ArrowLeft:
        return VK_LEFT;
    case KeyBoard::ArrowRight:
        return VK_RIGHT;
    case KeyBoard::End:
        return VK_END;
    case KeyBoard::ArrowDown:
        return VK_DOWN;
    case KeyBoard::PageDown:
        return VK_NEXT;
    case KeyBoard::Insert:
        return VK_INSERT;
    case KeyBoard::Delete:
        return VK_DELETE;

    // Mouse
    case Mouse::MouseButtonLeft:
        return VK_LBUTTON;
    case Mouse::MouseButtonRight:
        return VK_RBUTTON;
    case Mouse::MouseButtonMiddle:
        return VK_MBUTTON;
    case Mouse::MouseButton3:
        return VK_XBUTTON1;
    case Mouse::MouseButton4:
        return VK_XBUTTON2;
    case Mouse::MouseButton5:
        return 0;
    case Mouse::MouseButton6:
        return 0;
    case Mouse::MouseButton7:
        return 0;
    case Mouse::MouseWheelUp:
        return MouseWheelStatus > 0;
    case Mouse::MouseWheelDown:
        return MouseWheelStatus < 0;

        // GamePad
        /*
        case GamePad::GamepadButtonDpadUp:
            return VK_DELETE;
        case GamePad::GamepadButtonDpadDown:
            return VK_DELETE;
        case GamePad::GamepadButtonDpadLfet:
            return VK_DELETE;
        case GamePad::GamepadButtonDpadRight:
            return VK_DELETE;
        */
    }
    return 0;
}

bool GetKeyState(uint32_t code)
{
    if (!code)
        return false;
    auto vKey = GetWinKeyMap(code);
    if (vKey)
        return SKSE::WinAPI::GetKeyState(vKey) & 0x8000;
    return false;
}

uint32_t GetVanillaKeyMap(RE::BSFixedString userEvent)
{
    auto keyBoard = VarUtils::ctrlMap->GetMappedKey(userEvent, RE::INPUT_DEVICE::kKeyboard);
    auto mouse = VarUtils::ctrlMap->GetMappedKey(userEvent, RE::INPUT_DEVICE::kMouse);
    if (keyBoard != 0xFF)
        return keyBoard;
    if (mouse != 0xFF)
        return mouse + Mouse::MouseButtonOffset;
    return 0;
}

uint32_t GetEventKeyMap(RE::InputEvent *a_event)
{
    if (a_event->eventType == RE::INPUT_EVENT_TYPE::kButton)
    {
        auto evn = a_event->AsButtonEvent();
        switch (evn->GetDevice())
        {
        case RE::INPUT_DEVICE::kKeyboard:
            return evn->GetIDCode();
        case RE::INPUT_DEVICE::kMouse:
            return MouseButtonOffset + evn->GetIDCode();
        default:
            return 0;
        }
    }
    else if (a_event->eventType == RE::INPUT_EVENT_TYPE::kMouseMove)
    {
        return 0;
    }
    return 0;
}

uint32_t GetEventKeyMap(RE::ButtonEvent *evn)
{
    switch (evn->GetDevice())
    {
    case RE::INPUT_DEVICE::kKeyboard:
        return evn->GetIDCode();
    case RE::INPUT_DEVICE::kMouse:
        return MouseButtonOffset + evn->GetIDCode();
    default:
        return 0;
    }
}
} // namespace KeyUtils

namespace TimeUtils
{
uint64_t GetTime()
{
    auto now = std::chrono::steady_clock::now();
    uint64_t time = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count();
    return time;
}
} // namespace TimeUtils

namespace FormUtils
{
RE::TESForm *GetForm(RE::FormID FormId)
{
    return RE::TESForm::LookupByID(FormId);
}
RE::TESForm *GetForm(std::string modForm)
{
    std::string modName;
    RE::FormID formId;

    auto delimiterPos = modForm.find('|');
    if (delimiterPos == std::string::npos)
    {
        logger::error("Error: Unable to resolve FormID: {}\t\tReason: Can't find sysbol '|' ", modForm.c_str());
        return nullptr;
    }
    modName = modForm.substr(0, delimiterPos);
    std::string formid_str = modForm.substr(delimiterPos + 1);
    try
    {
        std::stringstream hexStream;
        hexStream << std::hex << formid_str;
        hexStream >> formId;
    }
    catch (std::exception e)
    {
        logger::error("Error:{}\t\tUnable to resolve FormID: {}\t\tReason: Can't transform {} to hex\t\t", e.what(),
                      modForm.c_str(), formid_str.c_str());
        return nullptr;
    }
    RE::TESDataHandler *dataHandler = RE::TESDataHandler::GetSingleton();
    RE::TESFile *modFile = nullptr;
    for (auto it = dataHandler->files.begin(); it != dataHandler->files.end(); it++)
    {
        RE::TESFile *f = *it;
        if (strcmp(f->fileName, modName.c_str()) == 0)
        {
            modFile = f;
            break;
        }
    }
    if (!modFile)
    {
        logger::error("Error: mod {} not exist", modName.c_str());
        return nullptr;
    }
    uint8_t modIndex = modFile->compileIndex;
    if (modIndex < 0xFE)
    {
        formId |= ((uint32_t)modIndex) << 24;
    }
    else
    {
        uint16_t lightModIndex = modFile->smallFileCompileIndex;
        if (lightModIndex != 0xFFFF)
        {
            formId |= 0xFE000000 | (uint32_t(lightModIndex) << 12);
        }
    }
    logger::trace("resolve formID:{} success, result:{}", modForm.c_str(), formId);
    return RE::TESForm::LookupByID(formId);
}
} // namespace FormUtils

namespace GameStatus
{
bool isGameRunning()
{
    return !VarUtils::main->quitGame;
}
} // namespace GameStatus

namespace PlayerStatus
{
bool dodge = false;

bool IsAttacking()
{
    bool res;
    VarUtils::player->GetGraphVariableBool("IsAttacking", std::ref(res));
    return res;
}
bool IsBlocking()
{
    return VarUtils::player->IsBlocking();
}
bool IsSheathe()
{
    return VarUtils::player->AsActorState()->IsWeaponDrawn();
}
bool IsBashing()
{
    bool res;
    VarUtils::player->GetGraphVariableBool("IsBashing", std::ref(res));
    return res;
}
bool IsMoving()
{
    return VarUtils::player->IsMoving();
}
bool IsSprinting()
{
    return VarUtils::player->AsActorState()->IsSprinting() && IsMoving();
}
bool IsSneaking()
{
    return VarUtils::player->IsSneaking();
}

bool IsDodging()
{
    return dodge;
}
bool IsJumping()
{
    bool res;
    VarUtils::player->GetGraphVariableBool("bInJumpState", std::ref(res));
    return res && VarUtils::player->IsInMidair();
}
bool IsRiding()
{
    return (VarUtils::player->AsActorState()->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kRidingMount);
}
bool IsSwiming()
{
    return VarUtils::player->AsActorState()->actorState1.swimming;
}
bool IsInKillmove()
{
    return VarUtils::player->GetActorRuntimeData().boolFlags.all(RE::Actor::BOOL_FLAGS::kIsInKillMove);
}
} // namespace PlayerStatus

namespace ActionList
{
Animation NormalAttackRight;
Animation NormalAttackLeft;
Animation NormalAttackDual;

Animation PowerAttackRight;
Animation PowerAttackLeft;
Animation PowerAttackDual;

Animation BlockStart;

Animation NormalAttackSprint;
Animation PowerAttackSprint1H;
Animation PowerAttackSprint2H;

void init()
{
    NormalAttackRight.type = ActionType::Idle;
    NormalAttackRight.idle = (RE::TESIdleForm *)RE::TESForm::LookupByID(0x13215);
    NormalAttackRight.action = (RE::BGSAction *)RE::TESForm::LookupByID(0x13005);
    NormalAttackRight.event = "attackStart";

    NormalAttackLeft.type = ActionType::Idle;
    NormalAttackLeft.idle = (RE::TESIdleForm *)RE::TESForm::LookupByID(0xBACC3);
    NormalAttackLeft.action = (RE::BGSAction *)RE::TESForm::LookupByID(0x13004);
    NormalAttackLeft.event = "";

    NormalAttackDual.type = ActionType::Idle;
    NormalAttackDual.idle = (RE::TESIdleForm *)RE::TESForm::LookupByID(0xBCF31);
    NormalAttackDual.action = (RE::BGSAction *)RE::TESForm::LookupByID(0x50C96);
    NormalAttackDual.event = "";

    PowerAttackRight.type = ActionType::Action;
    PowerAttackRight.idle = (RE::TESIdleForm *)RE::TESForm::LookupByID(0x19B26);
    PowerAttackRight.action = (RE::BGSAction *)RE::TESForm::LookupByID(0x13383);
    PowerAttackRight.event = "attackPowerStartInPlace";

    PowerAttackLeft.type = ActionType::Action;
    PowerAttackLeft.idle = (RE::TESIdleForm *)RE::TESForm::LookupByID(0x2E900);
    PowerAttackLeft.action = (RE::BGSAction *)RE::TESForm::LookupByID(0x2E2F6);
    PowerAttackLeft.event = "";

    PowerAttackDual.type = ActionType::Action;
    PowerAttackDual.idle = (RE::TESIdleForm *)RE::TESForm::LookupByID(0x2E901);
    PowerAttackDual.action = (RE::BGSAction *)RE::TESForm::LookupByID(0x2E2F7);
    PowerAttackDual.event = "";

    BlockStart.type = ActionType::Idle;
    BlockStart.idle = (RE::TESIdleForm *)RE::TESForm::LookupByID(0x13217);
    BlockStart.action = nullptr;
    BlockStart.event = "";

    NormalAttackSprint.type = ActionType::AniamtionEvent;
    NormalAttackSprint.idle = nullptr;
    NormalAttackSprint.action = nullptr;
    NormalAttackSprint.event = "attackStartSprint";

    PowerAttackSprint1H.type = ActionType::AniamtionEvent;
    PowerAttackSprint1H.idle = nullptr;
    PowerAttackSprint1H.action = nullptr;
    PowerAttackSprint1H.event = "attackPowerStart_Sprint";

    PowerAttackSprint2H.type = ActionType::AniamtionEvent;
    PowerAttackSprint2H.idle = nullptr;
    PowerAttackSprint2H.action = nullptr;
    PowerAttackSprint2H.event = "attackPowerStart_2HMSprint";
}
} // namespace ActionList

namespace ModSupport
{
bool ModExist(std::string modName)
{
    RE::TESDataHandler *dataHandler = RE::TESDataHandler::GetSingleton();
    RE::TESFile *modFile = nullptr;
    for (auto it = dataHandler->files.begin(); it != dataHandler->files.end(); it++)
    {
        RE::TESFile *f = *it;
        if (strcmp(f->fileName, modName.c_str()) == 0)
        {
            modFile = f;
            break;
        }
    }
    if (!modFile)
        return false;
    return true;
}
bool DLLExist(std::string DLLName)
{
    const std::string dll_dir = "Data/SKSE/Plugins/";
    for (auto entry : std::filesystem::directory_iterator(dll_dir))
        if (entry.is_regular_file() && entry.path().filename() == DLLName)
            return true;
    return false;
}
} // namespace ModSupport