#pragma once

#include "HMUI/ImageView.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"
#include <functional>

DECLARE_CLASS_CODEGEN(ScoreSaber::UI, DelayedCreateImage, UnityEngine::MonoBehaviour,
                      public
                      : static HMUI::ImageView* CreateImage(UnityEngine::Transform* parent, std::string_view url, std::function<void(HMUI::ImageView*)> finished);
                      static custom_types::Helpers::Coroutine WaitForImage(HMUI::ImageView * parent, std::string_view url, std::function<void(HMUI::ImageView*)> finished);

)