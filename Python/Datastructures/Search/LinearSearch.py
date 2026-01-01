def LinearSearch(A, X):
    i = 0
    while i < len(A):
        if X == A[i]:
            return i
        i += 1
    return -1
