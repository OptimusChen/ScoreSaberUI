#include "UI/Other/PlayerProfileModal.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ImageView.hpp"

#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

DEFINE_TYPE(ScoreSaber::UI, PlayerProfileModal);

using namespace HMUI;
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

#define WIDTH 80.0f
#define HEIGHT 60.0f
namespace ScoreSaber::UI
{
    PlayerProfileModal* PlayerProfileModal::Create(UnityEngine::Transform* parent)
    {
        auto modal = CreateModal(parent, Vector2(WIDTH, HEIGHT), nullptr);
        auto ppmodal = modal->get_gameObject()->AddComponent<PlayerProfileModal*>();
        ppmodal->modal = modal;
        ppmodal->Setup();
        return ppmodal;
    }

    void PlayerProfileModal::Show()
    {
        modal->Show(true, true, nullptr);
    }

    void PlayerProfileModal::Setup()
    {
        auto vertical = CreateVerticalLayoutGroup(get_transform());
        SetPreferredSize(vertical, WIDTH, HEIGHT);

        // header stuff
        auto headerHorizon = CreateHorizontalLayoutGroup(vertical->get_transform());
        SetPreferredSize(headerHorizon, WIDTH, HEIGHT * 0.1f);

        auto bg = headerHorizon->get_gameObject()->AddComponent<Backgroundable*>();
        bg->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("title-gradient"), 1.0f);

        auto bgImage = bg->get_gameObject()->GetComponentInChildren<ImageView*>();
        bgImage->dyn__gradient() = false;
        bgImage->set_color0(Color(1, 1, 1, 1));
        bgImage->set_color1(Color(1, 1, 1, 1));

        // placeholder color
        bgImage->set_color(Color(1, 0, 0, 1));
        bgImage->dyn__curvedCanvasSettingsHelper()->Reset();

        headerText = CreateText(headerHorizon->get_transform(), "Profile Placeholder");
        SetPreferredSize(headerText, WIDTH, HEIGHT * 0.1f);
        headerHorizon->set_childAlignment(TextAnchor::MiddleCenter);
        headerText->set_alignment(TMPro::TextAlignmentOptions::Center);
        // actual data stuff
        auto dataHorizon = CreateHorizontalLayoutGroup(vertical->get_transform());
        dataHorizon->set_padding(RectOffset::New_ctor(2, 2, 2, 2));

        auto pfpVertical = CreateVerticalLayoutGroup(dataHorizon->get_transform());
        auto seperatorVertical = CreateVerticalLayoutGroup(dataHorizon->get_transform());
        auto dataVertical = CreateVerticalLayoutGroup(dataHorizon->get_transform());

        SetPreferredSize(pfpVertical, WIDTH * 0.4f, HEIGHT * 0.75f);
        SetPreferredSize(seperatorVertical, 1.0f, HEIGHT * 0.75f);
        SetPreferredSize(dataVertical, WIDTH * 0.4f, HEIGHT * 0.75f);

        // pfp setup
        // TODO: Add actual placeholder
        auto pfpHorizontal = CreateHorizontalLayoutGroup(pfpVertical->get_transform());
        SetPreferredSize(pfpHorizontal, WIDTH * 0.4f, WIDTH * 0.4f);
        auto oculusSprite = Base64ToSprite(oculus_base64);
        pfpImage = CreateImage(pfpHorizontal->get_transform(), oculusSprite, Vector2(0, 0), Vector2(0, 0));
        SetPreferredSize(pfpImage, WIDTH * 0.4f, WIDTH * 0.4f);

        badgeHorizontal = CreateHorizontalLayoutGroup(pfpVertical->get_transform());
        badgeHorizontal->set_spacing(1.0f);
        AddBadge(oculusSprite);
        AddBadge(oculusSprite);
        AddBadge(oculusSprite);
        AddBadge(oculusSprite);
        // seperator setup
        auto texture = Texture2D::get_whiteTexture();
        auto seperatorSprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);

        auto image = CreateImage(seperatorVertical->get_transform(), seperatorSprite, Vector2(0, 0), Vector2(0, 0));
        auto imageLayout = image->get_gameObject()->AddComponent<LayoutElement*>();
        imageLayout->set_preferredWidth(1.0f);

        // data setup
        CreateText(dataVertical->get_transform(), "Global Player Ranking");
        globalRanking = CreateText(dataVertical->get_transform(), "#Placeholder");
        CreateText(dataVertical->get_transform(), "Performance Points");
        performancePoints = CreateText(dataVertical->get_transform(), "Placeholder pp");
        CreateText(dataVertical->get_transform(), "Average Ranked Accuracy");
        averageRankedAccuracy = CreateText(dataVertical->get_transform(), "Placeholder%");
        CreateText(dataVertical->get_transform(), "Total Score");
        totalScore = CreateText(dataVertical->get_transform(), "Placeholder");

        set_player("Placeholder");
        set_globalRanking(0);
        set_performancePoints(420.69f);
        set_averageRankedAccuracy(69.69f);
        set_totalScore(42042069);
    }

    void PlayerProfileModal::ClearBadges()
    {
        auto transform = badgeHorizontal->get_transform();
        UnityEngine::Transform* child;
        while ((child = transform->GetChild(0)))
        {
            Object::DestroyImmediate(child->get_gameObject());
        }
    }

    void PlayerProfileModal::AddBadge(UnityEngine::Sprite* badge)
    {
        constexpr const float generalSize = 5.0f;
        auto rect = badge->get_textureRect();
        auto badgeVertical = CreateVerticalLayoutGroup(badgeHorizontal->get_transform());
        Vector2 ratio = {rect.get_width(), rect.get_height()};
        ratio.Normalize();
        ratio.x *= generalSize;
        ratio.y *= generalSize;
        //SetPreferredSize(badgeVertical, ratio.x, ratio.y);
        auto image = CreateImage(badgeHorizontal->get_transform(), badge, Vector2(0, 0), Vector2(0, 0));
        SetPreferredSize(image, ratio.x, ratio.y);
    }

    void PlayerProfileModal::set_player(std::string_view header)
    {
        set_header(string_format("%s's Profile", header.data()));
    }

    void PlayerProfileModal::set_header(std::string_view header)
    {
        headerText->set_text(il2cpp_utils::newcsstr(string_format("<i>%s</i>", header.data())));
    }

    // TODO: consider changing these to use strings and actually properly format things, for now this'll do though
    void PlayerProfileModal::set_globalRanking(int globalRanking)
    {
        this->globalRanking->set_text(il2cpp_utils::newcsstr(string_format("<i>#%d</i>", globalRanking)));
    }

    void PlayerProfileModal::set_performancePoints(float performancePoints)
    {
        this->performancePoints->set_text(il2cpp_utils::newcsstr(string_format("<i><color=#6772E5>%.2fpp</color></i>", performancePoints)));
    }

    void PlayerProfileModal::set_averageRankedAccuracy(float averageRankedAccuracy)
    {
        this->averageRankedAccuracy->set_text(il2cpp_utils::newcsstr(string_format("<i>%.2f</i>", averageRankedAccuracy)));
    }

    void PlayerProfileModal::set_totalScore(long totalScore)
    {
        this->totalScore->set_text(il2cpp_utils::newcsstr(string_format("<i>%ld</i>", totalScore)));
    }
}