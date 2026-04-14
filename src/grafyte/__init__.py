from __future__ import annotations

import inspect
from pathlib import Path

from __grafyte_internal import Application as _NativeApplication
from __grafyte_internal import InputManager as _NativeInputManager
from __grafyte_internal import Key, InputTrigger, Direction, Anchor
from __grafyte_internal import Object as _NativeObject
from __grafyte_internal import Scene as _NativeScene
from __grafyte_internal import TextObject as _NativeTextObject
from __grafyte_internal import Camera as _NativeCamera
from __grafyte_internal import UIManager as _NativeUIManager
from __grafyte_internal import Text as _NativeText
from __grafyte_internal import Hit as _NativeHit

from .__class_utils import _KeyAccessor, _KeyPressedAccessor, _KeyReleasedAccessor, Vec2Proxy, ColorProxy, TintProxy, \
    RotProxy, AssetResolver
from .__converters import *


class Hit(_NativeHit):
    def __init__(self):
        super().__init__()


class Object:
    def __init__(self, native_object: _NativeObject, has_texture: bool = False):
        self.__native = native_object
        self.__has_texture = has_texture
        self.__color = (255, 255, 255, 1.0)
        self.__tint = (255, 255, 255, 1.0)
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
        self.__color_proxy = ColorProxy(
            getter=self._get_color,
            setter=self._set_color,
            checker=self.__ensure_non_textured,
            name="color",
        )
        self.__tint_proxy = TintProxy(
            getter=self._get_tint,
            setter=self._set_tint,
            checker=self.__ensure_textured,
            name="tint",
        )

    def _get_native(self) -> _NativeObject:
        return self.__native

    def _get_pos(self):
        if not self.alive: return Vec2(0, 0)

        return self.__native.pos

    def _set_pos(self, value: Vec2Like):
        if not self.alive: return

        n_value = ensure_vec2f("Object.__set_pos(value=...)", value)
        self.__native.move_to(*n_value)

    def _get_scale(self):
        if not self.alive: return 0

        return self.__native.scale

    def _set_scale(self, value: Vec2Like):
        if not self.alive: return

        n_value = ensure_vec2f("Object.__set_scale(value=...)", value)
        self.__native.set_scale(*n_value)

    def _get_rot(self):
        if not self.alive: return 0

        return self.__native.rot

    def _set_rot(self, value: float):
        if not self.alive: return

        self.__native.set_rotation(value)

    def _get_color(self):
        if not self.alive: return 0, 0, 0, 0

        return self.__color

    def _set_color(self, value: Color | tuple[int, int, int, float], a: float = 1):
        if not self.alive: return

        self.__ensure_non_textured("color")
        if isinstance(value, tuple) and len(value) == 2 and isinstance(value[0], (tuple, list)):
            color, a = value[0], float(value[1])
        elif isinstance(value, tuple) and len(value) == 4:
            color, a = value[:3], float(value[3])
        else:
            color = value

        n_color = ensure_color_normalize("Object.set_color(color=...)", color)
        self.__native.set_color(*n_color, a)
        self.__color = (*n_color, float(a))

    def _get_tint(self):
        if not self.alive: return 0, 0, 0, 0

        return self.__tint

    def _set_tint(self, value: Color | tuple[int, int, int, float], strength: float = 0):
        if not self.alive: return

        self.__ensure_textured("tint")
        if isinstance(value, tuple) and len(value) == 2 and isinstance(value[0], (tuple, list)):
            tint, strength = value[0], float(value[1])
        elif isinstance(value, tuple) and len(value) == 4:
            tint, strength = value[:3], float(value[3])
        else:
            tint = value

        n_tint = ensure_color_normalize("Object.set_tint(tint=...)", tint)
        self.__native.set_color(*n_tint, strength)
        self.__tint = (*ensure_color("Object.set_tint(tint=...)", tint), float(strength))

    def __ensure_non_textured(self, attr: str):
        if not self.alive: return

        if self.__has_texture:
            raise RuntimeError(f"Object.{attr} is only available on non-textured objects; use tint instead.")

    def __ensure_textured(self, attr: str):
        if not self.alive: return

        if not self.__has_texture:
            raise RuntimeError(f"Object.{attr} is only available on textured objects; use color instead.")

    @property
    def pos(self):
        return self.__pos_proxy

    @pos.setter
    def pos(self, v: Vec2Like):
        if v is self.__pos_proxy: return
        self._set_pos(v)

    @property
    def rot(self):
        return self.__rot_proxy.value

    @rot.setter
    def rot(self, v: float):
        if v is self.__rot_proxy: return
        self._set_rot(v)

    @property
    def scale(self):
        return self.__scale_proxy

    @scale.setter
    def scale(self, v: Vec2Like | Vec1Like):
        if v is self.__scale_proxy: return
        if isinstance(v, Vec1Like):
            self._set_scale((float(v), float(v)))
            return
        self._set_scale(v)

    @property
    def color(self):
        return self.__color_proxy

    @color.setter
    def color(self, v: Color | tuple[int, int, int, float]):
        if v is self.__color_proxy: return
        self._set_color(v)

    @property
    def tint(self):
        return self.__tint_proxy

    @tint.setter
    def tint(self, v: Color | tuple[int, int, int, float]):
        if v is self.__tint_proxy: return
        self._set_tint(v)

    def add_collision_box(self, pos: Vec2Like, size: Vec2Like):
        if not self.alive: return

        n_pos = ensure_vec2f("Object.add_collision_box(pos=...", pos)
        n_size = ensure_vec2f("Object.add_collision_box(size=...", size)
        self.__native.add_collision_box(*n_pos, *n_size)

    def collides_with(self, other: Object):
        if not self.alive: return Hit()

        return self.__native.collides_with(other.__native)

    def is_colliding(self):
        if not self.alive: return []

        return self.__native.is_colliding()

    @property
    def auto_collides(self):
        raise AttributeError("Object.auto_collides is write-only")

    @auto_collides.setter
    def auto_collides(self, order: int):
        if not self.alive: return
        self.__native.enable_auto_collides(order)

    def use_texture(self, texture_source_path: str, slot: int):
        if not self.alive: return
        self.__has_texture = True
        self.__native.use_texture(texture_source_path, slot)

    def kill(self):
        self.__native.kill()

    @property
    def alive(self):
        return self.__native.alive


