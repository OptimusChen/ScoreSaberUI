#pragma once
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"

#include "Data/Score.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/types.hpp"

namespace ScoreSaberUI
{
    namespace Utils
    {
        namespace StringUtils
        {

            std::string to_utf8(std::u16string_view view);

            std::string GetRoleColor(std::string role);

            std::string FormatScore(std::string percent);

            std::string FormatPP(std::string pp,
                                 rapidjson::GenericObject<true, rapidjson::Value> score);
            std::u16string FormatPP(const ::ScoreSaber::Data::Score&& score);
            std::u16string FormatPP(const ::ScoreSaber::Data::Score& score);

            std::string Colorize(std::string s, std::string color);
            std::u16string Colorize(std::u16string s, std::string color);

            std::string Resize(std::string s, int sizePercent);
            std::u16string Resize(std::u16string s, int sizePercent);

            std::string RemoveTrailingZeros(std::string s, int zeros);

            std::string ColorizePosNeg(std::string s);

            std::string Il2cppStrToStr(Il2CppString* s);

            Il2CppString* StrToIl2cppStr(std::u16string_view s);
            Il2CppString* StrToIl2cppStr(std::string_view s);

        } // namespace StringUtils
    }     // namespace Utils
} // namespace ScoreSaberUI