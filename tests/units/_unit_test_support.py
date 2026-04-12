from __future__ import annotations

import importlib
import inspect
import sys
import types
from enum import Enum, auto


class Vec2:
    def __init__(self, x: float = 0.0, y: float = 0.0):
        self.x = x
        self.y = y


class Key(Enum):
    A = auto()
    D = auto()
    F = auto()
    G = auto()
    J = auto()
    Q = auto()
    S = auto()
    Space = auto()
    Z = auto()
    LeftShift = auto()


class InputTrigger(Enum):
    Hold = auto()
    Press = auto()
    Release = auto()


class Direction(Enum):
    Bottom = auto()
    Top = auto()
    Left = auto()
    Right = auto()


class Anchor(Enum):
    TopLeft = auto()
    TopRight = auto()
    BottomLeft = auto()
    BottomRight = auto()
    Center = auto()


class Hit:
    def __init__(self, collision: bool = False, direction: Direction | None = None):
        self.collision = collision
        self.direction = direction


class Object:
    def __init__(self):
        self.pos = Vec2()
        self.scale = Vec2(1.0, 1.0)
        self.rot = 0.0
        self.alive = True
        self.last_color = None
        self.last_texture = None
        self.auto_collides_order = None
        self.collision_boxes = []

    def move_to(self, x: float, y: float):
        self.pos = Vec2(x, y)

    def set_scale(self, x: float, y: float):
        self.scale = Vec2(x, y)

    def set_rotation(self, value: float):
        self.rot = float(value)

    def set_color(self, r: float, g: float, b: float, a: float):
        self.last_color = (r, g, b, a)

    def add_collision_box(self, x: float, y: float, w: float, h: float):
        self.collision_boxes.append((x, y, w, h))

    def collides_with(self, other):
        return Hit(False, None)

    def is_colliding(self):
        return []

    def enable_auto_collides(self, order: int):
        self.auto_collides_order = order

    def use_texture(self, texture_source_path: str, slot: int):
        self.last_texture = (texture_source_path, slot)

    def kill(self):
        self.alive = False


class TextObject:
    def __init__(self):
        self.text_value = ""
        self.scale_value = 0.0
        self.last_color = None

    def set_text(self, value: str):
        self.text_value = value

    def set_scale(self, value: float):
        self.scale_value = float(value)

    def set_color(self, r: float, g: float, b: float, a: float):
        self.last_color = (r, g, b, a)


class Text(TextObject):
    pass


class Camera:
    def __init__(self):
        self.followed = None
        self.follow_offset_value = None
        self.zoom_value = None
        self.last_move_to = None

    def move_to(self, x: float, y: float):
        self.last_move_to = (x, y)

    def follow(self, native_object: Object):
        self.followed = native_object

    def follow_offset(self, x: float, y: float):
        self.follow_offset_value = (x, y)

    def zoom(self, value: float):
        self.zoom_value = float(value)


class Scene:
    def __init__(self):
        self.camera = Camera()
        self.spawn_calls = []
        self.text_calls = []

    def spawn_object(self, width, height, shader_source, x, y, has_texture, layer):
        native = Object()
        native.move_to(x, y)
        native.set_scale(width, height)
        self.spawn_calls.append((width, height, shader_source, x, y, has_texture, layer, native))
        return native

    def spawn_text_object(self, x, y, text, scale):
        native = TextObject()
        native.move_to = (x, y)
        native.text_value = text
        native.scale_value = scale
        self.text_calls.append((x, y, text, scale, native))
        return native

    def get_camera(self):
        return self.camera


class UIManager:
    def __init__(self):
        self.texts = []
        self.removed = []

    def add_text(self, x, y, text, scale, anchor):
        native = Text()
        native.move_to = (x, y)
        native.text_value = text
        native.scale_value = scale
        native.anchor = anchor
        self.texts.append(native)
        return native

    def remove_text(self, native_text):
        self.removed.append(native_text)


class InputManager:
    actions = {}
    active_actions = set()
    down_keys = set()
    pressed_keys = set()
    released_keys = set()

    def __init__(self):
        pass

    @staticmethod
    def create_action(name, trigger, keys):
        InputManager.actions[name] = (trigger, list(keys))

    @staticmethod
    def is_action_active(name):
        return name in InputManager.active_actions

    @staticmethod
    def is_key_down(key):
        return key in InputManager.down_keys

    @staticmethod
    def was_key_pressed(key):
        return key in InputManager.pressed_keys

    @staticmethod
    def was_key_released(key):
        return key in InputManager.released_keys


class Application:
    instances = []

    def __init__(self, name: str, font_path: str):
        self.name = name
        self.font_path = font_path
        self.init_args = None
        self.clear_color = None
        self.now_value = 0.0
        self.dt_value = 0.0
        self.scene = Scene()
        self.ui = UIManager()
        Application.instances.append(self)

    def init(self, width: int, height: int):
        self.init_args = (width, height)

    def make_new_scene(self):
        return self.scene

    def make_new_ui(self):
        return self.ui

    def get_now(self):
        return self.now_value

    def get_delta_time(self):
        return self.dt_value

    def set_clear_color(self, r: float, g: float, b: float, a: float):
        self.clear_color = (r, g, b, a)


def _make_internal_module() -> types.ModuleType:
    module = types.ModuleType("__grafyte_internal")
    module.Application = Application
    module.InputManager = InputManager
    module.Key = Key
    module.InputTrigger = InputTrigger
    module.Direction = Direction
    module.Anchor = Anchor
    module.Object = Object
    module.Scene = Scene
    module.TextObject = TextObject
    module.Camera = Camera
    module.UIManager = UIManager
    module.Text = Text
    module.Hit = Hit
    module.Vec2 = Vec2
    return module


def load_grafyte():
    sys.modules["__grafyte_internal"] = _make_internal_module()
    for name in list(sys.modules):
        if name == "grafyte" or name.startswith("grafyte."):
            del sys.modules[name]

    return importlib.import_module("grafyte")


def make_caller(filename: str):
    return inspect.FrameInfo(frame=None, filename=filename, lineno=1, function="test", code_context=None, index=None)