class TextObject:
    def __init__(self, native_object: _NativeTextObject):
        self.__native = native_object
        self.__color = (255, 255, 255, 1.0)
        self.__color_proxy = ColorProxy(
            getter=self._get_color,
            setter=self._set_color,
        )

    def _get_color(self):
        r = int(self.__native.color[0] * 255)
        g = int(self.__native.color[1] * 255)
        b = int(self.__native.color[2] * 255)
        return r, g, b, self.__native.color[3]

    def _set_color(self, value: Color | tuple[int, int, int, float], a: float = 1):
        if isinstance(value, tuple) and len(value) == 2 and isinstance(value[0], (tuple, list)):
            color, a = value[0], float(value[1])
        elif isinstance(value, tuple) and len(value) == 4:
            color, a = value[:3], float(value[3])
        else:
            color = value

        n_color = ensure_color_normalize("TextObject.set_color(color=...)", color)
        self.__native.set_color(*n_color, a)
        self.__color = (*n_color, float(a))

    @property
    def text(self):
        return self.__native.text

    @text.setter
    def text(self, value: str):
        self.__native.set_text(value)

    @property
    def scale(self):
        return self.__native.scale

    @scale.setter
    def scale(self, value: float):
        self.__native.set_scale(value)

    @property
    def color(self):
        return self.__color_proxy

    @color.setter
    def color(self, v: Color | tuple[int, int, int, float]):
        if v is self.__color_proxy: return
        self._set_color(v)


class Text:
    def __init__(self, native_text: _NativeText):
        self.__native = native_text
        self.__color = (255, 255, 255, 1.0)
        self.__color_proxy = ColorProxy(
            getter=self._get_color,
            setter=self._set_color,
        )

    def _get_color(self):
        r = int(self.__native.color[0] * 255)
        g = int(self.__native.color[1] * 255)
        b = int(self.__native.color[2] * 255)
        return r, g, b, self.__native.color[3]

    def _set_color(self, value: Color | tuple[int, int, int, float], a: float = 1):
        if isinstance(value, tuple) and len(value) == 2 and isinstance(value[0], (tuple, list)):
            color, a = value[0], float(value[1])
        elif isinstance(value, tuple) and len(value) == 4:
            color, a = value[:3], float(value[3])
        else:
            color = value

        n_color = ensure_color_normalize("Text.set_color(color=...)", color)
        self.__native.set_color(*n_color, a)
        self.__color = (*n_color, float(a))

    @property
    def text(self):
        return self.__native.text

    @text.setter
    def text(self, value: str):
        self.__native.set_text(value)

    @property
    def scale(self):
        return self.__native.scale

    @scale.setter
    def scale(self, value: float):
        self.__native.set_scale(value)

    @property
    def color(self):
        return self.__color_proxy

    @color.setter
    def color(self, v: Color | tuple[int, int, int, float]):
        if v is self.__color_proxy: return
        self._set_color(v)

    def _get_native(self) -> _NativeText:
        return self.__native


class UIManager:
    def __init__(self, native_ui_manager: _NativeUIManager):
        self.__native = native_ui_manager

    def add_text(self,
                 pos: Vec2Like,
                 text: str,
                 scale: float = 12,
                 anchor: Anchor = Anchor.TopLeft) -> Text:
        n_pos = ensure_vec2f("UIManager.add_text(pos=...)", pos)
        return Text(self.__native.add_text(*n_pos, text, scale, anchor))

    def remove_text(self, text: Text) -> None:
        self.__native.remove_text(text._get_native())


