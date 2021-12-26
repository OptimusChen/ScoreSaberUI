#pragma once

#include "GlobalNamespace/BeatmapData.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/types.hpp"

namespace ScoreSaberUI {
namespace Utils {
namespace BeatmapUtils {

int getDiff(GlobalNamespace::IDifficultyBeatmap* beatmap);
int getMaxScore(GlobalNamespace::IDifficultyBeatmap* beatmap);

}  // namespace BeatmapUtils
}  // namespace Utils
}  // namespace ScoreSaberUI