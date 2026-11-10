def add_matrix(A: list[list[float]], B: list[list[float]]) -> list[list[float]]:
    """
    Takes two matrices and adds them together as such A + B. More on matrix addition on https://en.wikipedia.org/wiki/Matrix_addition
    :param A: list of lists of floats representing a matrix, sublist represents a row
    :param B: list of lists of floats representing a matrix, sublist represents a row
    :return: the result of addition of two matrices, list of lists of floats representing a matrix, sublist represents a row
    """
    return [
            [a+b for a, b in zip(rowA, rowB)] 
        for rowA, rowB in zip(A, B)]

def sub_matrix(A: list[list[float]], B: list[list[float]]) -> list[list[float]]:
    # you included "-" here the example in solve() description, which is why sub_matrix() exists.
    # it isnt in the test cases but I did not notice that 
    return [
            [a-b for a, b in zip(rowA, rowB)] 
        for rowA, rowB in zip(A, B)]


def multiply_matrix(A: list[list[float]], B: list[list[float]]) -> list[list[float]]:
    """
    takes two matrices and multiplies them together as such A @ B. More on matrix multiplication https://en.wikipedia.org/wiki/Matrix_multiplication
    :param A: list of lists of floats representing a matrix, sublist represents a row
    :param B: list of lists of floats representing a matrix, sublist represents a row
    :return: the result of multiplication of two matrices, list of lists of floats representing a matrix, sublist represents a row
    """
    # the unformatted version is even worse
    # return [[sum(A[row][i] * B[i][col] for i in range(len(A[0]))) for col in range(len(B[0]))] for row in range(len(A))]

    return [
        [
            sum(A[row_i][i] * B[i][col_i] for i in range(len(A[0])))
            for col_i in range(len(B[0]))
        ]
        for row_i in range(len(A))
    ]


def solve(equation: list[str | list[list[float]]]) -> list[list[float]]: 
    """
    takes an argument list of either matrices or strings ie [A, "+", B "@", C, ... "-", Z]
    where A, B, ..., Z are matrices represented as list of lists of floats.
    This function will than calculate the given input ie. A + B @ C ... - Z and will return the result
    :param equation: list of either strings representing a mathematical operation, or list of list of floats representing a matrix.
    :return: the result of solving the input equation, list of lists of floats representing a matrix, sublist represents a row
    """
    # I did this in C# a while ago, its kinda cool https://github.com/pointandlaugheveryone/AlgeBruh/blob/main/src/Models/Calculator.cs
    precedence = {"@": 2, "+": 1, "-": 1}
    q = []
    stack = []

    # convert to rpn
    for token in equation:
        if isinstance(token, list):
            q.append(token)
        else:
            # while there are tokens remaining, check for precedence
            while stack and precedence[stack[-1]] >= precedence[token]: # comparison between stack tokens and current token
                q.append(stack.pop())   
            stack.append(token)
    
    while(stack): # add operator after matrices
        q.append(stack.pop())
    
    # actually get result lol
    for t in q:
        if isinstance(t, list): # is an operator
            stack.append(t)
        else:
            b = stack.pop()
            a = stack.pop()
            if t == "+":
                stack.append(add_matrix(a,b))
            elif t =="-":
                stack.append(sub_matrix(a,b))
            elif t =="@":
                stack.append(multiply_matrix(a,b))

    return stack[0]


if __name__ == "__main__":
    A = [[1, 2, 3], [4, 5, 6]]
    B = [[7, 8, 9],[10, 11,12]]
    #A = [[1, 2, 3], [4, 5, 6]]
    #B = [[7, 8],[9],[11,12]]

    C = add_matrix(A, B)
    print(C)

    
