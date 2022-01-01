#pragma once

#include "CustomTypes/Components/ImageButton.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/EventSystems/PointerEventData.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"

using namespace UnityEngine;
namespace ScoreSaberUI {
namespace Utils {
namespace UIUtils {
UnityEngine::UI::HorizontalLayoutGroup* CreateHeader(
    UnityEngine::Transform* parent, UnityEngine::Color color);

ScoreSaberUI::CustomTypes::Components::ImageButton* CreateImageButton(
    GameObject* parent, Sprite* sprite, Vector2 anchoredPosition,
    Vector2 sizeDelta, std::function<void()> onClick);
}  // namespace UIUtils
}  // namespace Utils
}  // namespace ScoreSaberUI