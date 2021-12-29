#pragma once

#include "UnityEngine/Color.hpp"
#include "UnityEngine/EventSystems/PointerEventData.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"

using namespace UnityEngine;
namespace ScoreSaberUI {
namespace Utils {
namespace UIUtils {
UnityEngine::UI::HorizontalLayoutGroup* CreateHeader(
    UnityEngine::Transform* parent, UnityEngine::Color color);
}  // namespace UIUtils
}  // namespace Utils
}  // namespace ScoreSaberUI