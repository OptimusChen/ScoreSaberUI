#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include <string>

namespace ScoreSaber::Data
{
    struct ScoreStats
    {
        ScoreStats(rapidjson::Value& value);
        ScoreStats(rapidjson::GenericValue<rapidjson::UTF16<char16_t>>& value);
        ScoreStats(rapidjson::GenericObject<true, rapidjson::Value> value);
        ScoreStats(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value);

        int totalScore;
        int totalRankedScore;
        double averageRankedAccuracy;
        int totalPlayCount;
        int rankedPlayCount;
        int replaysWatched;
    };
}