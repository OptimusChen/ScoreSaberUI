#include "UI/MainMenuFlowCoordinator.hpp"
#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"
#include "questui/shared/BeatSaberUI.hpp"

DEFINE_TYPE(ScoreSaber::UI, MainMenuFlowCoordinator);

using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace HMUI;

namespace ScoreSaber::UI
{
    void MainMenuFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (firstActivation)
        {
            globalViewController = CreateViewController<GlobalViewController*>();
            faqViewController = CreateViewController<ScoreSaberUI::UI::ViewControllers::FAQViewController*>();
            teamAndContributorsViewController = CreateViewController<TeamAndContributorsViewController*>();

            SetTitle(il2cpp_utils::newcsstr("ScoreSaber"), ViewController::AnimationType::Out);
            set_showBackButton(true);
            ProvideInitialViewControllers(globalViewController, teamAndContributorsViewController, faqViewController, nullptr, nullptr);
        }
        // HACK: if we don't do this the viewcontroller remains active when returning to the main song menu (don't ask me why)
        faqViewController->get_gameObject()->SetActive(true);
    }

    void MainMenuFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController)
    {
        // HACK: if we don't do this the viewcontroller remains active when returning to the main song menu (don't ask me why)
        faqViewController->get_gameObject()->SetActive(false);
        this->parentFlowCoordinator->DismissFlowCoordinator(this, ViewController::AnimationDirection::Horizontal, nullptr, false);
    }
}