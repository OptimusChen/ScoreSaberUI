#pragma once

#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"
#include "UI/ViewControllers/FAQViewController.hpp"
#include "UI/ViewControllers/GlobalViewController.hpp"
#include "UI/ViewControllers/TeamViewController.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(
    ScoreSaberUI::UI::FlowCoordinators, ScoreSaberFlowCoordinator,
    HMUI::FlowCoordinator,
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, teamView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, globalView);
    DECLARE_INSTANCE_FIELD(HMUI::ViewController*, faqView);
    DECLARE_OVERRIDE_METHOD(void, DidActivate,
                            il2cpp_utils::FindMethodUnsafe("HMUI",
                                                           "FlowCoordinator",
                                                           "DidActivate", 3),
                            bool firstActivation, bool addedToHierarchy,
                            bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(
        void, BackButtonWasPressed,
        il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator",
                                       "BackButtonWasPressed", 1),
        HMUI::ViewController* topViewController););