#include "UI/Other/ScoreInfoModal.hpp"
#include "Sprites.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"

#include "Utils/UIUtils.hpp"

DEFINE_TYPE(ScoreSaber::UI::Other, ScoreInfoModal);

using namespace UnityEngine;
using namespace QuestUI;
using namespace QuestUI::BeatSaberUI;
namespace ScoreSaber::UI::Other
{
    void ScoreInfoModal::Hide()
    {
        modal->Hide(true, nullptr);
    }

    void ScoreInfoModal::Show(ScoreSaber::Data::Score& score)
    {

        modal->Show(true, true, nullptr);

        set_player(score.leaderboardPlayerInfo.name);
        // TODO: get device from the score.hmd integer;
        set_device("Placeholder");
        // TODO: Get the percentage for this:
        set_score(score.baseScore, 0.0f);
        set_pp(score.pp);
        set_combo(score.maxCombo);
        set_fullCombo(score.fullCombo);
        set_badCuts(score.badCuts);
        set_missedNotes(score.missedNotes);
        set_modifiers(score.modifiers);
        // TODO: Modify timeset to be like "N hours and M minutes ago" instead of timestamp
        set_timeSet(score.timeSet);

        playerId = score.leaderboardPlayerInfo.id;
    }

    ScoreInfoModal* ScoreInfoModal::Create(UnityEngine::Transform* parent)
    {
        auto modal = CreateModal(parent, Vector2(60, 80), nullptr);
        auto ppmodal = modal->get_gameObject()->AddComponent<ScoreInfoModal*>();
        ppmodal->modal = modal;
        ppmodal->Setup();
        return ppmodal;
    }

    void ScoreInfoModal::Setup()
    {
        auto mainVertical = CreateVerticalLayoutGroup(get_transform());
        auto headerHorizontal = CreateHorizontalLayoutGroup(mainVertical->get_transform());
        player = CreateText(headerHorizontal->get_transform(), "", false);
        auto userSprite = Base64ToSprite(user_base64);
        UIUtils::CreateClickableImage(headerHorizontal->get_transform(), userSprite, {0, 0}, {0, 0}, std::bind(&ScoreInfoModal::ShowPlayerProfileModal, this));
        auto seperatorHorizontal = CreateHorizontalLayoutGroup(mainVertical->get_transform());
        auto texture = Texture2D::get_whiteTexture();
        auto whiteSprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        auto seperatorImage = CreateImage(seperatorHorizontal->get_transform(), whiteSprite, {0, 0}, {0, 0});

        auto textVertical = CreateVerticalLayoutGroup(mainVertical->get_transform());
        device = CreateText(textVertical->get_transform(), "", false);
        score = CreateText(textVertical->get_transform(), "", false);
        pp = CreateText(textVertical->get_transform(), "", false);
        combo = CreateText(textVertical->get_transform(), "", false);
        fullCombo = CreateText(textVertical->get_transform(), "", false);
        badCuts = CreateText(textVertical->get_transform(), "", false);
        missedNotes = CreateText(textVertical->get_transform(), "", false);
        modifiers = CreateText(textVertical->get_transform(), "", false);
        timeSet = CreateText(textVertical->get_transform(), "", false);

        set_player(u"placeholder");
        set_device("Oculus Quest");
        set_score(0, 0);
        set_pp(0);
        set_combo(0);
        set_fullCombo(0);
        set_badCuts(0);
        set_missedNotes(0);
        set_modifiers("");
        set_timeSet("");
    }

    void ScoreInfoModal::set_player(std::u16string player)
    {
        this->player->set_text(il2cpp_utils::newcsstr(player + u"'s Score"));
    }

    void ScoreInfoModal::set_device(std::string_view device)
    {
        this->device->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Device:</color> %s", device.data())));
    }

    void ScoreInfoModal::set_score(long score, double percent)
    {
        this->score->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Score:</color> %ld (<color=#ffd42a>%.1f</color>)", score, percent)));
    }

    void ScoreInfoModal::set_pp(double pp)
    {
        this->pp->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Performance Points:</color> <color=#6872e5>%.0fpp</color>", pp)));
    }

    void ScoreInfoModal::set_combo(int combo)
    {
        this->combo->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Combo:</color> %d", combo)));
    }

    void ScoreInfoModal::set_fullCombo(bool value)
    {
        // TODO: Set correct color values
        this->fullCombo->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Full Combo:</color> %s", value ? "<color=\"green\">yes</color>" : "<color=\"red\">no</color>")));
    }

    void ScoreInfoModal::set_badCuts(int badCuts)
    {
        this->badCuts->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Bad Cuts:</color> %d", badCuts)));
    }

    void ScoreInfoModal::set_missedNotes(int missedNotes)
    {
        this->missedNotes->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Missed Notes:</color> %d", missedNotes)));
    }

    void ScoreInfoModal::set_modifiers(std::string_view modifiers)
    {
        this->modifiers->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Modifiers:</color> %s", modifiers.data())));
    }

    void ScoreInfoModal::set_timeSet(std::string_view timeSet)
    {
        this->timeSet->set_text(il2cpp_utils::newcsstr(string_format("<color=#6F6F6F>Time Set:</color> %s", timeSet.data())));
    }

    void ScoreInfoModal::ShowPlayerProfileModal()
    {
        if (playerProfileModal)
            playerProfileModal->Show(playerId);
    }
}