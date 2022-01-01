#include "Utils/UIUtils.hpp"

#include "UnityEngine/Application.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "questui/shared/ArrayUtil.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "CustomTypes/DelayedCreateImage.hpp"

#include "Sprites.hpp"
#include "logging.hpp"
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

#define SetPreferredSize(identifier, width, height)                                         \
    auto layout##identifier = identifier->get_gameObject()->GetComponent<LayoutElement*>(); \
    if (!layout##identifier)                                                                \
        layout##identifier = identifier->get_gameObject()->AddComponent<LayoutElement*>();  \
    layout##identifier->set_preferredWidth(width);                                          \
    layout##identifier->set_preferredHeight(height)

#define SocialButton(identifier)                                                                                                                                                                                     \
    std::string str##identifier = member.get_##identifier();                                                                                                                                                         \
    if (str##identifier != "")                                                                                                                                                                                       \
    {                                                                                                                                                                                                                \
        auto identifier##_sprite = Base64ToSprite(identifier##_base64);                                                                                                                                              \
        auto btn##identifier = CreateClickableImage(socialHorizontal->get_transform(), identifier##_sprite, {0, 0}, {0, 0}, [str##identifier]() { Application::OpenURL(il2cpp_utils::newcsstr(str##identifier)); }); \
        SetPreferredSize(btn##identifier, 3, 3);                                                                                                                                                                     \
        AddHoverHint(btn##identifier->get_gameObject(), "Opens in Browser");                                                                                                                                         \
    }

namespace UIUtils
{
    ScoreSaber::ClickableText* CreateClickableText(UnityEngine::Transform* parent, std::u16string_view text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick)
    {
        auto clickableText = CreateClickableText(parent, text, anchoredPosition, sizeDelta);
        if (onClick)
            clickableText->get_onPointerClickEvent() += [onClick](auto _)
            { onClick(); };
        return clickableText;
    }
    ScoreSaber::ClickableText* CreateClickableText(UnityEngine::Transform* parent, std::u16string_view text, std::function<void()> onClick)
    {
        auto clickableText = CreateClickableText(parent, text);
        if (onClick)
            clickableText->get_onPointerClickEvent() += [onClick](auto _)
            { onClick(); };
        return clickableText;
    }

    ScoreSaber::ClickableText* CreateClickableText(Transform* parent, std::u16string_view text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta)
    {
        static auto name = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("QuestUIText");
        GameObject* gameObj = GameObject::New_ctor(name);
        gameObj->SetActive(false);

        ScoreSaber::ClickableText* textMesh = gameObj->AddComponent<ScoreSaber::ClickableText*>();
        RectTransform* rectTransform = textMesh->get_rectTransform();
        rectTransform->SetParent(parent, false);
        textMesh->set_font(GetMainTextFont());
        textMesh->set_fontSharedMaterial(GetMainUIFontMaterial());
        Il2CppString* text_cs = nullptr;
        text_cs = il2cpp_utils::newcsstr(text);
        textMesh->set_text(text_cs);
        textMesh->set_fontSize(4.0f);
        textMesh->set_color(UnityEngine::Color::get_white());
        textMesh->set_richText(true);
        rectTransform->set_anchorMin(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchorMax(UnityEngine::Vector2(0.5f, 0.5f));
        rectTransform->set_anchoredPosition(anchoredPosition);
        rectTransform->set_sizeDelta(sizeDelta);

        gameObj->AddComponent<LayoutElement*>();

        gameObj->SetActive(true);
        return textMesh;
    }

    ScoreSaber::ClickableText* CreateClickableText(UnityEngine::Transform* parent, std::string_view text, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta)
    {
        return CreateClickableText(parent, to_utf16(text), anchoredPosition, sizeDelta);
    };

    UnityEngine::GameObject* CreateLoadingIndicator(UnityEngine::Transform* parent)
    {
        auto original = QuestUI::ArrayUtil::First(UnityEngine::Resources::FindObjectsOfTypeAll<GameObject*>(), [](auto el)
                                                  { return to_utf8(csstrtostr(el->get_name())) == "LoadingIndicator"; });
        auto loadingIndicator = Object::Instantiate(original, parent, false);
        static auto scoreSaberLoadingIndicator = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("ScoreSaberLoadingIndicator");
        loadingIndicator->set_name(scoreSaberLoadingIndicator);

        loadingIndicator->AddComponent<LayoutElement*>();
        return loadingIndicator;
    }

    UnityEngine::UI::HorizontalLayoutGroup* CreateTeamMemberLayout(UnityEngine::Transform* parent, TeamUtils::TeamMember& member)
    {
        auto horizontal = CreateHorizontalLayoutGroup(parent);
        SetPreferredSize(horizontal, 30, -1);
        horizontal->set_spacing(2.0f);
        auto url = member.get_profilePicture();
        auto image = ScoreSaber::UI::DelayedCreateImage::CreateImage(horizontal->get_transform(), url, nullptr);
        SetPreferredSize(image, 12, 12);

        auto infoVertical = CreateVerticalLayoutGroup(horizontal->get_transform());
        SetPreferredSize(infoVertical, -1, 15);
        auto nameVertical = CreateVerticalLayoutGroup(infoVertical->get_transform());
        SetPreferredSize(nameVertical, 15, -1);
        auto socialHorizontal = CreateHorizontalLayoutGroup(infoVertical->get_transform());
        socialHorizontal->set_childAlignment(TextAnchor::LowerLeft);
        socialHorizontal->set_childForceExpandWidth(false);
        socialHorizontal->set_childForceExpandHeight(false);
        SetPreferredSize(socialHorizontal, 15, 5);

        auto nameText = CreateText(nameVertical->get_transform(), to_utf16(member.get_name()));
        SocialButton(discord);
        SocialButton(github);
        SocialButton(twitch);
        SocialButton(twitter);
        SocialButton(youtube);
        return horizontal;
    }
    ScoreSaber::ClickableImage* CreateClickableImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta, std::function<void()> onClick)
    {
        auto image = CreateClickableImage(parent, sprite, anchoredPosition, sizeDelta);
        if (onClick)
            image->get_onPointerClickEvent() += [onClick](auto _)
            { onClick(); };
        return image;
    }

    ScoreSaber::ClickableImage* CreateClickableImage(UnityEngine::Transform* parent, UnityEngine::Sprite* sprite, UnityEngine::Vector2 anchoredPosition, UnityEngine::Vector2 sizeDelta)
    {
        static auto name = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("ScoreSaberClickableImage");
        auto go = GameObject::New_ctor(name);

        auto image = go->AddComponent<ScoreSaber::ClickableImage*>();
        auto mat_UINoGlow = ArrayUtil::First(Resources::FindObjectsOfTypeAll<Material*>(), [](Material* x)
                                             { return to_utf8(csstrtostr(x->get_name())) == "UINoGlow"; });
        image->set_material(mat_UINoGlow);

        go->get_transform()->SetParent(parent, false);
        image->get_rectTransform()->set_sizeDelta(sizeDelta);
        image->get_rectTransform()->set_anchoredPosition(anchoredPosition);
        image->set_sprite(sprite);

        go->AddComponent<LayoutElement*>();
        return image;
    }

    HorizontalLayoutGroup* CreateHeader(UnityEngine::Transform* parent,
                                             UnityEngine::Color color) {
  VerticalLayoutGroup* vertical = CreateVerticalLayoutGroup(parent);
  vertical->get_rectTransform()->set_anchoredPosition({0.0f, 45.0f});
  HorizontalLayoutGroup* horizontal =
      CreateHorizontalLayoutGroup(vertical->get_transform());

  Backgroundable* background =
      horizontal->get_gameObject()->AddComponent<Backgroundable*>();
  background->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("panel-top"),
                                       1.0f);

  ImageView* imageView =
      background->get_gameObject()->GetComponentInChildren<ImageView*>();
  imageView->gradient = true;
  imageView->gradientDirection = 0;
  imageView->set_color(Color::get_white());
  imageView->set_color0(color);
  imageView->set_color1(color);
  imageView->curvedCanvasSettingsHelper->Reset();

  return horizontal;
}
}