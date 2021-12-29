#include "UI/FlowCoordinators/ScoreSaberFlowCoordinator.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/QuestUI.hpp"

using namespace QuestUI;
using namespace ScoreSaberUI::UI::FlowCoordinators;

DEFINE_TYPE(ScoreSaberUI::UI::FlowCoordinators, ScoreSaberFlowCoordinator);

void ScoreSaberFlowCoordinator::DidActivate(bool firstActivation,
                                            bool addedToHierarchy,
                                            bool screenSystemEnabling) {
  if (firstActivation) {
    this->SetTitle(il2cpp_utils::newcsstr<il2cpp_utils::CreationType::Manual>(
                       "SCORESABER"),
                   HMUI::ViewController::AnimationDirection::Horizontal);
    this->showBackButton = true;
    if (!this->faqView) {
      this->faqView = QuestUI::BeatSaberUI::CreateViewController<
          ScoreSaberUI::UI::ViewControllers::FAQViewController*>();
    }
    if (!this->globalView) {
      this->globalView = QuestUI::BeatSaberUI::CreateViewController<
          ScoreSaberUI::UI::ViewControllers::GlobalViewController*>();
    }
    if (!this->teamView) {
      this->teamView = QuestUI::BeatSaberUI::CreateViewController<
          ScoreSaberUI::UI::ViewControllers::TeamViewController*>();
    }

    this->ProvideInitialViewControllers(globalView, teamView, faqView, nullptr,
                                        nullptr);
  }
}

void ScoreSaberFlowCoordinator::BackButtonWasPressed(
    HMUI::ViewController* topView) {
  faqView->get_gameObject()->SetActive(false);
  SetRightScreenViewController(nullptr,
                               HMUI::ViewController::AnimationType::Out);
  this->parentFlowCoordinator->DismissFlowCoordinator(
      this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr,
      false);
}