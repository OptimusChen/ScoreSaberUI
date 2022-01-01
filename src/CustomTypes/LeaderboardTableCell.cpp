#include "CustomTypes/LeaderboardTableCell.hpp"
#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/Vector2.hpp"

#include "logging.hpp"

DEFINE_TYPE(ScoreSaber::UI, LeaderboardTableCell);

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

// TODO: Proper URL implementation
std::string country_url(std::string_view country)
{
    return string_format("https://some.url/%s", country.data());
}

namespace ScoreSaber::UI
{
    LeaderboardTableCell* LeaderboardTableCell::CreateTemplate()
    {
        auto go = GameObject::New_ctor();
        auto horizontal = CreateHorizontalLayoutGroup(go->get_transform());
        horizontal->set_childForceExpandWidth(false);
        horizontal->set_spacing(2.0f);
        auto leaderboardTableCell = horizontal->get_gameObject()->AddComponent<LeaderboardTableCell*>();
        horizontal->get_transform()->SetParent(nullptr);
        leaderboardTableCell->SetupCell();
        Object::Destroy(go);
        return leaderboardTableCell;
    }

    void LeaderboardTableCell::SetData(Data data)
    {
        // save data so we can re-use it here
        this->data = data;
        pfp->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(
            custom_types::Helpers::CoroutineHelper::New(
                LoadImage(pfp, data.pfpURL))));
        flag->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(
            custom_types::Helpers::CoroutineHelper::New(
                LoadImage(flag, country_url(data.country)))));
        playerName->set_text(il2cpp_utils::newcsstr(data.playerName));
        ranking->set_text(il2cpp_utils::newcsstr(data.ranking));
        pp->set_text(il2cpp_utils::newcsstr(data.pp));
        country->set_text(il2cpp_utils::newcsstr(data.country));
        history->set_text(il2cpp_utils::newcsstr(data.history));
    }

    void LeaderboardTableCell::SetupCell()
    {
        // setup pfp
        auto texture = Texture2D::get_whiteTexture();
        auto sprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);

        auto pfpVertical = CreateVerticalLayoutGroup(get_transform());
        pfpVertical->set_padding(RectOffset::New_ctor(1, 1, 1, 1));
        SetPreferredSize(pfpVertical, 12.0f, 12.0f);
        pfpVertical->set_childAlignment(TextAnchor::MiddleCenter);
        pfp = UIUtils::CreateClickableImage(pfpVertical->get_transform(), sprite, {0, 0}, {0, 0}, std::bind(&LeaderboardTableCell::OpenPlayerInfoModal, this));

        // setup name text & ranking text
        auto nameRankingVertical = CreateVerticalLayoutGroup(get_transform());
        nameRankingVertical->set_childAlignment(TextAnchor::MiddleLeft);
        playerName = UIUtils::CreateClickableText(nameRankingVertical->get_transform(), u"Placeholder", std::bind(&LeaderboardTableCell::OpenPlayerInfoModal, this));
        ranking = CreateText(nameRankingVertical->get_transform(), "#placeholder");

        // setup pp, country flag & country text
        auto ppCountryVertical = CreateVerticalLayoutGroup(get_transform());
        ppCountryVertical->set_childAlignment(TextAnchor::MiddleLeft);
        pp = CreateText(ppCountryVertical->get_transform(), "placeholderPP");

        auto countryHorizontal = CreateHorizontalLayoutGroup(ppCountryVertical->get_transform());
        auto flagVertical = CreateVerticalLayoutGroup(countryHorizontal->get_transform());
        SetPreferredSize(flagVertical, 4.0f, 2.0f);
        flagVertical->set_childForceExpandWidth(false);
        flagVertical->set_childForceExpandHeight(false);
        flagVertical->set_childAlignment(TextAnchor::MiddleCenter);
        flag = CreateImage(flagVertical->get_transform(), sprite, {0, 0}, {0, 0});
        SetPreferredSize(flag, 4.0f, 2.0f);

        country = CreateText(countryHorizontal->get_transform(), "XX");
        // setup history text
        auto historyVertical = CreateVerticalLayoutGroup(get_transform());
        historyVertical->set_childAlignment(TextAnchor::MiddleLeft);
        history = CreateText(historyVertical->get_transform(), "<color=green>+69</color>");
        inited = true;
    }

    custom_types::Helpers::Coroutine LeaderboardTableCell::LoadImage(HMUI::ImageView* out, std::string_view url)
    {
        // load the pfp async :)
        INFO("Loading url %s", url.data());
        UnityEngine::Networking::UnityWebRequest* www = UnityEngine::Networking::UnityWebRequestTexture::GetTexture(il2cpp_utils::newcsstr(url));
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(www->SendWebRequest());
        auto downloadHandlerTexture = reinterpret_cast<UnityEngine::Networking::DownloadHandlerTexture*>(www->get_downloadHandler());
        auto texture = downloadHandlerTexture->get_texture();
        INFO("got texture %p", texture);
        auto sprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        out->set_sprite(sprite);

        co_return;
    }

    void LeaderboardTableCell::OpenPlayerInfoModal()
    {
        /// idk yet how but we gotta open this, perhaps set a leaderboardviewcontroller field and use that ? or just pass in the modal to open?
    }
}