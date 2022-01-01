#include "CustomTypes/Components/CustomCellTableData.hpp"
#include "CustomTypes/Components/PlayerTableCell.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "HMUI/Touchable.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/UIUtils.hpp"
#include "Utils/WebUtils.hpp"
#include "main.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::CustomTypes::Components, CustomCellListTableData);

using namespace ScoreSaberUI::Utils;
using namespace ScoreSaberUI::CustomTypes::Components;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace TMPro;

#define BeginCoroutine(method)                                             \
  GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine( \
      reinterpret_cast<System::Collections::IEnumerator*>(                 \
          custom_types::Helpers::CoroutineHelper::New(method)));

float ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::
    CellSize() {
  return 11.0f;
}
int ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::
    NumberOfCells() {
  return 5;
}

rapidjson::Document document;

custom_types::Helpers::Coroutine GetDocument(
    ScoreSaberUI::CustomTypes::Components::CustomCellListTableData* self) {
  UnityEngine::Networking::UnityWebRequest* webRequest =
      UnityEngine::Networking::UnityWebRequest::Get(StringUtils::StrToIl2cppStr(
          "https://scoresaber.com/api/players?page=" +
          std::to_string(self->page) + "&withMetadata=false"));
  co_yield reinterpret_cast<System::Collections::IEnumerator*>(
      CRASH_UNLESS(webRequest->SendWebRequest()));
  if (!webRequest->get_isNetworkError()) {
    std::string s = StringUtils::to_utf8(
        csstrtostr(webRequest->get_downloadHandler()->get_text()));
    getLogger().info("%s", s.c_str());
    document.Parse(s);
    self->initialized = true;
  }
  co_return;
}

void ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::ctor() {
  page = 1;
  page2 = 1;
  GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
      reinterpret_cast<System::Collections::IEnumerator*>(
          custom_types::Helpers::CoroutineHelper::New(GetDocument(this))));
}

void ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::
    DownButtonWasPressed() {
  getLogger().info("DownButtonWasPressed test1");
  page2 = page2 + 1;
  getLogger().info("DownButtonWasPressed test2");
  if (page2 % 50 == 0) {
    page2 = 1;
    page = page + 1;
  }
  getLogger().info("DownButtonWasPressed test3");
  GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
      reinterpret_cast<System::Collections::IEnumerator*>(
          custom_types::Helpers::CoroutineHelper::New(Refresh())));
  getLogger().info("DownButtonWasPressed test4");
}

void ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::
    UpButtonWasPressed() {
  if (page != 1 && page2 != 1) {
    page2--;
    if (page2 % 50 == 0) {
      page2 = 10;
      page++;
    }
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
        reinterpret_cast<System::Collections::IEnumerator*>(
            custom_types::Helpers::CoroutineHelper::New(Refresh())));
  }
}

