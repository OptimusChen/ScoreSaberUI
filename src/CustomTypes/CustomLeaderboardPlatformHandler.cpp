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
#include "Utils/StringUtils.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/error/en.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::CustomTypes, CustomLeaderboardPlatformHandler);

using namespace GlobalNamespace;
using namespace ScoreSaberUI::Utils;

int getDiff(IDifficultyBeatmap* beatmap) {
  return beatmap->get_difficultyRank();
}

int getMaxScore(IDifficultyBeatmap* beatmap) {
  int notesCount = beatmap->get_beatmapData()->get_cuttableNotesCount();
  if (notesCount > 13) {
    return (8 * ((115 * (notesCount - 13)))) + 4 * (115 * 7) + 2 * (115 * 5) +
           115;
  } else {
    int diff = (beatmap->get_beatmapData()->get_cuttableNotesCount() - 14) * -1;
    if (diff == 15) {
      return 0;
    } else if (diff == 14) {
      return 115;
    } else if (diff == 13) {
      return 115 + (2 * 115);
    } else if (diff < 13 && diff > 8) {
      return 115 + diff * (115 * 2);
    } else if (diff < 8 && diff > 0) {
      return 115 + diff * (115 * 4);
    }
  }
  return 0;
}

custom_types::Helpers::Coroutine GetScoresInternal(
    ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler* self,
    IDifficultyBeatmap* beatmap, PlatformLeaderboardsModel::ScoresScope scope,
    PlatformLeaderboardsModel::GetScoresCompletionHandler* completionHandler) {
  UnityEngine::Networking::UnityWebRequest* webRequest =
      UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::createcsstr(
          "https://scoresaber.com/api/leaderboard/by-hash/"
          "" +
          to_utf8(csstrtostr(
              reinterpret_cast<IPreviewBeatmapLevel*>(beatmap->get_level())
                  ->get_levelID()
                  ->Replace(il2cpp_utils::newcsstr("custom_level_"),
                            il2cpp_utils::newcsstr("")))) +
          "/"
          "scores?difficulty=" +
          std::to_string(getDiff(beatmap)) +
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
      double pp = score["pp"].GetDouble();
      if (pp > 0.0f) {
        self->mapRanked = true;
        self->ranked->set_text(
            il2cpp_utils::newcsstr("<i><color=#ffde1c>Ranked Status: "
                                   "</color>Ranked (modifiers disabled)</i>"));
      } else {
        self->mapRanked = false;
        self->ranked->set_text(il2cpp_utils::newcsstr(
            "<i><color=#ffde1c>Ranked Status: "
            "</color>Unranked (modifiers disabled)</i>"));
      }
      auto leaderboardPlayerInfo = score["leaderboardPlayerInfo"].GetObject();
      int modifiedScore = score["modifiedScore"].GetInt();
      int modifiedScoreDouble = score["modifiedScore"].GetDouble();
      int rank = score["rank"].GetInt();
      std::string name = std::string(leaderboardPlayerInfo["name"].GetString());
      if (leaderboardPlayerInfo["role"].IsString()) {
        std::string role =
            std::string(leaderboardPlayerInfo["role"].GetString());
        scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
            modifiedScore, rank,
            il2cpp_utils::newcsstr(StringUtils::Resize(
                StringUtils::Colorize(name, StringUtils::GetRoleColor(role)) +
                    " - (" +
                    StringUtils::FormatScore(
                        std::to_string(score["modifiedScore"].GetDouble() /
                                       getMaxScore(beatmap))) +
                    ")" + StringUtils::FormatPP(std::to_string(pp), score),
                80)),
            il2cpp_utils::newcsstr("0"), modifiers));
      } else {
        scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
            modifiedScore, rank,
            il2cpp_utils::newcsstr(StringUtils::Resize(
                name + " - (" +
                    StringUtils::FormatScore(
                        std::to_string(score["modifiedScore"].GetDouble() /
                                       getMaxScore(beatmap))) +
                    ")" + StringUtils::FormatPP(std::to_string(pp), score),
                80)),
            il2cpp_utils::newcsstr("0"), modifiers));
      }
    }
    if (scores->size == 0) {
      scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
          000, 1, il2cpp_utils::newcsstr("No scores on this leaderboard!"),
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
ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler::UploadScore(
    LeaderboardScoreUploader::ScoreData* scoreData,
    PlatformLeaderboardsModel::UploadScoreCompletionHandler*
        completionHandler) {
  return nullptr;
}