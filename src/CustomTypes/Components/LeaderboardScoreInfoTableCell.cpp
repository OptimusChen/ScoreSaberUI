#include "CustomTypes/Components/LeaderboardScoreInfoTableCell.hpp"

DEFINE_TYPE(ScoreSaber::CustomTypes::Components, LeaderboardScoreInfoTableCell);

namespace ScoreSaber::CustomTypes::Components
{
    LeaderboardScoreInfoTableCell* LeaderboardScoreInfoTableCell::CreateCell()
    {
        // TODO: implement
        return nullptr;
    }

    void LeaderboardScoreInfoTableCell::Refresh(ScoreSaber::Data::Score& score)
    {
        this->score = score;
    }

    void LeaderboardScoreInfoTableCell::OpenScoreInfoModal()
    {
        // TODO: implement
        scoreInfoModal->Show(score);
    }
}