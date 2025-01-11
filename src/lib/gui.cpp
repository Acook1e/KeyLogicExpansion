#include "gui.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <nlohmann/json.hpp>

namespace GUI
{

static ID3D11Device *device;

typedef struct
{
    uint32_t code;
    std::string name;
} NameMap;
static std::vector<NameMap> KeyNameMap;
static std::vector<NameMap> attackTypeName;

static std::vector<std::string> localeList;

static bool showGui = false;
bool showSettings;
static bool showCustom;
static bool showStatus;
static bool showStances;

static uint64_t tmpMouse;

class Locale
{
  public:
    Locale()
    {
        std::regex json_pattern(".*\\.json$");
        for (const auto &entry : std::filesystem::directory_iterator(localePath))
            if (std::filesystem::is_regular_file(entry.status()) &&
                std::regex_match(entry.path().filename().string(), json_pattern))
                localeList.push_back(entry.path().stem().string());
    }

    Locale(std::string locale)
    {
        std::ifstream file(localePath + locale + ".json");
        if (!file.is_open())
        {
            logger::warn("Cannot find Locale file: {}.json ,try using en_US.", locale);
            file.close();
            std::ifstream file(localePath + "en_US.json");
        }

        file >> json;
        file.close();

        buildMap();
    }

    ~Locale()
    {
    }

    std::string GetStr(const std::initializer_list<std::string> &strings)
    {
        nlohmann::json current = json;
        for (const auto &str : strings)
        {
            if (current.contains(str))
            {
                current = current.at(str);
                if (current.is_string())
                    return current.get<std::string>();
            }
            else
                return NotExist;
        }
        return NotExist;
    }

    double GetNumber(const std::initializer_list<std::string> &strings)
    {
        nlohmann::json current = json;
        for (const auto &str : strings)
        {
            if (current.contains(str))
            {
                current = current.at(str);
                if (current.is_number())
                    return current.get<double>();
            }
            else
                return 0;
        }
        return 0;
    }

