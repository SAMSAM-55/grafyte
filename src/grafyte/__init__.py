from array import array
from typing_extensions import Buffer, SupportsInt

from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Renderer as _NativeRenderer


class Application(_NativeApplication):
    def set_background_color(self, r: int, g: int, b: int):
        super().set_clear_color(r/255, g/255, b/255, 1)

class Renderer(_NativeRenderer): ...

class Object(_NativeObject):
    def __init__(self, size: tuple[float, float], pos: tuple[float, float], shader_source_path: str):
        positions = array("f",[
            -size[0] + pos[0], -size[1] + pos[1],
            size[0]  + pos[0], -size[1] + pos[1],
            size[0]  + pos[0], size[1]  + pos[1],
            -size[0] + pos[0], size[1]  + pos[1]
        ])

        indices = array("I", [
            0, 1, 2,
            2, 3, 0
        ])

        super().__init__(positions, 4, indices, shader_source_path)
        super().add_layout_slot("float", 2)
        super().add_buffer_to_vertex_array()

    def set_color(self, r: int, g: int, b:int):
        super().set_shader_uniform_4f("u_Color", r/255, g/255, b/255, 1)
