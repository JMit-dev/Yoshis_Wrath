# Yoshi's Wrath

A Doom-style first-person shooter featuring Yoshi in a brutal revenge mission to recover stolen eggs. Built with Raylib and featuring BSP-based rendering with sector portals.

## Features

- Classic Doom-style FPS gameplay
- BSP-based rendering with sector portal support
- Custom level editor
- Cross-platform support (Windows, Linux, macOS)

## Building from Source

### Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler
  - Windows: Visual Studio 2017 or later / MinGW
  - Linux: GCC 7+ or Clang 5+
  - macOS: Xcode 10+

### Linux Additional Dependencies

```bash
sudo apt-get update
sudo apt-get install -y libgl1-mesa-dev libx11-dev libxrandr-dev libxi-dev libxcursor-dev libxinerama-dev
```

### Build Steps

1. Clone the repository with submodules:
```bash
git clone --recursive https://github.com/yourusername/yoshis-wrath.git
cd yoshis-wrath
```

2. Configure and build:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

3. Run the game:
```bash
# Windows
.\Release\yoshis_wrath.exe

# Linux/macOS
./yoshis_wrath
```

The executable will automatically find the `assets` folder in the same directory.

## Project Structure

```
yoshis-wrath/
├── src/           # Source code
├── include/       # Header files
├── assets/        # Game assets (textures, sounds, etc.)
├── external/      # External dependencies (Raylib)
└── build/         # Build output directory
```

## License

See [LICENSE](LICENSE) for details.

## Development

This project follows strict C++17 standards and SOLID principles. See [CLAUDE.md](CLAUDE.md) for detailed development guidelines and architecture documentation.
