#include "CustomTypes/DelayedCreateImage.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "UnityEngine/Rect.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

DEFINE_TYPE(ScoreSaber::UI, DelayedCreateImage);

using namespace UnityEngine;
using namespace UnityEngine::UI;

namespace ScoreSaber::UI
{
    HMUI::ImageView* DelayedCreateImage::CreateImage(UnityEngine::Transform* parent, std::string_view url, std::function<void(HMUI::ImageView*)> finished)
    {
        auto imageView = QuestUI::BeatSaberUI::CreateImage(parent, nullptr, {0, 0}, {0, 0});
        imageView->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(
            custom_types::Helpers::CoroutineHelper::New(
                WaitForImage(imageView, url, finished))));
        return imageView;
    }

    custom_types::Helpers::Coroutine DelayedCreateImage::WaitForImage(HMUI::ImageView* imageView, std::string_view url, std::function<void(HMUI::ImageView*)> finished)
    {
        UnityEngine::Networking::UnityWebRequest* www = UnityEngine::Networking::UnityWebRequestTexture::GetTexture(il2cpp_utils::newcsstr(url));
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(www->SendWebRequest());
        auto downloadHandlerTexture = reinterpret_cast<UnityEngine::Networking::DownloadHandlerTexture*>(www->get_downloadHandler());
        auto texture = downloadHandlerTexture->get_texture();
        auto sprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        imageView->set_sprite(sprite);
        if (finished)
            finished(imageView);
        co_return;
    }

}