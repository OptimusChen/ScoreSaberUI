#include "CustomTypes/Components/PlayerTableCell.hpp"

#include "CustomTypes/Components/ImageButton.hpp"
#include "HMUI/ImageView.hpp"
#include "UnityEngine/Sprite.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/WebUtils.hpp"
#include "main.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/MainThreadScheduler.hpp"
#include "questui/shared/QuestUI.hpp"

DEFINE_TYPE(ScoreSaberUI::CustomTypes::Components, PlayerTableCell);

using namespace ScoreSaberUI::CustomTypes::Components;
using namespace ScoreSaberUI::Utils::StringUtils;
using namespace ScoreSaberUI::Utils;
using namespace UnityEngine;
using namespace QuestUI;

void PlayerTableCell::ctor() {}

void PlayerTableCell::Refresh(
    rapidjson::GenericObject<true, rapidjson::Value> player) {
  getLogger().info("test1");

  std::string iconPath =
      "/sdcard/ModData/com.beatgames.beatsaber/"
      "Mods/ScoreSaberUI/Icons/";

  profile->set_sprite(BeatSaberUI::FileToSprite(iconPath + "oculus.png"));
  WebUtils::GetAsync(std::string(player["profilePicture"].GetString()), 64,
                     [=](long httpCode, std::string data) {
                       std::vector<uint8_t> bytes(data.begin(), data.end());
                       MainThreadScheduler::Schedule([=]() {
                         getLogger().info("test8");
                         Sprite* profilePicture =
                             BeatSaberUI::VectorToSprite(bytes);
                         profile->set_sprite(profilePicture);
                         getLogger().info("test9");
                       });
                     });
  getLogger().info("test2");

  name->set_text(StrToIl2cppStr(std::string(player["name"].GetString())));
  rank->set_text(StrToIl2cppStr(std::string("#") +
                                std::to_string(player["rank"].GetInt())));
  getLogger().info("test3");
  pp->set_text(StrToIl2cppStr(
      StringUtils::Colorize(StringUtils::RemoveTrailingZeros(
                                std::to_string(player["pp"].GetDouble()), 5) +
                                std::string("pp"),
                            "#6872e5")));
  getLogger().info("test4");
  country->set_text(StrToIl2cppStr(std::string(player["country"].GetString())));

  ::Array<Il2CppString*>* histories =
      StringUtils::StrToIl2cppStr(player["histories"].GetString())->Split(',');
  getLogger().info("test5");

  int weeklyChange = stof(StringUtils::Il2cppStrToStr(histories->get(41))) -
                     stof(StringUtils::Il2cppStrToStr(histories->get(48)));
  getLogger().info("test6");

  weekly->set_text(StrToIl2cppStr(std::to_string(weeklyChange)));
  getLogger().info("test7");
}