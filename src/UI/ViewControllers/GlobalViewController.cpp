#include "UI/ViewControllers/GlobalViewController.hpp"

#include "CustomTypes/Components/CustomCellListTableData.hpp"
#include "CustomTypes/Components/PlayerTableCell.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "HMUI/TableView_ScrollPositionType.hpp"
#include "ScoreSaberUI.hpp"
#include "Sprites.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
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
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

custom_types::Helpers::Coroutine WaitForInit(
    SafePtr<ScoreSaberUI::CustomTypes::Components::CustomCellListTableData>
        leaderboard,
    std::function<void()> callback)
{
    while (!leaderboard->initialized)
    {
        co_yield nullptr;
    }
    callback();
    co_return;
}

#define SetPreferredSize(identifier, width, height)                                         \
    auto layout##identifier = identifier->get_gameObject()->GetComponent<LayoutElement*>(); \
    if (!layout##identifier)                                                                \
        layout##identifier = identifier->get_gameObject()->AddComponent<LayoutElement*>();  \
    layout##identifier->set_preferredWidth(width);                                          \
    layout##identifier->set_preferredHeight(height)

void GlobalViewController::DidActivate(bool firstActivation,
                                       bool addedToHierarchy,
                                       bool screenSystemEnabling)
{
    if (firstActivation)
    {
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

        /*
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
        */

        auto headerHorizontal = CreateHorizontalLayoutGroup(vertical->get_transform());
        SetPreferredSize(headerHorizontal, 120.0f, -1);
        headerHorizontal->set_childControlHeight(true);
        auto textHorizontal = CreateHorizontalLayoutGroup(headerHorizontal->get_transform());
        textHorizontal->set_childForceExpandWidth(false);
        textHorizontal->set_childAlignment(TextAnchor::MiddleCenter);
        SetPreferredSize(textHorizontal, 80.0f, -1);
        textHorizontal->get_rectTransform()->set_sizeDelta(Vector2(-40.0f, 0.0f));
        auto headerText = UIUtils::CreateClickableText(
            textHorizontal->get_transform(), u"Global Leaderboards", Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), []()
            {
                static auto rankURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("https://scoresaber.com/rankings");
                Application::OpenURL(rankURL);
            });
        headerText->set_alignment(TMPro::TextAlignmentOptions::Center);
        headerText->set_fontSize(7.0f);

        auto infoButton = CreateUIButton(headerHorizontal->get_transform(), " ?", Vector2(54.0f, 0.0f), Vector2(10.0f, 8.5f), std::bind(&GlobalViewController::OpenMoreInfoModal, this));
        //SetPreferredSize(infoButton, 10.0f, 8.5f);
        auto layoutinfoButton = infoButton->GetComponent<LayoutElement*>();
        layoutinfoButton->set_ignoreLayout(true);
        auto textObject = infoButton->GetComponentInChildren<TMPro::TextMeshProUGUI*>();
        textObject->set_alignment(TMPro::TextAlignmentOptions::Left);
        //reinterpret_cast<RectTransform*>(infoButton->get_transform())->set_anchoredPosition({40.0f, 0.0f});
        auto headerBG = headerHorizontal->get_gameObject()->AddComponent<Backgroundable*>();
        headerBG->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 0.5f);
        auto headerImageView = headerBG->get_gameObject()->GetComponentInChildren<HMUI::ImageView*>()->dyn__skew() = 0.18f;

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

        ImageButton* arrow = UIUtils::CreateImageButton(
            scoreScopesHost->get_gameObject(),
            Base64ToSprite(carat_up_base64), {0.0f, 25.0f},
            {9.0f, 9.0f}, std::bind(&GlobalViewController::UpButtonWasPressed, this));

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
        auto globalIcon = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Sprite*>(), [](auto x)
                                           { return to_utf8(csstrtostr(x->get_name())) == "GlobalIcon"; });
        auto playerIcon = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Sprite*>(), [](auto x)
                                           { return to_utf8(csstrtostr(x->get_name())) == "PlayerIcon"; });
        auto friendsIcon = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Sprite*>(), [](auto x)
                                            { return to_utf8(csstrtostr(x->get_name())) == "FriendsIcon"; });
        UIUtils::CreateImageButton(imagesGroup->get_gameObject(),
                                   globalIcon, {0.0f, 0.0f},
                                   {4.0f, 4.0f}, std::bind(&GlobalViewController::FilterWasClicked, this, ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::LeaderboardType::Global));
        UIUtils::CreateImageButton(imagesGroup->get_gameObject(),
                                   playerIcon, {0.0f, 0.0f},
                                   {4.0f, 4.0f}, std::bind(&GlobalViewController::FilterWasClicked, this, ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::LeaderboardType::AroundYou));
        UIUtils::CreateImageButton(imagesGroup->get_gameObject(),
                                   friendsIcon, {0.0f, 0.0f},
                                   {4.0f, 4.0f}, std::bind(&GlobalViewController::FilterWasClicked, this, ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::LeaderboardType::Friends));
        UIUtils::CreateImageButton(
            imagesGroup->get_gameObject(),
            Base64ToSprite(country_base64), {0.0f, 0.0f},
            {4.0f, 4.0f}, std::bind(&GlobalViewController::FilterWasClicked, this, ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::LeaderboardType::Country));

        ImageButton* downrrow = UIUtils::CreateImageButton(
            scoreScopesHost->get_gameObject(),
            Base64ToSprite(carat_down_base64), {0.0f, 25.0f},
            {9.0f, 9.0f}, std::bind(&GlobalViewController::DownButtonWasPressed, this));

        VerticalLayoutGroup* globalVerticalHost =
            BeatSaberUI::CreateVerticalLayoutGroup(globalHost->get_transform());

        VerticalLayoutGroup* playersHost = BeatSaberUI::CreateVerticalLayoutGroup(
            globalVerticalHost->get_transform());

        auto loadingVertical = CreateVerticalLayoutGroup(get_transform());
        SetPreferredSize(loadingVertical, 10, 10);
        layoutloadingVertical->set_ignoreLayout(true);
        loadingVertical->get_rectTransform()->set_anchoredPosition(playersHost->get_rectTransform()->get_anchoredPosition());
        auto loadingHorizontal = CreateHorizontalLayoutGroup(loadingVertical->get_transform());
        SetPreferredSize(loadingHorizontal, 10, 10);

        loadingIndicator =
            UIUtils::CreateLoadingIndicator(loadingHorizontal->get_transform());
        auto loadingIndicatorLayout =
            loadingIndicator->GetComponent<LayoutElement*>();
        auto loadingIndicatorRectTransform =
            reinterpret_cast<RectTransform*>(loadingIndicator->get_transform());

        playersHost->set_padding(RectOffset::New_ctor(2, 2, 2, 2));

        Backgroundable* playersHostBg =
            playersHost->get_gameObject()->AddComponent<Backgroundable*>();
        playersHostBg->ApplyBackgroundWithAlpha(
            il2cpp_utils::newcsstr("round-rect-panel"), 1.0f);

        LayoutElement* playersHostElement =
            playersHost->GetComponent<LayoutElement*>();
        playersHostElement->set_preferredWidth(105.0f);
        playersHostElement->set_preferredHeight(60.0f);

        /*
        leaderboardList = BeatSaberUI::CreateCustomSourceList<
            ScoreSaberUI::CustomTypes::Components::CustomCellListTableData*>(
            playersHost->get_transform(), nullptr);
        */

        auto sizeDelta = Vector2(105.0f, 60.0f);
        auto anchoredPosition = Vector2(0.0f, 0.0f);
        playersHost->get_rectTransform()->set_sizeDelta(sizeDelta);
        playersHost->get_rectTransform()->set_anchoredPosition(anchoredPosition);
        playersHost->set_childForceExpandHeight(false);
        playersHost->set_childControlHeight(false);
        playersHost->set_childScaleHeight(false);

        leaderboardList = CreateCustomSourceList<ScoreSaberUI::CustomTypes::Components::CustomCellListTableData*>(playersHost->get_transform(), Vector2(sizeDelta.x, sizeDelta.y), nullptr);
        leaderboardList->globalViewController = this;
        leaderboardList->StartRefresh(true);

        playerProfileModal = ::ScoreSaber::UI::PlayerProfileModal::Create(get_transform());
        leaderboardList->playerProfileModal = playerProfileModal;
    }
}
void GlobalViewController::set_loading(bool value)
{
    loadingIndicator->SetActive(value);
}

