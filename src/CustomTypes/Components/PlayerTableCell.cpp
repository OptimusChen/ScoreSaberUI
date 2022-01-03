#include "CustomTypes/Components/PlayerTableCell.hpp"

#include "CustomTypes/Components/ImageButton.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/Touchable.hpp"
#include "Sprites.hpp"
#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/WebUtils.hpp"
#include "logging.hpp"
#include "main.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::CustomTypes::Components, PlayerTableCell);

using namespace ScoreSaberUI::CustomTypes::Components;
using namespace ScoreSaberUI::Utils::StringUtils;
using namespace ScoreSaberUI::Utils;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace TMPro;

#define BeginCoroutine(method)                                               \
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine( \
        reinterpret_cast<System::Collections::IEnumerator*>(                 \
            custom_types::Helpers::CoroutineHelper::New(method)));

custom_types::Helpers::Coroutine WaitForImageDownload(std::string url, HMUI::ImageView* out)
{
    UnityEngine::Networking::UnityWebRequest* www = UnityEngine::Networking::UnityWebRequestTexture::GetTexture(il2cpp_utils::newcsstr(url));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(www->SendWebRequest());
    auto downloadHandlerTexture = reinterpret_cast<UnityEngine::Networking::DownloadHandlerTexture*>(www->get_downloadHandler());
    auto texture = downloadHandlerTexture->get_texture();
    auto sprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
    out->set_sprite(sprite);
    co_return;
}

void PlayerTableCell::ctor() {}

VerticalLayoutGroup* CreateHost(Transform* parent, Vector2 anchoredPos,
                                Vector2 size)
{
    VerticalLayoutGroup* group = BeatSaberUI::CreateVerticalLayoutGroup(parent);
    group->get_rectTransform()->set_anchoredPosition(anchoredPos);

    LayoutElement* elem = group->GetComponent<LayoutElement*>();
    elem->set_preferredHeight(size.y);
    elem->set_preferredWidth(size.x);

    ContentSizeFitter* fitter = group->GetComponent<ContentSizeFitter*>();
    fitter->set_verticalFit(ContentSizeFitter::FitMode::PreferredSize);
    fitter->set_horizontalFit(ContentSizeFitter::FitMode::PreferredSize);
    return group;
}

std::string flag_url(std::string_view COUNTRY)
{
    auto country = std::string(COUNTRY);
    for (auto& c : country)
        c = tolower(c);
    auto url = string_format("https://github.com/hampusborgos/country-flags/raw/main/png250px/%s.png", country.c_str());
    return url;
}

void PlayerTableCell::Refresh(
    rapidjson::GenericObject<true, rapidjson::Value> player)
{
    this->StopAllCoroutines();
    std::string iconPath =
        "/sdcard/ModData/com.beatgames.beatsaber/"
        "Mods/ScoreSaberUI/Icons/";
    auto profilePictureItr = player.FindMember("profilePicture");
    if (profilePictureItr != player.MemberEnd() && profilePictureItr->value.IsString())
    {
        std::string profilePictureURL = profilePictureItr->value.GetString();
        INFO("Getting profile picture @ %s", profilePictureURL.c_str());
        profile->set_sprite(Base64ToSprite(oculus_base64));
        // if it ends with oculus.png then there is no reason to redownload the image, so let's not redownload it :)
        if (!profilePictureURL.ends_with("oculus.png"))
        {
            BeginCoroutine(WaitForImageDownload(profilePictureURL, profile));
        }
        /*
        WebUtils::GetAsync(profilePictureURL, 64,
                           [=](long httpCode, std::string data)
                           {
                               if (httpCode == 200)
                               {
                                   std::vector<uint8_t> bytes(data.begin(), data.end());
                                   MainThreadScheduler::Schedule([=]()
                                                                 {
                                                                     getLogger().info("test9");
                                                                     Sprite* profilePicture =
                                                                         BeatSaberUI::VectorToSprite(bytes);
                                                                     profile->set_sprite(profilePicture);
                                                                     getLogger().info("test10");
                                                                 });
                               };
                           });
                           */
    }

    auto nameItr = player.FindMember("name");
    if (nameItr != player.MemberEnd() && nameItr->value.IsString())
    {
        std::string name = nameItr->value.GetString();
        INFO("Setting playername %s", name.c_str());
        this->name->set_text(StrToIl2cppStr(name));
    }

    auto rankItr = player.FindMember("rank");
    if (rankItr != player.MemberEnd() && rankItr->value.IsInt())
    {
        int rank = rankItr->value.GetInt();
        INFO("Setting rank %d", rank);
        this->rank->set_text(StrToIl2cppStr(string_format("#%d", rank)));
    }

    auto ppItr = player.FindMember("pp");
    if (ppItr != player.MemberEnd() && ppItr->value.IsDouble())
    {
        double pp = ppItr->value.GetDouble();
        INFO("Setting pp %.2f", pp);
        this->pp->set_text(StrToIl2cppStr(string_format("<color=#6872e5>%.0fpp<color>", pp)));
    }

    auto countryItr = player.FindMember("country");
    if (countryItr != player.MemberEnd() && countryItr->value.IsString())
    {
        std::string country = countryItr->value.GetString();
        INFO("Setting country %s", country.c_str());
        flag->set_sprite(Base64ToSprite(country_base64));
        BeginCoroutine(WaitForImageDownload(flag_url(country), flag));
        /*
        WebUtils::GetAsync(flag_url(country), 64,
                           [=](long httpCode, std::string data)
                           {
                               if (httpCode == 200)
                               {
                                   std::vector<uint8_t> bytes(data.begin(), data.end());
                                   MainThreadScheduler::Schedule([=]()
                                                                 {
                                                                     getLogger().info("test11");
                                                                     Sprite* flagPicture =
                                                                         BeatSaberUI::VectorToSprite(bytes);
                                                                     flag->set_sprite(flagPicture);
                                                                     getLogger().info("test12");
                                                                 });
                               };
                           });
                           */
        this->country->set_text(StrToIl2cppStr(country));
    }

    auto historiesItr = player.FindMember("histories");
    if (historiesItr != player.MemberEnd() && historiesItr->value.IsString())
    {
        ::Array<Il2CppString*>* histories =
            StringUtils::StrToIl2cppStr(player["histories"].GetString())->Split(',');
        auto length = histories->Length();
        int weeklyChange;
        if (length < 48)
        {
            if (length > 7)
                weeklyChange = stof(StringUtils::Il2cppStrToStr(histories->get(length - 8))) -
                               stof(StringUtils::Il2cppStrToStr(histories->get(length - 1)));
            else if (length != 0)
                weeklyChange = stof(StringUtils::Il2cppStrToStr(histories->get(0))) -
                               stof(StringUtils::Il2cppStrToStr(histories->get(length - 1)));
            else
                weeklyChange = 0;
        }
        else
        {
            weeklyChange = stof(StringUtils::Il2cppStrToStr(histories->get(41))) -
                           stof(StringUtils::Il2cppStrToStr(histories->get(48)));
        }
        std::string result;
        if (weeklyChange > 0)
        {
            result = string_format("<color=green>+%d</color>", weeklyChange);
        }
        else if (weeklyChange < 0)
        {
            result = string_format("<color=red>%d</color>", weeklyChange);
        }
        else
        {
            result = string_format("%d", weeklyChange);
        }
        weekly->set_text(StrToIl2cppStr(result));
    }
}

