#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

#include <string>

namespace ScoreSaber::Data
{
    struct Difficulty
    {
        Difficulty(const rapidjson::Value&& value);
        Difficulty(const rapidjson::GenericValue<rapidjson::UTF16<char16_t>>&& value);
        Difficulty(rapidjson::GenericObject<true, rapidjson::Value> value);
        Difficulty(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value);

        int leaderboardId;
        int difficulty;
        std::string gameMode;
        std::string difficultyRaw;
    };
}