#include "UI/Other/PanelView.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "ScoreSaberUI.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UI/FlowCoordinators/ScoreSaberFlowCoordinator.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "main.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::Other, PanelView);

using namespace ScoreSaberUI::UI::Other;
using namespace ScoreSaberUI::UI::FlowCoordinators;
using namespace QuestUI;
using namespace UnityEngine::UI;

void PanelView::Init(PlatformLeaderboardViewController* viewController) {
  this->leaderboardView = viewController;

  GameObject* canvas = BeatSaberUI::CreateCanvas();

  HMUI::Screen* screen = canvas->AddComponent<HMUI::Screen*>();

  Transform* transform = screen->get_transform();
  RectTransform* rectTransform = screen->GetComponent<RectTransform*>();

  rectTransform->set_sizeDelta({100.0f, 15.0f});
  transform->SetParent(leaderboardView->get_transform(), false);
  transform->set_localPosition({0.0f, 50.0f, 0.0f});
  transform->set_eulerAngles(Quaternion::get_identity().get_eulerAngles());
  transform->set_localScale(Vector3::get_one());

  this->floatingScreen = screen;
}

void PanelView::Show() {
  this->floatingScreen->SetRootViewController(leaderboardView, 1);

  std::string iconPath =
      "/sdcard/ModData/com.beatgames.beatsaber/"
      "Mods/ScoreSaberUI/Icons/";

  VerticalLayoutGroup* vertical =
      QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(
          floatingScreen->get_transform());
  vertical->get_rectTransform()->set_anchoredPosition({0.0f, 0.0f});
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
  background->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("title-gradient"),
                                       1.0f);

  HMUI::ImageView* imageView =
      background->get_gameObject()->GetComponentInChildren<HMUI::ImageView*>();
  imageView->gradient = true;
  imageView->gradientDirection = 0;
  imageView->set_color(Color::get_white());
  imageView->set_color0(Color(0.0f, 0.45f, 0.65f, 1.0f));
  imageView->set_color1(Color(0.0f, 0.45f, 0.65f, 0.0f));
  imageView->curvedCanvasSettingsHelper->Reset();

  Button* menu = QuestUI::BeatSaberUI::CreateUIButton(
      floatingScreen->get_transform(), "", "SettingsButton",
      Vector2(-37.0f, 0.0f), Vector2(12.0f, 12.0f), []() {
        getLogger().info("ssu: test1");
        HMUI::FlowCoordinator* currentFlowCoordinator =
            QuestUI::BeatSaberUI::GetMainFlowCoordinator()
                ->YoungestChildFlowCoordinatorOrSelf();
        ScoreSaberFlowCoordinator* flowCoordinator =
            ScoreSaberUI::ScoreSaber::flowCoordinator;

        if (!flowCoordinator) {
          ScoreSaberUI::ScoreSaber::flowCoordinator =
              BeatSaberUI::CreateFlowCoordinator<ScoreSaberFlowCoordinator*>();
          flowCoordinator = ScoreSaberUI::ScoreSaber::flowCoordinator;
        }

        currentFlowCoordinator->PresentFlowCoordinator(
            ScoreSaberUI::ScoreSaber::flowCoordinator, nullptr,
            HMUI::ViewController::AnimationDirection::Horizontal,
            HMUI::ViewController::AnimationType::In, false);
      });

  Sprite* sprite = BeatSaberUI::FileToSprite(iconPath + "scoresaber.png");

  QuestUI::BeatSaberUI::SetButtonSprites(menu, sprite, sprite);
  RectTransform* rectTransform =
      reinterpret_cast<RectTransform*>(menu->get_transform()->GetChild(0));
  rectTransform->set_sizeDelta({12.0f, 12.0f});
  for (int i = 0; i < 13; i++) {
    QuestUI::BeatSaberUI::CreateImage(
        leaderboardView->get_transform(),
        BeatSaberUI::FileToSprite(iconPath + "pixel.png"),
        Vector2(-27.0f, 44.0f + i), Vector2(0.5f, 1.0f));
  }

  QuestUI::BeatSaberUI::CreateText(
      leaderboardView->get_transform(),
      "<color=#ffde1c>Global Ranking: "
      "</color>#893 (<size=80%><color=#6771e5>11,141.92pp</color></size>)",
      true, Vector2(5.0f, 50.0f));
  ScoreSaberUI::ScoreSaber::leaderboard->ranked =
      QuestUI::BeatSaberUI::CreateText(
          leaderboardView->get_transform(),
          "<color=#ffde1c>Ranked Status: </color>Ranked (modifiers disabled)",
          true, Vector2(5.0f, 45.0f));
}
