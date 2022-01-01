#pragma once

#include "CustomTypes/ClickableText.hpp"
#include "UI/Other/PlayerProfileModal.hpp"

#include "HMUI/ImageView.hpp"
#include "HMUI/ModalView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "custom-types/shared/macros.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"
#include <string_view>

DECLARE_CLASS_CODEGEN(
    ScoreSaber::UI, Banner, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_FIELD(QuestUI::Backgroundable*, bg);
    DECLARE_INSTANCE_FIELD(HMUI::ImageView*, bgImage);
    DECLARE_INSTANCE_FIELD(ScoreSaber::ClickableText*, topText);
    DECLARE_INSTANCE_FIELD(ScoreSaber::ClickableText*, bottomText);
    DECLARE_INSTANCE_FIELD(ScoreSaber::UI::PlayerProfileModal*, playerProfileModal);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, loadingVertical);

    public
    : static ScoreSaber::UI::Banner * Create(UnityEngine::Transform * parent);
    void Setup();
    void OpenMainMenuFlowCoordinator();
    void OpenPlayerInfoModal();
    void OpenSongInBrowser();

    void set_rainbow(bool value);
    void set_color(UnityEngine::Color color);

    void set_ranking(int rank, float pp);
    void set_status(std::string_view status);

    void set_loading(bool value);
    private
    : bool rainbow = false;
    bool wasRainbow = false;
    float colorAngle = 0.0f;
    static constexpr const UnityEngine::Color defaultColor = {0, 0.47, 0.72, 1.0};
    void set_topText(std::u16string_view newText);
    void set_topText(std::string_view newText) { set_topText(to_utf16(newText)); };
    void set_bottomText(std::u16string_view newText);
    void set_bottomText(std::string_view newText) { set_bottomText(to_utf16(newText)); };)