#include "core/assetmanager.hpp"

#include "core/all.hpp"
#include "gl/all.hpp"
#include "gl/mtsdf/font.hpp"

#ifdef NEX_ASSET_LOADER_TOML
#define TOML_IMPLEMENTATION
#include <toml++/toml.hpp>

#include <iostream>
#endif

using namespace NEX::Core;
using namespace NEX::GL;

bool AssetManager::remove(const std::string& key)
{
    std::lock_guard<std::recursive_mutex> lock(mutex());

    auto& assets = storage();
    auto it = assets.find(key);
    if (it == assets.end())
        return false;

    assets.erase(it);
    return true;
}

void AssetManager::remove_all()
{
    auto& assets = storage();
    assets.clear();
}

bool AssetManager::process_enqueued()
{
    std::lock_guard<std::recursive_mutex> lock(mutex());

    auto& q = get_queue();
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

#ifdef NEX_ASSET_LOADER_TOML
static GLint parse_filter(const std::string& v)
{
    if (v == "nearest")
        return GL_NEAREST;
    if (v == "linear")
        return GL_LINEAR;
    return GL_NEAREST;
}

static GLint parse_wrap(const std::string& v)
{
    if (v == "repeat")
        return GL_REPEAT;
    if (v == "clamp")
        return GL_CLAMP_TO_EDGE;
    return GL_CLAMP_TO_EDGE;
}

static GLint parse_format(const std::string& v)
{
    if (v == "rgb")
        return GL_RGB;
    if (v == "rgba")
        return GL_RGBA;
    return GL_RGB;
}

size_t AssetManager::queue_from_toml(const std::string& path)
{
    const toml::parse_result result = toml::parse_file(path);
    if (!result)
    {
        std::cerr << TTY_RED << "[EROR] Could not parse toml at path " << path << ": (" << result.error() << ")\n" << TTY_RESET;
        return 0;
    }

    size_t count = 0;
    const toml::table& tbl = result.table();

    if (auto* shader_array = tbl["shader"].as_array())
        for (const toml::node& node : *shader_array)
        {
            const auto* t = node.as_table();
            if (!t)
                continue;

            const auto id = t->get_as<std::string>("id");
            const auto vertex = t->get_as<std::string>("vertex");
            const auto fragment = t->get_as<std::string>("fragment");

            if (!id || !vertex || !fragment)
                continue;

            queue<Shader>(std::string(*id), std::string(*vertex), std::string(*fragment));
            count++;
        }

    if (auto* font_array = tbl["font_mtsdf"].as_array())
        for (const toml::node& node : *font_array)
        {
            const auto* t = node.as_table();
            if (!t)
                continue;

            const auto id = t->get_as<std::string>("id");
            const auto image = t->get_as<std::string>("image");
            const auto data = t->get_as<std::string>("data");

            if (!id || !image || !data)
                continue;

            queue<MTSDF::Font>(std::string(*id), std::string(*image), std::string(*data));
            count++;
        }

    if (auto* audio_array = tbl["audio"].as_array())
        for (const toml::node& node : *audio_array)
        {
            const auto* t = node.as_table();
            if (!t)
                continue;

            const auto id = t->get_as<std::string>("id");
            const auto path = t->get_as<std::string>("path");
            const auto predecode = t->get_as<bool>("predecode");

            if (!id || !path)
                continue;

            queue<Audio>(std::string(*id), std::string(*path), predecode ? bool(*predecode) : false);
            count++;
        }

    if (auto* tex_array = tbl["texture"].as_array())
        for (const toml::node& node : *tex_array)
        {
            const auto* t = node.as_table();
            if (!t)
                continue;

            const auto id = t->get_as<std::string>("id");
            const auto path = t->get_as<std::string>("path");

            if (!id || !path)
                continue;

            TextureConfig cfg;
            cfg.path = std::string(*path);

            if (auto v = t->get_as<std::string>("min_filter"))
                cfg.min_filter = parse_filter(std::string(*v));

            if (auto v = t->get_as<std::string>("mag_filter"))
                cfg.mag_filter = parse_filter(std::string(*v));

            if (auto v = t->get_as<std::string>("wrap_s"))
                cfg.wrap_s = parse_wrap(std::string(*v));

            if (auto v = t->get_as<std::string>("wrap_t"))
                cfg.wrap_t = parse_wrap(std::string(*v));

            if (auto v = t->get_as<std::string>("gpu_format"))
                cfg.gpu_format = parse_format(std::string(*v));

            if (auto v = t->get_as<std::string>("format"))
                cfg.format = parse_format(std::string(*v));

            queue<Texture>(std::string(*id), std::move(cfg));
            count++;
        }

    if (auto* tex_arrays = tbl["texture_array"].as_array())
        for (const toml::node& node : *tex_arrays)
        {
            const auto* t = node.as_table();
            if (!t)
                continue;

            const auto id = t->get_as<std::string>("id");
            const auto* textures = t->get_as<toml::array>("textures");

            if (!id || !textures || textures->empty())
                continue;

            std::vector<TextureConfig> configs;
            configs.reserve(textures->size());

            for (const toml::node& tex_node : *textures)
            {
                const auto* tex = tex_node.as_table();
                if (!tex)
                    continue;

                const auto path = tex->get_as<std::string>("path");
                if (!path)
                    continue;

                TextureConfig cfg;
                cfg.path = std::string(*path);

                if (auto v = tex->get_as<std::string>("min_filter"))
                    cfg.min_filter = parse_filter(std::string(*v));

                if (auto v = tex->get_as<std::string>("mag_filter"))
                    cfg.mag_filter = parse_filter(std::string(*v));

                if (auto v = tex->get_as<std::string>("wrap_s"))
                    cfg.wrap_s = parse_wrap(std::string(*v));

                if (auto v = tex->get_as<std::string>("wrap_t"))
                    cfg.wrap_t = parse_wrap(std::string(*v));

                if (auto v = tex->get_as<std::string>("gpu_format"))
                    cfg.gpu_format = parse_format(std::string(*v));

                if (auto v = tex->get_as<std::string>("format"))
                    cfg.format = parse_format(std::string(*v));

                configs.push_back(std::move(cfg));
            }

            if (!configs.empty())
            {
                queue<TextureArray>(std::string(*id), std::move(configs));
                count++;
            }
        }

    return count;
}
#endif

AssetManager::Storage& AssetManager::storage()
{
    static Storage s;
    return s;
}

std::recursive_mutex& AssetManager::mutex()
{
    static std::recursive_mutex m;
    return m;
}

std::vector<AssetManager::AssetLoad>& AssetManager::get_queue()
{
    static std::vector<AssetLoad> q;
    return q;
}
