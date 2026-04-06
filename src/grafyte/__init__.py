from __future__ import annotations

from array import array
from typing import Union

from .__converters import *

from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Scene as _NativeScene
from __grafyte_internal import TextObject as _NativeTextObject
from __grafyte_internal import InputManager as _NativeInputManager
from __grafyte_internal import Key, InputTrigger, Vec2, Direction, Hit

class Vec2Proxy:
    def __init__(self, getter, setter):
        self._getter = getter
        self._setter = setter

    def _get(self):
        return self._getter()

    def _set(self, v):
        self._setter(v)

    @property
    def x(self):
        return self._get().x

    @x.setter
    def x(self, value):
        v = self._get()
        self._set((value, v.y))

    @property
    def y(self):
        return self._get().y

    @y.setter
    def y(self, value):
        v = self._get()
        self._set((v.x, value))

    def __iadd__(self, other: Vec2Like):
        ox, oy = ensure_vec2f("Vec2Proxy.__iadd__(other=...)", other)
        v = self._get()
        self._set((v.x + ox, v.y + oy))
        return self

    def __isub__(self, other: Vec2Like):
        ox, oy = ensure_vec2f("Vec2Proxy.__iadd__(other=...)", other)
        v = self._get()
        self._set((v.x - ox, v.y - oy))
        return self
    def __imul__(self, other: float):
        v = self._get()
        self._set((v.x * other, v.y * other))
        return self

    def __iter__(self):
        v = self._get()
        yield v.x
        yield v.y

    def __repr__(self):
        v = self._get()
        return f"Vec2Proxy({v.x}, {v.y})"

class RotProxy:
    def __init__(self, getter, setter):
        self._getter = getter
        self._setter = setter

    def _get(self):
        return float(self._getter())

    def _set(self, value):
        self._setter(float(value))

    @property
    def value(self):
        return self._get()

    @value.setter
    def value(self, value):
        self._set(value)

    def set(self, value):
        self._set(value)
        return self

    def __float__(self):
        return self._get()

    def __int__(self):
        return int(self._get())

    def __iadd__(self, other):
        self._set(self._get() + float(other))
        return self

    def __isub__(self, other):
        self._set(self._get() - float(other))
        return self

    def __imul__(self, other):
        self._set(self._get() * float(other))
        return self

    def __itruediv__(self, other):
        self._set(self._get() / float(other))
        return self

    def __repr__(self):
        return f"RotProxy({self._get()})"

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
        return self.__rot_proxy

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
    @staticmethod
    def is_key_down(key: Key) -> bool: return _NativeInputManager.is_key_down(key);
    @staticmethod
    def was_key_pressed(key: Key) -> bool: return _NativeInputManager.was_key_pressed(key)
    @staticmethod
    def was_key_released(key: Key) -> bool: return _NativeInputManager.was_key_released(key)
    @staticmethod
    def create_action(name: str, trigger: InputTrigger, *key: Key) -> None:
        _NativeInputManager.create_action(name, [*key], trigger)
    @staticmethod
    def is_action_active(action: str) -> bool: return _NativeInputManager.is_action_active(action)

class Application(_NativeApplication):
    @property
    def input(self) -> type[InputManager]:
        return InputManager

    def make_new_scene(self) -> Scene:
        native_scene = super().make_new_scene()
        return Scene(native_scene)

    def __init__(self, name: str, window_dimensions: Vec2Like):
        super().__init__(name, "@embed/Fonts/Base")
        n_window_dimensions = ensure_vec2i("Application(window_dimensions=...)", window_dimensions)
        super().init(*n_window_dimensions)

    def set_background_color(self, color: Color):
        r, g, b = ensure_color("Application.set_background_color(color=...)", color)
        super().set_clear_color(r / 255, g / 255, b / 255, 1)