PlayerTableCell* PlayerTableCell::CreateCell()
{
    static auto playerTableCellStr = il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>("CustomPlayerTableCell");
    auto cellGO = UnityEngine::GameObject::New_ctor();
    auto playerCell = cellGO->AddComponent<PlayerTableCell*>();
    cellGO->set_name(playerTableCellStr);

    cellGO->AddComponent<HMUI::Touchable*>();
    playerCell->set_interactable(false);

    auto verticalLayoutGroup = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(
        playerCell->get_transform());

    auto layout = verticalLayoutGroup->get_gameObject()
                      ->GetComponent<UnityEngine::UI::LayoutElement*>();
    layout->set_preferredHeight(11.0f);
    layout->set_preferredWidth(200.0f);

    Transform* t = playerCell->get_transform();

    playerCell->profile = BeatSaberUI::CreateImage(
        CreateHost(t, {-45.0f, 0.0f}, {10.0f, 10.0f})->get_transform(),
        Base64ToSprite(oculus_base64), {0.0f, 0.0f},
        {10.0f, 10.0f});

    playerCell->name = BeatSaberUI::CreateText(
        CreateHost(t, {-11.0f, 2.8f}, {55.0f, 8.0f})->get_transform(),
        "Username", false, {0.0f, 0.0f});
    playerCell->name->set_overflowMode(TextOverflowModes::Ellipsis);
    playerCell->name->set_alignment(TextAlignmentOptions::Left);
    playerCell->name->set_fontSize(5.0f);

    playerCell->rank = BeatSaberUI::CreateText(
        CreateHost(t, {-18.0f, -2.0f}, {40.0f, 8.0f})->get_transform(),
        "#---", false,
        {0.0f, 0.0f});
    playerCell->rank->set_alignment(TextAlignmentOptions::Left);

    playerCell->pp = BeatSaberUI::CreateText(
        CreateHost(t, {27.0f, 0.0f}, {20.0f, 11.0f})->get_transform(),
        "---<color=#6872e5>pp</color>", false, {0.0f, 0.0f});
    playerCell->pp->set_fontSize(5.0f);
    playerCell->pp->set_overflowMode(TextOverflowModes::Ellipsis);

    playerCell->flag = BeatSaberUI::CreateImage(
        CreateHost(t, {19.42f, -1.65f}, {4.0f, 3.0f})->get_transform(),
        Base64ToSprite(country_base64), {0.0f, 0.0f},
        {4.0, 3.0f});
    playerCell->flag->set_preserveAspect(true);

    playerCell->country = BeatSaberUI::CreateText(
        CreateHost(t, {31.0f, -2.0f}, {17.0f, 0.0f})->get_transform(), "N/A", false, {0.0f, 0.0f});
    playerCell->country->set_alignment(TextAlignmentOptions::Left);
    playerCell->country->set_fontSize(3.5f);

    playerCell->weekly = BeatSaberUI::CreateText(
        CreateHost(t, {41.0f, -1.0f}, {15.0f, 0.0f})->get_transform(), "0", false,
        {0.0f, 0.0f});
    playerCell->weekly->set_alignment(TextAlignmentOptions::Right);
    playerCell->weekly->set_fontSize(5.0f);
    return playerCell;
}
