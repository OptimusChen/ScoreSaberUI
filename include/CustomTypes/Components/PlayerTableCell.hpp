#pragma once

#include "CustomTypes/Components/ImageButton.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/TableCell.hpp"
#include "HMUI/TableView.hpp"
#include "HMUI/TableView_IDataSource.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "custom-types/shared/macros.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/CustomTypes/Components/List/CustomCellListWrapper.hpp"
#include "questui/shared/CustomTypes/Components/List/QuestUITableView.hpp"
#include "questui/shared/CustomTypes/Components/SegmentedControl/CustomTextSegmentedControlData.hpp"

#define GET_FIND_METHOD(mPtr) \
  il2cpp_utils::il2cpp_type_check::MetadataGetter<mPtr>::get()

DECLARE_CLASS_CODEGEN(
    ScoreSaberUI::CustomTypes::Components, PlayerTableCell, HMUI::TableCell,
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, rank);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, pp);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, country);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, weekly);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, name);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, flag);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, profile);
    DECLARE_INSTANCE_FIELD(QuestUI::Backgroundable*, bg);

    DECLARE_CTOR(ctor);

    public
    : void Refresh(rapidjson::GenericObject<true, rapidjson::Value> players);)