    void buildMap()
    {
        KeyNameMap = {
            {0, "Disable"},
            {KeyUtils::KeyBoard::ESC, "ESC"},
            {KeyUtils::KeyBoard::Enter, "Enter"},
            {KeyUtils::KeyBoard::Space, "Space"},
            {KeyUtils::KeyBoard::LeftCtrl, "LeftCtrl"},
            {KeyUtils::KeyBoard::RightCtrl, "RightCtrl"},
            {KeyUtils::KeyBoard::LeftShift, "LeftShift"},
            {KeyUtils::KeyBoard::RightShift, "RightShift"},
            {KeyUtils::KeyBoard::LeftAlt, "LeftAlt"},
            {KeyUtils::KeyBoard::RightAlt, "RightAlt"},
            {KeyUtils::KeyBoard::Backspace, "Backspace"},
            {KeyUtils::KeyBoard::Home, "Home"},
            {KeyUtils::KeyBoard::End, "End"},
            {KeyUtils::KeyBoard::Insert, "Insert"},
            {KeyUtils::KeyBoard::PageUp, "PageUp"},
            {KeyUtils::KeyBoard::PageDown, "PageDown"},
            {KeyUtils::KeyBoard::Delete, "Delete"},
            {KeyUtils::KeyBoard::PrtSc, "PrtSc"},
            {KeyUtils::KeyBoard::Pause, "Pause"},
            {KeyUtils::KeyBoard::CapsLock, "CapsLock"},
            {KeyUtils::KeyBoard::NumLock, "NumLock"},
            {KeyUtils::KeyBoard::SrcollLock, "SrcollLock"},
            {KeyUtils::KeyBoard::Tab, "Tab"},
            {KeyUtils::KeyBoard::MAIN1, "1"},
            {KeyUtils::KeyBoard::MAIN2, "2"},
            {KeyUtils::KeyBoard::MAIN3, "3"},
            {KeyUtils::KeyBoard::MAIN4, "4"},
            {KeyUtils::KeyBoard::MAIN5, "5"},
            {KeyUtils::KeyBoard::MAIN6, "6"},
            {KeyUtils::KeyBoard::MAIN7, "7"},
            {KeyUtils::KeyBoard::MAIN8, "8"},
            {KeyUtils::KeyBoard::MAIN9, "9"},
            {KeyUtils::KeyBoard::MAIN0, "0"},
            {KeyUtils::KeyBoard::Minus, "-"},
            {KeyUtils::KeyBoard::Equals, "="},
            {KeyUtils::KeyBoard::Q, "Q"},
            {KeyUtils::KeyBoard::W, "W"},
            {KeyUtils::KeyBoard::E, "E"},
            {KeyUtils::KeyBoard::R, "R"},
            {KeyUtils::KeyBoard::T, "T"},
            {KeyUtils::KeyBoard::Y, "Y"},
            {KeyUtils::KeyBoard::U, "U"},
            {KeyUtils::KeyBoard::I, "I"},
            {KeyUtils::KeyBoard::O, "O"},
            {KeyUtils::KeyBoard::P, "P"},
            {KeyUtils::KeyBoard::A, "A"},
            {KeyUtils::KeyBoard::S, "S"},
            {KeyUtils::KeyBoard::D, "D"},
            {KeyUtils::KeyBoard::F, "F"},
            {KeyUtils::KeyBoard::G, "G"},
            {KeyUtils::KeyBoard::H, "H"},
            {KeyUtils::KeyBoard::J, "K"},
            {KeyUtils::KeyBoard::L, "L"},
            {KeyUtils::KeyBoard::Z, "Z"},
            {KeyUtils::KeyBoard::X, "X"},
            {KeyUtils::KeyBoard::C, "C"},
            {KeyUtils::KeyBoard::V, "V"},
            {KeyUtils::KeyBoard::B, "B"},
            {KeyUtils::KeyBoard::N, "N"},
            {KeyUtils::KeyBoard::M, "M"},
            {KeyUtils::KeyBoard::ArrowUp, "ArrowUp"},
            {KeyUtils::KeyBoard::ArrowDown, "ArrowDown"},
            {KeyUtils::KeyBoard::ArrowLeft, "ArrowLeft"},
            {KeyUtils::KeyBoard::ArrowRight, "ArrowRight"},
            {KeyUtils::KeyBoard::LeftBracket, "["},
            {KeyUtils::KeyBoard::RightBracket, "]"},
            {KeyUtils::KeyBoard::Console, "~"},
            {KeyUtils::KeyBoard::Semicolon, ";"},
            {KeyUtils::KeyBoard::Apostrophe, "'"},
            {KeyUtils::KeyBoard::Comma, ","},
            {KeyUtils::KeyBoard::Period, "."},
            {KeyUtils::KeyBoard::Slash, "/"},
            {KeyUtils::KeyBoard::BackSlash, "\\"},
            {KeyUtils::KeyBoard::F1, "F1"},
            {KeyUtils::KeyBoard::F2, "F2"},
            {KeyUtils::KeyBoard::F3, "F3"},
            {KeyUtils::KeyBoard::F4, "F4"},
            {KeyUtils::KeyBoard::F5, "F5"},
            {KeyUtils::KeyBoard::F6, "F6"},
            {KeyUtils::KeyBoard::F7, "F7"},
            {KeyUtils::KeyBoard::F8, "F8"},
            {KeyUtils::KeyBoard::F9, "F9"},
            {KeyUtils::KeyBoard::F10, "F10"},
            {KeyUtils::KeyBoard::F11, "F11"},
            {KeyUtils::KeyBoard::F12, "F12"},
            {KeyUtils::KeyBoard::NUM0, "NUM0"},
            {KeyUtils::KeyBoard::NUM1, "NUM1"},
            {KeyUtils::KeyBoard::NUM2, "NUM2"},
            {KeyUtils::KeyBoard::NUM3, "NUM3"},
            {KeyUtils::KeyBoard::NUM4, "NUM4"},
            {KeyUtils::KeyBoard::NUM5, "NUM5"},
            {KeyUtils::KeyBoard::NUM6, "NUM6"},
            {KeyUtils::KeyBoard::NUM7, "NUM7"},
            {KeyUtils::KeyBoard::NUM8, "NUM8"},
            {KeyUtils::KeyBoard::NUM9, "NUM9"},
            {KeyUtils::KeyBoard::NUM_Minus, "NUM-"},
            {KeyUtils::KeyBoard::NUM_Plus, "NUM+"},
            {KeyUtils::KeyBoard::NUM_star, "NUM*"},
            {KeyUtils::KeyBoard::NUM_Slash, "NUM/"},
            {KeyUtils::KeyBoard::NUM_Dot, "NUM."},
            {KeyUtils::KeyBoard::NUM_Enter, "NUM_Enter"},
            {KeyUtils::Mouse::MouseButtonLeft, "MouseButtonLeft"},
            {KeyUtils::Mouse::MouseButtonRight, "MouseButtonRight"},
            {KeyUtils::Mouse::MouseButtonMiddle, "MouseButtonMiddle"},
            {KeyUtils::Mouse::MouseButton3, "M4"},
            {KeyUtils::Mouse::MouseButton4, "M5"},
            {KeyUtils::Mouse::MouseWheelUp, "MouseWheelUp"},
            {KeyUtils::Mouse::MouseWheelDown, "MouseWheelDown"},
            {1000000, "None"},
        };

        attackTypeName = {
            {0, this->GetStr({"AttackTypeName", "None"})},       {1, this->GetStr({"AttackTypeName", "RightHand"})},
            {2, this->GetStr({"AttackTypeName", "LeftHand"})},   {3, this->GetStr({"AttackTypeName", "DualHand"})},
            {4, this->GetStr({"AttackTypeName", "VanillaLMB"})}, {5, this->GetStr({"AttackTypeName", "VanillaRMB"})}};
    }

