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

class ColorProxy:
    def __init__(self, getter, setter, checker=None, name: str = "color"):
        self._getter = getter
        self._setter = setter
        self._checker = checker
        self._name = name

    def _get(self):
        if self._checker is not None:
            self._checker(self._name)
        return self._getter()

    def _set(self, v):
        if self._checker is not None:
            self._checker(self._name)
        self._setter(v)

    @property
    def r(self):
        return self._get()[0]

    @r.setter
    def r(self, value):
        v = self._get()
        self._set((value, v[1], v[2]))

    @property
    def g(self):
        return self._get()[1]

    @g.setter
    def g(self, value):
        v = self._get()
        self._set((v[0], value, v[2]))

    @property
    def b(self):
        return self._get()[2]

    @b.setter
    def b(self, value):
        v = self._get()
        self._set((v[0], v[1], value))

    @property
    def a(self):
        v = self._get()
        return v[3]

    @a.setter
    def a(self, value):
        v = self._get()
        self._set((v[0], v[1], v[2], value))

    def __iter__(self):
        v = self._get()
        yield v[0]
        yield v[1]
        yield v[2]
        yield v[3]

    def __repr__(self):
        v = self._get()
        return f"ColorProxy({v[0]}, {v[1]}, {v[2]}, {v[3]})"

class TintProxy(ColorProxy):
    @property
    def strength(self):
        v = self._get()
        return v[3]

    @strength.setter
    def strength(self, value):
        v = self._get()
        self._set((v[0], v[1], v[2], value))

    @property
    def a(self):
        return self.strength

    @a.setter
    def a(self, value):
        self.strength = value

    def __repr__(self):
        v = self._get()
        return f"TintProxy({v[0]}, {v[1]}, {v[2]}, {v[3]})"

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
