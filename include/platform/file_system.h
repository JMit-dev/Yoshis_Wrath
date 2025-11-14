#pragma once

#include <string>

namespace platform {

// Cross-platform file system utilities
class FileSystem {
public:
    // Get the assets directory path with proper separators for current platform
    static std::string get_assets_path();

    // Join path components with proper separator for current platform
    static std::string join_path(const std::string& path1, const std::string& path2);

    // Join multiple path components
    static std::string join_paths(const std::string& base,
                                  const std::string& path1,
                                  const std::string& path2);

    // Convert path to use native separators
    static std::string normalize_path(const std::string& path);

    // Check if file exists
    static bool file_exists(const std::string& path);

    // Check if directory exists
    static bool directory_exists(const std::string& path);

    // Get platform-specific path separator ('/' or '\\')
    static char get_path_separator();

private:
    FileSystem() = delete;  // Static class, no instances
};

} // namespace platform
