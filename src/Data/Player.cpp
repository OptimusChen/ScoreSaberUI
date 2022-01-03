#include "Data/Player.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
namespace ScoreSaber::Data
{
    Player::Player(rapidjson::Value& value)
    {
        id = value["id"].GetString();
        name = to_utf16(value["name"].GetString());
        profilePicture = value["profilePicture"].GetString();
        country = value["country"].GetString();
        pp = value["pp"].GetDouble();
        rank = value["rank"].GetInt();
        countryRank = value["countryRank"].GetInt();
        role = value["role"].GetString();
        auto badgesItr = value.FindMember("badges");
        if (!badgesItr->value.IsNull() && badgesItr->value.IsArray())
            for (auto& badge : badgesItr->value.GetArray())
                badges.emplace_back(badge);
        histories = value["histories"].GetString();
        auto scoreStatsItr = value.FindMember("scoreStats");
        if (!scoreStatsItr->value.IsNull())
        {
            scoreStats = std::make_optional(ScoreStats(scoreStatsItr->value));
        }
        else
        {
            scoreStats = std::move(std::nullopt);
        }
        permissions = value["permissions"].GetInt();
        banned = value["banned"].GetBool();
        inactive = value["inactive"].GetBool();
    }

    Player::Player(rapidjson::GenericValue<rapidjson::UTF16<char16_t>>& value)
    {
        id = to_utf8(value[u"id"].GetString());
        name = std::u16string(value[u"name"].GetString());
        profilePicture = to_utf8(value[u"profilePicture"].GetString());
        country = to_utf8(value[u"country"].GetString());
        pp = value[u"pp"].GetDouble();
        rank = value[u"rank"].GetInt();
        countryRank = value[u"countryRank"].GetInt();
        role = to_utf8(value[u"role"].GetString());
        auto badgesItr = value.FindMember(u"badges");
        if (!badgesItr->value.IsNull() && badgesItr->value.IsArray())
            for (auto& badge : badgesItr->value.GetArray())
                badges.emplace_back(badge);
        histories = to_utf8(value[u"histories"].GetString());
        auto scoreStatsItr = value.FindMember(u"scoreStats");
        if (!scoreStatsItr->value.IsNull())
        {
            scoreStats = std::make_optional(ScoreStats(scoreStatsItr->value));
        }
        else
        {
            scoreStats = std::move(std::nullopt);
        }
        permissions = value[u"permissions"].GetInt();
        banned = value[u"banned"].GetBool();
        inactive = value[u"inactive"].GetBool();
    }

    Player::Player(rapidjson::GenericObject<true, rapidjson::Value> value)
    {
        id = value["id"].GetString();
        name = to_utf16(value["name"].GetString());
        profilePicture = value["profilePicture"].GetString();
        country = value["country"].GetString();
        pp = value["pp"].GetDouble();
        rank = value["rank"].GetInt();
        countryRank = value["countryRank"].GetInt();
        role = value["role"].GetString();
        auto badgesItr = value.FindMember("badges");
        if (!badgesItr->value.IsNull() && badgesItr->value.IsArray())
            for (auto& badge : badgesItr->value.GetArray())
                badges.emplace_back(badge.GetObject());
        histories = value["histories"].GetString();
        auto scoreStatsItr = value.FindMember("scoreStats");
        if (!scoreStatsItr->value.IsNull())
        {
            scoreStats = std::make_optional(ScoreStats(scoreStatsItr->value.GetObject()));
        }
        else
        {
            scoreStats = std::move(std::nullopt);
        }
        permissions = value["permissions"].GetInt();
        banned = value["banned"].GetBool();
        inactive = value["inactive"].GetBool();
    }

    Player::Player(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value)
    {
        id = to_utf8(value[u"id"].GetString());
        name = std::u16string(value[u"name"].GetString());
        profilePicture = to_utf8(value[u"profilePicture"].GetString());
        country = to_utf8(value[u"country"].GetString());
        pp = value[u"pp"].GetDouble();
        rank = value[u"rank"].GetInt();
        countryRank = value[u"countryRank"].GetInt();
        role = to_utf8(value[u"role"].GetString());
        auto badgesItr = value.FindMember(u"badges");
        if (!badgesItr->value.IsNull() && badgesItr->value.IsArray())
            for (auto& badge : badgesItr->value.GetArray())
                badges.emplace_back(badge.GetObject());
        histories = to_utf8(value[u"histories"].GetString());
        auto scoreStatsItr = value.FindMember(u"scoreStats");
        if (!scoreStatsItr->value.IsNull())
        {
            scoreStats = std::make_optional(ScoreStats(scoreStatsItr->value.GetObject()));
        }
        else
        {
            scoreStats = std::move(std::nullopt);
        }
        permissions = value[u"permissions"].GetInt();
        banned = value[u"banned"].GetBool();
        inactive = value[u"inactive"].GetBool();
    }

}