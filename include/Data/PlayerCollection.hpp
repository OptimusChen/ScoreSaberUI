#pragma once

#include "Data/Metadata.hpp"
#include "Data/Player.hpp"
struct Il2CppString;

namespace ScoreSaber::Data
{
    struct PlayerCollection
    {
        PlayerCollection();
        PlayerCollection(Il2CppString* string);
        PlayerCollection(std::string_view string);
        PlayerCollection(std::u16string_view string);
        PlayerCollection(const rapidjson::Document&& doc);
        PlayerCollection(const rapidjson::GenericDocument<rapidjson::UTF16<char16_t>>&& doc);
        PlayerCollection(rapidjson::GenericObject<true, rapidjson::Value> doc);
        PlayerCollection(rapidjson::GenericObject<true, rapidjson::GenericValue<rapidjson::UTF16<char16_t>>> doc);

        std::vector<Player> players;
        std::optional<Metadata> metadata;
    };
}