#include "ScoreSaberUI.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

ScoreSaberUI::CustomTypes::CustomLeaderboardPlatformHandler*
    ScoreSaberUI::ScoreSaber::leaderboard = nullptr;
ScoreSaberUI::UI::FlowCoordinators::ScoreSaberFlowCoordinator*
    ScoreSaberUI::ScoreSaber::flowCoordinator = nullptr;
