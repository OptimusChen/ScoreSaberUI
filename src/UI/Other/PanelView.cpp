#include "UI/Other/PanelView.hpp"

#include "CustomTypes/Components/ImageButton.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "ScoreSaberUI.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UI/FlowCoordinators/ScoreSaberFlowCoordinator.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/UIUtils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "main.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::Other, PanelView);

using namespace ScoreSaberUI::Utils;
using namespace ScoreSaberUI::UI::Other;
using namespace ScoreSaberUI::CustomTypes::Components;
using namespace ScoreSaberUI::UI::FlowCoordinators;
using namespace QuestUI;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace GlobalNamespace;

#define BeginCoroutine(method)                               \
    SharedCoroutineStarter::get_instance()->StartCoroutine(  \
        reinterpret_cast<System::Collections::IEnumerator*>( \
            custom_types::Helpers::CoroutineHelper::New(method)));

void PanelView::Init(PlatformLeaderboardViewController* viewController)
{
    this->leaderboardView = viewController;

    GameObject* canvas = BeatSaberUI::CreateCanvas();

    HMUI::Screen* screen = canvas->AddComponent<HMUI::Screen*>();

    Transform* transform = screen->get_transform();
    RectTransform* rectTransform = screen->GetComponent<RectTransform*>();

    rectTransform->set_sizeDelta({100.0f, 25.0f});
    transform->SetParent(leaderboardView->get_transform(), false);
    transform->set_localPosition({0.0f, 50.0f, 0.0f});
    transform->set_eulerAngles(Quaternion::get_identity().get_eulerAngles());
    transform->set_localScale(Vector3::get_one());

    this->floatingScreen = screen;

    promptRoot =
        BeatSaberUI::CreateHorizontalLayoutGroup(screen->get_transform());
    promptRoot->set_childAlignment(TextAnchor::UpperLeft);
    promptRoot->set_childForceExpandWidth(false);
    promptRoot->set_spacing(1.0f);

    RectTransform* promptRootRect = promptRoot->get_rectTransform();
    promptRootRect->set_anchoredPosition({0.0f, 10.3f});

    LayoutElement* promptElement = promptRoot->GetComponent<LayoutElement*>();
    promptElement->set_preferredHeight(7.0f);
    promptElement->set_preferredWidth(87.0f);

    ContentSizeFitter* promptFitter =
        promptRoot->GetComponent<ContentSizeFitter*>();
    promptFitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);

    HorizontalLayoutGroup* textGroup =
        BeatSaberUI::CreateHorizontalLayoutGroup(promptRootRect);
    textGroup->get_rectTransform()->set_anchoredPosition({0.0f, 10.0f});

    this->promptText =
        BeatSaberUI::CreateText(textGroup->get_transform(), "...", false);
    this->promptText->set_alignment(TMPro::TextAlignmentOptions::BottomLeft);

    HorizontalLayoutGroup* indicatorGroup =
        BeatSaberUI::CreateHorizontalLayoutGroup(promptRootRect);

    LayoutElement* indicatorElement =
        indicatorGroup->GetComponent<LayoutElement*>();
    indicatorElement->set_preferredHeight(4.0f);
    indicatorElement->set_preferredWidth(4.0f);
}

<<<<<<< HEAD
void PanelView::Show()
{
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

    Sprite* sprite = BeatSaberUI::FileToSprite(iconPath + "scoresaber.png");

    Button* menu = QuestUI::BeatSaberUI::CreateUIButton(
        floatingScreen->get_transform(), "", "SettingsButton",
        Vector2(-37.0f, 0.0f), Vector2(12.0f, 12.0f), []()
        {
            getLogger().info("ssu: test1");
            HMUI::FlowCoordinator* currentFlowCoordinator =
                QuestUI::BeatSaberUI::GetMainFlowCoordinator()
                    ->YoungestChildFlowCoordinatorOrSelf();
            ScoreSaberFlowCoordinator* flowCoordinator =
                ScoreSaberUI::ScoreSaber::flowCoordinator;

            if (!flowCoordinator)
            {
                ScoreSaberUI::ScoreSaber::flowCoordinator =
                    BeatSaberUI::CreateFlowCoordinator<ScoreSaberFlowCoordinator*>();
                flowCoordinator = ScoreSaberUI::ScoreSaber::flowCoordinator;
            }

            currentFlowCoordinator->PresentFlowCoordinator(
                ScoreSaberUI::ScoreSaber::flowCoordinator, nullptr,
                HMUI::ViewController::AnimationDirection::Horizontal,
                HMUI::ViewController::AnimationType::In, false);
        });

    QuestUI::BeatSaberUI::SetButtonSprites(menu, sprite, sprite);
    RectTransform* rectTransform =
        reinterpret_cast<RectTransform*>(menu->get_transform()->GetChild(0));
    rectTransform->set_sizeDelta({12.0f, 12.0f});
    for (int i = 0; i < 13; i++)
    {
        QuestUI::BeatSaberUI::CreateImage(
            leaderboardView->get_transform(),
            BeatSaberUI::FileToSprite(iconPath + "pixel.png"),
            Vector2(-27.0f, 44.0f + i), Vector2(0.5f, 1.0f));
    }

    QuestUI::BeatSaberUI::CreateText(
=======
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

  Sprite* sprite = BeatSaberUI::FileToSprite(iconPath + "scoresaber.png");

  ImageButton* menu = UIUtils::CreateImageButton(
      floatingScreen->get_gameObject(), sprite, {-37.0f, 0.0f}, {12.0f, 12.0f},
      []() {
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

  for (int i = 0; i < 13; i++) {
    QuestUI::BeatSaberUI::CreateImage(
>>>>>>> master
        leaderboardView->get_transform(),
        "<color=#ffde1c>Global Ranking: "
        "</color>#893 "
        "(<size=80%><color=#6771e5>11,141.92pp</color></size>)",
        true, Vector2(5.0f, 50.0f));
    ScoreSaberUI::ScoreSaber::leaderboard->ranked =
        QuestUI::BeatSaberUI::CreateText(
            leaderboardView->get_transform(),
            "<color=#ffde1c>Ranked Status: </color>Ranked (modifiers "
            "disabled)",
            true, Vector2(5.0f, 45.0f));

    BeginCoroutine(SetPrompt("Connecting to ScoreSaber", true, 5.0f, [=]()
                             { BeginCoroutine(SetPrompt(
                                   StringUtils::Colorize("Successfully Connected to ScoreSaber", "green"),
                                   false, 2.0f, nullptr)); }));
}

custom_types::Helpers::Coroutine PanelView::SetPrompt(
    std::string status, bool showIndicator, float dismiss,
    std::function<void()> callback)
{
    this->promptText->SetText(il2cpp_utils::newcsstr(status));

    std::string text = status;

    for (int i = 1; i < (dismiss * 2) + 1; i++)
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(
            CRASH_UNLESS(WaitForSeconds::New_ctor(0.5f)));

        // Couldn't get the loading indicator to work so right now it just displays
        // dots as the loading indicator
        if (showIndicator)
        {
            if (i % 4 != 0)
            {
                text = text + ".";
                promptText->SetText(il2cpp_utils::newcsstr(text));
            }
            else
            {
                for (int k = 0; k < 3; k++)
                {
                    text.pop_back();
                }
                promptText->SetText(il2cpp_utils::newcsstr(text));
            }
        }
    }

    promptText->SetText(il2cpp_utils::newcsstr(""));

    if (callback)
    {
        callback();
    }
    co_return;
}