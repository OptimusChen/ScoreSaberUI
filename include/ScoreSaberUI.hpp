#pragma once

#include "CustomTypes/CustomLeaderboardPlatformHandler.hpp"
#include "main.hpp"

namespace ScoreSaberUI {
namespace Hooks {
void PlatformLeaderboardViewController();
void LeaderboardTableView();
void MainSystemInit();

}  // namespace Hooks
void InstallHooks();

class ScoreSaber {
 public:
  static ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler*
      leaderboard;
};

}  // namespace ScoreSaberUI