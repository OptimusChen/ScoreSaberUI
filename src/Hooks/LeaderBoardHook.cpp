/*
#include "CustomTypes/ScoreSaber.hpp"
#include "GlobalNamespace/PlatformLeaderboardViewController.hpp"
#include "hooks.hpp"
#include "questui/shared/BeatSaberUI.hpp"

MAKE_AUTO_HOOK_MATCH(PlatformLeaderboardViewController_DidActivate, &GlobalNamespace::PlatformLeaderboardViewController::DidActivate, void, GlobalNamespace::PlatformLeaderboardViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (firstActivation)
    {
        // idk do whatever the fuck you want
        auto banner = ScoreSaber::UI::Banner::Create(self->get_transform());
        banner->playerProfileModal = ScoreSaber::UI::PlayerProfileModal::Create(self->get_transform());
    }
}
*/