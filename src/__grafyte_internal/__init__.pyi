from enum import IntEnum
from typing_extensions import Buffer, SupportsInt

class Key(IntEnum):
    Space = 32
    Num0 = 48
    Num1 = 49
    Num2 = 50
    Num3 = 51
    Num4 = 52
    Num5 = 53
    Num6 = 54
    Num7 = 55
    Num8 = 56
    Num9 = 57
    A = 65
    B = 66
    C = 67
    D = 68
    E = 69
    F = 70
    G = 71
    H = 72
    I = 73
    J = 74
    K = 75
    L = 76
    M = 77
    N = 78
    O = 79
    P = 80
    Q = 81
    R = 82
    S = 83
    T = 84
    U = 85
    V = 86
    W = 87
    X = 88
    Y = 89
    Z = 90
    Right = 262
    Left = 263
    Down = 264
    Up = 265
    LeftShift = 340
    RightShift = 344

class Application:
    def __init__(self, name: str, font: str): ...
    def init(self, win_width: int, win_height: int): ...
    def should_close(self) -> bool: ...
    def quit(self): ...
    def get_now(self) -> float: ...
    def get_delta_time(self) -> float: ...
    def render(self) -> None: ...
    def _native_add_text(self, text:str, scale: float, pos_x: float, pos_y: float) -> int: ...
    def remove_text(self, id: int): ...
    def set_clear_color(self, r: float, g: float, b: float, a: float): ...
    @staticmethod
    def is_key_down(key: Key) -> bool: ...
    @staticmethod
    def was_key_pressed(key: Key) -> bool: ...
    @staticmethod
    def was_key_released(key: Key) -> bool: ...
    @staticmethod
    def create_input_action(name: str, key: Key) -> None: ...
    @staticmethod
    def is_action_down(action: str) -> bool: ...
    @staticmethod
    def was_action_pressed(action: str) -> bool: ...
    @staticmethod
    def was_action_released(action: str) -> bool: ...
    def use_renderer(self, renderer: Renderer): ...

class Renderer:
    def __init__(self): ...
    def add_object(self, obj: Object): ...

class Object:
    def __init__(self,
                 positions: Buffer,
                 vertex_count: SupportsInt,
                 indices: Buffer,
                 shader_source_path: str,
                 pos_x: float,
                 pos_y: float,
                 layer: int = 0): ...
    def add_layout_slot(self, type: str, size: int): ...
    def add_buffer_to_vertex_array(self): ...
    def set_shader_uniform_4f(self, name: str, float_x: float, float_y: float, float_z: float, float_w: float): ...
    def set_shader_uniform_mat_4f(self, name: str, matrix: Buffer): ...
    def _native_move(self, offset_x: float, offset_y: float): ...
    def _native_move_to(self, pos_x: float, pos_y: float): ...
    def set_texture(self, texture_source_path: str, slot: int): ...