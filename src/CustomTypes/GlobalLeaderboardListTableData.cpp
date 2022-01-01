#include "CustomTypes/GlobalLeaderboardListTableData.hpp"

#include "UnityEngine/Rect.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/Vector2.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "questui/shared/ArrayUtil.hpp"

#include "logging.hpp"

DEFINE_TYPE(ScoreSaber::UI, GlobalLeaderboardListTableData);

using namespace HMUI;
using namespace TMPro;
using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace ScoreSaber::UI
{
    void GlobalLeaderboardListTableData::ctor()
    {
        INVOKE_CTOR();
        INFO("ctor");
        expandCell = false;
        reuseIdentifier = il2cpp_utils::newcsstr("QuestUIListTableCell");
        cellSize = 12.0f;
        tableView = nullptr;
        leaderboardTableCellInstance = nullptr;
    }

    void GlobalLeaderboardListTableData::dtor()
    {
        Finalize();
    }

    HMUI::TableCell* GlobalLeaderboardListTableData::CellForIdx(HMUI::TableView* tableView, int idx)
    {
        INFO("Getting cell for idx %d", idx);
        auto tableCell = reinterpret_cast<LeaderboardTableCell*>(tableView->DequeueReusableCellForIdentifier(reuseIdentifier));
        if (!tableCell)
        {
            tableCell = LeaderboardTableCell::CreateTemplate();
        }
        static std::vector<LeaderboardTableCell::Data> data = {
            {"https://cdn.scoresaber.com/avatars/76561198333869741.jpg",
             "cerret",
             "#1",
             "16968.5pp",
             "US",
             "0"},
            {"https://cdn.scoresaber.com/avatars/76561198187936410.jpg",
             "Garsh",
             "#2",
             "16874.5pp",
             "US",
             "0"},
            {"https://cdn.scoresaber.com/avatars/76561199104169308.jpg",
             "udon 🥚",
             "#3",
             "16874.5pp",
             "US",
             "0"},
            {"https://cdn.scoresaber.com/avatars/76561198362923485.jpg",
             "Tseska",
             "#4",
             "16477pp",
             "FI",
             "0"},
            {"https://cdn.scoresaber.com/avatars/oculus.png",
             "Smallfox",
             "#5",
             "16477.5pp",
             "SE",
             "0"}};

        tableCell->set_reuseIdentifier(reuseIdentifier);
        tableCell->SetData(data[idx]);
        tableCell->set_interactable(false);
        return tableCell;
    }

    float GlobalLeaderboardListTableData::CellSize()
    {
        return cellSize;
    }

    int GlobalLeaderboardListTableData::NumberOfCells()
    {
        return 5;
    }
}