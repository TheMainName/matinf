import numpy as np
import ctypes

class Planner:

    def __init__(self, grid: np.ndarray):
        """
        initialize the planner

        :param grid: 2D numpy array where 1 = obstacle, 0 = free space
        """
        self.grid = grid
        self.l = ctypes.CDLL("./astar.so")
        # noinspection PyDeprecation
        self.l.aStar.argtypes = [
            np.ctypeslib.ndpointer(dtype=np.int8, flags="C_CONTIGUOUS"),
            ctypes.c_int, ctypes.c_int,
            ctypes.c_int, ctypes.c_int,
            ctypes.c_int, ctypes.c_int,
            np.ctypeslib.ndpointer(dtype=np.int32, flags="C_CONTIGUOUS"),
            ctypes.POINTER(ctypes.c_int)
        ]
        self.l.aStar.restype = None


    def plan(self, start: np.ndarray, goal: np.ndarray)->np.ndarray:
        """
        will return the planned path from start to goal

        :param start: 1D numpy array of i, j position of start in the grid ie grid[i][j]
        :param goal: 1D numpy array of i, j position of goal in the grid ie grid[i][j]
        :return: 2D numpy array of i, j positions of planned path
        """
        grid = np.ascontiguousarray(self.grid).astype(np.int8,copy=False) # I am actually passing a pointer to grid start index here

        r, c = grid.shape
        sx, sy = int(start[0]), int(start[1])
        ex, ey = int(goal[0]), int(goal[1])

        out_path = np.empty(r*c*2, dtype=np.int32)
        out_len  = ctypes.c_int(0)

        self.l.aStar(
            grid,
            r, c, # grid array, rows, cols
            sx, sy, ex, ey, # start/end cords
            out_path, ctypes.byref(out_len)
        )

        return out_path[:out_len.value * 2].reshape(-1, 2)


