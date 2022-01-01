#include "UI/ViewControllers/TeamViewController.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::ViewControllers, TeamViewController);

using namespace ScoreSaberUI::UI::ViewControllers;
using namespace ScoreSaberUI::Utils;
using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

void TeamViewController::DidActivate(bool firstActivation,
                                     bool addedToHierarchy,
                                     bool screenSystemEnabling)
{
    if (firstActivation)
    {
        UnityEngine::UI::HorizontalLayoutGroup* horizontal =
            UIUtils::CreateHeader(get_transform(),
                                  Color(0.1f, 0.1f, 0.1f, 0.7f));

        TextMeshProUGUI* text = QuestUI::BeatSaberUI::CreateText(
            horizontal->get_transform(), "Team and Contributors");
        text->set_fontSize(text->get_fontSize() * 2.0f);
        text->set_alignment(TextAlignmentOptions::Center);

        LayoutElement* layoutelem =
            text->get_gameObject()->AddComponent<LayoutElement*>();
        layoutelem->set_preferredHeight(10.0f);
        layoutelem->set_preferredWidth(90.0f);
    }
}