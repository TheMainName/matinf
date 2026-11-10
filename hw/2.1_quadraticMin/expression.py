
class Expression:
    def __init__(self, formula: str):
        self.formula = formula

    def equation(self, x: int, string=False):
        if string:
            return self.formula
        else:
            return eval(self.formula)