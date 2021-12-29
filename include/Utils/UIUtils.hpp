#pragma once

#include "UnityEngine/Color.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"

using namespace UnityEngine;

namespace ScoreSaberUI {
namespace Utils {
namespace UIUtils {
UnityEngine::UI::HorizontalLayoutGroup* CreateHeader(
    UnityEngine::Transform* parent, UnityEngine::Color color);
}
}  // namespace Utils
}  // namespace ScoreSaberUI