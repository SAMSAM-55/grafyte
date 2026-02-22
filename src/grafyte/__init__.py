from array import array

from .__converters import *

from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Scene as _NativeScene
from __grafyte_internal import Key


class Object:
    def __init__(self, native_object: _NativeObject):
        self.__native = native_object

    def move(self, offset: Vec2d):
        self.__native.move(*offset)

    def move_to(self, pos: Vec2d):
        self.__native.move_to(*pos)

    def use_texture(self, texture_source_path: str, slot: int):
        self.__native.use_texture(texture_source_path, slot)

    def set_tint(self, tint: Color, strength: float = 1):
        tint = ensure_color_normalize("Object.set_tint(tint=...", tint)
        self.__native.set_tint(*tint, strength)
    def set_color(self, color: Color, a: float = 1):
        color = ensure_color_normalize("Object.set_color(color=...", color)
        self.__native.set_color(*color, a)

class Scene:
    def __init__(self, native_scene: _NativeScene):
        self.__native = native_scene

    def spawn_object(self,
                     pos: Vec2d,
                     size: Vec2d,
                     layer: int = 0,
                     has_texture: bool = False,
                     shader_source_path: str = "") -> Object:
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

        native_obj = self.__native.spawn_object(positions, 4, indices, shader_source, *pos, has_texture, layer)
        return Object(native_obj)

class Application(_NativeApplication):
    def make_new_scene(self) -> Scene:
        native_scene = super().make_new_scene()
        return Scene(native_scene)

    def __init__(self, name: str, window_dimensions: Vec2d):
        ensure_vec2d("Application(window_dimensions=...)", window_dimensions)

        super().__init__(name, "@embed/Fonts/Base")

        super().init(*window_dimensions)

    def set_background_color(self, color: Color):
        r, g, b = ensure_color("Application.set_background_color(color=...)", color)
        super().set_clear_color(r / 255, g / 255, b / 255, 1)

    # def add_text(self, text, scale, pos: Vec2d) -> int:
    #     pos = ensure_vec2d("Application.draw_text(pos=...)", pos)
    #     return super()._native_add_text(text, scale, *pos)