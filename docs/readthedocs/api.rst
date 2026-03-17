API Overview
============

This page provides a high-level overview of the main classes in Grafyte.

Application
-----------

The main entry point for the engine.

- ``render()``: Draw the current frame.
- ``should_close()``: Returns True if the window should close.
- ``get_delta_time()``: Get time since last frame.
- ``create_input_action(name, key, trigger)``: Map a key to a named action.

Scene
-----

Manages game objects.

- ``spawn_object(pos, size, z_index=0, has_texture=False)``: Create a new sprite.
- ``spawn_text_object(pos, text)``: Create a text object.

Object
------

A 2D entity in the game.

- ``move(offset)``: Relative movement.
- ``move_to(position)``: Absolute movement.
- ``set_color(rgba, alpha)``: Set object color.
- ``use_texture(path, slot)``: Load and use a texture.
- ``add_collision_box(pos, size)``: Add collision properties.
- ``is_colliding()``: Check for collisions.
