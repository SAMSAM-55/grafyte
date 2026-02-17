from array import array
from typing import TypeAlias

from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Renderer as _NativeRenderer

_vec2d: TypeAlias = tuple[float, float]
_color: TypeAlias = tuple[int, int, int]


def _ensure_vec2d(name: str, v: _vec2d) -> _vec2d:
    if (
            not isinstance(v, tuple) or len(v) != 2
            or not all(isinstance(x, (int, float)) for x in v)
    ):
        raise TypeError(f"{name} must be vec2d = tuple[float, float], got {v!r}")
    return float(v[0]), float(v[1])


class Application(_NativeApplication):
    def __init__(self, name: str, window_dimensions: _vec2d):
        _ensure_vec2d("Application(window_dimensions=...)", window_dimensions)

        super().__init__(name)
        super().init(*window_dimensions)

    def set_background_color(self, r: int, g: int, b: int):
        super().set_clear_color(r / 255, g / 255, b / 255, 1)

    def is_key_down(self, key: str) -> bool:
        return super().is_key_down(key.upper())


class Renderer(_NativeRenderer): ...


class Object(_NativeObject):
    def __init__(self, pos: _vec2d, size: _vec2d, layer: int = 0, has_texture: bool = False,
                 shader_source_path: str = ""):
        self.__has_texture = has_texture

        pos = _ensure_vec2d("Object(pos=...)", pos)
        size = _ensure_vec2d("Object(size=...)", size)

        positions = array("f", [
            -size[0], -size[1], 0, 0,
            size[0], -size[1], 1, 0,
            size[0], size[1], 1, 1,
            -size[0], size[1], 0, 1
        ]) if has_texture else array("f", [
            -size[0], -size[1],
            size[0], -size[1],
            size[0], size[1],
            -size[0], size[1],
        ])

        indices = array("I", [
            0, 1, 2,
            2, 3, 0
        ])

        if shader_source_path == "":
            if has_texture:
                shader_source = "@embed/Shaders/Texture"
            else:
                shader_source = "@embed/Shaders/Basic"
        else:
            shader_source = shader_source_path

        super().__init__(positions, 4, indices, shader_source, pos[0], pos[1], layer)
        super().add_layout_slot("float", 2)
        if has_texture: super().add_layout_slot("float", 2)
        super().add_buffer_to_vertex_array()

    def set_color(self, color: _color):
        # if self.__has_texture:
        #     raise AttributeError("This object has texture, use set_tint instead of set_color")

        super().set_shader_uniform_4f("u_Color", color[0] / 255, color[1] / 255, color[2] / 255, 1)

    def move(self, offset: _vec2d):
        super()._native_move(offset[0], offset[1])

    def move_to(self, pos: _vec2d):
        super()._native_move_to(pos[0], pos[1])

    def use_texture(self, path: str):
        if not self.__has_texture:
            raise AttributeError("This object was not initialized with has_texture=True")

        super().set_texture(path, 0)

    def set_tint(self, tint: _color, strength: float = 1):
        if not self.__has_texture:
            raise AttributeError("This object has no texture, use set_color instead of set_tint")

        super().set_shader_uniform_4f("u_Tint",
                                      tint[0] / 255,
                                      tint[1] / 255,
                                      tint[2] / 255,
                                      strength)
