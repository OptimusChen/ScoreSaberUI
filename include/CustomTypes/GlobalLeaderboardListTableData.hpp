#pragma once

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Sprite.hpp"
#include "custom-types/shared/macros.hpp"

#include "HMUI/SelectableCell.hpp"
#include "HMUI/TableCell.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/TableView_IDataSource.hpp"

#include "CustomTypes/LeaderboardTableCell.hpp"

#include "GlobalNamespace/AnnotatedBeatmapLevelCollectionCell.hpp"
#include "GlobalNamespace/LevelListTableCell.hpp"
#include "GlobalNamespace/SimpleTextTableCell.hpp"

#include <vector>

// this is what that used to be called, but this is more clear and understandable (Blame BSML) we just copy this
namespace GlobalNamespace
{
    using LevelPackCell = AnnotatedBeatmapLevelCollectionCell;

}

___DECLARE_TYPE_WRAPPER_INHERITANCE(
    ScoreSaber::UI, GlobalLeaderboardListTableData, Il2CppTypeEnum::IL2CPP_TYPE_CLASS, UnityEngine::MonoBehaviour, "ScoreSaber", {classof(HMUI::TableView::IDataSource*)}, 0, nullptr,

    DECLARE_INSTANCE_FIELD(Il2CppString*, reuseIdentifier);
    DECLARE_INSTANCE_FIELD(float, cellSize);
    DECLARE_INSTANCE_FIELD(HMUI::TableView*, tableView);
    DECLARE_INSTANCE_FIELD(bool, expandCell);
    DECLARE_INSTANCE_FIELD(LeaderboardTableCell*, leaderboardTableCellInstance);

    DECLARE_CTOR(ctor);
    DECLARE_DTOR(dtor);

    DECLARE_OVERRIDE_METHOD(HMUI::TableCell*, CellForIdx, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::CellForIdx>::get(), HMUI::TableView* tableView, int idx);
    DECLARE_OVERRIDE_METHOD(float, CellSize, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::CellSize>::get());
    DECLARE_OVERRIDE_METHOD(int, NumberOfCells, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::TableView::IDataSource::NumberOfCells>::get());

    // use push_back, or even better emplace_back to add to the data list
    public:

)