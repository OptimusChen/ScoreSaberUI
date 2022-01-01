#pragma once

#include "HMUI/ImageView.hpp"
#include "HMUI/ModalView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "custom-types/shared/macros.hpp"
#include <string_view>

DECLARE_CLASS_CODEGEN(ScoreSaber::UI, PlayerProfileModal, UnityEngine::MonoBehaviour,
                      DECLARE_INSTANCE_FIELD(HMUI::ModalView*, modal);
                      DECLARE_INSTANCE_FIELD(HMUI::ImageView*, pfpImage);
                      DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, badgeHorizontal);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, headerText);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, globalRanking);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, performancePoints);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, averageRankedAccuracy);
                      DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, totalScore);

                      public
                      :

                      static ScoreSaber::UI::PlayerProfileModal * Create(UnityEngine::Transform * parent);
                      void Show();
                      void Setup();
                      void set_player(std::string_view playername);
                      void set_header(std::string_view header);
                      void set_globalRanking(int globalRanking);
                      void set_performancePoints(float performancePoints);
                      void set_averageRankedAccuracy(float averageRankedAccuracy);
                      void set_totalScore(long totalScore);
                      void set_pfp(UnityEngine::Sprite* pfp);
                      void ClearBadges();
                      void AddBadge(UnityEngine::Sprite* badge);

)