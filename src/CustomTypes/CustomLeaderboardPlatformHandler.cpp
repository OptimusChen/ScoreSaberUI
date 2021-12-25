#include "CustomTypes/CustomLeaderboardPlatformHandler.hpp"

#include <chrono>
#include <codecvt>

#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/DifficultyBeatmapSet.hpp"
#include "GlobalNamespace/GameplayModifierParamsSO.hpp"
#include "GlobalNamespace/GameplayModifiers.hpp"
#include "GlobalNamespace/GameplayModifiersModelSO.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IGameEnergyCounter.hpp"
#include "GlobalNamespace/IPlatformUserModel.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/LeaderboardTableView_ScoreData.hpp"
#include "GlobalNamespace/LoadingControl.hpp"
#include "GlobalNamespace/MainMenuViewController.hpp"
#include "GlobalNamespace/OculusPlatformLeaderboardsHandler.hpp"
#include "GlobalNamespace/OculusPlatformLeaderboardsHandler_--c__DisplayClass5_0.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "GlobalNamespace/PlatformLeaderboardsHandler.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_GetScoresCompletionHandler.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_GetScoresResult.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_LeaderboardScore.hpp"
#include "GlobalNamespace/PlatformLeaderboardsModel_ScoresScope.hpp"
#include "GlobalNamespace/QuestAppInit.hpp"
#include "GlobalNamespace/RelativeScoreAndImmediateRankCounter.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/ScoreController.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "System/Collections/Generic/List_1.hpp"
#include "UnityEngine/JsonUtility.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "Utils/BeatmapUtils.hpp"
#include "Utils/StringUtils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/error/en.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::CustomTypes, CustomLeaderboardPlatformHandler);

using namespace GlobalNamespace;
using namespace ScoreSaberUI::Utils;
using namespace ScoreSaberUI::Utils::StringUtils;
using namespace ScoreSaberUI::Utils::BeatmapUtils;

custom_types::Helpers::Coroutine GetScoresInternal(
    ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler* self,
    IDifficultyBeatmap* beatmap, PlatformLeaderboardsModel::ScoresScope scope,
    PlatformLeaderboardsModel::GetScoresCompletionHandler* completionHandler) {
  Il2CppString* csHash =
      reinterpret_cast<IPreviewBeatmapLevel*>(beatmap->get_level())
          ->get_levelID();
  csHash->Replace(il2cpp_utils::newcsstr("custom_level_"),
                  Il2CppString::_get_Empty());
  std::string hash = to_utf8(csstrtostr(csHash));

  UnityEngine::Networking::UnityWebRequest* webRequest =
      UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::newcsstr(
          "https://scoresaber.com/api/leaderboard/by-hash/" + hash + "/" +
          "scores?difficulty=" + std::to_string(getDiff(beatmap)) +
          "&page=" + std::to_string(self->page) + "&gameMode=SoloStandard" +
          "&withMetadata=true"));

  co_yield reinterpret_cast<System::Collections::IEnumerator*>(
      CRASH_UNLESS(webRequest->SendWebRequest()));

  auto scores = System::Collections::Generic::List_1<
      PlatformLeaderboardsModel::LeaderboardScore*>::New_ctor();
  auto modifiers = System::Collections::Generic::List_1<
      GameplayModifierParamsSO*>::New_ctor();

  if (!webRequest->get_isNetworkError()) {
    rapidjson::Document doc;
    std::string s = StringUtils::to_utf8(
        csstrtostr(webRequest->get_downloadHandler()->get_text()));
    doc.Parse(s);

    const rapidjson::Value& scoreArray = doc["scores"];

    for (int i = 0; i < scoreArray.Size(); i++) {
      auto score = scoreArray[i].GetObject();

      auto leaderboardPlayerInfo = score["leaderboardPlayerInfo"].GetObject();

      int modifiedScore = score["modifiedScore"].GetInt();
      int modifiedScoreDouble = score["modifiedScore"].GetDouble();
      int rank = score["rank"].GetInt();
      double pp = score["pp"].GetDouble();

      bool ranked = pp > 0.0f;

      std::string name = std::string(leaderboardPlayerInfo["name"].GetString());
      std::string role = "";

      if (leaderboardPlayerInfo["role"].IsString()) {
        role = std::string(leaderboardPlayerInfo["role"].GetString());
      }

      std::string rankedStatus = ranked ? "Ranked" : "Unranked";

      self->ranked->set_text(il2cpp_utils::newcsstr(
          "<i>" + StringUtils::Colorize("Ranked Status: ", "#ffde1c") +
          +rankedStatus + " (modifiers disabled)</i>"));

      self->mapRanked = ranked;

      double scoreDouble = score["modifiedScore"].GetDouble();

      scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
          modifiedScore, rank,
          il2cpp_utils::newcsstr(Resize(
              Colorize(name, GetRoleColor(role)) + " - (" +
                  FormatScore(std::to_string(
                      scoreDouble / BeatmapUtils::getMaxScore(beatmap))) +
                  ")" + FormatPP(std::to_string(pp), score),
              80)),
          il2cpp_utils::newcsstr("0"), modifiers));
    }
    if (scores->size == 0) {
      scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
          0, 0, il2cpp_utils::newcsstr("No scores on this leaderboard!"),
          il2cpp_utils::newcsstr("0"), modifiers));
    }
    completionHandler->Invoke(PlatformLeaderboardsModel::GetScoresResult::Ok,
                              scores->ToArray(), -1);
  } else {
    completionHandler->Invoke(
        PlatformLeaderboardsModel::GetScoresResult::Failed, scores->ToArray(),
        -1);
  }
  webRequest->Dispose();
  co_return;
}

void ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler::changePage(
    bool inc) {
  if (inc) {
    page++;
  } else {
    if (page != 1) {
      page--;
    }
  }
}

HMAsyncRequest*
ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler::GetScores(
    IDifficultyBeatmap* beatmap, int count, int fromRank,
    PlatformLeaderboardsModel::ScoresScope scope,
    ::Il2CppString* referencePlayerId,
    PlatformLeaderboardsModel::GetScoresCompletionHandler* completionHandler) {
  GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
      reinterpret_cast<custom_types::Helpers::enumeratorT*>(
          custom_types::Helpers::CoroutineHelper::New(
              GetScoresInternal(this, beatmap, scope, completionHandler))));

  return nullptr;
}

HMAsyncRequest*
ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler::mloadScore(
    LeaderboardScoreUploader::ScoreData* scoreData,
    PlatformLeaderboardsModel::UploadScoreCompletionHandler*
        completionHandler) {
  return nullptr;
}