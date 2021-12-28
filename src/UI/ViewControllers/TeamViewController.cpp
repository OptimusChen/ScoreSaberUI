#include "UI/ViewControllers/TeamViewController.hpp"

#include "Utils/UIUtils.hpp"

DEFINE_TYPE(ScoreSaberUI::UI::ViewControllers, TeamViewController);

using namespace ScoreSaberUI::UI::ViewControllers;

void TeamViewController::DidActivate(bool firstActivation,
                                     bool addedToHierarchy,
                                     bool screenSystemEnabling) {
  if (firstActivation) {
    Utils::UIUtils::CreateHeader(get_transform(), "Team and Contributors",
                                 Color(0.3f, 0.3f, 0.3f, 0.5f));
  }
}