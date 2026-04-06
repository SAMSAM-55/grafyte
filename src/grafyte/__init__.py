from __future__ import annotations

from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import InputManager as _NativeInputManager
from __grafyte_internal import Key, InputTrigger, Direction
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Scene as _NativeScene
from __grafyte_internal import TextObject as _NativeTextObject

from .__class_utils import _KeyAccessor, _KeyPressedAccessor, _KeyReleasedAccessor, Vec2Proxy, RotProxy
from .__converters import *


class Object:
    def __init__(self, native_object: _NativeObject):
        self.__native = native_object
        self.__pos_proxy = Vec2Proxy(
            getter=self._get_pos,
            setter=self._set_pos,
        )
        self.__scale_proxy = Vec2Proxy(
            getter=self._get_scale,
            setter=self._set_scale
        )
        self.__rot_proxy = RotProxy(
            getter=self._get_rot,
            setter=self._set_rot,
        )

    def _get_pos(self):
        return self.__native.pos

    def _set_pos(self, value: Vec2Like):
        n_value = ensure_vec2f("Object.__set_pos(value=...)", value)
        self.__native.move_to(*n_value)

    def _get_scale(self):
        return self.__native.scale

    def _set_scale(self, value: Vec2Like):
        n_value = ensure_vec2f("Object.__set_scale(value=...)", value)
        self.__native.set_scale(*n_value)

    def _get_rot(self):
        return self.__native.rot

    def _set_rot(self, value: float):
        self.__native.set_rotation(value)

    @property
    def pos(self):
        return self.__pos_proxy

    @pos.setter
    def pos(self, v: Vec2Like):
        if v is self.__pos_proxy: return
        self._set_pos(v)

    @property
    def rot(self):
        return self.__rot_proxy.value

    @rot.setter
    def rot(self, v: float):
        if v is self.__rot_proxy: return
        self._set_rot(v)

    @property
    def scale(self):
        return self.__native.scale

    @scale.setter
    def scale(self, v: Vec2Like):
        if v is self.__scale_proxy: return
        self._set_scale(v)

    def add_collision_box(self, pos: Vec2Like, size: Vec2Like):
        n_pos = ensure_vec2f("Object.add_collision_box(pos=...", pos)
        n_size = ensure_vec2f("Object.add_collision_box(size=...", size)
        self.__native.add_collision_box(*n_pos, *n_size)

    def collides_with(self, other: Object) -> bool:
        return self.__native.collides_with(other.__native)

    def is_colliding(self) -> bool:
        return self.__native.is_colliding()

    def enable_auto_collides(self, order: int):
        self.__native.enable_auto_collides(order)

    def use_texture(self, texture_source_path: str, slot: int):
        self.__native.use_texture(texture_source_path, slot)

    def set_tint(self, tint: Color, strength: float = 1):
        n_tint = ensure_color_normalize("Object.set_tint(tint=...", tint)
        self.__native.set_tint(*n_tint, strength)
    def set_color(self, color: Color, a: float = 1):
        n_color = ensure_color_normalize("Object.set_color(color=...", color)
        self.__native.set_color(*n_color, a)

class TextObject:
    def __init__(self, native_object: _NativeTextObject):
        self.__native = native_object
    def set_text(self, text: str):
        self.__native.set_text(text)

    def set_scale(self, scale: float):
        self.__native.set_scale(scale)

    def set_color(self, color: Color, a: float = 1):
        n_color = ensure_color_normalize("TextObject.set_color(color=...)", color)
        self.__native.set_color(*n_color, a)

class Scene:
    def __init__(self, native_scene: _NativeScene):
        self.__native = native_scene

    def spawn_object(self,
                     pos: Vec2Like,
                     size: Vec2Like,
                     layer: int = 0,
                     has_texture: bool = False,
                     shader_source_path: str = "") -> Object:
        n_pos = ensure_vec2f("Object(pos=...)", pos)
        n_size = ensure_vec2f("Object(size=...)", size)

        if shader_source_path == "":
            if has_texture:
                shader_source = "@embed/Shaders/Texture"
            else:
                shader_source = "@embed/Shaders/Basic"
        else:
            shader_source = shader_source_path

        native_obj = self.__native.spawn_object(*n_size, shader_source, *n_pos, has_texture, layer)
        return Object(native_obj)

    def spawn_text_object(self, pos: Vec2Like, text: str, scale: float = 12) -> TextObject:
        n_pos = ensure_vec2f("Scene.spawn_text_object(pos=...)", pos)

        native_obj = self.__native.spawn_text_object(*n_pos, text, scale)
        return TextObject(native_obj)

class InputManager(_NativeInputManager):
    def __init__(self):
        super().__init__()

        self.key = _KeyAccessor()
        self.key_pressed = _KeyPressedAccessor()
        self.key_released = _KeyReleasedAccessor()

    @staticmethod
    def create_action(name: str, trigger: InputTrigger, *key: Key) -> None:
        _NativeInputManager.create_action(name, [*key], trigger)

    def __getitem__(self, action: str) -> bool:
        return _NativeInputManager.is_action_active(action)

class Application(_NativeApplication):
    @property
    def input(self) -> InputManager:
        return self.__input

    def make_new_scene(self) -> Scene:
        native_scene = super().make_new_scene()
        return Scene(native_scene)

    def __init__(self, name: str, window_dimensions: Vec2Like):
        super().__init__(name, "@embed/Fonts/Base")
        n_window_dimensions = ensure_vec2i("Application(window_dimensions=...)", window_dimensions)
        self.__input = InputManager()
        super().init(*n_window_dimensions)

    def set_background_color(self, color: Color):
        r, g, b = ensure_color("Application.set_background_color(color=...)", color)
        super().set_clear_color(r / 255, g / 255, b / 255, 1)
