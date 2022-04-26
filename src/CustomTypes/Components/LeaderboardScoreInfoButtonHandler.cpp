#include "CustomTypes/Components/LeaderboardScoreInfoButtonHandler.hpp"
#include "Utils/UIUtils.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"

#include "Sprites.hpp"
#include "logging.hpp"

DEFINE_TYPE(ScoreSaber::CustomTypes::Components, LeaderboardScoreInfoButtonHandler);

using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;

namespace ScoreSaber::CustomTypes::Components
{
    void LeaderboardScoreInfoButtonHandler::Setup()
    {
        // buttons = Array<ScoreSaber::CustomTypes::Components::ClickableImage*>::NewLength(10);
        buttons = Array<UnityEngine::UI::Button*>::NewLength(10);
        auto infoSprite = Base64ToSprite(info_base64);
        Vector2 basePosition = {45.0f, 26.17f};
        for (int i = 0; i < 10; i++)
        {
            // auto btn = UIUtils::CreateClickableImage(get_transform(), infoSprite, basePosition, {3.5, 3.5}, std::bind(&LeaderboardScoreInfoButtonHandler::ShowScoreInfoModal, this, i));
            // btn->set_preserveAspect(true);
            auto btn = CreateUIButton(get_transform(), "", "SettingsButton", basePosition, {3.5, 3.5}, std::bind(&LeaderboardScoreInfoButtonHandler::ShowScoreInfoModal, this, i));
            SetButtonSprites(btn, infoSprite, infoSprite);
            auto btnRect = reinterpret_cast<UnityEngine::RectTransform*>(btn->get_transform()->GetChild(0));
            btnRect->set_sizeDelta({3.5, 3.5f});
            btnRect->set_anchoredPosition({0.0f, 0.0f});
            basePosition.y -= 6.0f;
            buttons->values[i] = btn;
        }

        scoreInfoModal = ScoreSaber::UI::Other::ScoreInfoModal::Create(get_transform());
    }

    void LeaderboardScoreInfoButtonHandler::set_buttonCount(int count)
    {
        for (int i = 0; i < 10; i++)
        {
            bool active = i < count;
            INFO("Setting button %d %s", i, active ? "Active" : "Inactive");
            buttons->values[i]->get_gameObject()->SetActive(active);
        }
    }

    void LeaderboardScoreInfoButtonHandler::set_scoreCollection(ScoreSaber::Data::ScoreCollection& scoreCollection)
    {
        this->scoreCollection = scoreCollection;
        set_buttonCount(scoreCollection.size());
    }

    void LeaderboardScoreInfoButtonHandler::ShowScoreInfoModal(int buttonIdx)
    {
        if (scoreInfoModal && buttonIdx < scoreCollection.size())
        {
            scoreInfoModal->Show(scoreCollection.at(buttonIdx));
        }
    }
}