  private:
    static std::string localePath;
    static std::string NotExist;
    nlohmann::json json;
};
std::string Locale::localePath = "Data/SKSE/Plugins/KeyLogicExpansion/Locale/";
std::string Locale::NotExist = "Value Not Exist";
static Locale locale;

const char *GetNameByKey(uint32_t code)
{
    auto res = std::find_if(KeyNameMap.begin(), KeyNameMap.end(), [code](NameMap map) { return code == map.code; });
    if (res == KeyNameMap.end())
        return "None";
    else
        return res->name.data();
}

void SwitchButton(const char *name, bool &value, const char *description = "")
{
    ImGui::Checkbox(name, &value);
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip(description);
}

void SelectLanguage(const char *name, std::string &language, const char *description = nullptr)
{
    if (ImGui::BeginCombo(name, language.data()))
    {
        for (auto item : localeList)
        {
            bool is_selected = (language == item);
            if (ImGui::Selectable(item.data(), is_selected))
            {
                language = item;
                showGui = false;
                locale = Locale(language);
                showGui = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (description && ImGui::IsItemHovered())
        ImGui::SetTooltip(description);
}

void SelectButton(const char *name, uint32_t &code, const char *description = nullptr)
{
    if (ImGui::BeginCombo(name, GetNameByKey(code)))
    {
        for (auto item : KeyNameMap)
        {
            bool is_selected = (code == item.code);
            if (ImGui::Selectable(item.name.data(), is_selected))
                code = item.code;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (description && ImGui::IsItemHovered())
        ImGui::SetTooltip(description);
}

void SelectAttackType(const char *name, Style::AttackType &type, const char *description = nullptr)
{
    auto res =
        std::find_if(attackTypeName.begin(), attackTypeName.end(), [type](NameMap map) { return type == map.code; });
    if (ImGui::BeginCombo(name, res->name.data()))
    {
        for (auto item : attackTypeName)
        {
            bool is_selected = (type == item.code);
            if (ImGui::Selectable(item.name.data(), is_selected))
                type = (Style::AttackType)item.code;
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (description && ImGui::IsItemHovered())
        ImGui::SetTooltip(description);
}
void IntText(const char *name, uint32_t &value, const char *description = nullptr, uint32_t min = 0U,
             uint32_t max = (1U << 31) - 1, int step = 1, int step_fast = 10)
{
    if (ImGui::InputInt(name, (int *)&value, step, step_fast))
    {
        if (value <= min)
            value = min;
        else if (value >= max)
            value = max;
    }
    if (description && ImGui::IsItemHovered())
        ImGui::SetTooltip(description);
}

typedef struct
{
    ID3D11ShaderResourceView *srv;
    float width;
    float height;
} Image;
Image GetImage(std::string image)
{
    int width, height, channels;
    stbi_uc *data = stbi_load(image.data(), &width, &height, &channels, 0);
    ID3D11ShaderResourceView *pSRV = nullptr;
    if (data)
    {
        D3D11_TEXTURE2D_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA subResource;
        subResource.pSysMem = data;
        subResource.SysMemPitch = width * channels;
        subResource.SysMemSlicePitch = 0;

        ID3D11Texture2D *pTexture = nullptr;
        HRESULT hr = device->CreateTexture2D(&desc, &subResource, &pTexture);
        if (SUCCEEDED(hr))
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
            ZeroMemory(&srvDesc, sizeof(srvDesc));
            srvDesc.Format = desc.Format;
            srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = desc.MipLevels;
            srvDesc.Texture2D.MostDetailedMip = 0;
            hr = device->CreateShaderResourceView(pTexture, &srvDesc, &pSRV);
            pTexture->Release();
        }
        stbi_image_free(data);
    }
    if (pSRV)
        return Image{pSRV, (float)width, (float)height};
    logger::error("Cannot load image:{}", image);
    return Image{nullptr, 0, 0};
}

void KeyBindSettings()
{
    ImGui::Begin("KeyBind Settings", &showSettings, ImGuiWindowFlags_NoCollapse);

    if (ImGui::Button(locale.GetStr({"Save", "name"}).data()))
    {
        Config::saveInI();
        ImGui::Text(locale.GetStr({"Save", "description"}).data());
    }

    SelectLanguage(locale.GetStr({"Language", "name"}).data(), Config::locale,
                   locale.GetStr({"Language", "description"}).data());

    SwitchButton(locale.GetStr({"ShowCustomSettings", "name"}).data(), showCustom,
                 locale.GetStr({"ShowCustomSettings", "description"}).data());
    SwitchButton(locale.GetStr({"ShowStatusSettings", "name"}).data(), showStatus,
                 locale.GetStr({"ShowStatusSettings", "description"}).data());
    SwitchButton(locale.GetStr({"ShowCurrentStance", "name"}).data(), showStances);
    ImGui::Spacing();
    if (ImGui::TreeNode(locale.GetStr({"Nodes", "Features"}).data()))
    {
        SwitchButton(locale.GetStr({"EnableCustomInput", "name"}).data(), Custom::enableCustomInput,
                     locale.GetStr({"EnableCustomInput", "description"}).data());
        SwitchButton(locale.GetStr({"EnableStances", "name"}).data(), Stances::enableStances,
                     locale.GetStr({"EnableStances", "description"}).data());
        SwitchButton(locale.GetStr({"EnableHoldSprint", "name"}).data(), Config::enableHoldSprint,
                     locale.GetStr({"EnableHoldSprint", "description"}).data());
        SwitchButton(locale.GetStr({"EnableHoldSneak", "name"}).data(), Config::enableHoldSneak,
                     locale.GetStr({"EnableHoldSneak", "description"}).data());
        SwitchButton(locale.GetStr({"EnableSprintAttack", "name"}).data(), Config::enableSprintAttack,
                     locale.GetStr({"EnableSprintAttack", "description"}).data());
        SwitchButton(locale.GetStr({"EnableReverseHorseAttack", "name"}).data(), Config::enableReverseHorseAttack,
                     locale.GetStr({"EnableReverseHorseAttack", "description"}).data());
        SelectButton(locale.GetStr({"EnableSheatheAttack", "name"}).data(), Config::enableSheatheAttack,
                     locale.GetStr({"EnableSheatheAttack", "description"}).data());
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(locale.GetStr({"Nodes", "Stances"}).data()))
    {
        SelectButton(locale.GetStr({"StancesModifier", "name"}).data(), Stances::StancesModfier,
                     locale.GetStr({"StancesModifier", "description"}).data());
        SelectButton(locale.GetStr({"ChangeToLow", "name"}).data(), Stances::ChangeToLow,
                     locale.GetStr({"ChangeToLow", "description"}).data());
        SelectButton(locale.GetStr({"ChangeToMid", "name"}).data(), Stances::ChangeToMid,
                     locale.GetStr({"ChangeToMid", "description"}).data());
        SelectButton(locale.GetStr({"ChangeToHigh", "name"}).data(), Stances::ChangeToHigh,
                     locale.GetStr({"ChangeToHigh", "description"}).data());
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(locale.GetStr({"Nodes", "Vanilla"}).data()))
    {
        SelectButton(locale.GetStr({"NormalAttack", "name"}).data(), Config::normalAttack,
                     locale.GetStr({"NormalAttack", "description"}).data());
        SelectButton(locale.GetStr({"PowerAttack", "name"}).data(), Config::powerAttack,
                     locale.GetStr({"PowerAttack", "description"}).data());
        SelectButton(locale.GetStr({"OtherAttack", "name"}).data(), Config::otherAttack,
                     locale.GetStr({"OtherAttack", "description"}).data());
        SelectButton(locale.GetStr({"Block", "name"}).data(), Config::block,
                     locale.GetStr({"Block", "description"}).data());
        ImGui::Spacing();
        SelectButton(locale.GetStr({"AltTweenMenu", "name"}).data(), Config::altTweenMenu,
                     locale.GetStr({"AltTweenMenu", "description"}).data());
        SelectButton(locale.GetStr({"AltTogglePOV", "name"}).data(), Config::altTogglePOV,
                     locale.GetStr({"AltTogglePOV", "description"}).data());
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(locale.GetStr({"Nodes", "Style"}).data()))
    {
        IntText(locale.GetStr({"PreInputTime", "name"}).data(), Style::styleMap[Style::currentStyle].preInputTime,
                locale.GetStr({"PreInputTime", "description"}).data(), 20, 1000, 10, 100);
        IntText(locale.GetStr({"IntervalTime", "name"}).data(), Style::styleMap[Style::currentStyle].intervalTime,
                locale.GetStr({"IntervalTime", "description"}).data(), 10, 200, 5, 10);
        ImGui::Spacing();
        ImGui::Text((locale.GetStr({"CurrentStyle", "name"}) +
                     locale.GetStr({"Styles", Style::GetStyleName(Style::currentStyle)}))
                        .data());
        SelectAttackType(locale.GetStr({"NormalAttackType", "name"}).data(),
                         Style::styleMap[Style::currentStyle].normalAttackType,
                         locale.GetStr({"NormalAttackType", "description"}).data());
        SwitchButton(locale.GetStr({"RepeatNormalAttack", "name"}).data(),
                     Style::styleMap[Style::currentStyle].repeatNormalAttack,
                     locale.GetStr({"RepeatNormalAttack", "description"}).data());
        SwitchButton(locale.GetStr({"SheatheNormalAttack", "name"}).data(),
                     Style::styleMap[Style::currentStyle].sheatheNormalAttack,
                     locale.GetStr({"SheatheNormalAttack", "description"}).data());
        ImGui::Spacing();
        SelectAttackType(locale.GetStr({"PowerAttackType", "name"}).data(),
                         Style::styleMap[Style::currentStyle].powerAttackType,
                         locale.GetStr({"PowerAttackType", "description"}).data());
        SwitchButton(locale.GetStr({"RepeatPowerAttack", "name"}).data(),
                     Style::styleMap[Style::currentStyle].repeatPowerAttack,
                     locale.GetStr({"RepeatPowerAttack", "description"}).data());
        SwitchButton(locale.GetStr({"SheathePowerAttack", "name"}).data(),
                     Style::styleMap[Style::currentStyle].sheathePowerAttack,
                     locale.GetStr({"SheathePowerAttack", "description"}).data());
        ImGui::Spacing();
        SelectAttackType(locale.GetStr({"OtherAttackType", "name"}).data(),
                         Style::styleMap[Style::currentStyle].otherAttackType,
                         locale.GetStr({"OtherAttackType", "description"}).data());
        SwitchButton(locale.GetStr({"SheatheOtherAttack", "name"}).data(),
                     Style::styleMap[Style::currentStyle].sheatheOtherAttack,
                     locale.GetStr({"SheatheOtherAttack", "description"}).data());
        ImGui::Spacing();
        SwitchButton(locale.GetStr({"IsAltTypeEnable", "name"}).data(),
                     Style::styleMap[Style::currentStyle].isAltTypeEnable,
                     locale.GetStr({"IsAltTypeEnable", "description"}).data());
        SwitchButton(locale.GetStr({"IsUsingHold", "name"}).data(), Style::styleMap[Style::currentStyle].isUsingHold,
                     locale.GetStr({"IsUsingHold", "description"}).data());
        SelectButton(locale.GetStr({"AltModifier", "name"}).data(),
                     Style::styleMap[Style::currentStyle].attackTypeModifier,
                     locale.GetStr({"AltModifier", "description"}).data());
        SelectAttackType(locale.GetStr({"AltNormalAttackType", "name"}).data(),
                         Style::styleMap[Style::currentStyle].altNormalAttackType,
                         locale.GetStr({"AltNormalAttackType", "description"}).data());
        SelectAttackType(locale.GetStr({"AltPowerAttackType", "name"}).data(),
                         Style::styleMap[Style::currentStyle].altPowerAttackType,
                         locale.GetStr({"AltPowerAttackType", "description"}).data());
        SelectAttackType(locale.GetStr({"AltOtherAttackType", "name"}).data(),
                         Style::styleMap[Style::currentStyle].altOtherAttackType,
                         locale.GetStr({"AltOtherAttackType", "description"}).data());
        ImGui::TreePop();
    }
    if (Compatibility::BFCO)
    {
        if (ImGui::TreeNode(locale.GetStr({"Nodes", "BFCO"}).data()))
        {
            SelectButton(locale.GetStr({"BFCO_SpecialAttackModifier", "name"}).data(),
                         Config::BFCO_SpecialAttackModifier,
                         locale.GetStr({"BFCO_SpecialAttackModifier", "description"}).data());
            SelectButton(locale.GetStr({"BFCO_ComboAttack", "name"}).data(), Config::BFCO_ComboAttack,
                         locale.GetStr({"BFCO_ComboAttack", "description"}).data());
            ImGui::TreePop();
        }
    }

    if (ImGui::TreeNode(locale.GetStr({"Nodes", "Expand"}).data()))
    {
        SelectButton(locale.GetStr({"WarAshModifier", "name"}).data(), Config::warAshModifier,
                     locale.GetStr({"WarAshModifier", "description"}).data());
        SelectButton(locale.GetStr({"WarAsh", "name"}).data(), Config::warAsh,
                     locale.GetStr({"WarAsh", "description"}).data());
        ImGui::Spacing();
        SelectButton(locale.GetStr({"ZoomModifier", "name"}).data(), Config::zoomModifier,
                     locale.GetStr({"ZoomModifier", "description"}).data());
        SelectButton(locale.GetStr({"AltZoomIn", "name"}).data(), Config::altZoomIn,
                     locale.GetStr({"AltZoomIn", "description"}).data());
        SelectButton(locale.GetStr({"AltZoomOut", "name"}).data(), Config::altZoomOut,
                     locale.GetStr({"AltZoomOut", "description"}).data());
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(locale.GetStr({"Nodes", "Modifier"}).data()))
    {
        SelectButton(VarUtils::userEvent->forward.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->forward)]);
        SelectButton(VarUtils::userEvent->strafeLeft.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->strafeLeft)]);
        SelectButton(VarUtils::userEvent->back.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->back)]);
        SelectButton(VarUtils::userEvent->strafeRight.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->strafeRight)]);
        SelectButton(VarUtils::userEvent->rightAttack.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->rightAttack)]);
        SelectButton(VarUtils::userEvent->leftAttack.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->leftAttack)]);
        SelectButton(VarUtils::userEvent->activate.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->activate)]);
        SelectButton(VarUtils::userEvent->readyWeapon.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->readyWeapon)]);
        SelectButton(VarUtils::userEvent->tweenMenu.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->tweenMenu)]);
        SelectButton(VarUtils::userEvent->togglePOV.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->togglePOV)]);
        SelectButton(VarUtils::userEvent->jump.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->jump)]);
        SelectButton(VarUtils::userEvent->sprint.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sprint)]);
        SelectButton(VarUtils::userEvent->shout.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->shout)]);
        SelectButton(VarUtils::userEvent->sneak.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->sneak)]);
        SelectButton(VarUtils::userEvent->run.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->run)]);
        SelectButton(VarUtils::userEvent->toggleRun.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->toggleRun)]);
        SelectButton(VarUtils::userEvent->autoMove.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->autoMove)]);
        SelectButton(VarUtils::userEvent->favorites.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->favorites)]);
        SelectButton(VarUtils::userEvent->quicksave.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quicksave)]);
        SelectButton(VarUtils::userEvent->quickload.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickload)]);
        SelectButton(VarUtils::userEvent->wait.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->wait)]);
        SelectButton(VarUtils::userEvent->journal.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->journal)]);
        SelectButton(VarUtils::userEvent->pause.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->pause)]);
        SelectButton(VarUtils::userEvent->quickInventory.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickInventory)]);
        SelectButton(VarUtils::userEvent->quickMagic.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickMagic)]);
        SelectButton(VarUtils::userEvent->quickStats.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickStats)]);
        SelectButton(VarUtils::userEvent->quickMap.data(),
                     Config::needModifier[KeyUtils::GetVanillaKeyMap(VarUtils::userEvent->quickMap)]);
        ImGui::TreePop();
    }
    ImGui::End();
}

