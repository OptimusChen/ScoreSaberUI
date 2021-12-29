#include "UI/ViewControllers/GlobalViewController.hpp"

#include "UnityEngine/RectTransform.hpp"
#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::ViewControllers, GlobalViewController);

using namespace ScoreSaberUI::UI::ViewControllers;
using namespace TMPro;
using namespace UnityEngine::UI;

void GlobalViewController::DidActivate(bool firstActivation,
                                       bool addedToHierarchy,
                                       bool screenSystemEnabling) {
  if (firstActivation) {
    UnityEngine::UI::HorizontalLayoutGroup* horizontal =
        Utils::UIUtils::CreateHeader(get_transform(),
                                     Color(0.1f, 0.1f, 0.1f, 0.7f));
    horizontal->get_rectTransform()->set_anchoredPosition({0.0f, 0.0f});

    TextMeshProUGUI* text = QuestUI::BeatSaberUI::CreateText(
        horizontal->get_transform(), "Global Leaderboards");
    text->set_fontSize(text->get_fontSize() * 2.0f);
    text->set_alignment(TextAlignmentOptions::Center);

    LayoutElement* layoutelem =
        text->get_gameObject()->AddComponent<LayoutElement*>();
    layoutelem->set_preferredHeight(10.0f);
    layoutelem->set_preferredWidth(90.0f);
  }
}