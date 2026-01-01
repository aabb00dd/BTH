def BinarySearchRecursive(A, X, low, high):
    if low > high:
        return -1

    mid = (low + high) // 2

    if X == A[mid]:
        return mid
    elif X < A[mid]:
        return BinarySearchRecursive(A, X, low, mid - 1)
    else:
        return BinarySearchRecursive(A, X, mid + 1, high)