static std::vector<Image> StancesImage;
static std::string StancesImage_dir = "Data/SKSE/Plugins/KeyLogicExpansion/Stances/";
void ShowStances()
{
    auto image = StancesImage[Stances::currentStance];
    ImGui::Image((ImTextureID)image.srv, ImVec2(image.width, image.height));
}
void ShowStatus()
{
    float alpha = 0;
    ImGuiWindowFlags flag = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground;
    if (showSettings)
    {
        alpha = 0.3f;
        flag = ImGuiWindowFlags_NoCollapse;
    }
    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowBorderSize = 0.0f;
    ImGui::SetNextWindowBgAlpha(alpha);
    ImGui::Begin("Status", &showStatus, flag);
    if (showStances)
        ShowStances();
    ImGui::End();
    style.WindowBorderSize = 1.0f;
}

class Win32Hook
{
  public:
    static ATOM RegisterClassA(WNDCLASSA *a_wndClass)
    {
        FnWP = reinterpret_cast<uintptr_t>(a_wndClass->lpfnWndProc);
        a_wndClass->lpfnWndProc = &WND_PROC;
        return FnRC(a_wndClass);
    }

    static LRESULT WND_PROC(const HWND h_wnd, const UINT u_msg, const WPARAM w_param, const LPARAM l_param)
    {
        switch (u_msg)
        {
        case WM_KILLFOCUS:
            showGui = false;
            break;
        case WM_SETFOCUS:
            showGui = true;
            break;
        }
        return FnWP(h_wnd, u_msg, w_param, l_param);
    }

