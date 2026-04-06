from __grafyte_internal import InputManager as _NativeInputManager, Key
from .__converters import *


class Vec2Proxy:
    def __init__(self, getter, setter):
        self._getter = getter
        self._setter = setter

    def _get(self):
        return self._getter()

    def _set(self, v):
        self._setter(v)

    @property
    def x(self):
        return self._get().x

    @x.setter
    def x(self, value):
        v = self._get()
        self._set((value, v.y))

    @property
    def y(self):
        return self._get().y

    @y.setter
    def y(self, value):
        v = self._get()
        self._set((v.x, value))

    def __iadd__(self, other: Vec2Like):
        ox, oy = ensure_vec2f("Vec2Proxy.__iadd__(other=...)", other)
        v = self._get()
        self._set((v.x + ox, v.y + oy))
        return self

    def __isub__(self, other: Vec2Like):
        ox, oy = ensure_vec2f("Vec2Proxy.__iadd__(other=...)", other)
        v = self._get()
        self._set((v.x - ox, v.y - oy))
        return self
    def __imul__(self, other: float):
        v = self._get()
        self._set((v.x * other, v.y * other))
        return self

    def __iter__(self):
        v = self._get()
        yield v.x
        yield v.y

    def __repr__(self):
        v = self._get()
        return f"Vec2Proxy({v.x}, {v.y})"

class RotProxy:
    def __init__(self, getter, setter):
        self._getter = getter
        self._setter = setter

    def _get(self):
        return float(self._getter())

    def _set(self, value):
        self._setter(float(value))

    @property
    def value(self):
        return self._get()

    @value.setter
    def value(self, value):
        self._set(value)

    def set(self, value):
        self._set(value)
        return self

    def __float__(self):
        return self._get()

    def __int__(self):
        return int(self._get())

    def __iadd__(self, other):
        self._set(self._get() + float(other))
        return self

    def __isub__(self, other):
        self._set(self._get() - float(other))
        return self

    def __imul__(self, other):
        self._set(self._get() * float(other))
        return self

    def __itruediv__(self, other):
        self._set(self._get() / float(other))
        return self

    def __repr__(self):
        return f"RotProxy({self._get()})"

class _KeyAccessor:
    def __getitem__(self, key: Key) -> bool:
        return _NativeInputManager.is_key_down(key)

class _KeyPressedAccessor:
    def __getitem__(self, key: Key) -> bool:
        return _NativeInputManager.was_key_pressed(key)

class _KeyReleasedAccessor:
    def __getitem__(self, key: Key) -> bool:
        return _NativeInputManager.was_key_released(key)