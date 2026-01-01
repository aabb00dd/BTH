def Binarysearch(A, X):
    low = 0
    high = len(A) - 1
    while low <= high:
        mid = (low + high) // 2
        if X == A[mid]:
            return mid
        if X < A[mid]:
            high = mid - 1
        elif X > A[mid]:
            low = mid + 1
    return -1
