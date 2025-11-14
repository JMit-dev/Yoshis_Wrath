#pragma once

#include <memory>
#include <string>

namespace core {

// Core application class managing window and main loop
// Follows separation of concerns - no game logic, just application lifecycle
class Application {
public:
    struct Config {
        std::string window_title;
        int window_width;
        int window_height;
        int target_fps;
        bool fullscreen;

        Config()
            : window_title("Yoshi's Wrath")
            , window_width(1280)
            , window_height(720)
            , target_fps(60)
            , fullscreen(false) {}
    };

    Application(const Config& config);
    ~Application();

    // Disable copy and move
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    // Main application loop - returns exit code
    int run();

    // Request application shutdown
    void shutdown();

    // Get delta time for current frame
    float get_delta_time() const;

    // Get current FPS
    int get_fps() const;

    // Check if window should close
    bool should_close() const;

private:
    Config m_config;
    bool m_is_running;
    float m_delta_time;

    void initialize();
    void update();
    void render();
    void cleanup();
};

} // namespace core
