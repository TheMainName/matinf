import ctypes
from ctypes import CDLL, c_int, c_char_p, c_float
import os

l =  CDLL("./equation.so")
l.parseEq.argtypes = [
    c_char_p,
    ctypes.POINTER(c_int),
    ctypes.POINTER(c_int),
    ctypes.POINTER(c_int),
]
l.parseEq.restype = None
l.findMin.argtypes = [
    c_char_p,
    ctypes.POINTER(c_float),
    ctypes.POINTER(c_int)
]
l.findMin.restype = None

equations = [
    "3+x-x**2",
    "3 + x + x**2",
    "10 + (x − 4)**2",
    "2x**2 − 8x + 12",
]

failed = c_int(0)

for eq in equations:
    min = c_float(1.0)
    l.findMin(eq.encode(),ctypes.byref(min), ctypes.byref(failed))
    if failed.value == 1:
        print("None")
        continue

    print(min.value)
    failed = c_int(0)


