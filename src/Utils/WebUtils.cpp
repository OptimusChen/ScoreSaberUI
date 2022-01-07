#include "Utils/WebUtils.hpp"

#include "libcurl/shared/curl.h"
#include "libcurl/shared/easy.h"
#include "main.hpp"

#include "UnityEngine/Networking/DownloadHandler.hpp"
#include "UnityEngine/Networking/DownloadHandlerTexture.hpp"
#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/UnityWebRequestTexture.hpp"
#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/SpriteMeshType.hpp"
#include "UnityEngine/Texture2D.hpp"

#include "gif-lib/shared/gif_lib.h"

#include "logging.hpp"
#include <sstream>

//#include "gif_read.h"

using namespace UnityEngine;

// from
// https://github.com/darknight1050/SongDownloader/blob/master/src/Utils/WebUtils.cpp

#define TIMEOUT 10
#define USER_AGENT                                    \
    (std::string("ScoreSaber/") + VERSION +           \
     " (+https://github.com/OptimusChen/ScoreSaber)") \
        .c_str()

#include "gif-lib/shared/gif_lib.h"
struct Gif
{
    Gif(std::string& text) : data(text), datastream(&this->data){};
    Gif(std::vector<uint8_t>& vec) : data(reinterpret_cast<std::vector<char>&>(vec)), datastream(&this->data){};
    Gif(std::vector<char>& vec) : data(vec), datastream(&this->data){};
    Gif(Array<char>* array) : data(array), datastream(&this->data){};
    Gif(Array<uint8_t>* array) : Gif(reinterpret_cast<Array<char>*>(array)){};
    ~Gif()
    {
        int error = 0;
        DGifCloseFile(gif, &error);
    }
    int Parse()
    {
        int error = 0;
        gif = DGifOpen(this, &Gif::read, &error);
        return error;
    }

    int Slurp()
    {
        return DGifSlurp(gif);
    }

    static int read(GifFileType* pGifHandle, GifByteType* dest, int toRead)
    {
        Gif& dataWrapper = *(Gif*)pGifHandle->UserData;
        return dataWrapper.datastream.readsome(reinterpret_cast<char*>(dest), toRead);
    }

    Texture2D* get_frame(int idx)
    {
        if (!gif || idx > get_length()) return nullptr;

        GifColorType* color;
        SavedImage* frame;
        ExtensionBlock* ext = 0;
        GifImageDesc* frameInfo;
        ColorMapObject* colorMap;
        int x, y, j, loc;

        frame = &(gif->SavedImages[idx]);

        frameInfo = &(frame->ImageDesc);

        if (frameInfo->ColorMap)
        {
            colorMap = frameInfo->ColorMap;
        }
        else
        {
            colorMap = gif->SColorMap;
        }

        for (j = 0; j < frame->ExtensionBlockCount; ++j)
        {
            if (frame->ExtensionBlocks[j].Function == GRAPHICS_EXT_FUNC_CODE)
            {
                ext = &(frame->ExtensionBlocks[j]);
                break;
            }
        }

        int width = get_width();
        int height = get_height();
        auto texture = Texture2D::New_ctor(width, height);
        // should be same size :)
        Array<Color32>* pixelData = texture->GetPixels32();
        uint8_t* px = reinterpret_cast<uint8_t*>(pixelData->values);

        for (y = frameInfo->Top + frameInfo->Height; y >= frameInfo->Top; --y)
        {
            auto line = px;
            for (x = frameInfo->Left; x < frameInfo->Left + frameInfo->Width; ++x)
            {
                loc = (y - frameInfo->Top) * frameInfo->Width + (x - frameInfo->Left);
                if (frame->RasterBits[loc] == ext->Bytes[3] && ext->Bytes[0])
                {
                    continue;
                }

                color = &colorMap->Colors[frame->RasterBits[loc]];
                int linepos = x * 4;

                line[linepos] = color->Red;
                line[linepos + 1] = color->Green;
                line[linepos + 2] = color->Blue;
                line[linepos + 3] = 0xff;
            }

            px += (frameInfo->Width * 4);
        }

        texture->SetAllPixels32(pixelData, 0);
        texture->Apply();
        return texture;
    }
    int get_width() { return gif ? gif->SWidth : 0; };
    int get_height() { return gif ? gif->SHeight : 0; };
    int get_length() { return gif ? gif->ImageCount : 0; };

public:
    GifFileType* gif = nullptr;

private:
    template <typename CharT, typename TraitsT = std::char_traits<CharT>>
    class vectorwrapbuf : public std::basic_streambuf<CharT, TraitsT>
    {
    public:
        vectorwrapbuf(std::string& text)
        {
            this->std::basic_streambuf<CharT, TraitsT>::setg(text.data(), text.data(), text.data() + text.size());
        }

