#include "UI/ViewControllers/GlobalViewController.hpp"

#include "CustomTypes/Components/CustomCellTableData.hpp"
#include "CustomTypes/Components/PlayerTableCell.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "ScoreSaberUI.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/List/CustomCellListTableData.hpp"
#include "questui/shared/CustomTypes/Components/List/CustomCellListWrapper.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::ViewControllers, GlobalViewController);

using namespace ScoreSaberUI::UI::ViewControllers;
using namespace ScoreSaberUI::Utils;
using namespace ScoreSaberUI::CustomTypes::Components;
using namespace TMPro;
using namespace UnityEngine::UI;
using namespace QuestUI;

custom_types::Helpers::Coroutine WaitForInit(
    SafePtr<ScoreSaberUI::CustomTypes::Components::CustomCellListTableData>
        leaderboard,
    std::function<void()> callback) {
  while (!leaderboard->initialized) {
    co_yield nullptr;
  }
  callback();
  co_return;
}

void GlobalViewController::DidActivate(bool firstActivation,
                                       bool addedToHierarchy,
                                       bool screenSystemEnabling) {
  if (firstActivation) {
    std::string iconPath =
        "/sdcard/ModData/com.beatgames.beatsaber/"
        "Mods/ScoreSaberUI/Icons/";

    VerticalLayoutGroup* vertical =
        BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
    vertical->set_spacing(2.0f);
    vertical->set_childControlHeight(false);
    vertical->set_childAlignment(TextAnchor::MiddleCenter);

    ContentSizeFitter* verticalFitter =
        vertical->GetComponent<ContentSizeFitter*>();
    verticalFitter->set_horizontalFit(
        ContentSizeFitter::FitMode::PreferredSize);
    verticalFitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);

    LayoutElement* verticalElement = vertical->GetComponent<LayoutElement*>();
    verticalElement->set_preferredWidth(120.0f);

    HorizontalLayoutGroup* header =
        BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());

    ContentSizeFitter* headerFitter =
        header->GetComponent<ContentSizeFitter*>();
    headerFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);

    LayoutElement* headerElement = header->GetComponent<LayoutElement*>();
    headerElement->set_preferredWidth(120.0f);

    HorizontalLayoutGroup* globalHeader = Utils::UIUtils::CreateHeader(
        header->get_transform(), Color(0.1f, 0.1f, 0.1f, 0.7f));

    TextMeshProUGUI* text = QuestUI::BeatSaberUI::CreateText(
        globalHeader->get_transform(), "Global Leaderboards");
    text->set_fontSize(text->get_fontSize() * 2.0f);
    text->set_alignment(TextAlignmentOptions::Center);

    LayoutElement* layoutelem =
        text->get_gameObject()->AddComponent<LayoutElement*>();
    layoutelem->set_preferredHeight(10.0f);
    layoutelem->set_preferredWidth(90.0f);

    HorizontalLayoutGroup* buttonGroup =
        BeatSaberUI::CreateHorizontalLayoutGroup(header->get_transform());

    RectTransform* buttonRect = buttonGroup->get_rectTransform();
    buttonRect->set_anchoredPosition({0.0f, 0.0f});
    buttonRect->set_anchorMin({0.94f, 0.5f});
    buttonRect->set_anchorMax({0.94f, 0.5f});

    LayoutElement* buttonElement = buttonGroup->GetComponent<LayoutElement*>();
    buttonElement->set_preferredWidth(10.0f);
    buttonElement->set_preferredHeight(8.5f);

    BeatSaberUI::CreateUIButton(buttonRect, "?", []() {});

    HorizontalLayoutGroup* globalHost =
        BeatSaberUI::CreateHorizontalLayoutGroup(vertical->get_transform());
    globalHost->set_spacing(1.0f);

    ContentSizeFitter* globalHostFitter =
        globalHost->GetComponent<ContentSizeFitter*>();
    globalHostFitter->set_horizontalFit(
        ContentSizeFitter::FitMode::PreferredSize);
    globalHostFitter->set_verticalFit(
        ContentSizeFitter::FitMode::PreferredSize);

    LayoutElement* globalHostElement =
        globalHost->GetComponent<LayoutElement*>();
    globalHostElement->set_preferredWidth(120.0f);

    VerticalLayoutGroup* scoreScopesHost =
        BeatSaberUI::CreateVerticalLayoutGroup(globalHost->get_transform());

    LayoutElement* scoreScopesHostElement =
        scoreScopesHost->GetComponent<LayoutElement*>();
    scoreScopesHostElement->set_preferredWidth(9.0f);

    ScoreSaberUI::CustomTypes::Components::CustomCellListTableData*
        leaderboard = nullptr;

    ImageButton* arrow = UIUtils::CreateImageButton(
        scoreScopesHost->get_gameObject(),
        BeatSaberUI::FileToSprite(iconPath + "arrow.png"), {0.0f, 25.0f},
        {9.0f, 9.0f}, [=]() { leaderboard->UpButtonWasPressed(); });

    VerticalLayoutGroup* scoreScopes = BeatSaberUI::CreateVerticalLayoutGroup(
        scoreScopesHost->get_transform());

    RectOffset* scoreScopesPad = scoreScopes->get_padding();
    scoreScopesPad->set_bottom(1);
    scoreScopesPad->set_left(1);
    scoreScopesPad->set_right(1);
    scoreScopesPad->set_top(1);

    LayoutElement* scoreScopesElement =
        scoreScopes->GetComponent<LayoutElement*>();
    scoreScopesElement->set_preferredWidth(9.0f);
    scoreScopesElement->set_preferredHeight(40.0f);

    Backgroundable* background =
        scoreScopes->get_gameObject()->AddComponent<Backgroundable*>();
    background->ApplyBackgroundWithAlpha(
        il2cpp_utils::newcsstr("round-rect-panel"), 1.0f);

    VerticalLayoutGroup* imagesGroup =
        BeatSaberUI::CreateVerticalLayoutGroup(scoreScopes->get_transform());

    LayoutElement* imagesGroupElement =
        imagesGroup->GetComponent<LayoutElement*>();
    imagesGroupElement->set_preferredWidth(4.0f);
    imagesGroupElement->set_preferredHeight(4.0f);

    UIUtils::CreateImageButton(imagesGroup->get_gameObject(),
                               ScoreSaber::globalIcon, {0.0f, 0.0f},
                               {4.0f, 4.0f}, []() {});
    UIUtils::CreateImageButton(imagesGroup->get_gameObject(),
                               ScoreSaber::playerIcon, {0.0f, 0.0f},
                               {4.0f, 4.0f}, []() {});
    UIUtils::CreateImageButton(imagesGroup->get_gameObject(),
                               ScoreSaber::friendsIcon, {0.0f, 0.0f},
                               {4.0f, 4.0f}, []() {});
    UIUtils::CreateImageButton(
        imagesGroup->get_gameObject(),
        BeatSaberUI::FileToSprite(iconPath + "country.png"), {0.0f, 0.0f},
        {4.0f, 4.0f}, []() {});

    ImageButton* downrrow = UIUtils::CreateImageButton(
        scoreScopesHost->get_gameObject(),
        BeatSaberUI::FileToSprite(iconPath + "arrow_down.png"), {0.0f, 25.0f},
        {9.0f, 9.0f}, [=]() { leaderboard->DownButtonWasPressed(); });

    VerticalLayoutGroup* globalVerticalHost =
        BeatSaberUI::CreateVerticalLayoutGroup(globalHost->get_transform());

    VerticalLayoutGroup* playersHost = BeatSaberUI::CreateVerticalLayoutGroup(
        globalVerticalHost->get_transform());

    playersHost->set_padding(RectOffset::New_ctor(2, 2, 2, 2));

    Backgroundable* playersHostBg =
        playersHost->get_gameObject()->AddComponent<Backgroundable*>();
    playersHostBg->ApplyBackgroundWithAlpha(
        il2cpp_utils::newcsstr("round-rect-panel"), 1.0f);

    LayoutElement* playersHostElement =
        playersHost->GetComponent<LayoutElement*>();
    playersHostElement->set_preferredWidth(105.0f);
    playersHostElement->set_preferredHeight(60.0f);

    leaderboard = BeatSaberUI::CreateCustomSourceList<
        ScoreSaberUI::CustomTypes::Components::CustomCellListTableData*>(
        playersHost->get_transform(), nullptr);
    leaderboard->ctor();
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
        reinterpret_cast<System::Collections::IEnumerator*>(
            custom_types::Helpers::CoroutineHelper::New(
                WaitForInit(leaderboard, [=]() {
                  leaderboard->tableView->ReloadData();
                  leaderboard->get_transform()
                      ->get_parent()
                      ->SetAsFirstSibling();
                }))));
  }
}