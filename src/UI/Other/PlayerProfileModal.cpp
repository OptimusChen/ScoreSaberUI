#include "UI/Other/PlayerProfileModal.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/ImageView.hpp"

#include "GlobalNamespace/SharedCoroutineStarter.hpp"

#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

#include "Sprites.hpp"
#include "logging.hpp"

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

#define BeginCoroutine(method)                                               \
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine( \
        reinterpret_cast<System::Collections::IEnumerator*>(                 \
            custom_types::Helpers::CoroutineHelper::New(method)))

static custom_types::Helpers::Coroutine WaitForImageDownload(std::string url, HMUI::ImageView* out)
{
    UnityEngine::Networking::UnityWebRequest* www = UnityEngine::Networking::UnityWebRequestTexture::GetTexture(il2cpp_utils::newcsstr(url));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(www->SendWebRequest());
    auto downloadHandlerTexture = reinterpret_cast<UnityEngine::Networking::DownloadHandlerTexture*>(www->get_downloadHandler());
    auto texture = downloadHandlerTexture->get_texture();
    auto sprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
    out->set_sprite(sprite);
    co_return;
}

#define WIDTH 80.0f
#define HEIGHT 60.0f
namespace ScoreSaber::UI
{
    custom_types::Helpers::Coroutine PlayerProfileModal::FetchPlayerData(std::string playerId)
    {
        if (playerId == "")
            co_return;
        std::string url = string_format("https://scoresaber.com/api/player/%s/full", playerId.c_str());
        INFO("Getting player data from url %s", url.c_str());
        UnityEngine::Networking::UnityWebRequest* webRequest =
            UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::newcsstr(url));
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(
            CRASH_UNLESS(webRequest->SendWebRequest()));
        if (!webRequest->get_isNetworkError())
        {
            std::u16string response = std::u16string(csstrtostr(webRequest->get_downloadHandler()->get_text()));
            rapidjson::GenericDocument<rapidjson::UTF16<char16_t>> doc;
            doc.Parse(response.c_str());
            ScoreSaber::Data::Player player(doc.GetObject());

            SetPlayerData(player);
        }
        co_return;
    }

    void PlayerProfileModal::SetPlayerData(ScoreSaber::Data::Player& player)
    {
        set_player(player.name);
        set_globalRanking(player.rank);
        set_performancePoints(player.pp);
        set_averageRankedAccuracy(player.scoreStats->averageRankedAccuracy);
        set_totalScore(player.scoreStats->totalScore);

        profileRoutine = BeginCoroutine(WaitForImageDownload(player.profilePicture, pfpImage));
        for (auto& badge : player.badges)
        {
            AddBadge(badge);
        }
    }

    PlayerProfileModal* PlayerProfileModal::Create(UnityEngine::Transform* parent)
    {
        auto modal = CreateModal(parent, Vector2(WIDTH, HEIGHT), nullptr);
        auto ppmodal = modal->get_gameObject()->AddComponent<PlayerProfileModal*>();
        ppmodal->modal = modal;
        ppmodal->Setup();
        return ppmodal;
    }

    void PlayerProfileModal::Hide()
    {
        // TODO: stop the method(s) that is/are setting player data
        modal->Hide(true, nullptr);
        stopProfileRoutine();
        stopBadgeRoutines();
    }

    void PlayerProfileModal::Show(std::string playerId)
    {
        INFO("Showing modal: %p", modal);
        modal->Show(true, true, nullptr);
        INFO("Clearing badges: %p", badgeHorizontal);
        ClearBadges();
        INFO("Starting getting player data for player %s", playerId.c_str());
        BeginCoroutine(FetchPlayerData(playerId));
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
        auto pfpHorizontal = CreateHorizontalLayoutGroup(pfpVertical->get_transform());
        SetPreferredSize(pfpHorizontal, WIDTH * 0.4f, WIDTH * 0.4f);
        auto oculusSprite = Base64ToSprite(oculus_base64);
        pfpImage = CreateImage(pfpHorizontal->get_transform(), oculusSprite, Vector2(0, 0), Vector2(0, 0));
        SetPreferredSize(pfpImage, WIDTH * 0.4f, WIDTH * 0.4f);

        badgeHorizontal = CreateHorizontalLayoutGroup(pfpVertical->get_transform());
        badgeHorizontal->set_spacing(1.0f);

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

        badgeRoutines = List<UnityEngine::Coroutine*>::New_ctor();
        set_player(u"Placeholder");
        set_globalRanking(0);
        set_performancePoints(420.69f);
        set_averageRankedAccuracy(69.69f);
        set_totalScore(42042069);
    }

    void PlayerProfileModal::ClearBadges()
    {
        if (!badgeHorizontal)
            return;
        auto transform = badgeHorizontal->get_transform();
        INFO("Badge transform: %p", transform);
        int childCount = transform->get_childCount();
        INFO("childCount: %d", childCount);
        for (int i = childCount - 1; i >= 0; i--)
        {
            auto child = transform->GetChild(i);
            INFO("child: %p", child);
            auto go = child->get_gameObject();
            INFO("child GO: %p", go);
            Object::DestroyImmediate(go);
        }
        /*
        while (child)
        {
            auto go = child->get_gameObject();
            INFO("child GO: %p", go);
            Object::DestroyImmediate(go);
            child = transform->GetChild(0);
        }
        */
    }

    void PlayerProfileModal::AddBadge(ScoreSaber::Data::Badge& badge)
    {
        constexpr const float generalSize = 5.0f;
        auto badgeVertical = CreateVerticalLayoutGroup(badgeHorizontal->get_transform());
        //SetPreferredSize(badgeVertical, ratio.x, ratio.y);'
        auto texture = Texture2D::get_blackTexture();
        auto sprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        auto image = CreateImage(badgeHorizontal->get_transform(), sprite, Vector2(0, 0), Vector2(0, 0));
        //TODO: fix size
        SetPreferredSize(image, 2, 3);
        badgeRoutines->Add(BeginCoroutine(WaitForImageDownload(badge.image, image)));
        AddHoverHint(badgeVertical->get_gameObject(), badge.description);

        image->set_preserveAspect(true);
    }

    void PlayerProfileModal::set_player(std::u16string_view header)
    {
        set_header(std::u16string(header) + u"'s Profile");
    }

    void PlayerProfileModal::set_header(std::u16string_view header)
    {
        headerText->set_text(il2cpp_utils::newcsstr(u"<i>" + std::u16string(header) + u"</i>"));
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

    void PlayerProfileModal::stopProfileRoutine()
    {
        if (profileRoutine)
            GlobalNamespace::SharedCoroutineStarter::get_instance()->StopCoroutine(profileRoutine);
        profileRoutine = nullptr;
    }

    void PlayerProfileModal::stopBadgeRoutines()
    {
        if (badgeRoutines)
        {
            int length = badgeRoutines->get_Count();
            for (int i = 0; i < length; i++)
            {
                auto routine = badgeRoutines->get_Item(i);
                GlobalNamespace::SharedCoroutineStarter::get_instance()->StopCoroutine(routine);
            }

            badgeRoutines->Clear();
        }
    }

}