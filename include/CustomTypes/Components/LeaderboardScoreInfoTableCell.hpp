#pragma once

#include "UI/Other/ScoreInfoModal.hpp"

#include "Data/Score.hpp"

#include "HMUI/TableCell.hpp"
#include "custom-types/shared/macros.hpp"

#define GET_FIND_METHOD(mPtr) \
    il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::get()

DECLARE_CLASS_CODEGEN(ScoreSaber::CustomTypes::Components, LeaderboardScoreInfoTableCell, HMUI::TableCell,
                      DECLARE_INSTANCE_FIELD(ScoreSaber::UI::Other::ScoreInfoModal*, scoreInfoModal);

                      static LeaderboardScoreInfoTableCell * CreateCell();
                      void Refresh(ScoreSaber::Data::Score& score);
                      void OpenScoreInfoModal();

                      private
                      :

                      ScoreSaber::Data::Score score;

)