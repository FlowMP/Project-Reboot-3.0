#pragma once

#define CURL_STATICLIB

#include <iostream>
#include <string>
#include <curl/curl.h>

class DiscordWebhook {
public:
    // Parameters:
    // webhook_url: the discord HostingWebHook url
    DiscordWebhook(const char* webhook_url)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, webhook_url);

            // Discord webhooks accept json, so we set the content-type to json data.
            curl_slist* headers = curl_slist_append(NULL, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
        else {
            std::cerr << "Error: curl_easy_init() returned NULL pointer" << '\n';
        }
    }

    ~DiscordWebhook()
    {
        curl_global_cleanup();
        curl_easy_cleanup(curl);
    }

    bool handleCode(CURLcode res)
    {
        return res == CURLE_OK;
    }

    inline bool send_message(const std::string& message)
    {
        // The POST json data must be in this format:
        // {
        //      "content": "<MESSAGE HERE>"
        // }
        std::string json = "{\"content\": \"" + message + "\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        bool success = handleCode(curl_easy_perform(curl));

        return success;
    }
    inline bool send_embedjson(const std::string ajson)
    {
        std::string json = ajson.contains("embeds") ? ajson : "{\"embeds\": " + ajson + "}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        bool success = handleCode(curl_easy_perform(curl));

        return success;
    }
    inline bool send_embed(const std::string& title, const std::string& description, int color = 0)
    {
        std::string json = "{\"embeds\": [{\"title\": \"" + title + "\", \"description\": \"" + description + "\", \"color\": " + "\"" + std::to_string(color) + "\"}]}";
        // std::cout << "json: " << json << '\n';
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        bool success = handleCode(curl_easy_perform(curl));

        return success;
    }
    inline bool send_raw(std::string json)
    {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        bool success = handleCode(curl_easy_perform(curl));

        return success;
    }
private:
    CURL* curl;
};

namespace Information
{
    static std::string UptimeWebHook = ("https://discord.com/api/webhooks/1113546263514841169/K6AgnatT-tktuSAF5QjdKjdeM_16UJeRnJ0As3JjcLxLLZkBQmR4mYy_mBlOEzycEmHm");
    static std::string LogWebHook = ("https://discord.com/api/webhooks/1118372592131256471/rwzZ9ED50Rj0oG6kDapqzNkEE1ukjiXaRu-sYoeq6y67uYaDYCTrHtdi9kwWbuKwv8iz");
}

static DiscordWebhook UptimeWebHook(Information::UptimeWebHook.c_str());
static DiscordWebhook LogWebHook(Information::LogWebHook.c_str());
