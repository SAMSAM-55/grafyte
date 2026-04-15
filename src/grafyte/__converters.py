from typing import overload, TypeAlias
from __grafyte_internal import Vec2

Vec2f: TypeAlias = tuple[float, float]
Vec2i: TypeAlias = tuple[int, int]
Vec1Like: TypeAlias = int | float
Vec2Like: TypeAlias = Vec2 | Vec2f | Vec2i
Color: TypeAlias = tuple[int, int, int]
ColorNormalized: TypeAlias = tuple[float, float, float]


@overload
def __clamp_float(r: tuple[float, float], v1: float) -> tuple[float]: ...


@overload
def __clamp_float(r: tuple[float, float], v1: float, v2: float) -> tuple[float, float]: ...


@overload
def __clamp_float(r: tuple[float, float], v1: float, v2: float, v3: float) -> tuple[float, float, float]: ...


@overload
def __clamp_float(r: tuple[float, float], v1: float, *args: float) -> tuple[float, ...]: ...


def __clamp_float(r: tuple[float, float], v1: float, *args: float) -> tuple[float, ...]:
    lo, hi = r
    return tuple(float(min(max(x, lo), hi)) for x in (v1, *args))


@overload
def __clamp_int(r: tuple[int, int], v1: int) -> tuple[int]: ...


@overload
def __clamp_int(r: tuple[int, int], v1: int, v2: int) -> tuple[int, int]: ...


@overload
def __clamp_int(r: tuple[int, int], v1: int, v2: int, v3: int) -> tuple[int, int, int]: ...


@overload
def __clamp_int(r: tuple[int, int], v1: int, *args: int) -> tuple[int, ...]: ...


def __clamp_int(r: tuple[int, int], v1: int, *args: int) -> tuple[int, ...]:
    lo, hi = r
    return tuple(int(min(max(x, lo), hi)) for x in (v1, *args))


def ensure_vec2f(name: str, v: Vec2Like) -> Vec2f:
    """
    Validates that the input is either a Vec2 or a Vec2f (tuple[float, float]) or a Vec2i (tuple[int, int])
    and returns a Vec2f (tuple[float, float]).
    """
    if isinstance(v, Vec2):
        try:
            x = float(v.x)
            y = float(v.y)
            return x, y
        except (AttributeError, TypeError, ValueError):
            pass
    elif isinstance(v, tuple) and len(v) == 2:
        try:
            x = float(v[0])
            y = float(v[1])
            return x, y
        except (TypeError, ValueError):
            pass

    raise TypeError(f"{name} must be Vec2(x: float, y: float) or tuple[float, float] or tuple[int, int], got {v!r}")


def ensure_vec2i(name: str, v: Vec2Like) -> Vec2i:
    """
    Validates that the input is either a Vec2 or a Vec2f (tuple[float, float]) or a Vec2i (tuple[int, int])
    and returns a Vec2i (tuple[int, int]).
    """
    if isinstance(v, Vec2):
        try:
            x = int(v.x)
            y = int(v.y)
            return x, y
        except (AttributeError, TypeError, ValueError):
            pass
    elif isinstance(v, tuple) and len(v) == 2:
        try:
            x = int(v[0])
            y = int(v[1])
            return x, y
        except (TypeError, ValueError):
            pass

    raise TypeError(f"{name} must be Vec2(x: float, y: float) or tuple[float, float] or tuple[int, int], got {v!r}")


def ensure_color(name: str, v: Color) -> Color:
    try:
        r, g, b = v
    except Exception:
        raise TypeError(f"{name} must be Color = tuple[int, int, int], got {v!r}")

    if not all(isinstance(x, int) for x in (r, g, b)):
        raise TypeError(f"{name} must be Color = tuple[int, int, int], got {v!r}")

    return __clamp_int((0, 255), r, g, b)


def ensure_color_normalize(name: str, v: Color) -> ColorNormalized:
    v = ensure_color(name, v)
    return (
        v[0] / 255,
        v[1] / 255,
        v[2] / 255
    )
