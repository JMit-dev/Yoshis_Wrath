#include "core/application.h"
#include "game/level.h"
#include "raylib.h"

namespace core {

Application::Application(const Config& config)
    : m_config(config)
    , m_is_running(false)
    , m_delta_time(0.0f)
    , m_game_state(nullptr)
    , m_input_provider(nullptr)
    , m_renderer(nullptr) {
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

    // Initialize subsystems
    m_input_provider = std::make_unique<platform::RaylibInputProvider>();
    m_input_provider->capture_mouse(true);

    m_renderer = std::make_unique<rendering::BasicRenderer>();

    m_game_state = std::make_unique<game::GameState>();

    // Load test level and move it into game state
    m_game_state->initialize(game::Level::create_test_level());

    m_is_running = true;
}

void Application::update() {
    m_delta_time = GetFrameTime();

    // Get input state
    platform::InputState input = m_input_provider->get_current_state();

    // Update game state
    m_game_state->update(m_delta_time, input);

    // Update weapon animation (cast to BasicRenderer to access weapon methods)
    auto* basic_renderer = static_cast<rendering::BasicRenderer*>(m_renderer.get());
    basic_renderer->update_weapon(m_delta_time);

    // Handle weapon attack
    if (input.shoot) {
        basic_renderer->trigger_weapon_attack();
    }

    // Handle pause
    if (input.pause) {
        m_game_state->set_paused(!m_game_state->is_paused());
    }
}

void Application::render() {
    m_renderer->begin_frame();
    m_renderer->render(m_game_state->get_level(), m_game_state->get_camera());
    m_renderer->end_frame();
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
