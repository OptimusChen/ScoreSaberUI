#include "Utils/StringUtils.hpp"

#include <chrono>
#include <codecvt>

static std::string color_prefix = "<color=";
static std::string color_suffix = "</color>";
static std::string size_prefix = "<size=";
static std::string size_suffix = "</size>";

using namespace std;

namespace ScoreSaberUI::Utils
{
    namespace StringUtils
    {

        std::string to_utf8(std::u16string_view view)
        {
            return std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>{}
                .to_bytes(view.data());
        }

        std::string GetRoleColor(std::string role)
        {
            if (role.compare("Supporter") == 0)
            {
                return "#f76754";
            }
            if (role.compare("Ranking Team") == 0)
            {
                return "#1cbc9c";
            }
            return "#FFFFFF";
        }

        std::string FormatScore(std::string s)
        {
            for (int i = 0; i < 2; i++)
            {
                s.pop_back();
            }
            s = s.substr(2);
            s.insert(2, ".");
            s = s + "%";
            return Colorize(s, "#ffd42a");
        }

        std::string RemoveTrailingZeros(std::string s, int zeros)
        {
            for (int i = 0; i < zeros; i++)
            {
                s.pop_back();
            }
            return s;
        }

        std::string ColorizePosNeg(std::string s)
        {
            float f = stof(s);
            if (f > 0)
            {
                return Colorize(s, "green");
            }
            else if (f < 0)
            {
                return Colorize(s, "red");
            }
            else
            {
                return s;
            }
        }

        std::string FormatPP(std::string s,
                             rapidjson::GenericObject<true, rapidjson::Value> score)
        {
            std::string modifiers = std::string(score["modifiers"].GetString());
            double pp = score["pp"].GetDouble();
            for (int i = 0; i < 4; i++)
            {
                s.pop_back();
            }
            s = s + Resize("pp", 50);
            s = Colorize(" - (", "\"white\"") + s + Colorize(")", "\"white\"");
            if (pp > 0.0f)
            {
                s = Colorize(s, "#6872e5");
                if (modifiers.compare("") == 0)
                {
                    return s;
                }
                else
                {
                    return s + string(" - ") + Colorize("[" + modifiers + "]", "#464f55");
                }
            }
            else
            {
                if (modifiers.compare("") == 0)
                {
                    return "";
                }
                else
                {
                    return " - " + Colorize("[" + modifiers + "]", "#464f55");
                }
            }
        }

        std::string Colorize(std::string s, std::string color)
        {
            return color_prefix + color + string(">") + s + color_suffix;
        }

        std::string Resize(std::string s, int sizePercent)
        {
            return size_prefix + to_string(sizePercent) + string("%>") + s + size_suffix;
        }

        std::string Il2cppStrToStr(Il2CppString* s) { return to_utf8(csstrtostr(s)); }

        Il2CppString* StrToIl2cppStr(std::string s)
        {
            return il2cpp_utils::newcsstr(s);
        }

    } // namespace StringUtils
} // namespace ScoreSaberUI::Utils
