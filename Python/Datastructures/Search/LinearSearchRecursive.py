def LinearSearchRecursive(A, X, index=0):
    if index >= len(A):
        return -1
    if A[index] == X:
        return index
    return LinearSearchRecursive(A, X, index + 1)