    static void Hook()
    {
        auto &trampoline = SKSE::GetTrampoline();
        SKSE::AllocTrampoline(8);
        const REL::Relocation<uintptr_t> registerWindowHook{REL::VariantID(75591, 77226, 0xDC4B90)};
        FnRC = *(uintptr_t *)trampoline.write_call<6>(
            registerWindowHook.address() + REL::VariantOffset(0x8E, 0x15C, 0x99).offset(), RegisterClassA);
    }

  private:
    static inline REL::Relocation<decltype(RegisterClassA)> FnRC;
    static inline REL::Relocation<decltype(WND_PROC)> FnWP;
};

class DX11Hook
{
  public:
    static void CreateD3D11()
    {
        FnCD11();

        const auto renderManager = RE::BSGraphics::Renderer::GetSingleton();

        device = reinterpret_cast<ID3D11Device *>(renderManager->data.forwarder);
        auto context = reinterpret_cast<ID3D11DeviceContext *>(renderManager->data.context);
        auto swapChain = reinterpret_cast<IDXGISwapChain *>(renderManager->data.renderWindows[0].swapChain);

        DXGI_SWAP_CHAIN_DESC sd{};
        swapChain->GetDesc(&sd);
        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = {static_cast<float>(sd.BufferDesc.Width), static_cast<float>(sd.BufferDesc.Height)};
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.BackendFlags = ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_RendererHasVtxOffset;

        ImGui_ImplWin32_Init(sd.OutputWindow);
        ImGui_ImplDX11_Init(device, context);
        ImGui::StyleColorsDark();

        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile((fontPath + "Font.ini").data());
        std::string font = ini.GetValue("Locale", "Font");
        float fontSize = ini.GetDoubleValue("Locale", "FontSize");
        std::string glyph = ini.GetValue("Locale", "Glyph");
        const ImWchar *range;
        if (glyph == "Chinese")
            range = io.Fonts->GetGlyphRangesChineseFull();
        else if (glyph == "Japanese")
            range = io.Fonts->GetGlyphRangesJapanese();
        else if (glyph == "Korean")
            range = io.Fonts->GetGlyphRangesKorean();
        else if (glyph == "Greek")
            range = io.Fonts->GetGlyphRangesGreek();
        else if (glyph == "Cyrillic")
            range = io.Fonts->GetGlyphRangesCyrillic();
        else if (glyph == "Thai")
            range = io.Fonts->GetGlyphRangesThai();
        else if (glyph == "Vietnamese")
            range = io.Fonts->GetGlyphRangesVietnamese();
        else
            range = io.Fonts->GetGlyphRangesDefault();
        io.Fonts->AddFontFromFileTTF((fontPath + font).data(), fontSize, nullptr, range);
    }

