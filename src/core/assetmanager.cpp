#include "core/assetmanager.hpp"

#include "core/audio.hpp"
#include "core/defines.hpp"
#include "gl/assets/all.hpp"
#include "gl/mtsdf/font.hpp"

#define TOML_IMPLEMENTATION
#include <toml++/toml.hpp>

#include <iostream>

using namespace NEX::Core;

bool AssetManager::remove(const std::string &key)
{
    std::lock_guard<std::recursive_mutex> lock(mutex());

    auto &assets = storage();
    auto it = assets.find(key);
    if (it == assets.end())
        return false;

    assets.erase(it);
    return true;
}

void AssetManager::remove_all()
{
    auto &assets = storage();
    assets.clear();
}

bool AssetManager::process_enqueued()
{
    std::lock_guard<std::recursive_mutex> lock(mutex());

    auto &q = get_queue();
    if (q.empty())
        return false;

    auto task = q.front();
    q.erase(q.begin());

    storage()[task.key] = task.loader();
    return true;
}

size_t AssetManager::enqueued_count()
{
    return get_queue().size();
}

size_t AssetManager::queue_from_toml(const std::string &path)
{
    const toml::parse_result result = toml::parse_file(path);
    if (!result)
    {
        std::cerr << TTY_RED << "[EROR] Could not parse toml at path " << path << ": (" << result.error() << ")\n" << TTY_RESET;
        return 0;
    }

    size_t count = 0;
    const toml::table &tbl = result.table();

    if (auto *shader_array = tbl["shader"].as_array())
        for (const toml::node &node : *shader_array)
        {
            const auto &t = *node.as_table();
            if (!t["id"] || !t["vertex"] || !t["fragment"])
                continue;
            queue<GL::Shader>(std::string(t["id"].value_or("<error>")), std::string(t["vertex"].value_or("<error>")),
                              std::string(t["fragment"].value_or("<error>")));
            count++;
        }

    if (auto *shader_array = tbl["font_mtsdf"].as_array())
        for (const toml::node &node : *shader_array)
        {
            const auto &t = *node.as_table();
            if (!t["id"] || !t["image"] || !t["data"])
                continue;
            queue<GL::MTSDF::Font>(std::string(t["id"].value_or("<error>")), std::string(t["image"].value_or("<error>")),
                                   std::string(t["data"].value_or("<error>")));
            count++;
        }

    if (auto *shader_array = tbl["audio"].as_array())
        for (const toml::node &node : *shader_array)
        {
            const auto &t = *node.as_table();
            if (!t["id"] || !t["path"] || !t["predecode"])
                continue;
            queue<Core::Audio>(std::string(t["id"].value_or("<error>")), std::string(t["path"].value_or("<error>")), t["predecode"].value_or(false));
            count++;
        }

    return count;
}

AssetManager::Storage &AssetManager::storage()
{
    static Storage s;
    return s;
}

std::recursive_mutex &AssetManager::mutex()
{
    static std::recursive_mutex m;
    return m;
}

std::vector<AssetManager::AssetLoad> &AssetManager::get_queue()
{
    static std::vector<AssetLoad> q;
    return q;
}
