#pragma once

#include "CustomTypes/ClickableImage.hpp"
#include "CustomTypes/ClickableText.hpp"
#include "HMUI/TableCell.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(
    ScoreSaber::UI, LeaderboardTableCell, HMUI::TableCell,
    DECLARE_INSTANCE_FIELD(ScoreSaber::ClickableImage*, pfp);
    DECLARE_INSTANCE_FIELD(ScoreSaber::ClickableText*, playerName);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, ranking);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, pp);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, flag);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, country);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, history);

    public
    :

    struct Data
    {
        std::string pfpURL;
        std::string playerName;
        std::string ranking;
        std::string pp;
        std::string country;
        std::string history;
    };
    bool inited = false;
    Data data;
    static LeaderboardTableCell * CreateTemplate();

    void SetupCell();
    void SetData(Data data);
    custom_types::Helpers::Coroutine LoadImage(HMUI::ImageView* out, std::string_view url);
    void OpenPlayerInfoModal();

)
