def Merge(arr, left, middle, right):
    temp = [0] * len(arr)
    i = left
    j = middle + 1
    k = left

    while i <= middle and j <= right:
        if arr[i] <= arr[j]:
            temp[k] = arr[i]
            i += 1
        else:
            temp[k] = arr[j]
            j += 1
        k += 1

    while i <= middle:
        temp[k] = arr[i]
        i += 1
        k += 1

    while j <= right:
        temp[k] = arr[j]
        j += 1
        k += 1

    for i in range(left, right + 1):
        arr[i] = temp[i]


def MergeSortBottomUp(arr):
    n = len(arr)
    curr_size = 1

    while curr_size < n:
        left = 0

        while left < n - 1:
            middle = min(left + curr_size - 1, n - 1)
            right = min(left + 2 * curr_size - 1, n - 1)
            Merge(arr, left, middle, right)
            left = left + 2 * curr_size

        curr_size *= 2
