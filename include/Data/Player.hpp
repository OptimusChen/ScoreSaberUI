#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

#include "Data/Badge.hpp"
#include "Data/ScoreStats.hpp"

#include <optional>
#include <string>
#include <vector>

namespace ScoreSaber::Data
{
    struct Player
    {
        Player(rapidjson::Value& value);
        Player(rapidjson::GenericValue<rapidjson::UTF16<char16_t>>& value);
        Player(rapidjson::GenericObject<true, rapidjson::Value> value);
        Player(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value);

        std::string id;
        std::u16string name;
        std::string profilePicture;
        std::string country;
        double pp;
        int rank;
        int countryRank;
        std::string role;
        std::vector<Badge> badges;
        std::string histories;
        std::optional<::ScoreSaber::Data::ScoreStats> scoreStats;
        int permissions;
        bool banned;
        bool inactive;
    };
}