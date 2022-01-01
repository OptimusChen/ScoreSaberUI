#include "UI/LinksViewController.hpp"
#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "UnityEngine/Application.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

#include "TMPro/TextAlignmentOptions.hpp"
#include "logging.hpp"

#include "Sprites.hpp"

DEFINE_TYPE(ScoreSaber::UI, LinksViewController);

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

#define SCORESABER_LINK "https://scoresaber.com/"
#define SCORESABER_DISCORD "https://discord.gg/scoresaber"
#define SCORESABER_TWITTER "https://twitter.com/scoresaber"
#define SCORESABER_PATREON "https://www.patreon.com/scoresaber"

#define BSMG_LINK "https://bsmg.wiki/"
#define BSMG_DISCORD "https://discord.gg/beatsabermods"
#define BSMG_TWITTER "https://twitter.com/beatsabermods"
#define BSMG_PATREON "https://www.patreon.com/beatsabermods"

namespace ScoreSaber::UI
{
    void LinksViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            auto vertical = CreateVerticalLayoutGroup(get_transform());
            vertical->set_spacing(1.0f);

            auto headerHorizontal = CreateHorizontalLayoutGroup(vertical->get_transform());
            SetPreferredSize(headerHorizontal, 80.0f, 10.0f);
            headerHorizontal->set_childAlignment(TextAnchor::MiddleCenter);
            auto headerText = CreateText(headerHorizontal->get_transform(), "Links");
            headerText->set_alignment(TMPro::TextAlignmentOptions::Center);
            headerText->set_fontSize(7.0f);
            auto headerBG = headerHorizontal->get_gameObject()->AddComponent<Backgroundable*>();
            headerBG->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 0.5f);

            auto linkHorizontal = CreateHorizontalLayoutGroup(vertical->get_transform());
            linkHorizontal->set_spacing(2.0f);
            SetPreferredSize(linkHorizontal, 80.0f, 60.0f);

            auto scoreSaberVertical = CreateVerticalLayoutGroup(linkHorizontal->get_transform());
            auto bsmgVertical = CreateVerticalLayoutGroup(linkHorizontal->get_transform());
            scoreSaberVertical->set_padding(RectOffset::New_ctor(4, 4, 4, 4));
            bsmgVertical->set_padding(RectOffset::New_ctor(4, 4, 4, 4));
            scoreSaberVertical->set_childAlignment(TextAnchor::UpperCenter);
            bsmgVertical->set_childAlignment(TextAnchor::UpperCenter);

            SetPreferredSize(scoreSaberVertical, 39.0f, 60.0f);
            SetPreferredSize(bsmgVertical, 39.0f, 60.0f);

            auto scoreSaberBG = scoreSaberVertical->get_gameObject()->AddComponent<Backgroundable*>();
            auto bsmgBG = bsmgVertical->get_gameObject()->AddComponent<Backgroundable*>();
            scoreSaberBG->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 1.0f);
            bsmgBG->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("round-rect-panel"), 1.0f);
            auto bsmgImageView = bsmgBG->GetComponent<HMUI::ImageView*>();
            bsmgImageView->set_color(Color::get_magenta());
            auto scoreSaberImageView = scoreSaberBG->GetComponent<HMUI::ImageView*>();
            scoreSaberImageView->set_color(Color::get_cyan());

            // ScoreSaber stuff
            //TODO: Implement the actual URLs opening, for now it just logs the button press so it's a thing we can see in the logs
            auto SSlink = UIUtils::CreateClickableText(
                scoreSaberVertical->get_transform(), u"ScoreSaber", []()
                {
                    static auto SSlinkURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(SCORESABER_LINK);
                    Application::OpenURL(SSlinkURL);
                });
            SSlink->set_alignment(TMPro::TextAlignmentOptions::Center);
            SSlink->set_fontSize(6.5f);

            auto ssDiscord = CreateUIButton(scoreSaberVertical->get_transform(), "Discord", []()
                                            {
                                                static auto ssDiscordURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(SCORESABER_DISCORD);
                                                Application::OpenURL(ssDiscordURL);
                                            });
            auto ssTwitter = CreateUIButton(scoreSaberVertical->get_transform(), "Twitter", []()
                                            {
                                                static auto ssTwitterURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(SCORESABER_TWITTER);
                                                Application::OpenURL(ssTwitterURL);
                                            });

            auto ssPatreon = CreateUIButton(scoreSaberVertical->get_transform(), "Patreon", []()
                                            {
                                                static auto ssPatreonURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(SCORESABER_PATREON);
                                                Application::OpenURL(ssPatreonURL);
                                            });

            SetPreferredSize(ssDiscord, 20.0f, 8.0f);
            SetPreferredSize(ssTwitter, 20.0f, 8.0f);
            SetPreferredSize(ssPatreon, 20.0f, 8.0f);
            auto SSprite = Base64ToSprite(ScoreSaber_Active);
            auto SSpriteVertical = CreateVerticalLayoutGroup(scoreSaberVertical->get_transform());
            auto SSimage = CreateImage(SSpriteVertical->get_transform(), SSprite, Vector2(0, 0), Vector2(0, 0));
            SetPreferredSize(SSpriteVertical, 20.0f, 20.0f);
            SetPreferredSize(SSimage, 20.0f, 20.0f);

            // BSMG stuff
            auto BSMGlink = UIUtils::CreateClickableText(
                bsmgVertical->get_transform(), u"BSMG", []()
                {
                    static auto BSMGlinkURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(BSMG_LINK);
                    Application::OpenURL(BSMGlinkURL);
                });
            BSMGlink->set_alignment(TMPro::TextAlignmentOptions::Center);
            BSMGlink->set_fontSize(6.5f);

            auto bsmgDiscord = CreateUIButton(bsmgVertical->get_transform(), "Discord", []()
                                              {
                                                  static auto bsmgDiscordURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(BSMG_DISCORD);
                                                  Application::OpenURL(bsmgDiscordURL);
                                              });
            auto bsmgTwitter = CreateUIButton(bsmgVertical->get_transform(), "Twitter", []()
                                              {
                                                  static auto bsmgTwitterURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(BSMG_TWITTER);
                                                  Application::OpenURL(bsmgTwitterURL);
                                              });
            auto bsmgPatreon = CreateUIButton(bsmgVertical->get_transform(), "Patreon", []()
                                              {
                                                  static auto bsmgPatreonURL = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(BSMG_PATREON);
                                                  Application::OpenURL(bsmgPatreonURL);
                                              });
            SetPreferredSize(bsmgDiscord, 20.0f, 8.0f);
            SetPreferredSize(bsmgTwitter, 20.0f, 8.0f);
            SetPreferredSize(bsmgPatreon, 20.0f, 8.0f);

            // TODO: Proper BSMG image
            auto BSMGsprite = Base64ToSprite(bsmg_base64);

            auto BSMGspriteVertical = CreateVerticalLayoutGroup(bsmgVertical->get_transform());
            auto BSMGimage = CreateImage(BSMGspriteVertical->get_transform(), BSMGsprite, Vector2(0, 0), Vector2(0, 0));
            SetPreferredSize(BSMGspriteVertical, 20.0f, 20.0f);
            SetPreferredSize(BSMGimage, 20.0f, 20.0f);
        }
    }
}