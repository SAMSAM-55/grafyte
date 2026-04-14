from .__converters import Vec2f, Color, Vec2Like
from .__class_utils import _KeyAccessor, _KeyPressedAccessor, _KeyReleasedAccessor

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
from __grafyte_internal import Camera as _NativeCamera
from __grafyte_internal import UIManager as _NativeUIManager
from __grafyte_internal import Text as _NativeText
from __grafyte_internal import Anchor as _NativeAnchor


class Key(_NativeKey): ...


class InputTrigger(_NativeInputTrigger): ...


class Vec2(_NativeVec2): ...


class Direction(_NativeDirection): ...


class AABB(_NativeAABB): ...


class Hit(_NativeHit): ...


class Anchor(_NativeAnchor): ...


class Vec2Proxy:
    def __init__(self, getter, setter): ...

    def __iadd__(self, other: Vec2Like): ...

    def __isub__(self, other: Vec2Like): ...

    def __imul__(self, other: float): ...

    def __iter__(self): ...

    @property
    def x(self): ...

    @x.setter
    def x(self, value): ...

    @property
    def y(self): ...

    @y.setter
    def y(self, value): ...


class ColorProxy:
    @property
    def r(self) -> int: ...

    @r.setter
    def r(self, value: int) -> None: ...

    @property
    def g(self) -> int: ...

    @g.setter
    def g(self, value: int) -> None: ...

    @property
    def b(self) -> int: ...

    @b.setter
    def b(self, value: int) -> None: ...

    @property
    def a(self) -> float: ...

    @a.setter
    def a(self, value: float) -> None: ...

    def __init__(self, getter, setter): ...

    def __iter__(self): ...


class TintProxy(ColorProxy):
    @property
    def strength(self) -> float: ...

    @strength.setter
    def strength(self, value: float) -> None: ...

    @property
    def a(self) -> float: ...

    @a.setter
    def a(self, value: float) -> None: ...

    def __repr__(self) -> str: ...


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


class Camera:
    __native: _NativeCamera
    __pos_proxy: Vec2Proxy
    __follow_offset_proxy: Vec2Proxy

    def __init__(self): ...

    @property
    def pos(self) -> Vec2Proxy:
        """
        The current position of the camera.
        """
        ...

    @pos.setter
    def pos(self, v: Vec2Like): ...

    def follow(self, object: Object) -> None:
        """
        Sets the current object that the camera follows.
        :param object: The object to follow.
        """
        ...

    @property
    def follow_offset(self) -> Vec2Proxy:
        """
        The offset of the camera relative to the object it follows.
        """
        ...

    @follow_offset.setter
    def follow_offset(self, v: Vec2Like): ...

    @property
    def zoom(self) -> float:
        """
        The current zoom of the camera.
        """
        ...

    @zoom.setter
    def zoom(self, v: float): ...


class InputManager(_NativeInputManager):
    key: _KeyAccessor
    key_pressed: _KeyPressedAccessor
    key_released: _KeyReleasedAccessor

    def __init__(self): ...

    def __getitem__(self, action: str) -> bool:
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

        :param trigger: The trigger mode for the action (e.g., Press or Hold).
        :param name: The name of the action.
        :param key: The Key(s) to bind to the action.
        """
        ...


class Text:
    __native: _NativeText
    __color_proxy: ColorProxy

    def __init__(self, native: _NativeText): ...

    @property
    def text(self) -> str:
        """
        The current text displayed by the object.
        This property is write-only.
        """
        ...

    @text.setter
    def text(self, value: str) -> None: ...

    @property
    def scale(self) -> float:
        """
        The current scale of the text.
        This property is write-only.
        """
        ...

    @scale.setter
    def scale(self, value: float) -> None: ...

    @property
    def color(self) -> ColorProxy:
        """
        The current color of the text.
        """
        ...

    @color.setter
    def color(self, v: Color | tuple[Color, float] | tuple[int, int, int, float]) -> None: ...


class UIManager:
    """
    The main class for managing screen space elements like text.
    """
    __native: _NativeUIManager

    def __init__(self, native_ui_manager: _NativeUIManager): ...

    def add_text(self,
                 pos: Vec2Like,
                 text: str,
                 scale: float = 12,
                 anchor: Anchor = Anchor.TopLeft) -> Text:
        """
        Displays a new screen space text on the screen.

        :param pos: The position of the text relative to its anchor in pixels.
        :param text: The text to display on the screen.
        :param scale: The scale of the text in pt (default = 12)
        :param anchor: The position to anchor the text onto. (default = Anchor.TopLeft)
        """
        ...

    def remove_text(self, text: Text) -> None:
        """
        Removes the given text from the screen.

        :param text: The text to remove.
        """
        ...


class Application:
    """
    The main application class that manages the window, rendering context, and input.
    """

    __input: InputManager
    __active_scene: Scene | None
    __scene_cache: dict[int, Scene]
    __active_ui: UIManager | None
    __ui_cache: dict[int, UIManager]

    @property
    def input(self) -> InputManager:
        """
        The inputs system of the Application
        """
        ...

    def __init__(self, name: str, window_dimensions: Vec2Like, font_path: str = "@embed/Fonts/Base"):
        """
        Initializes the application window and rendering context.

        :param name: The title of the window.
        :param window_dimensions: The width and height of the window.
        :param font_path: The path to the Application font, if omitted, an embedded font is used.
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

    def render(self) -> None:
        """
        Renders all objects and text in the current frame.
        """
        ...

    @property
    def background_color(self) -> Color:
        """
        The background (clear) color of the application window.
        This property is write-only.
        """
        ...

    @background_color.setter
    def background_color(self, color: Color) -> None: ...

    @property
    def now(self) -> float:
        """
        The time the application has been running for in seconds.
        """
        ...

    @property
    def dt(self) -> float:
        """
        The elapsed time since the last frame was rendered (delta-time).
        """
        ...

    def make_new_scene(self) -> Scene:
        """
        Creates a new scene for the application.
        :return: A new scene.
        """
        ...

    @property
    def scene(self) -> Scene:
        """
        The active scene of the Application.
        """
        ...

    @scene.setter
    def scene(self, value: Scene):
        ...

    def make_new_ui(self) -> UIManager:
        """
        Creates a new UI subsystem (manager) for the application.
        :return: A new UI Manager.
        """
        ...

    @property
    def ui(self) -> UIManager:
        """
        The active UI of the Application.
        """
        ...

    @ui.setter
    def ui(self, value: UIManager):
        ...


