#include "Utils/BeatmapUtils.hpp"

namespace ScoreSaberUI::Utils {
namespace BeatmapUtils {
int getDiff(IDifficultyBeatmap* beatmap) {
  return beatmap->get_difficultyRank();
}

int getMaxScore(IDifficultyBeatmap* beatmap) {
  int notesCount = beatmap->get_beatmapData()->get_cuttableNotesCount();
  if (notesCount > 13) {
    return (8 * ((115 * (notesCount - 13)))) + 4 * (115 * 7) + 2 * (115 * 5) +
           115;
  } else {
    int diff = (beatmap->get_beatmapData()->get_cuttableNotesCount() - 14) * -1;
    if (diff == 15) {
      return 0;
    } else if (diff == 14) {
      return 115;
    } else if (diff == 13) {
      return 115 + (2 * 115);
    } else if (diff < 13 && diff > 8) {
      return 115 + diff * (115 * 2);
    } else if (diff < 8 && diff > 0) {
      return 115 + diff * (115 * 4);
    }
  }
  return 0;
}
}  // namespace BeatmapUtils
}  // namespace ScoreSaberUI::Utils