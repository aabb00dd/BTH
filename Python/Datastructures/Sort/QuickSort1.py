def Partition(A, start, end):
    x = A[end]
    i = start - 1
    for j in range(start, end):
        if A[j] <= x:
            i = i + 1
            A[i], A[j] = A[j], A[i]
    A[i + 1], A[end] = A[end], A[i + 1]
    return i + 1


def QuickSort(A, start, end):
    if start < end:
        pivot = Partition(A, start, end)
        QuickSort(A, start, pivot - 1)
        QuickSort(A, pivot + 1, end)

