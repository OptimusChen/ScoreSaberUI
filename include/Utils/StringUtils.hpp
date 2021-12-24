#pragma once

#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/types.hpp"


namespace ScoreSaberUI {
namespace Utils {
namespace StringUtils {
std::string to_utf8(std::u16string_view view);
std::string GetRoleColor(std::string role);
std::string FormatScore(std::string percent);
std::string FormatPP(std::string pp,
                     rapidjson::GenericObject<true, rapidjson::Value> score);
std::string Colorize(std::string s, std::string color);
std::string Resize(std::string s, int sizePercent);
}  // namespace StringUtils
}  // namespace Utils
}  // namespace ScoreSaberUI