void GlobalViewController::UpButtonWasPressed()
{
    leaderboardList->UpButtonWasPressed();
}

void GlobalViewController::DownButtonWasPressed()
{
    leaderboardList->DownButtonWasPressed();
}

void GlobalViewController::OpenMoreInfoModal()
{
    if (!moreInfoModal)
    {
        // make it
        moreInfoModal = CreateModal(get_transform(), Vector2(84.0f, 64.0f), nullptr, true);
        auto textVertical = CreateVerticalLayoutGroup(moreInfoModal->get_transform());
        textVertical->set_spacing(1.0f);
        textVertical->set_padding(RectOffset::New_ctor(2, 2, 2, 2));
        auto h1 = CreateText(textVertical->get_transform(), "What is <color=#6772E5>PP</color>?", false);
        h1->set_alignment(TMPro::TextAlignmentOptions::TopLeft);
        h1->set_fontSize(4.0f);
        auto t1 = CreateText(textVertical->get_transform(), "Performance Points (<color=#6772E5>PP</color>) are earned through playing ranked maps. Harder maps and higher\n"
                                                            "scores will increase the amount of <color=#6772E5>PP</color> you receive from beating a map. Performance Points\n"
                                                            "determine your rank on the ScoreSaber global leaderboards, the more <color=#6772E5>PP</color> you have the higher\n"
                                                            "your rank will be.",
                             false);
        t1->set_alignment(TMPro::TextAlignmentOptions::TopLeft);
        t1->set_fontSize(2.5f);
        t1->set_enableWordWrapping(true);
        t1->get_rectTransform()->set_sizeDelta({0, 0});
        Object::Destroy(t1->GetComponent<LayoutElement*>());
        auto h2 = CreateText(textVertical->get_transform(), "How Does Ranking Work?", false);
        h2->set_alignment(TMPro::TextAlignmentOptions::TopLeft);
        h2->set_fontSize(4.0f);
        auto t2 = CreateText(textVertical->get_transform(), "Ranked maps are maps passed through a ranking system that judges maps to make sure they\n"
                                                            "are objectively fair. The Ranking Team votes based on the Ranking Criteria, a document that\n"
                                                            "goes over the rules a map must follow to be ranked. The Quality Assurance Team votes to\n"
                                                            "either speed up or slow down maps in queue based on whether the map meets their quality\n"
                                                            "standards, however they cannot fully block a map that meets the criteria from being ranked.",
                             false);
        t2->set_alignment(TMPro::TextAlignmentOptions::TopLeft);
        t2->set_fontSize(2.5f);
        t2->set_enableWordWrapping(true);
        t2->get_rectTransform()->set_sizeDelta({0, 0});
        Object::Destroy(t2->GetComponent<LayoutElement*>());
        auto buttonHorizontal = CreateHorizontalLayoutGroup(textVertical->get_transform());
        auto dismiss = CreateUIButton(buttonHorizontal->get_transform(), "Dismiss", [&]()
                                      { moreInfoModal->Hide(true, nullptr); });
        auto moreInfo = CreateUIButton(
            buttonHorizontal->get_transform(), "More Info", []()
            { Application::OpenURL(il2cpp_utils::newcsstr("http://bit.ly/2X8Anko")); });
    }
    moreInfoModal->Show(true, true, nullptr);
}

void GlobalViewController::FilterWasClicked(ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::LeaderboardType type)
{
    leaderboardList->set_leaderboardType(type);
}
