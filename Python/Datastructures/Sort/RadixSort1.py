def counting_sort(arr, B, k):
    C = [0] * (k + 1)
    n = len(arr)

    for j in range(n):
        C[arr[j]] += 1

    for i in range(1, k + 1):
        C[i] += C[i - 1]

    for j in range(n - 1, -1, -1):
        B[C[arr[j]] - 1] = arr[j]
        C[arr[j]] -= 1


def radix_sort(arr):
    max_element = max(arr)
    exp = 1
    while max_element // exp > 0:
        output = [0] * len(arr)
        counting_sort(arr, output, max_element)
        for i in range(len(arr)):
            arr[i] = output[i]
        exp *= 10

