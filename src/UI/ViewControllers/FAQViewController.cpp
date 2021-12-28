#include "UI/ViewControllers/FAQViewController.hpp"

#include "UnityEngine/Color.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::ViewControllers, FAQViewController);

using namespace ScoreSaberUI::Utils;
using namespace ScoreSaberUI::UI::ViewControllers;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;

void FAQViewController::DidActivate(bool firstActivation, bool addedToHierarchy,
                                    bool screenSystemEnabling) {
  if (firstActivation) {
    UIUtils::CreateHeader(get_transform(), "Links",
                          Color(0.3f, 0.3f, 0.3f, 0.5f));
    // HorizontalLayoutGroup* horzontal =
    //     BeatSaberUI::CreateHorizontalLayoutGroup(get_transform());
    // LayoutElement* horizontalElement =
    //     horzontal->GetComponent<LayoutElement*>();
    // horizontalElement->set_preferredWidth(90.0f);

    // VerticalLayoutGroup* ssLayoutGroup =
    //     BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
    // LayoutElement* ssLayoutElement =
    //     ssLayoutGroup->GetComponent<LayoutElement*>();
    // ssLayoutElement->set_preferredWidth(40.0f);
    // ssLayoutElement->set_preferredHeight(60.0f);
  }
}