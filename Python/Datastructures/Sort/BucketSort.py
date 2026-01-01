def insertion_sort(arr):
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        while j >= 0 and key < arr[j]:
            arr[j + 1] = arr[j]
            j -= 1
        arr[j + 1] = key


def bucket_sort(A, k=None):
    if k is None:
        k = len(A)

    n = len(A)
    B = [[] for _ in range(k)]
    m = max(A) + 1

    for i in range(n):
        index = int(k * A[i] / m)
        B[index].append(A[i])

    for i in range(k):
        insertion_sort(B[i])

    result = []
    for i in range(k):
        result.extend(B[i])

    return result
