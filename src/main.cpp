#include "main.hpp"

#include <chrono>
#include <codecvt>

#include "CustomTypes/CustomLeaderboardPlatformHandler.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/GameplayModifiersModelSO.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IGameEnergyCounter.hpp"
#include "GlobalNamespace/IPlatformUserModel.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/LeaderboardTableView_ScoreData.hpp"
#include "GlobalNamespace/LoadingControl.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/MainSystemInit.hpp"
#include "GlobalNamespace/OculusPlatformLeaderboardsHandler.hpp"
#include "GlobalNamespace/OculusPlatformLeaderboardsHandler_--c__DisplayClass5_0.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "GlobalNamespace/PlatformLeaderboardsHandler.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_GetScoresCompletionHandler.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_GetScoresResult.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_LeaderboardScore.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_ScoresScope.hpp"
#include "GlobalNamespace/QuestAppInit.hpp"
#include "GlobalNamespace/RelativeScoreAndImmediateRankCounter.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/ScoreController.hpp"
#include "GlobalNamespace/ScreenCaptureCache.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ImageView.hpp"
#include "TMPro/TextMeshPro.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/JsonUtility.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/CanvasUpdate.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "VRUIControls/PhysicsRaycasterWithCache.hpp"
#include "Zenject/ConcreteIdBinderGeneric_1.hpp"
#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"
#include "Zenject/NameTransformScopeConcreteIdArgConditionCopyNonLazyBinder.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/error/en.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "config-utils/shared/config-utils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/QuestUI.hpp"

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace GlobalNamespace;

static ModInfo modInfo;  // Stores the ID and version of our mod, and is sent to
                         // the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
  static Configuration config(modInfo);
  config.Load();
  return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
  static Logger* logger = new Logger(modInfo);
  return *logger;
}

ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler*
    leaderboardsHandler;

MAKE_HOOK_MATCH(
    MainSystemInit_InstallPlatformLeaderboardsModel,
    &GlobalNamespace::MainSystemInit::InstallPlatformLeaderboardsModel, void,
    GlobalNamespace::MainSystemInit* self, Zenject::DiContainer* container) {
  using namespace GlobalNamespace;
  using namespace VRUIControls;
  using namespace Zenject;

  leaderboardsHandler = CRASH_UNLESS(
      il2cpp_utils::New<
          ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler*>());
  leaderboardsHandler->page = 1;
  container->Bind<PlatformLeaderboardsHandler*>()
      ->FromInstance(leaderboardsHandler)
      ->AsSingle();
  container->Bind<LeaderboardScoreUploader*>()
      ->FromComponentInNewPrefab(self->leaderboardScoreUploader)
      ->AsSingle();
  container->BindInterfacesAndSelfTo<PlatformLeaderboardsModel*>()
      ->FromComponentInNewPrefab(self->platformLeaderboardsModel)
      ->AsSingle();

  container->Bind<ScreenCaptureCache*>()->AsSingle();
  container->Bind<PhysicsRaycasterWithCache*>()->AsSingle();
}

MAKE_HOOK_MATCH(TextMeshProUGUI_GenerateTextMesh,
                &::TMPro::TextMeshProUGUI::GenerateTextMesh, void,
                TMPro::TextMeshProUGUI* self) {
  static auto literalString =
      il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("PlayerName");
  if (self->get_name()->Equals(literalString)) {
    self->set_richText(true);
    TextMeshProUGUI_GenerateTextMesh(self);
    self->Rebuild(UnityEngine::UI::CanvasUpdate::PreRender);
  } else {
    TextMeshProUGUI_GenerateTextMesh(self);
  }
}

UnityEngine::UI::Button* up;
UnityEngine::UI::Button* down;
std::string iconPath =
    "/sdcard/ModData/com.beatgames.beatsaber/"
    "Mods/ScoreSaberLeaderboards/";

