#pragma once
#include "PCH.h"

namespace VarUtils
{
extern const SKSE::SerializationInterface *cosave;
extern RE::Main *main;
extern RE::PlayerCharacter *player;
extern RE::PlayerControls *pCtrl;
extern RE::ControlMap *ctrlMap;
extern RE::UserEvents *userEvent;
extern RE::UI *ui;

void init();
} // namespace VarUtils

namespace KeyUtils
{
extern bool TracingMouse;
extern uint64_t MouseWheelTime;
extern float MouseWheelStatus;

enum INPUT_DEVICE
{
    KeyBoard = 0,
    Mouse,
    Gamepad,
    Total
};

enum KeyBoard
{
    ESC = 1,
    MAIN1,
    MAIN2,
    MAIN3,
    MAIN4,
    MAIN5,
    MAIN6,
    MAIN7,
    MAIN8,
    MAIN9,
    MAIN0,
    Minus,  // -
    Equals, // =
    Backspace,
    Tab,
    Q,
    W,
    E,
    R,
    T,
    Y,
    U,
    I,
    O,
    P,
    LeftBracket,  // [
    RightBracket, // ]
    Enter,
    LeftCtrl,
    A,
    S,
    D,
    F,
    G,
    H,
    J,
    K,
    L,
    Semicolon,  // ;
    Apostrophe, // '
    Console,    // ~
    LeftShift,
    BackSlash, /* \ */
    Z,
    X,
    C,
    V,
    B,
    N,
    M,
    Comma,  //,
    Period, //.
    Slash,  // /
    RightShift,
    NUM_star, // NUM*
    LeftAlt,
    Space,
    CapsLock,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    NumLock,
    SrcollLock,
    NUM7,
    NUM8,
    NUM9,
    NUM_Minus, // NUM-
    NUM4,
    NUM5,
    NUM6,
    NUM_Plus, // NUM+
    NUM1,
    NUM2,
    NUM3,
    NUM0,
    NUM_Dot, // NUM.

    F11 = 87,
    F12,

    NUM_Enter = 156,
    RightCtrl,

    NUM_Slash = 181,

    PrtSc = 183, // Print Srceen or Sys Rq
    RightAlt,

    Pause = 197,

    Home = 199,
    ArrowUp, // ↑
    PageUp,

    ArrowLeft = 203, // ←

    ArrowRight = 205, // →

    End = 207,
    ArrowDown, // ↓
    PageDown,
    Insert,
    Delete
};

enum Mouse
{
    // first 256 for keyboard, then 8 mouse buttons, then mouse wheel up, wheel down, then 16 gamepad buttons
    MouseButtonOffset = 256,
    MouseButtonLeft = 256,
    MouseButtonRight,
    MouseButtonMiddle,
    MouseButton3,
    MouseButton4,
    MouseButton5,
    MouseButton6,
    MouseButton7,
    MouseWheelUp,
    MouseWheelDown, // 265
};

enum GamePad
{
    GamepadButtonOffset = 266,
    GamepadButtonDpadUp = 266,
    GamepadButtonDpadDown,
    GamepadButtonDpadLfet,
    GamepadButtonDpadRight,
    GamepadButtonStart,
    GamepadButtonBack,
    GamepadButtonLeftThumb,
    GamepadButtonRightThumb,
    GamepadButtonLeftShoulder,
    GamepadButtonRightShoulder,
    GamepadButtonA,
    GamepadButtonB,
    GamepadButtonX,
    GamepadButtonY,
    GamepadButtonLT,
    GamepadButtonRT // 281
};

extern uint32_t deleteTime;
bool GetKeyState(uint32_t code);

uint32_t GetVanillaKeyMap(RE::BSFixedString userEvent);
uint32_t GetEventKeyMap(RE::InputEvent *a_event);
uint32_t GetEventKeyMap(RE::ButtonEvent *evn);
} // namespace KeyUtils

namespace TimeUtils
{
uint64_t GetTime();
}

namespace FormUtils
{
RE::TESForm *GetForm(RE::FormID FormId);
RE::TESForm *GetForm(std::string modForm);
} // namespace FormUtils

namespace GameStatus
{
bool isGameRunning();
} // namespace GameStatus

namespace PlayerStatus
{
extern bool dodge;

bool IsAttacking();
bool IsBlocking();
bool IsSheathe();
bool IsBashing();
bool IsMoving();
bool IsSprinting();
bool IsSneaking();
bool IsJumping();
bool IsDodging();
bool IsRiding();
bool IsSwiming();
bool IsInKillmove();
} // namespace PlayerStatus

namespace ActionList
{
enum ActionType
{
    Idle = 0,
    Action,
    AniamtionEvent
};
typedef struct
{
    ActionType type;
    RE::TESIdleForm *idle;
    RE::BGSAction *action;
    std::string event;
} Animation;

extern Animation NormalAttackRight;
extern Animation NormalAttackLeft;
extern Animation NormalAttackDual;

extern Animation PowerAttackRight;
extern Animation PowerAttackLeft;
extern Animation PowerAttackDual;

extern Animation BlockStart;

extern Animation NormalAttackSprint;
extern Animation PowerAttackSprint1H;
extern Animation PowerAttackSprint2H;

void init();
} // namespace ActionList

namespace ModSupport
{
bool ModExist(std::string modName);
bool DLLExist(std::string DLLName);
} // namespace ModSupport