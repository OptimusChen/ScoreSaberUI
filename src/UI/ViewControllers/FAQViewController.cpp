#include "UI/ViewControllers/FAQViewController.hpp"

#include "UnityEngine/Color.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::ViewControllers, FAQViewController);

using namespace ScoreSaberUI;
using namespace ScoreSaberUI::UI::ViewControllers;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace TMPro;

void CreateLinkBoxes(Transform* parent, int i)
{
    VerticalLayoutGroup* layoutGroup =
        BeatSaberUI::CreateVerticalLayoutGroup(parent);
    layoutGroup->get_padding()->set_bottom(4);
    layoutGroup->get_padding()->set_left(4);
    layoutGroup->get_padding()->set_right(4);
    layoutGroup->get_padding()->set_top(4);

    ContentSizeFitter* layoutGroupFitter =
        layoutGroup->GetComponent<ContentSizeFitter*>();
    layoutGroupFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
    layoutGroupFitter->set_horizontalFit(
        ContentSizeFitter::FitMode::PreferredSize);

    QuestUI::Backgroundable* bg =
        layoutGroup->get_gameObject()->AddComponent<QuestUI::Backgroundable*>();
    bg->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("title-gradient"), 1.0f);

    HMUI::ImageView* imageView =
        bg->get_gameObject()->GetComponentInChildren<HMUI::ImageView*>();
    imageView->gradient = true;
    imageView->gradientDirection = 1;
    imageView->set_color(Color::get_white());
    imageView->set_color0(Color(0.0f, 0.2f, 0.25f, 1.0f));
    imageView->set_color1(Color(0.0f, 0.35f, 0.4f, 1.0f));

    LayoutElement* layoutElement = layoutGroup->GetComponent<LayoutElement*>();
    layoutElement->set_preferredWidth(40.0f);
    layoutElement->set_preferredHeight(60.0f);

    TextMeshProUGUI* text =
        BeatSaberUI::CreateText(layoutGroup->get_transform(), "ScoreSaber", true);
    text->set_fontSize(6.5f);
    text->set_alignment(TextAlignmentOptions::Center);

    BeatSaberUI::CreateUIButton(layoutGroup->get_transform(), "Discord", []() {});
    BeatSaberUI::CreateUIButton(layoutGroup->get_transform(), "Twitter", []() {});
    BeatSaberUI::CreateUIButton(layoutGroup->get_transform(), "Patreon", []() {});

    VerticalLayoutGroup* imageLayout =
        BeatSaberUI::CreateVerticalLayoutGroup(layoutGroup->get_transform());

    ContentSizeFitter* imageFitter =
        imageLayout->GetComponent<ContentSizeFitter*>();
    imageFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
    imageFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

    std::string iconPath =
        "/sdcard/ModData/com.beatgames.beatsaber/"
        "Mods/ScoreSaberUI/Icons/";

    VerticalLayoutGroup* imageParent =
        BeatSaberUI::CreateVerticalLayoutGroup(imageLayout->get_transform());

    LayoutElement* imageElement = imageParent->GetComponent<LayoutElement*>();
    imageElement->set_preferredWidth(20.0f);
    imageElement->set_preferredHeight(20.0f);

    Sprite* logo = BeatSaberUI::FileToSprite(iconPath + "scoresaber.png");

    if (i == 0)
    {
        logo = BeatSaberUI::FileToSprite(iconPath + "bsmg.jpg");
        text->SetText(il2cpp_utils::newcsstr("BSMG"));
        imageView->set_color0(Color(0.05f, 0.0f, 0.05f, 1.0f));
        imageView->set_color1(Color(0.1f, 0.0f, 0.1f, 1.0f));
    }

    HMUI::ImageView* image = BeatSaberUI::CreateImage(
        imageElement->get_transform(), logo, {0.0f, 0.0f}, {0.0f, 0.0f});
    image->set_preserveAspect(true);
}

void FAQViewController::DidActivate(bool firstActivation, bool addedToHierarchy,
                                    bool screenSystemEnabling)
{
    if (firstActivation)
    {
        VerticalLayoutGroup* vertical =
            BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        ContentSizeFitter* sizeFitter =
            vertical->GetComponent<ContentSizeFitter*>();
        sizeFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
        sizeFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
        vertical->set_spacing(2.0f);

        UnityEngine::UI::HorizontalLayoutGroup* header =
            UIUtils::CreateHeader(vertical->get_transform(),
                                  Color(0.1f, 0.1f, 0.1f, 0.7f));

        TextMeshProUGUI* text =
            QuestUI::BeatSaberUI::CreateText(header->get_transform(), "Links");
        text->set_fontSize(text->get_fontSize() * 2.0f);
        text->set_alignment(TextAlignmentOptions::Center);

        LayoutElement* layoutelem =
            text->get_gameObject()->AddComponent<LayoutElement*>();
        layoutelem->set_preferredHeight(10.0f);
        layoutelem->set_preferredWidth(90.0f);

        HorizontalLayoutGroup* horizontal =
            BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());

        ContentSizeFitter* horizFitter =
            horizontal->GetComponent<ContentSizeFitter*>();
        horizFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
        horizFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);

        LayoutElement* horizontalElement =
            horizontal->GetComponent<LayoutElement*>();
        horizontalElement->set_preferredWidth(90.0f);

        CreateLinkBoxes(horizontal->get_transform(), 1);
        CreateLinkBoxes(horizontal->get_transform(), 0);
    }
}