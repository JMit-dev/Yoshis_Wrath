#include "core/application.h"

int main() {
    // Configure application
    core::Application::Config config;
    config.window_title = "Yoshi's Wrath";
    config.window_width = 1920;  // Default to 1080p widescreen
    config.window_height = 1080;
    config.target_fps = 60;
    config.fullscreen = false;

    // Create and run application
    core::Application app(config);
    return app.run();
}
