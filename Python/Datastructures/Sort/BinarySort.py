def BinarySearch(A, low, high, key):
    while low <= high:
        mid = (low + high) // 2
        if key == A[mid]:
            return mid
        if key < A[mid]:
            high = mid - 1
        else:
            low = mid + 1
    return low


def BinarySort(A):
    for j in range(1, len(A)):
        key = A[j]
        i = j - 1
        index = BinarySearch(A, 0, i, key)
        while i >= index:
            A[i + 1] = A[i]
            i = i - 1
        A[i + 1] = key

