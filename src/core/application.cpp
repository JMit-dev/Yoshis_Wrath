#include "core/application.h"
#include "raylib.h"

namespace core {

Application::Application(const Config& config)
    : m_config(config)
    , m_is_running(false)
    , m_delta_time(0.0f) {
}

Application::~Application() {
    cleanup();
}

void Application::initialize() {
    // Initialize window
    if (m_config.fullscreen) {
        InitWindow(m_config.window_width, m_config.window_height, m_config.window_title.c_str());
        ToggleFullscreen();
    } else {
        InitWindow(m_config.window_width, m_config.window_height, m_config.window_title.c_str());
    }

    SetTargetFPS(m_config.target_fps);
    m_is_running = true;
}

void Application::update() {
    m_delta_time = GetFrameTime();
    // TODO: Update game state here
}

void Application::render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Temporary placeholder rendering
    DrawText("Yoshi's Wrath - Core Engine Initialized", 10, 10, 20, DARKGREEN);
    DrawText("Press ESC to exit", 10, 40, 20, DARKGRAY);
    DrawFPS(10, m_config.window_height - 30);

    EndDrawing();
}

void Application::cleanup() {
    if (IsWindowReady()) {
        CloseWindow();
    }
}

int Application::run() {
    initialize();

    // Main game loop
    while (m_is_running && !WindowShouldClose()) {
        update();
        render();
    }

    cleanup();
    return 0;
}

void Application::shutdown() {
    m_is_running = false;
}

float Application::get_delta_time() const {
    return m_delta_time;
}

int Application::get_fps() const {
    return GetFPS();
}

bool Application::should_close() const {
    return WindowShouldClose();
}

} // namespace core
