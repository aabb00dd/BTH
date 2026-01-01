def SelectionSortRecursive(A, n):
    if n <= 1:
        return

    for i in range(n):
        min_index = i
        for j in range(i + 1, n):
            if A[j] < A[min_index]:
                min_index = j

        A[i], A[min_index] = A[min_index], A[i]

    SelectionSortRecursive(A, n - 1)


def SelectionSort2(A):
    SelectionSortRecursive(A, len(A))
