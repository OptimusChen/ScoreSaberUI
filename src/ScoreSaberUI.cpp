#include "ScoreSaberUI.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler*
    ScoreSaberUI::ScoreSaber::leaderboard = nullptr;
ScoreSaberUI::UI::FlowCoordinators::ScoreSaberFlowCoordinator*
    ScoreSaberUI::ScoreSaber::flowCoordinator = nullptr;
UnityEngine::Sprite* ScoreSaberUI::ScoreSaber::globalIcon = nullptr;
UnityEngine::Sprite* ScoreSaberUI::ScoreSaber::playerIcon = nullptr;
UnityEngine::Sprite* ScoreSaberUI::ScoreSaber::friendsIcon = nullptr;

void ScoreSaberUI::InstallHooks() {
  ScoreSaberUI::Hooks::PlatformLeaderboardViewController();
  ScoreSaberUI::Hooks::MainSystemInit();
  ScoreSaberUI::Hooks::LeaderboardTableView();
}
