import sys, importlib, inspect


def _imported_from_grafyte() -> bool:
    for frame_info in inspect.stack()[2:8]:
        modname = frame_info.frame.f_globals.get("__name__", "")
        if modname.startswith("grafyte"):
            return True
    return False

if not _imported_from_grafyte():
    raise ImportError("This module is internal to the grafyte library and is not meant to be a public API.")


_major, _minor = sys.version_info[:2]
_tag = f"{_major}{_minor}"

_name = {
    "310": "grafyte_py310",
    "311": "grafyte_py311",
    "312": "grafyte_py312",
    "313": "grafyte_py313",
    "314": "grafyte_py314"
}.get(_tag)

if _name is None:
    raise ImportError(f"Grafyte: unsupported Python {_major}.{_minor} (supported: 3.10–3.14)")

_mod = importlib.import_module("." + _name, __package__)

globals().update({k: getattr(_mod, k) for k in dir(_mod) if not k.startswith("_")})

__all__ = [k for k in globals().keys() if not k.startswith("_")]
__version__ = getattr(_mod, "__version__", "0.0.0")
