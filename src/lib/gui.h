#pragma once
#include "compatibility.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_internal.h"

namespace GUI
{
extern bool showSettings;

void init();
void showGUI();
} // namespace GUI
