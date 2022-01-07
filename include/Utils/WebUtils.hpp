#pragma once
#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "HMUI/ImageView.hpp"

#include <functional>
#include <string>

namespace WebUtils
{

    std::optional<rapidjson::Document> GetJSON(std::string url);

    long Get(std::string url, std::string& val);

    long Get(std::string url, long timeout, std::string& val);

    void GetAsync(std::string url, std::function<void(long, std::string)> finished,
                  std::function<void(float)> progressUpdate = nullptr);

    void GetAsync(std::string url, long timeout,
                  std::function<void(long, std::string)> finished,
                  std::function<void(float)> progressUpdate = nullptr);

    void GetJSONAsync(
        std::string url,
        std::function<void(long, bool, rapidjson::Document&)> finished);

    /// @brief gets texture @ url and applies it to out->set_sprite() after downloading
    custom_types::Helpers::Coroutine WaitForImageDownload(std::string url, HMUI::ImageView* out);
    /// @brief gets gif @ url and applies it's first frame to out->set_sprite() after downloading
    custom_types::Helpers::Coroutine WaitForGifDownload(std::string url, HMUI::ImageView* out);

} // namespace WebUtils
