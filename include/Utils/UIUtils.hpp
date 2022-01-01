#pragma once

#include "CustomTypes/ClickableImage.hpp"
#include "CustomTypes/ClickableText.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "Utils/TeamUtils.hpp"
#include <functional>
#include <string_view>

namespace UIUtils
{
    ScoreSaber::ClickableText* CreateClickableText(UnityEngine::Transform* parent, std::u16string_view text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick);
    ScoreSaber::ClickableText* CreateClickableText(UnityEngine::Transform* parent, std::u16string_view text, std::function<void()> onClick);
    ScoreSaber::ClickableText* CreateClickableText(UnityEngine::Transform* parent, std::u16string_view text, UnityEngine::Vector2 anchoredPosition = {0, 0}, UnityEngine::Vector2 sizeDelta = {60.0f, 10.0f});
    ScoreSaber::ClickableText* CreateClickableText(UnityEngine::Transform* parent, std::string_view text, UnityEngine::Vector2 anchoredPosition = {0, 0}, UnityEngine::Vector2 sizeDelta = {60.0f, 10.0f});
    UnityEngine::UI::HorizontalLayoutGroup* CreateTeamMemberLayout(UnityEngine::Transform* parent, TeamUtils::TeamMember& member);
    UnityEngine::GameObject* CreateLoadingIndicator(UnityEngine::Transform* parent);
    ScoreSaber::ClickableImage* CreateClickableImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick);
    ScoreSaber::ClickableImage* CreateClickableImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta);
    UnityEngine::UI::HorizontalLayoutGroup* CreateHeader(UnityEngine::Transform* parent, UnityEngine::Color color);
}