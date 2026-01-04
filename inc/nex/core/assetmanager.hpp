#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "asset.hpp"

namespace NEX::Core
{
    using AssetPtr = std::shared_ptr<IAsset>;

    struct AssetManager
    {
        AssetManager() = delete;

        template <typename T, typename... Args> static std::shared_ptr<T> ensure_loaded(const std::string& key, Args&&... args)
        {
            std::lock_guard<std::recursive_mutex> lock(mutex());

            auto& assets = storage();
            auto it = assets.find(key);
            if (it != assets.end())
                return std::dynamic_pointer_cast<T>(it->second);

            auto asset = std::make_shared<T>(std::forward<Args>(args)...);
            assets[key] = asset;
            return asset;
        }

        template <typename T> static bool save(const std::string& key, const std::shared_ptr<T>& asset)
        {
            if (!asset)
                return false;

            std::lock_guard<std::recursive_mutex> lock(mutex());

            auto& assets = storage();
            auto it = assets.find(key);

            if (it != assets.end())
                if (!std::dynamic_pointer_cast<T>(it->second))
                    return false;

            assets[key] = asset;
            return true;
        }

        template <typename T> static std::shared_ptr<T> get(const std::string& key)
        {
            std::lock_guard<std::recursive_mutex> lock(mutex());

            auto& assets = storage();
            auto it = assets.find(key);
            if (it == assets.end())
                return nullptr;

            return std::dynamic_pointer_cast<T>(it->second);
        }

        template <typename T> static bool remove_ptr(const std::shared_ptr<T>& ptr)
        {
            if (!ptr)
                return false;

            std::lock_guard<std::recursive_mutex> lock(mutex());

            auto& assets = storage();

            for (auto it = assets.begin(); it != assets.end(); ++it)
                if (std::dynamic_pointer_cast<T>(it->second) == ptr)
                {
                    assets.erase(it);
                    return true;
                }
            return false;
        }

        template <typename T, typename... Args> static void queue(const std::string& key, Args&&... args)
        {
            std::lock_guard<std::recursive_mutex> lock(mutex());

            get_queue().push_back({key, [=]() { return std::make_shared<T>(args...); }});
        }

#ifdef NEX_ASSET_LOADER_TOML
        static size_t queue_from_toml(const std::string& path);
#endif

        static bool remove(const std::string& key);
        static void remove_all();
        static bool process_enqueued();
        static size_t enqueued_count();

      private:
        using Storage = std::unordered_map<std::string, AssetPtr>;

        struct AssetLoad
        {
            std::string key;
            std::function<AssetPtr()> loader;
        };

        static Storage& storage();
        static std::recursive_mutex& mutex();
        static std::vector<AssetLoad>& get_queue();
    };
}; // namespace NEX::Core