        vectorwrapbuf(std::vector<CharT>& vec)
        {
            this->std::basic_streambuf<CharT, TraitsT>::setg(vec.data(), vec.data(), vec.data() + vec.size());
        }

        vectorwrapbuf(Array<CharT>*& arr)
        {
            this->std::basic_streambuf<CharT, TraitsT>::setg(arr->values, arr->values, arr->values + arr->Length());
        }
    };

    std::istream datastream;
    vectorwrapbuf<char> data;
};

namespace WebUtils
{

    // https://stackoverflow.com/a/55660581
    std::string query_encode(const std::string& s)
    {
        std::string ret;

#define IS_BETWEEN(ch, low, high) (ch >= low && ch <= high)
#define IS_ALPHA(ch) (IS_BETWEEN(ch, 'A', 'Z') || IS_BETWEEN(ch, 'a', 'z'))
#define IS_DIGIT(ch) IS_BETWEEN(ch, '0', '9')
#define IS_HEXDIG(ch) \
    (IS_DIGIT(ch) || IS_BETWEEN(ch, 'A', 'F') || IS_BETWEEN(ch, 'a', 'f'))

        for (size_t i = 0; i < s.size();)
        {
            char ch = s[i++];

            if (IS_ALPHA(ch) || IS_DIGIT(ch))
            {
                ret += ch;
            }
            else if ((ch == '%') && IS_HEXDIG(s[i + 0]) && IS_HEXDIG(s[i + 1]))
            {
                ret += s.substr(i - 1, 3);
                i += 2;
            }
            else
            {
                switch (ch)
                {
                    case '-':
                    case '.':
                    case '_':
                    case '~':
                    case '!':
                    case '$':
                    case '&':
                    case '\'':
                    case '(':
                    case ')':
                    case '*':
                    case '+':
                    case ',':
                    case ';':
                    case '=':
                    case ':':
                    case '@':
                    case '/':
                    case '?':
                    case '[':
                    case ']':
                        ret += ch;
                        break;

                    default:
                    {
                        static const char hex[] = "0123456789ABCDEF";
                        char pct[] = "%  ";
                        pct[1] = hex[(ch >> 4) & 0xF];
                        pct[2] = hex[ch & 0xF];
                        ret.append(pct, 3);
                        break;
                    }
                }
            }
        }

        return ret;
    }

    std::size_t CurlWrite_CallbackFunc_StdString(void* contents, std::size_t size,
                                                 std::size_t nmemb,
                                                 std::string* s)
    {
        std::size_t newLength = size * nmemb;
        try
        {
            s->append((char*)contents, newLength);
        }
        catch (std::bad_alloc& e)
        {
            // handle memory problem
            getLogger().critical("Failed to allocate string of size: %lu", newLength);
            return 0;
        }
        return newLength;
    }

    std::optional<rapidjson::Document> GetJSON(std::string url)
    {
        std::string data;
        Get(url, data);
        rapidjson::Document document;
        document.Parse(data);
        if (document.HasParseError() || !document.IsObject())
            return std::nullopt;
        return document;
    }

    long Get(std::string url, std::string& val) { return Get(url, TIMEOUT, val); }

    long Get(std::string url, long timeout, std::string& val)
    {
        // Init curl
        auto* curl = curl_easy_init();
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Accept: */*");
        // Set headers
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_URL, query_encode(url).c_str());

        // Don't wait forever, time out after TIMEOUT seconds.
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

        // Follow HTTP redirects if necessary.
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                         CurlWrite_CallbackFunc_StdString);

        long httpCode(0);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&val));
        curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        auto res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
        {
            getLogger().critical("curl_easy_perform() failed: %u: %s", res,
                                 curl_easy_strerror(res));
        }
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_cleanup(curl);

        return httpCode;
    }

    struct ProgressUpdateWrapper
    {
        std::function<void(float)> progressUpdate;
    };

    void GetAsync(std::string url, std::function<void(long, std::string)> finished,
                  std::function<void(float)> progressUpdate)
    {
        GetAsync(url, TIMEOUT, finished, progressUpdate);
    }

    void GetAsync(std::string url, long timeout,
                  std::function<void(long, std::string)> finished,
                  std::function<void(float)> progressUpdate)
    {
        std::thread t([url, timeout, progressUpdate, finished]
                      {
                          std::string val;
                          // Init curl
                          auto* curl = curl_easy_init();
                          struct curl_slist* headers = NULL;
                          headers = curl_slist_append(headers, "Accept: */*");
                          // Set headers
                          curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

                          curl_easy_setopt(curl, CURLOPT_URL, query_encode(url).c_str());

                          // Don't wait forever, time out after TIMEOUT seconds.
                          curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

                          // Follow HTTP redirects if necessary.
                          curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

                          curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
                          curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                                           CurlWrite_CallbackFunc_StdString);

