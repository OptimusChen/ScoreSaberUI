#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

namespace ScoreSaber::Data
{
    struct Metadata
    {
        Metadata(rapidjson::Value& value);
        Metadata(rapidjson::GenericValue<rapidjson::UTF16<char16_t>>& value);
        Metadata(rapidjson::GenericObject<true, rapidjson::Value> value);
        Metadata(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value);

        int total;
        int page;
        int itemsPerPage;
    };
}