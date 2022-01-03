#include "Data/ScoreStats.hpp"

namespace ScoreSaber::Data
{
    ScoreStats::ScoreStats(rapidjson::Value& value)
    {
        totalScore = value["totalScore"].GetInt();
        totalRankedScore = value["totalRankedScore"].GetInt();
        averageRankedAccuracy = value["averageRankedAccuracy"].GetDouble();
        totalPlayCount = value["totalPlayCount"].GetInt();
        rankedPlayCount = value["rankedPlayCount"].GetInt();
        replaysWatched = value["replaysWatched"].GetInt();
    }

    ScoreStats::ScoreStats(rapidjson::GenericValue<rapidjson::UTF16<char16_t>>& value)
    {
        totalScore = value[u"totalScore"].GetInt();
        totalRankedScore = value[u"totalRankedScore"].GetInt();
        averageRankedAccuracy = value[u"averageRankedAccuracy"].GetDouble();
        totalPlayCount = value[u"totalPlayCount"].GetInt();
        rankedPlayCount = value[u"rankedPlayCount"].GetInt();
        replaysWatched = value[u"replaysWatched"].GetInt();
    }

    ScoreStats::ScoreStats(rapidjson::GenericObject<true, rapidjson::Value> value)
    {
        totalScore = value["totalScore"].GetInt();
        totalRankedScore = value["totalRankedScore"].GetInt();
        averageRankedAccuracy = value["averageRankedAccuracy"].GetDouble();
        totalPlayCount = value["totalPlayCount"].GetInt();
        rankedPlayCount = value["rankedPlayCount"].GetInt();
        replaysWatched = value["replaysWatched"].GetInt();
    }

    ScoreStats::ScoreStats(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value)
    {
        totalScore = value[u"totalScore"].GetInt();
        totalRankedScore = value[u"totalRankedScore"].GetInt();
        averageRankedAccuracy = value[u"averageRankedAccuracy"].GetDouble();
        totalPlayCount = value[u"totalPlayCount"].GetInt();
        rankedPlayCount = value[u"rankedPlayCount"].GetInt();
        replaysWatched = value[u"replaysWatched"].GetInt();
    }

}