                          ProgressUpdateWrapper* wrapper = new ProgressUpdateWrapper{progressUpdate};
                          if (progressUpdate)
                          {
                              // Internal CURL progressmeter must be disabled if we provide our own
                              // callback
                              curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
                              curl_easy_setopt(curl, CURLOPT_XFERINFODATA, wrapper);
                              // Install the callback function
                              curl_easy_setopt(
                                  curl, CURLOPT_XFERINFOFUNCTION,
                                  +[](void* clientp, curl_off_t dltotal, curl_off_t dlnow,
                                      curl_off_t ultotal, curl_off_t ulnow)
                                  {
                                      float percentage = (float)dlnow / (float)dltotal * 100.0f;
                                      if (isnan(percentage))
                                          percentage = 0.0f;
                                      reinterpret_cast<ProgressUpdateWrapper*>(clientp)->progressUpdate(
                                          percentage);
                                      return 0;
                                  });
                          }

                          long httpCode(0);
                          curl_easy_setopt(curl, CURLOPT_WRITEDATA, &val);
                          curl_easy_setopt(curl, CURLOPT_USERAGENT, USER_AGENT);
                          curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

                          curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

                          auto res = curl_easy_perform(curl);
                          /* Check for errors */
                          if (res != CURLE_OK)
                          {
                              getLogger().critical("curl_easy_perform() failed: %u: %s", res,
                                                   curl_easy_strerror(res));
                          }
                          curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
                          curl_easy_cleanup(curl);
                          delete wrapper;
                          finished(httpCode, val);
                      });
        t.detach();
    }

    void GetJSONAsync(
        std::string url,
        std::function<void(long, bool, rapidjson::Document&)> finished)
    {
        GetAsync(url, [finished](long httpCode, std::string data)
                 {
                     rapidjson::Document document;
                     document.Parse(data);
                     finished(httpCode, document.HasParseError() || !document.IsObject(),
                              document);
                 });
    }

    custom_types::Helpers::Coroutine WaitForImageDownload(std::string url, HMUI::ImageView* out)
    {
        UnityEngine::Networking::UnityWebRequest* www = UnityEngine::Networking::UnityWebRequestTexture::GetTexture(il2cpp_utils::newcsstr(url));
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(www->SendWebRequest());
        auto downloadHandlerTexture = reinterpret_cast<UnityEngine::Networking::DownloadHandlerTexture*>(www->get_downloadHandler());
        auto texture = downloadHandlerTexture->get_texture();
        auto sprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)texture->get_width(), (float)texture->get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
        out->set_sprite(sprite);
        co_return;
    }

    custom_types::Helpers::Coroutine WaitForGifDownload(std::string url, HMUI::ImageView* out)
    {
        UnityEngine::Networking::UnityWebRequest* www = UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::newcsstr(url));
        co_yield reinterpret_cast<System::Collections::IEnumerator*>(www->SendWebRequest());
        auto downloadHandler = reinterpret_cast<UnityEngine::Networking::DownloadHandler*>(www->get_downloadHandler());
        auto gifDataArr = downloadHandler->GetData();
        Gif gif(gifDataArr);
        int error = gif.Parse();
        co_yield nullptr;
        if (!error && gif.Slurp())
        {
            co_yield nullptr;
            auto texture = gif.get_frame(0);
            auto sprite = Sprite::Create(texture, Rect(0.0f, 0.0f, (float)gif.get_width(), (float)gif.get_height()), Vector2(0.5f, 0.5f), 1024.0f, 1u, SpriteMeshType::FullRect, Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);
            out->set_sprite(sprite);
        }
        else
        {
            INFO("Failed to read gif with error code %d", error);
        }
        co_return;
    }

} // namespace WebUtils
