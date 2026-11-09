import ctypes
from ctypes import c_int, c_char_p, CDLL
import os

l =  CDLL("./equation.so")
l.parseEq.argtypes = [
    c_char_p,
    ctypes.POINTER(c_int), ctypes.POINTER(c_int), ctypes.POINTER(c_int)
]
l.parseEq.restype = None

equations = [
    "3 + x + x**2",
    "10 + (x − 4)**2",
    "2x**2 − 8x + 12"
]

a = c_int(0)
b = c_int(0)
c = c_int(0)

for eq in equations:
    l.parseEq(eq.encode(), ctypes.byref(a), ctypes.byref(b), ctypes.byref(c))
    print(f'a={a.value} b={b.value} c={c.value}')
    a = c_int(0)
    b = c_int(0)
    c = c_int(0)

