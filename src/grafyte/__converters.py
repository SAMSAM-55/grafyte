from typing import overload, TypeAlias

Vec2d: TypeAlias = tuple[float, float]
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

def ensure_vec2d(name: str, v: Vec2d) -> Vec2d:
    if (
            not isinstance(v, tuple) or len(v) != 2
            or not all(isinstance(x, (int, float)) for x in v)
    ):
        raise TypeError(f"{name} must be Vec2d = tuple[float, float], got {v!r}")
    return float(v[0]), float(v[1])

def ensure_color(name: str, v: Color) -> Color:
    if (
            not isinstance(v, tuple) or len(v) != 3
            or not all(isinstance(x, int) for x in v)
    ):
        raise TypeError(f"{name} must be Color = tuple[int, int, int], got {v!r}")
    return __clamp_int((0, 255), *v)

def ensure_color_normalize(name: str, v: Color) -> ColorNormalized:
    v = ensure_color(name, v)
    return (
        v[0] / 255,
        v[1] / 255,
        v[2] / 255
    )