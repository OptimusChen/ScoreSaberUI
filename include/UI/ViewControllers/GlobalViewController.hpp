#pragma once
#include "CustomTypes/Components/CustomCellListTableData.hpp"
#include "HMUI/ViewController.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(
    ScoreSaberUI::UI::ViewControllers, GlobalViewController,
    HMUI::ViewController,
    DECLARE_OVERRIDE_METHOD(void, DidActivate,
                            il2cpp_utils::FindMethodUnsafe("HMUI",
                                                           "ViewController",
                                                           "DidActivate", 3),
                            bool firstActivation, bool addedToHierarchy,
                            bool screenSystemEnabling);
    DECLARE_INSTANCE_FIELD(ScoreSaberUI::CustomTypes::Components::CustomCellListTableData*, leaderboardList);
    private
    :

    void UpButtonWasPressed();
    void DownButtonWasPressed();

);