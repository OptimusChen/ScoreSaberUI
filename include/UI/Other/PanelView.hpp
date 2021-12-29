#pragma once

#include "CustomTypes/CustomLeaderboardPlatformHandler.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Screen.hpp"
#include "HMUI/ViewController.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/util.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "questui/shared/QuestUI.hpp"


using namespace UnityEngine;
using namespace GlobalNamespace;

DECLARE_CLASS_CODEGEN(
    ScoreSaberUI::UI::Other, PanelView, HMUI::ViewController,
    DECLARE_INSTANCE_METHOD(void, Init, PlatformLeaderboardViewController*);
    DECLARE_INSTANCE_METHOD(void, Show);
    DECLARE_INSTANCE_FIELD(PlatformLeaderboardViewController*, leaderboardView);
    DECLARE_INSTANCE_FIELD(HMUI::Screen*, floatingScreen);)