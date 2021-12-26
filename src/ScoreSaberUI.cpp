#include "ScoreSaberUI.hpp"

ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler*
    ScoreSaberUI::ScoreSaber::leaderboard = nullptr;

void ScoreSaberUI::InstallHooks() {
  ScoreSaberUI::Hooks::PlatformLeaderboardViewController();
  ScoreSaberUI::Hooks::MainSystemInit();
  ScoreSaberUI::Hooks::LeaderboardTableView();
}
