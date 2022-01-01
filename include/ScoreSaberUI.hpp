#pragma once

#include "CustomTypes/CustomLeaderboardPlatformHandler.hpp"
#include "UI/FlowCoordinators/ScoreSaberFlowCoordinator.hpp"
#include "UnityEngine/Sprite.hpp"
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
  static ScoreSaberUI::UI::FlowCoordinators::ScoreSaberFlowCoordinator*
      flowCoordinator;
  static UnityEngine::Sprite* globalIcon;
  static UnityEngine::Sprite* playerIcon;
  static UnityEngine::Sprite* friendsIcon;
};

}  // namespace ScoreSaberUI