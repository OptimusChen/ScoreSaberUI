#include "UI/ViewControllers/GlobalViewController.hpp"

#include "Utils/UIUtils.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::ViewControllers, GlobalViewController);

using namespace ScoreSaberUI::UI::ViewControllers;

void GlobalViewController::DidActivate(bool firstActivation,
                                       bool addedToHierarchy,
                                       bool screenSystemEnabling) {
  if (firstActivation) {
    Utils::UIUtils::CreateHeader(get_transform(), "Global Leaderboards",
                                 Color(0.3f, 0.3f, 0.3f, 0.5f));
  }
}