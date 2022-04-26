#pragma once

// #include "CustomTypes/Components/ClickableImage.hpp"
#include "UI/Other/ScoreInfoModal.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "custom-types/shared/macros.hpp"

#include "Data/ScoreCollection.hpp"

DECLARE_CLASS_CODEGEN(ScoreSaber::CustomTypes::Components, LeaderboardScoreInfoButtonHandler, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_FIELD(Array<UnityEngine::UI::Button*>*, buttons);
                      // DECLARE_INSTANCE_FIELD(Array<ScoreSaber::CustomTypes::Components::ClickableImage*>*, buttons);

                      DECLARE_INSTANCE_FIELD(ScoreSaber::UI::Other::ScoreInfoModal*, scoreInfoModal);
                      public
                      :

                      void Setup();
                      void set_buttonCount(int count);
                      void set_scoreCollection(ScoreSaber::Data::ScoreCollection& scoreCollection);
                      private
                      :

                      ScoreSaber::Data::ScoreCollection scoreCollection;
                      void ShowScoreInfoModal(int buttonIdx);

)