MAKE_HOOK_MATCH(
    PlatformLeaderboardViewController_DidActivate,
    &GlobalNamespace::PlatformLeaderboardViewController::DidActivate, void,
    GlobalNamespace::PlatformLeaderboardViewController* self,
    bool firstActivation, bool addedToHeirarchy, bool screenSystemEnabling) {
  PlatformLeaderboardViewController_DidActivate(
      self, firstActivation, addedToHeirarchy, screenSystemEnabling);

  leaderboardsHandler->page = 1;
  if (firstActivation) {
    VerticalLayoutGroup* vertical =
        QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(self->get_transform());
    vertical->get_rectTransform()->set_anchoredPosition({0.0f, 50.0f});
    HorizontalLayoutGroup* horizontal =
        QuestUI::BeatSaberUI::CreateHorizontalLayoutGroup(
            vertical->get_transform());

    TMPro::TextMeshProUGUI* text =
        QuestUI::BeatSaberUI::CreateText(horizontal->get_transform(), "");
    text->set_fontSize(text->get_fontSize() * 2.0f);
    text->set_alignment(TMPro::TextAlignmentOptions::Center);

    LayoutElement* layoutelem =
        text->get_gameObject()->AddComponent<LayoutElement*>();
    layoutelem->set_preferredHeight(15.0f);
    layoutelem->set_preferredWidth(90.0f);

    Backgroundable* background =
        horizontal->get_gameObject()->AddComponent<Backgroundable*>();
    background->ApplyBackgroundWithAlpha(
        il2cpp_utils::newcsstr("title-gradient"), 1.0f);

    HMUI::ImageView* imageView =
        background->get_gameObject()
            ->GetComponentInChildren<HMUI::ImageView*>();
    imageView->gradient = true;
    imageView->gradientDirection = 0;
    imageView->set_color(Color::get_white());
    imageView->set_color0(Color(0.0f, 0.45f, 0.65f, 1.0f));
    imageView->set_color1(Color(0.0f, 0.45f, 0.65f, 0.0f));
    imageView->curvedCanvasSettingsHelper->Reset();

    QuestUI::BeatSaberUI::CreateImage(
        self->get_transform(),
        BeatSaberUI::FileToSprite(iconPath + "scoresaber.png"),
        Vector2(-37.0f, 50.0f), Vector2(12.0f, 12.0f));
    for (int i = 0; i < 13; i++) {
      QuestUI::BeatSaberUI::CreateImage(
          self->get_transform(),
          BeatSaberUI::FileToSprite(iconPath + "pixel.png"),
          Vector2(-27.0f, 44.0f + i), Vector2(0.5f, 1.0f));
    }

    QuestUI::BeatSaberUI::CreateText(
        self->get_transform(),
        "<color=#ffde1c>Global Ranking: "
        "</color>#893 (<size=80%><color=#6771e5>11,141.92pp</color></size>)",
        true, Vector2(5.0f, 50.0f));
    leaderboardsHandler->ranked = QuestUI::BeatSaberUI::CreateText(
        self->get_transform(),
        "<color=#ffde1c>Ranked Status: </color>Ranked (modifiers disabled)",
        true, Vector2(5.0f, 45.0f));
  }
}

MAKE_HOOK_MATCH(PlatformLeaderboardViewController_Refresh,
                &GlobalNamespace::PlatformLeaderboardViewController::Refresh,
                void, GlobalNamespace::PlatformLeaderboardViewController* self,
                bool showLoadingIndicator, bool clear) {
  PlatformLeaderboardViewController_Refresh(self, showLoadingIndicator, clear);
  if (self->difficultyBeatmap == nullptr) {
    self->StopAllCoroutines();
    self->ClearContent();
  } else {
    if (showLoadingIndicator) {
      self->loadingControl->ShowLoading(il2cpp_utils::newcsstr(""));
    } else {
      self->loadingControl->Hide();
    }

    self->StopAllCoroutines();
    self->StartCoroutine(self->RefreshDelayed(showLoadingIndicator, clear));
  }

  if (!up) {
    up = QuestUI::BeatSaberUI::CreateUIButton(
        self->get_transform(), "", "SettingsButton", Vector2(-40.0f, 20.0f),
        Vector2(5.0f, 5.0f), [=]() {
          leaderboardsHandler->changePage(false);
          self->StartCoroutine(
              self->RefreshDelayed(showLoadingIndicator, clear));
        });
    QuestUI::BeatSaberUI::SetButtonSprites(
        up, BeatSaberUI::FileToSprite(iconPath + "arrow.png"),
        BeatSaberUI::FileToSprite(iconPath + "arrowactive.png"));
    RectTransform* rectTransform =
        reinterpret_cast<RectTransform*>(up->get_transform()->GetChild(0));
    rectTransform->set_sizeDelta({10.0f, 10.0f});
    if (leaderboardsHandler->page == 1) {
      up->set_interactable(false);
    } else {
      up->set_interactable(true);
    }
  }

  if (!down) {
    down = QuestUI::BeatSaberUI::CreateUIButton(
        self->get_transform(), "", "SettingsButton", Vector2(-40.0f, -20.0f),
        Vector2(5.0f, 5.0f), [=]() {
          leaderboardsHandler->changePage(true);
          self->StartCoroutine(
              self->RefreshDelayed(showLoadingIndicator, clear));
        });
    QuestUI::BeatSaberUI::SetButtonSprites(
        down, BeatSaberUI::FileToSprite(iconPath + "arrow_down.png"),
        BeatSaberUI::FileToSprite(iconPath + "arrow_downactive.png"));
    RectTransform* rectTransform =
        reinterpret_cast<RectTransform*>(down->get_transform()->GetChild(0));
    rectTransform->set_sizeDelta({10.0f, 10.0f});
  }
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
  info.id = ID;
  info.version = VERSION;
  modInfo = info;

  getConfig().Load();
  getConfig().Reload();
  getConfig().Write();
  getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
  il2cpp_functions::Init();
  QuestUI::Init();
  custom_types::Register::AutoRegister();

  getLogger().info("Installing hooks...");
  INSTALL_HOOK(getLogger(), PlatformLeaderboardViewController_Refresh);
  INSTALL_HOOK(getLogger(), PlatformLeaderboardViewController_DidActivate);
  INSTALL_HOOK(getLogger(), MainSystemInit_InstallPlatformLeaderboardsModel);
  INSTALL_HOOK(getLogger(), TextMeshProUGUI_GenerateTextMesh);
  getLogger().info("Installed all hooks!");
}