#include "platform/file_system.h"
#include <sys/stat.h>
#include <algorithm>

#ifdef _WIN32
    #define PLATFORM_WINDOWS
    #include <windows.h>
#elif defined(__APPLE__) || defined(__MACH__)
    #define PLATFORM_MACOS
    #include <unistd.h>
#elif defined(__linux__)
    #define PLATFORM_LINUX
    #include <unistd.h>
#endif

namespace platform {

std::string FileSystem::get_assets_path() {
#ifdef ASSETS_PATH
    return std::string(ASSETS_PATH);
#else
    return "./assets/";
#endif
}

std::string FileSystem::join_path(const std::string& path1, const std::string& path2) {
    if (path1.empty()) {
        return path2;
    }
    if (path2.empty()) {
        return path1;
    }

    char sep = get_path_separator();

    // Check if path1 ends with separator
    bool path1_ends_with_sep = (path1.back() == '/' || path1.back() == '\\');
    // Check if path2 starts with separator
    bool path2_starts_with_sep = (path2.front() == '/' || path2.front() == '\\');

    if (path1_ends_with_sep && path2_starts_with_sep) {
        // Both have separator, remove one
        return path1 + path2.substr(1);
    } else if (!path1_ends_with_sep && !path2_starts_with_sep) {
        // Neither has separator, add one
        return path1 + sep + path2;
    } else {
        // One has separator, just concatenate
        return path1 + path2;
    }
}

std::string FileSystem::join_paths(const std::string& base,
                                   const std::string& path1,
                                   const std::string& path2) {
    return join_path(join_path(base, path1), path2);
}

std::string FileSystem::normalize_path(const std::string& path) {
    std::string result = path;
    char target_sep = get_path_separator();
    char other_sep = (target_sep == '/') ? '\\' : '/';

    // Replace all wrong separators with correct ones
    std::replace(result.begin(), result.end(), other_sep, target_sep);

    return result;
}

bool FileSystem::file_exists(const std::string& path) {
#ifdef PLATFORM_WINDOWS
    DWORD attrib = GetFileAttributesA(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES &&
            !(attrib & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISREG(buffer.st_mode));
#endif
}

bool FileSystem::directory_exists(const std::string& path) {
#ifdef PLATFORM_WINDOWS
    DWORD attrib = GetFileAttributesA(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES &&
            (attrib & FILE_ATTRIBUTE_DIRECTORY));
#else
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && S_ISDIR(buffer.st_mode));
#endif
}

char FileSystem::get_path_separator() {
#ifdef PLATFORM_WINDOWS
    return '\\';
#else
    return '/';
#endif
}

} // namespace platform
