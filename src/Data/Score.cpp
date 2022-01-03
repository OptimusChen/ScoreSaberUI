#include "Data/Score.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

namespace ScoreSaber::Data
{
    Score::Score(const rapidjson::Value&& value) : leaderboardPlayerInfo(value["leaderboardPlayerInfo"].GetObject())
    {
        id = value["id"].GetInt();
        rank = value["rank"].GetInt();
        baseScore = value["baseScore"].GetInt();
        modifiedScore = value["modifiedScore"].GetInt();
        pp = value["pp"].GetDouble();
        weight = value["weight"].GetDouble();
        modifiers = value["modifiers"].GetString();
        multiplier = value["multiplier"].GetDouble();
        badCuts = value["badCuts"].GetInt();
        missedNotes = value["missedNotes"].GetInt();
        maxCombo = value["maxCombo"].GetInt();
        fullCombo = value["fullCombo"].GetBool();
        hmd = value["hmd"].GetInt();
        hasReplay = value["hasReplay"].GetBool();
        timeSet = value["timeSet"].GetString();
    }

    Score::Score(const rapidjson::GenericValue<rapidjson::UTF16<char16_t>>&& value) : leaderboardPlayerInfo(value[u"leaderboardPlayerInfo"].GetObject())
    {
        id = value[u"id"].GetInt();
        rank = value[u"rank"].GetInt();
        baseScore = value[u"baseScore"].GetInt();
        modifiedScore = value[u"modifiedScore"].GetInt();
        pp = value[u"pp"].GetDouble();
        weight = value[u"weight"].GetDouble();
        modifiers = to_utf8(value[u"modifiers"].GetString());
        multiplier = value[u"multiplier"].GetDouble();
        badCuts = value[u"badCuts"].GetInt();
        missedNotes = value[u"missedNotes"].GetInt();
        maxCombo = value[u"maxCombo"].GetInt();
        fullCombo = value[u"fullCombo"].GetBool();
        hmd = value[u"hmd"].GetInt();
        hasReplay = value[u"hasReplay"].GetBool();
        timeSet = to_utf8(value[u"timeSet"].GetString());
    }

    Score::Score(rapidjson::GenericObject<true, rapidjson::Value> value) : leaderboardPlayerInfo(value["leaderboardPlayerInfo"].GetObject())
    {
        id = value["id"].GetInt();
        rank = value["rank"].GetInt();
        baseScore = value["baseScore"].GetInt();
        modifiedScore = value["modifiedScore"].GetInt();
        pp = value["pp"].GetDouble();
        weight = value["weight"].GetDouble();
        modifiers = value["modifiers"].GetString();
        multiplier = value["multiplier"].GetDouble();
        badCuts = value["badCuts"].GetInt();
        missedNotes = value["missedNotes"].GetInt();
        maxCombo = value["maxCombo"].GetInt();
        fullCombo = value["fullCombo"].GetBool();
        hmd = value["hmd"].GetInt();
        hasReplay = value["hasReplay"].GetBool();
        timeSet = value["timeSet"].GetString();
    }

    Score::Score(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value) : leaderboardPlayerInfo(value[u"leaderboardPlayerInfo"].GetObject())
    {
        id = value[u"id"].GetInt();
        rank = value[u"rank"].GetInt();
        baseScore = value[u"baseScore"].GetInt();
        modifiedScore = value[u"modifiedScore"].GetInt();
        pp = value[u"pp"].GetDouble();
        weight = value[u"weight"].GetDouble();
        modifiers = to_utf8(value[u"modifiers"].GetString());
        multiplier = value[u"multiplier"].GetDouble();
        badCuts = value[u"badCuts"].GetInt();
        missedNotes = value[u"missedNotes"].GetInt();
        maxCombo = value[u"maxCombo"].GetInt();
        fullCombo = value[u"fullCombo"].GetBool();
        hmd = value[u"hmd"].GetInt();
        hasReplay = value[u"hasReplay"].GetBool();
        timeSet = to_utf8(value[u"timeSet"].GetString());
    }

}