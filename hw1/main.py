import ctypes
import os
import expression as e


_LIB_NAME = "libequation.so"
_LIB_PATH = os.path.join(os.path.dirname(__file__), _LIB_NAME)
if not os.path.exists(_LIB_PATH):
    raise OSError(f"Shared library not found: {_LIB_PATH}")

_equation_lib = ctypes.CDLL(_LIB_PATH)

_equation_lib.parse_eq.argtypes = [ctypes.c_char_p, ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int), ctypes.POINTER(ctypes.c_int)]
_equation_lib.parse_eq.restype = None


def parse_eq(eq: str) -> dict:
    """Return the quadratic coefficients a, b, c for the provided expression."""
    a = ctypes.c_int()
    b = ctypes.c_int()
    c = ctypes.c_int()
    _equation_lib.parse_eq(eq.encode("utf-8"), ctypes.byref(a), ctypes.byref(b), ctypes.byref(c))
    return {"a": a.value, "b": b.value, "c": c.value}


if __name__ == "__main__":
    expr = e.Expression("3+2*x-x**2")
    print(expr.equation(4))
