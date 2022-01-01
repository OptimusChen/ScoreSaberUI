#pragma once

#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/ViewController.hpp"
#include "UI/GlobalViewController.hpp"
#include "UI/LinksViewController.hpp"
#include "UI/TeamAndContributorsViewController.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(ScoreSaber::UI, MainMenuFlowCoordinator, HMUI::FlowCoordinator,
                      DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::FlowCoordinator::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
                      DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::FlowCoordinator::BackButtonWasPressed>::get(), HMUI::ViewController* topViewController);
                      DECLARE_INSTANCE_FIELD_DEFAULT(ScoreSaber::UI::GlobalViewController*, globalViewController, nullptr);
                      DECLARE_INSTANCE_FIELD_DEFAULT(ScoreSaber::UI::LinksViewController*, linksViewController, nullptr);
                      DECLARE_INSTANCE_FIELD_DEFAULT(ScoreSaber::UI::TeamAndContributorsViewController*, teamAndContributorsViewController, nullptr);

)
