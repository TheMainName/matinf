import ctypes
# ctypes should be valid as a "basic python module"
lib = ctypes.CDLL("./rsort.so")
lib.find_min.argtypes = [
    ctypes.POINTER(ctypes.c_int),
    ctypes.c_int
]
lib.find_min.restype = ctypes.c_int
lib.find_max.argtypes = [
    ctypes.POINTER(ctypes.c_int),
    ctypes.c_int
]
lib.find_max.restype = ctypes.c_int
lib.radix_bin_sort.argtypes = [
    ctypes.POINTER(ctypes.c_int),
    ctypes.c_int
]
lib.radix_bin_sort.restype = None


def find_minimum(numbers) -> int:    # removed type annotations because ctypes wouldn't shut up about unexpected type even though it works
    n = len(numbers)
    ArrayType = ctypes.c_int * n
    c_num = ArrayType(*numbers)
    minimum = lib.find_min(c_num,n)
    return minimum


def find_maximum(numbers) -> int:
    n = len(numbers)
    ArrayType = ctypes.c_int * n
    c_num = ArrayType(*numbers)
    maximum = lib.find_max(c_num,n)
    return maximum

def sort_numbers(numbers) -> list[int]:
    n = len(numbers)
    ArrayType = ctypes.c_int * n
    c_num = ArrayType(*numbers)
    lib.radix_bin_sort(c_num,n)
    return list(c_num)[:n]


if __name__ == "__main__":
    num = [1, 2, 12, 4, 32, 6, 75, 68, 9, 110]
    N = len(num)

    print(sort_numbers(num))
