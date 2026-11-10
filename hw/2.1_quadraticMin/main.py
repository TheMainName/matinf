import ctypes
from ctypes import CDLL, c_int, c_char_p, c_float, byref, POINTER
import os
from expression import Expression

l =  CDLL("./equation.so")
l.parseEq.argtypes = [
    c_char_p,
    POINTER(c_int),
    POINTER(c_int),
    POINTER(c_int),
]
l.parseEq.restype = None

l.findMin.argtypes = [
    c_char_p,
    POINTER(c_float),
    POINTER(c_float),
    POINTER(c_int)
]
l.findMin.restype = None

def find_minimum(eq : str):
    min = c_float(0.0)
    value = c_float(0.0)
    failed = c_int(0)
    l.findMin(eq.encode(), byref(min), byref(value), byref(failed))
    if failed:
        return None,None
    return (min.value, value.value)


eq = input()
print(find_minimum(eq))