    static void Present(uint32_t a1)
    {
        FnPr(a1);

        if (showGui)
        {
            ImGui_ImplWin32_NewFrame();
            ImGui_ImplDX11_NewFrame();
            ImGui::NewFrame();
            ImGuiIO &io = ImGui::GetIO();
            io.MouseDrawCursor = showSettings;
            if (showSettings)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
                KeyBindSettings();
            }
            if (showStatus)
                ShowStatus();
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    }

    static void Hook()
    {
        auto &trampoline = SKSE::GetTrampoline();
        SKSE::AllocTrampoline(14);
        const REL::Relocation<uintptr_t> created3d11Hook{REL::VariantID(75595, 77226, 0xDC5530)};
        FnCD11 = trampoline.write_call<5>(created3d11Hook.address() + REL::VariantOffset(0x9, 0x275, 0x9).offset(),
                                          CreateD3D11);

        const REL::Relocation<uintptr_t> presentHook{REL::VariantID(75461, 77246, 0xDBBDD0)};
        SKSE::AllocTrampoline(14);
        FnPr = trampoline.write_call<5>(presentHook.address() + REL::VariantOffset(0x9, 0x9, 0x15).offset(), Present);
    }

  private:
    static inline REL::Relocation<decltype(CreateD3D11)> FnCD11;
    static inline REL::Relocation<decltype(Present)> FnPr;
    static std::string fontPath;
};
std::string DX11Hook::fontPath = "Data/SKSE/Plugins/KeyLogicExpansion/Fonts/";

