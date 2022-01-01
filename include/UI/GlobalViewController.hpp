#pragma once

#include "HMUI/ViewController.hpp"
#include "UI/Other/PlayerProfileModal.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(ScoreSaber::UI, GlobalViewController, HMUI::ViewController,
                      DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::ViewController::DidActivate>::get(), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
                      DECLARE_INSTANCE_FIELD(ScoreSaber::UI::PlayerProfileModal*, playerProfileModal);
                      DECLARE_INSTANCE_FIELD(HMUI::ModalView*, moreInfoModal);

                      void OpenMoreInfoModal();

)