VerticalLayoutGroup* CreateHost(Transform* parent, Vector2 anchoredPos,
                                Vector2 size) {
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

custom_types::Helpers::Coroutine
ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::Refresh() {
  getLogger().info("refresh test1");
  this->initialized = false;
  getLogger().info("refresh test2");
  co_yield reinterpret_cast<System::Collections::IEnumerator*>(
      custom_types::Helpers::CoroutineHelper::New(GetDocument(this)));
  getLogger().info("refresh test3");
  co_yield reinterpret_cast<System::Collections::IEnumerator*>(
      CRASH_UNLESS(WaitForSeconds::New_ctor(2.0f)));
  getLogger().info("refresh test4");
  tableView->ReloadData();
  getLogger().info("refresh test5");
  get_transform()->get_parent()->SetAsFirstSibling();
  getLogger().info("refresh test6");
  co_return;
}

HMUI::TableCell*
ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::CellForIdx(
    HMUI::TableView* tableView, int idx) {
  this->tableView = tableView;
  static auto PlayerTableCellList =
      il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(
          "CustomPlayerTableCellList");
  static auto PlayerTableCellStr =
      il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(
          "CustomPlayerTableCell");

  PlayerTableCell* playerCell = reinterpret_cast<PlayerTableCell*>(
      tableView->DequeueReusableCellForIdentifier(PlayerTableCellList));

  const rapidjson::Value& players = document["players"];

  auto player = players[idx - (10 * (page2 - 1))].GetObject();

  if (!playerCell) {
    playerCell =
        UnityEngine::GameObject::New_ctor()->AddComponent<PlayerTableCell*>();
    playerCell->set_reuseIdentifier(PlayerTableCellList);
    playerCell->set_name(PlayerTableCellStr);

    playerCell->get_gameObject()->AddComponent<HMUI::Touchable*>();
    playerCell->set_interactable(true);

    auto verticalLayoutGroup = QuestUI::BeatSaberUI::CreateVerticalLayoutGroup(
        playerCell->get_transform());

    auto layout = verticalLayoutGroup->get_gameObject()
                      ->GetComponent<UnityEngine::UI::LayoutElement*>();
    layout->set_preferredHeight(11.0f);
    layout->set_preferredWidth(200.0f);

    playerCell->get_transform()->SetParent(
        tableView->get_transform()->GetChild(0)->GetChild(0), false);

    std::string iconPath =
        "/sdcard/ModData/com.beatgames.beatsaber/"
        "Mods/ScoreSaberUI/Icons/";

    Transform* t = playerCell->get_transform();

    playerCell->profile = BeatSaberUI::CreateImage(
        CreateHost(t, {-45.0f, 0.0f}, {10.0f, 10.0f})->get_transform(),
        BeatSaberUI::FileToSprite(iconPath + "oculus.png"), {0.0f, 0.0f},
        {10.0f, 10.0f});

    WebUtils::GetAsync(std::string(player["profilePicture"].GetString()), 64,
                       [=](long httpCode, std::string data) {
                         std::vector<uint8_t> bytes(data.begin(), data.end());
                         MainThreadScheduler::Schedule([=]() {
                           Sprite* sprite = BeatSaberUI::VectorToSprite(bytes);
                           playerCell->profile->set_sprite(sprite);
                         });
                       });

    playerCell->name = BeatSaberUI::CreateText(
        CreateHost(t, {-11.0f, 2.8f}, {55.0f, 8.0f})->get_transform(),
        std::string(player["name"].GetString()), false, {0.0f, 0.0f});
    playerCell->name->set_overflowMode(TextOverflowModes::Ellipsis);
    playerCell->name->set_alignment(TextAlignmentOptions::Left);
    playerCell->name->set_fontSize(5.0f);

    playerCell->rank = BeatSaberUI::CreateText(
        CreateHost(t, {-18.0f, -2.0f}, {40.0f, 8.0f})->get_transform(),
        std::string("#") + std::to_string(player["rank"].GetInt()), false,
        {0.0f, 0.0f});
    playerCell->rank->set_alignment(TextAlignmentOptions::Left);

    playerCell->pp = BeatSaberUI::CreateText(
        CreateHost(t, {27.0f, 0.0f}, {20.0f, 11.0f})->get_transform(),
        StringUtils::Colorize(
            StringUtils::RemoveTrailingZeros(
                std::to_string((player["pp"].GetDouble())), 5) +
                std::string("pp"),
            "#6872e5"),
        false, {0.0f, 0.0f});
    playerCell->pp->set_fontSize(5.0f);
    playerCell->pp->set_overflowMode(TextOverflowModes::Ellipsis);

    playerCell->country = BeatSaberUI::CreateText(
        CreateHost(t, {31.0f, -2.0f}, {17.0f, 0.0f})->get_transform(),
        std::string(player["country"].GetString()), false, {0.0f, 0.0f});
    playerCell->country->set_alignment(TextAlignmentOptions::Left);
    playerCell->country->set_fontSize(3.5f);

    ::Array<Il2CppString*>* histories =
        StringUtils::StrToIl2cppStr(player["histories"].GetString())
            ->Split(',');

    int weeklyChange = (stof(StringUtils::Il2cppStrToStr(histories->get(41))) -
                        stof(StringUtils::Il2cppStrToStr(histories->get(48))));

    playerCell->weekly = BeatSaberUI::CreateText(
        CreateHost(t, {41.0f, -1.0f}, {15.0f, 0.0f})->get_transform(),
        StringUtils::ColorizePosNeg(std::to_string(weeklyChange)), false,
        {0.0f, 0.0f});
    playerCell->weekly->set_alignment(TextAlignmentOptions::Right);
    playerCell->weekly->set_fontSize(5.0f);
  }
  playerCell->Refresh(player);
  return playerCell;
}