#pragma once
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"

#include <string>

namespace ScoreSaber::Data
{
    struct LeaderboardPlayer
    {
        LeaderboardPlayer(const rapidjson::Value&& value);
        LeaderboardPlayer(const rapidjson::GenericValue<rapidjson::UTF16<char16_t>>&& value);
        LeaderboardPlayer(rapidjson::GenericObject<true, rapidjson::Value> value);
        LeaderboardPlayer(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> value);

        std::string id;
        std::u16string name;
        std::string profilePicture;
        std::string country;
        int permissions;
        std::string role;
    };
}