#include "CustomLeaderboardPlatformHandler.hpp"

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
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/error/en.h"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(FakeSaber, CustomLeaderboardPlatformHandler);

using namespace GlobalNamespace;

std::string to_utf8(std::u16string_view view) {
  return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}
      .to_bytes(view.data());
}

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
    if (diff == 14) {
      return 0;
    } else if (diff == 13) {
      return 115;
    } else if (diff < 13 && diff > 8) {
      return 115 + diff * (115 * 2);
    } else if (diff < 8 && diff > 0) {
      return 115 + diff * (115 * 4);
    }
  }
}

std::string format(std::string s, std::string color, int i,
                   rapidjson::GenericObject<true, rapidjson::Value> score) {
  if (i == 0) {
    for (int i = 0; i < 2; i++) {
      s.pop_back();
    }
    s = s.substr(2);
    s.insert(2, ".");
    s = s + "%";
    return string_format("%s", (std::string("<color=") + color +
                                std::string(">") + s + std::string("</color>"))
                                   .c_str());
  } else {
    for (int i = 0; i < 4; i++) {
      s.pop_back();
    }
    s = s + "<size=50%>pp</size>";
    s = "<color=\"white\"> - (</color>" + s + "<color=\"white\">)</color>";
    if (score["pp"].GetDouble() > 0.0f) {
      return string_format(
          "%s", (std::string("<color=") + color + std::string(">") + s +
                 std::string("</color>"))
                    .c_str());

    } else {
      return "";
    }
  }
}

custom_types::Helpers::Coroutine GetScoresInternal(
    FakeSaber::CustomLeaderboardPlatformHandler* self,
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
  if (!webRequest->get_isNetworkError()) {
    il2cpp_utils::getLogger().info("[ssl] test1");
    rapidjson::Document doc;
    std::string s =
        to_utf8(csstrtostr(webRequest->get_downloadHandler()->get_text()));
    il2cpp_utils::getLogger().info("[ssl] test2");
    rapidjson::ParseResult result = doc.Parse(s);
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    il2cpp_utils::getLogger().info("[ssl] test3");
    std::string errorCodeStr(rapidjson::GetParseError_En(result.Code()));
    il2cpp_utils::getLogger().info("[ssl] test4");
    const rapidjson::Value& scoreArray = doc["scores"];
    for (int i = 0; i < scoreArray.Size(); i++) {
      auto score = scoreArray[i].GetObject();
      if (score["pp"].GetDouble() > 0.0f) {
        self->mapRanked = true;
      } else {
        self->mapRanked = false;
      }
      il2cpp_utils::getLogger().info("[ssl] test5");
      if (score["leaderboardPlayerInfo"]["role"].IsString()) {
        if (std::string(score["leaderboardPlayerInfo"]["role"].GetString())
                .compare("Supporter") == 0) {
          scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
              score["baseScore"].GetInt(), score["rank"].GetInt(),
              il2cpp_utils::newcsstr(
                  "<size=80%><color=#f76754>" +
                  std::string(
                      score["leaderboardPlayerInfo"]["name"].GetString()) +
                  "</color> - (" +
                  format(std::to_string(score["baseScore"].GetDouble() /
                                        getMaxScore(beatmap)),
                         "#ffd42a", 0, score) +
                  ")" +
                  format(std::to_string(score["pp"].GetDouble()), "#6872e5", 1,
                         score) +
                  "</size>"),
              il2cpp_utils::newcsstr("0"),
              System::Collections::Generic::List_1<
                  GameplayModifierParamsSO*>::New_ctor()));
        } else {
          scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
              score["baseScore"].GetInt(), score["rank"].GetInt(),
              il2cpp_utils::newcsstr(
                  "<size=80%>" +
                  std::string(
                      score["leaderboardPlayerInfo"]["name"].GetString()) +
                  " - (" +
                  format(std::to_string(score["baseScore"].GetDouble() /
                                        getMaxScore(beatmap)),
                         "#ffd42a", 0, score) +
                  ")" +
                  format(std::to_string(score["pp"].GetDouble()), "#6872e5", 1,
                         score) +
                  "</size>"),
              il2cpp_utils::newcsstr("0"),
              System::Collections::Generic::List_1<
                  GameplayModifierParamsSO*>::New_ctor()));
        }
      } else {
        scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
            score["baseScore"].GetInt(), score["rank"].GetInt(),
            il2cpp_utils::newcsstr(
                "<size=80%>" +
                std::string(
                    score["leaderboardPlayerInfo"]["name"].GetString()) +
                " - (" +
                format(std::to_string(score["baseScore"].GetDouble() /
                                      getMaxScore(beatmap)),
                       "#ffd42a", 0, score) +
                ")" +
                format(std::to_string(score["pp"].GetDouble()), "#6872e5", 1,
                       score) +
                "</size>"),
            il2cpp_utils::newcsstr("0"),
            System::Collections::Generic::List_1<
                GameplayModifierParamsSO*>::New_ctor()));
      }

      il2cpp_utils::getLogger().info("[ssl] test6");
    }
    il2cpp_utils::getLogger().info("[ssl] test7");
    if (scores->size == 0) {
      scores->Add(PlatformLeaderboardsModel::LeaderboardScore::New_ctor(
          000, 1, il2cpp_utils::newcsstr("No scores on this leaderboard!"),
          il2cpp_utils::newcsstr("0"),
          System::Collections::Generic::List_1<
              GameplayModifierParamsSO*>::New_ctor()));
    }
    completionHandler->Invoke(PlatformLeaderboardsModel::GetScoresResult::Ok,
                              scores->ToArray(), -1);
    if (self->ranked) {
      if (self->mapRanked) {
        self->ranked->set_text(
            il2cpp_utils::newcsstr("<color=#ffde1c>Ranked Status: "
                                   "</color>Ranked (modifiers disabled)"));
      } else {
        self->ranked->set_text(
            il2cpp_utils::newcsstr("<color=#ffde1c>Ranked Status: "
                                   "</color>Unranked (modifiers disabled)"));
      }
    }
    il2cpp_utils::getLogger().info("[ssl] test8");
  } else {
    completionHandler->Invoke(
        PlatformLeaderboardsModel::GetScoresResult::Failed, scores->ToArray(),
        -1);
  }
  webRequest->Dispose();
  co_return;
}

void FakeSaber::CustomLeaderboardPlatformHandler::changePage(bool inc) {
  if (inc) {
    page++;
  } else {
    if (page != 1) {
      page--;
    }
  }
}

HMAsyncRequest* FakeSaber::CustomLeaderboardPlatformHandler::GetScores(
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

HMAsyncRequest* FakeSaber::CustomLeaderboardPlatformHandler::UploadScore(
    LeaderboardScoreUploader::ScoreData* scoreData,
    PlatformLeaderboardsModel::UploadScoreCompletionHandler*
        completionHandler) {
  // ApiController::get_instance()->UploadScore(scoreData, completionHandler);
  return nullptr;
}