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

class Direction(IntEnum):
    Top = 0
    Bottom = 1
    Right = 2
    Left = 3

class AABB:
    pos: Vec2
    width: float
    height: float

class Hit:
    A: AABB
    B: AABB
    collision: bool
    direction: Direction
    def __bool__(self) -> bool: ...

class InputTrigger(IntEnum):
    Press = 0
    Hold = 1
    Release = 2

class Camera:
    def move(self, offset_x: float, offset_y: float) -> None: ...
    def move_to(self, pos_x: float, pos_y: float) -> None: ...
    def zoom(self, zoom: float) -> None: ...
    def follow(self, object: Object) -> None: ...
    def follow_offset(self, offset_x: float, offset_y: float) -> None: ...

class InputManager:
    def __init__(self): ...

    @staticmethod
    def is_key_down(key: Key) -> bool: ...
    @staticmethod
    def was_key_pressed(key: Key) -> bool: ...
    @staticmethod
    def was_key_released(key: Key) -> bool: ...
    @staticmethod
    def create_action(name: str, keys: list[Key], trigger: InputTrigger) -> None: ...
    @staticmethod
    def is_action_active(action: str) -> bool: ...

class Application:
    input: InputManager

    def __init__(self, name: str, font: str): ...
    def init(self, win_width: int, win_height: int): ...
    def should_close(self) -> bool: ...
    def quit(self): ...
    def get_now(self) -> float: ...
    def get_delta_time(self) -> float: ...
    def make_new_scene(self) -> Scene: ...
    def render(self) -> None: ...
    def _native_add_text(self, text:str, scale: float, pos_x: float, pos_y: float) -> int: ...
    def set_text(self, id: int, text: str): ...
    def remove_text(self, id: int): ...
    def set_clear_color(self, r: float, g: float, b: float, a: float): ...

class TextObject:
    def set_text(self, text: str): ...
    def set_scale(self, scale: float): ...
    def set_color(self, color_r: float, color_g: float, color_b: float, color_a: float): ...

class Scene:
    def spawn_object(self,
                     sx: float,
                     sy: float,
                     shader_source_path: str,
                     pos_x: float,
                     pos_y: float,
                     has_texture: bool,
                     z_index: int = 0) -> Object: ...

    def spawn_text_object(self, text: str, scale: float, pos_x: float, pos_y: float) -> TextObject: ...
    def get_camera(self) -> Camera: ...

class Object:
    pos: Vec2
    rot: float
    scale: Vec2

    def use_texture(self, texture_source_path: str, slot: int): ...
    def set_color(self, color_r: float, color_g: float, color_b: float, color_a: float): ...
    def move(self, offset_x: float, offset_y: float): ...
    def move_to(self, pos_x: float, pos_y: float): ...
    def rotate(self,angle: float) -> None: ...
    @overload
    def set_scale(self, scale: float) -> None: ...
    @overload
    def set_scale(self, scale_x: float, scale_y: float) -> None: ...
    def set_rotation(self,angle: float) -> None: ...
    def add_collision_box(self, pos_x: float, pos_y: float, size_x: float, size_y: float): ...
    def collides_with(self, other: Object) -> Hit: ...
    def is_colliding(self) -> list[Hit]: ...
    def enable_auto_collides(self, order: int): ...
