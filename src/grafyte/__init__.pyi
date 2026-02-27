from typing import overload, Union

from .__converters import Vec2f, Color, Vec2Like

from __grafyte_internal import Key as _NativeKey
from __grafyte_internal import InputTrigger as _NativeInputTrigger
from __grafyte_internal import Vec2 as _NativeVec2
from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Scene as _NativeScene
from __grafyte_internal import Direction as _NativeDirection
from __grafyte_internal import AABB as _NativeAABB
from __grafyte_internal import Hit as _NativeHit

class Key(_NativeKey): ...
class InputTrigger(_NativeInputTrigger): ...
class Vec2(_NativeVec2): ...
class Direction(_NativeDirection): ...
class AABB(_NativeAABB): ...
class Hit(_NativeHit): ...

class Application:
    """
    The main application class that manages the window, rendering context, and input.
    """
    def __init__(self, name: str, window_dimensions: Vec2Like):
        """
        Initializes the application window and rendering context.

        :param name: The title of the window.
        :param window_dimensions: A Vec2d containing the width and height of the window.
        """
        ...
    def should_close(self) -> bool:
        """
        Checks if the application window should close (e.g., if the close button was clicked).
        Also updates internal timing for delta time calculations.

        :return: True if the window should close, False otherwise.
        """
        ...
    def quit(self) -> None:
        """
        Closes the application window and cleans up resources.
        """
        ...
    def get_now(self) -> float:
        """
        Returns the current time in seconds since the application started.

        :return: Current time as a float.
        """
        ...
    def get_delta_time(self) -> float:
        """
        Returns the time elapsed since the last frame.

        :return: Delta time in seconds.
        """
        ...
    def render(self) -> None:
        """
        Renders all objects and text in the current frame.
        """
        ...
    # def add_text(self, text: str, scale: float, pos: Vec2d) -> int:
    #     """
    #     Adds text to be rendered in the application window.

    #     :param text: The string of text to display.
    #     :param scale: The scale/size of the text.
    #     :param pos: A Vec2d representing the (x, y) position of the text.
    #     :return: An integer ID for the added text, which can be used to remove it later.
    #     """
    #     ...
    # def remove_text(self, id: int):
    #     """
    #     Removes previously added text from the application.

    #     :param id: The ID of the text to remove (returned by add_text).
    #     """
    #     ...
    def set_background_color(self, color: Color):
        """
        Sets the background (clear) color of the application window.

        :param color: A Color object (RGB) representing the new background color.
        """
        ...
    @staticmethod
    def is_key_down(key: Key) -> bool:
        """
        Checks if a specific key is currently being held down.

        :param key: The Key enum value.
        :return: True if the key is down, False otherwise.
        """
        ...
    @staticmethod
    def was_key_pressed(key: Key) -> bool:
        """
        Checks if a specific key was pressed in the current frame.

        :param key: The Key enum value.
        :return: True if the key was just pressed, False otherwise.
        """
        ...
    @staticmethod
    def was_key_released(key: Key) -> bool:
        """
        Checks if a specific key was released in the current frame.

        :param key: The Key enum value.
        :return: True if the key was just released, False otherwise.
        """
        ...
    @staticmethod
    def create_input_action(name: str, key: Key, trigger: InputTrigger) -> None:
        """
        Creates a new input action for the application.

        :param trigger:
        :param name: The name of the action.
        :param key: The Key to bind to the action.
        """
        ...
    @staticmethod
    def is_action_active(action: str) -> bool:
        """
        Checks if a specific action is currently being held down.

        :param action: The name of the action.
        :return: True if the action is down, False otherwise.
        """
        ...

    def make_new_scene(self) -> Scene: ...

class Scene:
    __native: _NativeScene

    def __init__(self, native: _NativeScene): ...

    def spawn_object(self,
                     pos: Vec2Like,
                     size: Vec2Like,
                     layer: int = 0,
                     has_texture: bool = False,
                     shader_source_path: str = "") -> Object: ...

class Object:
    """
    Represents a renderable 2D object in the engine.
    """

    __native: _NativeObject

    @property
    def pos(self) -> Vec2:
        """The current position of the object."""
        ...

    @property
    def rot(self) -> float:
        """The current rotation of the object."""
        ...

    @property
    def scale(self) -> Vec2:
        """The current scale of the object."""
        ...

    def __init__(self, native: _NativeObject): ...

    def set_color(self, color: Color, a: float = 1) -> None:
        """
        Sets the uniform color of the object. Only applicable if the object doesn't have a texture.

        :param color: A Color object (RGB) for the object.
        :param a: The alpha chanel for the color. (default = 1)
        """
        ...

    def add_collision_box(self, pos: Vec2Like, size: Vec2Like): ...
    def collides_with(self, other: Object) -> Hit: ...
    def is_colliding(self) -> Hit: ...
    def enable_auto_collides(self): ...

    def move(self, offset: Vec2Like) -> None:
        """
        Moves the object by a given offset from its current position.

        :param offset: A Vec2d representing the (dx, dy) to move the object.
        """
        ...
    def move_to(self, pos: Vec2Like) -> None:
        """
        Moves the object to a specific absolute position.

        :param pos: A Vec2d representing the new (x, y) position.
        """
        ...
    def rotate(self,angle: float) -> None: ...
    def set_rotation(self,angle: float) -> None: ...

    @overload
    def set_scale(self, scale: Vec2Like) -> None: ...

    @overload
    def set_scale(self, scale: float) -> None: ...

    def use_texture(self, path: str, slot: int = 0) -> None:
        """
        Loads and applies a texture to the object.

        :param slot: The slot to bind the texture to
        :param path: The file path to the texture.
        """
        ...
    def set_tint(self, tint: Color, strength: float = 1):
        """
        Sets a color tint to be applied over the object's texture.

        :param tint: A Color object (RGB) to use as the tint.
        :param strength: The strength of the tint effect. (default = 1)
        """
        ...