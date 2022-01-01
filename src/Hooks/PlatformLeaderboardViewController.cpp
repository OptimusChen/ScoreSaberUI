#include "ScoreSaberUI.hpp"
#include "Sprites.hpp"
#include "hooks.hpp"

#include "CustomTypes/CustomLeaderboardPlatformHandler.hpp"
#include "UI/Other/Banner.hpp"

#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/LoadingControl.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "GlobalNamespace/PlatformLeaderboardsHandler.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_GetScoresCompletionHandler.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_GetScoresResult.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_LeaderboardScore.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_ScoresScope.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/IconSegmentedControl.hpp"
#include "HMUI/IconSegmentedControl_DataItem.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/Screen.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "UI/FlowCoordinators/ScoreSaberFlowCoordinator.hpp"
#include "UI/Other/PanelView.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/SpriteRenderer.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "Utils/StringUtils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/FloatingScreen/FloatingScreen.hpp"
#include "questui/shared/QuestUI.hpp"

using namespace HMUI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace GlobalNamespace;
using namespace ScoreSaberUI;
using namespace ScoreSaberUI::Utils::StringUtils;
using namespace ScoreSaberUI::CustomTypes;
using namespace ScoreSaberUI::UI::FlowCoordinators;

ScoreSaberUI::UI::Other::PanelView* view;

UnityEngine::UI::Button* up;
UnityEngine::UI::Button* down;
std::string iconPath =
    "/sdcard/ModData/com.beatgames.beatsaber/"
    "Mods/ScoreSaberUI/Icons/";

MAKE_AUTO_HOOK_MATCH(
    PlatformLeaderboardViewController_DidActivate,
    &GlobalNamespace::PlatformLeaderboardViewController::DidActivate, void,
    GlobalNamespace::PlatformLeaderboardViewController* self,
    bool firstActivation, bool addedToHeirarchy, bool screenSystemEnabling)
{
    PlatformLeaderboardViewController_DidActivate(
        self, firstActivation, addedToHeirarchy, screenSystemEnabling);

    CustomLeaderboardPlatformHandler* leaderboardsHandler =
        ScoreSaberUI::ScoreSaber::leaderboard;

    leaderboardsHandler->page = 1;
    if (firstActivation)
    {
        Sprite* globalLeaderboardIcon = self->globalLeaderboardIcon;
        Sprite* friendsLeaderboardIcon = self->friendsLeaderboardIcon;
        Sprite* aroundPlayerLeaderboardIcon = self->aroundPlayerLeaderboardIcon;
        Sprite* countryLeaderboardIcon =
            Base64ToSprite(country_base64);
        countryLeaderboardIcon->get_textureRect()
            .set_size({64.0f, 64.0f});

        IconSegmentedControl* scopeSegmentedControl = self->scopeSegmentedControl;

        ::Array<IconSegmentedControl::DataItem*>* array =
            ::Array<IconSegmentedControl::DataItem*>::New({
                IconSegmentedControl::DataItem::New_ctor(globalLeaderboardIcon,
                                                         StrToIl2cppStr("Global")),
                IconSegmentedControl::DataItem::New_ctor(
                    aroundPlayerLeaderboardIcon, StrToIl2cppStr("Around You")),
                IconSegmentedControl::DataItem::New_ctor(friendsLeaderboardIcon,
                                                         StrToIl2cppStr("Friends")),
                IconSegmentedControl::DataItem::New_ctor(countryLeaderboardIcon,
                                                         StrToIl2cppStr("Country")),
            });

        scopeSegmentedControl->SetData(array);

        // RedBrumbler top panel
        if (firstActivation)
        {
            leaderboardsHandler->scoreSaberBanner = ::ScoreSaber::UI::Banner::Create(self->get_transform());
            leaderboardsHandler->scoreSaberBanner->playerProfileModal = ::ScoreSaber::UI::PlayerProfileModal::Create(self->get_transform());

            leaderboardsHandler->scoreSaberBanner->Prompt("Connecting to ScoreSaber", true, 5.0f, [=]()
                                                          { leaderboardsHandler->scoreSaberBanner->Prompt(
                                                                "<color=green>Successfully Connected to ScoreSaber</color>",
                                                                false, 2.0f, nullptr); });
        }

        /*
        view = BeatSaberUI::CreateViewController<
            ScoreSaberUI::UI::Other::PanelView*>();
        view->Init(self);
        view->Show();
        */
    }
}

MAKE_AUTO_HOOK_MATCH(PlatformLeaderboardViewController_Refresh,
                     &GlobalNamespace::PlatformLeaderboardViewController::Refresh,
                     void, GlobalNamespace::PlatformLeaderboardViewController* self,
                     bool showLoadingIndicator, bool clear)
{
    PlatformLeaderboardViewController_Refresh(self, showLoadingIndicator, clear);

    CustomLeaderboardPlatformHandler* leaderboardsHandler =
        ScoreSaberUI::ScoreSaber::leaderboard;

    if (self->difficultyBeatmap == nullptr)
    {
        self->StopAllCoroutines();
        self->ClearContent();
    }
    else
    {
        if (showLoadingIndicator)
        {
            self->loadingControl->ShowLoading(il2cpp_utils::newcsstr(""));
        }
        else
        {
            self->loadingControl->Hide();
        }

        self->StopAllCoroutines();
        self->StartCoroutine(self->RefreshDelayed(showLoadingIndicator, clear));
    }

    std::function<void()> updateUpButton = std::function([=]()
                                                         {
                                                             if (leaderboardsHandler->page == 1)
                                                             {
                                                                 up->get_gameObject()->SetActive(false);
                                                             }
                                                             else
                                                             {
                                                                 up->get_gameObject()->SetActive(true);
                                                             }
                                                         });

    if (!up)
    {
        up = QuestUI::BeatSaberUI::CreateUIButton(
            self->get_transform(), "", "SettingsButton", Vector2(-40.0f, 20.0f),
            Vector2(5.0f, 5.0f), [=]()
            {
                leaderboardsHandler->changePage(false);
                self->StartCoroutine(
                    self->RefreshDelayed(showLoadingIndicator, clear));
                updateUpButton();
            });
        QuestUI::BeatSaberUI::SetButtonSprites(
            up, Base64ToSprite(carat_up_inactive_base64),
            Base64ToSprite(carat_up_base64));
        RectTransform* rectTransform =
            reinterpret_cast<RectTransform*>(up->get_transform()->GetChild(0));
        rectTransform->set_sizeDelta({10.0f, 10.0f});
        updateUpButton();
    }

    if (!down)
    {
        down = QuestUI::BeatSaberUI::CreateUIButton(
            self->get_transform(), "", "SettingsButton", Vector2(-40.0f, -20.0f),
            Vector2(5.0f, 5.0f), [=]()
            {
                leaderboardsHandler->changePage(true);
                self->StartCoroutine(
                    self->RefreshDelayed(showLoadingIndicator, clear));
                updateUpButton();
            });
        QuestUI::BeatSaberUI::SetButtonSprites(
            down, Base64ToSprite(carat_down_inactive_base64),
            Base64ToSprite(carat_down_base64));

        RectTransform* rectTransform =
            reinterpret_cast<RectTransform*>(down->get_transform()->GetChild(0));
        rectTransform->set_sizeDelta({10.0f, 10.0f});
    }
}