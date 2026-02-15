import sys, importlib

_major, _minor = sys.version_info[:2]
_tag = f"{_major}{_minor}"

print(_tag)
print("hi")

_name = {
    "310": "grafyte_py310",
    "311": "grafyte_py311",
    "312": "grafyte_py312",
    "313": "grafyte_py313",
}.get(_tag)

if _name is None:
    raise ImportError(f"Grafyte: unsupported Python {_major}.{_minor} (supported: 3.10–3.13)")

_mod = importlib.import_module("." + _name, __package__)

globals().update({k: getattr(_mod, k) for k in dir(_mod) if not k.startswith("_")})

__all__ = [k for k in globals().keys() if not k.startswith("_")]
__version__ = getattr(_mod, "__version__", "0.0.0")
