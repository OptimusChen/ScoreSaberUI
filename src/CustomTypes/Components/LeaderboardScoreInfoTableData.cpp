#include "CustomTypes/Components/LeaderboardScoreInfoTableData.hpp"

DEFINE_TYPE(ScoreSaber::CustomTypes::Components, LeaderboardScoreInfoTableData);

namespace ScoreSaber::CustomTypes::Components
{
    void LeaderboardScoreInfoTableData::ctor() {}

    HMUI::TableCell* LeaderboardScoreInfoTableData::CellForIdx(HMUI::TableView* tableView, int idx)
    {
        // TODO: actually implement this
        return nullptr;
    }

    float LeaderboardScoreInfoTableData::CellSize()
    {
        // TODO: Actual good size
        return 2.5f;
    }

    int LeaderboardScoreInfoTableData::NumberOfCells()
    {
        // TODO: if not 10 scores, don't give 10 back (idk how yet?)
        return 10;
    }
}