class Camera:
    def __init__(self, native_camera: _NativeCamera):
        self.__native = native_camera
        self.__pos_proxy = Vec2Proxy(
            getter=self._get_pos,
            setter=self._set_pos
        )
        self.__follow_offset_proxy = Vec2Proxy(
            getter=self._get_follow_offset,
            setter=self._set_follow_offset
        )

    def _get_pos(self) -> Vec2:
        return self.__native.pos_v

    def _set_pos(self, value: Vec2Like):
        n_value = ensure_vec2f("Object.__set_pos(value=...)", value)
        self.__native.move_to(*n_value)

    def _get_follow_offset(self) -> Vec2:
        return self.__native.follow_offset_v

    def _set_follow_offset(self, value: Vec2Like):
        n_value = ensure_vec2f("Object.__set_pos(value=...)", value)
        self.__native.follow_offset(*n_value)

    @property
    def pos(self):
        return self.__pos_proxy

    @pos.setter
    def pos(self, v: Vec2Like):
        if v is self.__pos_proxy: return
        self._set_pos(v)

    def follow(self, obj: Object):
        self.__native.follow(obj._get_native())

    @property
    def follow_offset(self):
        return self.__follow_offset_proxy

    @follow_offset.setter
    def follow_offset(self, v: Vec2Like):
        if v is self.__follow_offset_proxy: return
        self._set_follow_offset(v)

    @property
    def zoom(self):
        return self.__native.zoom_v

    @zoom.setter
    def zoom(self, v: float):
        self.__native.zoom(v)


class Scene:
    def __init__(self, native_scene: _NativeScene):
        self.__native = native_scene

    def get_native(self) -> _NativeScene:
        return self.__native

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

        native_obj = self.__native.spawn_object(*n_pos, *n_size, shader_source, has_texture, layer)
        return Object(native_obj, has_texture=has_texture)

    def spawn_text_object(self, pos: Vec2Like, text: str, scale: float = 12) -> TextObject:
        n_pos = ensure_vec2f("Scene.spawn_text_object(pos=...)", pos)

        native_obj = self.__native.spawn_text_object(*n_pos, text, scale)
        return TextObject(native_obj)

    @property
    def camera(self) -> Camera:
        return Camera(self.__native.get_camera())


class InputManager(_NativeInputManager):
    def __init__(self):
        super().__init__()

        self.key = _KeyAccessor()
        self.key_pressed = _KeyPressedAccessor()
        self.key_released = _KeyReleasedAccessor()

    @staticmethod
    def create_action(name: str, trigger: InputTrigger, *key: Key) -> None:
        _NativeInputManager.create_action(name, trigger, [*key])

    def __getitem__(self, action: str) -> bool:
        return _NativeInputManager.is_action_active(action)


class Application(_NativeApplication):
    def __init__(self, name: str, window_dimensions: Vec2Like, font_path: str = "@embed/Fonts/Base"):
        # We need to resolve the path of the script to allow for relative font import
        frame = inspect.stack()[1]
        called = Path(frame.filename).resolve().parent
        resolver = AssetResolver(called)

        super().__init__(name, resolver.resolve(font_path))
        n_window_dimensions = ensure_vec2i("Application(window_dimensions=...)", window_dimensions)
        self.__input = InputManager()
        super().init(*n_window_dimensions)

        self.__scene_cache = {}
        self.__active_scene = None

    @property
    def input(self) -> InputManager:
        return self.__input

    def make_new_scene(self) -> Scene:
        native_scene = super().make_new_scene()
        n_scene = Scene(native_scene)
        self.__scene_cache[id(native_scene)] = n_scene
        self.__active_scene = n_scene
        return n_scene

    @property
    def scene(self) -> Scene:
        native_scene = super().get_active_scene()
        key = id(native_scene)
        if key not in self.__scene_cache:
            self.__scene_cache[key] = Scene(native_scene)
        return self.__scene_cache[key]

    @scene.setter
    def scene(self, value: Scene):
        super().set_active_scene(value.get_native())
        self.__active_scene = value

    def make_new_ui(self) -> UIManager:
        native_ui = super().make_new_ui()
        return UIManager(native_ui)

    @property
    def now(self) -> float:
        return super().get_now()

    @property
    def dt(self) -> float:
        return super().get_delta_time()

    @property
    def background_color(self):
        raise AttributeError("Application.background_color is write-only")

    @background_color.setter
    def background_color(self, color: Color):
        r, g, b = ensure_color("Application.background_color= ...", color)
        super().set_clear_color(r / 255, g / 255, b / 255, 1)
