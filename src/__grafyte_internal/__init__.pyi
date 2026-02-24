from enum import IntEnum
from typing_extensions import Buffer, SupportsInt
from typing import overload, Union

class Vec2:
    def __init__(self, x: float = 0.0, y: float = 0.0): ...
    x: float
    y: float

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
    def make_new_scene(self) -> Scene: ...
    def render(self) -> None: ...
    # def _native_add_text(self, text:str, scale: float, pos_x: float, pos_y: float) -> int: ...
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

class Scene:
    def spawn_object(self,
                     positions: Buffer,
                     vertex_count: SupportsInt,
                     indices: Buffer,
                     shader_source_path: str,
                     pos_x: float,
                     pos_y: float,
                     has_texture: bool,
                     z_index: int = 0) -> Object: ...

class Object:
    pos: Vec2
    rot: float
    scale: Vec2

    def use_texture(self, texture_source_path: str, slot: int): ...
    def set_tint(self, tint_r: float, tint_g: float, tint_b: float, strength: float): ...
    def set_color(self, color_r: float, color_g: float, color_b: float, color_a: float): ...
    def move(self, offset_x: float, offset_y: float): ...
    def move_to(self, pos_x: float, pos_y: float): ...
    def rotate(self,angle: float) -> None: ...
    @overload
    def set_scale(self, scale: float) -> None: ...
    @overload
    def set_scale(self, scale: Vec2) -> None: ...
    def set_rotation(self,angle: float) -> None: ...
    def add_collision_box(self, pos_x: float, pos_y: float, size_x: float, size_y: float): ...
    def collides_with(self, other: Object) -> bool: ...
    def is_colliding(self) -> bool: ...
    def enable_auto_collides(self): ...
