#include "Utils/StringUtils.hpp"

#include <chrono>
#include <codecvt>

static std::string color_prefix = "<color=";
static std::string color_suffix = "</color>";
static std::string size_prefix = "<size=";
static std::string size_suffix = "</size>";

using namespace std;

namespace ScoreSaberUI::Utils {
namespace StringUtils {

std::string to_utf8(std::u16string_view view) {
  return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}
      .to_bytes(view.data());
}

std::string GetRoleColor(std::string role) {
  if (role.compare("Supporter") == 0) {
    return "#f76754";
  }
  return "#FFFFFF";
}

std::string FormatScore(std::string s) {
  for (int i = 0; i < 2; i++) {
    s.pop_back();
  }
  s = s.substr(2);
  s.insert(2, ".");
  s = s + "%";
  return Colorize(s, "#ffd42a");
}

std::string FormatPP(std::string s,
                     rapidjson::GenericObject<true, rapidjson::Value> score) {
  std::string modifiers = std::string(score["modifiers"].GetString());
  double pp = score["pp"].GetDouble();
  for (int i = 0; i < 4; i++) {
    s.pop_back();
  }
  s = s + Resize("pp", 50);
  s = Colorize(" - (", "\"white\"") + s + Colorize(")", "\"white\"");
  if (pp > 0.0f) {
    s = Colorize(s, "#6872e5");
    if (modifiers.compare("") == 0) {
      return s;
    } else {
      return s + string(" - ") + Colorize("[" + modifiers + "]", "#464f55");
    }

  } else {
    if (modifiers.compare("") == 0) {
      return "";
    } else {
      return " - " + Colorize("[" + modifiers + "]", "#464f55");
    }
  }
}

std::string Colorize(std::string s, std::string color) {
  return color_prefix + color + string(">") + s + color_suffix;
}

std::string Resize(std::string s, int sizePercent) {
  return size_prefix + to_string(sizePercent) + string("%>") + s + size_suffix;
}

}  // namespace StringUtils
}  // namespace ScoreSaberUI::Utils
