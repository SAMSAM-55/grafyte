# Grafyte

Grafyte is a lightweight and simple 2D game engine written in C++ with OpenGL and exposed to Python using pybind11. It aims to provide a fast and easy-to-use interface for creating 2D games and interactive applications.

## Features

- **Simple API**: Designed for ease of use with a straightforward Pythonic interface.
- **Fast Rendering**: Core engine written in C++ utilizing OpenGL.
- **2D Sprites**: Support for textures and colored objects.
- **Text Rendering**: Easy text display with scale and color control.
- **Simple Collision System**: Integrated AABB-based collision detection with automatic resolution.
- **Input Management**: Easy-to-use action-based input system with support for keyboard triggers (Press, Hold, Release).
- **Scene-based Workflow**: Manage game objects within scenes.

## Installation

To install grafyte , simply run the following command in your terminal:

```bash
python -m pip install grafyte
```

## Building from Source

### Prerequisites

- Python 3.10+
- A C++ compiler (MSVC, GCC, or Clang)
- CMake
- OpenGL drivers

>[!NOTE]
> When applicable please make sure to test your code on both windows and linux as well as on Python 3.10 **to** 3.14 if possible.

### Build

To set up the development environment and build Grafyte:

**Windows:**
```powershell
scripts\setup.bat
.\.venv\Scripts\activate
pip install .
```

**Linux:**
```bash
chmod +x scripts/setup.sh
./scripts/setup.sh
source .venv/bin/activate
pip install .
```

Finally, you can build the python extension using CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Quick Start

Here's a minimal example to get you started:

```python
import grafyte
from grafyte import Key, InputTrigger

# Initialize application
app = grafyte.Application("My Game", (800, 600))
scene = app.make_new_scene()

# Create a player object
player = scene.spawn_object((0, 0), (50, 50))
player.set_color((0, 255, 0), 1.0) # Green color

# Register an action
app.create_input_action("move_right", Key.D, InputTrigger.Hold)

while not app.should_close():
    dt = app.get_delta_time()
    
    # Handle input
    if app.is_action_active("move_right"):
        player.move((100 * dt, 0))
    
    # Render scene
    app.render()

app.quit()
```

### Demo

Here is another exemple of a game made using only grafyte:
![grafyte demo](docs/grafyte_demo.gif)

## API Overview

### `Application`
The main entry point for the engine.
- `render()`: Draw the current frame.
- `should_close()`: Returns True if the window should close.
- `get_delta_time()`: Get time since last frame.
- `create_input_action(name, key, trigger)`: Map a key to a named action.

### `Scene`
Manages game objects.
- `spawn_object(pos, size, z_index=0, has_texture=False)`: Create a new sprite.
- `spawn_text_object(pos, text)`: Create a text object.

### `Object`
A 2D entity in the game.
- `move(offset)`: Relative movement.
- `move_to(position)`: Absolute movement.
- `set_color(rgba, alpha)`: Set object color.
- `use_texture(path, slot)`: Load and use a texture.
- `add_collision_box(pos, size)`: Add collision properties.
- `is_colliding()`: Check for collisions.

## License

Grafyte is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