class Scene:
    __native: _NativeScene

    def __init__(self, native_scene: _NativeScene): ...

    def spawn_object(self,
                     pos: Vec2Like,
                     size: Vec2Like,
                     layer: int = 0,
                     has_texture: bool = False,
                     shader_source_path: str = "") -> Object:
        """
        Creates a new object and adds it to the scene.

        :param pos: The initial position of the object in world units.
        :param size: The size of the object in world units.
        :param layer: The layer (z-index) of the object.
        :param has_texture: Whether the object uses a texture.
        :param shader_source_path: The shader to use for this object, if omitted, the engine will use an embedded shader.
        :return: The newly created object.
        """
        ...

    def spawn_text_object(self, pos: Vec2Like, text: str, scale: float = 12) -> TextObject:
        """
        Creates a new text inside the world space and adds it to the scene.

        :param pos: The initial position of the text in world units.
        :param scale: The scale of the text in pt.
        :param text: The text to display.
        :return: The newly created text object.
        """
        ...

    @property
    def camera(self) -> Camera:
        """
        The camera of the scene.
        """
        ...


class TextObject:
    __native: _NativeTextObject
    __color_proxy: ColorProxy

    def __init__(self, native: _NativeTextObject): ...

    @property
    def text(self) -> str:
        """
        The current text displayed.
        This property is write-only.
        """
        ...

    @text.setter
    def text(self, value: str) -> None: ...

    @property
    def scale(self) -> float:
        """
        The current scale of the text in pt.
        This property is write-only.
        """
        ...

    @scale.setter
    def scale(self, value: float) -> None: ...

    @property
    def color(self) -> ColorProxy:
        """
        The current color of the text.
        """
        ...

    @color.setter
    def color(self, v: Color | tuple[Color, float] | tuple[int, int, int, float]) -> None: ...


class Object:
    """
    Represents a renderable 2D object in the engine.
    """

    __native: _NativeObject
    __pos_proxy: Vec2Proxy
    __scale_proxy: Vec2Proxy
    __rot_proxy: RotProxy
    __color_proxy: ColorProxy
    __tint_proxy: TintProxy
    __has_texture: bool
    __color: tuple[int, int, int, float]

    def _get_pos(self): ...

    def _set_pos(self, value): ...

    def _get_scale(self): ...

    def _set_scale(self, value): ...

    def _get_rot(self): ...

    def _set_rot(self, value): ...

    def _get_color(self): ...

    def _set_color(self, value, a: float = 1): ...

    def _get_tint(self): ...

    def _set_tint(self, value, strength: float = 1): ...

    def __ensure_non_textured(self, attr: str): ...

    def __ensure_textured(self, attr: str): ...

    @property
    def pos(self) -> Vec2Proxy:
        """
        The current position of the object.
        """
        ...

    @pos.setter
    def pos(self, v: Vec2Like) -> None: ...

    @property
    def rot(self) -> float:
        """
        The current rotation of the object.
        """
        ...

    @rot.setter
    def rot(self, v: float) -> None: ...

    @property
    def scale(self) -> Vec2Proxy:
        """
        The current scale of the object.
        """
        ...

    @scale.setter
    def scale(self, v: Vec2Like | float) -> None: ...

    @property
    def color(self) -> ColorProxy:
        """
        The current color of the object.
        """
        ...

    @color.setter
    def color(self, v: Color | tuple[Color, float] | tuple[int, int, int, float]) -> None: ...

    @property
    def tint(self) -> TintProxy:
        """
        The current tint of the object.
        """
        ...

    @tint.setter
    def tint(self, v: Color | tuple[Color, float] | tuple[int, int, int, float]) -> None: ...

    def __init__(self, native: _NativeObject, has_texture: bool = False): ...

    def add_collision_box(self, pos: Vec2Like, size: Vec2Like):
        """
        Adds a new collision box to the object.

        :param pos: The position of the collision box relative to the object's origin (center)
        :param size: The size of the collision box.
        """
        ...

    def collides_with(self, other: Object) -> Hit:
        """
        Checks for collision between two objects.
        :param other: The objects to check collision against.
        :return: The result of the collision or a default Hit result if Hit.collision is false.
        """
        ...

    def is_colliding(self) -> list[Hit]:
        """
        Checks for collision on the object.
        :return: The list of collisions or an empty list if the object is not colliding.
        """
        ...

    @property
    def auto_collides(self) -> int:
        """
        Sets the automatic collision resolution order on the current object.
        Once this property is set, it cannot be unset.
        This property is write-only.
        """
        ...

    @auto_collides.setter
    def auto_collides(self, order: int) -> None: ...

    def use_texture(self, path: str, slot: int = 0) -> None:
        """
        Loads and applies a texture to the object.

        :param slot: The slot to bind the texture to.
        :param path: The file path to the texture.
        """
        ...

    def kill(self) -> None:
        """
        Kills the object and removes it from the scene.
        """
        ...

    @property
    def alive(self) -> bool:
        """
        Whether the object is alive.
        """
        ...
