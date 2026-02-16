from array import array

from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Renderer as _NativeRenderer

vec2d = tuple[float, float]

class Application(_NativeApplication):
    def set_background_color(self, r: int, g: int, b: int):
        super().set_clear_color(r/255, g/255, b/255, 1)

class Renderer(_NativeRenderer): ...

class Object(_NativeObject):
    def __init__(self, size: vec2d, pos: vec2d, shader_source_path: str, layer: int = 0, has_texture: bool = False):
        positions = array("f",[
            -size[0], -size[1], 0, 0,
            size[0] , -size[1], 1, 0,
            size[0] , size[1],  1, 1,
            -size[0], size[1],  0, 1
        ]) if has_texture else array("f",[
            -size[0], -size[1],
            size[0] , -size[1],
            size[0] , size[1],
            -size[0], size[1],
        ])

        print("Pos: ", pos)

        indices = array("I", [
            0, 1, 2,
            2, 3, 0
        ])

        super().__init__(positions, 4, indices, shader_source_path, pos[0], pos[1], layer)
        super().add_layout_slot("float", 2)
        if has_texture: super().add_layout_slot("float", 2)
        super().add_buffer_to_vertex_array()

    def set_color(self, r: int, g: int, b:int):
        super().set_shader_uniform_4f("u_Color", r/255, g/255, b/255, 1)

    def move(self, offset: vec2d):
        super()._native_move(offset[0], offset[1])

    def move_to(self, pos: vec2d):
        super()._native_move_to(pos[0], pos[1])

    def use_texture(self, path: str):
        super().set_texture(path, 0)