class InputHook
{
  public:
    static void ProcessEvent(RE::BSTEventSource<RE::InputEvent *> *a_dispatcher, RE::InputEvent *const *a_events)
    {
        if (showGui && showSettings)
        {
            ImGuiIO &io = ImGui::GetIO();
            io.MouseDown[0] = GetKeyState(VK_LBUTTON) & 0x8000;
            if (KeyUtils::MouseWheelTime > tmpMouse)
            {
                tmpMouse = KeyUtils::MouseWheelTime;
                io.AddMouseWheelEvent(0, KeyUtils::MouseWheelStatus);
            }
        }
        FnPE(a_dispatcher, a_events);
    }
    static void Hook()
    {
        const REL::Relocation<uintptr_t> inputHook{REL::VariantID(67315, 68617, 0xC519E0)};
        auto &trampoline = SKSE::GetTrampoline();
        SKSE::AllocTrampoline(14);
        FnPE =
            trampoline.write_call<5>(inputHook.address() + REL::VariantOffset(0x7B, 0x7B, 0x81).offset(), ProcessEvent);
    }

  private:
    static inline REL::Relocation<decltype(ProcessEvent)> FnPE;
};

void init()
{
    Win32Hook::Hook();
    DX11Hook::Hook();
    InputHook::Hook();
}
void showGUI()
{
    locale = Locale(Config::locale);
    StancesImage.push_back(GetImage(StancesImage_dir + "Stances-Null.png"));
    StancesImage.push_back(GetImage(StancesImage_dir + "Stances-Low.png"));
    StancesImage.push_back(GetImage(StancesImage_dir + "Stances-Mid.png"));
    StancesImage.push_back(GetImage(StancesImage_dir + "Stances-High.png"));
    StancesImage.push_back(GetImage(StancesImage_dir + "Stances-Sheathe.png"));
    showGui = true;
    showSettings = false;
    showCustom = false;
    showStatus = true;
    showStances = Stances::enableStances;
}
} // namespace GUI