# Voxel Game Engine

A lightweight, open-source voxel-based game engine built with modern game development practices.

## Features

- **Voxel-based world generation**: Create infinite procedurally generated worlds
- **Chunk-based rendering**: Efficient terrain loading and rendering
- **Block placement and destruction**: Interactive voxel manipulation
- **Lighting system**: Dynamic and static light sources
- **Physics**: Basic collision detection and player movement
- **Multiplayer ready**: Network architecture for future multiplayer support

## Tech Stack

- **Engine**: C++17 with modern CMake
- **Graphics**: OpenGL 4.6+
- **Physics**: Custom AABB collision system
- **Build System**: CMake 3.15+

## Getting Started

### Prerequisites

- C++ compiler (C++17 or higher)
- CMake 3.15+
- GLFW3
- GLM
- GLAD

### Installation

```bash
git clone https://github.com/brightcodek-sys/voxel-game.git
cd voxel-game
mkdir build
cd build
cmake ..
make
```

### Running

```bash
./voxel_game
```

## Controls

- **WASD** - Move forward/back/left/right
- **SPACE** - Jump
- **TAB** - Toggle mouse capture
- **LMB** - Destroy blocks
- **RMB** - Place blocks
- **ESC** - Exit

## Project Structure

```
voxel-game/
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── engine/
│   │   ├── world.cpp
│   │   ├── chunk.cpp
│   │   └── block.cpp
│   ├── graphics/
│   │   ├── shader.cpp
│   │   ├── texture.cpp
│   │   └── renderer.cpp
│   ├── physics/
│   │   └── collision.cpp
│   └── utils/
│       └── noise.cpp
├── include/
│   ├── engine/
│   │   ├── world.h
│   │   ├── chunk.h
│   │   └── block.h
│   ├── graphics/
│   │   ├── shader.h
│   │   └── renderer.h
│   ├── physics/
│   │   └── collision.h
│   └── utils/
│       └── noise.h
├── shaders/
│   ├── vertex.glsl
│   └── fragment.glsl
└── README.md
```

## Development Roadmap

- [x] Basic project structure
- [x] Chunk system and world generation
- [x] Block placement/destruction
- [x] Player controls and camera
- [x] Rendering pipeline
- [ ] Texture mapping
- [ ] Advanced lighting (shadows, AO)
- [ ] Inventory system
- [ ] More biomes
- [ ] Multiplayer support
- [ ] World persistence

## License

MIT License - See LICENSE file for details

## Contributing

Contributions are welcome! Please fork and submit pull requests.
