#include "CustomTypes/ScoreSaberBanner.hpp"
#include "UI/MainMenuFlowCoordinator.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "TMPro/TextAlignmentOptions.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

#include "Sprites.hpp"
#include "logging.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"

#include "Utils/UIUtils.hpp"

DEFINE_TYPE(ScoreSaber::UI, Banner);

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

#define SetPreferredSize(identifier, width, height)                                         \
    auto layout##identifier = identifier->get_gameObject()->GetComponent<LayoutElement*>(); \
    if (!layout##identifier)                                                                \
        layout##identifier = identifier->get_gameObject()->AddComponent<LayoutElement*>();  \
    layout##identifier->set_preferredWidth(width);                                          \
    layout##identifier->set_preferredHeight(height)

namespace ScoreSaber::UI
{
    Banner* Banner::Create(Transform* parent)
    {
        auto panel = CreateCanvas();
        auto panelTransform = panel->get_transform();
        panelTransform->SetParent(parent, false);

        auto panelRectTransform = panel->GetComponent<RectTransform*>();
        panelRectTransform->set_localScale(Vector3(1, 1, 1));
        panelRectTransform->set_anchoredPosition({7.5f, 50.0f});

        auto banner = panel->get_gameObject()->AddComponent<Banner*>();
        banner->Setup();
        return banner;
    }

    void Banner::Setup()
    {
        // tbh with all this layout stuff I fuck around till it works the way I want it to
        auto horizon = CreateHorizontalLayoutGroup(get_transform());
        horizon->set_childForceExpandWidth(false);
        horizon->set_childForceExpandHeight(true);
        horizon->set_childControlWidth(false);
        horizon->set_childControlHeight(true);
        horizon->set_spacing(2.0f);
        horizon->set_padding(RectOffset::New_ctor(2, 2, 2, 2));
        SetPreferredSize(horizon, 90.5, 14);

        auto buttonVertical = CreateVerticalLayoutGroup(horizon->get_transform());
        auto seperatorVertical = CreateVerticalLayoutGroup(horizon->get_transform());
        auto infoVertical = CreateVerticalLayoutGroup(horizon->get_transform());
        loadingVertical = CreateVerticalLayoutGroup(get_transform());

        SetPreferredSize(buttonVertical, 10, 10);
        SetPreferredSize(seperatorVertical, 0.5f, 10);
        SetPreferredSize(infoVertical, 70, 10);
        SetPreferredSize(loadingVertical, 10, 10);

        bg = horizon->get_gameObject()->AddComponent<Backgroundable*>();
        bg->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("title-gradient"), 1.0f);

        bgImage = bg->get_gameObject()->GetComponentInChildren<ImageView*>();
        bgImage->dyn__skew() = 0.18f;
        bgImage->dyn__gradient() = true;
        bgImage->dyn__gradientDirection() = 0;
        bgImage->set_color0(Color(1, 1, 1, 1));
        bgImage->set_color1(Color(1, 1, 1, 0));
        bgImage->dyn__curvedCanvasSettingsHelper()->Reset();

        set_color(defaultColor);

        // main menu button setup
        float buttonSize = 10.0f;
        auto btn = CreateUIButton(buttonVertical->get_transform(), "", "SettingsButton", Vector2(0, 0), Vector2(buttonSize, buttonSize), std::bind(&Banner::OpenMainMenuFlowCoordinator, this));
        reinterpret_cast<RectTransform*>(btn->get_transform()->GetChild(0))->set_sizeDelta({buttonSize, buttonSize});

        auto scoresaber_active = Base64ToSprite(ScoreSaber_Active);
        auto scoresaber_inactive = Base64ToSprite(ScoreSaber_Inactive);
        SetButtonSprites(btn, scoresaber_inactive, scoresaber_active);
        auto btnImageView = btn->get_gameObject()->GetComponentInChildren<ImageView*>();
        btnImageView->dyn__skew() = 0.18f;
        AddHoverHint(btn->get_gameObject(), "Opens the ScoreSaber main menu");
        auto btnLayout = buttonVertical->get_gameObject()->AddComponent<LayoutElement*>();
        btnLayout->set_preferredWidth(buttonSize);

        // seperator setup
        auto texture = Texture2D::get_whiteTexture();
        auto seperatorSprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);

        auto image = CreateImage(seperatorVertical->get_transform(), seperatorSprite, Vector2(0, 0), Vector2(0, 0));
        image->dyn__skew() = 0.18f;
        auto imageLayout = image->get_gameObject()->AddComponent<LayoutElement*>();
        imageLayout->set_preferredWidth(1.0f);

        // info setup
        topText = UIUtils::CreateClickableText(infoVertical->get_transform(), "");
        bottomText = UIUtils::CreateClickableText(infoVertical->get_transform(), "");

        topText->get_onPointerClickEvent() += [&](auto _)
        { OpenPlayerInfoModal(); };
        bottomText->get_onPointerClickEvent() += [&](auto _)
        { OpenSongInBrowser(); };

        auto loadingHorizontal = CreateHorizontalLayoutGroup(loadingVertical->get_transform());
        UIUtils::CreateLoadingIndicator(loadingHorizontal->get_transform());
        SetPreferredSize(loadingHorizontal, 10, 10);
    }

    void Banner::OpenMainMenuFlowCoordinator()
    {
        // do whatever is needed here for this to work
        set_loading(false);
        set_ranking(std::rand() % 10000, (float)(std::rand() % 10000) / 100.0f);
        set_status("Unranked (Mod Unfinished)");

        // HACK: Resources call to get these objects to use for changing menu is bad but there's no good other choice afaik
        auto mainfc = GetMainFlowCoordinator();
        auto youngest = mainfc->YoungestChildFlowCoordinatorOrSelf();

        auto fc = ArrayUtil::First(Resources::FindObjectsOfTypeAll<ScoreSaber::UI::MainMenuFlowCoordinator*>());
        if (!fc)
            fc = CreateFlowCoordinator<ScoreSaber::UI::MainMenuFlowCoordinator*>();

        youngest->PresentFlowCoordinator(fc, nullptr, ViewController::AnimationDirection::Horizontal, false, false);
    }

    void Banner::OpenPlayerInfoModal()
    {
        // just make sure to have this actually assigned
        if (playerProfileModal && Object::IsNativeObjectAlive(playerProfileModal))
            playerProfileModal->Show();
    }

    void Banner::OpenSongInBrowser()
    {
        set_rainbow(!rainbow);
    }

    void Banner::set_rainbow(bool value)
    {
        rainbow = value;
    }

    void Banner::Update()
    {
        // if we are not going to be rainbow, and we were just rainbow
        if (!rainbow && wasRainbow)
        {
            // set bg color to default
            wasRainbow = false;
            set_color(defaultColor);
        }
        else if (rainbow)
        {
            wasRainbow = true;
            // make this color dependent on some kind of rainbow / gradient source
            colorAngle += Time::get_deltaTime() * 0.1f;
            colorAngle = std::fmod(colorAngle, 1.0f);
            UnityEngine::Color color = UnityEngine::Color::HSVToRGB(colorAngle, 1.0f, 1.0f);
            set_color(color);
        }
    }

    void Banner::set_color(UnityEngine::Color color)
    {
        bgImage->set_color(color);
    }

    void Banner::set_loading(bool value)
    {
        loadingVertical->get_gameObject()->SetActive(value);
        if (value)
        {
            set_topText("");
            set_bottomText("");
        }
    }

    void Banner::set_ranking(int rank, float pp)
    {
        set_topText(string_format("<b><color=#FFDE1A>Global Ranking: </color></b>#%d<size=3> (<color=#6772E5>%.2fpp</color></size>)", rank, pp));
    }

    void Banner::set_status(std::string_view status)
    {
        set_bottomText(string_format("<b><color=#FFDE1A>Ranked Status:</color></b> %s", status.data()));
    }

    void Banner::set_topText(std::u16string_view newText)
    {
        topText->set_text(il2cpp_utils::newcsstr(u"<i>" + std::u16string(newText) + u"</i>"));
    }

    void Banner::set_bottomText(std::u16string_view newText)
    {
        bottomText->set_text(il2cpp_utils::newcsstr(u"<i>" + std::u16string(newText) + u"</i>"));
    }
}