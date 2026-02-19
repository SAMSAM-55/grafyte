from array import array

from grafyte.__converters import *

from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Key, Renderer

class Application(_NativeApplication):
    def __init__(self, name: str, window_dimensions: Vec2d):
        ensure_vec2d("Application(window_dimensions=...)", window_dimensions)

        super().__init__(name, "@embed/Fonts/Base")

        super().init(*window_dimensions)

    def set_background_color(self, color: Color):
        super().set_clear_color(color[0] / 255, color[1] / 255, color[2] / 255, 1)
    
    def add_text(self, text, scale, pos: Vec2d) -> int:
        pos = ensure_vec2d("Application.draw_text(pos=...)", pos)
        return super()._native_add_text(text, scale, *pos)


class Object(_NativeObject):
    def __init__(self, pos: Vec2d, size: Vec2d, layer: int = 0, has_texture: bool = False,
                 shader_source_path: str = ""):
        self.__has_texture = has_texture

        pos = ensure_vec2d("Object(pos=...)", pos)
        size = ensure_vec2d("Object(size=...)", size)

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

    def set_color(self, color: Color, a: float = 1):
        if self.__has_texture:
            raise AttributeError("This object has texture, use set_tint instead of set_color")
        r, g, b = ensure_color("Object.set_color(color=...)", color)

        super().set_shader_uniform_4f("u_Color", r / 255, g / 255, b / 255, a)

    def move(self, offset: Vec2d):
        super()._native_move(*offset)

    def move_to(self, pos: Vec2d):
        super()._native_move_to(*pos)

    def use_texture(self, path: str):
        if not self.__has_texture:
            raise AttributeError("This object was not initialized with has_texture=True")

        super().set_texture(path, 0)

    def set_tint(self, tint: Color, strength: float = 1):
        if not self.__has_texture:
            raise AttributeError("This object has no texture, use set_color instead of set_tint")
        r, g, b = ensure_color("Object.set_tint(tint=...)", tint)

        super().set_shader_uniform_4f("u_Tint",
                                      r / 255,
                                      g / 255,
                                      b / 255,
                                      strength)
