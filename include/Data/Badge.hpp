#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include <string>

namespace ScoreSaber::Data
{
    struct Badge
    {
        Badge(rapidjson::Value& value);
        Badge(rapidjson::GenericValue<rapidjson::UTF16<char16_t>>& value);
        Badge(rapidjson::GenericObject<true, rapidjson::Value> value);
        Badge(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value);

        std::string description;
        std::string image;
    };
}