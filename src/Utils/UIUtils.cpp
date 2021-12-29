#include "Utils/UIUtils.hpp"

#include "HMUI/CurvedCanvasSettingsHelper.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "HMUI/ImageView.hpp"
#include "HMUI/Touchable.hpp"
#include "Polyglot/LocalizedTextMeshProUGUI.hpp"
#include "UnityEngine/AdditionalCanvasShaderChannels.hpp"
#include "UnityEngine/Canvas.hpp"
#include "UnityEngine/CanvasGroup.hpp"
#include "UnityEngine/CanvasRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/RenderMode.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Vector3.hpp"
#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include "questui/shared/QuestUI.hpp"

using namespace HMUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
using namespace TMPro;

namespace ScoreSaberUI::Utils {
HorizontalLayoutGroup* UIUtils::CreateHeader(UnityEngine::Transform* parent,
                                             UnityEngine::Color color) {
  VerticalLayoutGroup* vertical = CreateVerticalLayoutGroup(parent);
  vertical->get_rectTransform()->set_anchoredPosition({0.0f, 45.0f});
  HorizontalLayoutGroup* horizontal =
      CreateHorizontalLayoutGroup(vertical->get_transform());

  Backgroundable* background =
      horizontal->get_gameObject()->AddComponent<Backgroundable*>();
  background->ApplyBackgroundWithAlpha(il2cpp_utils::newcsstr("panel-top"),
                                       1.0f);

  ImageView* imageView =
      background->get_gameObject()->GetComponentInChildren<ImageView*>();
  imageView->gradient = true;
  imageView->gradientDirection = 0;
  imageView->set_color(Color::get_white());
  imageView->set_color0(color);
  imageView->set_color1(color);
  imageView->curvedCanvasSettingsHelper->Reset();

  return horizontal;
}
}  // namespace ScoreSaberUI::Utils