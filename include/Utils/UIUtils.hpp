#pragma once

#include "UnityEngine/Color.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"

using namespace UnityEngine;

namespace ScoreSaberUI {
namespace Utils {
namespace UIUtils {
UnityEngine::UI::VerticalLayoutGroup* CreateHeader(
    UnityEngine::Transform* parent, std::string title, Color color);
}
}  // namespace Utils
}  // namespace ScoreSaberUI