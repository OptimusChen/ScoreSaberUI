#pragma once

#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "HMUI/ModalView.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/types.hpp"
#include "custom-types/shared/util.hpp"

using namespace UnityEngine;
using namespace GlobalNamespace;

DECLARE_CLASS_CODEGEN(ScoreSaberUI::UI::Other, ScoreSaberLeaderboardView,
                      UnityEngine::MonoBehaviour,
                      DECLARE_CTOR(ctor, PlatformLeaderboardViewController);
                      DECLARE_INSTANCE_FIELD(HMUI::ModalView*, infoModal);
                      DECLARE_INSTANCE_FIELD(UI::Button*, playButton);
                      DECLARE_INSTANCE_METHOD(void, SetPlayButtonState, bool);
                      DECLARE_INSTANCE_METHOD(void, ShowInfoModal, int););