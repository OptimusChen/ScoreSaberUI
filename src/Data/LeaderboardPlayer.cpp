#include "Data/LeaderboardPlayer.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

namespace ScoreSaber::Data
{
    LeaderboardPlayer::LeaderboardPlayer(const rapidjson::Value&& value)
    {
        id = value["id"].GetString();
        name = to_utf16(value["id"].GetString());
        role = value["role"].GetString();
        profilePicture = value["profilePicture"].GetString();
        country = value["country"].GetString();
        permissions = value["permissions"].GetInt();
        role = value["role"].GetString();
    }

    LeaderboardPlayer::LeaderboardPlayer(const rapidjson::GenericValue<rapidjson::UTF16<char16_t>>&& value)
    {
        id = to_utf8(value[u"id"].GetString());
        name = value[u"id"].GetString();
        role = to_utf8(value[u"role"].GetString());
        profilePicture = to_utf8(value[u"profilePicture"].GetString());
        country = to_utf8(value[u"country"].GetString());
        permissions = value[u"permissions"].GetInt();
        role = to_utf8(value[u"role"].GetString());
    }

    LeaderboardPlayer::LeaderboardPlayer(rapidjson::GenericObject<true, rapidjson::Value> value)
    {
        id = value["id"].GetString();
        name = to_utf16(value["id"].GetString());
        role = value["role"].GetString();
        profilePicture = value["profilePicture"].GetString();
        country = value["country"].GetString();
        permissions = value["permissions"].GetInt();
        role = value["role"].GetString();
    }

    LeaderboardPlayer::LeaderboardPlayer(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value)
    {
        id = to_utf8(value[u"id"].GetString());
        name = value[u"id"].GetString();
        role = to_utf8(value[u"role"].GetString());
        profilePicture = to_utf8(value[u"profilePicture"].GetString());
        country = to_utf8(value[u"country"].GetString());
        permissions = value[u"permissions"].GetInt();
        role = to_utf8(value[u"role"].GetString());
    }

}