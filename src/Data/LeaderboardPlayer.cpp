#include "Data/LeaderboardPlayer.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

namespace ScoreSaber::Data
{
    LeaderboardPlayer::LeaderboardPlayer(const rapidjson::Value&& value)
    {
        id = value["id"].GetString();
        name = to_utf16(value["name"].GetString());
        profilePicture = value["profilePicture"].GetString();
        country = value["country"].GetString();
        permissions = value["permissions"].GetInt();
        auto roleItr = value.FindMember("role");
        if (roleItr != value.MemberEnd() && roleItr->value.IsString())
            role = roleItr->value.GetString();
    }

    LeaderboardPlayer::LeaderboardPlayer(const rapidjson::GenericValue<rapidjson::UTF16<char16_t>>&& value)
    {
        id = to_utf8(value[u"id"].GetString());
        name = value[u"name"].GetString();
        profilePicture = to_utf8(value[u"profilePicture"].GetString());
        country = to_utf8(value[u"country"].GetString());
        permissions = value[u"permissions"].GetInt();
        auto roleItr = value.FindMember(u"role");
        if (roleItr != value.MemberEnd() && roleItr->value.IsString())
            role = to_utf8(roleItr->value.GetString());
    }

    LeaderboardPlayer::LeaderboardPlayer(rapidjson::GenericObject<true, rapidjson::Value> value)
    {
        id = value["id"].GetString();
        name = to_utf16(value["name"].GetString());
        profilePicture = value["profilePicture"].GetString();
        country = value["country"].GetString();
        permissions = value["permissions"].GetInt();
        auto roleItr = value.FindMember("role");
        if (roleItr != value.MemberEnd() && roleItr->value.IsString())
            role = roleItr->value.GetString();
    }

    LeaderboardPlayer::LeaderboardPlayer(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value)
    {
        id = to_utf8(value[u"id"].GetString());
        name = value[u"name"].GetString();
        profilePicture = to_utf8(value[u"profilePicture"].GetString());
        country = to_utf8(value[u"country"].GetString());
        permissions = value[u"permissions"].GetInt();
        auto roleItr = value.FindMember(u"role");
        if (roleItr != value.MemberEnd() && roleItr->value.IsString())
            role = to_utf8(roleItr->value.GetString());
    }

}