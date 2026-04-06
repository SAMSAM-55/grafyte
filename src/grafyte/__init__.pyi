from typing import overload, Union

from .__converters import Vec2f, Color, Vec2Like
from .__class_utils import _KeyAccessor, _KeyPressedAccessor, _KeyReleasedAccessor, _ActionAccessor

from __grafyte_internal import Key as _NativeKey
from __grafyte_internal import InputTrigger as _NativeInputTrigger
from __grafyte_internal import InputManager as _NativeInputManager
from __grafyte_internal import Vec2 as _NativeVec2
from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import TextObject as _NativeTextObject
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
class Vec2Proxy:
    def __init__(self, getter, setter): ...
    def __iadd__(self, other: Vec2Like): ...
    def __isub__(self, other: Vec2Like): ...
    def __imul__(self, other: float): ...
    def __iter__(self): ...

class RotProxy:
    @property
    def value(self) -> float: ...
    @value.setter
    def value(self, value: float) -> None: ...
    def set(self, value: float) -> RotProxy: ...
    def __init__(self, getter, setter): ...
    def __float__(self) -> float: ...
    def __int__(self) -> int: ...
    def __iadd__(self, other: float): ...
    def __isub__(self, other: float): ...
    def __imul__(self, other: float): ...
    def __itruediv__(self, other: float): ...

class InputManager(_NativeInputManager):
    key: _KeyAccessor
    key_pressed: _KeyPressedAccessor
    key_released: _KeyReleasedAccessor

    def __init__(self): ...

    def __getitem__(self, action: str):
        """
        Checks if a specific action is currently active.

        :param action: The name of the action.
        :return: True if the action is active, False otherwise.
        """
        ...

    @staticmethod
    def create_action(name: str, trigger: InputTrigger, *key: Key) -> None:
        """
        Creates a new input action for the application.

        :param trigger:
        :param name: The name of the action.
        :param key: The Key(s) to bind to the action.
        """
        ...

class Application:
    """
    The main application class that manages the window, rendering context, and input.
    """

    __input: InputManager

    @property
    def input(self) -> InputManager: ...

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
    def add_text(self, text: str, scale: float, pos: Vec2Like) -> int:
        """
        Adds text to be rendered in the application window.

        :param text: The string of text to display.
        :param scale: The scale/size of the text.
        :param pos: A Vec2d representing the (x, y) position of the text.
        :return: An integer ID for the added text, which can be used to remove it later.
        """
        ...
    def set_text(self, id: int, text: str): ...
    def remove_text(self, id: int):
        """
        Removes previously added text from the application.

        :param id: The ID of the text to remove (returned by add_text).
        """
        ...
    def set_background_color(self, color: Color):
        """
        Sets the background (clear) color of the application window.

        :param color: A Color object (RGB) representing the new background color.
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

    def spawn_text_object(self, pos: Vec2Like, text: str, scale: float = 12) -> TextObject: ...

class TextObject:
    __native: _NativeTextObject

    def __init__(self, native: _NativeTextObject): ...

    def set_text(self, text: str) -> None: ...
    def set_scale(self, scale: float) -> None: ...
    def set_color(self, color: Color, a: float = 1) -> None: ...

def set_text(self, text: str): ...

class Object:
    """
    Represents a renderable 2D object in the engine.
    """

    __native: _NativeObject
    __pos_proxy: Vec2Proxy
    __scale_proxy: Vec2Proxy
    __rot_proxy: RotProxy

    def _get_pos(self): ...
    def _set_pos(self, value): ...

    def _get_scale(self): ...
    def _set_scale(self, value): ...

    def _get_rot(self): ...
    def _set_rot(self, value): ...

    @property
    def pos(self) -> Vec2:
        """The current position of the object."""
        ...

    @pos.setter
    def pos(self, v: Vec2Like) -> None: ...

    @property
    def rot(self) -> float:
        """The current rotation of the object."""
        ...

    @rot.setter
    def rot(self, v: float) -> None: ...

    @property
    def scale(self) -> Vec2:
        """The current scale of the object."""
        ...

    @scale.setter
    def scale(self, v: Vec2Like) -> None: ...

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
    def enable_auto_collides(self, order: int): ...

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
