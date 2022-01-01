#include "UI/GlobalViewController.hpp"
#include "CustomTypes/GlobalLeaderboardListTableData.hpp"
#include "Sprites.hpp"
#include "UnityEngine/Application.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

#include "HMUI/TableView_ScrollPositionType.hpp"
#include "HMUI/Touchable.hpp"

#include "Utils/UIUtils.hpp"
#include "logging.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

DEFINE_TYPE(ScoreSaber::UI, GlobalViewController);

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;

#define SetPreferredSize(identifier, width, height)                                         \
    auto layout##identifier = identifier->get_gameObject()->GetComponent<LayoutElement*>(); \
    if (!layout##identifier)                                                                \
        layout##identifier = identifier->get_gameObject()->AddComponent<LayoutElement*>();  \
    layout##identifier->set_preferredWidth(width);                                          \
    layout##identifier->set_preferredHeight(height)

namespace ScoreSaber::UI
{
    void GlobalViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            get_gameObject()->AddComponent<HMUI::Touchable*>();
            /*
            auto headerHorizontal = CreateHorizontalLayoutGroup(get_transform());
            auto headerRectTransform = headerHorizontal->get_rectTransform();
            headerRectTransform->set_anchoredPosition(Vector2(0, 30.0f));
            headerRectTransform->set_sizeDelta(Vector2(-40.0f, 0.0f));
            SetPreferredSize(headerHorizontal, 40.0f, 10.0f);
            headerHorizontal->set_childAlignment(TextAnchor::MiddleCenter);
            auto headerText = UIUtils::CreateClickableText(
                headerHorizontal->get_transform(), u"Global Leaderboards", Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f), []()
                {
                    static auto rankURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("https://scoresaber.com/rankings");
                    Application::OpenURL(rankURL);
                });
            SetPreferredSize(headerText, 40.0f, 10.0f);
            headerText->set_alignment(TMPro::TextAlignmentOptions::Center);
            headerText->set_fontSize(7.0f);
            auto headerBG = headerHorizontal->get_gameObject()->AddComponent<Backgroundable*>();
            headerBG->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 0.5f);

            auto infoButtonHorizontal = CreateHorizontalLayoutGroup(get_transform());
            infoButtonHorizontal->set_childForceExpandWidth(false);
            infoButtonHorizontal->set_childControlWidth(false);
            SetPreferredSize(infoButtonHorizontal, 5.0f, 8.0f);
            auto infoButton = CreateUIButton(infoButtonHorizontal->get_transform(), "?", Vector2(0, 0), Vector2(0.0f, 0.0f), std::bind(&GlobalViewController::OpenMoreInfoModal, this));
            auto infoButtonRectTransform = infoButtonHorizontal->get_rectTransform();
            infoButtonRectTransform->set_anchoredPosition(Vector2(120.0f, 30.0f));
            infoButtonRectTransform->set_sizeDelta(Vector2(0.0f, 0.0f));
            */
            auto vertical = CreateVerticalLayoutGroup(get_transform());
            SetPreferredSize(vertical, 120.0f, -1);
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

            // actual leaderboard stuff
            auto leaderboardHorizontal = CreateHorizontalLayoutGroup(vertical->get_transform());
            leaderboardHorizontal->set_spacing(2.0f);

            auto buttonVertical = CreateVerticalLayoutGroup(leaderboardHorizontal->get_transform());
            auto filterVertical = CreateVerticalLayoutGroup(buttonVertical->get_transform());
            auto playerVertical = CreateVerticalLayoutGroup(leaderboardHorizontal->get_transform());
            buttonVertical->set_childForceExpandHeight(false);
            buttonVertical->set_childAlignment(TextAnchor::MiddleCenter);
            buttonVertical->set_spacing(2.0f);

            filterVertical->set_childForceExpandHeight(false);
            filterVertical->set_childForceExpandWidth(false);
            filterVertical->set_childAlignment(TextAnchor::MiddleCenter);
            filterVertical->set_spacing(2.0f);
            filterVertical->set_padding(RectOffset::New_ctor(2, 2, 2, 2));

            //SetPreferredSize(playerVertical, 100.0f, -1);
            auto filterBG = filterVertical->get_gameObject()->AddComponent<Backgroundable*>();
            filterBG->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 1.0f);
            auto playerBG = playerVertical->get_gameObject()->AddComponent<Backgroundable*>();
            playerBG->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 1.0f);

            auto sizeDelta = Vector2(100.0f, 60.0f);
            auto anchoredPosition = Vector2(0.0f, 0.0f);
            auto listVertical = playerVertical; //CreateVerticalLayoutGroup(playerVertical->get_transform());
            SetPreferredSize(listVertical, sizeDelta.x, sizeDelta.y);
            listVertical->get_rectTransform()->set_sizeDelta(sizeDelta);
            listVertical->get_rectTransform()->set_anchoredPosition(anchoredPosition);
            listVertical->set_childForceExpandHeight(false);
            listVertical->set_childControlHeight(false);
            listVertical->set_childScaleHeight(false);

            auto list = CreateCustomSourceList<ScoreSaber::UI::GlobalLeaderboardListTableData*>(listVertical->get_transform(), Vector2(sizeDelta.x, sizeDelta.y), nullptr);

            list->tableView->ReloadData();
            list->tableView->RefreshCells(true, true);
            list->tableView->ScrollToCellWithIdx(0, HMUI::TableView::ScrollPositionType::Beginning, false);

            auto tex = Texture2D::get_whiteTexture();
            auto sprite = Sprite::Create(tex, Rect(0.0f, 0.0f, (float)tex->get_width(), (float)tex->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
            auto upSprite = Base64ToSprite(carat_up_base64);
            auto upButton = UIUtils::CreateClickableImage(buttonVertical->get_transform(), upSprite, {0, 0}, {0, 0}, []()
                                                          { INFO("Up"); });
            upButton->get_transform()->SetAsFirstSibling();
            auto downSprite = Base64ToSprite(carat_down_base64);
            auto downButton = UIUtils::CreateClickableImage(buttonVertical->get_transform(), downSprite, {0, 0}, {0, 0}, []()
                                                            { INFO("Down"); });
            auto globalSprite = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Sprite*>(), [](auto x)
                                                 { return to_utf8(csstrtostr(x->get_name())) == "GlobalIcon"; });
            auto filterGlobal = UIUtils::CreateClickableImage(filterVertical->get_transform(), globalSprite, {0, 0}, {0, 0}, []()
                                                              { INFO("Global"); });
            auto nearSprite = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Sprite*>(), [](auto x)
                                               { return to_utf8(csstrtostr(x->get_name())) == "PlayerIcon"; });
            auto filterNear = UIUtils::CreateClickableImage(filterVertical->get_transform(), nearSprite, {0, 0}, {0, 0}, []()
                                                            { INFO("Near"); });
            auto friendSprite = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Sprite*>(), [](auto x)
                                                 { return to_utf8(csstrtostr(x->get_name())) == "FriendsIcon"; });
            auto filterFriends = UIUtils::CreateClickableImage(filterVertical->get_transform(), friendSprite, {0, 0}, {0, 0}, []()
                                                               { INFO("Friends"); });
            auto countrySprite = Base64ToSprite(country_base64);
            auto filterCountry = UIUtils::CreateClickableImage(filterVertical->get_transform(), countrySprite, {0, 0}, {0, 0}, []()
                                                               { INFO("Country"); });

            SetPreferredSize(filterGlobal, 8.0f, 8.0f);
            SetPreferredSize(filterNear, 8.0f, 8.0f);
            SetPreferredSize(filterFriends, 8.0f, 8.0f);
            SetPreferredSize(filterCountry, 8.0f, 8.0f);
        }
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
}