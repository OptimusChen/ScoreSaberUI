#include "CustomTypes/Components/CustomCellListTableData.hpp"
#include "CustomTypes/Components/PlayerTableCell.hpp"
#include "GlobalNamespace/SharedCoroutineStarter.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/Touchable.hpp"
#include "System/Action_1.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/WaitForSeconds.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/UIUtils.hpp"
#include "Utils/WebUtils.hpp"
#include "logging.hpp"
#include "main.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::CustomTypes::Components, CustomCellListTableData);

using namespace ScoreSaberUI::Utils;
using namespace ScoreSaberUI::CustomTypes::Components;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace TMPro;

#define BeginCoroutine(method)                                               \
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine( \
        reinterpret_cast<System::Collections::IEnumerator*>(                 \
            custom_types::Helpers::CoroutineHelper::New(method)));

float ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::
    CellSize()
{
    return 12.0f;
}
int ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::
    NumberOfCells()
{
    return 5;
}

rapidjson::Document document;

custom_types::Helpers::Coroutine GetDocument(
    ScoreSaberUI::CustomTypes::Components::CustomCellListTableData* self)
{
    UnityEngine::Networking::UnityWebRequest* webRequest =
        UnityEngine::Networking::UnityWebRequest::Get(StringUtils::StrToIl2cppStr(
            "https://scoresaber.com/api/players?page=" +
            std::to_string(self->page) + "&withMetadata=false"));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(
        CRASH_UNLESS(webRequest->SendWebRequest()));
    if (!webRequest->get_isNetworkError())
    {
        std::string s = StringUtils::to_utf8(
            csstrtostr(webRequest->get_downloadHandler()->get_text()));
        getLogger().info("Received player objects: %s", s.c_str());
        document.Parse(s);
        self->initialized = true;
    }
    co_return;
}

void ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::ctor()
{
    page = 1;
    page2 = 0;
    reuseIdentifier = il2cpp_utils::newcsstr("CustomPlayerCellList");
}

void ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::
    DownButtonWasPressed()
{
    if (isLoading)
        return;
    INFO("Before page: %d, subpage: %d", page, page2);
    page2++;
    if (page2 > 9)
    {
        // we went out of bounds for this page, get page changed!
        page++;
        page2 = 0;
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
            reinterpret_cast<System::Collections::IEnumerator*>(
                custom_types::Helpers::CoroutineHelper::New(Refresh(true))));
    }
    else
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
            reinterpret_cast<System::Collections::IEnumerator*>(
                custom_types::Helpers::CoroutineHelper::New(Refresh())));
    }
    INFO("After page: %d, subpage: %d", page, page2);
}

void ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::
    UpButtonWasPressed()
{
    if (isLoading)
        return;
    INFO("Before page: %d, subpage: %d", page, page2);
    // on up press:
    // page controls the SS page,
    // page2 controls locally the 5 people we see within the 50 we get on a page
    page2--;
    if (page2 >= 0)
    {
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
            reinterpret_cast<System::Collections::IEnumerator*>(
                custom_types::Helpers::CoroutineHelper::New(Refresh())));
        INFO("After page: %d, subpage: %d", page, page2);
        return;
    }

    page--;
    if (page < 1)
    {
        // we went over the top of the first page, this means we went too far, revert changes
        page = 1;
        page2 = 0;
    }
    else
    {
        // page could be lowered, so we start at the bottom of of this new page
        page2 = 9; // end of page of 50 = page2 * 5 = 45 + the 5 we can see now
        GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
            reinterpret_cast<System::Collections::IEnumerator*>(
                custom_types::Helpers::CoroutineHelper::New(Refresh(true))));
    }
    /*
    else
    {
        // we could lower page 2 without entering a new SS page, we can refresh normally
    }
    */
    INFO("After page: %d, subpage: %d", page, page2);
}

void ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::StartRefresh(bool redownload)
{
    if (isLoading)
        return;
    GlobalNamespace::SharedCoroutineStarter::get_instance()->StartCoroutine(
        reinterpret_cast<System::Collections::IEnumerator*>(
            custom_types::Helpers::CoroutineHelper::New(Refresh(redownload))));
}

/// really just here because of the way it reloads twice by doing ReloadData and then RefreshCells(true, true), now it's combined
void ReloadTableViewData(HMUI::TableView* self)
{
    if (!self->dyn__isInitialized())
    {
        self->LazyInit();
    }
    auto visibleCells = self->dyn__visibleCells();
    int length = visibleCells->get_Count();
    for (int i = 0; i < length; i++)
    {
        auto tableCell = visibleCells->get_Item(i);
        tableCell->get_gameObject()->SetActive(false);
        self->AddCellToReusableCells(tableCell);
    }
    self->visibleCells->Clear();
    if (self->dyn__dataSource() != nullptr)
    {
        self->dyn__numberOfCells() = self->dyn__dataSource()->NumberOfCells();
        self->dyn__cellSize() = self->dyn__dataSource()->CellSize();
    }
    else
    {
        self->dyn__numberOfCells() = 0;
        self->dyn__cellSize() = 1.0f;
    }

    self->dyn__scrollView()->dyn__fixedCellSize() = self->dyn__cellSize();
    self->RefreshContentSize();
    if (!self->get_gameObject()->get_activeInHierarchy())
    {
        self->dyn__refreshCellsOnEnable() = true;
    }
    else
    {
        self->RefreshCells(true, true);
    }

    if (self->dyn_didReloadDataEvent())
        self->dyn_didReloadDataEvent()->Invoke(self);
}

custom_types::Helpers::Coroutine
ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::Refresh(bool redownload)
{
    isLoading = true;
    if (redownload)
    {
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(
            custom_types::Helpers::CoroutineHelper::New(GetDocument(this)));
    }
    //co_yield reinterpret_cast<System::Collections::IEnumerator*>(
    //    CRASH_UNLESS(WaitForSeconds::New_ctor(2.0f)));
    ReloadTableViewData(tableView);
    //tableView->RefreshCells(true, true);
    isLoading = false;
    co_return;
}

HMUI::TableCell*
ScoreSaberUI::CustomTypes::Components::CustomCellListTableData::CellForIdx(
    HMUI::TableView* tableView, int idx)
{
    PlayerTableCell* playerCell = reinterpret_cast<PlayerTableCell*>(
        tableView->DequeueReusableCellForIdentifier(reuseIdentifier));

    if (!playerCell)
    {
        playerCell = PlayerTableCell::CreateCell();
        //playerCell->get_transform()->SetParent(tableView->get_transform()->GetChild(0)->GetChild(0), false);
    }

    playerCell->set_reuseIdentifier(reuseIdentifier);
    if (initialized)
    {
        const rapidjson::Value& players = document["players"];
        int playerIDX = (page2 * 5) + idx;
        INFO("Getting player %d", playerIDX);
        auto player = players.GetArray()[playerIDX].GetObject();
        playerCell->Refresh(player);
    }
    return playerCell;
}